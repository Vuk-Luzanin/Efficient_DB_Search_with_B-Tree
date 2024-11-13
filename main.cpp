#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;


struct CustomerAccount{
    long int CA_ID;
    int CA_TAX_ST;
    double CA_BAL;
    CustomerAccount* next = nullptr;

    friend ostream& operator<<(ostream& os, CustomerAccount acc);
};
struct Node{
    vector<long int> C_ID;
    vector<CustomerAccount*> firsts;
    vector<CustomerAccount*> lasts;
    vector<Node*> children;
    Node* parent = nullptr;

    void addAccountToCustomer(CustomerAccount* account, int i);

    friend ostream& operator<<(ostream& os, Node node);

    Node(int m) {
        vector<long int> arr0(m);
        this->C_ID = arr0;
        vector<CustomerAccount*> arr(m);
        this->firsts = arr;
        vector<CustomerAccount*> arr1(m);
        this->lasts = arr1;
        vector<Node*> arr3(m + 1);
        this->children = arr3;
    }
};

class Btree{
public:
    int getM() const { return m; }
    Node* getRoot() {return root;}
    void printBTree();
    explicit Btree(int m);
    void addCustomerToBTree(long int key, Node* cur, CustomerAccount* account);
    void addAccountToBTree(long int key, CustomerAccount* account);

private:
    Node* root = nullptr;
    int m;
};


ostream& operator<<(ostream& os, CustomerAccount acc) {
    os << "CA_ID: " << acc.CA_ID << ", " << "CA_TAX_ST: " << acc.CA_TAX_ST << ", " << "CA_BAL: " << acc.CA_BAL << ", ";
    return os;
}

ostream& operator<<(ostream& os, Node node) {
    for (int j = 0; j < node.C_ID.size(); j++) {
        os << endl << "C_ID: " << node.C_ID[j] << endl;
        CustomerAccount *cur = node.firsts[j];
        while (cur != nullptr) {
            os << *cur << endl;
            cur = cur->next;
        }
    }
    os << endl;
    return os;
}

int validElements(vector<long int>& arr) {
    int valid = 0;
    for (long i : arr) {
        if (i != 0) {
            valid++;
        }
    }
    return valid;
}

int validElementsChildren(vector<Node*>& arr) {
    int valid = 0;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] != nullptr) {
            valid++;
        }
    }
    return valid;
}

int binarySearchForLongInt(const vector<long int>& vec, long int key) {
    int i;
    for (i = 0; i < vec.size(); i++) {
        if (vec[i] == 0) {
            i--;
            break;
        }
    }
    int low = 0;
    int high = i;    //vraca indeks posldnjeg elementa != 0
    int mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (key == vec[mid]) {
            return mid;
        } else {
            if (key < vec[mid]) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
    }
    return mid;
}   //binarnom pretragom vraca poziciju na koju treba umetnuti C_ID

int findIndexForLongInt(const vector<long int>& vec, long int key) {
    int i;
    for (i = 0; i < vec.size(); i++) {
        if (vec[i] == 0 || key < vec[i]) {
            break;
        }
    }
    return i;
}

long int parseStringLineIntoAcc(const string& line, CustomerAccount* account) {
    long int key = 0;
    istringstream iss(line);
    //cout << line;
    string ex;
    getline(iss, ex, '|');
    account->CA_ID = std::stol(ex);

    getline(iss, ex, '|');          //Preskace citanje CA_B_ID

    getline(iss, ex, '|');
    key = std::stol(ex);

    getline(iss, ex, '|');          //Preskace citanje imena racuna

    getline(iss, ex, '|');
    account->CA_TAX_ST = std::stoi(ex);

    getline(iss, ex, '|');
    account->CA_BAL = std::stod(ex);
    return key;
}

