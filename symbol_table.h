#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>

// Height convention : leaf height = 1, empty subtree (nullptr) = 0
struct Node {
    std::string name;
    double value;
    Node* left;
    Node* right;
    int height;

    Node(const std::string& n, double v)
        : name(n), value(v), left(nullptr), right(nullptr), height(1) {}
};

class SymbolTable {
private:
    Node* root;

    // internal recursive helpers
    Node* insertHelper(Node* node, const std::string& name, double value);
    Node* removeHelper(Node* node, const std::string& name, bool& wasRemoved);
    Node* searchHelper(Node* node, const std::string& name);
    void inorderHelper(Node* node, std::vector<std::pair<std::string, double>>& result);
    void destroy(Node* node);
    Node* findMin(Node* node); // leftmost node — used to find in-order successor on delete
    
    //AVL balancing helpers
    int getHeight(Node* node);              // 0 for nullptr
    int getBalanceFactor(Node* node);       // height(left) - height(right)
    void updateHeight(Node* node);          // recompute node->height from children
 
    Node* rotateRight(Node* y);             // LL case
    Node* rotateLeft(Node* x);              // RR case
    Node* balance(Node* node);              // checks balance factor, applies correct rotation

public:
    SymbolTable();
    ~SymbolTable();

    // Inserts a new variable, or updates its value if the name already
    // exists (handles reassignment, e.g. "x = 5" followed by "x = 8").
    // Use for: CLI variable-assignment command (e.g. "x = 5").
    void insert(const std::string& name, double value);

    // Looks up a variable by name.
    // Returns true and writes the value into outValue if found;
    // returns false (outValue left untouched) if the name doesn't exist.
    // Use for: resolving variable references during expression evaluation.    
    bool search(const std::string& name, double& outValue);

    // Removes a variable by name.
    // Returns true if the variable existed and was removed;
    // returns false if the name was never inserted (no-op, no crash).
    // Use for: CLI "clear <name>" command.    
    bool remove(const std::string& name);

    // Returns all stored variables as (name, value) pairs, sorted
    // alphabetically by name (in-order traversal of the tree).
    // Use for: CLI "list-vars" command.    
    std::vector<std::pair<std::string, double>> listVariables();
    
    // Returns the current height of the tree (Convention A: empty
    // tree = 0, single node = 1). Not needed for normal CLI operation —
    // exposed for testing/demo purposes to show the tree stays balanced.    
    int getTreeHeight();
};

#endif