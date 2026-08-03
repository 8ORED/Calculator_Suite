#include <iostream>
#include <sstream>
#include <cmath>
#include "symbol_table.h"
#include "expression_engine.h"
#include "data_storage.h"
#include "huffman.h"

using namespace std;

// ---- shared module state for this CLI session ----
static SymbolTable symbolTable;
static CircularQueue history;
static Huffman huffman;
static string lastEncoded = ""; // most recently compressed bitstring, for "decompress"

// ------------------------------------------------------
// Helper: substitute single-letter variable names inside an
// expression string with their stored digit value (0-9 only).
// Returns true on success; on failure, prints an error and returns false.
// ------------------------------------------------------
bool substituteVariables(string& expr) {
    for (size_t i = 0; i < expr.length(); i++) {
        char ch = expr[i];
        if (isalpha(static_cast<unsigned char>(ch))) {
            string name(1, ch);
            double value;
            if (!symbolTable.search(name, value)) {
                cout << "Error: undefined variable '" << name << "'" << endl;
                return false;
            }
            if (value != floor(value) || value < 0 || value > 9) {
                cout << "Error: variable '" << name << "' = " << value
                     << " cannot be used in an expression (must be a whole number 0-9, "
                     << "since the expression engine only supports single-digit operands)"
                     << endl;
                return false;
            }
            expr[i] = static_cast<char>('0' + static_cast<int>(value));
        }
    }
    return true;
}

// ------------------------------------------------------
// Command handlers
// ------------------------------------------------------

void handleEval(const string& rawExpr) {
    string expr = rawExpr;
    string originalExpr = rawExpr; // keep original (with variable names) for history/logging

    if (!substituteVariables(expr)) return;

    string postfix = infixToPostfix(expr);
    int result = evaluatePostfix(postfix);

    cout << "Postfix: " << postfix << endl;
    cout << "Result:  " << result << endl;

    ostringstream record;
    record << originalExpr << " = " << result;
    enqueue(history, record.str());
}

void handlePrefixEval(const string& rawExpr) {
    string expr = rawExpr;
    string originalExpr = rawExpr;

    if (!substituteVariables(expr)) return;

    string prefix = infixToPrefix(expr);
    int result = evaluatePrefix(prefix);

    cout << "Prefix: " << prefix << endl;
    cout << "Result: " << result << endl;

    ostringstream record;
    record << originalExpr << " = " << result << " (prefix)";
    enqueue(history, record.str());
}

void handleAssignment(const string& name, double value) {
    symbolTable.insert(name, value);
    cout << name << " = " << value << endl;
}

void handleClear(const string& name) {
    if (symbolTable.remove(name)) {
        cout << "Cleared variable: " << name << endl;
    } else {
        cout << "No such variable: " << name << endl;
    }
}

void handleListVars() {
    auto vars = symbolTable.listVariables();
    if (vars.empty()) {
        cout << "No variables defined." << endl;
        return;
    }
    cout << "Variables (sorted):" << endl;
    for (auto& pair : vars) {
        cout << "  " << pair.first << " = " << pair.second << endl;
    }
}

void handleHistory() {
    displayHistory(history);
}

void handlePoly() {
    cout << "\n-- Polynomial 1 --" << endl;
    PolyNode* poly1 = NULL;
    inputPolynomial(poly1);

    cout << "\n-- Polynomial 2 --" << endl;
    PolyNode* poly2 = NULL;
    inputPolynomial(poly2);

    cout << "\nPolynomial 1: ";
    displayPoly(poly1);
    cout << "\nPolynomial 2: ";
    displayPoly(poly2);
    cout << endl;

    cout << "\nChoose operation - (a)dd or (m)ultiply: ";
    char op;
    cin >> op;

    PolyNode* result = NULL;
    string opName;
    if (op == 'a' || op == 'A') {
        result = addPoly(poly1, poly2);
        opName = "add";
    } else if (op == 'm' || op == 'M') {
        result = multiplyPoly(poly1, poly2);
        opName = "multiply";
    } else {
        cout << "Unknown operation." << endl;
        clearPoly(poly1);
        clearPoly(poly2);
        return;
    }

    cout << "Result (" << opName << "): ";
    displayPoly(result);
    cout << endl;

    ostringstream record;
    record << "poly " << opName << ": (" << polyToString(poly1) << ") and ("
           << polyToString(poly2) << ") = " << polyToString(result);
    enqueue(history, record.str());

    clearPoly(poly1);
    clearPoly(poly2);
    clearPoly(result);
}

