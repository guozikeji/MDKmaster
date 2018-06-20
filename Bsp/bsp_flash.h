#ifndef _BSP_FLASH_H
#define _BSP_FLASH_H

/*Ԥ����ȫ�ֱ���*/
#ifndef TASK_GPIO_GLOBALS
#define TASK_GPIO_EXT
#else
#define TASK_GPIO_EXT extern
#endif

#include "stm32f10x_conf.h "
#define FlashInitFlag	        0x1238                          //¥�Ÿ�	  840 ASCII

#define FlashStartAddr          ((uint32_t)0x0803F800)			//Flash��ʼ��ַ
#define FlashEndAddr            ((uint32_t)0x08040000)          //Flash������ַ
#define RamStartAddr            ((uint32_t)0x2000B800)			//raw��ʼ��ַ
#define RamEndAddr      		((uint32_t)0x2000C000)          //raw������ַ

#define RamBulidingAddr	        ((uint32_t)(0x2000B800+1680))	   //¥�Ÿ�	  840 ASCII
#define RamMeterAddr			((uint32_t)(0x2000B800+1684))	   //�ȱ����� 842
#define RamUserAddr	    		((uint32_t)(0x2000B800+1686))	   //�ȱ����� 843
#define RamCircuitChangeAddr	((uint32_t)(0x2000B800+1688))	  //��·�л�ʱ�� 30S 
#define RamRunModeAddr			((uint32_t)(0x2000B800+1690))	  //����ģʽ 
#define RamMinValveAddr			((uint32_t)(0x2000B800+1692))	  // ��������10%
#define RamMinTempRoomAddr		((uint32_t)(0x2000B800+1694))	  // �����¶�16��
#define RamIntegralTimeAddr		((uint32_t)(0x2000B800+1696))	  // ����ʱ��

#define RamMbusStartAddr        ((uint32_t)(0x2000B800+1700))	   //MBUS ��ʼ��   (11.10-3.20)
#define RamMbusEndAddr			((uint32_t)(0x2000B800+1702))	   //MBUS ��ʼ��    (11.10-3.20)
#define RamTimeSection1Addr	  	((uint32_t)(0x2000B800+1704))	   //�ϴ�ʱ���1 Сʱ  ����
#define RamTimeSection2Addr	    ((uint32_t)(0x2000B800+1706))	   //�ϴ�ʱ���2 Сʱ  ����
#define RamTimeSection3Addr		((uint32_t)(0x2000B800+1708))	   //�ϴ�ʱ���3 Сʱ  ����
#define RamTimeSection4Addr		((uint32_t)(0x2000B800+1710))	   // �ϴ�ʱ���4 Сʱ  ����
#define RamTimeSection5Addr		((uint32_t)(0x2000B800+1712))	   // �ϴ�ʱ���5 Сʱ  ����
#define RamTimeSection6Addr		((uint32_t)(0x2000B800+1714))	   // �ϴ�ʱ���6 Сʱ  ����
#define RamTimeSection7Addr		((uint32_t)(0x2000B800+1716))	   // �ϴ�ʱ���7 Сʱ  ����
#define RamTimeSection8Addr		((uint32_t)(0x2000B800+1718))	   // �ϴ�ʱ���8 Сʱ  ����

#define RamMbusSubpackageTimeAddr   ((uint32_t)(0x2000B800+1720))	   //MBUS �ְ�ʱ�� ��λ
#define RamMbusBaudRateAddr		    ((uint32_t)(0x2000B800+1722))	   //MBUS ������
#define RamMbusDateBitsAddr	        ((uint32_t)(0x2000B800+1724))	   //MBUS ����λ ����8-0x0000 ��9-0x1000
#define RamMbusStopBitsAddr	        ((uint32_t)(0x2000B800+1726))	   //1ֹͣλ0X0000 0.5 ֹͣλ 0X1000 2ֹͣλ 0X2000 1.5ֹͣλ0X3000
#define RamMbusParityBitsAddr       ((uint32_t)(0x2000B800+1728))	   //0X0000��У�� 0X0600��У��  0X0400żУ��
#define RamMbusRestartTimeAddr	    ((uint32_t)(0x2000B800+1730))	   // MBUS ����ʱ�� 5s   
#define RamMbusCircuitTimeAddr	    ((uint32_t)(0x2000B800+1732))	   // MBUS ѭ��ʱ�� 2S
#define RamMbusErrorCountAddr		((uint32_t)(0x2000B800+1734))	   // MBUS ��ȡ�������	4

