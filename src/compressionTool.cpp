#include "compressionTool.hpp"

#include <iostream>
#include <stdexcept>

// --- Constructor ---

CompressionTool::CompressionTool()
    : authManager("users.txt") {}

// --- Input helpers ---

std::string CompressionTool::getInput(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

// Safe integer input: wraps std::stoi so a typo doesn't crash the program.
// Returns -1 to signal invalid input; callers treat it as "invalid choice".
int CompressionTool::getChoice() const {
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) return -1;
    try {
        return std::stoi(line);
    } catch (const std::exception&) {
        return -1;
    }
}

// --- Menu display ---

void CompressionTool::displayAuthMenu() const {
    std::cout << "\n1. Login   2. Signup   3. Guest   4. Exit\n> ";
}

void CompressionTool::displayMainMenu() const {
    std::cout << "\nUser: " << authManager.getCurrentUser() << "\n\n"
              << "1. Compress   2. Decompress   3. Statistics   4. Logout   5. Exit\n> ";
}

void CompressionTool::displayPostCompressionMenu() const {
    std::cout << "\n1. Decompress this file   2. Back to menu\n> ";
}

void CompressionTool::displayPostStatsMenu() const {
    std::cout << "\n1. Back to menu   2. Logout   3. Exit\n> ";
}

// --- Authentication ---

void CompressionTool::handleAuthentication() {
    // Loop until the user successfully logs in or exits
    while (authManager.getCurrentUser() == "Guest" ||
           authManager.getCurrentUser().empty()) {
        // Re-check isRunning inside the loop so Exit works from the auth menu
        if (!isRunning) return;

        displayAuthMenu();
        const int choice = getChoice();

        if (choice == 1) {
            const std::string username = getInput("Username: ");
            const std::string password = getInput("Password: ");
            authManager.login(username, password);
        }
        else if (choice == 2) {
            const std::string username = getInput("Username: ");
            const std::string password = getInput("Password: ");
            authManager.signup(username, password);
        }
        else if (choice == 3) {
            // Guest access — getCurrentUser() returns "Guest" so the loop exits
            std::cout << "\nLogged in as Guest.\n";
            break;
        }
        else if (choice == 4) {
            std::cout << "\nGoodbye!\n";
            isRunning = false;
            return;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

// --- Main loop ---

void CompressionTool::run() {
    std::cout << "\nHUFFMAN TEXT COMPRESSION TOOL\nVersion 1.0\n";

    handleAuthentication();

    while (isRunning) {
        displayMainMenu();
        const int choice = getChoice();

        switch (choice) {
            case 1: handleCompression();  break;
            case 2: handleDecompression(); break;
            case 3: handleStatistics();   break;
            case 4:
                std::cout << "\nLogging out...\n";
                authManager.logout();
                handleAuthentication();
                break;
            case 5:
                std::cout << "\nThank you for using Huffman Compression Tool!\n";
                isRunning = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

// --- Compression ---

void CompressionTool::handleCompression() {
    std::cout << "\n=== FILE COMPRESSION ===\n";

    const std::string inputPath = getInput("Enter input file path: ");
    if (inputPath.empty() || !fileManager.fileExists(inputPath)) {
        std::cout << "Error: File not found.\n";
        return;
    }

    const std::string content = fileManager.readFile(inputPath);
    if (content.empty()) {
        std::cout << "Error: File is empty.\n";
        return;
    }

    std::cout << "Compressing...\n";
    const std::string compressed = compressor.compress(content);

    std::string outputPath = getInput("Enter output file path (default: output/compressed.huff): ");
    if (outputPath.empty()) outputPath = "output/compressed.huff";

    fileManager.writeFile(outputPath, compressed);

    // Record stats — single source of truth, no inline math elsewhere
    stats.setCompressionStats(inputPath, outputPath,
                              static_cast<long long>(content.size()),
                              static_cast<long long>(compressed.size()));

    std::cout << "\nCompression complete!\n";
    stats.displayStats();

    // Offer to immediately decompress — pass the output path so the user
    // doesn't have to type it again
    displayPostCompressionMenu();
    const int choice = getChoice();
    if (choice == 1) handleDecompression(outputPath);
}

// --- Decompression ---
// preloadedPath: when called right after compression, the compressed file path
// is already known — skip asking for it.
// When called from the main menu, preloadedPath is empty and we prompt normally.
void CompressionTool::handleDecompression(const std::string& preloadedPath) {
    std::cout << "\n=== FILE DECOMPRESSION ===\n";

    std::string inputPath = preloadedPath;
    if (inputPath.empty()) {
        inputPath = getInput("Enter compressed file path: ");
    }

    if (inputPath.empty() || !fileManager.fileExists(inputPath)) {
        std::cout << "Error: File not found.\n";
        return;
    }

    const std::string compressed = fileManager.readFile(inputPath);
    if (compressed.empty()) {
        std::cout << "Error: File is empty.\n";
        return;
    }

    std::cout << "Decompressing...\n";
    const std::string decompressed = decompressor.decompress(compressed);

    std::string outputPath = getInput("Enter output file path (default: output/decompressed.txt): ");
    if (outputPath.empty()) outputPath = "output/decompressed.txt";

    fileManager.writeFile(outputPath, decompressed);
    stats.setDecompressionStats(inputPath, outputPath);

    std::cout << "\nDecompression complete. Output written to: " << outputPath << "\n";

    // Ask if they want to see stats, then show the post-stats navigation
    const std::string showStats = getInput("\nShow compression statistics? (y/n): ");
    if (!showStats.empty() && (showStats[0] == 'y' || showStats[0] == 'Y'))
        handleStatistics();
}

// --- Statistics ---

void CompressionTool::handleStatistics() {
    std::cout << "\n=== STATISTICS ===\n";
    stats.displayStats();

    // Post-stats navigation — same pattern used after decompression
    bool loop = true;
    while (loop) {
        displayPostStatsMenu();
        const int choice = getChoice();

        if (choice == 1) {
            loop = false; // back to main menu
        }
        else if (choice == 2) {
            std::cout << "\nLogging out...\n";
            authManager.logout();
            handleAuthentication();
            loop = false;
        }
        else if (choice == 3) {
            std::cout << "\nThank you for using Huffman Compression Tool!\n";
            isRunning = false;
            loop = false;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
}