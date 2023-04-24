#include "SymbolTable.h"

string SymbolTable::key(string name, int level) {
    string key = "";
    if (level != 0) {
        key += to_string(level);
    }
    int n = name.length();
    for (int i = 0; i < n; i++) {
        key += to_string((int)name[i] - 48);
    }
    return key;
}

bool checkASSIGN_ins(string a) {
    return regex_match(a, regex("ASSIGN\\s[a-z]\\w*\\s([1-9]\\d*|0|'([a-zA-Z0-9\\s]*)'|[a-z]\\w*|[a-z]\\w*\\((([a-z]\\w*,|[1-9]\\d*,|0,|'([a-zA-Z0-9\\s]*)',)*([a-z]\\w*|[1-9]\\d*|0|'([a-zA-Z0-9\\s]*)'))?\\))"));
}

bool checkCALL_ins(string ins) {
    return regex_match(ins, regex("CALL\\s[a-z]\\w*\\((([a-z]\\w*,|[1-9]\\d*,|'[a-zA-Z0-9\\s]*',|0,)*([a-z]\\w*|[1-9]\\d*|0|'[a-zA-Z0-9\\s]*'))?\\)"));
}

int checkCaseAss(string ins) {
    if (regex_match(ins, regex("ASSIGN\\s[a-z]\\w*\\s([1-9]\\d*|0)"))) return 1; // number
    if (regex_match(ins, regex("ASSIGN\\s[a-z]\\w*\\s('[a-zA-Z0-9\\s]*')"))) return 2; // string
    if (regex_match(ins, regex("ASSIGN\\s[a-z]\\w*\\s[a-z]\\w*"))) return 3; // identifier
    //if (regex_match(ins, regex("ASSIGN\\s[a-z]\\w*\\s[a-z]\\w*\\((([a-z]\\w*,|[1-9]\\d*,|0,|'([a-zA-Z0-9\\s]*)',)*([a-z]\\w*|[1-9]\\d*|0|'([a-zA-Z0-9\\s]*)'))?\\)"))) return 4; // function
    return 4;   
}

SymbolTable::symbol* SymbolTable::findID(string name, const int scope, int& num) {
    string ky = key(name, scope);
    int numtmp = 0;
    for (int i = 0; i < size; i++) {
        symbol* tem = tab->at(hp(ky, i));
        if (tem->lvl == scope) {
            if (tem->identifiers == name) {
                num += numtmp;
                return tem;
            }
            numtmp++;
        }
    }
    return nullptr;
}


void SymbolTable::INSERT(symbol a, string ins) {
    int i = 0;
    int h = -1;
    do {
        if (i >= size) break;
        h = hp(a.k, i);
        if (tab->getLvl(h) == -1) {
            add(a, h);
            nE++;
            cout << i << endl;
            return;
        }
        i++;
    } while (h < size && h >= 0);
    if (i >= size) {
        clear(tab);
        throw Overflow(ins);
    }
}

