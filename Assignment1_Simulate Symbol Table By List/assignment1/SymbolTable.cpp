#include "SymbolTable.h"


//-----------------------------function defined-----------------------------------

void SymbolTable::deleteALL() {
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
void SymbolTable::addLL(symbol* a) { // add node to current scope
    if (LL->getIsNull()) {
        LL->LoadData(a->getData());
        LL->LoadIsNull(false);
        delete a;
    }
    else {
        symbol* tmp = LL;
        while (tmp->getNext()) {
            tmp = tmp->getNext();
        }
        tmp->LoadNext(a);
        tmp = tmp->getNext()->getNext();
        delete tmp;
    }
}
void SymbolTable::addSC(symbol* a) {
    a->LoadSC(LL);
    LL = a;
}
bool SymbolTable::findID(string id) {
    if (!LL->getIsNull()) {
        symbol* tmp = LL;
        while (tmp) {
            if (tmp->getData().getID() == id) {
                while (tmp) {//--------------------------------
                    tmp = tmp->getNext();
                }
                delete tmp;
                return true;
            }
            else tmp = tmp->getNext();
        }
        delete tmp;
        return false;
    }
    else return false;
}
void SymbolTable::clearScope() {
    symbol* tmp = LL;
    LL = tmp->getSC();
    while (tmp) {
        symbol* a = tmp;
        tmp = tmp->getNext();
        delete a;
    }
}
//------------------------------------INSERT------------------------------
bool SymbolTable::INSERT(string id, string type) {
    if (this->findID(id)) {
        return false;
    }
    else {
        symbol* tmp = new symbol();
        Data a = Data(id, "", type, scope);
        tmp->LoadData(a);
        tmp->LoadIsNull(false);
        this->addLL(tmp);
        return true;
    }
}

//------------------------------------INSERT------------------------------

//----------------------------------ASSIGN--------------------------------
bool SymbolTable::findIDinScope(string id) {
    symbol* an = LL;
    while (an) {
        symbol* tmp = an;
        while (tmp) {
            if (tmp->getData().getID() == id) {
                while (tmp) {//--------------------------------
                    tmp = tmp->getNext();
                }
                delete tmp;
                while (an) {
                    an = an->getSC();
                }
                delete an;
                return true;
            }
            else tmp = tmp->getNext();
        }
        delete tmp;
        an = an->getSC();
    }
    delete an;
    return false;
}
bool SymbolTable::checkstring(string id, string s) {
    if (s == "''") return true;
    int len = s.length();
    if ((int)s[0] == 39 && (int)s[len - 1] == 39) {
        for (int i = 1; i < len - 1; i++) {
            if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9') || s[i] == ' ') continue;
            else {
                deleteALL();
                throw InvalidInstruction("ASSIGN " + id + " " + s);
            }
        }
        return true;
    }
    return false;
}
bool SymbolTable::checknumber(string id, string s) {
    if (s[0] >= '0' && s[0] <= '9') {
        int tmp = stoi(s);
        string tmp1 = to_string(tmp);
        if (tmp1 == s) return true;
        else {
            deleteALL();
            throw InvalidInstruction("ASSIGN " + id + " " + s);
        }
    }
    return false;;
}
void SymbolTable::ASSIGN(string id, string value) {
    if (findIDinScope(id)) {
        string valuetmp = value;
        string type = "";
        if (checkstring(id, value)) type = "string";
        else if (checknumber(id, value)) type = "number";
        else {
            symbol* mm = LL;
            bool isF = false;
            while (mm) {
                symbol* tmp = mm;
                while (tmp) {
                    if (tmp->getData().getID() == value) {
                        type = tmp->getData().getType();
                        value = tmp->getData().getValue();
                        isF = true;
                        break;
                    }
                    else tmp = tmp->getNext();
                }
                if (isF) break;
                mm = mm->getSC();
            }
            if (!isF) {
                deleteALL();
                throw Undeclared("ASSIGN " + id + " " + value);
            }
            mm = NULL;
            delete mm;
        }

        symbol* an = LL;
        while (an) {
            symbol* tmp = an;
            while (tmp) {
                if (tmp->getData().getID() == id) {
                    if (tmp->getData().getType() == type) {
                        tmp->getData().LoadValue(value);
                        cout << "success" << endl;
                        tmp = NULL;
                        an = NULL;
                        //delete tmp;
                        //delete an;
                        return;
                    }
                    else {
                        tmp = NULL;
                        an = NULL;
                        delete tmp;
                        delete an;
                        deleteALL();
                        throw TypeMismatch("ASSIGN " + id + " " + valuetmp);
                    }
                }
                else tmp = tmp->getNext();
            }
            //delete tmp;
            an = an->getSC();
        }
    }
    else {
        deleteALL();
        throw Undeclared("ASSIGN " + id + " " + value);
    }
}

//----------------------------------ASSIGN--------------------------------

//------------------------------------LOOKUP------------------------------
int SymbolTable::LOOKUP(string id) {

    symbol* tt = LL;
    while (tt) {
        symbol* tmp = tt;
        while (tmp) {
            if (tmp->getData().getID() == id) {
                int an = tmp->getData().getScope();
                tt = NULL;
                tmp = NULL;
                delete tt;
                delete tmp;
                return an;
            }
            else tmp = tmp->getNext();
        }
        //delete tmp;
        tt = tt->getSC();
    }
    delete tt;
    return -1;
}
//------------------------------------LOOKUP------------------------------

//----------------------------PRINT---------------------------------------
bool findVarInLL(symbol* source, string id) {
    if (source->getIsNull()) {
        return false;
    }
    else {
        symbol* tmp = source;
        while (tmp) {
            if (tmp->getData().getID() == id) {
                tmp = NULL;
                //delete tmp;
                return true;
            }
            tmp = tmp->getNext();
        }
        return false;
    }
}

symbol* getSingleLL(SymbolTable* table) {
    symbol* a1 = table->getLL();
    while (a1->getIsNull() && a1->getSC()) {
        a1 = a1->getSC();
    }
    if (a1->getSC() == NULL && a1->getIsNull()) return NULL;

    symbol* head = new symbol();
    head->LoadData(a1->getData());
    head->LoadIsNull(false);
    
    symbol* scope = a1;

    while (scope != NULL) {
        if (!scope->getIsNull()) {
            symbol* cur = head;
            symbol* cur_scope = scope;
            if (cur_scope->getData().getScope() == a1->getData().getScope()) {
                
                cur_scope = cur_scope->getNext();
                while (cur_scope != NULL) {
                    symbol* tempt = new symbol();
                    tempt->LoadData(cur_scope->getData());
                    tempt->LoadIsNull(false);
                    cur->LoadNext(tempt);
                    cur = cur->getNext();
                    cur_scope = cur_scope->getNext();
                }
            }
            else {
                symbol* t = new symbol();
                while (cur_scope != NULL) {
                    bool find = findVarInLL(cur, cur_scope->getData().getID());
                    if (!find) {
                        if (t->getIsNull()) {
                            t->LoadData(cur_scope->getData());
                            t->LoadIsNull(false);
                        }
                        else {
                            symbol* tempt = new symbol();
                            tempt->LoadData(cur_scope->getData());
                            tempt->LoadIsNull(false);
                            symbol* tt = t;
                            while (tt->getNext()) {
                                tt = tt->getNext();
                            }
                            tt->LoadNext(tempt);
                        }
                    }
                    cur_scope = cur_scope->getNext();
                }
                if (t->getIsNull()) {
                    delete t;
                }
                else {
                    symbol* tt = t;
                    while (tt->getNext()) {
                        tt = tt->getNext();
                    }
                    tt->LoadNext(head);
                    head = t;
                }
            }
        }
        scope = scope->getSC();
    }

    return head;
}

void SymbolTable::PRINT() {
    if (LL->getIsNull() && LL->getSC() == NULL) return;
    else {
        symbol* head = getSingleLL(this);
        if (head == NULL) return;
        else {
            while (head) {
                cout << head->getData().getID() << "//" << head->getData().getScope();
                if (head->getNext()) cout << " ";
                symbol* cur = head;
                head = head->getNext();
                delete cur;
            }
            cout << endl;
        }
    }
}

void display(symbol* an) {
    if (an->getNext() == NULL) { cout << an->getData().getID() << "//" << an->getData().getScope(); }
    else {
        display(an->getNext());
        cout << " " << an->getData().getID() << "//" << an->getData().getScope();
    }
    delete an;
}

void SymbolTable::RPRINT() {
    if (LL->getIsNull() && LL->getSC() == NULL) return;
    else {
        symbol* head = getSingleLL(this);
        if (head == NULL) return;
        else {
            display(head);
            cout << endl;
        }
    }
}
//----------------------------PRINT---------------------------------------

//------------------------------------------------------------------------

void SymbolTable::run(string filename)
{
    ifstream ifs;
    ifs.open(filename);
    string instruction;
    do {
        ifs >> instruction;
        if (instruction == "INSERT") {
            string id, type;
            ifs >> id >> type;
            if (INSERT(id, type)) {
                cout << "success" << endl;
            }
            else {
                deleteALL();
                throw Redeclared("INSERT " + id + " " + type);
            }
        }
        else if (instruction == "ASSIGN") { 
            string idAssign;
            string value = "";
            ifs >> idAssign;
            string tmp;
            getline(ifs, tmp);
            for (int i = 1; i < (int)tmp.length(); i++) {
                value += tmp[i];
            }
            ASSIGN(idAssign, value);
        }
        else if (instruction == "BEGIN") {
            scope++;
            symbol* a = new symbol();
            a->getData().LoadScope(scope);
            addSC(a);
        }
        else if (instruction == "END") {
            if (scope == 0) {
                deleteALL();
                throw UnknownBlock();
            }
            else {
                clearScope();
                scope--;
            }
        }
        else if (instruction == "LOOKUP") {
            string id;
            ifs >> id;
            int sc = LOOKUP(id);
            if (sc < 0) {
                deleteALL();
                Undeclared a("LOOKUP " + id);
                throw a;
            }
            else cout << sc << endl;
        }
        else if (instruction == "PRINT") {
            PRINT();
        }
        else if (instruction == "RPRINT") {
            RPRINT();
        }
        else {
            deleteALL();
            string s;
            getline(ifs, s);
            throw InvalidInstruction(instruction + s);

        }
    } while (!ifs.eof());
    if (scope) {
        deleteALL();
        throw UnclosedBlock(scope);
    }
    ifs.close();
}