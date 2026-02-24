#pragma once

#include <string>

class FileManager {
public:
    std::string readFile(const std::string& filename);
    void writeFile(const std::string& filename, const std::string& content);
    bool fileExists(const std::string& filename);
};