void Node::addAccountToCustomer(CustomerAccount* account, int i) {
    if (firsts[i] == nullptr) {
        firsts[i] = account;
        lasts[i] = account;
        return;
    }
    CustomerAccount* cur = firsts[i];
    while (cur) {
        if (cur->CA_ID == account->CA_ID) {
            cout << "Primarni ključ (Account): " << account->CA_ID << ", već postoji za izabranog Customer-a" << endl;
            delete account;
            return;
        }
        cur = cur->next;
    }
    if (firsts[i] == nullptr || firsts[i]->CA_ID >= account->CA_ID) {
        account->next = firsts[i];
        firsts[i] = account;
        lasts[i] = account;
    } else {
        CustomerAccount* prev = nullptr;
        cur = firsts[i];
        while (cur && cur->CA_ID < account->CA_ID) {
            prev = cur;
            cur = cur->next;
        }
        if (prev) {
            prev->next = account;
        }
        account->next = cur;
        if (cur == nullptr) {
            lasts[i] = account;
        }
    }
}          //ulancava u listu accounte, poziv - kada se nadje C_ID

void Btree::addCustomerToBTree(long int key, Node* cur, CustomerAccount* account) {
    int index;
    if (cur == root && validElements(root->C_ID) < m-1) {          //u korenu ima mesta
        index = findIndexForLongInt(root->C_ID, key);
        for (int i = root->C_ID.size()-1; i > index; i--) {
            root->C_ID[i] = root->C_ID[i - 1];
        }
        root->C_ID[index] = key;
        for (int i = root->firsts.size()-1; i > index; i--) {
            root->firsts[i] = root->firsts[i - 1];
            root->lasts[i] = root->lasts[i - 1];
        }
        root->firsts[index] = nullptr;
        root->lasts[index] = nullptr;
        root->addAccountToCustomer(account, index);

    } else if (validElements(cur->C_ID) < m-1) {                    //u listu ima mesta
        index = findIndexForLongInt(cur->C_ID, key);
        for (int i = cur->C_ID.size()-1; i > index; i--) {
            cur->C_ID[i] = cur->C_ID[i - 1];
        }
        cur->C_ID[index] = key;
        int i;
        for (i = cur->firsts.size()-1; i > index; i--) {
            cur->firsts[i] = cur->firsts[i - 1];
            cur->lasts[i] = cur->lasts[i - 1];
        }
        cur->firsts[index] = nullptr;
        cur->lasts[index] = nullptr;
        cur->addAccountToCustomer(account, i);
    } else if (cur == root && validElements(root->C_ID) == m-1) {         //u korenu nema mesta - cepanje -> novi koren se pravi + desni brat
        Node* father = new Node(m);
        Node* rightBrother = new Node(m);
        vector<long int> pom(m);
        vector<CustomerAccount*> firstsPom(m);
        vector<CustomerAccount*> lastsPom(m);

        for (int i = 0; i < m-1; i++) {
            pom[i] = cur->C_ID[i];
            firstsPom[i] = cur->firsts[i];
            lastsPom[i] = cur->lasts[i];
        }
        index = findIndexForLongInt(pom, key);
        for (int i = pom.size()-1; i > index; i--) {
            pom[i] = pom[i - 1];
            firstsPom[i] = firstsPom[i - 1];
            lastsPom[i] = lastsPom[i - 1];
        }
        pom[index] = key;
        firstsPom[index] = account;
        lastsPom[index] = account;

        index = (m % 2) ? m/2 : m/2-1;
        father->C_ID[0] = pom[index];
        father->firsts[0] = firstsPom[index];
        father->lasts[0] = lastsPom[index];
        father->children[0] = root;
        father->children[1] = rightBrother;
        int j = 0;
        for (int i = index+1; i < m; i++) {
            rightBrother->C_ID[j] = pom[i];
            rightBrother->firsts[j] = firstsPom[i];
            rightBrother->lasts[j] = lastsPom[i];
            j++;
        }
        for (int i = index; i < m-1; i++) {
            cur->C_ID[i] = 0;
            cur->firsts[i] = nullptr;
            cur->lasts[i] = nullptr;
        }
        cur -> parent = father;
        rightBrother -> parent = father;
        root = father;


    } else if (cur != root && validElements(cur->C_ID) == m-1) {                //srediti children -- pun list koji nije koren
        Node* prev = nullptr;
        index = findIndexForLongInt(cur->C_ID, key);
        for (int j = cur->C_ID.size() - 1; j > index; j--) {
            cur->C_ID[j] = cur->C_ID[j - 1];
            cur->firsts[j] = cur->firsts[j - 1];
            cur->lasts[j] = cur->lasts[j - 1];
        }
        cur->C_ID[index] = key;
        cur->firsts[index] = account;
        cur->lasts[index] = account;
        for (int i = m; i>index; i--) {
            cur->children[i] = cur->children[i - 1];
        }
        cur->children[index] = nullptr;
        cur->children[index + 1] = nullptr;                                         //LIST JE PUN

        while (cur != nullptr) {
            if (cur == root && validElements(root->C_ID) == m) {                //CEPA KOREN KOJI SE NAPUNIO POPAGACIJOM

                Node* father = new Node(m);
                Node* rightBrother = new Node(m);


                index = (m % 2) ? m/2 : m/2-1;
                father->C_ID[0] = cur->C_ID[index];
                father->firsts[0] = cur->firsts[index];
                father->lasts[0] = cur->lasts[index];
                father->children[0] = cur;
                father->children[1] = rightBrother;
                int j = 0;
                int i;
                for (i = index+1; i < m; i++) {
                    rightBrother->C_ID[j] = cur->C_ID[i];
                    rightBrother->firsts[j] = cur->firsts[i];
                    rightBrother->lasts[j] = cur->lasts[i];
                    rightBrother->children[j] = cur->children[i];
                    j++;
                }
                rightBrother->children[j] = cur->children[i];
                j = index + 1;
                for ( i = index; i < m; i++) {
                    cur->C_ID[i] = 0;
                    cur->firsts[i] = nullptr;
                    cur->lasts[i] = nullptr;
                    cur->children[j] = nullptr;
                    j++;
                }
                cur->children[j] = nullptr;
                cur -> parent = father;
                rightBrother -> parent = father;
                for (i = 0; i < rightBrother->children.size(); i++) {
                    if (rightBrother->children[i] != nullptr) {
                        rightBrother->children[i]->parent = rightBrother;
                    }
                }
                root = father;
                return;
            }
            if (validElements(cur->C_ID) > m - 1) {                     //cepa se cur, srednji ide kod oca, i otac je pun

                Node pomoc = Node(m);
                Node *rightBrother = new Node(m);
                prev = cur;
                cur = cur->parent;
                index = (m % 2) ? m / 2 : m / 2 - 1;
                pomoc.C_ID[0] = prev->C_ID[index];
                pomoc.firsts[0] = prev->firsts[index];
                pomoc.lasts[0] = prev->lasts[index];
                int j = 0;
                int i;
                for (i = index+1; i < m; i++, j++) {
                    rightBrother->C_ID[j] = prev->C_ID[i];
                    rightBrother->firsts[j] = prev->firsts[i];
                    rightBrother->lasts[j] = prev->lasts[i];
                    rightBrother->children[j] = prev->children[i];
                }
                rightBrother->children[j] = prev->children[i];
                for (i = index; i < m; i++) {
                    prev->C_ID[i] = 0;
                    prev->firsts[i] = nullptr;
                    prev->lasts[i] = nullptr;
                    prev->children[i+1] = nullptr;
                }

                index = findIndexForLongInt(cur->C_ID, pomoc.C_ID[0]);

                for (j = cur->C_ID.size() - 1; j > index; j--) {
                    cur->C_ID[j] = cur->C_ID[j - 1];
                    cur->firsts[j] = cur->firsts[j - 1];
                    cur->lasts[j] = cur->lasts[j - 1];
                }
                cur->C_ID[index] = pomoc.C_ID[0];
                cur->firsts[index] = pomoc.firsts[0];
                cur->lasts[index] = pomoc.lasts[0];

                for (i = m; i>index; i--) {
                    cur->children[i] = cur->children[i - 1];
                }
                cur->children[index] = prev;
                cur->children[index + 1] = rightBrother;
                rightBrother->parent=cur;
                for (i = 0; i < rightBrother->children.size(); i++) {
                    if (rightBrother->children[i] != nullptr) {
                        rightBrother->children[i]->parent = rightBrother;
                    }
                }
                if (validElements(cur->C_ID) <= m - 1) {
                    break;
                }
            }
        }

    }


}

