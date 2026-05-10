#pragma once
// huffmanNode.hpp — Shared node type for the Huffman tree.
// Previously defined inside huffmanCompressor.hpp, which forced
// huffmanDecompressor to depend on the compressor's header just to get Node.
// Extracting it here breaks that coupling.

#include <memory>

struct Node {
    char     character;
    int      frequency;
    // unique_ptr replaces raw Node* — no manual deleteTree() needed.
    // Ownership is clear: parent owns its children.
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    // Leaf node constructor
    Node(char c, int freq)
        : character(c), frequency(freq) {}

    // Internal node constructor (no character)
    explicit Node(int freq)
        : character('\0'), frequency(freq) {}
};

// Comparator for the min-heap priority queue (lower frequency = higher priority)
struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->frequency > b->frequency;
    }
};