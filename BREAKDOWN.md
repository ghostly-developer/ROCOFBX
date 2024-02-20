
# Overview

This document provides an overview of RO:CO designed for file compression and decompression using the LZMA algorithm.

## Program Structure

The program is structured into several key functions:

- `main`: The entry point of the program. It handles user input to determine whether to compress or decompress a file and specifies the file paths.
- `readFile`: Reads the contents of a file into a byte array (`std::vector<unsigned char>`). It opens a file in binary mode and reads it into memory.
- `writeFile`: Writes the contents of a byte array to a file. It is used to save the compressed or decompressed data to disk.
- `compressFile`: Handles the compression of file data using the LZMA algorithm. It takes input and output file paths as arguments, reads the input file, compresses its contents, and writes the compressed data to the output file (.rco file).
- `decompressFile`: Manages the decompression of .rco compressed data. Similar to `compressFile`, it reads the compressed file, decompresses it, and writes the output to a specified path.

## Compression and Decompression

- **Compression**: The `compressFile` function calls `LzmaCompress` from the LZMA SDK, passing parameters that control the compression algorithm's behavior, such as compression level and dictionary size.
- **Decompression**: The `decompressFile` function uses `LzmaUncompress` to reverse the compression process, restoring the original file contents from the compressed data.

## User Interaction

- The program prompts the user to choose between compression and decompression modes and to specify the input and output file paths.
- Error handling is included to manage issues like file read/write errors and compression/decompression failures.

## Dependencies

- LZMA SDK: The program relies on the LZMA SDK for compression and decompression functionalities.

