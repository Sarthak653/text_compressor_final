#include "compressionTool.hpp"
#include <iomanip>

CompressionTool::CompressionTool() 
    : authManager("users.txt"), currentUser(""), isRunning(true),
      lastOriginalSize(0), lastCompressedSize(0) {}

void CompressionTool::run() {
    std::cout << "\nHUFFMAN TEXT COMPRESSION TOOL" << std::endl;
    std::cout << "Version 1.0\n" << std::endl;
    
    handleAuthentication();
    
    while (isRunning && !currentUser.empty()) {
        displayMainMenu();
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        
        if (choiceStr.empty()) continue;
        
        int choice = std::stoi(choiceStr);
        
        switch (choice) {
            case 1:
                handleCompression();
                break;
            case 2:
                handleDecompression();
                break;
            case 3:
                handleStatistics();
                break;
            case 4:
                std::cout << "\nLogging out..." << std::endl;
                currentUser = "";
                authManager.logout();
                handleAuthentication();
                break;
            case 5:
                std::cout << "\nThank you for using Huffman Compression Tool!" << std::endl;
                isRunning = false;
                break;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
}

void CompressionTool::displayAuthMenu() {
    std::cout << "\nHUFFMAN TEXT COMPRESSION TOOL" << std::endl;
    std::cout << "Version 1.0\n" << std::endl;
    std::cout << "1. Login  2. Signup  3. Guest  4. Exit\n" << std::endl;
    std::cout << "> ";
}

void CompressionTool::displayMainMenu() {
    std::cout << "\nUser: " << currentUser << "\n" << std::endl;
    std::cout << "1. Compress  2. Decompress  3. View statistics  4. Logout  5. Exit\n" << std::endl;
    std::cout << "> ";
}

void CompressionTool::displayPostCompressionMenu() {
    std::cout << "\n1. Decompress file  2. Exit\n" << std::endl;
    std::cout << "> ";
}

void CompressionTool::displayPostDecompressionMenu() {
    std::cout << "\nDo you want to see statistics? (1. Yes / 2. No)\n" << std::endl;
    std::cout << "> ";
}

void CompressionTool::displayPostStatsMenu() {
    std::cout << "\nDo you want to use again? (1. Yes / 2. Logout / 3. Exit)\n" << std::endl;
    std::cout << "> ";
}

void CompressionTool::handleAuthentication() {
    while (currentUser.empty() && isRunning) {
        displayAuthMenu();
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        
        if (choiceStr.empty()) continue;
        
        int authChoice = std::stoi(choiceStr);
        
        if (authChoice == 1) {
            std::string username = getInput("Username: ");
            std::string password = getInput("Password: ");
            
            if (authManager.login(username, password)) {
                currentUser = username;
            }
        } 
        else if (authChoice == 2) {
            std::string username = getInput("Username: ");
            std::string password = getInput("Password: ");
            
            if (authManager.signup(username, password)) {
                currentUser = username;
            }
        } 
        else if (authChoice == 3) {
            currentUser = "Guest";
            std::cout << "\nLogged in as Guest\n" << std::endl;
        } 
        else if (authChoice == 4) {
            std::cout << "\nThank you for using Huffman Compression Tool!" << std::endl;
            isRunning = false;
            break;
        } 
        else {
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
}

void CompressionTool::handleCompression() {
    std::cout << "\n=== FILE COMPRESSION ===" << std::endl;
    
    std::string inputPath = getInput("Enter input file path: ");
    
    if (inputPath.empty() || !fileManager.fileExists(inputPath)) {
        std::cout << "Error: File not found!" << std::endl;
        return;
    }
    
    std::string content = fileManager.readFile(inputPath);
    
    if (content.empty()) {
        std::cout << "Error: File is empty!" << std::endl;
        return;
    }
    
    std::cout << "Compressing..." << std::endl;
    std::string compressed = compressor.compress(content);
    
    std::string outputPath = getInput("Enter output file path (default: output/compressed.huff): ");
    
    if (outputPath.empty()) {
        outputPath = "output/compressed.huff";
    }
    
    fileManager.writeFile(outputPath, compressed);
    
    lastInputFile = inputPath;
    lastOutputFile = outputPath;
    lastOriginalSize = content.size();
    lastCompressedSize = compressed.size();
    
    bool postCompressionLoop = true;
    while (postCompressionLoop) {
        displayPostCompressionMenu();
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        
        if (choiceStr.empty()) continue;
        
        int choice = std::stoi(choiceStr);
        
        if (choice == 1) {
            handleDecompressionAfterCompression();
            postCompressionLoop = false;
        }
        else if (choice == 2) {
            postCompressionLoop = false;
        }
        else {
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
}

void CompressionTool::handleDecompression() {
    std::cout << "\n=== FILE DECOMPRESSION ===" << std::endl;
    
    std::string inputPath = getInput("Enter compressed file path: ");
    
    if (inputPath.empty() || !fileManager.fileExists(inputPath)) {
        std::cout << "Error: File not found!" << std::endl;
        return;
    }
    
    std::string compressed = fileManager.readFile(inputPath);
    
    if (compressed.empty()) {
        std::cout << "Error: File is empty!" << std::endl;
        return;
    }
    
    std::cout << "Decompressing..." << std::endl;
    std::string decompressed = decompressor.decompress(compressed);
    
    std::string outputPath = getInput("Enter output file path (default: output/decompressed.txt): ");
    
    if (outputPath.empty()) {
        outputPath = "output/decompressed.txt";
    }
    
    fileManager.writeFile(outputPath, decompressed);
    
    bool showStats = false;
    bool statsLoop = true;
    while (statsLoop) {
        displayPostDecompressionMenu();
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        
        if (choiceStr.empty()) continue;
        
        int choice = std::stoi(choiceStr);
        
        if (choice == 1) {
            showStats = true;
            statsLoop = false;
        }
        else if (choice == 2) {
            showStats = false;
            statsLoop = false;
        }
        else {
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
    
    if (showStats) {
        std::cout << "\nCompression complete! ";
        
        double originalKB = lastOriginalSize / 1024.0;
        double compressedKB = lastCompressedSize / 1024.0;
        double percentReduction = 100.0 - (100.0 * lastCompressedSize / lastOriginalSize);
        
        std::cout << std::fixed << std::setprecision(1);
        std::cout << originalKB << " KB -> " << compressedKB << " KB ";
        std::cout << "(" << percentReduction << "% smaller)\n" << std::endl;
    }
    
    bool postStatsLoop = true;
    while (postStatsLoop) {
        displayPostStatsMenu();
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        
        if (choiceStr.empty()) continue;
        
        int choice = std::stoi(choiceStr);
        
        if (choice == 1) {
            postStatsLoop = false;
        }
        else if (choice == 2) {
            std::cout << "\nLogging out..." << std::endl;
            currentUser = "";
            authManager.logout();
            handleAuthentication();
            postStatsLoop = false;
        }
        else if (choice == 3) {
            std::cout << "\nThank you for using Huffman Compression Tool!" << std::endl;
            isRunning = false;
            postStatsLoop = false;
        }
        else {
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
}

void CompressionTool::handleDecompressionAfterCompression() {
    std::cout << "\n=== FILE DECOMPRESSION ===" << std::endl;
    
    std::string inputPath = getInput("Enter compressed file path: ");
    
    if (inputPath.empty() || !fileManager.fileExists(inputPath)) {
        std::cout << "Error: File not found!" << std::endl;
        return;
    }
    
    std::string compressed = fileManager.readFile(inputPath);
    
    if (compressed.empty()) {
        std::cout << "Error: File is empty!" << std::endl;
        return;
    }
    
    std::cout << "Decompressing..." << std::endl;
    std::string decompressed = decompressor.decompress(compressed);
    
    std::string outputPath = getInput("Enter output file path (default: output/decompressed.txt): ");
    
    if (outputPath.empty()) {
        outputPath = "output/decompressed.txt";
    }
    
    fileManager.writeFile(outputPath, decompressed);
    
    bool showStats = false;
    bool statsLoop = true;
    while (statsLoop) {
        displayPostDecompressionMenu();
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        
        if (choiceStr.empty()) continue;
        
        int choice = std::stoi(choiceStr);
        
        if (choice == 1) {
            showStats = true;
            statsLoop = false;
        }
        else if (choice == 2) {
            showStats = false;
            statsLoop = false;
        }
        else {
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
    
    if (showStats) {
        std::cout << "\nCompression complete! ";
        
        double originalKB = lastOriginalSize / 1024.0;
        double compressedKB = lastCompressedSize / 1024.0;
        double percentReduction = 100.0 - (100.0 * lastCompressedSize / lastOriginalSize);
        
        std::cout << std::fixed << std::setprecision(1);
        std::cout << originalKB << " KB → " << compressedKB << " KB ";
        std::cout << "(" << percentReduction << "% smaller)\n" << std::endl;
    }
    
    bool postStatsLoop = true;
    while (postStatsLoop) {
        displayPostStatsMenu();
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        
        if (choiceStr.empty()) continue;
        
        int choice = std::stoi(choiceStr);
        
        if (choice == 1) {
            postStatsLoop = false;
        }
        else if (choice == 2) {
            std::cout << "\nLogging out..." << std::endl;
            currentUser = "";
            authManager.logout();
            handleAuthentication();
            postStatsLoop = false;
        }
        else if (choice == 3) {
            std::cout << "\nThank you for using Huffman Compression Tool!" << std::endl;
            isRunning = false;
            postStatsLoop = false;
        }
        else {
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
}

void CompressionTool::handleStatistics() {
    if (lastOriginalSize == 0) {
        std::cout << "\nNo compression statistics yet. Compress a file first!\n" << std::endl;
        return;
    }
    
    std::cout << "\nCompression complete! ";
    
    double originalKB = lastOriginalSize / 1024.0;
    double compressedKB = lastCompressedSize / 1024.0;
    double percentReduction = 100.0 - (100.0 * lastCompressedSize / lastOriginalSize);
    
    std::cout << std::fixed << std::setprecision(1);
    std::cout << originalKB << " KB -> " << compressedKB << " KB ";
    std::cout << "(" << percentReduction << "% smaller)\n" << std::endl;
    
    bool postStatsLoop = true;
    while (postStatsLoop) {
        displayPostStatsMenu();
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        
        if (choiceStr.empty()) continue;
        
        int choice = std::stoi(choiceStr);
        
        if (choice == 1) {
            postStatsLoop = false;
        }
        else if (choice == 2) {
            std::cout << "\nLogging out..." << std::endl;
            currentUser = "";
            authManager.logout();
            handleAuthentication();
            postStatsLoop = false;
        }
        else if (choice == 3) {
            std::cout << "\nThank you for using Huffman Compression Tool!" << std::endl;
            isRunning = false;
            postStatsLoop = false;
        }
        else {
            std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
}

std::string CompressionTool::getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}