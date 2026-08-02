#include "person2.h"
#include <iostream>
using namespace std;

// ------------------------------------------------------
// PART 1: POLYNOMIAL USING LINKED LIST
// ------------------------------------------------------

void insertTerm(Node*& head, int coeff, int exponent) {
    if (coeff == 0) return; // don't store zero terms

    Node* newNode = new Node();
    newNode->coeff = coeff;
    newNode->exponent = exponent;
    newNode->next = NULL;

    // Case 1: empty list or new term has the biggest exponent
    if (head == NULL || exponent > head->exponent) {
        newNode->next = head;
        head = newNode;
        return;
    }

    // Case 2: same exponent as head -> combine
    if (head->exponent == exponent) {
        head->coeff += coeff;
        delete newNode;
        if (head->coeff == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        return;
    }

    // Case 3: find correct position in the middle/end
    Node* cur = head;
    while (cur->next != NULL && cur->next->exponent > exponent) {
        cur = cur->next;
    }

    if (cur->next != NULL && cur->next->exponent == exponent) {
        cur->next->coeff += coeff;
        delete newNode;
        if (cur->next->coeff == 0) {
            Node* temp = cur->next;
            cur->next = temp->next;
            delete temp;
        }
        return;
    }

    newNode->next = cur->next;
    cur->next = newNode;
}

void inputPolynomial(Node*& head) {
    int n;
    cout << "How many terms does this polynomial have? ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        int coeff, exponent;
        cout << "Term " << i + 1 << " - enter coefficient: ";
        cin >> coeff;
        cout << "Term " << i + 1 << " - enter exponent: ";
        cin >> exponent;
        insertTerm(head, coeff, exponent);
    }
}

Node* addPoly(Node* poly1, Node* poly2) {
    Node* result = NULL;

    for (Node* p1 = poly1; p1 != NULL; p1 = p1->next) {
        insertTerm(result, p1->coeff, p1->exponent);
    }
    for (Node* p2 = poly2; p2 != NULL; p2 = p2->next) {
        insertTerm(result, p2->coeff, p2->exponent);
    }
    return result;
}

Node* multiplyPoly(Node* poly1, Node* poly2) {
    Node* result = NULL;

    for (Node* p1 = poly1; p1 != NULL; p1 = p1->next) {
        for (Node* p2 = poly2; p2 != NULL; p2 = p2->next) {
            insertTerm(result, p1->coeff * p2->coeff, p1->exponent + p2->exponent);
        }
    }
    return result;
}

void displayPoly(Node* head) {
    if (head == NULL) {
        cout << "0";
        return;
    }

    Node* cur = head;
    bool first = true;

    while (cur != NULL) {
        if (!first) {
            cout << (cur->coeff < 0 ? " - " : " + ");
        } else if (cur->coeff < 0) {
            cout << "-";
        }

        int absCoeff = (cur->coeff < 0) ? -cur->coeff : cur->coeff;

        if (cur->exponent == 0) {
            cout << absCoeff;
        } else {
            if (absCoeff != 1) cout << absCoeff;
            cout << "x";
            if (cur->exponent != 1) cout << "^" << cur->exponent;
        }

        first = false;
        cur = cur->next;
    }
}

string polyToString(Node* head) {
    if (head == NULL) return "0";

    string result = "";
    Node* cur = head;
    bool first = true;

    while (cur != NULL) {
        if (!first) {
            result += (cur->coeff < 0 ? " - " : " + ");
        } else if (cur->coeff < 0) {
            result += "-";
        }

        int absCoeff = (cur->coeff < 0) ? -cur->coeff : cur->coeff;

        if (cur->exponent == 0) {
            result += to_string(absCoeff);
        } else {
            if (absCoeff != 1) result += to_string(absCoeff);
            result += "x";
            if (cur->exponent != 1) {
                result += "^" + to_string(cur->exponent);
            }
        }

        first = false;
        cur = cur->next;
    }
    return result;
}

void clearPoly(Node*& head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}


// ------------------------------------------------------
// PART 2: CIRCULAR QUEUE FOR CALCULATION HISTORY
// ------------------------------------------------------

void initQueue(CircularQueue& q) { 
    q.front = 0;
    q.rear = -1;
    q.count = 0;
}

bool isEmpty(CircularQueue& q) {
    return q.count == 0;
}

bool isFull(CircularQueue& q) {
    return q.count == MAX_HISTORY;
}

void enqueue(CircularQueue& q, string record) {
    if (isFull(q)) {
        q.front = (q.front + 1) % MAX_HISTORY; // drop oldest
        q.count--;
    }
    q.rear = (q.rear + 1) % MAX_HISTORY;
    q.data[q.rear] = record;
    q.count++;
}

bool dequeue(CircularQueue& q, string& removed) {
    if (isEmpty(q)) return false;

    removed = q.data[q.front];
    q.front = (q.front + 1) % MAX_HISTORY;
    q.count--;
    return true;
}

void displayHistory(CircularQueue& q) {
    if (isEmpty(q)) {
        cout << "(history is empty)" << endl;
        return;
    }

    cout << "---- Calculation History ----" << endl;
    for (int i = 0; i < q.count; i++) {
        int index = (q.front + i) % MAX_HISTORY;
        cout << i + 1 << ". " << q.data[index] << endl;
    }
    cout << "------------------------------" << endl;
}
