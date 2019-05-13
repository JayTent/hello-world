void ADC1_Init(void){
		GPIO_InitTypeDef GPIO_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		/* DMA channel1 configuration */
		DMA_DeInit(DMA1_Channel1);//复位DMA1
		DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//DMA外设地址：ADC地址
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue[0];//内存地址
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//SRC转换模式 由外设到内存
		DMA_InitStructure.DMA_BufferSize = 96;//DMA缓存大小
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址累加
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据宽度：半字
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存数据宽度：半字
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//不断循环传输
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//DMA最高优先级
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//禁止内存到内存传输
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);//配置DMA1通道1

		/* Configure PA.0\1\2\3\4\5\6  as analog input */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
		ADC_DeInit(ADC1);
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立ADC模式
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//循环ADC转换
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不使用外部触发转换
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//采集数据右对齐
		ADC_InitStructure.ADC_NbrOfChannel = 6;//要转换的通道数目6
		ADC_Init(ADC1, &ADC_InitStructure);
		
		/*配置ADC239.	5个采样周期，序列为1 */ 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5);
		/* Enable ADC1 */
		ADC_DMACmd(ADC1, ENABLE);
		ADC_Cmd(ADC1, ENABLE);
		
		/*复位校准寄存器 */   
		ADC_ResetCalibration(ADC1);
		/*等待校准寄存器复位完成 */
		while(ADC_GetResetCalibrationStatus(ADC1));
		/* ADC校准 */
		ADC_StartCalibration(ADC1);
		/* 等待校准完成*/
		while(ADC_GetCalibrationStatus(ADC1));
		/* Enable DMA channel1 */
		DMA_Cmd(DMA1_Channel1, ENABLE);
		/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}