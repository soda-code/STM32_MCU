#include "./DMA_APP/dma_usart_rec_send.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/DMA/dma.h"
#include "string.h"

#define SEND_BUF_SIZE       (sizeof(TEXT_TO_SEND) + 2) * 200        /* �������ݳ���, ����sizeof(TEXT_TO_SEND) + 2��200��. */

uint8_t TEXT_TO_SEND[] = {"����ԭ�� STM32 DMA ����ʵ��\r\n"};     /* Ҫѭ�����͵��ַ��� */

uint8_t g_sendbuf[100];           /* �������ݻ����� */



void DMA_USART_Send(void)
{
	uint16_t i;
	uint16_t len;
	float pro = 0;                          /* ����:0~100 */
	
	memcpy(g_sendbuf,TEXT_TO_SEND,sizeof(TEXT_TO_SEND));
	
	lcd_show_string(30, 130, 200, 16, 16, "Start Transimit....", BLUE);
	lcd_show_string(30, 150, 200, 16, 16, "   %", BLUE);                /* ��ʾ�ٷֺ� */

	HAL_UART_Transmit_DMA(&g_uart1_handle, g_sendbuf, 100);   /* ��ʼһ��DMA���䣡 */

	/* �ȴ�DMA������ɣ���ʱ������������һЩ���飬������
	* ʵ��Ӧ���У����������ڼ䣬����ִ����������� 
	*/
	while (1)
	{
		if (__HAL_DMA_GET_FLAG(&TX_dma_handle, DMA_FLAG_TCIF3_7))        /* �ȴ�DMA2_Stream7������� */
		{
			__HAL_DMA_CLEAR_FLAG(&TX_dma_handle, DMA_FLAG_TCIF3_7);      /* ���DMA2_Stream7������ɱ�־ */
			HAL_UART_DMAStop(&g_uart1_handle);      /* ��������Ժ�رմ���DMA */
			break;
		}
		len = SEND_BUF_SIZE;                        /* �ܳ��� */
		pro = 1 - (pro / len);                      /* �õ��ٷֱ� */
		pro *= 100;                                 /* ����100�� */
		lcd_show_num(30, 150, pro, 3, 16, BLUE);
	}
	lcd_show_num(30, 150, 100, 3, 16, BLUE);        /* ��ʾ100% */
	lcd_show_string(30, 130, 200, 16, 16, "Transimit Finished!", BLUE);     /* ��ʾ������� */
}

void DMA_USART_Recv(void)
{
	HAL_UART_Receive_DMA(&g_uart1_handle, (uint8_t *)g_sendbuf, 30);
	lcd_show_string(30, 130, 200, 16, 16, "Transimit Finished!", BLUE);     /* ��ʾ������� */
	lcd_show_string(30, 130, 200, 16, 16, &g_sendbuf[0], BLUE);     /* ��ʾ������� */
	memset(g_sendbuf,0,sizeof(g_sendbuf));
}