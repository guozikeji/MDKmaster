

#include "bsp_crc.h"
#include "bsp_flash.h"
#include "bsp_rtc_time.h"
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** @defgroup Output_Private_Variables
  * @{
  */

#define 	BSP_CRC_LENGTH_MIN 12
#define 	BSP_CRC_READ_DATA_LENGTH_MAX  254   //512-4-5  	缓冲区长度 文件头长度  MODBUS	发送数据包基本长度5+字节数
extern struct datetime SETDATE;  
extern struct datetime REALDATE;

void BspCrcProcess(u8 *array,u16 *lengthPoint,u8 *control)
{
		u16 crc;
		switch(BspCrcCheckProcess(array,lengthPoint,control))
		{
				//功能码未识别
				case 0X01:							
							array[5]=array[5]|0X80;
							array[6]=1;
							CRC16_Array(array,4,3,&crc);
							array[7]=((crc&0xff00)>>8);
							array[8]=crc&0x00ff;
							*lengthPoint=9;
							*control=0;
							break;
				//地址错误
				case 0X02:
							array[5]=array[5]|0X80;
							array[6]=2;
							CRC16_Array(array,4,3,&crc);
							array[7]=((crc&0xff00)>>8);
							array[8]=crc&0x00ff;
							*lengthPoint=9;
							*control=0;
							break;
				//参数错误
				case 0X03:
							array[5]=array[5]|0X80;
							array[6]=3;
							CRC16_Array(array,4,3,&crc);
							array[7]=((crc&0xff00)>>8);
							array[8]=crc&0x00ff;
							*lengthPoint=9;
							*control=0;
							break;
				//CRC校验错误
				case 0X04:
							array[5]=array[5]|0X80;
							array[6]=4;
							CRC16_Array(array,4,3,&crc);
							array[7]=((crc&0xff00)>>8);
							array[8]=crc&0x00ff;
							*lengthPoint=9;
							*control=0;
							break;
				default:  break;
		}
}
u8 BspCrcCheckProcess(u8 *array,u16 *lengthPoint,u8 *control)
{
		u8 err=0;
		u16 crc,addr,length;
		length=*lengthPoint;
		*control=0;
		//检查数据包  参数错误
		if(length<BSP_CRC_LENGTH_MIN)return err=3;
		//地址错误
		addr=array[6];
		addr=(addr<<8)+array[7];	
		if(addr>999)return err=2;
	
		//读写字节长度                      
		length=array[8];
		length=((length<<8)+array[9]);
		if(((addr+length)>999)||(length>(BSP_CRC_READ_DATA_LENGTH_MAX/2)))return err=2;
		length=length<<1;
		//功能码检查 CRC
		switch(array[5])
		{
			case 0X03: 
								CRC16_Array(array,4,6,&crc);
								if((((crc&0xff00)>>8)!=array[10])||((crc&0x00ff)!=array[11]))err=4;
								else BspCrcReadProcess( array,addr,length,lengthPoint);
								break;
			case 0X10: CRC16_Array(array,4,(length+7),&crc);
								if((((crc&0xff00)>>8)!=array[length+11])&&((crc&0x00ff)==array[length+12])){err=4;}
								else if(BspCrcWriteProcess( array,addr,length,lengthPoint,control)){err=3;}
			          break;
			default: err=1; break;	
		}	
	return err;
}
//control =1 写入flash 2重启
u8 BspCrcWriteProcess(u8 *array,u16 addr,u16 length, u16 *lengthPoint,u8 *control)
{
	u16 i,crc;
	u8 err=0;
	switch(addr)
	{
		case 990:
					if((array[11]<100)&&(array[12]<13)&&(array[13]<32)&&(array[14]<24)&&(array[15]<60)&&(array[16]<60))
					{
						SETDATE.year=2000+array[11];SETDATE.month=array[12];SETDATE.day=array[13];
						SETDATE.hour=array[14];SETDATE.minit=array[15];SETDATE.second=array[16];
						BspRtcTimeSetUnixTime();
					}else {err=3;return err;}
					break;
		case 993:if((array[11]==0)&&(array[12]==1))*control=1;break;
		case 994:if((array[11]==0)&&(array[12]==1))*control=2;break;
		default:
						addr=addr<<1;
						//STM32 存储高字节在前 0x1234   34 12
//						for(i=0;i<length;i++)(*(__IO uint8_t*)(RamStartAddr+addr+i))=array[11+i];
							for(i=0;i<length;i++)
							{
								if(i&0x0001)(*(__IO uint8_t*)(RamStartAddr+addr+i))=array[11+i-1];
								else(*(__IO uint8_t*)(RamStartAddr+addr+i))=array[11+i+1];
							}
						break;		
	}	
	//数据包头 array[0]=0x5a;array[1]=0x03; 不变
	//MODBUS数据头 array[4]=0x01; 地址不变 array[5]=0x10;功能码\写地址\寄存器个数不变 
	array[2]=0;
	array[3]=8 ;
	CRC16_Array(array,4,6,&crc);	
	array[10]=((crc&0xff00)>>8);
	array[11]=crc&0x00ff;	
	*lengthPoint=12;
	return err;
}
void BspCrcReadProcess(u8 *array,u16 addr,u16 length, u16 *lengthPoint)
{
	u16 i,crc;
	//数据包头 array[0]=0x5a;array[1]=0x03; 不变
	array[2]=((length+5)&0xff00)>>8;
	array[3]=(length+5)&0x00ff ;
	//MODBUS数据头 array[4]=0x01; 地址不变 array[5]=0x03;功能码不变
	array[6]=length;
	i=0;
	switch(addr)
	{
		case 990:
						if(i<length){array[7+i]=REALDATE.year-2000;i++;}	
						if(i<length){array[7+i]=REALDATE.month;i++;}	
						if(i<length){array[7+i]=REALDATE.day;i++;}
						if(i<length){array[7+i]=REALDATE.hour;i++;}	
						if(i<length){array[7+i]=REALDATE.minit;i++;}	
						if(i<length){array[7+i]=REALDATE.second;i++;}
						break;
		default:break;		
	}	
	addr=addr<<1;
	//STM32 存储高字节在前 0x1234   34 12
	for(;i<length;i++)
	{
		if(i&0x0001)array[7+i]=(*(__IO uint8_t*)(RamStartAddr+addr+i-1));
		else array[7+i]=(*(__IO uint8_t*)(RamStartAddr+addr+i+1));
	}
	CRC16_Array(array,4,(length+3),&crc);
	array[length+7]=((crc&0xff00)>>8);
	array[length+8]=crc&0x00ff;
	*lengthPoint=length+7+2;
}
//u16 CRC16(u8 *array, u16 Length)
//{
//	u16  ix,iy,crc=0xffff;
//	
//	
//	for (ix=0;ix<Length;ix++)
//	{
//		crc=crc^(u16)(array[ix]);
//		crc=crc^(u16)(Array[ix]);
//		for(iy=0;iy<=7;iy++)
//		{
//			if ((crc&1)!=0 )
//			crc=(crc>>1)^0xA001;
//			else
//			crc=crc>>1;    //
//		}
//	}
//    return crc;
//}
void CRC16_Array(u8 *array, u8 start_length,u16 length,u16 *crcPoint)
{
	u16  ix,iy,crc=0xffff;


	for (ix=start_length;ix<start_length+length;ix++)
	{
		crc=crc^(u16)(array[ix]);
		for(iy=0;iy<=7;iy++)
		{
			if ((crc&1)!=0 )
			crc=(crc>>1)^0xA001;
			else
			crc=crc>>1;   
		}
	}
    *crcPoint=crc;
}

