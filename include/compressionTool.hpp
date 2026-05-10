#pragma once
// compressionTool.hpp
// Orchestrates authentication, compression, decompression, and statistics.
// currentUser is no longer duplicated here — authManager is the single
// source of truth; we call authManager.getCurrentUser() wherever we need it.

#include "authenticationManager.hpp"
#include "fileManager.hpp"
#include "huffmanCompressor.hpp"
#include "huffmanDecompressor.hpp"
#include "statistics.hpp"

#include <string>

class CompressionTool {
private:
    AuthenticationManager authManager;
    FileManager           fileManager;
    HuffmanCompressor     compressor;
    HuffmanDecompressor   decompressor;
    Statistics            stats;
    bool                  isRunning = true;

    // Menu display
    void displayAuthMenu() const;
    void displayMainMenu() const;
    void displayPostCompressionMenu() const;
    void displayPostStatsMenu() const;

    // Handlers
    void handleAuthentication();
    void handleCompression();
    void handleDecompression(const std::string& preloadedPath = "");
    void handleStatistics();

    // Input helpers
    std::string getInput(const std::string& prompt) const;
    // Safe integer input — returns -1 on invalid input instead of throwing
    int         getChoice() const;

public:
    CompressionTool();
    void run();
};