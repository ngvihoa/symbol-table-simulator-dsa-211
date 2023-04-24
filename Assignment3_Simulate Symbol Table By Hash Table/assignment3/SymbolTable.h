#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"


class SymbolTable
{
public:
    class symbol;
    class table;
private:
    int size;
    int nE;
    table* tab = new table();
public:
    int scope = 0;
    int c1 = 0;
    int c2 = 0;
    string prob = "";
    SymbolTable() {  
        this->size = 0;
        this->nE = 0;
    }
    ~SymbolTable() {
        if (tab) {
            tab->clearT();
        }
        delete tab;
        nE = size = 0;
    }
    void lodSize(int s) {
        size = s;
        tab->initial(s);
    }
    string key(string name, int level);
    void clear(table * A) {
        A->clearT();
        nE = size = 0;
    }
    int h1(string k) {
        int a = 0;
        int n = k.length();
        for (int i = 0; i < n; i++) {
            a = (a * 10 + int(k[i] - '0')) % size;
        }
        return a;
    }
    int h2(string k) {
        int a = 0;
        int n = k.length();
        for (int i = 0; i < n; i++) {
            a = (a * 10 + int(k[i] - '0')) % (size - 2);
        }
        return 1 + a;
    }
    int hL(string k, int i) { return (h1(k) + c1 * i) % size; }
    int hQ(string k, int i) { return (h1(k) + c1 * i + c2 * i * i) % size; }
    int hD(string k, int i) { return (h1(k) + c1 * i * h2(k)) % size; }
    int hp(string k, int i) {
        if (prob == "LINEAR") return hL(k, i);
        else if (prob == "QUADRATIC") return hQ(k, i);
        else return hD(k, i);
    }
    bool checkDec(symbol a) { return tab->checkDecT(a); }
    bool checkOF() { return (nE == size); }
    void add(symbol m, int k) { tab->asg(m, k); }
    void Dltscope() { tab->rest(scope); scope--; }

    void INSERT(symbol a, string ins);
    symbol* findID(string name, const int scope, int& num);
    void ASSIGN(string ins);
    void CALL(string ins);
    void LOOKUP(string name);
    void PRINT() { tab->print(); }
    void run(string filename);
public:
    class symbol {
        string identifiers = "";
        int lvl = -1;
        string type = "";
        string value = "";
        string k = "";
        int numP = -1;
        symbol* next;
        friend class SymbolTable;
        friend class table;
    public:
        symbol(string id, int lvl, string type = "", string value = "", string k = "", int num = -1, symbol* next = nullptr) {
            this->identifiers = id;
            this->lvl = lvl;
            this->type = type;
            this->value = value;
            this->k = k;
            this->numP = num;
            this->next = next;
        }
        void reini() {
            this->identifiers = "";
            this->lvl = -1;
            this->type = "";
            this->value = "";
            this->k = "";
            this->numP = -1;
        }
    };

    class table {
        symbol* head;
        symbol* tail;
        friend class SymbolTable;
    public:
        table() {
            head = tail = nullptr;
        }
        ~table() {
            while (head) {
                symbol* tmp = head;
                head = head->next;
                delete tmp;
            }
            tail = nullptr;
        }
        void clearT() {
            while (head) {
                symbol* tmp = head;
                head = head->next;
                delete tmp;
            }
            tail = nullptr;
        }
        void initial(int n) {
            for (int i = 0; i < n; i++) {
                if (head == nullptr) {
                    head = tail = new symbol("", -1);
                }
                else {
                    tail->next = new symbol("", -1);
                    tail = tail->next;
                }
            }
        }
        symbol* at(int k) {
            symbol* tmp = head;
            while (k > 0) {
                tmp = tmp->next;
                k--;
            }
            return tmp;
        }
        bool checkDecT(symbol a) {
            symbol* tmp = head;
            while(tmp) {
                if (tmp->identifiers == a.identifiers && tmp->lvl == a.lvl) { return false; }
                tmp = tmp->next;
            }
            delete tmp;
            return true;
        }
        void asg(symbol m, int k) {
            symbol* tmp = head;
            while (k > 0) {
                tmp = tmp->next;
                k--;
            }
            tmp->identifiers = m.identifiers;
            tmp->lvl = m.lvl;
            tmp->type = m.type;
            tmp->value = m.value;
            tmp->k = m.k;
            tmp->numP = m.numP;
        }
        int getLvl(int k) {
            symbol* tmp = head;
            while (k > 0) {
                tmp = tmp->next;
                k--;
            }
            return tmp->lvl;
        }
        void rest(const int sc) {
            symbol* tmp = head;
            while (tmp) {
                if (tmp->lvl == sc) {
                    tmp->reini();
                }
                tmp = tmp->next;
            }
        }
        int lookup(string name, const int scope) {
            symbol* tmp = head;
            int k = 0;
            int index = -1;
            int sc = -1;
            while (tmp) {
                if (tmp->identifiers == name) {
                    if (tmp->lvl == scope) {
                        return k;
                    }
                    if (tmp->lvl > sc) {
                        sc = tmp->lvl;
                        index = k;
                    }
                }
                tmp = tmp->next;
                k++;
            }
            return index;
        }
        void print() {
            bool print_s = false;
            int k = 0;
            bool p = false;
            symbol* tmp = head;
            while (tmp) {
                if (tmp->lvl != -1) {
                    print_s = true;
                    if (p) cout << ";";
                    else p = true;
                    cout << k << " " << tmp->identifiers << "//" << tmp->lvl;
                }
                k++;
                tmp = tmp->next;
            }
            delete tmp;
            if(print_s) cout << endl;
        }

    };

};

#endif