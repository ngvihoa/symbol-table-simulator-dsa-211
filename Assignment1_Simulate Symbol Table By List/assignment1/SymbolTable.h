#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

//----------------------------------EXTENDED CLASS---------------------------------


class Data {
    string identifier;
    string value;
    string type;
    int scope;
public:
    Data() {
        this->identifier = "";
        this->value = "";
        this->type = "";
        this->scope = 0;
    }
    Data(string id, string value, string a, int s) {
        this->identifier = id;
        this->value = value;
        this->type = a;
        this->scope = s;
    }
    //-----------------Load data-------------------
    void LoadID(string id) {
        this->identifier = id;
    }
    void LoadValue(string id) {
        this->value = id;
    }
    void LoadType(string a) {
        this->type = a;
    }
    void LoadScope(int id) {
        this->scope = id;
    }
    //----------------get data--------------------
    string getID() {
        return identifier;
    }
    string getValue() {
        return value;
    }
    string getType() {
        return type;
    }
    int getScope() {
        return scope;
    }
};

class symbol {
    Data data;
    bool isNull;
    symbol* nextsc;
    symbol* next;
public:
    symbol() {
        data = Data();
        isNull = true;
        nextsc = NULL;
        next = NULL;
    }
    
    void LoadData(Data a) {
        this->data.LoadID(a.getID());
        this->data.LoadValue(a.getValue());
        this->data.LoadType(a.getType());
        this->data.LoadScope(a.getScope());
    }
    Data getData() {
        return data;
    }
    void LoadNext(symbol* a) {
        this->next = a;
    }
    symbol* getNext() {
        return next;
    }
    void LoadIsNull(bool a) {
        isNull = a;
    }
    bool getIsNull() {
        return isNull;
    }

    void LoadSC(symbol* a) {
        this->nextsc = a;
    }
    symbol* getSC() {
        return nextsc;
    }
};

//-------------------------------MAIN CLASS-------------------------------------

class SymbolTable
{
    symbol* LL;
public:
    int scope = 0;
    SymbolTable() {
        LL = new symbol();
    }
    ~SymbolTable() {
        while (LL) {
            symbol* tmp = LL;
            LL = LL->getSC();
            while (tmp) {
                symbol* a = tmp;
                tmp = tmp->getNext();
                delete a;
            }
            delete tmp;
        }
        delete LL;
    }
    //-------------new method-------------
    symbol* getLL() {
        return LL;
    }
    void deleteALL();
    void addLL(symbol* a);
    void addSC(symbol* a);
    bool findID(string id); 
    void clearScope();
    bool INSERT(string id, string type);
    bool findIDinScope(string id);
    bool checkstring(string id, string s);
    bool checknumber(string id, string s);
    void ASSIGN(string id, string value);
    int LOOKUP(string id);
    void PRINT();
    void RPRINT();
    //------------------------------------
    void run(string filename);
};


#endif