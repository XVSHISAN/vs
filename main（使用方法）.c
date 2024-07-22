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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �ж����ȼ������2��
	Key_Init();
	LEDSEG_Init();
	Serial_Init();	   // ��ʼ��USART1�����ڵ��ԣ�
	USART3_init(9600); // ��ʼ��USART3�����ڹ�����
	robot_Init();	   // �����˳�ʼ��

	while (1)
	{
		// ����USART3���յ�������
		if (MyUsart3.flag) // ���յ�һ��������
		{
			MyUsart3.flag = 0; // ��ձ�־λ

			if (strcmp((const char *)MyUsart3.buff, "ONA") == 0)
			{
				printf("go forward!\r\n"); // �������
				makerobo_run(80, 100);	   // ������ǰ��
			}
			else if (strcmp((const char *)MyUsart3.buff, "ONB") == 0)
			{
				printf("go back!\r\n"); // �������
				makerobo_back(80, 100); // �����˺���
			}
			else if (strcmp((const char *)MyUsart3.buff, "ONC") == 0)
			{
				printf("go left!\r\n"); // �������
				makerobo_Left(80, 100); // ��������ת
			}
			else if (strcmp((const char *)MyUsart3.buff, "OND") == 0)
			{
				printf("go right!\r\n"); // �������
				makerobo_Right(80, 100); // ��������ת
			}
			else if (strcmp((const char *)MyUsart3.buff, "ONF") == 0 || strcmp((const char *)MyUsart3.buff, "ONE") == 0)
			{
				printf("Stop!\r\n"); // �������
				makerobo_brake(100); // ������ֹͣ
			}
		}

		// ��ѡ��������Դ��ڽ��յ�������
		// if (Serial_GetRxFlag())
		// {
		//     uint8_t data = Serial_GetRxData();
		//     // ���ݽ��յ������ݽ�����Ӧ�ĵ��Դ���
		// }
	}
}
