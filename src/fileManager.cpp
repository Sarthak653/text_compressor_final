#include "fileManager.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string FileManager::readFile(const std::string& filename) const {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file for reading: " + filename);

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void FileManager::writeFile(const std::string& filename, const std::string& content) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file for writing: " + filename);

    file << content;
}

bool FileManager::fileExists(const std::string& filename) const {
    std::ifstream file(filename);
    return file.good();
}