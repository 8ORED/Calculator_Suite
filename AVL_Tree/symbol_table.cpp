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

    // Height update (Convention B: nullptr counts as height -1)
    int leftHeight = node->left ? node->left->height : -1;
    int rightHeight = node->right ? node->right->height : -1;
    node->height = 1 + std::max(leftHeight, rightHeight);

    return node;
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