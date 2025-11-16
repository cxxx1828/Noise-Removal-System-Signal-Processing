#ifndef WAVEHEADER_H_
#define WAVEHEADER_H_

#include <stdio.h>
#include "tistdtypes.h"

typedef struct WAV_HEADER_RIFF_
{
		char		ChunkID[4];			// "RIFF"
		Uint32		ChunkSize;			// = 36 + SubChunk2Size
		char		Format[4];			// "WAVE"
} WAV_HEADER_RIFF;

typedef struct WAV_HEADER_FMT_
{
		char	SubChunk1ID[4];		// "fmt "
		Uint32	SubChunk1Size;		// 16 for PCM
		Uint16	AudioFormat;		// PCM = 1 (i.e. Linear quantization)
		Uint16	NumChannels;
		Uint32	SampleRate;
		Uint32	ByteRate;
		Uint16	BlockAlign;
		Uint16	BitsPerSample;
 } WAV_HEADER_FMT;

typedef struct WAV_HEADER_DATA_
{
		char		SubChunk2ID[4];		// "data"
		Uint32		SubChunk2Size;
} WAV_HEADER_DATA;

typedef struct WAV_HEADER_
{
	WAV_HEADER_RIFF		riff;
	WAV_HEADER_FMT		fmt;
	WAV_HEADER_DATA		data;
	Int16				HeaderSize;
} WAV_HEADER;

extern const WAV_HEADER defaultWavHeader;

#endif /*WAVEHEADER_H_*/
