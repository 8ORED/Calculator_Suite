#include "expression_engine.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <vector>
#include <cctype>
using namespace std;

// CharStack

CharStack::CharStack()
{
    top = -1;
}

void CharStack::push(char value)
{
    st[++top] = value;
}

char CharStack::pop()
{
    return st[top--];
}

char CharStack::peek()
{
    return st[top];
}

bool CharStack::isEmpty()
{
    return top == -1;
}

// IntStack

IntStack::IntStack()
{
    top = -1;
}

void IntStack::push(int value)
{
    st[++top] = value;
}

int IntStack::pop()
{
    return st[top--];
}

bool IntStack::isEmpty()
{
    return top == -1;
}

// Helper functions
bool isOperand(char ch)
{
    return ch >= '0' && ch <= '9';
}

bool isOperator(char ch)
{
    return ch == '+' ||
           ch == '-' ||
           ch == '*' ||
           ch == '/' ||
           ch == '^';
}

int precedence(char ch)
{
    if (ch == '^')
        return 3;

    if (ch == '*' || ch == '/')
        return 2;

    if (ch == '+' || ch == '-')
        return 1;

    return 0;
}

static vector<string> tokenize(const string& exp){
    vector<string> tokens;
    size_t i = 0;
    while (i < exp.length())
    {
        char ch = exp[i];
        if (isspace(static_cast<unsigned char>(ch)))
        {
            i++;
        }
        else if (isdigit(static_cast<unsigned char>(ch)))
        {
            string number;
            while (i < exp.length() && isdigit(static_cast<unsigned char>(exp[i])))
            {
                number += exp[i];
                i++;
            }
            tokens.push_back(number);
        }
        else if (ch == '(' || ch == ')' || isOperator(ch))
        {
            tokens.push_back(string(1, ch));
            i++;
        }
        else
        {
            // Unrecognized character (should not occur if the CLI has
            // already substituted variable names before calling this).
            i++;
        }
    }
    return tokens;    
}

static bool isNumberToken(const string& tok){
    return !tok.empty() && isdigit(static_cast<unsigned char>(tok[0]));
}

string infixToPostfix(string exp){
    vector<string> tokens = tokenize(exp);
    CharStack s;
    vector<string> output;

    for (const string& tok : tokens)
    {
        if (isNumberToken(tok))
        {
            output.push_back(tok);
        }
        else if (tok == "(")
        {
            s.push('(');
        }
        else if (tok == ")")
        {
            while (!s.isEmpty() && s.peek() != '(')
                output.push_back(string(1, s.pop()));

            s.pop(); // discard the '('
        }
        else
        {
            char op = tok[0];
            while (!s.isEmpty() &&
                   precedence(s.peek()) >= precedence(op))
            {
                output.push_back(string(1, s.pop()));
            }
            s.push(op);
        }
    }
    while (!s.isEmpty())
        output.push_back(string(1, s.pop()));

    string postfix;
    for (size_t i = 0; i < output.size(); i++)
    {
        postfix += output[i];
        if (i + 1 < output.size()) postfix += ' ';
    }
    return postfix;
}

string infixToPrefix(string exp){
    vector<string> tokens = tokenize(exp);
    reverse(tokens.begin(), tokens.end());
    for (string& tok : tokens)
    {
        if (tok == "(") tok = ")";
        else if (tok == ")") tok = "(";
    }

    // Same postfix-building logic as infixToPostfix, applied to the
    // reversed/paren-swapped token list, then the result is reversed.
    CharStack s;
    vector<string> output;
    for (const string& tok : tokens)
    {
        if (isNumberToken(tok))
        {
            output.push_back(tok);
        }
        else if (tok == "(")
        {
            s.push('(');
        }
        else if (tok == ")")
        {
            while (!s.isEmpty() && s.peek() != '(')
                output.push_back(string(1, s.pop()));
            s.pop();
        }
        else
        {
            char op = tok[0];
            while (!s.isEmpty() &&
                   precedence(s.peek()) >= precedence(op))
            {
                output.push_back(string(1, s.pop()));
            }
            s.push(op);
        }
    }
    while (!s.isEmpty())
        output.push_back(string(1, s.pop()));

    reverse(output.begin(), output.end());

    string prefix;
    for (size_t i = 0; i < output.size(); i++)
    {
        prefix += output[i];
        if (i + 1 < output.size()) prefix += ' ';
    }
    return prefix;
}

int operation(int a, int b, char op){
    switch (op)
    {
        case '+':
            return a + b;
            break;
        case '-':
            return a - b;
            break;
        case '*':
            return a * b;
            break;
        case '/':
            return a / b;
            break;
        case '^':
            return pow(a, b);
            break;
    }
    return 0;
}

int evaluatePostfix(string exp){
    IntStack s;
    istringstream iss(exp);
    string tok;
    while (iss >> tok)
    {
        if (isNumberToken(tok))
        {
            s.push(stoi(tok));
        }
        else if (isOperator(tok[0]))
        {
            int val1 = s.pop();
            int val2 = s.pop();
            s.push(operation(val2, val1, tok[0]));
        }
    }
    return s.pop();
}

int evaluatePrefix(string exp){
    IntStack s;
    istringstream iss(exp);
    vector<string> tokens;
    string tok;
    while (iss >> tok) tokens.push_back(tok);

    for (int i = (int)tokens.size() - 1; i >= 0; i--)
    {
        const string& t = tokens[i];
        if (isNumberToken(t))
        {
            s.push(stoi(t));
        }
        else if (isOperator(t[0]))
        {
            int val1 = s.pop();
            int val2 = s.pop();
            s.push(operation(val1, val2, t[0]));
        }
    }
    return s.pop();
}