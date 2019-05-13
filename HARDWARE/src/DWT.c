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
*	�� �� ��: InitDWT
*	����˵��: ��ʼ��DWT
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void InitDWT(void){
		DEM_CR    |= (unsigned int)DEM_CR_TRCENA;
		DWT_CYCCNT = (unsigned int)0u;
		DWT_CR    |= (unsigned int)DWT_CR_CYCCNTENA;
}

/*
*********************************************************************************************************
*	�� �� ��: DWT_DelayUs
*	����˵��: �������ʱ����CPU���ڲ�����ʵ�֣�32λ������
*	��    ��: nus���ӳٳ��ȣ���λ1us
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DWT_DelayUs(unsigned int nus){
    unsigned int tCnt, tDelayCnt;
		unsigned int tStart;
		tStart = DWT_CYCCNT;//�ս���ʱ�ļ�����ֵ
		tCnt = 0;
		tDelayCnt = nus*(SystemCoreClock/1000000);//��Ҫ�Ľ�����
		while(tCnt < tDelayCnt){
			tCnt = DWT_CYCCNT - tStart;//��������У����������һ��32λ���������¼�������Ȼ������ȷ����
		}
}

/*
*********************************************************************************************************
*	�� �� ��: DWT_DelayMs
*	����˵��: Ϊ���õײ������ڴ�RTOS�����������и��õļ�����
*           ר������һ������ʽ���ӳٺ������ڵײ�������ms�����ӳ���Ҫ���ڳ�ʼ����������Ӱ��ʵʱ�ԡ�
*	��    ��: nms:�ӳٳ��ȣ���λ1ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DWT_DelayMs(unsigned int nms){
		DWT_DelayUs(1000*nms);
}
