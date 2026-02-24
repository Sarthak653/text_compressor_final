#include "huffmanDecompressor.hpp"
#include <sstream>
#include <iostream>

HuffmanDecompressor::HuffmanDecompressor() : root(nullptr) {}

HuffmanDecompressor::~HuffmanDecompressor() {
    deleteTree(root);
}

std::string HuffmanDecompressor::decompress(const std::string& compressed) {
    if (compressed.empty()) {
        return "";
    }
    
    frequencies.clear();
    deleteTree(root);
    
    auto [freqTable, encodedText] = splitCompressed(compressed);
    
    if (freqTable.empty() || encodedText.empty()) {
        std::cerr << "Error: Invalid compressed file format" << std::endl;
        return "";
    }
    
    frequencies = deserializeFrequencies(freqTable);
    
    if (frequencies.empty()) {
        std::cerr << "Error: Could not parse frequency table" << std::endl;
        return "";
    }
    
    root = buildDecompressionTree();
    
    if (!root) {
        std::cerr << "Error: Could not build Huffman tree" << std::endl;
        return "";
    }
    
    std::string decoded = decodeText(encodedText);
    
    return decoded;
}

std::pair<std::string, std::string> HuffmanDecompressor::splitCompressed(const std::string& compressed) {
    size_t separatorPos = compressed.find("|||");
    
    if (separatorPos == std::string::npos) {
        return {"", ""};
    }
    
    std::string freqTable = compressed.substr(0, separatorPos);
    std::string encodedText = compressed.substr(separatorPos + 3);
    
    return {freqTable, encodedText};
}

std::map<char, int> HuffmanDecompressor::deserializeFrequencies(const std::string& freqStr) {
    std::map<char, int> freqs;
    
    size_t i = 0;
    while (i < freqStr.length()) {
        char c = freqStr[i];
        i++;
        
        if (i < freqStr.length() && freqStr[i] == '|') {
            i++;
        }
        
        std::string freqNum = "";
        while (i < freqStr.length() && freqStr[i] != ':') {
            freqNum += freqStr[i];
            i++;
        }
        
        if (i < freqStr.length() && freqStr[i] == ':') {
            i++;
        }
        
        if (!freqNum.empty()) {
            int freq = std::stoi(freqNum);
            freqs[c] = freq;
        }
    }
    
    return freqs;
}

Node* HuffmanDecompressor::buildDecompressionTree() {
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

std::string HuffmanDecompressor::decodeText(const std::string& encodedText) {
    if (!root) return "";
    
    std::string decoded = "";
    Node* current = root;
    
    for (char bit : encodedText) {
        if (bit == '0') {
            current = current->left;
        } else if (bit == '1') {
            current = current->right;
        }
        
        if (current && current->left == nullptr && current->right == nullptr) {
            decoded += current->character;
            current = root;
        }
    }
    
    return decoded;
}

void HuffmanDecompressor::deleteTree(Node* node) {
    if (!node) return;
    
    deleteTree(node->left);
    deleteTree(node->right);
    
    delete node;
}
