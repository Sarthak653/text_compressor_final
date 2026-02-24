#pragma once

#include <string>
#include <map>
#include <utility>
#include "huffmanCompressor.hpp"

class HuffmanDecompressor {
private:
    std::map<char, int> frequencies;
    Node* root;

public:
    HuffmanDecompressor();
    ~HuffmanDecompressor();
    
    std::string decompress(const std::string& compressed);
    
private:
    std::pair<std::string, std::string> splitCompressed(const std::string& compressed);
    std::map<char, int> deserializeFrequencies(const std::string& freqStr);
    Node* buildDecompressionTree();
    std::string decodeText(const std::string& encodedText);
    void deleteTree(Node* node);
};
