#include<stdio.h>
#include "decode.h"
#include "types.h"
#include "encode.h"
#include "common.h"


Status open_decode_files(DecodeInfo *decInfo)
{
    //encoded file 
    decInfo->fptr_encoded_image=fopen(decInfo->encoded_image_fname, "r");
    if(decInfo->fptr_encoded_image==NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->encoded_image_fname);
        return e_failure;
    }
    //decoded output file
    decInfo->fptr_output=fopen(decInfo->output_fname, "w+");
    if(decInfo->fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_fname);
        return e_failure;
    }
    return e_success;

}

Status read_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //validate encoded image file
    if((strstr(argv[2], ".bmp"))==NULL)
    {
        printf("\nINFO : Input file format '.bmp' not available\n");
        return e_failure;
    }
    else
    {
        if(strcmp(strstr(argv[2], ".bmp"), ".bmp")==0)
        {
            printf("\nINFO :  Input file format '.bmp' is available\n");
            decInfo->encoded_image_fname=argv[2];
        }
    }
    //validate output decoded file
if(argv[3]!=NULL)
{
    if((strstr(argv[3], ".txt"))==NULL)
    {
        printf("\nINFO : Decoded output file format '.txt' not available\n");
        return e_failure;
    
    }
    else
    {
        if(strcmp(strstr(argv[3], ".txt"), ".txt")==0)
        {
            printf("\nINFO : Decoded output file format '.txt' is available\n");
            decInfo->output_fname=argv[3];
        }
    }
}
else
{
    decInfo->output_fname="default.txt";
    printf("\nINFO : Output file is created successfully\n");
}
return e_success;

}
Status decode_lsb_to_byte( DecodeInfo *decInfo)
{
    uint  data=0;
    uint  lsb;
    for(int i=0;i<8;i++)
    {
     lsb = decInfo->encode_data[i] & 0x01;
     data = data | (lsb<<i);   
    }
   // printf("\n%c", data);
    fprintf(decInfo->fptr_output, "%c", data);
    return e_success;
}
Status decode_image_to_data( int size, DecodeInfo *decInfo)
{
    for(int i=0 ; i<size ; i++)
   {
     //read 8 bytes from encoded input file 
      fread(decInfo->encode_data, 8, 1, decInfo->fptr_encoded_image);
      //perform decoding on 8 bytes of data to fetch 1 char 
      decode_lsb_to_byte(decInfo);

   }
   
   return e_success;
}
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    int ret=0;
    char ch;
     fseek(decInfo->fptr_encoded_image, 54, SEEK_SET);
    decode_image_to_data(strlen(magic_string), decInfo);
    rewind(decInfo->fptr_output);
    //validate magic string
    for(int i=0;i<strlen(magic_string);i++)
    {   
        fscanf(decInfo->fptr_output, "%c", &ch);
       // printf("\nMagic string: %c", ch);
        if(magic_string[i]==ch)
        {
            ret++;
        }
        
    }
    rewind(decInfo->fptr_output);
    if((ret==strlen(magic_string)))
    {
        printf("\nMagic String decoded is verified successfully");
    }
    else
    {
        printf("\nMagic string decoded is not verified successfully");
        return e_failure;
    }
    return e_success;
}
uint decode_lsb_to_size(DecodeInfo *decInfo)
{
    char str[32];
    uint size =0;
    uint lsb_size;
    fread(str, 32, 1, decInfo->fptr_encoded_image);
    for(int i=0 ; i<32 ; i++)
    {
        lsb_size = str[i] & 0x01;
        size = size | (lsb_size << i);
    }
   // printf("\n%d\n", size);
    return size;
}
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    decInfo->size_file_extn = decode_lsb_to_size(decInfo);
    //printf("\nSecret File extension size: %d", decInfo->size_file_extn);
    return e_success;
}
Status decode_secret_file_extn(DecodeInfo *decInfo)
{   
    int res=0;
    char ch;
    strcpy(decInfo->dextn_secret_file, strstr(decInfo->output_fname, "."));
    decode_image_to_data( decInfo->size_file_extn,decInfo);
    rewind(decInfo->fptr_output);
    //validate secret file extension
    for(int i=0;i<decInfo->size_file_extn;i++)
    {
        fscanf(decInfo->fptr_output, "%c", &ch);
        if(decInfo->dextn_secret_file[i]==ch)
        {
            res++;
        }
    }
    rewind(decInfo->fptr_output);
    if(res==decInfo->size_file_extn)
    {
        printf("\nSecret file extension decoded is validated successfully");
    }
    else
    {
        printf("\nThe decoded secret file extension validation failed");
        return e_failure;
    }
    return e_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decInfo->file_size = decode_lsb_to_size(decInfo);
    //printf("\nSEcret file size : %d", decInfo->file_size);
    return e_success;
}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    decode_image_to_data( decInfo->file_size, decInfo);

    return e_success;
}
Status do_decoding(DecodeInfo *decInfo)
{
     if(open_decode_files(decInfo)==e_success)
     {
        printf("\nINFO : Files are opened successfully\n");
        if(decode_magic_string(MAGIC_STRING, decInfo)==e_success)
        {
            printf("\nINFO : Magic string is decoded successfully\n");
            if(decode_secret_file_extn_size(decInfo)==e_success)
            {
                printf("\nINFO : File extension size is decoded successfully\n");
                if(decode_secret_file_extn(decInfo)==e_success)
                {
                    printf("\nINFO : File extension decoded successfully\n");
                    if(decode_secret_file_size(decInfo)==e_success)
                    {
                        printf("\nINFO : Secret file size decoded successfully\n");
                        if(decode_secret_file_data(decInfo)==e_success)
                        {
                            printf("\nINFO : Secret file data decoded successfully\n");
                        }
                        else
                        {
                            printf("\nINFO : Secret file data decoding failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("\nINFO : Secret file size decoding failed\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("\nINFO : File extension decoding failed\n");
                    return e_failure;
                }
            
            }
            else
            {
                printf("\nINFO : File extension size decoding failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("\nINFO : Magic string decoding is failure\n");
            return e_failure;
        }
     }
     else
     {
        printf("\nINFO : Failed to open files\n");
        return e_failure;
     }
     return e_success;

}