#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <string>
using namespace std;

// ------------------------------------------------------
// PART 1: POLYNOMIAL USING LINKED LIST
// ------------------------------------------------------

struct Node {
    int coeff;
    int exponent;
    Node* next;
};


void insertTerm(Node*& head, int coeff, int exponent);

void inputPolynomial(Node*& head);

// returns head of a new polynomial = poly1 + poly2
Node* addPoly(Node* poly1, Node* poly2);

// returns head of a new polynomial = poly1 * poly2
Node* multiplyPoly(Node* poly1, Node* poly2);

// prints polynomial like: 5x^3 + 2x^2 - 3x + 7
void displayPoly(Node* head);

// same as displayPoly, but RETURNS the text instead of
// printing it - used so a result can be saved into history.
string polyToString(Node* head);

// frees all nodes in a polynomial list
void clearPoly(Node*& head);

// PART 2: CIRCULAR QUEUE FOR CALCULATION HISTORY

const int MAX_HISTORY = 10; // fixed size of history

struct CircularQueue {
    string data[MAX_HISTORY];
    int front;
    int rear;
    int count;
};

void initQueue(CircularQueue& q);
bool isEmpty(CircularQueue& q);
bool isFull(CircularQueue& q);

// adds a new record; if full, overwrites the oldest one
void enqueue(CircularQueue& q, string record);

// removes the oldest record, stores it in "removed"
bool dequeue(CircularQueue& q, string& removed);

// prints all records, oldest first
void displayHistory(CircularQueue& q);

#endif // DATA_STORAGE_H