void Btree::addAccountToBTree(long int key, CustomerAccount* account) {
    int i, flag = 0;
    if (root == nullptr) {
        root = new Node(m);
        root->C_ID[0] = key;
    }
    Node* tmp = root;
    Node* prev = nullptr;
    while (tmp != nullptr) {
        for (i = 0; i < m - 1; i++) {
            if (key < tmp->C_ID[i] || key == tmp->C_ID[i] || tmp->C_ID[i] == 0) {
                break;
            }
        }
        if (key == tmp->C_ID[i]) {
            tmp->addAccountToCustomer(account, i);
            flag = 1;
            break;
        } else if (key < tmp->C_ID[i] || tmp->C_ID[i] == 0) {
            prev = tmp;
            tmp = tmp->children[i];
        } else if (key > tmp->C_ID[i]) {
            if (i == m - 1) {                                   //potencijalna greska
                prev = tmp;
                tmp = tmp->children[i];
            } else {
                prev = tmp;
                tmp = tmp->children[i + 1];
            }
        }
    }
    if (!flag) {
        if (tmp == root) {
            addCustomerToBTree(key, tmp, account);
            tmp->addAccountToCustomer(account, i);              //salje se koren
        } else {
            addCustomerToBTree(key, prev, account);
        }
    }
}

