#include "DWT.h"
#include "stm32f10x.h"

#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
#define  DWT_CR      *(volatile unsigned int *)0xE0001000
#define  DEM_CR      *(volatile unsigned int *)0xE000EDFC
#define  DBGMCU_CR   *(volatile unsigned int *)0xE0042004

#define  DEM_CR_TRCENA               (1 << 24)
#define  DWT_CR_CYCCNTENA            (1 <<  0)

/*
*********************************************************************************************************
*	函 数 名: InitDWT
*	功能说明: 初始化DWT
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitDWT(void){
		DEM_CR    |= (unsigned int)DEM_CR_TRCENA;
		DWT_CYCCNT = (unsigned int)0u;
		DWT_CR    |= (unsigned int)DWT_CR_CYCCNTENA;
}

/*
*********************************************************************************************************
*	函 数 名: DWT_DelayUs
*	功能说明: 这里的延时采用CPU的内部计数实现，32位计数器
*	形    参: nus：延迟长度，单位1us
*	返 回 值: 无
*********************************************************************************************************
*/
void DWT_DelayUs(unsigned int nus){
    unsigned int tCnt, tDelayCnt;
		unsigned int tStart;
		tStart = DWT_CYCCNT;//刚进入时的计数器值
		tCnt = 0;
		tDelayCnt = nus*(SystemCoreClock/1000000);//需要的节拍数
		while(tCnt < tDelayCnt){
			tCnt = DWT_CYCCNT - tStart;//求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算
		}
}

/*
*********************************************************************************************************
*	函 数 名: DWT_DelayMs
*	功能说明: 为了让底层驱动在带RTOS和裸机情况下有更好的兼容性
*           专门制作一个阻塞式的延迟函数，在底层驱动中ms毫秒延迟主要用于初始化，并不会影响实时性。
*	形    参: nms:延迟长度，单位1ms
*	返 回 值: 无
*********************************************************************************************************
*/
void DWT_DelayMs(unsigned int nms){
		DWT_DelayUs(1000*nms);
}
