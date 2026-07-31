#include "symbol_table.h"
#include <algorithm>

SymbolTable::SymbolTable() : root(nullptr) {}

SymbolTable::~SymbolTable() {
    destroy(root);
}

void SymbolTable::destroy(Node* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// AVL balancing helpers

int SymbolTable::getHeight(Node* node) {
    if (node == nullptr) return 0; // Convention A: empty subtree has height 0
    return node->height;
}
 
int SymbolTable::getBalanceFactor(Node* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}
 
void SymbolTable::updateHeight(Node* node) {
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
}

Node* SymbolTable::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
 
    // perform rotation
    x->right = y;
    y->left = T2;
 
    // update heights — y moved down, so update it first, then x
    updateHeight(y);
    updateHeight(x);
 
    return x; // new subtree root
}

Node* SymbolTable::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
 
    // perform rotation
    y->left = x;
    x->right = T2;
 
    // update heights — x moved down, so update it first, then y
    updateHeight(x);
    updateHeight(y);
 
    return y; // new subtree root
}

// Checks the balance factor of a node and applies the correct rotation
// (or combination of two rotations) to restore the AVL property.
Node* SymbolTable::balance(Node* node) {
    updateHeight(node);
    int bf = getBalanceFactor(node);
 
    // Left-heavy
    if (bf > 1) {
        if (getBalanceFactor(node->left) < 0) {
            // Left-Right case: rotate left child left first, then rotate node right
            node->left = rotateLeft(node->left);
        }
        // Left-Left case (or after LR fix above): rotate node right
        return rotateRight(node);
    }
 
    // Right-heavy
    if (bf < -1) {
        if (getBalanceFactor(node->right) > 0) {
            // Right-Left case: rotate right child right first, then rotate node left
            node->right = rotateRight(node->right);
        }
        // Right-Right case (or after RL fix above): rotate node left
        return rotateLeft(node);
    }
 
    // already balanced
    return node;
}

Node* SymbolTable::insertHelper(Node* node, const std::string& name, double value) {
    if (node == nullptr) {
        return new Node(name, value);
    }

    if (name < node->name) {
        node->left = insertHelper(node->left, name, value);
    } else if (name > node->name) {
        node->right = insertHelper(node->right, name, value);
    } else {
        // variable already exists -> update its value (reassignment case)
        node->value = value;
    }

    // On the way back up the recursion: update height, then rebalance this node
    return balance(node);
}

Node* SymbolTable::searchHelper(Node* node, const std::string& name) {
    if (node == nullptr || node->name == name) {
        return node;
    }
    if (name < node->name) {
        return searchHelper(node->left, name);
    }
    return searchHelper(node->right, name);
}

void SymbolTable::inorderHelper(Node* node, std::vector<std::pair<std::string, double>>& result) {
    if (node == nullptr) return;
    inorderHelper(node->left, result);
    result.push_back({node->name, node->value});
    inorderHelper(node->right, result);
}

void SymbolTable::insert(const std::string& name, double value) {
    root = insertHelper(root, name, value);
}

bool SymbolTable::search(const std::string& name, double& outValue) {
    Node* found = searchHelper(root, name);
    if (found == nullptr) return false;
    outValue = found->value;
    return true;
}

bool SymbolTable::remove(const std::string& name) {
    // TODO (Day 4): implement BST delete + rebalancing
    return false;
}

std::vector<std::pair<std::string, double>> SymbolTable::listVariables() {
    std::vector<std::pair<std::string, double>> result;
    inorderHelper(root, result);
    return result;
}

int SymbolTable::getTreeHeight() {
    return getHeight(root);
}