Btree::Btree(int m): m(m) {
    ifstream dat1;
    dat1.open("./CustomerAccount20.txt");
    if (!dat1.is_open()) cout << "Datoteka nije ispravno otvorena." << endl;
    while (dat1.peek() != -1) {
        long int key;
        string line;
        getline(dat1, line);

        CustomerAccount* account = new CustomerAccount;                                         //new - obrisati - kreiran ACC
        key = parseStringLineIntoAcc(line, account);

        addAccountToBTree(key, account);
    }
    dat1.close();
}

void Btree::printBTree() {

    if (!root) {
        cout << "Stablo je prazno." << endl;
        return;
    }

    queue<Node*> q;
    q.push(root);
    q.push(nullptr); //krajjjj

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current == nullptr) {

            cout << endl;
            if (!q.empty()) {
                q.push(nullptr);
            }
        } else {

            for (int i = 0; i < current->C_ID.size(); i++) {
                cout << current->C_ID[i] << " ";
            }
            cout << ": ";
            for (int i = 0; i < current->children.size(); i++) {
                if (current->children[i]) {
                    for (int j = 0; j < current->children[i]->C_ID.size(); j++) {
                        cout << current->children[i]->C_ID[j] << " ";
                    }
                    if (i != current->children.size() - 1) {
                        cout << " | ";
                    }
                }
            }
            cout << endl;

            for (Node* child : current->children) {
                if (child) {
                    q.push(child);
                }
            }
        }
    }
}

void deleteBTree(Btree tree) {
    if (!tree.getRoot()) {
        return;
    }
    queue<Node*> nodeQueue;
    nodeQueue.push(tree.getRoot());
    while (!nodeQueue.empty()) {
        Node *current = nodeQueue.front();
        nodeQueue.pop();

        for (CustomerAccount *account: current->firsts) {
            CustomerAccount* cur = account;
            CustomerAccount* prev = nullptr;
            while (cur) {
                prev = cur;
                cur = cur ->next;
                delete prev;
            }
        }
        current->C_ID.clear();
        current->firsts.clear();
        current->lasts.clear();
        current->children.clear();
        for (Node *child: current->children) {
            if (child) {
                nodeQueue.push(child);
            }
        }
        delete current;
    }
}