// Builds one combined text block out of the current history queue,
// used as the input text for Huffman compression.
string historyToText() {
    string text;
    // temporarily drain into a local copy so we don't disturb the real queue
    CircularQueue snapshot = history;
    string entry;
    bool first = true;
    while (dequeue(snapshot, entry)) {
        if (!first) text += "\n";
        text += entry;
        first = false;
    }
    return text;
}

void handleCompress() {
    string text = historyToText();
    if (text.empty()) {
        cout << "History is empty - nothing to compress." << endl;
        return;
    }

    lastEncoded = huffman.encode(text);

    int originalBits = static_cast<int>(text.length()) * 8;
    int compressedBits = static_cast<int>(lastEncoded.length());

    cout << "Original size:   " << originalBits << " bits (" << text.length() << " chars)" << endl;
    cout << "Compressed size: " << compressedBits << " bits" << endl;
    if (originalBits > 0) {
        double ratio = 100.0 * (1.0 - (double)compressedBits / originalBits);
        cout << "Space saved:     " << ratio << "%" << endl;
    }
    huffman.displayCodes();
}

void handleDecompress() {
    if (lastEncoded.empty()) {
        cout << "Nothing to decompress - run 'compress' first." << endl;
        return;
    }
    string decoded = huffman.decode(lastEncoded);
    cout << "Decoded text:" << endl << decoded << endl;
}

void printHelp() {
    cout << "\nCalc-Suite commands:" << endl;
    cout << "  <name> = <value>    assign a variable (e.g. x = 5)" << endl;
    cout << "  eval <expr>         convert to postfix and evaluate (e.g. eval 3+4*2)" << endl;
    cout << "  prefix <expr>       convert to prefix and evaluate" << endl;
    cout << "  list-vars           show all stored variables" << endl;
    cout << "  clear <name>        remove a variable" << endl;
    cout << "  history             show calculation history" << endl;
    cout << "  poly                build two polynomials and add/multiply them" << endl;
    cout << "  compress            Huffman-compress the current history log" << endl;
    cout << "  decompress          decode the most recently compressed log" << endl;
    cout << "  help                show this message" << endl;
    cout << "  quit                exit" << endl;
    cout << "\nNote: variables used inside eval/prefix expressions must have" << endl;
    cout << "single-letter names and whole-number values from 0-9, since the" << endl;
    cout << "expression engine parses one character per token." << endl << endl;
}

// ------------------------------------------------------
// Main command loop
// ------------------------------------------------------
int main() {
    initQueue(history);

    cout << "=== Calc-Suite ===" << endl;
    cout << "Type 'help' for a list of commands." << endl;

    string line;
    cout << "\n> ";
    while (getline(cin, line)) {
        istringstream iss(line);
        string first;
        iss >> first;

        if (first.empty()) {
            // do nothing
        } else if (first == "quit" || first == "exit") {
            break;
        } else if (first == "help") {
            printHelp();
        } else if (first == "eval") {
            string expr;
            getline(iss, expr);
            // strip leading space
            size_t start = expr.find_first_not_of(' ');
            if (start != string::npos) expr = expr.substr(start);
            if (expr.empty()) {
                cout << "Usage: eval <expression>" << endl;
            } else {
                handleEval(expr);
            }
        } else if (first == "prefix") {
            string expr;
            getline(iss, expr);
            size_t start = expr.find_first_not_of(' ');
            if (start != string::npos) expr = expr.substr(start);
            if (expr.empty()) {
                cout << "Usage: prefix <expression>" << endl;
            } else {
                handlePrefixEval(expr);
            }
        } else if (first == "clear") {
            string name;
            iss >> name;
            if (name.empty()) {
                cout << "Usage: clear <name>" << endl;
            } else {
                handleClear(name);
            }
        } else if (first == "list-vars") {
            handleListVars();
        } else if (first == "history") {
            handleHistory();
        } else if (first == "poly") {
            handlePoly();
        } else if (first == "compress") {
            handleCompress();
        } else if (first == "decompress") {
            handleDecompress();
        } else {
            // check for "<name> = <value>" assignment pattern
            string eq;
            iss >> eq;
            if (eq == "=") {
                double value;
                if (iss >> value) {
                    handleAssignment(first, value);
                } else {
                    cout << "Usage: <name> = <value>" << endl;
                }
            } else {
                cout << "Unknown command: '" << first << "'. Type 'help' for commands." << endl;
            }
        }

        cout << "\n> ";
    }

    cout << "Goodbye." << endl;
    return 0;
}