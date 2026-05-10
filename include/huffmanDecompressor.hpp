#pragma once
// huffmanDecompressor.hpp
// Depends only on huffmanNode.hpp — no longer coupled to huffmanCompressor.hpp.

#include "huffmanNode.hpp"

#include <map>
#include <memory>
#include <string>
#include <utility>

class HuffmanDecompressor {
private:
    std::map<char, int>   frequencies;
    std::unique_ptr<Node> root;

    std::pair<std::string, std::string> splitCompressed(const std::string& compressed) const;
    std::map<char, int>  deserializeFrequencies(const std::string& freqStr) const;
    std::string          unpackBits(const std::string& packedString) const;
    std::string          decodeText(const std::string& encodedText) const;

public:
    HuffmanDecompressor() = default;

    std::string decompress(const std::string& compressed);
};