void getInfoAboutCustomer(Btree tree, long int keyToFind) {
    ifstream dat1;
    int br = 0;
    long int tmp, C_ID;
    string Fname, Lname, email;
    dat1.open("/Users/vukluzanin/Documents/ETF/ASP/ASP2/ASP2 drugi domaci/Customer.txt");
    if (!dat1.is_open()) cout << "Datoteka nije ispravno otvorena." << endl;

    if (dat1.peek() != -1) {

        string line;
        getline(dat1, line);

        istringstream iss(line);
        string ex;
        getline(iss, ex, '|');
        C_ID = stol(ex);
        while (C_ID != keyToFind) {
            getline(dat1, line);
            iss.str(line);
            getline(iss, ex, '|');
            C_ID = stol(ex);
        }
        getline(iss, ex, '|');
        Fname = ex;
        getline(iss, ex, '|');
        Lname = ex;
        getline(iss, ex, '|');
        email = ex;
    }
    dat1.close();
    Node* cur = tree.getRoot();
    int flag = 0;
    int i;
    while (cur != nullptr) {
        for (i = 0; i < validElements(cur->C_ID);) {
            if (keyToFind == cur->C_ID[i]) {
                flag = 1;
                break;
            } else if (keyToFind > cur->C_ID[i]&& cur->C_ID[i+1] != 0) {
                i++;
            } else if (keyToFind > cur->C_ID[i] && cur->C_ID[i+1] == 0) {
                cur = cur->children[i+1];
                i = 0;
            }else {
                cur = cur->children[i];
                i = 0;
            }
            br++;
        }
        if (flag) {
            break;
        }
    }
    cout << endl;
    const char* fileName = "IspisRacuna.txt";
    ofstream outFile(fileName);
    if (outFile.is_open()) {
        if (cur != nullptr) {
            br++;
            CustomerAccount* acc = cur->firsts[i];
            while (acc) {
                outFile << C_ID << "|" << Fname << "|" << Lname << "|" << email << "|"<< acc->CA_ID << "|" << cur->C_ID[i] << "|" << acc->CA_TAX_ST <<"|"<< acc->CA_BAL << endl;
                acc = acc->next;
            }
        } else {
            br++;
            outFile << "Korisnik ne poseduje nijedan racun." << endl;
            outFile << C_ID << "|" << Fname << "|" << Lname << "|" << email << "|null|null|null|null" << endl;
        }
        outFile << "Broj koraka pretrage je: " << br << "." << endl;
        outFile.close();
    }
}

