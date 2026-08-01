#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

struct Node
{
    char ch;
    int freq;

    Node* left;
    Node* right;

    Node(char character, int frequency);
};

struct Compare
{
    bool operator()(Node* left, Node* right);
};

class Huffman
{
private:

    // Root of Huffman Tree
    Node* root;

    // Stores Huffman Codes
    unordered_map<char, string> huffmanCode;

    // Count frequency of each character
    unordered_map<char, int> buildFrequencyTable(const string& text);

    // Build Huffman Tree
    void buildTree(const unordered_map<char, int>& frequency);

    // Generate Huffman Codes
    void generateCodes(Node* node, const string& code);;

    // Free dynamically allocated memory
    void deleteTree(Node* node);

    bool isLeaf(Node* node);
    void clearCodes();
    void deleteTree(Node* node);

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