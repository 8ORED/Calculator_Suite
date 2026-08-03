#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <string>
#include <limits>

#include "Huffman.h"
#include "data_storage.h"
#include "symbol_table.h"
#include "expression_engine.h"

using namespace std;

class CLI
{
private:

    Huffman huffman;

    SymbolTable symbols;

    CircularQueue history;

    void showMenu();

    void processChoice(int choice);

    string encodedHistory;
    string historyText;

public:

    CLI();

    void run();

};

#endif