void deleteCustomer(Btree tree, long int key) {
    int min = (tree.getM()%2 ? tree.getM()/2 : tree.getM()/2 - 1);
    int br = 0, index;
    long int change;
    Node* cur = tree.getRoot();
    Node* father, *rightBrother, *leftBrother;
    int flag = 0;
    int i;
    while (cur != nullptr) {
        for (i = 0; i < validElements(cur->C_ID);) {
            if (key == cur->C_ID[i]) {
                flag = 1;
                break;
            } else if (key > cur->C_ID[i]&& cur->C_ID[i+1] != 0) {
                i++;
            } else if (key > cur->C_ID[i] && cur->C_ID[i+1] == 0) {
                cur = cur->children[i+1];
                i = 0;
            }else {
                cur = cur->children[i];
                i = 0;
            }
            br++;
        }
        if (flag) {
            break;
        }
    }                                                                   //nasli smo C_ID na poziciji i
    cout << endl;
    if (cur != nullptr) {                                               //nasli smo customera
        if (validElementsChildren(cur->children)) {                  //ako je kljuc u cvorovima grananja
            Node* tmp = cur->children[i+1];
            Node* prev = nullptr;
            while (tmp) {                                               //spustamo se do lista, prev ce pokazivati na list
                prev = tmp;
                tmp = tmp->children[0];
            }
            change = cur->C_ID[i];
            CustomerAccount* changeF = cur->firsts[i];
            CustomerAccount* changeL = cur->lasts[i];
            cur->C_ID[i] = prev->C_ID[0];
            cur->firsts[i] = prev->firsts[0];
            cur->lasts[i] = prev->lasts[0];
            prev->C_ID[0] = change;
            prev->firsts[0] = changeF;
            prev->lasts[0] = changeL;
            cur = prev;
            i = 0;
        }
        //BRISANJE IZ LISTA    element je na poziciji cur->C_ID[i]
        for (int j = i; j < validElements(cur->C_ID); j++) {
            cur->C_ID[j] = cur->C_ID[j+1];
            cur->firsts[j] = cur->firsts[j+1];                                                      //ovde dodati brisanje racuna
            cur->lasts[j] = cur->lasts[j+1];
        }


        flag = 0;
        if (validElements(cur->C_ID) >= min) {                                                  //u cvoru je ostalo dovoljno elemenata
            cout << "Broj koraka pretrage je: " << br << "." << endl;
            return;
        } else {                                                                                      //potrebna pozajmica
            //1. od desnog brata
            father = cur->parent;
            index = findIndexForLongInt(father->C_ID, cur->C_ID[0]);                                //index desnog oca
            if (father->children[index+1]) {                                                                    //postoji desni brat
                if (validElements(father->children[index+1]->C_ID) > min) {
                    cur->C_ID[validElements(cur->C_ID)] = father->C_ID[index];                               //DODATI DA SE PREVEZE I INFO
                    father->C_ID[index] = father->children[index+1]->C_ID[0];
                    flag = 1;

                    for (int j = 0; j < validElements(father->children[index+1]->C_ID); j++) {
                        father->children[index+1]->C_ID[j] = father->children[index+1]->C_ID[j+1];
                        father->children[index+1]->firsts[j] = father->children[index+1]->firsts[j+1];                          //ovde dodati brisanje racuna
                        father->children[index+1]->lasts[j] = father->children[index+1]->lasts[j+1];
                        father->children[index+1]->children[j] = father->children[index+1]->children[j+1];
                    }
                }
            }
            if (!flag && father->children[index-1]) {                                                                               //postoji levi brat
                if (validElements(father->children[index-1]->C_ID) > min) {
                    for (int y = cur->C_ID.size()-1; y > 0; y--) {
                        cur->C_ID[y] = cur->C_ID[y-1];
                        cur->children[y] = cur->children[y-1];
                    }
                    cur->C_ID[0] = father->C_ID[index-1];                                                                           //DODATI DA SE PREVEZE I INFO
                    cur->children[0] = father->children[index-1]->children[validElements(father->children[index-1]->C_ID)];
                    father->C_ID[index-1] = father->children[index-1]->C_ID[validElements(father->children[index-1]->C_ID) - 1];
                    father->children[index-1]->C_ID[validElements(father->children[index-1]->C_ID) - 1] = 0;
                    flag = 1;
                }
            }
            if (!flag && father->children[index+1]) {                                                                           //spajanje sa desnim bratom
                cur->C_ID[validElements(cur->C_ID)] = father->C_ID[index];
                int a = validElements(cur->C_ID);
                for (int y = 0; y < validElements(father->children[index+1]->C_ID); y++) {                              //dodati pomeranje dece
                    cur->C_ID[a + y] = father->children[index+1]->C_ID[y];
                }
                father->children[index+1] = nullptr;
                father->C_ID[index] = 0;
                flag = 1;
            }
            if (!flag && father->children[index-1]) {                                                                           //spajanje sa levim bratom
                father->children[index-1]->C_ID[validElements(father->children[index-1]->C_ID)] = father->C_ID[index-1];
                father->C_ID[index-1] = 0;
                int a = validElements(father->children[index-1]->C_ID);
                for (int y = 0; y < validElements(cur->C_ID); y++) {                              //dodati pomeranje dece
                    father->children[index-1]->C_ID[a + y] = cur->C_ID[y];
                }
                father->children[index] = nullptr;
                flag = 1;
            }

            cur = cur->parent;


        }
        //tree.printBTree();
    } else {
        br++;
        cout << "Korisnik ne poseduje nijedan racun." << endl;
    }
    cout << "Broj koraka pretrage je: " << br << "." << endl;
}

