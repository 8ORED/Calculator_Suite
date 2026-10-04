#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

struct HuffmanNode
{
    char ch;
    int freq;

    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char character, int frequency);
};

struct Compare
{
    bool operator()(HuffmanNode* left, HuffmanNode* right);
};

class Huffman
{
private:

    // Root of Huffman Tree
    HuffmanNode* root;

    // Stores Huffman Codes
    unordered_map<char, string> huffmanCode;

    // Count frequency of each character
    unordered_map<char, int> buildFrequencyTable(const string& text);

    // Build Huffman Tree
    void buildTree(const unordered_map<char, int>& frequency);

    // Generate Huffman Codes
    void generateCodes(HuffmanNode* node, const string& code);;

    // Free dynamically allocated memory
    void deleteTree(HuffmanNode* node);

    bool isLeaf(HuffmanNode* node);
    void clearCodes();

public:

    Huffman();

    ~Huffman();

    // Compress a string
    string encode(const string& text);

    // Decompress binary string
    string decode(const string& encodedText);

    // Print Huffman Codes
    void displayCodes();

};

#endif