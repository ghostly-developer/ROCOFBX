#include "./LZMA/C/LzmaLib.h"
#include <iostream>
#include <fstream>
#include <vector>

// Example function to read the entire contents of a file into a vector
std::vector<unsigned char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Example function to write the compressed data to a file
void writeFile(const std::string& filename, const std::vector<unsigned char>& data) {
    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

int main() {
    const std::string inputFilename = "input.fbx";
    const std::string outputFilename = "output.fbx.lzma";

    std::vector<unsigned char> inputFileData = readFile(inputFilename);

    // Placeholder for the compressed data (make sure it's large enough)
    std::vector<unsigned char> compressedData(inputFileData.size() * 2); // Simplistic size allocation

    size_t propSize = LZMA_PROPS_SIZE;
    size_t compressedDataSize = compressedData.size();
    int res = LzmaCompress(&compressedData[0], &compressedDataSize, &inputFileData[0], inputFileData.size(),
        &compressedData[LZMA_PROPS_SIZE], &propSize,
        -1, 0, -1, -1, -1, -1, -1);

    if (res != SZ_OK) {
        std::cerr << "Compression failed with code " << res << std::endl;
        return 1;
    }

    // Adjust the size of the compressed data vector to actual data size
    compressedData.resize(compressedDataSize + LZMA_PROPS_SIZE);

    writeFile(outputFilename, compressedData);

    std::cout << "Compression completed successfully." << std::endl;

    return 0;
}
