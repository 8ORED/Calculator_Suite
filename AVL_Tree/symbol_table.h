#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>

// Node structure — height field included now so Day 2 rotation code
// doesn't require restructuring the node later.
// Height convention A: leaf height = 1, empty subtree (nullptr) = 0
struct Node {
    std::string name;
    double value;
    Node* left;
    Node* right;
    int height;

    Node(const std::string& n, double v)
        : name(n), value(v), left(nullptr), right(nullptr), height(0) {}
};

class SymbolTable {
private:
    Node* root;

    // --- internal recursive helpers ---
    Node* insertHelper(Node* node, const std::string& name, double value);
    Node* searchHelper(Node* node, const std::string& name);
    void inorderHelper(Node* node, std::vector<std::pair<std::string, double>>& result);
    void destroy(Node* node);

    // --- AVL balancing helpers (Day 2) ---
    int getHeight(Node* node);              // -1 for nullptr, Convention B
    int getBalanceFactor(Node* node);       // height(left) - height(right)
    void updateHeight(Node* node);          // recompute node->height from children
 
    Node* rotateRight(Node* y);             // LL case
    Node* rotateLeft(Node* x);              // RR case
    Node* balance(Node* node);              // checks balance factor, applies correct rotation

public:
    SymbolTable();
    ~SymbolTable();

    // --- public interface (Person 4 / CLI calls these) ---
    void insert(const std::string& name, double value);            // insert or update
    bool search(const std::string& name, double& outValue);        // returns true if found
    bool remove(const std::string& name);                          // stubbed for Day 4, returns false for now
    std::vector<std::pair<std::string, double>> listVariables();   // sorted by name (in-order)
    int getTreeHeight();                                            // for demo/report: proves the tree stays balanced
};

#endif