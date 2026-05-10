#pragma once
// statistics.hpp
// Single place for all compression/decompression stat calculations and display.
// Previously this class existed but was bypassed — compressionTool.cpp did the
// math inline in three separate places. Now all stat logic lives here.

#include <string>

class Statistics {
private:
    std::string inputFile;
    std::string outputFile;
    long long   originalSize   = 0;
    long long   compressedSize = 0;
    bool        hasData        = false;

public:
    Statistics() = default;

    // Call after compression to record stats
    void setCompressionStats(const std::string& input, const std::string& output,
                             long long original, long long compressed);

    // Call after decompression (sizes already recorded from compression step)
    void setDecompressionStats(const std::string& input, const std::string& output);

    bool hasCompressionData() const { return hasData; }

    // Displays a one-line summary: "X KB → Y KB (Z% smaller)"
    void displayStats() const;

    void displayProjectInfo() const;
};