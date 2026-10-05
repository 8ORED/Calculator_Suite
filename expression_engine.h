#ifndef EXPRESSION_ENGINE_H
#define EXPRESSION_ENGINE_H

#include <string>
using namespace std;

#define MAX 100

// Stack for operators (used during infix -> postfix/prefix

template <typename T>
class Stack {
private:
    T st[MAX];
    int top;

public:
    Stack();
    void push(T value);
    T pop();
    T peek();
    bool isEmpty();
};

// Helper functions

// true if ch is a digit (0-9)
bool isOperand(char ch);

// true if ch is one of + - * / ^
bool isOperator(char ch);

// returns precedence rank of an operator (higher = evaluated first)
int precedence(char ch);

// converts an infix expression string to postfix
string infixToPostfix(string exp);

// converts an infix expression string to prefix
string infixToPrefix(string exp);

// performs a op b for a single operator
template <typename T>
T operation(T a, T b, char op);

// evaluates a postfix expression and returns the result
template <typename T>
T evaluatePostfix(string exp);

// evaluates a prefix expression and returns the result
template <typename T>
T evaluatePrefix(string exp);

#endif // EXPRESSION_ENGINE_H