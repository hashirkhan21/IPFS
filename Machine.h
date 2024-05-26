#include <iostream>
#include <cmath>
#include <bitset>
#include <string>
#include <Windows.h>
#include <fstream>
#include <cstdlib>
#include "SHA1.h"
#include "Btree.h"
using namespace std;

int identifier_space;
int choice = 0;
int numMachines = 0;
int numm = -1;
bool autoInsert = 0;

void takeInput() {
    cout << "Input identifier space: ";
    cin >> identifier_space;
    cout << "Enter degree of B tree: ";
    cin >> Bdegree;
    cout << "Enter Number of machines: ";
    cin >> numMachines;
    cout << endl;
    cout << "Do you want manual or automatic insertion of machines? Press 1 for manual and 2 for automatic. ";
    cin >> choice;
}

void routingTableOrinsertFile() {
    cout << "Press 1 to see routing table of specified machine\n";
    cout << "Press 2 to insert file\n";
    cout << "Press 3 to see B tree of machine\n";
    cout << "Press 4 to add another machine\n";
    cout << "Press 5 to remove machine\n";
    cout << "Press 6 to remove file\n";
    cout << "Press 7 to search a file\n";
    cout << "Press 0 to see Exit ";
    cin >> numm;
}

void createFolder(const wstring& machineValue) {
    wstring folderPath = L"Root\\";
    std::wstring result = folderPath + machineValue;
    if (CreateDirectory(result.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
        //wcout << L"Folder for machine " << machineValue << L" created successfully." << endl;
    }
    else {
        wcerr << L"Failed to create folder for machine " << machineValue << L". Error code: " << GetLastError() << endl;
    }
}


void extractElements(string& ans, string extractFrom, int length, int identifierSpace) {
    int l = extractFrom.size() - 1;
    while (true) {
        if (identifierSpace <= 0)
            break;
        ans += extractFrom[l--];
        identifierSpace -= 4;
    }
}
string string_to_binary(string d) {
    string n;
    int i = d.size() - 1;

    while (i >= 0)
    {
        if (d[i] == '0')
            n += "0000";
        else if (d[i] == '1')
            n += "0001";
        else if (d[i] == '2')
            n += "0010";
        else if (d[i] == '3')
            n += "0011";
        else if (d[i] == '4')
            n += "0100";
        else if (d[i] == '5')
            n += "0101";
        else if (d[i] == '6')
            n += "0110";
        else if (d[i] == '7')
            n += "0111";
        else if (d[i] == '8')
            n += "1000";
        else if (d[i] == '9')
            n += "1001";
        else if (d[i] == 'a')
            n += "1010";
        else if (d[i] == 'b')
            n += "1011";
        else if (d[i] == 'c')
            n += "1100";
        else if (d[i] == 'd')
            n += "1101";
        else if (d[i] == 'e')
            n += "1110";
        else if (d[i] == 'f')
            n += "1111";

        i--;
    }
    return n;
}


string reeeeee(string idd, int i)
{
    int size = idd.size() - 1;
    string h;


    while (i > 0)
    {
        h = idd[size] + h;
        size--;
        i--;
    }
    return h;
}

BigInt calc_bigint(string str) {
    string id;
    int size = str.size() - 1;
    extractElements(id, str, size, identifier_space);
    id = string_to_binary(id);
    id = reeeeee(id, identifier_space);

    BigInt sum("0");
    int x = 0;
    for (int i = id.size() - 1; i >= 0; i--)
    {
        BigInt ll(id[i] - '0');
        BigInt exp(2);
        exp ^= x;
        ll *= exp;
        sum += ll;
        x++;
    }
    return sum;
}



class node {
public:
    SHA1 hash;
    string name;
    string id;
    node* next;
    node* prev;
    BigInt data;
    node** routingTable;
    BTree tree;
    node(string name = "") {
        routingTable = new node * [identifier_space];
        this->name = name;
        hash.update(name);
        string str = hash.final();
        data = calc_bigint(str);
        next = prev = NULL;
        for (int i = 0; i < identifier_space; i++)
            routingTable[i] = nullptr;
    }
};
class Machine {
public:
    node* head;
    int size;
    Machine() {
        head = NULL;
        size = 0;
    }

    void insertMachine(string name) {
        bool collision = 0;
        if (!head) {
            head = new node(name);
            head->next = head;
            head->prev = head;
            size++;
            wstring wideStr(name.begin(), name.end());
            createFolder(wideStr);
            return;
        }
        node* temp = head;
        while (temp->next != head) {
            temp = temp->next;
        }
        node* tail = new node(name);
        if (findByHash(tail->data))
            collision = true;
        while (collision) {
            if (!findByHash(tail->data)) {
                collision = false;
                break;
            }
            else
                tail->data++;
        }
        temp->next = tail;
        tail->prev = temp;
        tail->next = head;
        head->prev = tail;
        size++;
        wstring wideStr(name.begin(), name.end());
        createFolder(wideStr);
        node* current = head;
        sort(head);
        do {
            for (int i = 0; i < identifier_space; i++) {
                current->routingTable[i] = findEntry(current, i + 1);
            }
            current = current->next;
        } while (current != head);

        if (tail->next->tree.root != NULL) {
            fileextract(tail->next->tree);
            while (tail->next->tree.btree_name.empty() == 0 || tail->next->tree.btree_values.empty() == 0) {
                insertFile(tail->next->tree.btree_name.top(), tail->next->tree.btree_values.top());
                tail->next->tree.btree_name.pop();
                tail->next->tree.btree_values.pop();
            }
        }
    }

    node* findEntry(node* startNode, int index) {
        if (index == 0)
            index++;
        int current_val = startNode->data;
        bool isMod = false;
        BigInt modder = pow(2, identifier_space);
        BigInt target = pow(2, index - 1) + current_val;
        BigInt large = findLargestValue(head);
        if (target > large && target <= modder)
            return head;
        if (target >= modder) {
            target = target % modder;
            isMod = true;
        }
        node* curr = startNode;
        do {
            if (curr->data == target) {
                return curr;
            }
            curr = curr->next;
        } while (curr != startNode);
        if (isMod) {
            startNode = head;
        }
        curr = startNode;
        do {
            curr = curr->next;
        } while (curr != startNode && curr->data < target);
        if (curr->data == startNode->data)
            return curr->next;
        return curr;
    }

    node* findNode(BigInt hash) {
        int current_val = head->data;
        bool isMod = false;
        BigInt modder = pow(2, identifier_space);
        BigInt target = hash;
        BigInt large = findLargestValue(head);
        if (target > large && target <= modder)
            return head;
        if (target >= modder) {
            target = target % modder;
            isMod = true;
        }
        node* curr = head->next;
        do {
            if (curr->data == target) {
                return curr;
            }
            curr = curr->next;
        } while (curr != head);
        if (isMod) {
            curr = head;
        }
        curr = head->next;
        do {
            curr = curr->next;
        } while (curr != head && curr->data < target);
        if (curr->data == head->data)
            return curr->next;
        return curr;
    }

    BigInt findLargestValue(node* startNode) {
        if (!startNode) {
            return -1;
        }
        BigInt maxVal = startNode->data;
        node* current = startNode->next;
        while (current != startNode) {
            if (current->data > maxVal) {
                maxVal = current->data;
            }
            current = current->next;
        }
        return maxVal;
    }

    bool findByHash(BigInt hash) {
        node* temp = head->next;
        while (temp != head) {
            if (temp->data == hash)
                return true;
            temp = temp->next;
        }
        return false;
    }


    void print() {
        node* temp = head;
        do {
            cout << temp->data << " " << temp->name << endl;;
            temp = temp->next;
        } while (temp != head);
    }
    void printTable(const string name) {
        node* curr = searchByName(name);
        if (!curr) {
            return;
        }
        if (curr->routingTable[0] == NULL) {
            return;
        }
        cout << "Routing table of " << curr->name << " (" << curr->data << ")\n";
        for (int i = 0; i < identifier_space; i++)
            cout << curr->routingTable[i]->data << "\n";
    }

    void sort(node*& head) {
        if (!head || !head->next)
            return;
        bool swapped;
        node* temp;
        do {
            swapped = false;
            temp = head;

            do {
                if (temp->data > temp->next->data) {
                    std::swap(temp->data, temp->next->data);
                    std::swap(temp->name, temp->next->name);
                    swapped = true;
                }
                temp = temp->next;
            } while (temp->next != head);
        } while (swapped);
    }

    node* searchByName(string startingName) {
        if (head == nullptr) {
            cout << "List is empty." << endl;
            return nullptr;
        }

        node* temp = head;
        node* reference = head;

        do {
            if (temp->name == startingName) {
                return temp;
            }

            temp = temp->next;
        } while (temp != nullptr && temp != reference);

        cout << "Machine does not exist! Recheck name." << endl;
        return nullptr;
    }


    bool search(BigInt value, string startingName) {
        string path;
        bool found = false;
        node* temp = searchByName(startingName);
        if (temp == NULL)
            return false;
        if (temp->tree.root->searchKey(value)) {
            cout << "File found in [" << temp->name << "] machine!\n";
            return true;
        }
            
        path += temp->name + " -> ";
        while (!temp->tree.root->searchKey(value)) {
            found = true;
            if (temp->data < value && temp->routingTable[0]->data >= value) {
                temp = temp->routingTable[0];
                path += temp->name + " -> ";
            }
            else {
                bool tableUpdated = false;
                for (int i = 0; i < identifier_space - 1; i++) {
                    if (value > temp->routingTable[i]->data && value <= temp->routingTable[i + 1]->data) {
                        temp = temp->routingTable[i];
                        path += temp->name + " -> ";
                        tableUpdated = true;
                        break;
                    }
                }
                if (!tableUpdated) {
                    temp = temp->next;
                }
            }
            if (temp == searchByName(startingName)) {
                break;
            }
        }
        path += temp->name;
        if (found) {
            cout << "File found in " << temp->name << "\nPath: " << path << endl;
            return true;
        }
        else {
            cout << "File not found" << endl;
            return false;
        }
    }


    void insertFile(const string& filename, const string& fileContent) {
        if (size == 0) {
            cout << "No machines available!\n";
            return;
        }
        SHA1 hash;
        hash.update(fileContent);
        BigInt j = calc_bigint(hash.final());
        node* targetMachine = findNode(j);
        if (targetMachine == nullptr) {
            std::cerr << "Machine with name " << targetMachine->name << " not found." << std::endl;
            return;
        }
        std::wstring wideMachineName(targetMachine->name.begin(), targetMachine->name.end());
        std::wstring wideFileName(filename.begin(), filename.end());
        std::wstring filePath = L"Root\\" + wideMachineName + L"\\" + wideFileName;
        string actualFilePath = "Root\\" + targetMachine->name + "\\" + filename;
        std::ofstream file(actualFilePath, std::ios::out);
        file << fileContent;
        file.close();
        std::ifstream fileStream(filePath, std::ios::binary);
        targetMachine->tree.insert(j, actualFilePath, fileContent, filename);
        cout << "File inserted at Machine " << targetMachine->name << endl;
    }


    void insertExistingFile(const string& filename) {
        std::string fileContent;
        std::ifstream file(filename, std::ios::binary);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            fileContent = buffer.str();
            file.close();
        }
        else {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        insertFile(filename, fileContent);
    }

    void deleteMachine(const string& machineName) {
        if (size == 1 && head->name == machineName) {
            node* current = head;
            for (int i = 0; i < identifier_space; i++) {
                current->routingTable[i] = NULL;
            }
        }
        node* temp = searchByName(machineName);
        node* prev = nullptr;
        prev = temp->prev;
        if (temp->name == machineName) {
            fileextract(temp->tree);
            wstring wideStr(machineName.begin(), machineName.end());
            wstring folderPath = L"Root\\" + wideStr;
            wcout << folderPath << endl;
            if (RemoveDirectory(folderPath.c_str())) {
                cout << "Folder for machine " << machineName << " deleted successfully." << endl;
                size--;
            }
            else {
                cerr << "Failed to delete folder for machine " << machineName << ". Error code: " << GetLastError() << endl;
            }
            if (temp == head) {
                head = temp->next;
            }
            if (prev) {
                prev->next = temp->next;
            }
            node* current = head;
            if (head && size > 0) {
                do {
                    for (int i = 0; i < identifier_space; i++) {
                        current->routingTable[i] = findEntry(current, i + 1);
                    }
                    current = current->next;
                } while (current != head);
            }
        }
        else {
            cout << "Machine not found: " << machineName << endl;
        }

        if (!head) {
            while (temp->tree.btree_name.empty() == 0 || temp->tree.btree_values.empty() == 0) {
                temp->tree.btree_name.pop();
                temp->tree.btree_values.pop();
                if(!temp->tree.btree_fp.empty())
                    temp->tree.btree_fp.pop();
            }
            return;
        }
        while (temp->tree.btree_name.empty() == 0 || temp->tree.btree_values.empty() == 0) {
            insertFile(temp->tree.btree_name.top(), temp->tree.btree_values.top());
            temp->tree.btree_name.pop();
            temp->tree.btree_values.pop();
        }
        delete temp;
    }



    void deleteF(BigInt hash) {
        node* targetMachine = findNode(hash);
        btnode* temp = targetMachine->tree.search(hash);
        if (temp == NULL) {
            cout << "File not found!\n";
            return;
        }
        int i = 0;
        for (; i < temp->count; i++) {
            if (temp->key[i] == hash) {
                deleteFile(hash, temp->name[i], targetMachine);
                i--;
            }
        }     
    }

    void deleteFile(BigInt hash, string filename, node* targetMachine) {

        if (targetMachine == nullptr) {
            std::cerr << "Machine with name " << targetMachine->name << " not found." << std::endl;
            return;
        }
        targetMachine->tree.deletion(hash);
        std::wstring wideMachineName(targetMachine->name.begin(), targetMachine->name.end());
        std::wstring wideFileName(filename.begin(), filename.end());
        std::wstring filePath = L"Root\\" + wideMachineName + L"\\" + wideFileName;
        std::string actualFilePath = "Root\\" + targetMachine->name + "\\" + filename;

        if (DeleteFile(filePath.c_str())) {
            std::wcout << L"File deleted successfully: " << filePath << std::endl;
        }
        else {
            std::wcerr << L"Failed to delete file. Error code: " << GetLastError() << std::endl;
        }
    }

    void printTree(const string name) {
        node* temp = searchByName(name);
        if (!temp) {
            cout << "Machine by that name does not exist!" << endl;
            return;
        }
        else
            temp->tree.levelOrderPrint();
    }
};
