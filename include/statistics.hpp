#pragma once

#include <string>

class Statistics {
private:
    std::string inputFile;
    std::string outputFile;
    long long originalSize;
    long long compressedSize;
    double compressionRatio;
    bool hasCompressionData;  

public:
    Statistics();
    
    void setCompressionStats(const std::string& input, const std::string& output, 
                            long long original, long long compressed);
    
    void setDecompressionStats(const std::string& input, const std::string& output, 
                              long long decompressed);
    
    void displayCompressionStats();
    void displayDecompressionStats();
    void displayProjectInfo();
    void displayStoredStats(); 
};