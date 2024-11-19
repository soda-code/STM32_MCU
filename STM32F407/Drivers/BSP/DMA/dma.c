#include "./BSP/DMA/dma.h"
#include "./SYSTEM/delay/delay.h"


DMA_HandleTypeDef  TX_dma_handle;                  /* DMA句柄 */
DMA_HandleTypeDef  RX_dma_handle;                  /* DMA句柄 */


extern UART_HandleTypeDef g_uart1_handle;         /* UART句柄 */

/**
 * @brief       串口TX DMA初始化函数
 *   @note      这里的传输形式是固定的, 这点要根据不同的情况来修改
 *              从存储器 -> 外设模式/8位数据宽度/存储器增量模式
 *
 * @param       dma_stream_handle : DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
 * @retval      无
 */
void DMA_TX_init(DMA_Stream_TypeDef *dma_stream_handle, uint32_t ch)
{ 
    if ((uint32_t)dma_stream_handle > (uint32_t)DMA2)       /* 得到当前stream是属于DMA2还是DMA1 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE();                        /* DMA2时钟使能 */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();                        /* DMA1时钟使能 */
    }

    __HAL_LINKDMA(&g_uart1_handle, hdmatx, TX_dma_handle);   /* 将DMA与USART1联系起来(发送DMA) */

    /* Tx DMA配置 */
    TX_dma_handle.Instance = dma_stream_handle;                    /* 数据流选择 */
    TX_dma_handle.Init.Channel = ch;                               /* DMA通道选择 */
    TX_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;           /* 存储器到外设 */
    TX_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;               /* 外设非增量模式 */
    TX_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                   /* 存储器增量模式 */
    TX_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  /* 外设数据长度:8位 */
    TX_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     /* 存储器数据长度:8位 */
    TX_dma_handle.Init.Mode = DMA_NORMAL;                          /* 外设流控模式 */
    TX_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;             /* 中等优先级 */
    TX_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            /* 关闭FIFO模式 */
    TX_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    /* FIFO阈值配置 */
    TX_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;               /* 存储器突发单次传输 */
    TX_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;            /* 外设突发单次传输 */

    HAL_DMA_DeInit(&TX_dma_handle);
    HAL_DMA_Init(&TX_dma_handle);
}

void DMA_RX_init(DMA_Stream_TypeDef *dma_stream_handle, uint32_t ch)
{ 
	__HAL_RCC_DMA2_CLK_ENABLE();                        /* DMA2时钟使能 */

    /* Tx DMA配置 */
    RX_dma_handle.Instance = dma_stream_handle;                    /* 数据流选择 */
    RX_dma_handle.Init.Channel = ch;                               /* DMA通道选择 */
    RX_dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;           /* 存储器到外设 */
    RX_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;               /* 外设非增量模式 */
    RX_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                   /* 存储器增量模式 */
    RX_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  /* 外设数据长度:8位 */
    RX_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     /* 存储器数据长度:8位 */
    RX_dma_handle.Init.Mode = DMA_NORMAL;                          /* 外设流控模式 */
    RX_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;             /* 中等优先级 */
	//HAL_DMA_DeInit(&RX_dma_handle);
	HAL_DMA_Init(&RX_dma_handle);
	
    __HAL_LINKDMA(&g_uart1_handle, hdmarx, RX_dma_handle);   /* 将DMA与USART1联系起来(发送DMA) */
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn,3, 2);
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

}

void DMA2_Stream5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&RX_dma_handle);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
		HAL_UART_DMAStop(&g_uart1_handle);		/* 传输完成以后关闭串口DMA */

	// 处理接收到的数据
    }
}


