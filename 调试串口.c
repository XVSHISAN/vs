#include "stm32f10x.h" // Device header
#include <stdio.h>

#include <stdarg.h>

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void Serial_Init(void) // 初始化串口
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // 使能串口1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 使能GPIOA时钟

	GPIO_InitTypeDef GPIO_InitStructure;			  // GPIO初始化结构体
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	  // 复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		  // PA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  // 初始化PA9

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	  // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		  // PA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  // 初始化PA10

	USART_InitTypeDef USART_InitStruture;										   // 串口初始化结构体
	USART_InitStruture.USART_BaudRate = 9600;									   // 波特率
	USART_InitStruture.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控
	USART_InitStruture.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;				   // 收发模式
	USART_InitStruture.USART_Parity = USART_Parity_No;							   // 无校验
	USART_InitStruture.USART_StopBits = USART_StopBits_1;						   // 1位停止位
	USART_InitStruture.USART_WordLength = USART_WordLength_8b;					   // 8位数据位
	USART_Init(USART1, &USART_InitStruture);									   // 初始化USART1

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			  // 使能接收中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			  // 设置中断优先级分组
	NVIC_InitTypeDef NVIC_InitStructure;					  // NVIC初始化结构体
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  // 串口1中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // 使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		  // 子优先级
	NVIC_Init(&NVIC_InitStructure);							  // 初始化

	USART_Cmd(USART1, ENABLE); // 使能USART1
}

void Serial_SendByte(uint8_t Byte) // 发送一个字节
{
	USART_SendData(USART1, Byte);								 // 发送一个字节
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) // 等待发送完成
		;
}

void Serial_SendArray(uint8_t *Array, uint16_t Length) // 发送数组
{
	uint16_t i;					 // 循环变量
	for (i = 0; i < Length; i++) // 循环发送
	{
		Serial_SendByte(Array[i]); // 发送一个字节
	}
}

void Serial_SendString(char *String) // 发送字符串
{
	uint8_t i;							// 循环变量
	for (i = 0; String[i] != '\0'; i++) // 循环发送
	{
		Serial_SendByte(String[i]); // 发送一个字节
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y) // 次方
{
	uint32_t Result = 1; // 结果
	while (Y--)			 // 循环
	{
		Result *= X; // 计算
	}
	return Result; // 返回
}

void Serial_SendNumber(uint32_t Number, uint8_t Length) // 发送数字
{
	uint8_t i;					 // 循环变量
	for (i = 0; i < Length; i++) // 循环
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0'); // 发送一个字节
	}
}

int fputc(int ch, FILE *f) // 发送一个字节
{
	Serial_SendByte(ch); // 发送一个字节
	return ch;			 // 返回
}

void Serial_Printf(char *format, ...) // 发送格式化字符串
{
	char String[100];			   // 字符串
	va_list arg;				   // 参数
	va_start(arg, format);		   // 开始
	vsprintf(String, format, arg); // 转换
	va_end(arg);				   // 结束
	Serial_SendString(String);	   // 发送字符串
}

uint8_t Serial_GetRxFlag(void) // 获取接收标志
{
	if (Serial_RxFlag == 1) // 接收完成
	{
		Serial_RxFlag = 0; // 清除标志
		return 1;		   // 接收完成
	}
	return 0; // 未完成
}

uint8_t Serial_GetRxData(void) // 获取接收数据
{
	return Serial_RxData; // 返回
}

void USART1_IRQHandler(void) // 串口1中断
{
	if (USART_GetFlagStatus(USART1, USART_IT_RXNE) == SET) // 接收中断
	{
		Serial_RxData = USART_ReceiveData(USART1);		// 接收一个字节
		Serial_RxFlag = 1;								// 接收完成
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断
	}
}
