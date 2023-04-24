#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class symbol {
private:
    string id = "";
    string type = "";
    string value = "";
    int Level;
    symbol* left;
    symbol* right;
    symbol* parent;
    friend class SymbolTable;

public:
    symbol(const string id, const string typ, const string val, const int lev, symbol* L = nullptr, symbol* R = nullptr, symbol* P = nullptr)
        : id(id), type(typ), value(val), Level(lev), left(L), right(R), parent(P) {}
};

template<typename T>
class Qu {
public:
    class cell;
protected:
    cell* head;
    cell* tail;
    int count;
    void clear() {
        while (head != nullptr) {
            cell* tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
        count = 0;
    }
public:
    Qu<T>(){
        this->count = 0;
        this->head = nullptr;
        this->tail = nullptr;
    }
    ~Qu<T>() {
        clear();
    }
    void enq(T& a) {
        if (count == 0) {
            head = tail = new cell(a);
        }
        else {
            tail->next = new cell(a);
            tail = tail->next;
        }
        count++;
    }
    void deq() {
        if (count == 0) return;
        else if (count == 1) {
            cell* tmp = head;
            head = tail = nullptr;
            count--;
            delete tmp;
        }
        else {
            cell* a = head;
            head = head->next;
            count--;
            delete a;
        }
    }
    cell* front() { return head; }
    int length() {
        return count;
    }
    bool isEmpty() {
        if (count == 0) return true;
        return false;
    }
    
public:
    class cell {
        T data;
        cell* next;
        friend class Qu<T>;
    public:
        cell (T& d, cell* n = nullptr) { 
            this->data = d;
            this->next = n;
        }
        T& loadDt() {
            return this->data;
        }
    };
};

template<class T>
class ST {
public:
    class Node;
protected:
    Node* top;
    int size;
    void clear() {
        while (top) {
            Node* tmp = top;
            top = top->next;
            delete tmp;
        }
        size = 0;
    }
public:
    ST<T>() { 
        this->size = 0;
        this->top = nullptr;
    }
    ~ST<T>() {
        clear();
    }
    void push(T& a) {
        if (size == 0) top = new Node(a);
        else {
            Node* tmp = new Node(a, top);
            top = tmp;
        }
        size++;
    }
    void pop() {
        if (size == 0) return;
        Node* a = top;
        top = top->next;
        size--;
        delete a;
    }
    Node* Top() {
        return this->top;
    }
    Node* first() {
        if (size == 0) return nullptr;
        Node* tmp = top;
        while (tmp->next != nullptr) {
            tmp = tmp->next;
        }
        return tmp;
    }
    int length() {
        return size;
    }
    bool isEmpty() {
        if (size == 0) return true;
        return false;
    }  
public:
    class Node {
    private:
        T data;
        Node* next;
        friend class ST<T>;
    public:
        Node(T& d, Node* n = nullptr) {
            this->data = d;
            this->next = n;
        }
        T& LoadDT() {
            return this->data;
        }
    };

};

class SymbolTable
{
private:
    symbol* pRoot;  
    ST<Qu<string>> scope;
public:
    int level = 0;
    int num_compare = 0;
    int num_splay = 0;
    SymbolTable() {
        pRoot = nullptr;
        Qu<string> a;
        scope.push(a);
    }
    ~SymbolTable() {}
    void clear(symbol* pR) {
        if (pR) {
            if (pR->right) {
                clear(pR->right);
            }
            if (pR->left) {
                clear(pR->left);
            }
            delete pR;
        }
        pRoot = nullptr;
    }
    void RotRight(symbol* pR);
    void RotLeft(symbol* pR);
    void LeftZigZag(symbol* pR);
    void RightZigZag(symbol* pR);
    void D_Right(symbol* pR);
    void D_Left(symbol* pR);
    void splay(symbol* pR);
    symbol* insert(symbol*& pR, string id, string type, string state, symbol* par);
    void INSERT(symbol*& pR, string id, string type, string state);
    symbol* checkExistFunc(symbol* pR, string name);
    symbol* find(symbol* pR, string id, int lev);
    symbol* find_no_splay(const string id);
    void ASSIGN(string id, string value);
    symbol* MaxLeft(symbol* pR);
    symbol* find4LK_RNL(symbol* pR, string id);
    void END(symbol* pR);
    void LOOKUP(symbol* pR, string id);
    void PRINT(symbol* pR);
    void run(string filename);   
};
#endif