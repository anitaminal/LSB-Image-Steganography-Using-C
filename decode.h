#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include<string.h>

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /*stegoimage.bmp info*/
    char *encoded_image_fname;
    FILE *fptr_encoded_image;
    char encode_data[MAX_IMAGE_BUF_SIZE];
    uint size_file_extn;
    uint file_size;
    char dextn_secret_file[MAX_IMAGE_BUF_SIZE];
    /* file.txt info*/
    char *output_fname;
    FILE *fptr_output;
}DecodeInfo;

/*Decoding Function Prototype*/

/*Read and validate Decode args from argv*/
Status read_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/*Perform decoding*/
Status do_decoding(DecodeInfo *decInfo);

/*Get the file pointers for input and output files*/
Status open_decode_files(DecodeInfo *decInfo);

/*Decode magic string*/
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/*Decode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*Decode secret file extension*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/*Decode secret file size*/
 Status decode_secret_file_size(DecodeInfo *decInfo);

 /*Decode secret file data*/
 Status decode_secret_file_data(DecodeInfo *decInfo);

 /*Decode funtion*/
 Status decode_image_to_data( int size, DecodeInfo *decInfo);

 /*Decode the lsb of image data array into a byte*/
 Status decode_lsb_to_byte(DecodeInfo *decInfo);

 /*Decode lsb to size*/
 uint decode_lsb_to_size(DecodeInfo *decInfo);

 #endif
