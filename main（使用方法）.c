#include "stm32f10x.h" // Device header
#include "LEDSEG.h"
#include "Delay.h"
#include "robot.h"
#include <string.h>
#include <stdio.h>
#include "Key.h"
#include "Serial.h"
#include "Uart3.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断优先级分组分2组
	Key_Init();
	LEDSEG_Init();
	Serial_Init();	   // 初始化USART1（用于调试）
	USART3_init(9600); // 初始化USART3（用于工作）
	robot_Init();	   // 机器人初始化

	while (1)
	{
		// 处理USART3接收到的数据
		if (MyUsart3.flag) // 接收到一次数据了
		{
			MyUsart3.flag = 0; // 清空标志位

			if (strcmp((const char *)MyUsart3.buff, "ONA") == 0)
			{
				printf("go forward!\r\n"); // 调试输出
				makerobo_run(80, 100);	   // 机器人前进
			}
			else if (strcmp((const char *)MyUsart3.buff, "ONB") == 0)
			{
				printf("go back!\r\n"); // 调试输出
				makerobo_back(80, 100); // 机器人后退
			}
			else if (strcmp((const char *)MyUsart3.buff, "ONC") == 0)
			{
				printf("go left!\r\n"); // 调试输出
				makerobo_Left(80, 100); // 机器人左转
			}
			else if (strcmp((const char *)MyUsart3.buff, "OND") == 0)
			{
				printf("go right!\r\n"); // 调试输出
				makerobo_Right(80, 100); // 机器人右转
			}
			else if (strcmp((const char *)MyUsart3.buff, "ONF") == 0 || strcmp((const char *)MyUsart3.buff, "ONE") == 0)
			{
				printf("Stop!\r\n"); // 调试输出
				makerobo_brake(100); // 机器人停止
			}
		}

		// 可选：处理调试串口接收到的数据
		// if (Serial_GetRxFlag())
		// {
		//     uint8_t data = Serial_GetRxData();
		//     // 根据接收到的数据进行相应的调试处理
		// }
	}
}
