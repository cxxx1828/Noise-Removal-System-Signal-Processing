#ifndef MY_DMA_PING_PONG_REGISTER_SETUP_H_
#define MY_DMA_PING_PONG_REGISTER_SETUP_H_

#define AUDIO_IO_SIZE  128		// DMA transfer size
#define PING_PONG_SIZE (2 * AUDIO_IO_SIZE)

void aic3204_dma_init(void);
void aic3204_read_block(Int16* buffer_left, Int16* buffer_right);
void aic3204_write_block(Int16* buffer_left, Int16* buffer_right);
void aic3204_write_data(Int16* buffer, Int16 length);

#endif /*MY_DMA_PING_PONG_REGISTER_SETUP_H_*/
