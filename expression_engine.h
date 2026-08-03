#ifndef EXPRESSION_ENGINE_H
#define EXPRESSION_ENGINE_H

#include <string>
using namespace std;

#define MAX 100

// Stack for operators (used during infix -> postfix/prefix

class CharStack {
private:
    char st[MAX];
    int top;

public:
    CharStack();
    void push(char value);
    char pop();
    char peek();
    bool isEmpty();
};

// Stack for evaluation (used while evaluating postfix/prefix)
class IntStack {
private:
    int st[MAX];
    int top;

public:
    IntStack();
    void push(int value);
    int pop();
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
int operation(int a, int b, char op);

// evaluates a postfix expression and returns the result
int evaluatePostfix(string exp);

// evaluates a prefix expression and returns the result
int evaluatePrefix(string exp);

#endif // EXPRESSION_ENGINE_H
