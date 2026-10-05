#include "expression_engine.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <vector>
#include <cctype>
#include <stdexcept>
using namespace std;

// CharStack

template <typename T>
Stack<T>::Stack()
{
    top = -1;
}

template <typename T>
void Stack<T>::push(T value)
{
    if (top == MAX - 1) throw runtime_error("Expression too long (stack overflow)");
    st[++top] = value;
}

template <typename T>
T Stack<T>::pop()
{
    if (isEmpty()) throw runtime_error("Invalid expression (empty stack)");
    return st[top--];
}

template <typename T>
T Stack<T>::peek()
{
    if (isEmpty()) throw runtime_error("Invalid expression (empty stack)");
    return st[top];
}

template <typename T>
bool Stack<T>::isEmpty()
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
        else if (isdigit(static_cast<unsigned char>(ch)) || ch == '.')
        {
            string number;
            while (i < exp.length() && (isdigit(static_cast<unsigned char>(exp[i])) || exp[i] == '.'))
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
    return !tok.empty() && (isdigit(static_cast<unsigned char>(tok[0])) || tok[0] == '.');
}

static double parseNumber(const string& tok){
    size_t pos = 0;
    double value;
    try {
        value = stod(tok, &pos);
    } catch (const exception&) {
        throw runtime_error("Invalid number '" + tok + "'");
    }
    if (pos != tok.size())
        throw runtime_error("Invalid number '" + tok + "'");
    return value;
}

string infixToPostfix(string exp){
    vector<string> tokens = tokenize(exp);
    Stack<char> s;
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
            if (s.isEmpty()) throw runtime_error("Mismatched brackets");
            s.pop(); // discard the '('
        }
        else
        {
            char op = tok[0];
            while (!s.isEmpty() && 
                    (precedence(s.peek()) > precedence(op) || 
                        (precedence(s.peek()) == precedence(op) && op != '^')))
            {
                output.push_back(string(1, s.pop()));
            }
            s.push(op);
        }
    }

    while (!s.isEmpty()){
    char c = s.pop();
    if (c == '(') throw runtime_error("Mismatched brackets");
    output.push_back(string(1, c));
    }

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
    Stack<char> s;
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

            if(s.isEmpty()) throw runtime_error("Mismatched brackets");
            s.pop();
        }
        else
        {
            char op = tok[0];
            while (!s.isEmpty() &&
                   (precedence(s.peek()) > precedence(op) ||
                    (precedence(s.peek()) == precedence(op) && op == '^')))
            {
                output.push_back(string(1, s.pop()));
            }
            s.push(op);
        }
    }

    
    while (!s.isEmpty()){
    char c = s.pop();
    if (c == '(') throw runtime_error("Mismatched brackets");
    output.push_back(string(1, c));
    }
    
    reverse(output.begin(), output.end());
    
    string prefix;
    for (size_t i = 0; i < output.size(); i++)
    {
        prefix += output[i];
        if (i + 1 < output.size()) prefix += ' ';
    }
    return prefix;
}

template <typename T>
T operation(T a, T b, char op){
    switch (op)
    {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b == 0) throw runtime_error("Division by zero");
            return a / b;
        case '^':
            return static_cast<T>(pow(a,b));
    }
    return 0;
}

template <typename T>
T evaluatePostfix(string exp){
    Stack<T> s;
    istringstream iss(exp);
    string tok;
    while (iss >> tok)
    {
        if (isNumberToken(tok))
        {
            s.push(static_cast<T>(parseNumber(tok)));
        }
        else if (isOperator(tok[0]))
        {
            T val1 = s.pop();
            T val2 = s.pop();
            s.push(operation<T>(val2, val1, tok[0]));
        }
        else
        {
            throw runtime_error("Unknown token '" + tok + "'");
        }
    }
    T result = s.pop();
    if (!s.isEmpty()) throw runtime_error("Invalid expression (missing operator)");
    return result;
}

template <typename T>
T evaluatePrefix(string exp){
    Stack<T> s;
    istringstream iss(exp);
    vector<string> tokens;
    string tok;
    while (iss >> tok) tokens.push_back(tok);

    for (int i = (int)tokens.size() - 1; i >= 0; i--)
    {
        const string& t = tokens[i];
        if (isNumberToken(t))
        {
            s.push(static_cast<T>(parseNumber(t)));
        }
        else if (isOperator(t[0]))
        {
            T val1 = s.pop();
            T val2 = s.pop();
            s.push(operation<T>(val1, val2, t[0]));
        }
        else
        {
            throw runtime_error("Unknown token '" + t + "'");
        }
    }
    T result = s.pop();
    if (!s.isEmpty()) throw runtime_error("Invalid expression (missing operator)");
    return result;
}

template double evaluatePostfix<double>(string);
template double evaluatePrefix<double>(string);