void SymbolTable::ASSIGN(string ins) {
    int Case = checkCaseAss(ins);
    int n = ins.length();
    int khe = 0;
    if (Case == 1) {
        string a[2] = { "","" };
        int m = 0;
        for (int i = 7; i < n; i++) {
            if (ins[i] == ' ') m++;
            else {
                a[m] += ins[i];
            }
        }
        string id = a[0];
        // find id
        symbol* ide = nullptr;
        for (int i = scope; i >= 0; i--) {
            ide = findID(id, i, khe);
            if (ide) break;
        }
        if (ide == nullptr) {
            clear(tab);
            throw Undeclared(id);
        }
        if (ide->numP >= 0 || (ide->type != "number" && ide->type != "")) {
            clear(tab);
            throw TypeMismatch(ins);
        }
        if (ide->type == "") {
            ide->type = "number";
        }
        ide->value = a[1];
        cout << khe << endl;
    }
    else if (Case == 2) {
        string a[2] = { "","" };
        int m = 0;
        for (int i = 7; i < n; i++) {
            if (ins[i] == ' ' && m == 0) m++;
            else {
                a[m] += ins[i];
            }
        }
        string id = a[0];
        // find id
        symbol* ide = nullptr;
        for (int i = scope; i >= 0; i--) {
            ide = findID(id, i, khe);
            if (ide) break;
        }
        if (ide == nullptr) {
            clear(tab);
            throw Undeclared(id);
        }
        if (ide->numP >= 0 || (ide->type != "string" && ide->type != "")) {
            clear(tab);
            throw TypeMismatch(ins);
        }
        if (ide->type == "") {
            ide->type = "string";
        }
        ide->value = a[1];
        cout << khe << endl;
    }
    else if (Case == 3) {
        string a[2] = { "","" };
        int m = 0;
        for (int i = 7; i < n; i++) {
            if (ins[i] == ' ') m++;
            else {
                a[m] += ins[i];
            }
        }
        // find id a[1] before a[0]
        string id1 = a[0];
        string id2 = a[1];
        symbol* ide1 = nullptr;
        symbol* ide2 = nullptr;
        for (int i = scope; i >= 0; i--) {
            ide2 = findID(id2, i, khe);
            if (ide2) break;
        }
        if (ide2 == nullptr) {
            clear(tab);
            throw Undeclared(id2);
        }
        if (ide2->numP >= 0) {
            clear(tab);
            throw InvalidInstruction(ins);
        }

        for (int i = scope; i >= 0; i--) {
            ide1 = findID(id1, i, khe);
            if (ide1) break;
        }
        if (ide1 == nullptr) {
            clear(tab);
            throw Undeclared(id1);
        }
        if (ide1->numP >= 0) {
            clear(tab);
            throw TypeMismatch(ins);
        }

        if (ide1->type == "" && ide2->type != "") {
            ide1->type = ide2->type;
        }
        else if (ide2->type == "" && ide1->type != "") {
            ide2->type = ide1->type;
        }
        else if (ide1->type != "" && ide2->type != "" && ide1->type != ide2->type) {
            clear(tab);
            throw TypeMismatch(ins);
        }
        else if (ide1->type == ide2->type && ide1->type == "") {
            clear(tab);
            throw TypeCannotBeInferred(ins);
        }
        ide1->value = id2;
        cout << khe << endl;
    }
    else {
        string a[2] = { "","" };
        int m = 0;
        for (int i = 7; i < n; i++) {
            if (ins[i] == ' ' && m == 0) m++;
            else {
                a[m] += ins[i];
            }
        }
        string id1 = a[0];
        int b = 0;
        int n2 = a[1].length();
        string id2 = "";
        for (int i = b; i < n2; i++, b++) {
            if (a[1][i] == '(') {
                b++;
                break;
            }
            else {
                id2 += a[1][i];
            }
        }
        symbol* id_func = nullptr;
        for (int i = scope; i >= 0; i--) {
            id_func = findID(id2, i, khe);
            if (id_func) break;
        }
        if (id_func == nullptr) {
            clear(tab);
            throw Undeclared(id2);
        }
        if (id_func->numP < 0) {
            clear(tab);
            throw TypeMismatch(ins);
        }
        int num_par = -1;
        if (a[1][b] == ')') num_par = 0;
        else {
            int num_com = 0;
            for (int i = b; i < n2; i++) {
                if (a[1][i] == ',') {
                    num_com++;
                }
            }
            num_par = num_com + 1;
        }
        if (id_func->numP != num_par) {
            clear(tab);
            throw TypeMismatch(ins);
        }
        // check inside the function call
        if (id_func->type == "") {
            if (a[1][b] == ')') {
                id_func->type += "()";
            }
            else {
                id_func->type += "(";
                string pa = "";
                for (int i = b; i < n2; i++) {
                    if (a[1][i] == ',') {
                        if (regex_match(pa, regex("([1-9]\\d*|0)"))) {
                            id_func->type += "number,";
                        }
                        else if (regex_match(pa, regex("'[a-zA-Z0-9\\s]*'"))) {
                            id_func->type += "string,";
                        }
                        else {
                            symbol* tmp = nullptr;
                            for (int i = scope; i >= 0; i--) {
                                tmp = findID(pa, i, khe);
                                if (tmp) break;
                            }
                            if (tmp == nullptr) {
                                clear(tab);
                                throw Undeclared(pa);
                            }
                            if (tmp->numP >= 0) {
                                clear(tab);
                                throw TypeMismatch(ins);
                            }
                            if (tmp->type == "") {
                                clear(tab);
                                throw TypeCannotBeInferred(ins);
                            }
                            id_func->type += tmp->type + ',';
                        }
                        pa = "";
                    }
                    else if (a[1][i] == ')') {
                        if (regex_match(pa, regex("([1-9]\\d*|0)"))) {
                            id_func->type += "number)";
                        }
                        else if (regex_match(pa, regex("'[a-zA-Z0-9\\s]*'"))) {
                            id_func->type += "string)";
                        }
                        else {
                            symbol* tmp = nullptr;
                            for (int i = scope; i >= 0; i--) {
                                tmp = findID(pa, i, khe);
                                if (tmp) break;
                            }
                            if (tmp == nullptr) {
                                clear(tab);
                                throw Undeclared(pa);
                            }
                            if (tmp->numP >= 0) {
                                clear(tab);
                                throw TypeMismatch(ins);
                            }
                            if (tmp->type == "") {
                                clear(tab);
                                throw TypeCannotBeInferred(ins);
                            }
                            id_func->type += tmp->type + ')';
                        }
                    }
                    else {
                        pa += a[1][i];
                    }
                }
            }
        }
        else if (id_func->type == "void") {
            if (a[1][b] == ')') {
                id_func->type = "()->" + id_func->type;
            }
            else {
                string ty = "(";
                string pa = "";
                for (int i = b; i < n2; i++) {
                    if (a[1][i] == ',') {
                        if (regex_match(pa, regex("([1-9]\\d*|0)"))) {
                            ty += "number,";
                        }
                        else if (regex_match(pa, regex("'[a-zA-Z0-9\\s]*'"))) {
                            ty += "string,";
                        }
                        else {
                            symbol* tmp = nullptr;
                            for (int i = scope; i >= 0; i--) {
                                tmp = findID(pa, i, khe);
                                if (tmp) break;
                            }
                            if (tmp == nullptr) {
                                clear(tab);
                                throw Undeclared(pa);
                            }
                            if (tmp->numP >= 0) {
                                clear(tab);
                                throw TypeMismatch(ins);
                            }
                            if (tmp->type == "") {
                                clear(tab);
                                throw TypeCannotBeInferred(ins);
                            }
                            ty += tmp->type + ',';
                        }
                        pa = "";
                    }
                    else if (a[1][i] == ')') {
                        if (regex_match(pa, regex("([1-9]\\d*|0)"))) {
                            ty += "number)";
                        }
                        else if (regex_match(pa, regex("'[a-zA-Z0-9\\s]*'"))) {
                            ty += "string)";
                        }
                        else {
                            symbol* tmp = nullptr;
                            for (int i = scope; i >= 0; i--) {
                                tmp = findID(pa, i, khe);
                                if (tmp) break;
                            }
                            if (tmp == nullptr) {
                                clear(tab);
                                throw Undeclared(pa);
                            }
                            if (tmp->numP >= 0) {
                                clear(tab);
                                throw TypeMismatch(ins);
                            }
                            if (tmp->type == "") {
                                clear(tab);
                                throw TypeCannotBeInferred(ins);
                            }
                            ty += ty + ')';
                        }
                    }
                    else {
                        pa += a[1][i];
                    }
                }
                id_func->type = ty + "->" + id_func->type;
            }

        }
        else {
            if (a[1][b] != ')') {
                string pa = "";
                int nn = 1;
                for (int i = b; i < n2; i++) {
                    if (a[1][i] == ',' || a[1][i] == ')') {
                        if ((regex_match(pa, regex("([1-9]\\d*|0)")) && id_func->type[nn] != 'n') ||
                            (regex_match(pa, regex("'[a-zA-Z0-9\\s]*'")) && id_func->type[nn] != 's')) {
                            clear(tab);
                            throw TypeMismatch(ins);
                        }
                        else if (regex_match(pa, regex("[a-z]\\w*"))) {
                            symbol* tmp = nullptr;
                            for (int i = scope; i >= 0; i--) {
                                tmp = findID(pa, i, khe);
                                if (tmp) break;
                            }
                            if (tmp == nullptr) {
                                clear(tab);
                                throw Undeclared(pa);
                            }
                            if (tmp->numP >= 0) {
                                clear(tab);
                                throw TypeMismatch(ins);
                            }
                            if (tmp->type == "") {
                                if (id_func->type[nn] == 'n') {
                                    tmp->type = "number";
                                }
                                else {
                                    tmp->type = "string";
                                }
                            }
                            else if ((tmp->type == "number" && id_func->type[nn] != 'n') ||
                                (tmp->type == "string" && id_func->type[nn] != 's')) {
                                clear(tab);
                                throw TypeMismatch(ins);
                            }
                        }
                        pa = "";
                        nn += 7;
                    }
                    else {
                        pa += a[1][i];
                    }
                }
            }
        }

        if (regex_match(id_func->type, regex("\\(((number,|string,)*(number|string))?\\)->void"))) {
            clear(tab);
            throw TypeMismatch(ins);
        }

        //check id1;
        symbol* ID1 = nullptr;
        for (int i = scope; i >= 0; i--) {
            ID1 = findID(id1, i, khe);
            if (ID1) break;
        }
        if (ID1 == nullptr) {
            clear(tab);
            throw Undeclared(id1);
        }
        if (ID1->numP >= 0) {
            clear(tab);
            throw TypeMismatch(ins);
        }
        if (ID1->type == "") {
            if (regex_match(id_func->type, regex("\\(((number,|string,)*(number|string))?\\)->number"))) {
                ID1->type = "number";
            }
            else if (regex_match(id_func->type, regex("\\(((number,|string,)*(number|string))?\\)->string"))) {
                ID1->type = "string";
            }
            else {
                clear(tab);
                throw TypeCannotBeInferred(ins);
            }
        }
        else {
            if ((regex_match(id_func->type, regex("\\(((number,|string,)*(number|string))?\\)->number")) && ID1->type != "number") ||
                (regex_match(id_func->type, regex("\\(((number,|string,)*(number|string))?\\)->string")) && ID1->type != "string")) {
                clear(tab);
                throw TypeMismatch(ins);
            }
            else if (regex_match(id_func->type, regex("\\(((number,|string,)*(number|string))?\\)"))) {
                id_func->type += "->" + ID1->type;
            }
        }

        ID1->value = a[1];
        cout << khe << endl;
    }
}