#define RamLcdControlAddr           ((uint32_t)(0x2000B800+1740))	   //0���ؿ��ƣ�1Զ�̿���
#define RamLcdRefreshTimeAddr		((uint32_t)(0x2000B800+1742))	   //LCDˢ��ʱ�� 	 250
#define RamLcdStartTimeAddr	        ((uint32_t)(0x2000B800+1744))	   // LCD��ʾ��ʼʱ�� 7:30��ʼ��ʾ
#define RamLcdEndTimeAddr	        ((uint32_t)(0x2000B800+1746))	   // LCD��ʾ����ʱ�� 18:30������ʾ
#define RamLcdHeadFrontColorAddr    ((uint32_t)(0x2000B800+1748))	   //LCDͷ�ļ�ǰ��ɫ ��ɫ
#define RamLcdHeadBackColorAddr	    ((uint32_t)(0x2000B800+1750))	   //LCDͷ�ļ�����ɫ ��ɫ
#define RamLcdContentFrontColorAddr	((uint32_t)(0x2000B800+1752))	   //LCD����ǰ��ɫ ��ɫ
#define RamLcdContentBackColorAddr  ((uint32_t)(0x2000B800+1754))	   //LCD���ݱ���ɫ	���
#define RamLcdHighLightColorAddr	((uint32_t)(0x2000B800+1752))	   //LCD������ʾɫ	��ɫ
#define RamLcdBrightnessAddr        ((uint32_t)(0x2000B800+1754))	   //LCD���� ���� 0�� 1000���

#define RamRs485SubpackageTimeAddr  ((uint32_t)(0x2000B800+1760))	   //RS485�ְ�ʱ��
#define RamRs485BaudRateAddr		((uint32_t)(0x2000B800+1762))	   // ������
#define RamRs485DateBitsAddr	    ((uint32_t)(0x2000B800+1764))	   // ����λ ����8-0x0000 ��9-0x1000
#define RamRs485StopBitsAddr	    ((uint32_t)(0x2000B800+1766))	   //1ֹͣλ0X0000 0.5 ֹͣλ 0X1000 2ֹͣλ 0X2000 1.5ֹͣλ0X3000
#define RamRs485ParityBitsAddr      ((uint32_t)(0x2000B800+1768))	   //0X0000��У�� 0X0600��У��  0X0400żУ��

#define RamGprsSubpackageTimeAddr   ((uint32_t)(0x2000B800+1770))	   //GPRS�ְ�ʱ��
#define RamGprsBaudRateAddr		    ((uint32_t)(0x2000B800+1772))	   // ������
#define RamGprsDateBitsAddr	        ((uint32_t)(0x2000B800+1774))	   // ����λ ����8-0x0000 ��9-0x1000
#define RamGprsStopBitsAddr	        ((uint32_t)(0x2000B800+1776))	   //1ֹͣλ0X0000 0.5 ֹͣλ 0X1000 2ֹͣλ 0X2000 1.5ֹͣλ0X3000
#define RamGprsParityBitsAddr       ((uint32_t)(0x2000B800+1778))	   //0X0000��У�� 0X0600��У��  0X0400żУ��

#define RamGprsDscModeAddr          ((uint32_t)(0x2000B800+1780))	   //GPRS 2DSC3  1�Զ��� ͨѶЭ��
#define RamGprsLinkTimeAddr		    ((uint32_t)(0x2000B800+1782))	   // GPRS�����ʱ��
#define RamGprsLinkStopAddr	        ((uint32_t)(0x2000B800+1784))	   // �ʱ��û������������ ֹͣ�೤ʱ�� ����������
#define RamGprsNoneReceiveTimeAddr	((uint32_t)(0x2000B800+1786))	   //�����������೤ʱ��?û�н��ܵ�����,��ʼ��������
#define RamGprsHeartTimeAddr        ((uint32_t)(0x2000B800+1788))	   // �������������ʱ��	
#define RamGprsUpdataTimeAddr       ((uint32_t)(0x2000B800+1790))	   // ������ʱ�ϴ�����ʱ����

#define RamGprsDns1DataLengthAddr       ((uint32_t)(0x2000B800+1800))	   //202.102.152.3 firstDns
#define RamGprsDns2DataLengthAddr       ((uint32_t)(0x2000B800+1820))	   //202.102.152.3 secondaryDns
#define RamGprsHeartDataLengthAddr      ((uint32_t)(0x2000B800+1840))	   //GPRS GPRS �������ַ���

#define RamGprsSmidTelAddr              ((uint32_t)(0x2000B800+1860))	   //GPRS SMID �ֻ���
#define RamGprsAdmitTelNumberAddr       ((uint32_t)(0x2000B800+1872))	   //��Ȩ�ֻ��� 13326309379	
#define RamGprsSmidNumberAddr           ((uint32_t)(0x2000B800+1884))	   //GPRS SMID	GPRS_SMID

#define RamGprsPortLengthAddr           ((uint32_t)(0x2000B800+1888))	   //GPRS IP�˿ں����ݳ���
//����� APN jyzhrl.sdapn
#define RamGprsApnLengthAddr            ((uint32_t)(0x2000B800+1900))	   //GPRS  �����  CMNET UNINET

#define RamGprsHttpLengthAddr           ((uint32_t)(0x2000B800+1940))	   //GPRS  ���� www.20cdata.com


void BspRawInit(void);
u8 BspFlashInit(void);
void BspFlashRead(void );
u8 BspFlashWrite(void );

#endif 
