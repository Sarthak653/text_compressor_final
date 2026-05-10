#pragma once
// fileManager.hpp — Simple file I/O abstraction.
// Keeping this as a thin wrapper makes it easy to swap implementations
// (e.g. for testing) without touching the rest of the codebase.

#include <string>

class FileManager {
public:
    std::string readFile(const std::string& filename) const;
    void        writeFile(const std::string& filename, const std::string& content) const;
    bool        fileExists(const std::string& filename) const;
};