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
    std::string freqTable = serializeFrequencies();
    
    return freqTable + "|||" + encoded;
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

void HuffmanCompressor::deleteTree(Node* node) {
    if (!node) return;
    
    deleteTree(node->left);
    deleteTree(node->right);
    
    delete node;
}
