#include "stm32f10x.h" // Device header
#include <stdio.h>

#include <stdarg.h>

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void Serial_Init(void) // ��ʼ������
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // ʹ�ܴ���1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // ʹ��GPIOAʱ��

	GPIO_InitTypeDef GPIO_InitStructure;			  // GPIO��ʼ���ṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  // �����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		  // PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  // ��ʼ��PA9

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	  // ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		  // PA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  // ��ʼ��PA10

	USART_InitTypeDef USART_InitStruture;										   // ���ڳ�ʼ���ṹ��
	USART_InitStruture.USART_BaudRate = 9600;									   // ������
	USART_InitStruture.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������
	USART_InitStruture.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;				   // �շ�ģʽ
	USART_InitStruture.USART_Parity = USART_Parity_No;							   // ��У��
	USART_InitStruture.USART_StopBits = USART_StopBits_1;						   // 1λֹͣλ
	USART_InitStruture.USART_WordLength = USART_WordLength_8b;					   // 8λ����λ
	USART_Init(USART1, &USART_InitStruture);									   // ��ʼ��USART1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			  // ʹ�ܽ����ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			  // �����ж����ȼ�����
	NVIC_InitTypeDef NVIC_InitStructure;					  // NVIC��ʼ���ṹ��
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  // ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  // �����ȼ�
	NVIC_Init(&NVIC_InitStructure);							  // ��ʼ��

	USART_Cmd(USART1, ENABLE); // ʹ��USART1
}

void Serial_SendByte(uint8_t Byte) // ����һ���ֽ�
{
	USART_SendData(USART1, Byte);								 // ����һ���ֽ�
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) // �ȴ��������
		;
}

void Serial_SendArray(uint8_t *Array, uint16_t Length) // ��������
{
	uint16_t i;					 // ѭ������
	for (i = 0; i < Length; i++) // ѭ������
	{
		Serial_SendByte(Array[i]); // ����һ���ֽ�
	}
}

void Serial_SendString(char *String) // �����ַ���
{
	uint8_t i;							// ѭ������
	for (i = 0; String[i] != '\0'; i++) // ѭ������
	{
		Serial_SendByte(String[i]); // ����һ���ֽ�
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y) // �η�
{
	uint32_t Result = 1; // ���
	while (Y--)			 // ѭ��
	{
		Result *= X; // ����
	}
	return Result; // ����
}

void Serial_SendNumber(uint32_t Number, uint8_t Length) // ��������
{
	uint8_t i;					 // ѭ������
	for (i = 0; i < Length; i++) // ѭ��
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0'); // ����һ���ֽ�
	}
}

int fputc(int ch, FILE *f) // ����һ���ֽ�
{
	Serial_SendByte(ch); // ����һ���ֽ�
	return ch;			 // ����
}

void Serial_Printf(char *format, ...) // ���͸�ʽ���ַ���
{
	char String[100];			   // �ַ���
	va_list arg;				   // ����
	va_start(arg, format);		   // ��ʼ
	vsprintf(String, format, arg); // ת��
	va_end(arg);				   // ����
	Serial_SendString(String);	   // �����ַ���
}

uint8_t Serial_GetRxFlag(void) // ��ȡ���ձ�־
{
	if (Serial_RxFlag == 1) // �������
	{
		Serial_RxFlag = 0; // �����־
		return 1;		   // �������
	}
	return 0; // δ���
}

uint8_t Serial_GetRxData(void) // ��ȡ��������
{
	return Serial_RxData; // ����
}

void USART1_IRQHandler(void) // ����1�ж�
{
	if (USART_GetFlagStatus(USART1, USART_IT_RXNE) == SET) // �����ж�
	{
		Serial_RxData = USART_ReceiveData(USART1);		// ����һ���ֽ�
		Serial_RxFlag = 1;								// �������
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); // ����ж�
	}
}
