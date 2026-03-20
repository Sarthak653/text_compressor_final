#include "huffmanCompressor.hpp"
#include <iostream>

HuffmanCompressor::HuffmanCompressor() : root(nullptr) {}

HuffmanCompressor::~HuffmanCompressor() {
    deleteTree(root);
}

std::string HuffmanCompressor::compress(const std::string& text) {
    if (text.empty()) {
        return "";
    }
    
    frequencies.clear();
    codes.clear();
    deleteTree(root);
    
    calculateFrequencies(text);
    root = buildCompressionTree();
    generateCodes(root, "");
    
    std::string encoded = encodeText(text);
    std::string packed = packBits(encoded);
    std::string freqTable = serializeFrequencies();
    
    return freqTable + "|||" + packed;
}

void HuffmanCompressor::calculateFrequencies(const std::string& text) {
    for (char c : text) {
        frequencies[c]++;
    }
}

Node* HuffmanCompressor::buildCompressionTree() {
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> pq;
    
    for (auto& pair : frequencies) {
        Node* node = new Node(pair.first, pair.second);
        pq.push(node);
    }
    
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        
        Node* right = pq.top();
        pq.pop();
        
        Node* parent = new Node(left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        
        pq.push(parent);
    }
    
    return pq.empty() ? nullptr : pq.top();
}

void HuffmanCompressor::generateCodes(Node* node, const std::string& code) {
    if (!node) return;
    
    if (node->left == nullptr && node->right == nullptr) {
        codes[node->character] = code.empty() ? "0" : code;
        return;
    }
    
    if (node->left) {
        generateCodes(node->left, code + "0");
    }
    
    if (node->right) {
        generateCodes(node->right, code + "1");
    }
}

std::string HuffmanCompressor::encodeText(const std::string& text) {
    std::string encoded = "";
    
    for (char c : text) {
        encoded += codes[c];
    }
    
    return encoded;
}

std::string HuffmanCompressor::serializeFrequencies() {
    std::string serialized = "";
    
    for (auto& pair : frequencies) {
        serialized += pair.first;
        serialized += "|";
        serialized += std::to_string(pair.second);
        serialized += ":";
    }
    
    return serialized;
}

std::string HuffmanCompressor::packBits(const std::string& binaryString) {
    std::string packed = "";
    
    // First byte: padding count (0-7)
    int padding = (8 - (binaryString.length() % 8)) % 8;
    packed += static_cast<char>(padding);
    
    // Pack bits into bytes
    for (size_t i = 0; i < binaryString.length(); i += 8) {
        unsigned char byte = 0;
        
        for (int j = 0; j < 8 && (i + j) < binaryString.length(); j++) {
            if (binaryString[i + j] == '1') {
                byte |= (1 << (7 - j));  // Set bit from left to right
            }
        }
        
        packed += static_cast<char>(byte);
    }
    
    return packed;
}

void HuffmanCompressor::deleteTree(Node* node) {
    if (!node) return;
    
    deleteTree(node->left);
    deleteTree(node->right);
    
    delete node;
}