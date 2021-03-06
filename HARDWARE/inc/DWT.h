#ifndef __DWT_H
#define __DWT_H
/*
*********************************************************************************************************
*	函 数 名: InitDWT
*	功能说明: 初始化DWT
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDWT(void);
/*
*********************************************************************************************************
*	函 数 名: DWT_DelayUs
*	功能说明: 这里的延时采用CPU的内部计数实现，32位计数器
*	形    参: nus：延迟长度，单位1us
*	返 回 值: 无
*********************************************************************************************************
*/
void DWT_DelayUs(unsigned int nus);
/*
*********************************************************************************************************
*	函 数 名: DWT_DelayMs
*	功能说明: 为了让底层驱动在带RTOS和裸机情况下有更好的兼容性
*           专门制作一个阻塞式的延迟函数，在底层驱动中ms毫秒延迟主要用于初始化，并不会影响实时性。
*	形    参: nms:延迟长度，单位1ms
*	返 回 值: 无
*********************************************************************************************************
*/
void DWT_DelayMs(unsigned int nms);
#endif
