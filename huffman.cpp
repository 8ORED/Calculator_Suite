#include "huffman.h"

HuffmanNode::HuffmanNode(char character, int frequency)
{
    ch = character;
    freq = frequency;

    left = nullptr;
    right = nullptr;
}

bool Compare::operator()(HuffmanNode* left, HuffmanNode* right)
{
    return left->freq > right->freq;
}

Huffman::Huffman()
{
    root = nullptr;
}

Huffman::~Huffman()
{
    deleteTree(root);
}

unordered_map<char, int> Huffman::buildFrequencyTable(const string& text)
{
    unordered_map<char, int> frequency;

    for(char ch : text)
    {
        frequency[ch]++;
    }

    return frequency;
}

void Huffman::buildTree(const unordered_map<char, int>& frequency)
{
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    for(const auto& pair : frequency)
    {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    if(pq.empty())
    {
        root = nullptr;
        return;
    }

    while(pq.size() > 1)
    {
        HuffmanNode* left = pq.top();
        pq.pop();

        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* parent =
            new HuffmanNode('\0', left->freq + right->freq);

        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    root = pq.top();
}

void Huffman::generateCodes(HuffmanNode* node, const string& code)
{
    if(node == nullptr)
        return;

    if(isLeaf(node))
    {
        if(code.empty())
            huffmanCode[node->ch] = "0";
        else
            huffmanCode[node->ch] = code;

        return;
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

string Huffman::encode(const string& text)
{
    deleteTree(root);
    root = nullptr;

    clearCodes();

    if(text.empty())
        return "";

    unordered_map<char, int> frequency = buildFrequencyTable(text);

    buildTree(frequency);

    generateCodes(root, "");

    string encodedText;

    for(char ch : text)
    {
        encodedText += huffmanCode[ch];
    }

    return encodedText;
}

string Huffman::decode(const string& encodedText)
{
    if(root == nullptr)
        return "";

    if(isLeaf(root))
    {
        string decoded;

        for(size_t i = 0; i < encodedText.length(); i++)
            decoded += root->ch;

        return decoded;
    }

    string decodedText;

    HuffmanNode* current = root;

    for(char bit : encodedText)
    {
        if(bit == '0')
            current = current->left;
        else if(bit == '1')
            current = current->right;
        else
            continue;

        if(current != nullptr && isLeaf(current))
        {
            decodedText += current->ch;
            current = root;
        }
    }

    return decodedText;
}

void Huffman::displayCodes()
{
    cout << "\nHuffman Codes\n";
    cout << "---------------------------\n";

    for(const auto& pair : huffmanCode)
    {
        if(pair.first == ' ')
            cout << "[space]";
        else if(pair.first == '\n')
            cout << "[newline]";
        else if(pair.first == '\t')
            cout << "[tab]";
        else
            cout << pair.first;

        cout << " : "
             << pair.second
             << endl;
    }
}

void Huffman::deleteTree(HuffmanNode* node)
{
    if(node == nullptr)
        return;

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
}

bool Huffman::isLeaf(HuffmanNode* node)
{
    return node->left == nullptr &&
           node->right == nullptr;
}

void Huffman::clearCodes()
{
    huffmanCode.clear();
}