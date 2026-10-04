# Calculator_Suite

A command-line calculator toolkit written in C++ that brings several classic data structures and algorithms together in one interactive program. It supports variables, infix-to-postfix/prefix expression evaluation, polynomial arithmetic, a calculation history log, and Huffman compression of that log.

## Features

- **Variables (Symbol Table):** assign, list (sorted), and clear named variables.
- **Expression evaluation:** convert infix expressions to **postfix** or **prefix** form and evaluate them (integer arithmetic).
- **Variable substitution:** use stored variables directly inside expressions.
- **Polynomial arithmetic:** build two polynomials interactively, then **add** or **multiply** them (linked-list representation).
- **Calculation history:** every evaluation and polynomial operation is recorded in a **circular queue**.
- **Huffman coding:** compress the history log into a bitstring, view the code table and space saved, and decode it back.

## Data Structures and Algorithms Used

| Feature | Concept |
|---|---|
| Variable storage | Symbol table (insert / search / remove / sorted listing) |
| Infix to postfix / prefix | Stack-based expression conversion |
| Postfix / prefix evaluation | Stack-based evaluation |
| Polynomials | Singly linked list (`PolyNode`) |
| History log | Circular queue |
| Compression | Huffman coding (frequency table, code tree, encode / decode) |

## Project Structure

```
.
├── main.cpp                  # CLI loop and command handlers
├── symbol_table.h / .cpp     # SymbolTable class (variables)
├── expression_engine.h / .cpp# infix/postfix/prefix conversion and evaluation
├── data_storage.h / .cpp     # CircularQueue (history) and polynomial linked list
├── huffman.h / .cpp          # Huffman encoder / decoder
├── setup.bat                 # Windows setup script
├── .gitignore
├── LICENSE
└── README.md
```

## Getting Started

### Prerequisites

- A C++ compiler with C++11 support or newer (e.g. `g++`, `clang++`)

### Build

```bash
g++ -std=c++11 -o calc-suite main.cpp symbol_table.cpp expression_engine.cpp data_storage.cpp huffman.cpp
```

On Windows you can also run `setup.bat`.

### Run

```bash
./calc-suite          # Linux / macOS
calc-suite.exe        # Windows
```

## Commands

| Command | Description |
|---|---|
| `<name> = <value>` | Assign a variable (e.g. `x = 5`) |
| `eval <expr>` | Convert to postfix and evaluate (e.g. `eval 3+4*2`) |
| `prefix <expr>` | Convert to prefix and evaluate |
| `list-vars` | Show all stored variables (sorted) |
| `clear <name>` | Remove a variable |
| `history` | Show calculation history |
| `poly` | Build two polynomials, then add or multiply them |
| `compress` | Huffman-compress the current history log |
| `decompress` | Decode the most recently compressed log |
| `help` | Show the command list |
| `quit` / `exit` | Exit the program |

## Notes and Limitations

- Variables used inside `eval` / `prefix` expressions must hold **whole numbers**, because the expression engine evaluates using integers. A non-integer value (e.g. `z = 2.5`) can be stored but will produce an error when used in an expression.
- Referencing an undefined variable in an expression prints an error and the expression is not evaluated.
- `decompress` requires that `compress` has been run first in the same session.
- The history log and variables live in memory only and are not saved between sessions.

## Future Improvements

- Floating-point expression support
- Persisting variables and history to disk
- Polynomial evaluation and differentiation
- Unit tests for each module

## License

This project is licensed under the terms of the [LICENSE](LICENSE) file.