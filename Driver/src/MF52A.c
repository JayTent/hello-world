#include "MF52A.h"

const unsigned short int AD_temp_data[117]=
{
	20700,19788,18838,17977,17160,16383,15646,14945,14280,13647,
	13045,12472,11928,11409,10916,10447,10000,9574 ,9168 ,8781 ,
	8413 ,8062 ,7727 ,7407 ,7103 ,6812 ,6534 ,6270 ,6017 ,5775 ,
	5545 ,5324 ,5114 ,4913 ,4720 ,4536 ,4360 ,4192 ,4031 ,3877 ,
	3730 ,3572 ,3454 ,3324 ,3201 ,3082 ,2968 ,2859 ,2755 ,2654 ,
	2558 ,2466 ,2378 ,2293 ,2212 ,2134 ,2059 ,1987 ,1918 ,1851 ,
	1788 ,1726 ,1668 ,1611 ,1557 ,1504 ,1454 ,1406 ,1359 ,1314 ,
	1271 ,1230 ,1190 ,1151 ,1114 ,1079 ,1045 ,1011 ,980  ,949  ,
	919  ,891  ,863  ,837  ,811  ,786  ,763  ,740  ,718  ,696  ,
	675  ,656  ,636  ,618  ,600  ,582  ,566  ,549  ,534  ,519  ,
	504  ,490  ,476  ,463  ,450  ,438  ,426  ,414  ,403  ,392  ,
	381  ,371  ,361  ,352  ,342  ,333  ,325
};

/*
*********************************************************************************************************
*	函 数 名: AD_Value_To_Temperature
*	功能说明: 折半法查表将AD值转换为温度值，精确到小数点后一位
*	形    参：AD_Value
*	返 回 值: Temp
*********************************************************************************************************
*/
unsigned short int AD_Value_To_Temperature(unsigned short int AD_Value){
		unsigned short int head, mid, tail;
		short int Temp;
		head=0;
		tail=116;
		AD_Value=AD_Value*10000/(MF52A_VOL-AD_Value);//换算为阻值，根据分压计算公式
		if((AD_Value<20700)&&(AD_Value>325)){//在检测范围内
			while(head<tail){
				if((tail-head)!=1){//尚未在两个相邻范围内
					mid=(head+tail)/2;
					if(AD_Value==AD_temp_data[mid]){//AD刚好在边界点上
						Temp = (mid+9)*10;
						break;
					}
					else if(AD_Value>AD_temp_data[mid]){
						tail=mid;
					}
					else if(AD_Value<AD_temp_data[mid]){
						head=mid;
					}
				}
				else{
					if(AD_Value<AD_temp_data[mid]){
						Temp=(mid+9)*10+10*(AD_temp_data[head]-AD_Value)/(AD_temp_data[head]-AD_temp_data[tail]);
					}
					else if(AD_Value>AD_temp_data[mid]){
						Temp=(mid+8)*10+10*(AD_temp_data[head]-AD_Value)/(AD_temp_data[head]-AD_temp_data[tail]);
					}
					break;
				}
			}
		}
		return Temp;
}
