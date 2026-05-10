#include "huffmanCompressor.hpp"

#include <queue>
#include <vector>

// Shared free function — builds a Huffman tree from a frequency map.
// Defined here and mirrored in huffmanDecompressor.cpp.
// Both compressor and decompressor need the same tree-build logic;
// keeping it as a free function avoids duplicating it inside each class
// while also avoiding the overhead of a shared base class.
static std::unique_ptr<Node> buildHuffmanTree(const std::map<char, int>& frequencies) {
    // Min-heap: node with lowest frequency has highest priority.
    // We store raw non-owning pointers in the queue and manage ownership separately
    // via a vector of unique_ptrs, then transfer ownership to the tree.
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;
    std::vector<std::unique_ptr<Node>> pool; // keeps nodes alive during tree construction

    for (const auto& [ch, freq] : frequencies) {
        pool.push_back(std::make_unique<Node>(ch, freq));
        pq.push(pool.back().get());
    }

    while (pq.size() > 1) {
        // Pop two lowest-frequency nodes
        Node* left  = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        // Create a parent whose frequency is the sum of its children
        auto parent = std::make_unique<Node>(left->frequency + right->frequency);

        // Transfer ownership of left and right into the parent
        // Find them in pool and release, then assign to parent
        for (auto& p : pool) {
            if (p.get() == left)  { parent->left  = std::move(p); }
            if (p.get() == right) { parent->right = std::move(p); }
        }

        pq.push(parent.get());
        pool.push_back(std::move(parent));
    }

    if (pq.empty()) return nullptr;

    // Transfer the root out of the pool
    Node* rootRaw = pq.top();
    for (auto& p : pool) {
        if (p.get() == rootRaw) return std::move(p);
    }
    return nullptr;
}

// --- HuffmanCompressor ---

void HuffmanCompressor::calculateFrequencies(const std::string& text) {
    for (char c : text) frequencies[c]++;
}

void HuffmanCompressor::generateCodes(const Node* node, const std::string& code) {
    if (!node) return;

    // Leaf node: assign its code (use "0" for single-character input edge case)
    if (!node->left && !node->right) {
        codes[node->character] = code.empty() ? "0" : code;
        return;
    }

    generateCodes(node->left.get(),  code + '0');
    generateCodes(node->right.get(), code + '1');
}

std::string HuffmanCompressor::encodeText(const std::string& text) const {
    std::string encoded;
    // Reserve an estimate to avoid repeated reallocations during encoding
    encoded.reserve(text.size() * 4);
    for (char c : text) encoded += codes.at(c);
    return encoded;
}

std::string HuffmanCompressor::serializeFrequencies() const {
    // Format: "c|freq:" repeated for each character.
    // The '|' separates character from its count; ':' ends each entry.
    // Note: if input can contain '|' or ':', this format needs escaping.
    // For plain text files this is sufficient.
    std::string serialized;
    for (const auto& [ch, freq] : frequencies)
        serialized += ch + ('|' + std::to_string(freq) + ':');
    return serialized;
}

std::string HuffmanCompressor::packBits(const std::string& binaryString) const {
    std::string packed;
    // First byte stores padding count so the decompressor knows how many
    // trailing bits to discard from the last byte.
    int padding = static_cast<int>((8 - (binaryString.size() % 8)) % 8);
    packed += static_cast<char>(padding);

    for (size_t i = 0; i < binaryString.size(); i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8 && (i + j) < binaryString.size(); ++j) {
            if (binaryString[i + j] == '1')
                byte |= static_cast<unsigned char>(1 << (7 - j));
        }
        packed += static_cast<char>(byte);
    }
    return packed;
}

std::string HuffmanCompressor::compress(const std::string& text) {
    if (text.empty()) return {};

    // Reset state so the compressor is safe to reuse across calls
    frequencies.clear();
    codes.clear();

    calculateFrequencies(text);
    root = buildHuffmanTree(frequencies);
    generateCodes(root.get(), "");

    const std::string encoded  = encodeText(text);
    const std::string packed   = packBits(encoded);
    const std::string freqData = serializeFrequencies();

    // "|||" is the delimiter between the frequency table and the packed bit data
    return freqData + "|||" + packed;
}