#include "FLASH.h"
#include "stm32f10x.h"

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����
unsigned short int STMFLASH_ReadHalfWord(unsigned int faddr){
		return *(volatile unsigned short int*)faddr;
}
#if STM32_FLASH_WREN//���ʹ����д
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Write_NoCheck(unsigned int WriteAddr, unsigned short int *pBuffer, unsigned short int NumToWrite){
		unsigned short int i;
		for(i=0;i<NumToWrite;i++){
			FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
			WriteAddr+=2;//��ַ����2
		}
}
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���)
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024//�ֽ�
#else
#define STM_SECTOR_SIZE	2048
#endif
unsigned short int STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
void STMFLASH_Write(unsigned int WriteAddr, unsigned short int *pBuffer, unsigned short int NumToWrite){
		unsigned int secpos;//������ַ
		unsigned short int secoff;	 //������ƫ�Ƶ�ַ(16λ�ּ���)
		unsigned short int secremain;//������ʣ���ַ(16λ�ּ���)
		unsigned short int i;
		unsigned int offaddr;//ȥ��0X08000000��ĵ�ַ
		if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE))) return;//�Ƿ���ַ
		FLASH_Unlock();//����
		offaddr=WriteAddr-STM32_FLASH_BASE;//ʵ��ƫ�Ƶ�ַ
		secpos=offaddr/STM_SECTOR_SIZE;		 //������ַ 0~127 for STM32F103RBT6
		secoff=(offaddr%STM_SECTOR_SIZE)/2;//�������ڵ�ƫ��(2���ֽ�Ϊ������λ)
		secremain=STM_SECTOR_SIZE/2-secoff;//����ʣ��ռ��С
		if(NumToWrite<=secremain) secremain=NumToWrite;//�����ڸ�������Χ
		while(1){
			STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE/2);//������������������
			for(i=0;i<secremain;i++){//У������
				if(STMFLASH_BUF[secoff+i]!=0XFFFF) break;//��Ҫ����
			}
			if(i<secremain){//��Ҫ����
				FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
				for(i=0;i<secremain;i++){//����
					STMFLASH_BUF[i+secoff]=pBuffer[i];
				}
				STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE/2);//д����������
			}else STMFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain);//д�Ѿ������˵�,ֱ��д������ʣ������
			if(NumToWrite==secremain) break;//д�������
			else{//д��δ����
				secpos++;//������ַ��1
				secoff=0;//ƫ��λ��Ϊ0
				pBuffer+=secremain;//ָ��ƫ��
				WriteAddr+=secremain;//д��ַƫ��
				NumToWrite-=secremain;//�ֽ�(16λ)���ݼ�
				if(NumToWrite>(STM_SECTOR_SIZE/2)) secremain=STM_SECTOR_SIZE/2;//��һ����������д����
				else secremain=NumToWrite;//��һ����������д����
			}
		}
		FLASH_Lock();//����
}
#endif
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(unsigned int ReadAddr, unsigned short int *pBuffer, unsigned short int NumToRead){
		unsigned short int i;
		for(i=0;i<NumToRead;i++){
			pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
			ReadAddr+=2;//ƫ��2���ֽ�.
		}
}