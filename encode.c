#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;   
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // validate beautiful.bmp
    if((strstr(argv[2], ".bmp"))==NULL)
    {
        printf("\nINFO : Input file format '.bmp' not available\n");
        return e_failure;
    }
    else
    {
        if(strcmp(strstr(argv[2], ".bmp"),".bmp")==0)
        {
        printf("\nINFO : Input file format .bmp available\n");
        encInfo->src_image_fname=argv[2];
        }
    }

    // validate secret.txt
    if((strstr(argv[3], ".txt"))==NULL)
    {
        printf("\nINFO : Input file format '.txt' not available\n");
        return e_failure;
    }
    else
    {
       if(strcmp(strstr(argv[3], ".txt"),".txt")==0)
       {
        printf("\nINFO : Input file format '.txt' is available\n");
        encInfo->secret_fname=argv[3];
       }
    }

    //output file
    //check if output file is given by user or not 
    if(argv[4]!=NULL)
    {
        // output file is present:validate .bmp or not
        if(strstr(argv[4], ".bmp")==NULL)
        {   
            printf("\nINFO : Output file format .bmp not available\n");
            return e_failure;
        } 
        else
        {
            printf("\nINFO : Output file format '.bmp' format is available\n");
            encInfo->stego_image_fname=argv[4];
        }

    }
    else
    {   //output file not available, create default.bmp
        encInfo->stego_image_fname="default.bmp";
        printf("\nINFO : Output file is successfully created\n");
    }
    return e_success;
}
//calculate the secret file size
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    //printf("\nSecret file size : %ld\n",ftell(fptr));
    return ftell(fptr);
}
Status check_capacity(EncodeInfo *encInfo)
{   
    //check the pixel size
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
    //check secret file size
    encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
    // check if image can hold secret data 
    if(encInfo->image_capacity > 16 + 32 + 32+ 32+ encInfo->size_secret_file*8)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
//copy .bmp header to output file
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{  
     // declare the character array of size 54
    char arr[54];
    //bring file pointer to beginning
    rewind(fptr_src_image);
    //read 54 bytes of header information from image file
    fread(arr, 54, 1, fptr_src_image);
    //writer the header information to output file
    fwrite(arr, 54, 1, fptr_dest_image);
    return e_success;
}
//encode the lsb of image data with secret data bit and store in output
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    //encode 1 byte of secret data to 8 byte of image data
  for(int i=0;i<8;i++)
  { 
    image_buffer[i] = (image_buffer[i] & 0XFE) | ((data>>i) & (0X01));
  }
    return e_success;
  
}
Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo)
{
    for(int i=0;i<size;i++)
    {   //read 8bytes from source file
        fread(encInfo->image_data, 8, 1, encInfo->fptr_src_image);
        //perform encoding on 8 Bytes of data for 1 char
        encode_byte_to_lsb(data[i], encInfo->image_data);
        //write the encoded data in output file
        fwrite(encInfo->image_data, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}
// function definition to encode integer data
Status encode_size_to_lsb(int size, EncodeInfo *encInfo)
{  
    //declare a character array
    char str[32];
    //read 32 byte data from source image
    fread(str, 32, 1, encInfo->fptr_src_image);
    //to encode integer 32 bit in lsb of 32 byte source image
    for(int i=0;i<32;i++)
    {
        str[i] = (str[i] & 0xFE) | ((size>>i) & 0x01);
    }
    //write the encoded data to destination file
    fwrite(str, 32, 1, encInfo->fptr_stego_image);
    return e_success;

}
//Function definitipn to encode magic string
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
   encode_data_to_image(magic_string, strlen(magic_string), encInfo );
   return e_success;
}

Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(size, encInfo);
    return e_success;
}
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn, strlen(file_extn), encInfo);
    return e_success;
}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(file_size, encInfo);
     return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{   
    char data[encInfo->size_secret_file];
    rewind(encInfo->fptr_secret);
     fgets(data, encInfo->size_secret_file, encInfo->fptr_secret);
     encode_data_to_image(data, encInfo->size_secret_file, encInfo);
     e_success;
}
Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    int size;
    size = encInfo->image_capacity + 54 - ftell(encInfo->fptr_src_image);
    char remaining_data[size];
    fread(remaining_data, size, 1, encInfo->fptr_src_image);
    fwrite(remaining_data, size, 1, encInfo->fptr_stego_image);
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{  
    //open the input and output files
    if(open_files(encInfo) == e_success)
    {
        printf("\nINFO : Files opened successfully\n");
        //check the input file capacity to encode the data into image
        if(check_capacity(encInfo) ==e_success)
        {
            printf("\nINFO : Secret data can be encoded in the given image file\n");
            //copy .bmp header to the output file 
            if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
            {
                printf("\nINFO : .bmp header copied successfully\n");
                if(encode_magic_string(MAGIC_STRING, encInfo)==e_success)
                {
                    printf("\nINFO : Magic string is encoded successfully\n");
                    strcpy(encInfo->extn_secret_file,strstr(encInfo->secret_fname, "."));
                    //printf("\nSecret file extension : %s\n", encInfo->extn_secret_file);
                    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo)==e_success)
                    {
                        printf("\nINFo : Secret file extension size encoded successfully\n");
                        if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo)==e_success)
                        {
                            printf("\nINFO : Secret file extension encoded successfully\n");
                            if(encode_secret_file_size(encInfo->size_secret_file, encInfo)==e_success)
                            {
                                printf("\nINFO : Secret file size encoded successfully\n");
                                if(encode_secret_file_data(encInfo)==e_success)
                                {
                                    printf("\nINFO : Secret File data encoded successfully\n");
                                    if(copy_remaining_img_data(encInfo)==e_success)
                                    {
                                        printf("\nINFO : Remaining file data copied successfully\n");
                                    }
                                    else
                                    {
                                        printf("\nINFO : Remaining file data failed to copy\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("\nINFO : Secret file data encoding failed\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("\nINFO : Secret file size encoding failed\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("\nINFO : Secret file extension encoding failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                         printf("\nINFO: Secret file extension size encoding failed\n");
                         return e_failure;
                    }
                }
                else
                {
                    printf("\nINFO : Failed to encode magic string\n");
                    return e_failure;
                }
            }
            else
            {
                printf("\nINFO : .bmp header is not copied\n");
                return e_failure;
            }
        }
        else
        {
            printf("\nINFO : Secret data cannot be encoded in the given image\n");
        }
    }
    else
    {   
         printf("\nINFO : Failed to open files\n");
         return e_failure;
    }
return e_success;
}
