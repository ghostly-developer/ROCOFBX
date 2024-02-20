#include "LZMA/C/LzmaLib.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring> // For memcpy
#include <filesystem> // C++17 filesystem library
#include <thread>

namespace fs = std::filesystem;

std::vector<unsigned char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return {};
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return {};
    }

    return buffer;
}

bool writeFile(const std::string& filename, const std::vector<unsigned char>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    return true;
}

bool compressFile(const std::string& inputPath, const std::string& outputPath) {
    auto inputFileData = readFile(inputPath);
    if (inputFileData.empty()) return false;

    size_t uncompressedSize = inputFileData.size();
    size_t propSize = LZMA_PROPS_SIZE;
    size_t compressedDataSize = inputFileData.size() * 2 + propSize; // Extra space for compression overhead
    std::vector<unsigned char> compressedData(compressedDataSize + sizeof(size_t)); // Plus uncompressed size

    int res = LzmaCompress(&compressedData[LZMA_PROPS_SIZE + sizeof(size_t)], &compressedDataSize,
        inputFileData.data(), uncompressedSize,
        &compressedData[0] + sizeof(size_t), &propSize,
        9, 1 << 24, 3, 0, 2, 32, std::thread::hardware_concurrency());

    if (res != SZ_OK) {
        std::cerr << "Compression failed with code " << res << std::endl;
        return false;
    }

    // Prepend uncompressed size to the compressed data
    memcpy(compressedData.data(), &uncompressedSize, sizeof(size_t));

    compressedData.resize(LZMA_PROPS_SIZE + compressedDataSize + sizeof(size_t)); // Adjust final size
    return writeFile(outputPath, compressedData);
}

bool decompressFile(const std::string& inputPath, const std::string& outputPath) {
    auto inputFileData = readFile(inputPath);
    if (inputFileData.empty()) return false;

    size_t uncompressedSize = 0;
    memcpy(&uncompressedSize, inputFileData.data(), sizeof(size_t));

    std::vector<unsigned char> decompressedData(uncompressedSize);
    size_t destLen = uncompressedSize;
    size_t srcLen = inputFileData.size() - LZMA_PROPS_SIZE - sizeof(size_t);

    int res = LzmaUncompress(decompressedData.data(), &destLen,
        inputFileData.data() + LZMA_PROPS_SIZE + sizeof(size_t), &srcLen,
        inputFileData.data() + sizeof(size_t), LZMA_PROPS_SIZE);

    if (res != SZ_OK) {
        std::cerr << "Decompression failed with code " << res << std::endl;
        return false;
    }

    decompressedData.resize(destLen); // Ensure vector size matches decompressed data size
    return writeFile(outputPath, decompressedData);
}

int main() {
    std::cout << "Enter operation (compress or decompress): ";
    std::string operation;
    std::cin >> operation;

    // Consume any newline characters left in the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    fs::path inputFilePath, outputFilePath;
    std::cout << "Enter input filename (with extension): ";

    // Use getline to read the path, allowing spaces to be included
    std::string inputPath;
    std::getline(std::cin, inputPath);
    inputFilePath = inputPath;

    if (operation == "compress") {
        std::cout << "Compressing File..." << std::endl;
        outputFilePath = inputFilePath;
        outputFilePath += ".lzma";
        if (!compressFile(inputFilePath.string(), outputFilePath.string())) {
            std::cerr << "Compression operation failed." << std::endl;
            return 1;
        }
    }
    else if (operation == "decompress") {
        outputFilePath = inputFilePath;
        outputFilePath.replace_extension(""); // Removes .lzma and assumes the original extension was correct
        if (!decompressFile(inputFilePath.string(), outputFilePath.string())) {
            std::cerr << "Decompression operation failed." << std::endl;
            return 1;
        }
    }
    else {
        std::cerr << "Invalid operation selected." << std::endl;
        return 1;
    }

    std::cout << "Operation completed successfully. Output file: " << outputFilePath << std::endl;
    return 0;
}
