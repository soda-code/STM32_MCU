#include "./DMA_APP/dma_usart_rec_send.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/DMA/dma.h"
#include "string.h"

#define SEND_BUF_SIZE       (sizeof(TEXT_TO_SEND) + 2) * 200        /* 发送数据长度, 等于sizeof(TEXT_TO_SEND) + 2的200倍. */

uint8_t TEXT_TO_SEND[] = {"正点原子 STM32 DMA 串口实验\r\n"};     /* 要循环发送的字符串 */

uint8_t g_sendbuf[100];           /* 发送数据缓冲区 */



void DMA_USART_Send(void)
{
	uint16_t i;
	uint16_t len;
	float pro = 0;                          /* 进度:0~100 */
	
	memcpy(g_sendbuf,TEXT_TO_SEND,sizeof(TEXT_TO_SEND));
	
	lcd_show_string(30, 130, 200, 16, 16, "Start Transimit....", BLUE);
	lcd_show_string(30, 150, 200, 16, 16, "   %", BLUE);                /* 显示百分号 */

	HAL_UART_Transmit_DMA(&g_uart1_handle, g_sendbuf, 100);   /* 开始一次DMA传输！ */

	/* 等待DMA传输完成，此时我们来做另外一些事情，比如点灯
	* 实际应用中，传输数据期间，可以执行另外的任务 
	*/
	while (1)
	{
		if (__HAL_DMA_GET_FLAG(&TX_dma_handle, DMA_FLAG_TCIF3_7))        /* 等待DMA2_Stream7传输完成 */
		{
			__HAL_DMA_CLEAR_FLAG(&TX_dma_handle, DMA_FLAG_TCIF3_7);      /* 清除DMA2_Stream7传输完成标志 */
			HAL_UART_DMAStop(&g_uart1_handle);      /* 传输完成以后关闭串口DMA */
			break;
		}
		len = SEND_BUF_SIZE;                        /* 总长度 */
		pro = 1 - (pro / len);                      /* 得到百分比 */
		pro *= 100;                                 /* 扩大100倍 */
		lcd_show_num(30, 150, pro, 3, 16, BLUE);
	}
	lcd_show_num(30, 150, 100, 3, 16, BLUE);        /* 显示100% */
	lcd_show_string(30, 130, 200, 16, 16, "Transimit Finished!", BLUE);     /* 提示传送完成 */
}

void DMA_USART_Recv(void)
{
	HAL_UART_Receive_DMA(&g_uart1_handle, (uint8_t *)g_sendbuf, 30);
	lcd_show_string(30, 130, 200, 16, 16, "Transimit Finished!", BLUE);     /* 提示传送完成 */
	lcd_show_string(30, 130, 200, 16, 16, &g_sendbuf[0], BLUE);     /* 提示传送完成 */
	memset(g_sendbuf,0,sizeof(g_sendbuf));
}