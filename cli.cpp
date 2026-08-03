#include "CLI.h"
#include <iostream>
#include <limits>

using namespace std;

CLI::CLI()
{
    initQueue(history);
    encodedHistory = "";
    historyText = "";
}

void CLI::showMenu()
{
    cout << "\n=========================================\n";
    cout << "           CALCULATOR SUITE\n";
    cout << "=========================================\n";
    cout << "1. Expression Engine\n";
    cout << "2. Polynomial Addition\n";
    cout << "3. Polynomial Multiplication\n";
    cout << "4. Symbol Table\n";
    cout << "5. Show History\n";
    cout << "6. Encode Result History\n";
    cout << "7. Decode Result History\n";
    cout << "8. Exit\n";
    cout << "=========================================\n";
    cout << "Enter Choice : ";
}

void CLI::processChoice(int choice)
{
    switch(choice)
    {

    case 1:
    {
        int option;

        do
        {
            cout << "\n========== EXPRESSION ENGINE ==========\n";
            cout << "1. Infix to Postfix\n";
            cout << "2. Infix to Prefix\n";
            cout << "3. Evaluate Postfix\n";
            cout << "4. Evaluate Prefix\n";
            cout << "5. Back\n";
            cout << "Enter Choice : ";
            cin >> option;

            switch(option)
            {

            case 1:
            {
                string expression;

                cout << "\nEnter Infix Expression : ";
                cin >> expression;

                string postfix = infixToPostfix(expression);

                cout << "\nPostfix Expression : "
                     << postfix
                     << endl;

                break;
            }

            case 2:
            {
                string expression;

                cout << "\nEnter Infix Expression : ";
                cin >> expression;

                string prefix = infixToPrefix(expression);

                cout << "\nPrefix Expression : "
                     << prefix
                     << endl;

               
                break;
            }

            case 3:
            {
                string expression;

                cout << "\nEnter Postfix Expression : ";
                cin >> expression;

                int result = evaluatePostfix(expression);

                cout << "\nAnswer : "
                     << result
                     << endl;

                break;
            }

            case 4:
            {
                string expression;

                cout << "\nEnter Prefix Expression : ";
                cin >> expression;

                int result = evaluatePrefix(expression);

                cout << "\nAnswer : "
                     << result
                     << endl;

                break;
            }

            case 5:
                break;

            default:
                cout << "\nInvalid Choice.\n";

            }

        }
        while(option != 5);

        break;
    }

    case 2:
    {
        PolyNode* poly1 = nullptr;
        PolyNode* poly2 = nullptr;

        cout << "\nFirst Polynomial\n";
        inputPolynomial(poly1);

        cout << "\nSecond Polynomial\n";
        inputPolynomial(poly2);

        PolyNode* result = addPoly(poly1, poly2);

        cout << "\nResult : ";
        displayPoly(result);
        cout << endl;

        enqueue(history,
                "Polynomial Addition : "
                + polyToString(result));

        clearPoly(poly1);
        clearPoly(poly2);
        clearPoly(result);

        break;
    }

    case 3:
    {
        PolyNode* poly1 = nullptr;
        PolyNode* poly2 = nullptr;

        cout << "\nFirst Polynomial\n";
        inputPolynomial(poly1);

        cout << "\nSecond Polynomial\n";
        inputPolynomial(poly2);

        PolyNode* result = multiplyPoly(poly1, poly2);

        cout << "\nResult : ";
        displayPoly(result);
        cout << endl;

        enqueue(history,
                "Polynomial Multiplication : "
                + polyToString(result));

        clearPoly(poly1);
        clearPoly(poly2);
        clearPoly(result);

        break;
    }
        case 4:
    {
        int option;

        do
        {
            cout << "\n========== SYMBOL TABLE ==========\n";
            cout << "1. Insert Variable\n";
            cout << "2. Search Variable\n";
            cout << "3. Delete Variable\n";
            cout << "4. List Variables\n";
            cout << "5. Display Tree Height\n";
            cout << "6. Back\n";
            cout << "Enter Choice : ";
            cin >> option;

            switch(option)
            {
                case 1:
                {
                    string name;
                    double value;

                    cout << "Variable Name : ";
                    cin >> name;

                    cout << "Value : ";
                    cin >> value;

                    symbols.insert(name, value);

                    cout << "Variable inserted successfully.\n";

                    enqueue(history,"Inserted Variable : "+ name + " = "+ to_string(value));

                    break;
                }

                case 2:
                {
                    string name;
                    double value;

                    cout << "Variable Name : ";
                    cin >> name;

                    if(symbols.search(name, value))
                        cout << name << " = " << value << endl;
                     else
                        cout << "Variable not found.\n";

                    break;
                }

                case 3:
                {
                    string name;

                    cout << "Variable Name : ";
                    cin >> name;

                    if(symbols.remove(name))
                    {
                        cout << "Variable deleted.\n";

                        enqueue(history,"Deleted Variable : "+ name);
                    }
                    else
                    {
                        cout << "Variable not found.\n";
                    }

                    break;
                }

                case 4:
                {
                    vector<pair<string,double>> vars =
                        symbols.listVariables();

                    if(vars.empty())
                    {
                        cout << "No variables stored.\n";
                    }
                    else
                    {
                        cout << "\nVariables\n";
                        cout << "-------------------------\n";

                        for(const auto& var : vars)
                        {
                            cout << var.first<< " = "<< var.second<< endl;
                        }
                    }

                    break;
                }

                case 5:
                {
                    cout << "Tree Height : "<< symbols.getTreeHeight()<< endl;
                    break;
                }

                case 6:
                    break;

                default:
                    cout << "Invalid Choice.\n";
            }

        } while(option != 6);

        break;
    }

    case 5:
    {
        cout << "\n========== HISTORY ==========\n";
        displayHistory(history);
        break;
    }

    case 6:
    {
        if(isEmpty(history))
        {
            cout << "\nNo history available.\n";
            break;
        }

        CircularQueue temp;
        initQueue(temp);

        string item;
        string historyText = "";

        while(dequeue(history, item))
        {
            historyText += item + "\n";
            enqueue(temp, item);
        }

        while(dequeue(temp, item))
        {
            enqueue(history, item);
        }

        encodedHistory = huffman.encode(historyText);

        cout << "\n========== RESULT HISTORY ==========\n";
        cout << historyText << endl;

        cout << "\n========== ENCODED HISTORY ==========\n";
        cout << encodedHistory << endl;

        cout << "\n========== HUFFMAN CODES ==========\n";
        huffman.displayCodes();

        break;
    }

    case 7:
    {
        if(encodedHistory.empty())
        {
            cout << "\nHistory has not been encoded yet.\n";
            break;
        }

        cout << "\n========== DECODED HISTORY ==========\n";

        cout << huffman.decode(encodedHistory)
             << endl;

        break;
    }

    case 8:
    {
        cout << "\nThank You!\n";
        exit(0);
    }

    default:
    {
        cout << "Invalid Choice.\n";
    }

    }
}

void CLI::run()
{
    int choice;

    while(true)
    {
        showMenu();
        cin >> choice;
        processChoice(choice);
    }
}
