#pragma once

#include <string>
#include <map>
#include <queue>
#include <vector>

struct Node {
    char character;
    int frequency;
    Node* left;
    Node* right;
    
    Node(char c, int freq) : character(c), frequency(freq), left(nullptr), right(nullptr) {}
    Node(int freq) : character('\0'), frequency(freq), left(nullptr), right(nullptr) {}
};

struct CompareNode {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;  
    }
};

class HuffmanCompressor {
private:
    std::map<char, int> frequencies;
    std::map<char, std::string> codes;
    Node* root;

public:
    HuffmanCompressor();
    ~HuffmanCompressor();
    
    std::string compress(const std::string& text);
    
private:
    void calculateFrequencies(const std::string& text);
    Node* buildCompressionTree();
    void generateCodes(Node* node, const std::string& code);
    std::string encodeText(const std::string& text);
    std::string serializeFrequencies();
    std::string packBits(const std::string& binaryString);
    void deleteTree(Node* node);
};