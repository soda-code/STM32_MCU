#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/DMA/dma.h"
#include "./DMA_APP/dma_usart_rec_send.h"



int main(void)
{
	uint8_t  key = 0;
	uint16_t i;



    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /* 设置时钟,168Mhz */
    delay_init(168);                        /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    led_init();                             /* 初始化LED */
    lcd_init();                             /* 初始化LCD */
    key_init();                             /* 初始化按键 */
    
    DMA_TX_init(DMA2_Stream7, DMA_CHANNEL_4);  /* 初始化DMA */
    DMA_RX_init(DMA2_Stream5,DMA_CHANNEL_4);



	
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "DMA TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY0:Start", RED);

    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)   /* KEY0按下 */
        {
			DMA_USART_Send();
		}
		else if (key == KEY1_PRES)   /* KEY0按下 */
		{
			DMA_USART_Recv();
		}

        i++;
        delay_ms(10);

        if (i == 20)
        {
            LED0_TOGGLE();  /* LED0闪烁,提示系统正在运行 */
            i = 0;
        }
    }
}
