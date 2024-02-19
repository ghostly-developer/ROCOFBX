#include "LZMA/C/LzmaLib.h" 
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Function to read the entire contents of a file into a vector
std::vector<unsigned char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Function to write the data to a file
void writeFile(const std::string& filename, const std::vector<unsigned char>& data) {
    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

int main() {
    std::cout << "Choose an option:\n1. Compress\n2. Decompress\nOption: ";
    int option;
    std::cin >> option;
    std::cin.ignore(); // Ignore newline character left in the input buffer

    std::string inputFilename, outputFilename;
    std::cout << "Enter the path to the input file: ";
    std::getline(std::cin, inputFilename);

    std::vector<unsigned char> inputFileData = readFile(inputFilename);

    if (option == 1) { // Compression
        outputFilename = inputFilename + ".lzma";
        std::vector<unsigned char> compressedData(inputFileData.size() * 2); // Allocation for compressed data

        size_t propSize = LZMA_PROPS_SIZE;
        size_t compressedDataSize = compressedData.size();
        int res = LzmaCompress(&compressedData[0], &compressedDataSize, &inputFileData[0], inputFileData.size(),
            &compressedData[LZMA_PROPS_SIZE], &propSize,
            -1, 0, -1, -1, -1, -1, -1);

        if (res != SZ_OK) {
            std::cerr << "Compression failed with code " << res << std::endl;
            return 1;
        }

        compressedData.resize(compressedDataSize + LZMA_PROPS_SIZE);
        writeFile(outputFilename, compressedData);
        std::cout << "Compression completed successfully.\n";
    }
    else if (option == 2) { // Decompression
        outputFilename = inputFilename.substr(0, inputFilename.size() - 5); // Assuming the original file has a .lzma extension
        size_t uncompressedSize = inputFileData.size() * 20; // Estimate the uncompressed size (needs adjustment based on your data)
        std::vector<unsigned char> decompressedData(uncompressedSize);

        size_t destLen = decompressedData.size();
        size_t srcLen = inputFileData.size() - LZMA_PROPS_SIZE;
        int res = LzmaUncompress(&decompressedData[0], &destLen,
            &inputFileData[LZMA_PROPS_SIZE], &srcLen,
            &inputFileData[0], LZMA_PROPS_SIZE);

        if (res != SZ_OK) {
            std::cerr << "Decompression failed with code " << res << std::endl;
            return 1;
        }

        decompressedData.resize(destLen);
        writeFile(outputFilename, decompressedData);
        std::cout << "Decompression completed successfully.\n";
    }
    else {
        std::cerr << "Invalid option selected." << std::endl;
        return 1;
    }

    std::cout << "Output file: " << outputFilename << std::endl;

    return 0;
}
