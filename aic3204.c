/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 aic3204.c                                                               */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Setup functions for aic3204 codec on the EZDSP 5535 USB Stick.          */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Dejan Bokan                                                   */
/*****************************************************************************/
#include <stdio.h>
#include "tistdtypes.h"
#include "WAVheader.h"

FILE* input_file;
FILE* output_file;

Int16 aic3204_enabled = 0;

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  aic3204_enable( )                                                       *
 *                                                                          *
 * ------------------------------------------------------------------------ */  

void aic3204_hardware_init(void)
{
    //Dummy for now
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  aic3204_disable( )                                                      *
 *                                                                          *
 * ------------------------------------------------------------------------ */

void aic3204_disable(void)
{
	aic3204_enabled = 0;
	if(input_file != 0)
		fclose(input_file);
	if(output_file != 0)
		fclose(output_file);
}



/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  aic3204_codec_read( )                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */

void aic3204_codec_read(Int16* left_input, Int16* right_input)
{
	Int16 n = 0;
	Int16 buff[2];
	if(aic3204_enabled)
	{
		n = fread(buff, 1, 2, input_file);
		if(n == 2)
		{
			*left_input = buff[0] | (buff[1] << 8);
			n = fread(buff, 1, 2, input_file);
			if(n == 2)
			{
				*right_input = buff[0] | (buff[1] << 8);
				return;
			}

		}
	}

	*left_input = 0;
	*right_input = 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  aic3204_codec_write( )                                                  *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 
void aic3204_codec_write(Int16 left_output, Int16 right_output)
{
	Int16 buff[2];
	if(aic3204_enabled)
	{
		buff[0] = left_output & 0x00FF;
		buff[1] = left_output >> 8;
		fwrite(buff, 1, 2, output_file);

		buff[0] = right_output & 0x00FF;
		buff[1] = right_output >> 8;
		fwrite(buff, 1, 2, output_file);
	}
}

static void write_uint16(Uint16 data)
{
	Int16 buff[2];
	buff[0] = data & 0x00FF;
	buff[1] = data >> 8;
	fwrite(buff, 1, 2, output_file);
}

static void write_uint32(Uint32 data)
{
	write_uint16((Uint16)(data & 0xFFFF));
	write_uint16((Uint16)(data >> 16) );
}


static void read_uint16(Uint16* data)
{
	Int16 buff[2];
	fread(buff, 1, 2, input_file);
	*data = (buff[1] << 8) & 0xFF00;
	*data += buff[0];
}

static void read_uint32(Uint32* data)
{
	read_uint16(((Uint16*)data)+1);
	read_uint16((Uint16*)data);
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  aic3204_codec_write( )                                                  *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 aic3204_write_wav_header (WAV_HEADER* header)
{
	if(output_file == NULL)

	{
		printf("ERROR: Invalid output file!\n");
		return -1;
	}

	fwrite(header->riff.ChunkID, 1, 4, output_file);
	write_uint32(header->riff.ChunkSize);
	fwrite(header->riff.Format, 1, 4, output_file);

	fwrite(header->fmt.SubChunk1ID, 1, 4, output_file);
	write_uint32(header->fmt.SubChunk1Size);
	write_uint16(header->fmt.AudioFormat);
	write_uint16(header->fmt.NumChannels);
	write_uint32(header->fmt.SampleRate);
	write_uint32(header->fmt.ByteRate);
	write_uint16(header->fmt.BlockAlign);
	write_uint16(header->fmt.BitsPerSample);

	fwrite(header->data.SubChunk2ID, 1, 4, output_file);
	write_uint32(header->data.SubChunk2Size);

	return 0;
}

Int16 aic3204_read_wav_header (WAV_HEADER* header)
{
	if(input_file == NULL)

	{
		printf("ERROR: Invalid input file!\n");
		return -1;
	}

	fread(header->riff.ChunkID, 1, 4, input_file);
	read_uint32(&(header->riff.ChunkSize));
	fread(header->riff.Format, 1, 4, input_file);

	fread(header->fmt.SubChunk1ID, 1, 4, input_file);
	read_uint32(&header->fmt.SubChunk1Size);
	read_uint16(&header->fmt.AudioFormat);
	read_uint16(&header->fmt.NumChannels);
	read_uint32(&header->fmt.SampleRate);
	read_uint32(&header->fmt.ByteRate);
	read_uint16(&header->fmt.BlockAlign);
	read_uint16(&header->fmt.BitsPerSample);

	fread(header->data.SubChunk2ID, 1, 4, input_file);
	read_uint32(&header->data.SubChunk2Size);

	return 0;
}



/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  End of aic3204.c                                                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