void deleteAccount(Btree tree, long int key, CustomerAccount* account) {
    int br = 0;
    Node* cur = tree.getRoot();
    int flag = 0;
    int i;
    while (cur != nullptr) {
        for (i = 0; i < validElements(cur->C_ID);) {
            if (key == cur->C_ID[i]) {
                flag = 1;
                break;
            } else if (key > cur->C_ID[i]&& cur->C_ID[i+1] != 0) {
                i++;
            } else if (key > cur->C_ID[i] && cur->C_ID[i+1] == 0) {
                cur = cur->children[i+1];
                i = 0;
            }else {
                cur = cur->children[i];
                i = 0;
            }
            br++;
        }
        if (flag) {
            break;
        }
    }
    cout << endl;
    if (cur != nullptr) {                                       //nasli smo customera
        br++;
        CustomerAccount* acc = cur->firsts[i];
        CustomerAccount* prev = nullptr;
        while (acc) {
            if (acc->CA_ID == account->CA_ID) {
                if (cur->firsts[i] == cur->lasts[i]) {
                    deleteCustomer(tree, key);
                }
                if (prev == nullptr) {
                    cur->firsts[i] = acc->next;
                } else {
                    prev->next = acc->next;
                }
                if (acc->next == nullptr) {
                    cur->lasts[i] = prev;
                }
                delete acc;
                break;
            }
            br++;
            prev = acc;
            acc = acc->next;
        }
        if (acc == nullptr) {
            cout << "Korisnik ne poseduje zadati racun." << endl;
        }
    } else {
        br++;
        cout << "Korisnik ne poseduje nijedan racun." << endl;
    }
    cout << "Broj koraka pretrage je: " << br << "." << endl;
}

//kad brisem customera, obrisati sve racune
//dodati proveru ako vec racun pod ca_id postoji vec u stablu
int main() {
    cout << fixed;
    cout << setprecision(2);
    int choice = 0, m, key;
    string line;
    cout << endl << "POKRENULI STE ZADATAK - EFIKASNO PRETRAZIVANJE PODATAKA IZ VISEA TABELA BAZE PODATAKA." << endl;
    for (int i = 0; i<200; i++) cout << '-'; cout << endl << endl;
    cout << "Unesite red B stabla koje želite da stvorite." << endl;
    cin >> m;
    if (m < 3 || m > 10) {
        cout << "Uneli ste nevalidans red stabla, pokušajte ponovo." << endl;
        cin >> m;
    }
    Btree tree(m);
    do {
        cout << endl << "Navođenjem broja stavke možete izabrati operaciju." << endl;
        cout << "0. Prekid programa." << endl;
        cout << "1. Uništavanje indeksa nad tabelom 'CUSTOMER_ACCOUNT'." << endl;
        cout << "2. Ispis indeksa na standardni izlaz." << endl;
        cout << "3. Ispis podataka o korisniku." << endl;
        cout << "4. Dodavanje novog zapisa u indeks." << endl;
        cout << "5. Brisanje računa koji se navede." << endl;
        cout << "6. Brisanje svih računa jednog CUSTOMER-a." << endl;
        cin >> choice;
        switch (choice) {
            case 1: {

                deleteBTree(tree);
                break;
            } case 2: {

                cout << "Ovako izgleda formiran indeks u obliku: cvor: potomci odvojeni |" << endl << endl;
                tree.printBTree();
                break;
            } case 3: {

                long int tmp;
                cout << "Unesite C_ID korisnika koga biste pretrazili: " << endl;
                cin >> tmp;
                getInfoAboutCustomer(tree, tmp);
                cout << "Rezultat je ispisan u datoteku IspisRacuna.txt." << endl;
                break;
            } case 4: {

                cout<< "Unesite novi zapis po formatu:" << endl;
                cout << "CA_ID|CA_B_ID|CA_C_ID|CA_NAME|CA_TAX_ST|CA_BAL" << endl << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, line);
                CustomerAccount* account = new CustomerAccount;                                         //new - obrisati - kreiran ACC
                key = parseStringLineIntoAcc(line, account);
                tree.addAccountToBTree(key, account);
                break;
            } case 5: {

                cout << "Unesite racun koji zelite da izbrisete po sledecem formatu: CA_ID|CA_B_ID|CA_C_ID|CA_NAME|CA_TAX_ST|CA_BAL." << endl << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, line);
                CustomerAccount* account = new CustomerAccount;                                         //new - obrisati - kreiran ACC
                long int k = parseStringLineIntoAcc(line, account);
                deleteAccount(tree, k, account);
            } break;
            case 6: {

                long int k;
                cout << "Unesite C_ID Customera koga zelite da izbrisete: " << endl << endl;
                cin >> k;
                deleteCustomer(tree, k);
            } break;
            case 0: cout << "Kraj programa."; break;
            default:
                cout << "Nekorektna opcija. Molimo vas da unesete ponovo." << endl;
        }
    } while (choice);
    return 0;
}