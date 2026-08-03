#include "expression_engine.h"
#include <algorithm>
#include <cmath>
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

string infixToPostfix(string exp)
{
    CharStack s;
    string postfix = "";
    for (int i = 0; i < (int)exp.length(); i++)
    {
        char ch = exp[i];
        if (isOperand(ch))
        {
            postfix += ch;
        }
        else if (ch == '(')
        {
            s.push(ch);
        }
        else if (ch == ')')
        {
            while (!s.isEmpty() && s.peek() != '(')
                postfix += s.pop();

            s.pop();
        }
        else
        {
            while (!s.isEmpty() &&
                   precedence(s.peek()) >= precedence(ch))
            {
                postfix += s.pop();
            }
            s.push(ch);
        }
    }
    while (!s.isEmpty())
        postfix += s.pop();
    return postfix;
}

string infixToPrefix(string exp)
{
    reverse(exp.begin(), exp.end());
    for (int i = 0; i < (int)exp.length(); i++)
    {
        if (exp[i] == '(')
            exp[i] = ')';
        else if (exp[i] == ')')
            exp[i] = '(';
    }
    string prefix = infixToPostfix(exp);
    reverse(prefix.begin(), prefix.end());
    return prefix;
}

int operation(int a, int b, char op)
{
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

int evaluatePostfix(string exp)
{
    IntStack s;
    for (int i = 0; i < (int)exp.length(); i++)
    {
        char ch = exp[i];
        if (isOperand(ch))
        {
            s.push(ch - '0');
        }
        else if (isOperator(ch))
        {
            int val1 = s.pop();
            int val2 = s.pop();
            s.push(operation(val2, val1, ch));
        }
    }
    return s.pop();
}

int evaluatePrefix(string exp)
{
    IntStack s;
    for (int i = (int)exp.length() - 1; i >= 0; i--)
    {
        char ch = exp[i];
        if (isOperand(ch))
        {
            s.push(ch - '0');
        }
        else if (isOperator(ch))
        {
            int val1 = s.pop();
            int val2 = s.pop();
            s.push(operation(val1, val2, ch));
        }
    }
    return s.pop();
}