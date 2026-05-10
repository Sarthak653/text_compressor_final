#include "statistics.hpp"

#include <iomanip>
#include <iostream>

void Statistics::setCompressionStats(const std::string& input, const std::string& output,
                                      long long original, long long compressed) {
    inputFile      = input;
    outputFile     = output;
    originalSize   = original;
    compressedSize = compressed;
    hasData        = true;
}

void Statistics::setDecompressionStats(const std::string& input, const std::string& output) {
    // Sizes are already set from the compression step — only update file paths
    inputFile  = input;
    outputFile = output;
}

void Statistics::displayStats() const {
    if (!hasData) {
        std::cout << "No compression statistics yet. Compress a file first!\n";
        return;
    }

    // Guard against division by zero on empty original
    if (originalSize == 0) {
        std::cout << "Original file was empty.\n";
        return;
    }

    const double originalKB  = originalSize   / 1024.0;
    const double compressedKB = compressedSize / 1024.0;
    // Negative means the compressed file is larger (can happen with tiny or random input)
    const double reduction   = 100.0 - (100.0 * compressedSize / originalSize);

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "  " << originalKB << " KB  ->  " << compressedKB << " KB"
              << "  (" << reduction << "% smaller)\n";
}

void Statistics::displayProjectInfo() const {
    std::cout << "\nHuffman Text Compression Tool — v1.0\n"
              << "Algorithm: Huffman coding with bit-packing\n"
              << "Storage format: freq_table|||packed_bits\n";
}