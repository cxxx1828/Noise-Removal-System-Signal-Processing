/*  ============================================================================
 *   Author: Nathan Zorndorf 
 *   Description: Setting up the DMA in Ping Pong mode by writing directly to registers. 
 */
#include "tistdtypes.h"
#include <stdio.h>
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"

#pragma DATA_ALIGN(SimulatorInput, 4)
Int16 SimulatorInput[AUDIO_IO_SIZE*4];
#pragma DATA_ALIGN(SimulatorOutput, 4)
Int16 SimulatorOutput[AUDIO_IO_SIZE*4];

void aic3204_dma_init(void)
{
}

void aic3204_read_block(Int16* buffer_left, Int16* buffer_right)
{
	int i;
	int n = fread(SimulatorInput, 1, AUDIO_IO_SIZE*4, input_file);
	n >>= 1;
	for(i = 0; i < AUDIO_IO_SIZE; i++)
	{
		if(i < n)
		{
			buffer_left[i] = SimulatorInput[4*i] | (SimulatorInput[4*i+1]<<8);
			buffer_right[i] = SimulatorInput[4*i+2] | (SimulatorInput[4*i+3]<<8);
		} else
		{
			buffer_left[i] = 0;
			buffer_right[i] = 0;
		}
	}
}

void aic3204_write_block(Int16* buffer_left, Int16* buffer_right)
{	
	int i;
	for(i = 0; i < AUDIO_IO_SIZE; i++)
	{
		SimulatorOutput[4*i] = buffer_left[i] & 0x00FF;
		SimulatorOutput[4*i+1] = buffer_left[i] >> 8;
		SimulatorOutput[4*i+2] = buffer_right[i] & 0x00FF;
		SimulatorOutput[4*i+3] = buffer_right[i] >> 8;
	}
	fwrite(SimulatorOutput, sizeof(Int16), AUDIO_IO_SIZE*4, output_file);
}
