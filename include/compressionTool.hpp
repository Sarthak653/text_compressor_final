// compressionTool.hpp
#pragma once

#include <string>
#include <iostream>
#include "fileManager.hpp"
#include "huffmanCompressor.hpp"
#include "huffmanDecompressor.hpp"
#include "authenticationManager.hpp"
#include "statistics.hpp"

class CompressionTool {
private:
    AuthenticationManager authManager;
    FileManager fileManager;
    HuffmanCompressor compressor;
    HuffmanDecompressor decompressor;
    Statistics stats;
    std::string currentUser;
    bool isRunning;
    std::string lastInputFile;
    std::string lastOutputFile;
    long long lastOriginalSize;
    long long lastCompressedSize;

public:
    CompressionTool();
    void run();
    
private:
    void displayAuthMenu();
    void displayMainMenu();
    void handleAuthentication();
    void handleCompression();
    void handleDecompression();
    void handleDecompressionAfterCompression();
    void handleStatistics();
    void displayPostCompressionMenu();
    void displayPostDecompressionMenu();
    void displayPostStatsMenu();
    std::string getInput(const std::string& prompt);
};