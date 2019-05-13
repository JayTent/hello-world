#ifndef __FLASH_H
#define __FLASH_H

//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 64//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1//ʹ��FLASHд��(0��������;1��ʹ��)

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE     0x08000000//STM32 FLASH����ʼ��ַ
#define Start_Standing_ADDR  0X0800FC00

void STMFLASH_Write(unsigned int WriteAddr, unsigned short int *pBuffer, unsigned short int NumToWrite);//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(unsigned int ReadAddr, unsigned short int *pBuffer, unsigned short int NumToRead);//��ָ����ַ��ʼ����ָ�����ȵ�����
#endif
