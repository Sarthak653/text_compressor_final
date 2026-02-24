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

public:
    CompressionTool();
    void run();
    
private:
    void displayAuthMenu();
    void displayMainMenu();
    void handleAuthentication();
    void handleCompression();
    void handleDecompression();
    void handleStatistics();
    std::string getInput(const std::string& prompt);
};
