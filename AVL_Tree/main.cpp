#include <iostream>
#include "symbol_table.h"

int main() {
    SymbolTable table;

    table.insert("x", 5);
    table.insert("y", 10);
    table.insert("z", 15);
    table.insert("x", 8); // reassignment test — should update, not duplicate

    double val;
    if (table.search("x", val)) {
        std::cout << "x = " << val << std::endl; // expect 8
    } else {
        std::cout << "x not found" << std::endl;
    }

    if (table.search("w", val)) {
        std::cout << "w = " << val << std::endl;
    } else {
        std::cout << "w not found (correct)" << std::endl;
    }

    std::cout << "All variables (sorted):" << std::endl;
    for (auto& pair : table.listVariables()) {
        std::cout << "  " << pair.first << " = " << pair.second << std::endl;
    }

    return 0;
}