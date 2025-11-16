/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/*   aic3204_init.c                                                          */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Initialisation of the AIC3204 Codec.                                    */
/*                                                                           */
/*   Sets up the codec for line inputs.                                      */
/*   Uses a sampling frequency of 48000 Hz.                                  */ 
/*                                                                           */
/*   Author  : Dejan Bokan                                                   */
/*                                                                           */
/*****************************************************************************/


#include "stdio.h"
#include "tistdtypes.h"
#include "aic3204.h"

#define AIC3204_SAMPLE_SIZE 16

#define DEFAULT_INPUT_FILENAME "input.wav"
#define DEFAULT_OUTPUT_FILENAME "output.wav"

 static const char* input_filename = 0;
 static const char* output_filename = 0;

 void aic3204_set_input_filename(const char* filename)
 {
	 input_filename = filename;
 }

 void aic3204_set_output_filename(const char* filename)
 {
	 output_filename = filename;
 }
 
 /* Setup: Line input. Fs = 48000 Hz */ 
void aic3204_init()
{
    if(input_filename != 0)
    {
    	input_file = fopen(input_filename, "rb");
    }
    else
    {
    	input_file = fopen(DEFAULT_INPUT_FILENAME, "rb");
    }
    if((input_file == 0))
    {
    	printf("\nAIC3204 ERROR: Invalid input filename:%s\n", input_filename);
    	return;
    }

    if(output_filename != 0)
    {
    	output_file = fopen(output_filename, "wb");
    }
    else
    {
    	output_file = fopen(DEFAULT_OUTPUT_FILENAME, "rb");
    }

    if((output_file == 0))
    {
        printf("\nAIC3204 ERROR: Invalid output filename: %s\n", output_filename);
    	return;
    }

    aic3204_enabled = 1;

}

Uint32 set_sampling_frequency_and_gain(Uint32 SamplingFrequency, Uint16 ADCgain)
{
	//Dummy for now
	return 0;
}


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  End of aic3204_init.c                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */





