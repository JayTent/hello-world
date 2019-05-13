void ADC1_Init(void){
		GPIO_InitTypeDef GPIO_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		/* DMA channel1 configuration */
		DMA_DeInit(DMA1_Channel1);//��λDMA1
		DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;//DMA�����ַ��ADC��ַ
		DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue[0];//�ڴ��ַ
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//SRCת��ģʽ �����赽�ڴ�
		DMA_InitStructure.DMA_BufferSize = 96;//DMA�����С
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�ۼ�
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݿ�ȣ�����
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ����ݿ�ȣ�����
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//����ѭ������
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//DMA������ȼ�
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//��ֹ�ڴ浽�ڴ洫��
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);//����DMA1ͨ��1

		/* Configure PA.0\1\2\3\4\5\6  as analog input */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
		ADC_DeInit(ADC1);
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//����ADCģʽ
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//ѭ��ADCת��
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ʹ���ⲿ����ת��
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�ɼ������Ҷ���
		ADC_InitStructure.ADC_NbrOfChannel = 6;//Ҫת����ͨ����Ŀ6
		ADC_Init(ADC1, &ADC_InitStructure);
		
		/*����ADC239.	5���������ڣ�����Ϊ1 */ 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5);
		/* Enable ADC1 */
		ADC_DMACmd(ADC1, ENABLE);
		ADC_Cmd(ADC1, ENABLE);
		
		/*��λУ׼�Ĵ��� */   
		ADC_ResetCalibration(ADC1);
		/*�ȴ�У׼�Ĵ�����λ��� */
		while(ADC_GetResetCalibrationStatus(ADC1));
		/* ADCУ׼ */
		ADC_StartCalibration(ADC1);
		/* �ȴ�У׼���*/
		while(ADC_GetCalibrationStatus(ADC1));
		/* Enable DMA channel1 */
		DMA_Cmd(DMA1_Channel1, ENABLE);
		/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}