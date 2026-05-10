#pragma once
// huffmanCompressor.hpp
// Node is now in huffmanNode.hpp — no need to include the decompressor
// or define tree types here.

#include "huffmanNode.hpp"

#include <map>
#include <memory>
#include <string>

class HuffmanCompressor {
private:
    std::map<char, int>         frequencies;
    std::map<char, std::string> codes;
    // unique_ptr owns the tree root — destructor cleans up automatically,
    // no manual deleteTree() needed.
    std::unique_ptr<Node>       root;

    void        calculateFrequencies(const std::string& text);
    void        generateCodes(const Node* node, const std::string& code);
    std::string encodeText(const std::string& text) const;
    std::string serializeFrequencies() const;
    std::string packBits(const std::string& binaryString) const;

public:
    HuffmanCompressor() = default;

    // compress() is the single public entry point.
    // Returns a self-contained binary blob: serialized freq table + packed bits.
    std::string compress(const std::string& text);
};