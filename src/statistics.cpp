#include "statistics.hpp"
#include <iostream>
#include <iomanip>
#include <string>

Statistics::Statistics() 
    : inputFile(""), outputFile(""), originalSize(0), compressedSize(0), 
      compressionRatio(0.0), hasCompressionData(false) {}

void Statistics::setCompressionStats(const std::string& input, const std::string& output, 
                                    long long original, long long compressed) {
    inputFile = input;
    outputFile = output;
    originalSize = original;
    compressedSize = compressed;
    compressionRatio = (100.0 * compressed / original);
    hasCompressionData = true;  // ← Mark that we have data
}

void Statistics::setDecompressionStats(const std::string& input, const std::string& output, 
                                      long long decompressed) {
    inputFile = input;
    outputFile = output;
    compressedSize = decompressed;
}

void Statistics::displayCompressionStats() {
    std::cout << "\n########################################" << std::endl;
    std::cout << "#     COMPRESSION STATISTICS          #" << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "# Input File:      " << std::setw(20) << std::left << (inputFile + " #") << std::endl;
    std::cout << "# Output File:     " << std::setw(20) << std::left << (outputFile + " #") << std::endl;
    std::cout << "# Original Size:   " << std::setw(19) << std::left << (std::to_string(originalSize) + " bytes #") << std::endl;
    std::cout << "# Compressed Size: " << std::setw(19) << std::left << (std::to_string(compressedSize) + " bytes #") << std::endl;
    
    std::string ratioStr = std::to_string(compressionRatio).substr(0, 5) + "%";
    std::cout << "# Compression Ratio: " << std::setw(17) << std::left << (ratioStr + " #") << std::endl;
    
    long long spaceSaved = originalSize - compressedSize;
    std::cout << "# Space Saved:     " << std::setw(20) << std::left << (std::to_string(spaceSaved) + " bytes #") << std::endl;
    std::cout << "########################################\n" << std::endl;
}

void Statistics::displayDecompressionStats() {
    std::cout << "\n########################################" << std::endl;
    std::cout << "#    DECOMPRESSION SUCCESSFUL         #" << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "# Input File:      " << std::setw(20) << std::left << (inputFile + " #") << std::endl;
    std::cout << "# Output File:     " << std::setw(20) << std::left << (outputFile + " #") << std::endl;
    std::cout << "# Decompressed Size: " << std::setw(17) << std::left << (std::to_string(compressedSize) + " bytes #") << std::endl;
    std::cout << "########################################\n" << std::endl;
}

void Statistics::displayStoredStats() {  // ← NEW METHOD
    if (!hasCompressionData) {
        std::cout << "\n########################################" << std::endl;
        std::cout << "# No compression statistics yet       #" << std::endl;
        std::cout << "# Compress a file first!              #" << std::endl;
        std::cout << "########################################\n" << std::endl;
        return;
    }
    
    std::cout << "\n########################################" << std::endl;
    std::cout << "#   LAST COMPRESSION STATISTICS       #" << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "# Input File:      " << std::setw(20) << std::left << (inputFile + " #") << std::endl;
    std::cout << "# Output File:     " << std::setw(20) << std::left << (outputFile + " #") << std::endl;
    std::cout << "# Original Size:   " << std::setw(19) << std::left << (std::to_string(originalSize) + " bytes #") << std::endl;
    std::cout << "# Compressed Size: " << std::setw(19) << std::left << (std::to_string(compressedSize) + " bytes #") << std::endl;
    
    std::string ratioStr = std::to_string(compressionRatio).substr(0, 5) + "%";
    std::cout << "# Compression Ratio: " << std::setw(17) << std::left << (ratioStr + " #") << std::endl;
    
    long long spaceSaved = originalSize - compressedSize;
    std::cout << "# Space Saved:     " << std::setw(20) << std::left << (std::to_string(spaceSaved) + " bytes #") << std::endl;
    std::cout << "########################################\n" << std::endl;
}

void Statistics::displayProjectInfo() {
    std::cout << "\n########################################" << std::endl;
    std::cout << "#        PROJECT INFORMATION          #" << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "# Project: Huffman Text Compression   #" << std::endl;
    std::cout << "# Algorithm: Huffman Coding           #" << std::endl;
    std::cout << "# File Extension: .huff               #" << std::endl;
    std::cout << "# Supported Files: Text Files         #" << std::endl;
    std::cout << "# Features:                           #" << std::endl;
    std::cout << "#   • User Authentication             #" << std::endl;
    std::cout << "#   • File Compression                #" << std::endl;
    std::cout << "#   • File Decompression              #" << std::endl;
    std::cout << "#   • Compression Statistics          #" << std::endl;
    std::cout << "########################################\n" << std::endl;
}