void SymbolTable::CALL(string ins) {
    int khe = 0;
    int n1 = ins.length();
    string fun = "";
    for (int i = 5; i < n1; i++) {
        fun += ins[i];
    }
    int b = 0;
    int n2 = fun.length();
    string id = "";
    for (int i = b; i < n2; i++, b++) {
        if (fun[i] == '(') {
            b++;
            break;
        }
        else {
            id += fun[i];
        }
    }
    symbol* id_func = nullptr;
    for (int i = scope; i >= 0; i--) {
        id_func = findID(id, i, khe);
        if (id_func) break;
    }
    if (id_func == nullptr) {
        clear(tab);
        throw Undeclared(id);
    }
    if (id_func->numP < 0) {
        clear(tab);
        throw TypeMismatch(ins);
    }
    int num_par = -1;
    if (fun[b] == ')') num_par = 0;
    else {
        int num_com = 0;
        for (int i = b; i < n2; i++) {
            if (fun[i] == ',') {
                num_com++;
            }
        }
        num_par = num_com + 1;
    }
    if (id_func->numP != num_par) {
        clear(tab);
        throw TypeMismatch(ins);
    }
    // check inside the function call
    if (id_func->type == "") {
        if (fun[b] == ')') {
            id_func->type += "()->void";
        }
        else {
            id_func->type += "(";
            string pa = "";
            for (int i = b; i < n2; i++) {
                if (fun[i] == ',') {
                    if (regex_match(pa, regex("([1-9]\\d*|0)"))) {
                        id_func->type += "number,";
                    }
                    else if (regex_match(pa, regex("'[a-zA-Z0-9\\s]*'"))) {
                        id_func->type += "string,";
                    }
                    else {
                        symbol* tmp = nullptr;
                        for (int i = scope; i >= 0; i--) {
                            tmp = findID(pa, i, khe);
                            if (tmp) break;
                        }
                        if (tmp == nullptr) {
                            clear(tab);
                            throw Undeclared(pa);
                        }
                        if (tmp->numP >= 0) {
                            clear(tab);
                            throw TypeMismatch(ins);
                        }
                        if (tmp->type == "") {
                            clear(tab);
                            throw TypeCannotBeInferred(ins);
                        }
                        id_func->type += tmp->type + ',';
                    }
                    pa = "";
                }
                else if (fun[i] == ')') {
                    if (regex_match(pa, regex("([1-9]\\d*|0)"))) {
                        id_func->type += "number)";
                    }
                    else if (regex_match(pa, regex("'[a-zA-Z0-9\\s]*'"))) {
                        id_func->type += "string)";
                    }
                    else {
                        symbol* tmp = nullptr;
                        for (int i = scope; i >= 0; i--) {
                            tmp = findID(pa, i, khe);
                            if (tmp) break;
                        }
                        if (tmp == nullptr) {
                            clear(tab);
                            throw Undeclared(pa);
                        }
                        if (tmp->numP >= 0) {
                            clear(tab);
                            throw TypeMismatch(ins);
                        }
                        if (tmp->type == "") {
                            clear(tab);
                            throw TypeCannotBeInferred(ins);
                        }
                        id_func->type += tmp->type + ')';
                    }
                }
                else {
                    pa += fun[i];
                }
            }
            id_func->type += "->void";
        }
    }
    else if (id_func->type == "void") {
        if (fun[b] == ')') {
            id_func->type = "()->" + id_func->type;
        }
        else {
            string ty = "(";
            string pa = "";
            for (int i = b; i < n2; i++) {
                if (fun[i] == ',') {
                    if (regex_match(pa, regex("([1-9]\\d*|0)"))) {
                        ty += "number,";
                    }
                    else if (regex_match(pa, regex("'[a-zA-Z0-9\\s]*'"))) {
                        ty += "string,";
                    }
                    else {
                        symbol* tmp = nullptr;
                        for (int i = scope; i >= 0; i--) {
                            tmp = findID(pa, i, khe);
                            if (tmp) break;
                        }
                        if (tmp == nullptr) {
                            clear(tab);
                            throw Undeclared(pa);
                        }
                        if (tmp->numP >= 0) {
                            clear(tab);
                            throw TypeMismatch(ins);
                        }
                        if (tmp->type == "") {
                            clear(tab);
                            throw TypeCannotBeInferred(ins);
                        }
                        ty += tmp->type + ',';
                    }
                    pa = "";
                }
                else if (fun[i] == ')') {
                    if (regex_match(pa, regex("([1-9]\\d*|0)"))) {
                        ty += "number)";
                    }
                    else if (regex_match(pa, regex("'[a-zA-Z0-9\\s]*'"))) {
                        ty += "string)";
                    }
                    else {
                        symbol* tmp = nullptr;
                        for (int i = scope; i >= 0; i--) {
                            tmp = findID(pa, i, khe);
                            if (tmp) break;
                        }
                        if (tmp == nullptr) {
                            clear(tab);
                            throw Undeclared(pa);
                        }
                        if (tmp->numP >= 0) {
                            clear(tab);
                            throw TypeMismatch(ins);
                        }
                        if (tmp->type == "") {
                            clear(tab);
                            throw TypeCannotBeInferred(ins);
                        }
                        ty += ty + ')';
                    }
                }
                else {
                    pa += fun[i];
                }
            }
            id_func->type = ty + "->" + id_func->type;
        }
    }
    else {
        if (fun[b] == ')') {
            if (regex_match(id_func->type, regex("\\(\\)->(number|string)"))) {
                clear(tab);
                throw TypeMismatch(ins);
            }
            else if (regex_match(id_func->type, regex("\\(\\)"))) {
                id_func->type += "->void";
            }
        }
        else {
            string pa = "";
            int n = 1;
            for (int i = b; i < n2; i++) {
                if (fun[i] == ',' || fun[i] == ')') {
                    if ((regex_match(pa, regex("([1-9]\\d*|0)")) && id_func->type[n] != 'n') || (regex_match(pa, regex("'[a-zA-Z0-9\\s]*'")) && id_func->type[n] != 's')) {
                        clear(tab);
                        throw TypeMismatch(ins);
                    }
                    else if (regex_match(pa, regex("[a-z]\\w*"))) {
                        symbol* tmp = nullptr;
                        for (int i = scope; i >= 0; i--) {
                            tmp = findID(pa, i, khe);
                            if (tmp) break;
                        }
                        if (tmp == nullptr) {
                            clear(tab);
                            throw Undeclared(pa);
                        }
                        if (tmp->numP >= 0) {
                            clear(tab);
                            throw TypeMismatch(ins);
                        }
                        if (tmp->type == "") {
                            if (id_func->type[n] == 'n') {
                                tmp->type = "number";
                            }
                            else {
                                tmp->type = "string";
                            }
                        }
                        else if ((tmp->type == "number" && id_func->type[n] != 'n') ||
                            (tmp->type == "string" && id_func->type[n] != 's')) {
                            clear(tab);
                            throw TypeMismatch(ins);
                        }
                    }
                    pa = "";
                    n += 7;
                }
                else {
                    pa += fun[i];
                }
            }
            if (regex_match(id_func->type, regex("\\((number,|string,)*(number|string)\\)"))) {
                id_func->type += "->void";
            }
            else if (regex_match(id_func->type, regex("\\((number,|string,)*(number|string)\\)->(number|string)"))) {
                clear(tab);
                throw TypeMismatch(ins);
            }
        }
    }
    cout << khe << endl;
}

