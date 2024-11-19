#include "./BSP/DMA/dma.h"
#include "./SYSTEM/delay/delay.h"


DMA_HandleTypeDef  TX_dma_handle;                  /* DMA��� */
DMA_HandleTypeDef  RX_dma_handle;                  /* DMA��� */


extern UART_HandleTypeDef g_uart1_handle;         /* UART��� */

/**
 * @brief       ����TX DMA��ʼ������
 *   @note      ����Ĵ�����ʽ�ǹ̶���, ���Ҫ���ݲ�ͬ��������޸�
 *              �Ӵ洢�� -> ����ģʽ/8λ���ݿ��/�洢������ģʽ
 *
 * @param       dma_stream_handle : DMA������,DMA1_Stream0~7/DMA2_Stream0~7
 * @retval      ��
 */
void DMA_TX_init(DMA_Stream_TypeDef *dma_stream_handle, uint32_t ch)
{ 
    if ((uint32_t)dma_stream_handle > (uint32_t)DMA2)       /* �õ���ǰstream������DMA2����DMA1 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE();                        /* DMA2ʱ��ʹ�� */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();                        /* DMA1ʱ��ʹ�� */
    }

    __HAL_LINKDMA(&g_uart1_handle, hdmatx, TX_dma_handle);   /* ��DMA��USART1��ϵ����(����DMA) */

    /* Tx DMA���� */
    TX_dma_handle.Instance = dma_stream_handle;                    /* ������ѡ�� */
    TX_dma_handle.Init.Channel = ch;                               /* DMAͨ��ѡ�� */
    TX_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;           /* �洢�������� */
    TX_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;               /* ���������ģʽ */
    TX_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                   /* �洢������ģʽ */
    TX_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  /* �������ݳ���:8λ */
    TX_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     /* �洢�����ݳ���:8λ */
    TX_dma_handle.Init.Mode = DMA_NORMAL;                          /* ��������ģʽ */
    TX_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;             /* �е����ȼ� */
    TX_dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;            /* �ر�FIFOģʽ */
    TX_dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;    /* FIFO��ֵ���� */
    TX_dma_handle.Init.MemBurst = DMA_MBURST_SINGLE;               /* �洢��ͻ�����δ��� */
    TX_dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE;            /* ����ͻ�����δ��� */

    HAL_DMA_DeInit(&TX_dma_handle);
    HAL_DMA_Init(&TX_dma_handle);
}

void DMA_RX_init(DMA_Stream_TypeDef *dma_stream_handle, uint32_t ch)
{ 
	__HAL_RCC_DMA2_CLK_ENABLE();                        /* DMA2ʱ��ʹ�� */

    /* Tx DMA���� */
    RX_dma_handle.Instance = dma_stream_handle;                    /* ������ѡ�� */
    RX_dma_handle.Init.Channel = ch;                               /* DMAͨ��ѡ�� */
    RX_dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;           /* �洢�������� */
    RX_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;               /* ���������ģʽ */
    RX_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                   /* �洢������ģʽ */
    RX_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  /* �������ݳ���:8λ */
    RX_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     /* �洢�����ݳ���:8λ */
    RX_dma_handle.Init.Mode = DMA_NORMAL;                          /* ��������ģʽ */
    RX_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM;             /* �е����ȼ� */
	//HAL_DMA_DeInit(&RX_dma_handle);
	HAL_DMA_Init(&RX_dma_handle);
	
    __HAL_LINKDMA(&g_uart1_handle, hdmarx, RX_dma_handle);   /* ��DMA��USART1��ϵ����(����DMA) */
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
		HAL_UART_DMAStop(&g_uart1_handle);		/* ��������Ժ�رմ���DMA */

	// ������յ�������
    }
}


