#include "SymbolTable.h"

bool checkID(string id) {
    return regex_match(id, regex("[a-z]\\w*"));
}
void SymbolTable::RotLeft(symbol* pR) {
    symbol* tmp1 = pR->right;
    symbol* tmp2 = pR->parent;
    symbol* tmp3 = tmp1->left;
    if (tmp2 != nullptr) {
        if (tmp2->left != nullptr && tmp2->left == pR) {
            tmp2->left = tmp1;
        }
        else {
            tmp2->right = tmp1;
        }
    }
    if (tmp3 != nullptr) tmp3->parent = pR;
    pR->right = tmp3;
    tmp1->parent = tmp2;
    tmp1->left = pR;
    pR->parent = tmp1;
}
void SymbolTable::RotRight(symbol* pR) {
    symbol* tmp1 = pR->left;
    symbol* tmp2 = pR->parent;
    symbol* tmp3 = tmp1->right;
    if (tmp2 != nullptr) {
        if (tmp2->left != nullptr && tmp2->left == pR) {
            tmp2->left = tmp1;
        }
        else {
            tmp2->right = tmp1;
        }
    }
    if (tmp3 != nullptr) tmp3->parent = pR;
    pR->left = tmp3;
    tmp1->parent = tmp2;
    tmp1->right = pR;
    pR->parent = tmp1;
}
void SymbolTable::LeftZigZag(symbol* pR) {
    symbol* tmp = pR->left;
    RotLeft(tmp);
    RotRight(pR);
}
void SymbolTable::RightZigZag(symbol* pR) {
    symbol* tmp = pR->right;
    RotRight(tmp);
    RotLeft(pR);
}
void SymbolTable::D_Right(symbol* pR) {
    symbol* tmp = pR->left;
    RotRight(pR);
    RotRight(tmp);
}
void SymbolTable::D_Left(symbol* pR) {
    symbol* tmp = pR->right;
    RotLeft(pR);
    RotLeft(tmp);
}
void SymbolTable::splay(symbol* pR) {
    if (pR->parent == nullptr) {
        pRoot = pR;
        return;
    }
    symbol* y = pR->parent;
    if (y->parent == nullptr) { 
        if (y->left != nullptr && y->left == pR) {
            RotRight(y);
        }
        else {
            RotLeft(y);
        }
    }
    else {
        symbol* z = y->parent;
        if (z->left != nullptr && z->left == y) {
            if (y->left != nullptr && y->left == pR) {
                D_Right(z);
            }
            else {
                LeftZigZag(z);
            }
        }
        else {
            if (y->right != nullptr && y->right == pR) {
                D_Left(z);
            }
            else {
                RightZigZag(z);
            }
        }
    }
    splay(pR);
    //num_splay = 1;
}
//////////////////////////////////////////////////
symbol* SymbolTable::insert(symbol* &pR, string id, string type, string state, symbol* par){
    int l = -1;
    if (state == "true") l = 0;
    else if (state == "false") l = level;
    if (pR == nullptr) {
        pR = new symbol(id, type, "", l);
        pR->parent = par;
        return pR;
    }
    else {
        if (pR->Level < l) {
            num_compare++;
            symbol* tmp = insert(pR->right, id, type, state, pR);
            return tmp;
        }
        else if (pR->Level > l) {
            num_compare++;
            symbol* tmp = insert(pR->left, id, type, state, pR);
            return tmp;
        }
        else if (pR->id.compare(id) < 0) {
            num_compare++;
            symbol* tmp = insert(pR->right, id, type, state, pR);
            return tmp;
        }
        else if (pR->id.compare(id) > 0) {
            num_compare++;
            symbol* tmp = insert(pR->left, id, type, state, pR);
            return tmp;
        }
        else {
            //clear!!
            clear(pRoot);
            throw Redeclared("INSERT " + id + " " + type + " " + state);
        }
    }
}
void SymbolTable::INSERT(symbol* &pR, string id, string type, string state) {
    symbol* tmp = insert(pR, id, type, state, nullptr);
    if (pR != tmp) {
        num_splay++;
        splay(tmp);
        pR = tmp;
    }
    cout << num_compare << " " << num_splay << endl;
}
////////////////////////////////////////////////////////////////
int checkValueVL(string s) {
    if (regex_match(s, regex("[a-z]\\w*\\((([1-9]\\d*,|0,|'[a-zA-Z0-9\\s]*',|[a-z]\\w*,)*([a-z]\\w*|[1-9]\\d*|0|'[a-zA-Z0-9\\s]*'))?\\)"))) return 1;//function call
    if (regex_match(s, regex("[a-z]\\w*"))) return 2; // identifiers
    if (regex_match(s, regex("[1-9]\\d*"))) return 3;
    if (regex_match(s, regex("'[a-zA-Z0-9\\s]*'"))) return 4;
    return 0;
}
symbol* SymbolTable::checkExistFunc(symbol*pR, string name) {
    if (pR == nullptr) {
        return nullptr;
    }
    if (pR->Level != 0) {
        num_compare++;
        return checkExistFunc(pR->left, name);
    }
    else {
        num_compare++;
        if (pR->id == name) {
            return pR;
        }
        if (pR->id.compare(name) < 0) {
            return checkExistFunc(pR->right, name);
        }
        else {
            return checkExistFunc(pR->left, name);
        }
    }
}
symbol* SymbolTable::find(symbol* pR, string id, int lev) {
    if (pR == nullptr) return nullptr;
    if (pR->Level < lev) {
        num_compare++;
        return find(pR->right, id, lev);
    }
    else if (pR->Level > lev) {
        num_compare++;
        return find(pR->left, id, lev);
    }
    else if (pR->id.compare(id) < 0) {
        num_compare++;
        return find(pR->right, id, lev);
    }
    else if (pR->id.compare(id) > 0) {
        num_compare++;
        return find(pR->left, id, lev);
    }
    num_compare++;
    return pR;
}
symbol* SymbolTable::find_no_splay(const string id) {
    int currentL = level;
    int com_tmp = num_compare;
    while (currentL >= 0) {
        num_compare = 0;
        symbol* a = find(pRoot, id, currentL);
        if (a) {
            num_compare += com_tmp;
            return a;
        }
        currentL--;
    }
    return nullptr;
}
void SymbolTable::ASSIGN(string id, string value) {
    int a = checkValueVL(value);
    if (a == 0) {
        //clear!!!
        clear(pRoot);
        throw InvalidInstruction("ASSIGN " + id + " " + value);
    }
    num_compare = 0;
    num_splay = 0;
    // number/string
    if (a == 4) { // string
        symbol* a = find_no_splay(id);
        if (a->parent != nullptr) {
            splay(a);
            num_splay++;
        }
        if (a == nullptr) {
            clear(pRoot);
            throw Undeclared("ASSIGN " + id + " " + value);
        }
        else if (a->type == "string") {
            a->value = value;
            cout << num_compare << " " << num_splay << endl;
        }
        else {
            clear(pRoot);
            throw TypeMismatch("ASSIGN " + id + " " + value);
        }
    }
    else if (a == 3) { //number
        symbol* a = find_no_splay(id);
        if (a->parent != nullptr) {
            splay(a);
            num_splay++;
        }
        if (a == nullptr) {
            clear(pRoot);
            throw Undeclared("ASSIGN " + id + " " + value);
        }
        else if (a->type == "number") {
            a->value = value;
            cout << num_compare << " " << num_splay << endl;
        }
        else {
            clear(pRoot);
            throw TypeMismatch("ASSIGN " + id + " " + value);
        }
    }
    //identifiers
    else if (a == 2) { //identifiers
        symbol* b = find_no_splay(value);
        if (b == nullptr) {
            clear(pRoot);
            throw Undeclared("ASSIGN " + id + " " + value);
        }
        if (b->parent != nullptr) {
            splay(b);
            num_splay++;
        }
        symbol* a = find_no_splay(id);
        if (a == nullptr) {
            clear(pRoot);
            throw Undeclared("ASSIGN " + id + " " + value);
        }
        if (a->parent != nullptr) {
            splay(a);
            num_splay++;
        }
        if (a->type == b->type) {
            a->value = value;
            cout << num_compare << " " << num_splay << endl;
        }
        else {
            clear(pRoot);
            throw TypeMismatch("ASSIGN " + id + " " + value);
        }
    }
    else { // function call
        string name = "";
        string par = "";
        int j = 0;
        int v = value.length();
        for (int i = j; i < v; i++, j++) {
            if (value[i] != '(') {
                name += value[i];
            }
            else break;
        }
        for (int i = j; i < v; i++, j++) {
            par += value[i];
        }
        // check exist
        symbol* nameFunc = checkExistFunc(pRoot, name);
        if (nameFunc == nullptr) {
            //clear!!
            clear(pRoot);
            throw Undeclared("ASSIGN " + id + " " + value);
        }
        else if (!regex_match(nameFunc->type, regex("\\(((number,|string,)*(number|string))?\\)->(string|number)"))) {
            //clear!!
            clear(pRoot);
            throw TypeMismatch("ASSIGN " + id + " " + value);
        }
        if (nameFunc->parent != nullptr) {
            splay(nameFunc);
            num_splay++;
        }
        // check parameter and  par of nameFunc
        int A = 1;
        int B = 1;
        bool isMatch = true;
        if (par[1] == ')' && nameFunc->type[1] == ')') {}
        else if ((par[1] == ')' && nameFunc->type[1] != ')') || (par[1] != ')' && nameFunc->type[1] == ')')) {
            isMatch = false;
        }
        else {
            int P = par.length();
            while (nameFunc->type[A] != '-' || B != P) {
                if ((nameFunc->type[A] == 'n' && !(par[B] >= '0' && par[B] <= '9')) || (nameFunc->type[A] == 's' && (int)par[B] != 39)) {
                    // check if it is variable!!!
                    string iden = "";
                    for (int i = B; i < P; i++, B++) {
                        if (par[i] == ')' || par[i] == ',') break;
                        if (par[i] != ',') {
                            iden += par[i];
                        }
                    }
                    if (regex_match(iden, regex("[a-z]\\w*"))) {
                        symbol* m = find_no_splay(iden);
                        if (m == nullptr) {
                            clear(pRoot);
                            throw Undeclared("ASSIGN " + id + " " + value);
                        }
                        else if ((nameFunc->type[A] == 'n' && m->type == "number") ||
                            (nameFunc->type[A] == 's' && m->type == "string")) {
                            if (m->parent != nullptr) {
                                splay(m);
                                num_splay++;
                            }
                            A += 7;
                            B++;
                            continue;
                        }
                    }
                    isMatch = false;
                    break;
                }
                else if (nameFunc->type[A] != 'n' && nameFunc->type[A] != 's') {
                    isMatch = false;
                    break;
                }
                A += 7;
                for (int i = B; i < P; i++, B++) {
                    if (par[i] == ',') {
                        B++;
                        break;
                    }
                }
            }
        }
        if (!isMatch) {
            clear(pRoot);
            throw TypeMismatch("ASSIGN " + id + " " + value);
        }
        //
        symbol* a = find_no_splay(id);
        if (a == nullptr) {
            clear(pRoot);
            throw Undeclared("ASSIGN " + id + " " + value);
        }
        if (a->parent != nullptr) {
            splay(a);
            num_splay++;
        }
        if (regex_match(a->type, regex("\\(((number,|string,)*(number|string))?\\)->(string|number)"))) {
            clear(pRoot);
            throw TypeMismatch("ASSIGN " + id + " " + value);
        }
        else {
            int nf = nameFunc->type.length();
            string tF = "";
            for (int i = nf - 6; i < nf; i++) {
                tF += nameFunc->type[i];
            }
            if (a->type != tF) {
                clear(pRoot);
                throw TypeMismatch("ASSIGN " + id + " " + value);
            }
            a->value = value;
            cout << num_compare << " " << num_splay << endl;
        }
    }
}
////////////////////////////////////////////////////////////////
symbol* SymbolTable::MaxLeft(symbol* pR) {
    if (pR->right) {
        return MaxLeft(pR->right);
    }
    else {
        return pR;
    }
}
symbol* SymbolTable::find4LK_RNL(symbol* pR, string id) {
    if (pR->right) {
        symbol* a = find4LK_RNL(pR->right, id);
        if (a) return a;
    }
    if (pR->id == id) {
        return pR;
    }
    if (pR->left) {
        symbol* a = find4LK_RNL(pR->left, id);
        if (a) return a;
    }

    symbol* tmp = nullptr;
    return tmp;
}
void SymbolTable::END(symbol* pR) {
    while (!scope.Top()->LoadDT().isEmpty()) {
        string tmp = scope.Top()->LoadDT().front()->loadDt();
        symbol* n = find4LK_RNL(pRoot, tmp);
        splay(n);
        if (n->left && n->right) {
            symbol* lef = n->left;
            lef->parent = nullptr;
            symbol* R = MaxLeft(lef);
            splay(R);
            R->right = n->right;
            R->right->parent = R;
        }
        else if (n->left && !n->right) {
            pRoot = n->left;
            if (pRoot) {
                pRoot->parent = nullptr;
            }
        }
        else {
            pRoot = n->right;
            if (pRoot) {
                pRoot->parent = nullptr;
            }
        }
        delete n;
        scope.Top()->LoadDT().deq();
    }
    scope.pop();
}
////////////////////////////////////////////////////////////////
void SymbolTable::LOOKUP(symbol* pR, string id) {
    if (pR) {
        symbol* a = find4LK_RNL(pR, id);
        if (a == nullptr) {
            // clear!!
            clear(pRoot);
            throw Undeclared("LOOKUP " + id);
        }
        else {
            splay(a);
            std::cout << a->Level << endl;
        }
    }
    else {
        clear(pRoot);
        throw Undeclared("LOOKUP " + id);
    }
}
void SymbolTable::PRINT(symbol* pR) {
    if (pR != nullptr) {
        std::cout << pR->id << "//" << pR->Level;
        if (pR->left) {
            std::cout << " ";
            PRINT(pR->left);
        }
        if (pR->right) {
            std::cout << " ";
            PRINT(pR->right);
        }
    }
}
////////////////////////////////////////////////////////////////
void SymbolTable::run(string filename)
{
    ifstream ifs;
    ifs.open(filename);
    string ins;
    do {
        ifs >> ins;
        if (ins == "INSERT") {
            string id, type, state;
            ifs >> id >> type >> state;
            // check id
            if (!checkID(id) || !regex_match(state, regex("(true|false)"))) {
                //clear!!
                clear(pRoot);
                throw InvalidInstruction(ins + " " + id + " " + type + " " + state);
            }
            if (regex_match(type, regex("\\(((number,|string,)*(number|string))?\\)->(string|number)"))) {
                if (level != 0 && state == "false") {
                    //clear!!
                    clear(pRoot);
                    throw InvalidDeclaration(ins + " " + id + " " + type + " " + state);
                }
            }
            else {
                if (!regex_match(type, regex("(string|number)"))) {
                    //clear!!
                    clear(pRoot);
                    throw InvalidInstruction(ins + " " + id + " " + type + " " + state);
                }
            }
            num_compare = 0;
            num_splay = 0;
            INSERT(pRoot, id, type, state);
            if (state == "true") scope.first()->LoadDT().enq(id);
            else scope.Top()->LoadDT().enq(id);
        }
        else if (ins == "ASSIGN") {
            string id;
            string value = "";
            ifs >> id;
            string tmp;
            getline(ifs, tmp);
            int T = tmp.length();
            for (int i = 1; i < T; i++) {
                value += tmp[i];
            }
            ASSIGN(id, value);
        }
        else if (ins == "BEGIN") {
            level++;
            Qu<string> a;
            scope.push(a);
        }
        else if (ins == "END") {
            if (level == 0) {
                //clear!!
                clear(pRoot);
                throw UnknownBlock();
            }
            END(pRoot);
            level--;
        }
        else if (ins == "LOOKUP") {
            string id;
            getline(ifs, id);
            string ID = "";
            int idl = id.length();
            for (int i = 1; i < idl; i++) {
                ID += id[i];
            }
            if (!checkID(ID)) {
                // clear!!
                clear(pRoot);
                throw InvalidInstruction(ins + id);
            }
            LOOKUP(pRoot, ID);
        }
        else if (ins == "PRINT") {
            PRINT(pRoot);
            if (pRoot) cout << endl;
        }
        else {
            // remember to clear!!!
            string s;
            getline(ifs, s);
            clear(pRoot);
            throw InvalidInstruction(ins + s);
        }
    } while (!ifs.eof());
    if (level) {
        // clear!!
        clear(pRoot);
        throw UnclosedBlock(level);
    }
    clear(pRoot);
    ifs.close();
}