void SymbolTable::LOOKUP(string name) {
    int ind = tab->lookup(name, scope);
    if (ind == -1) {
        clear(tab);
        throw Undeclared(name);
    }
    cout << ind << endl;
}

void SymbolTable::run(string filename)
{
    ifstream ifs;
    ifs.open(filename);
    string e = "";
    getline(ifs, e);
    int lenE = e.length();
    if (regex_match(e, regex("LINEAR\\s[1-9]\\d*\\s[1-9]\\d*"))) {
        prob = "LINEAR";
        string int1[2];
        int a = 0;
        for (int i = 7; i < lenE; i++) {
            if (e[i] == ' ') {
                a++;
            }
            else {
                int1[a] += e[i];
            }
        }
        lodSize(stoi(int1[0]));
        c1 = stoi(int1[1]);
    }
    else if (regex_match(e, regex("QUADRATIC\\s[1-9]\\d*\\s[1-9]\\d*\\s[1-9]\\d*"))) {
        prob = "QUADRATIC";
        string int1[3];
        int a = 0;
        for (int i = 10; i < lenE; i++) {
            if (e[i] == ' ') {
                a++;
            }
            else {
                int1[a] += e[i];
            }
        }
        lodSize(stoi(int1[0]));
        c1 = stoi(int1[1]);
        c2 = stoi(int1[2]);
    }
    else if (regex_match(e, regex("DOUBLE\\s[1-9]\\d*\\s[1-9]\\d*"))) {
        prob = "DOUBLE";
        string int1[2];
        int a = 0;
        for (int i = 7; i < lenE; i++) {
            if (e[i] == ' ') {
                a++;
            }
            else {
                int1[a] += e[i];
            }
        }
        lodSize(stoi(int1[0]));
        c1 = stoi(int1[1]);
    }
    else {
        clear(tab);
        throw InvalidInstruction(e);
    }

    string ins;
    do {
        getline(ifs, ins);
        if (regex_match(ins, regex("INSERT\\s[a-z]\\w*(\\s(0|[1-9]\\d*))?"))) {
            string a[2] = { "", "" };
            int n = ins.length();
            int m = 0;
            for (int i = 7; i < n; i++) {
                if (ins[i] == ' ' && m == 0) {
                    m++;
                }
                else {
                    a[m] += ins[i];
                }
            }
            if (m != 0 && scope != 0) { // check Invalid Dec
                throw InvalidDeclaration(a[0]);
            }
            if (m != 0 && scope == 0) {
                m = stoi(a[m]);
            }
            else m = -1;
            string k = key(a[0], scope);
            symbol tmp(a[0], scope, "","", k, m);
            // checck Redeclared
            if (!checkDec(tmp)) {
                throw Redeclared(a[0]);
            }
            //check Overflow
            if (checkOF()) {
                throw Overflow(ins);
            }
            //insert
            INSERT(tmp, ins);
        }
        else if (checkASSIGN_ins(ins)) {
            ASSIGN(ins);
        }
        else if (checkCALL_ins(ins)) {
            CALL(ins);
        }
        else if (ins == "BEGIN") {
            scope++;
        }
        else if (ins == "END") {
            if (scope == 0) {
                clear(tab);
                throw UnknownBlock();
            }
            Dltscope();
        }
        else if (regex_match(ins, regex("LOOKUP\\s[a-z]\\w*"))) {
            string name = "";
            int n = ins.length();
            for (int i = 7; i < n; i++) {
                name += ins[i];
            }
            LOOKUP(name);
        }
        else if (ins == "PRINT") {
            PRINT();
        }
        else {
            clear(tab);
            throw InvalidInstruction(ins);
        }
    } while (!ifs.eof());
    
    if (scope != 0) {
        clear(tab);
        throw UnclosedBlock(scope);
    }

    clear(tab);
    ifs.close();
}