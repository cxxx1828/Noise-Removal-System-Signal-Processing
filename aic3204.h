/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 aic3204.h                                                               */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Header file for aic3204 codec on the EZDSP 5535 USB Stick.              */
/*                                                                           */
/*****************************************************************************/
/*
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
*/

#ifndef TEMPLATE_DMA_AIC3204_H_
#define TEMPLATE_DMA_AIC3204_H_


 #include "WAVheader.h"

 extern void aic3204_init();
 extern void aic3204_set_input_filename(const char* input_filename);
 extern void aic3204_set_output_filename(const char* output_filename);
 extern Uint32 set_sampling_frequency_and_gain(Uint32, Uint16);
 extern void aic3204_hardware_init(void);
 extern void aic3204_codec_read(Int16*, Int16*);
 extern void aic3204_codec_write(Int16, Int16);
 extern void aic3204_disable(void);
 extern Int16 aic3204_write_wav_header (WAV_HEADER* header);
 extern Int16 aic3204_read_wav_header (WAV_HEADER* header);


 extern FILE* input_file;
 extern FILE* output_file;
 extern Int16 aic3204_enabled;

#endif /*TEMPLATE_DMA_AIC3204_H_*/


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  End of aic3204.h                                                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 
