#include "huffman.h"

Node::Node(char character, int frequency)
{
    ch = character;
    freq = frequency;

    left = nullptr;
    right = nullptr;
}

bool Compare::operator()(Node* left,Node* right)
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

    for (char ch : text)
    {
        frequency[ch]++;
    }

    return frequency;
}

void Huffman::buildTree(const unordered_map<char, int>& frequency)
{
    priority_queue<Node*, vector<Node*>, Compare> pq;

    // Create one leaf node for each character
    for (const auto& pair : frequency)
    {
        pq.push(new Node(pair.first, pair.second));
    }

    // Build the Huffman Tree
    while (pq.size() > 1)
    {
        Node* left = pq.top();
        pq.pop();

        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node('\0', left->freq + right->freq);

        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    root = pq.top();
}

void Huffman::generateCodes(Node* node, const string& code)
{
    if (node == nullptr)
    {
        return;
    }

    if (isLeaf(node))
    {
        huffmanCode[node->ch] = code;
        return;
    }

    generateCodes(node->left, code + "0");

    generateCodes(node->right, code + "1");
}

string Huffman::encode(const string& text)
{
    clearCodes();

    unordered_map<char, int> frequency = buildFrequencyTable(text);

    buildTree(frequency);

    generateCodes(root, "");

    string encodedText;

    for (char ch : text)
    {
        encodedText += huffmanCode[ch];
    }

    return encodedText;
}

string Huffman::decode(const string& encodedText)
{
    string decodedText;

    Node* current = root;

    for (char bit : encodedText)
    {
        if (bit == '0')
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }

        if (isLeaf(current))
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
    cout << "------------------------\n";

    for (const auto& pair : huffmanCode)
    {
        cout << pair.first
             << " : "
             << pair.second
             << endl;
    }
}

void Huffman::deleteTree(Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    deleteTree(node->left);

    deleteTree(node->right);

    delete node;
}