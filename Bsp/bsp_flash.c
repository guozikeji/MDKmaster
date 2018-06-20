
/* Includes ------------------------------------------------------------------*/
#define BSP_FLASH_GLOBALS

#include "bsp_flash.h"
  /* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//STM32F103VCT6


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/  
//��ȡFLASH����-�ɹ�����0
u8 BspFlashInit(void)
{
	u8 i,j;
	BspFlashRead();
	for(j=0;j<3;j++)
	{
		if(FlashInitFlag!=(*(__IO uint16_t*) (RamEndAddr-2)))
		{
			BspRawInit();
			for(i=0;i<3;i++)if(BspFlashWrite()==0)break;
			if(i>=3)return 1;
		}else break;
	}
	if(j>=3)return 1;
	return 0;
}
//��ʼ��ʱд���������
void BspRawInit(void)
{
		//��ʼ�����ݱ�־
		(*(__IO uint16_t*) (RamEndAddr-2))=FlashInitFlag;
	  //�����Ǳ�
		(*(__IO uint16_t*)RamStartAddr        )=0X0001;			//��1����	
		(*(__IO uint16_t*) (RamStartAddr+  2 ))=0x6104;			//��� 10020461
		(*(__IO uint16_t*) (RamStartAddr+  4 ))=0x0210;			//���
		(*(__IO uint16_t*) (RamStartAddr+  6 ))=0x0011;			//��Ÿ����� 00 11 11
		(*(__IO uint16_t*) (RamStartAddr+  8 ))=0x1100;			//��Ÿ����� 00 11 11
		(*(__IO uint16_t*) (RamStartAddr+ 10 ))=0xA001;			//��Ԫ��
		(*(__IO uint16_t*) (RamStartAddr+ 12 ))=0xF001;			//�����
		(*(__IO uint16_t*) (RamStartAddr+ 14 ))=0x1000;			//���ڱ��
		(*(__IO uint16_t*) (RamStartAddr+ 16 ))=0x0001;			//���ڱ��
		(*(__IO uint16_t*) (RamStartAddr+ 18 ))=2000;			  //�趨�¶�20.00��
		(*(__IO uint16_t*) (RamStartAddr+ 20 ))=0x2000;			//�����
		(*(__IO uint16_t*) (RamStartAddr+ 22 ))=0x0001;			//�����
		(*(__IO uint16_t*) (RamStartAddr+ 24 ))=1000;			  //������100.0%
		(*(__IO uint16_t*) (RamStartAddr+ 26 ))=1;			    //������ģʽ
	  //�����Ǳ�
		(*(__IO uint16_t*) (RamStartAddr+ 28 ))=0X0002;			//��2����	
		(*(__IO uint16_t*) (RamStartAddr+ 30 ))=0x0147;			//��� 01470759
		(*(__IO uint16_t*) (RamStartAddr+ 32 ))=0x0759;			//���
		(*(__IO uint16_t*) (RamStartAddr+ 34 ))=0x0011;			//��Ÿ����� 00 11 11
		(*(__IO uint16_t*) (RamStartAddr+ 36 ))=0x1100;			//��Ÿ����� 00 11 11
		(*(__IO uint16_t*) (RamStartAddr+ 38 ))=0xA002;			//��Ԫ��
		(*(__IO uint16_t*) (RamStartAddr+ 40 ))=0xF002;			//�����
		(*(__IO uint16_t*) (RamStartAddr+ 42 ))=0x1000;			//���ڱ��
		(*(__IO uint16_t*) (RamStartAddr+ 44 ))=0x0002;			//���ڱ��
		(*(__IO uint16_t*) (RamStartAddr+ 46 ))=2000;			  //�趨�¶�20.00��
		(*(__IO uint16_t*) (RamStartAddr+ 48 ))=0x2000;			//�����
		(*(__IO uint16_t*) (RamStartAddr+ 50 ))=0x0002;			//�����
		(*(__IO uint16_t*) (RamStartAddr+ 52 ))=1000;			  //������100.0%
		(*(__IO uint16_t*) (RamStartAddr+ 54 ))=1;			    //������ģʽ
	  //LANDIS 2WR6
		(*(__IO uint16_t*) (RamStartAddr+ 28 ))=0X0040;			//�� 0X40 LANDIS 2WER6
		(*(__IO uint16_t*) (RamStartAddr+ 30 ))=0x9630;			//��� 66073096
		(*(__IO uint16_t*) (RamStartAddr+ 32 ))=0x0766;			//���
		(*(__IO uint16_t*) (RamStartAddr+ 34 ))=0xA732;			//��Ÿ����� A7 32 03 04
		(*(__IO uint16_t*) (RamStartAddr+ 36 ))=0x0304;			//��Ÿ����� A7 32 03 04
		(*(__IO uint16_t*) (RamStartAddr+ 38 ))=0xA003;			//��Ԫ��
		(*(__IO uint16_t*) (RamStartAddr+ 40 ))=0xF003;			//�����
		(*(__IO uint16_t*) (RamStartAddr+ 42 ))=0x1000;			//���ڱ��
		(*(__IO uint16_t*) (RamStartAddr+ 44 ))=0x0003;			//���ڱ��
		(*(__IO uint16_t*) (RamStartAddr+ 46 ))=2000;			  //�趨�¶�20.00��
		(*(__IO uint16_t*) (RamStartAddr+ 48 ))=0x2000;			//�����
		(*(__IO uint16_t*) (RamStartAddr+ 50 ))=0x0003;			//�����
		(*(__IO uint16_t*) (RamStartAddr+ 52 ))=1000;			  //������100.0%
		(*(__IO uint16_t*) (RamStartAddr+ 54 ))=1;			    //������ģʽ
	
		(*(__IO uint16_t*) RamBulidingAddr     )=0X3132;   //¥�Ÿ�	  840 ASCII 1234
		(*(__IO uint16_t*) (RamBulidingAddr+2) )=0X3334;   //¥�Ÿ�	  840 ASCII
	
		(*(__IO uint16_t*) RamMeterAddr        )=0;   //�ȱ����� 842
		(*(__IO uint16_t*) RamUserAddr         )=1;   //�ȱ����� 843
		(*(__IO uint16_t*) RamCircuitChangeAddr)=30;   //��·�л�ʱ�� 30S
		(*(__IO uint16_t*) RamRunModeAddr      )=60;   //����ģʽ 
		(*(__IO uint16_t*) RamMinValveAddr     )=100;   // ��������10%
		(*(__IO uint16_t*) RamMinTempRoomAddr  )=1600;   // �����¶�16��
		(*(__IO uint16_t*) RamIntegralTimeAddr )=800;   // ����ʱ��  800=100S

		(*(__IO uint16_t*)  RamMbusStartAddr    )=0X0B0A; 	   //MBUS ��ʼ��   (11.10-3.20)
		(*(__IO uint16_t*)  RamMbusEndAddr			)=0X0314;    //MBUS ��ʼ��    (11.10-3.20)
		(*(__IO uint16_t*)  RamTimeSection1Addr	)=0X0100; 	   //�ϴ�ʱ���1 Сʱ  ����
		(*(__IO uint16_t*)  RamTimeSection2Addr	)=0X0400; 	   //�ϴ�ʱ���2 Сʱ  ����
		(*(__IO uint16_t*)  RamTimeSection3Addr	)=0X0700; 	   //�ϴ�ʱ���3 Сʱ  ����
		(*(__IO uint16_t*)  RamTimeSection4Addr	)=0X0A00; 	   // �ϴ�ʱ���4 Сʱ  ����
		(*(__IO uint16_t*)  RamTimeSection5Addr	)=0X0D00; 	   // �ϴ�ʱ���5 Сʱ  ����
		(*(__IO uint16_t*)  RamTimeSection6Addr	)=0X1000; 	   // �ϴ�ʱ���6 Сʱ  ����
		(*(__IO uint16_t*)  RamTimeSection7Addr	)=0X1300;    // �ϴ�ʱ���7 Сʱ  ����
		(*(__IO uint16_t*)  RamTimeSection8Addr	)=0X1600; 	   // �ϴ�ʱ���8 Сʱ  ����

		(*(__IO uint16_t*)  RamMbusSubpackageTimeAddr )=100; 	   //MBUS �ְ�ʱ�� ��λms
		(*(__IO uint16_t*)  RamMbusBaudRateAddr		    )=2400;    //MBUS ������
		(*(__IO uint16_t*)  RamMbusDateBitsAddr	      )=0X1000;   //MBUS ����λ ����8-0x0000 ��9-0x1000
		(*(__IO uint16_t*)  RamMbusStopBitsAddr	      )=0;    //1ֹͣλ0X0000 0.5 ֹͣλ 0X1000 2ֹͣλ 0X2000 1.5ֹͣλ0X3000
		(*(__IO uint16_t*)  RamMbusParityBitsAddr     )=0X0400; 	   //0X0000��У�� 0X0600��У��  0X0400żУ��
		(*(__IO uint16_t*)  RamMbusRestartTimeAddr	  )=5000;    // MBUS ����ʱ�� 5s   
		(*(__IO uint16_t*)  RamMbusCircuitTimeAddr	  )=3000; 	   // MBUS ѭ��ʱ�� 3S
		(*(__IO uint16_t*)  RamMbusErrorCountAddr		  )=4;    // MBUS ��ȡ�������	4

		(*(__IO uint16_t*)  RamLcdControlAddr           )=0; 	   //0���ؿ��ƣ�1Զ�̿���
		(*(__IO uint16_t*)  RamLcdRefreshTimeAddr		    )=5;    //LCDˢ��ʱ�� 	5
		(*(__IO uint16_t*)  RamLcdStartTimeAddr	        )=0; 	   // LCD��ʾ��ʼʱ�� 7:30��ʼ��ʾ
		(*(__IO uint16_t*)  RamLcdEndTimeAddr	          )=0x181e;; 	   // LCD��ʾ����ʱ�� 18:30������ʾ
		(*(__IO uint16_t*)  RamLcdHeadFrontColorAddr    )=0xf7de;    //LCDͷ�ļ�ǰ��ɫ ��ɫ
		(*(__IO uint16_t*)  RamLcdHeadBackColorAddr	    )=0xF800; 	   //LCDͷ�ļ�����ɫ ��ɫ
		(*(__IO uint16_t*)  RamLcdContentFrontColorAddr	)=0xf7de;    //LCD����ǰ��ɫ ��ɫ
		(*(__IO uint16_t*)  RamLcdContentBackColorAddr  )=0xF81F; 	   //LCD���ݱ���ɫ	���
		(*(__IO uint16_t*)  RamLcdHighLightColorAddr	  )=0x051F;    //LCD������ʾɫ	��ɫ
		(*(__IO uint16_t*)  RamLcdBrightnessAddr        )=800;    //LCD���� ���� 0�� 1000���

		(*(__IO uint16_t*)  RamRs485SubpackageTimeAddr )=10;    //RS485�ְ�ʱ��
		(*(__IO uint16_t*)  RamRs485BaudRateAddr		   )=9600;    // ������
		(*(__IO uint16_t*)  RamRs485DateBitsAddr	     )=0;    // ����λ ����8-0x0000 ��9-0x1000
		(*(__IO uint16_t*)  RamRs485StopBitsAddr	     )=0;    //1ֹͣλ0X0000 0.5 ֹͣλ 0X1000 2ֹͣλ 0X2000 1.5ֹͣλ0X3000
		(*(__IO uint16_t*)  RamRs485ParityBitsAddr     )=0;    //0X0000��У�� 0X0600��У��  0X0400żУ��

		(*(__IO uint16_t*)  RamGprsSubpackageTimeAddr  )=10;   //GPRS�ְ�ʱ��
		(*(__IO uint16_t*)  RamGprsBaudRateAddr		     )=9600;    // ������
		(*(__IO uint16_t*)  RamGprsDateBitsAddr	       )=0; 	   // ����λ ����8-0x0000 ��9-0x1000
		(*(__IO uint16_t*)  RamGprsStopBitsAddr	       )=0; 	   //1ֹͣλ0X0000 0.5 ֹͣλ 0X1000 2ֹͣλ 0X2000 1.5ֹͣλ0X3000
		(*(__IO uint16_t*)  RamGprsParityBitsAddr      )=0; 	   //0X0000��У�� 0X0600��У��  0X0400żУ��

		(*(__IO uint16_t*)  RamGprsDscModeAddr         )=1; 	   //GPRS 2DSC3  1�Զ��� ͨѶЭ��
		(*(__IO uint16_t*)  RamGprsLinkTimeAddr		     )=10; 	   // GPRS�����ʱ��
		(*(__IO uint16_t*)  RamGprsLinkStopAddr	       )=2;    // ��ʱ��û������������ ֹͣ�೤ʱ�� ����������
		(*(__IO uint16_t*)  RamGprsNoneReceiveTimeAddr )=1440;    //�����������೤ʱ��?û�н��ܵ�����,��ʼ�������� ����
		(*(__IO uint16_t*)  RamGprsHeartTimeAddr       )=3000; 	   // �������������ʱ�� 	S
		(*(__IO uint16_t*)  RamGprsUpdataTimeAddr      )=4000; 	   // ������ʱ�ϴ�����ʱ����

		(*(__IO uint16_t*)  RamGprsDns1DataLengthAddr  )=13;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+ 2))=0X3230;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+ 4))=0X322E;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+ 6))=0X3130;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+ 8))=0X322E;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+10))=0X3135;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+12))=0X342E;    //202.102.154.3 firstDns
		(*(__IO uint16_t*)  (RamGprsDns1DataLengthAddr+14))=0X3300;    //202.102.154.3 firstDns
		
		(*(__IO uint16_t*)  RamGprsDns2DataLengthAddr  )=13;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+ 2))=0X3230;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+ 4))=0X322E;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+ 6))=0X3130;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+ 8))=0X322E;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+10))=0X3135;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+12))=0X322E;    //202.102.152.3 secondaryDns
		(*(__IO uint16_t*)  (RamGprsDns2DataLengthAddr+14))=0X3300;    //202.102.152.3 secondaryDns
		
		(*(__IO uint16_t*)  RamGprsHeartDataLengthAddr     )=5;    //GPRS GPRS �������ַ��� 5A 05 00 01 FE
		(*(__IO uint16_t*)  (RamGprsHeartDataLengthAddr+ 2))=0X5A05;    //GPRS GPRS �������ַ���
		(*(__IO uint16_t*)  (RamGprsHeartDataLengthAddr+ 6))=0X0001;    //GPRS GPRS �������ַ���
		(*(__IO uint16_t*)  (RamGprsHeartDataLengthAddr+ 6))=0XFE00;    //GPRS GPRS �������ַ���

		(*(__IO uint16_t*)  RamGprsSmidTelAddr         )=0X3133; 	   //GPRS SMID �ֻ���
		(*(__IO uint16_t*)  (RamGprsSmidTelAddr+ 2)    )=0X3332; 	   //GPRS SMID �ֻ���
		(*(__IO uint16_t*)  (RamGprsSmidTelAddr+ 4)    )=0X3633;  	   //GPRS SMID �ֻ���
		(*(__IO uint16_t*)  (RamGprsSmidTelAddr+ 6)    )=0X3039;  	   //GPRS SMID �ֻ���
		(*(__IO uint16_t*)  (RamGprsSmidTelAddr+ 8)    )=0X3337;  	   //GPRS SMID �ֻ���
		(*(__IO uint16_t*)  (RamGprsSmidTelAddr+10)    )=0X3900;  	   //GPRS SMID �ֻ���
		
		(*(__IO uint16_t*)  RamGprsAdmitTelNumberAddr  )=0X3133; 	   //��Ȩ�ֻ��� 13326309379	
		(*(__IO uint16_t*)  (RamGprsAdmitTelNumberAddr+ 2))=0X3332; 	   //��Ȩ�ֻ��� 13326309379	
		(*(__IO uint16_t*)  (RamGprsAdmitTelNumberAddr+ 4))=0X3633; 	   //��Ȩ�ֻ��� 13326309379	
		(*(__IO uint16_t*)  (RamGprsAdmitTelNumberAddr+ 6))=0X3039; 	   //��Ȩ�ֻ��� 13326309379	
		(*(__IO uint16_t*)  (RamGprsAdmitTelNumberAddr+ 8))=0X3337; 	   //��Ȩ�ֻ��� 13326309379	
		(*(__IO uint16_t*)  (RamGprsAdmitTelNumberAddr+10))=0X3900; 	   //��Ȩ�ֻ��� 13326309379	
		
		(*(__IO uint16_t*)  RamGprsSmidNumberAddr      )=0X3000;    //GPRS SMID	GPRS_SMID
    (*(__IO uint16_t*)  (RamGprsSmidNumberAddr +2) )=0X0001;    //GPRS SMID	GPRS_SMID
		
		(*(__IO uint16_t*)  RamGprsPortLengthAddr      )=4;    //GPRS IP�˿ں����ݳ���
		(*(__IO uint16_t*)  (RamGprsPortLengthAddr+ 2 ))=0X3737;    //GPRS IP�˿ں����ݳ���
		(*(__IO uint16_t*)  (RamGprsPortLengthAddr+ 4 ))=0X3737;    //GPRS IP�˿ں����ݳ���
		//����� APN jyzhrl.sdapn
		(*(__IO uint16_t*)  RamGprsApnLengthAddr       )=5; 	   //GPRS  �����  CMNET UNINET
		(*(__IO uint16_t*)  (RamGprsApnLengthAddr+ 2  ))=0X434D; 	   //GPRS  �����  CMNET UNINET
		(*(__IO uint16_t*)  (RamGprsApnLengthAddr+ 4  ))=0X4E45; 	   //GPRS  �����  CMNET UNINET
		(*(__IO uint16_t*)  (RamGprsApnLengthAddr+ 6  ))=0X5400; 	   //GPRS  �����  CMNET UNINET
		
		(*(__IO uint16_t*)  RamGprsHttpLengthAddr      )=15; 	   //���� www.20cdata.com
		//WWW.
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+ 2 ))=0X7777; 	   //GPRS  �����  CMNET UNINET 
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+ 4 ))=0X772E; 	   //GPRS  �����  CMNET UNINET 
		//20CDATA.
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+ 6 ))=0X3230; 	   //GPRS  �����  CMNET UNINET 
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+ 8 ))=0X6364; 	   //GPRS  �����  CMNET UNINET 
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+10 ))=0X6174; 	   //GPRS  �����  CMNET UNINET 
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+12 ))=0X612E; 	   //GPRS  �����  CMNET UNINET 	
		//COM
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+14 ))=0X636F; 	   //GPRS  �����  CMNET UNINET 
		(*(__IO uint16_t*)  (RamGprsHttpLengthAddr+16 ))=0X6DFF; 	   //GPRS  �����  CMNET UNINET 		
}
//��ȡFlash����
void BspFlashRead(void )
{
	u32 flashAddress,rawAddress;
	flashAddress=FlashStartAddr;
	rawAddress=RamStartAddr;
	
	while(flashAddress < FlashEndAddr)
	{
		(*(__IO uint32_t*) rawAddress) =(*(__IO uint32_t*) flashAddress); 
		flashAddress += 4;
		rawAddress += 4;
	}

}
//д��FLSH����  �ɹ�����0
u8 BspFlashWrite(void )
{
	u32 flashAddress,rawAddress;
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	/* Unlock the Flash Program Erase controller */

	FLASH_Unlock();
	
	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	
	/* Erase the FLASH pages */
	FLASHStatus = FLASH_ErasePage(FlashStartAddr);
	if(FLASH_COMPLETE!=FLASHStatus)return 1;
	
	/*  FLASH Word program of data 0x15041979 at addresses defined by StartAddr and EndAddr*/
	flashAddress=FlashStartAddr;
	rawAddress=RamStartAddr;
	
	while(flashAddress < FlashEndAddr)
	{
		FLASHStatus = FLASH_ProgramWord(flashAddress, (*(__IO uint32_t*) rawAddress));
		flashAddress += 4;
		rawAddress+=4;
		if(FLASHStatus != FLASH_COMPLETE)return 1;
	}
	
	/* Check the corectness of written data */
	flashAddress=FlashStartAddr;
	rawAddress=RamStartAddr;
	while(flashAddress < FlashEndAddr)
	{
		if((*(__IO uint32_t*) rawAddress) !=(*(__IO uint32_t*) flashAddress))return 1;
		flashAddress += 4;
		rawAddress+=4;
	}
	return 0;
}

