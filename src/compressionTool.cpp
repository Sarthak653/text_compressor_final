#include "compressionTool.hpp"
#include <iomanip>

CompressionTool::CompressionTool() 
    : authManager("users.txt"), currentUser(""), isRunning(true) {}

void CompressionTool::run() {
    std::cout << "\n########################################" << std::endl;
    std::cout << "#  HUFFMAN TEXT COMPRESSION TOOL      #" << std::endl;
    std::cout << "#  Version 1.0                        #" << std::endl;
    std::cout << "########################################" << std::endl;
    
    handleAuthentication();
    
    while (isRunning) {
        displayMainMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
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
                std::cout << "Logging out..." << std::endl;
                currentUser = "";
                authManager.logout();
                handleAuthentication();
                break;
            case 5:
                std::cout << "Thank you for using Huffman Compression Tool!" << std::endl;
                isRunning = false;
                break;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
        }
    }
}

void CompressionTool::displayAuthMenu() {
    std::cout << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "#  HUFFMAN TEXT COMPRESSION TOOL      #" << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "#  1. Login                           #" << std::endl;
    std::cout << "#  2. Signup                          #" << std::endl;
    std::cout << "#  3. Continue as Guest               #" << std::endl;
    std::cout << "#  4. Exit                            #" << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "Enter your choice (1-4): ";
}

void CompressionTool::displayMainMenu() {
    std::cout << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "#   HUFFMAN TEXT COMPRESSION TOOL     #" << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "#  User: " << std::setw(29) << std::left << (currentUser + " #") << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "#  1. Compress File                   #" << std::endl;
    std::cout << "#  2. Decompress File                 #" << std::endl;
    std::cout << "#  3. View Statistics                 #" << std::endl;
    std::cout << "#  4. Logout                          #" << std::endl;
    std::cout << "#  5. Exit                            #" << std::endl;
    std::cout << "########################################" << std::endl;
    std::cout << "Enter your choice (1-5): ";
}

void CompressionTool::handleAuthentication() {
    while (currentUser.empty()) {
        displayAuthMenu();
        int authChoice;
        std::cin >> authChoice;
        std::cin.ignore();
        
        if (authChoice == 1) {
            std::string username = getInput("Username: ");
            std::string password = getInput("Password: ");
            
            if (authManager.login(username, password)) {
                currentUser = username;
            }
        } 
        else if (authChoice == 2) {
            std::string username = getInput("Choose username: ");
            std::string password = getInput("Choose password: ");
            
            if (authManager.signup(username, password)) {
                currentUser = username;
            }
        } 
        else if (authChoice == 3) {
            currentUser = "Guest";
            std::cout << "Logged in as Guest" << std::endl;
        } 
        else if (authChoice == 4) {
            std::cout << "Thank you for using Huffman Compression Tool!" << std::endl;
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
    
    stats.setCompressionStats(inputPath, outputPath, content.size(), compressed.size());
    stats.displayCompressionStats();
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
    
    stats.setDecompressionStats(inputPath, outputPath, decompressed.size());
    stats.displayDecompressionStats();
}

void CompressionTool::handleStatistics() {
    stats.displayStoredStats();  // ← Show stored compression stats instead
}

std::string CompressionTool::getInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}