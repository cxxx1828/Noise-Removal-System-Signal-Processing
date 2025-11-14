#include "stdio.h"
#include "ezdsp5535.h"
#include "ezdsp5535_i2c.h"
#include "aic3204.h"
#include "ezdsp5535_aic3204_dma.h"
#include "ezdsp5535_i2s.h"
#include "WAVheader.h"
#include "fir.h"
#include "iir.h"
#include "parametri.h"
#include "irr_coef.h"

static WAV_HEADER outputWAVhdr;
static WAV_HEADER inputWAVhdr;

#define SAMPLE_RATE 48000L
#define GAIN 0

#pragma DATA_ALIGN(InputBufferL,4)
Int16 InputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(InputBufferR,4)
Int16 InputBufferR[AUDIO_IO_SIZE];

#pragma DATA_ALIGN(OutputBufferL,4)
Int16 OutputBufferL[AUDIO_IO_SIZE];
#pragma DATA_ALIGN(OutputBufferR,4)
Int16 OutputBufferR[AUDIO_IO_SIZE];


Int16 historyL[129];
Int16 historyR[129];
//Int16 history[77];//35 , 77
//Int16 historyx[2];
//Int16 historyy[2];
//Int16 historyx2[2];
//Int16 historyy2[2];
Uint16 stateL=0 ;
Uint16 stateR=0 ;
Uint16 n_coeff=129;//35, 77
//Int16 historyx[2][2];
//Int16 historyy[2][2];
//Int16 historyx2[2][2];
//Int16 historyy2[2][2];
//Int16 historyx[3][2];
//Int16 historyy[3][2];
//Int16 historyx2[3][2];
//Int16 historyy2[3][2];



void main( void )
{
	int i, j; //k
	Int32 number_of_blocks;

	EZDSP5535_init( );

	aic3204_hardware_init();

	aic3204_set_input_filename("../22.wav");
	aic3204_set_output_filename("../f.wav");

	aic3204_init();

	aic3204_dma_init();

	aic3204_read_wav_header(&inputWAVhdr);

	outputWAVhdr = inputWAVhdr;

	number_of_blocks = inputWAVhdr.data.SubChunk2Size/
				(inputWAVhdr.fmt.NumChannels*inputWAVhdr.fmt.BitsPerSample*AUDIO_IO_SIZE/8);

	aic3204_write_wav_header(&outputWAVhdr);

				//2 , 35 , 77, 129,
	for (i = 0; i < 129; i++) {//broj filtera drugog reda
		historyL[i] = 0;
		historyR[i] = 0;
		//history[i] = 0;
		//historyx[i] = 0;
		//historyy[i] = 0;
		//historyx2[i] = 0;
		//historyy2[i] = 0;
		//for(k=0;k<2;k++){//broj historyja
			//historyx[i][k] = 0;
			//historyy[i][k] = 0;
			//historyx2[i][k] = 0;
			//historyy2[i][k] = 0;
		//}
	}


	for(i = 0; i < number_of_blocks; ++i)
	{
		aic3204_read_block(InputBufferL, InputBufferR);

		for(j = 0; j < AUDIO_IO_SIZE; j++)
		{
			//OutputBufferL[j] = fir_basic(InputBufferL[j], FIRCoef, history, n_coeff);
			//OutputBufferR[j] = fir_basic(InputBufferR[j], FIRCoef, history, n_coeff);
			//OutputBufferL[j]=fir_circular(InputBufferL[j], fir77, history, n_coeff, &stateL);
			//OutputBufferR[j]=fir_circular(InputBufferR[j], fir77, history, n_coeff, &stateR);
			//OutputBufferL[j] = second_order_IIR(InputBufferL[j], coef_iir, historyx, historyy);
			//OutputBufferR[j] = second_order_IIR(InputBufferR[j], coef_iir, historyx2, historyy2);
			OutputBufferL[j]=fir_circular(InputBufferL[j], FIR129, historyL, n_coeff, &stateL);
			OutputBufferR[j]=fir_circular(InputBufferR[j], FIR129, historyR, n_coeff, &stateR);

			//OutputBufferL[j] = fourth_order_IIR(InputBufferL[j], coef_iir_4th_order, historyx, historyy);
			//OutputBufferR[j] = fourth_order_IIR(InputBufferR[j], coef_iir_4th_order, historyx2, historyy2);

			//OutputBufferL[j] = Nth_order_IIR(InputBufferL[j], coef_iir_6th_order, historyx, historyy,6);
			//OutputBufferR[j] = Nth_order_IIR(InputBufferR[j], coef_iir_6th_order, historyx2, historyy2,6);

		}
		aic3204_write_block(OutputBufferL, OutputBufferR);

	}
	/* Disable I2S and put codec into reset */
    aic3204_disable();

    printf( "\n***Program has Terminated***\n" );
	SW_BREAKPOINT;
}

