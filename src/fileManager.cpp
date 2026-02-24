#include "fileManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::string FileManager::readFile(const std::string& filename) {
    if (!fileExists(filename)) {
        std::cerr << "Error: File '" << filename << "' not found!" << std::endl;
        return "";
    }

    std::ifstream inputFile(filename, std::ios::binary);
    
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    inputFile.close();
    
    return buffer.str();
}

void FileManager::writeFile(const std::string& filename, const std::string& content) {
    std::ofstream outputFile(filename, std::ios::binary);
    
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not create file '" << filename << "'" << std::endl;
        return;
    }
    
    outputFile << content;
    outputFile.close();
    
    std::cout << "Successfully wrote to file: " << filename << std::endl;
}

bool FileManager::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}
