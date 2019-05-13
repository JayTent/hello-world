#ifndef __FLASH_H
#define __FLASH_H

//用户根据自己的需要设置
#define STM32_FLASH_SIZE 64//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1//使能FLASH写入(0，不是能;1，使能)

//FLASH起始地址
#define STM32_FLASH_BASE     0x08000000//STM32 FLASH的起始地址
#define Start_Standing_ADDR  0X0800FC00

void STMFLASH_Write(unsigned int WriteAddr, unsigned short int *pBuffer, unsigned short int NumToWrite);//从指定地址开始写入指定长度的数据
void STMFLASH_Read(unsigned int ReadAddr, unsigned short int *pBuffer, unsigned short int NumToRead);//从指定地址开始读出指定长度的数据
#endif
