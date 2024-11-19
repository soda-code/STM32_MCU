#ifndef __DMA_H
#define	__DMA_H

#include "./SYSTEM/sys/sys.h"


extern DMA_HandleTypeDef  TX_dma_handle;                  /* DMAæ‰±˙ */
extern DMA_HandleTypeDef  RX_dma_handle;                  /* DMAæ‰±˙ */

void DMA_TX_init(DMA_Stream_TypeDef *dma_stream_handle, uint32_t ch);  /* ≈‰÷√DMAx_CHx */
void DMA_RX_init(DMA_Stream_TypeDef *dma_stream_handle, uint32_t ch);  /* ≈‰÷√DMAx_CHx */

#endif






























