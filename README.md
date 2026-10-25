# LSB-Image-Steganography-Using-C

LSB (Least Significant Bit) Image Steganography is a technique used to hide secret information within an image without causing noticeable visual changes.

This project implements image steganography using the C programming language. It hides secret text data within the least significant bits (LSBs) of the pixel data in a BMP image. Since modifying the LSBs typically results in minimal changes to pixel values, the hidden information is not easily noticeable to the human eye.

The application supports both encoding and decoding operations through command-line arguments. A magic string is used to identify images containing hidden data.

## Features

- Encode a text file (`.txt`) into a BMP image (`.bmp`).
- Decode hidden text data from an encoded BMP image.
- Validate input files and command-line arguments.
- Check whether the BMP image has sufficient capacity to store the secret data.
- Use a magic string (`#*`) to identify encoded image files.
- Preserve the BMP image header and remaining image data during encoding.

## Requirements

- Accept BMP (`.bmp`) files as cover images.
- Accept text (`.txt`) files as secret data.
- Verify that the cover image has sufficient capacity to hold the secret data.
- Use the magic string `#*` to identify encoded data.
- Support both encoding and decoding operations.
- Accept operation options and file names through command-line arguments.

## Prerequisites

- C Programming
- File Handling (File I/O)
- Pointers
- User-defined data types: `struct`, `typedef`, and `enum`
- Bitwise Operations and Bit Manipulation
- Command-line Arguments

## Validation

- Validate the command-line arguments.
- Identify whether the requested operation is encoding or decoding.
- Validate the required input and output files for the selected operation.
- Check whether the input files can be opened successfully.
- Verify that the cover image has sufficient capacity for the secret data.
- Validate the magic string and secret file extension during decoding.
- Handle errors at each stage of the encoding and decoding processes.

## Encoding Process

1. Open the input BMP image and secret text file in read mode, and create the output BMP file in write mode.
2. Validate the input files and check whether the BMP image can accommodate the secret data.
3. Copy the 54-byte BMP header to the output image.
4. Encode the magic string (`#*`) into the image data.
5. Encode the size of the secret text file extension.
6. Encode the secret file extension (`.txt`).
7. Encode the secret file size.
8. Encode the secret message data into the LSBs of the image bytes.
9. Copy the remaining image data to the output BMP file.

## Decoding Process

1. Open the encoded BMP image and create the output text file.
2. Decode the magic string and verify that it matches the expected value (`#*`).
3. Decode the size of the secret file extension.
4. Decode and validate the secret file extension.
5. Decode the size of the secret data.
6. Extract the secret message from the image by reading the encoded LSBs.
7. Write the recovered secret data to the output text file.

## Key Challenges and Learnings

- Implementing bitwise operations to encode secret data into the least significant bits of image bytes.
- Extracting individual bits from image data to reconstruct the original secret message.
- Calculating image capacity and verifying that sufficient space is available for the secret data and encoding metadata.
- Handling file operations and validating inputs at each stage.
- Understanding how to preserve the BMP header and remaining image data while embedding secret information.
- Implementing command-line argument handling and error management in C.

## Conclusion

This project  provides hands-on experience in implementing LSB-based data hiding and recovering hidden information from BMP images.
