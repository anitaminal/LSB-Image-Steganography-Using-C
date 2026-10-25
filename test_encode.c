#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    EncodeInfo E1;
    DecodeInfo D1;
    //validate command line argument count
    if(argc<3)
    {
        printf("Invalid input\n");
        printf("For encoding: \n./a.out -e beautiful.bmp secret.txt [stego-bmp]\n");
        printf("For decoding: \n ./a.out -d stego.bmp [decode.txt]\n");
        return -1;
    }
    //check command line argument index 1 to perform encoding/decoding
    int ret = check_operation_type(argv);
    {
        if(ret==e_encode)
        {
           printf("Selected Encoding\n");
           //validate the encode arguments
           if(read_and_validate_encode_args(argv, &E1)==e_success)
           {
            printf("\nINFO : Read and validate encode args are success\n");
            //perform encoding
            if((do_encoding(&E1))==e_success)
            {
                printf("\nINFO : Encoding is success\n");
            }
            else
            {
                printf("\nFailed to encode\n");
                return 0;
            }
           }
           else
           {
            printf("\nERROR: Read and validate encode args are failure\n ");
            return 0;
           }
        }
        else if (ret==e_decode)
        {
            printf("Selected Decoding\n");
            //validate the decode arguments
            if(read_validate_decode_args(argv, &D1)==e_success)
            {
                printf("\nINFO : Read and validate decode args are success\n");
                if(do_decoding(&D1)==e_success)
                {
                    printf("\nINFO : Decoding is done successfully");
                }
                else
                {
                    printf("\nFailed to decode");
                    return 0;
                }
            }
            
            else
            {
                printf("\nINFO: Failed to read and validate decode args\n");
                return 0;
            }
        }
        else
        {
            printf("Invalid Input\n");
        }
    }
    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1], "-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}


