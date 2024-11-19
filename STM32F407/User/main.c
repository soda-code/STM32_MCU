#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/DMA/dma.h"
#include "string.h"

#define SEND_BUF_SIZE       (sizeof(TEXT_TO_SEND) + 2) * 200        /* �������ݳ���, ����sizeof(TEXT_TO_SEND) + 2��200��. */

uint8_t TEXT_TO_SEND[] = {"����ԭ�� STM32 DMA ����ʵ��\r\n"};     /* Ҫѭ�����͵��ַ��� */

uint8_t g_sendbuf[100];           /* �������ݻ����� */

int main(void)
{
    uint8_t  key = 0;
    uint16_t i;
    uint16_t len;
    float pro = 0;                          /* ����:0~100 */
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    
    DMA_TX_init(DMA2_Stream7, DMA_CHANNEL_4);  /* ��ʼ��DMA */
    DMA_RX_init(DMA2_Stream5,DMA_CHANNEL_4);



	
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "DMA TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Start", RED);

    len = sizeof(TEXT_TO_SEND);
	memcpy(g_sendbuf,TEXT_TO_SEND,sizeof(TEXT_TO_SEND));



    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)   /* KEY0���� */
        {
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
		else if (key == KEY1_PRES)   /* KEY0���� */
		{
			HAL_UART_Receive_DMA(&g_uart1_handle, (uint8_t *)g_sendbuf, 30);

			
		}

        i++;
        delay_ms(10);

        if (i == 20)
        {
            LED0_TOGGLE();  /* LED0��˸,��ʾϵͳ�������� */
            i = 0;
        }
    }
}
