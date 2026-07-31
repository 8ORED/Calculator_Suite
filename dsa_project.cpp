#include<iostream>
#include<string>
#include<algorithm>
#include<cmath>
using namespace std;
#define MAX 100
// Stack for operators
class CharStack {
private:
    char st[MAX];
    int top;
public:
    CharStack()
    {
        top=-1;
    }
    void push(char value)
    {
        st[++top]=value;
    }
    char pop()
    {
        return st[top--];
    }
    char peek()
    {
        return st[top];
    }
    bool isEmpty()
    {
        return top==-1;
    }
};

// Stack for evaluation
class IntStack {
private:

    int st[MAX];
    int top;


public:
	
    IntStack()
    {
        top=-1;
    }
    void push(int value)
    {
        st[++top]=value;
    }
    int pop()
    {
        return st[top--];
    }
    bool isEmpty()
    {
        return top==-1;
    }
};
// Check operand
bool isOperand(char ch)
{
    return ch>='0' && ch<='9';
}
// Check operator
bool isOperator(char ch)
{
    return ch=='+'||
           ch=='-'||
           ch=='*'||
           ch=='/'||
           ch=='^';
}
// Precedence
int precedence(char ch)
{
    if(ch=='^')
        return 3;

    if(ch=='*'||ch=='/')
        return 2;

    if(ch=='+'||ch=='-')
        return 1;

    return 0;
}
// Infix to postfix
string infixToPostfix(string exp)
{
    CharStack s;
    string postfix="";
    for(int i=0;i<exp.length();i++)
    {
        char ch=exp[i];
        if(isOperand(ch))
        {
            postfix+=ch;
        }
        else if(ch=='(')
        {
            s.push(ch);
        }
        else if(ch==')')
        {
            while(!s.isEmpty() && s.peek()!='(')
                postfix+=s.pop();

            s.pop();
        }
        else
        {
            while(!s.isEmpty() &&
                  precedence(s.peek())>=precedence(ch))
            {
                postfix+=s.pop();
            }
            s.push(ch);
        }
    }
    while(!s.isEmpty())
        postfix+=s.pop();
    return postfix;
}
// Infix to prefix
string infixToPrefix(string exp)
{
    reverse(exp.begin(),exp.end());
    for(int i=0;i<exp.length();i++)
    {
        if(exp[i]=='(')
            exp[i]=')';
        else if(exp[i]==')')
           exp[i]='(';
    }
    string prefix=infixToPostfix(exp);
    reverse(prefix.begin(),prefix.end());
    return prefix;
}

// Perform calculation
int operation(int a,int b,char op)
{
    switch(op)
    {
        case '+':
            return a+b;
            break;
        case '-':
            return a-b;
			break;
        case '*':
            return a*b;
			break;
        case '/':
            return a/b;
			break;
        case '^':
            return pow(a,b);
			break;
    }
    return 0;
}
// Evaluate postfix
int evaluatePostfix(string exp)
{
    IntStack s;
    for(int i=0;i<exp.length();i++)
    {
        char ch=exp[i];
        if(isOperand(ch))
        {
            s.push(ch-'0');
        }
        else if(isOperator(ch))
        {
            int val1=s.pop();
            int val2=s.pop();
          s.push(operation(val2,val1,ch));
        }
    }
    return s.pop();

}
// Evaluate prefix
int evaluatePrefix(string exp)
{
    IntStack s;
    for(int i=exp.length()-1;i>=0;i--)
    {
        char ch=exp[i];
        if(isOperand(ch))
        {
            s.push(ch-'0');
        }
        else if(isOperator(ch))
        {
			int val1=s.pop();
            int val2=s.pop();
            s.push(operation(val1,val2,ch));
        }
    }
    return s.pop();
}

int main()
{
    string exp;
	cout<<"Enter Infix Expression: ";
    cin>>exp;
    string postfix=infixToPostfix(exp);
    string prefix=infixToPrefix(exp);
    cout<<"\nInfix : "<<exp;
    cout<<"\nPostfix : "<<postfix;
    cout<<"\nPrefix : "<<prefix;
    cout<<"\n\nPostfix Result = "
        <<evaluatePostfix(postfix);


    cout<<"\nPrefix Result = "
        <<evaluatePrefix(prefix);



    return 0;
}
