#include "huffmanDecompressor.hpp"

#include <iostream>
#include <queue>
#include <sstream>
#include <vector>

// Same tree-building logic as in huffmanCompressor.cpp.
// Duplicated as a static free function rather than shared via inheritance
// to keep both classes independent and avoid the complexity of a base class
// for what is a single shared function.
static std::unique_ptr<Node> buildHuffmanTree(const std::map<char, int>& frequencies) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;
    std::vector<std::unique_ptr<Node>> pool;

    for (const auto& [ch, freq] : frequencies) {
        pool.push_back(std::make_unique<Node>(ch, freq));
        pq.push(pool.back().get());
    }

    while (pq.size() > 1) {
        Node* left  = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        auto parent = std::make_unique<Node>(left->frequency + right->frequency);
        for (auto& p : pool) {
            if (p.get() == left)  parent->left  = std::move(p);
            if (p.get() == right) parent->right = std::move(p);
        }
        pq.push(parent.get());
        pool.push_back(std::move(parent));
    }

    if (pq.empty()) return nullptr;

    Node* rootRaw = pq.top();
    for (auto& p : pool) {
        if (p.get() == rootRaw) return std::move(p);
    }
    return nullptr;
}

// --- HuffmanDecompressor ---

std::pair<std::string, std::string>
HuffmanDecompressor::splitCompressed(const std::string& compressed) const {
    const size_t sep = compressed.find("|||");
    if (sep == std::string::npos) return {"", ""};
    return { compressed.substr(0, sep), compressed.substr(sep + 3) };
}

std::map<char, int>
HuffmanDecompressor::deserializeFrequencies(const std::string& freqStr) const {
    // Parses the format written by serializeFrequencies(): "c|freq:" per entry
    std::map<char, int> freqs;
    size_t i = 0;

    while (i < freqStr.size()) {
        char c = freqStr[i++];

        if (i >= freqStr.size() || freqStr[i] != '|') continue;
        ++i; // skip '|'

        std::string numStr;
        while (i < freqStr.size() && freqStr[i] != ':')
            numStr += freqStr[i++];
        if (i < freqStr.size()) ++i; // skip ':'

        if (!numStr.empty())
            freqs[c] = std::stoi(numStr);
    }
    return freqs;
}

std::string HuffmanDecompressor::unpackBits(const std::string& packed) const {
    if (packed.empty()) return {};

    // First byte is the padding count stored during compression
    const unsigned char padding = static_cast<unsigned char>(packed[0]);
    std::string binary;
    binary.reserve((packed.size() - 1) * 8);

    for (size_t i = 1; i < packed.size(); ++i) {
        unsigned char byte = static_cast<unsigned char>(packed[i]);
        for (int j = 7; j >= 0; --j)
            binary += ((byte >> j) & 1) ? '1' : '0';
    }

    // Strip trailing padding bits
    if (padding > 0 && binary.size() >= padding)
        binary.resize(binary.size() - padding);

    return binary;
}

std::string HuffmanDecompressor::decodeText(const std::string& bits) const {
    if (!root) return {};

    std::string decoded;
    decoded.reserve(bits.size()); // worst case; will be much smaller in practice
    const Node* current = root.get();

    for (char bit : bits) {
        current = (bit == '0') ? current->left.get() : current->right.get();

        // Reached a leaf — emit character and return to root
        if (current && !current->left && !current->right) {
            decoded += current->character;
            current = root.get();
        }
    }
    return decoded;
}

std::string HuffmanDecompressor::decompress(const std::string& compressed) {
    if (compressed.empty()) return {};

    frequencies.clear();

    auto [freqTable, packedText] = splitCompressed(compressed);
    if (freqTable.empty() || packedText.empty()) {
        std::cerr << "Error: Invalid compressed format.\n";
        return {};
    }

    frequencies = deserializeFrequencies(freqTable);
    if (frequencies.empty()) {
        std::cerr << "Error: Could not parse frequency table.\n";
        return {};
    }

    root = buildHuffmanTree(frequencies);
    if (!root) {
        std::cerr << "Error: Could not build Huffman tree.\n";
        return {};
    }

    return decodeText(unpackBits(packedText));
}