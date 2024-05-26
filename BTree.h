#include<string>
#include<fstream>
#include<cstring>
#include <stack>
#include "queue"
#include "bigInt.h"
using namespace std;



int Bdegree;

void inputDegree() {
    cout << "Enter degree of B tree: ";
    cin >> Bdegree;
    cout << endl;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class btnode
{
public:
    int min;
    int order;
    BigInt* key;
    string* value;
    string* name;
    btnode** child;
    int count;
    bool leaf;
    string* fp;


    btnode(int min, bool flag)
    {
        this->min = min;
        leaf = flag;
        order = min * 2;
        key = new BigInt[order - 1];
        value = new string[order - 1];
        name = new string[order - 1];
        child = new btnode * [order];
        fp = new string[order - 1];
        count = 0;
    }

    void notfull(BigInt k, string ff, string val, string hh)
    {
        int i = count - 1;//0

        if (leaf == true) {
            for (; i >= 0 && key[i] > k;)
            {
                key[i + 1] = key[i];
                value[i + 1] = value[i];
                fp[i + 1] = fp[i];
                name[i + 1] = name[i];

                i--;
            }

            key[i + 1] = k;
            value[i + 1] = val;
            fp[i + 1] = ff;
            name[i + 1] = hh;
            count++;
        }
        else
        {
            while (i >= 0 && key[i] > k)
                i--;

            if (child[i + 1]->count == (order)-1)
            {
                schild(i + 1, child[i + 1], ff, val, hh);

                if (key[i + 1] < k)
                    i++;
            }
            child[i + 1]->notfull(k, ff, val, hh);
        }
    }

    bool searchKey(BigInt val)
    {
        if (this == NULL)
            return false;
        int i = 0;
        while (i < count && key[i] < val)
            i++;
        if (i < count && key[i] == val)
            return true;
        else if (leaf)
            return false;
        else
            return child[i]->searchKey(val);
    }

    bool searchKey3(BigInt val)
    {
        int i = 0;
        while (i < count && key[i] < val)
            i++;
        if (i < count && key[i] == val)
            return true;
        else if (leaf)
            return false;
        else
            return child[i]->searchKey3(val);
    }

    void schild(int i, btnode* x, string ff, string val, string hh)
    {
        btnode* temp = new btnode(x->min, x->leaf);
        temp->count = min - 1;
        for (int j = 0; j < min - 1; j++)
        {
            temp->key[j] = x->key[j + min];
            temp->fp[j] = x->fp[j + min];
            temp->value[j] = x->value[j + min];
            temp->name[j] = x->name[j + min];
        }
        if (x->leaf == false)
        {
            for (int j = 0; j < min; j++)
                temp->child[j] = x->child[j + min];
        }

        x->count = min - 1;
        for (int j = count; j >= i + 1; j--)
        {
            child[j + 1] = child[j];
        }
        child[i + 1] = temp;

        for (int j = count - 1; j >= i; j--)
        {
            key[j + 1] = key[j];
            fp[j + 1] = fp[j];
            value[j + 1] = value[j];
            name[j + 1] = name[j];

        }
        key[i] = x->key[min - 1];
        fp[i] = x->fp[min - 1];
        value[i] = x->value[min - 1];
        name[i] = x->name[min - 1];
        count++;
    }
    btnode* search(BigInt val)
    {
        btnode* temp = NULL;
        int i = 0;
        while (i < count && key[i] < val)
            i++;

        int n = i;

        if (key[i] == val)
        {
            return this;
        }
        else if (leaf == true)
        {
            return temp;
        }
        return (child[i]->search(val));
    }

    btnode* deletekey(BigInt val)
    {
        int i;

        for (i = 0; i < count; i++)
        {
            if (leaf == false)
            {

                child[i]->deletekey(val);


            }
            if (key[i] == val)
            {
                //cout << key[i] << "||" << line[i] << "||" << filename[i] << endl;

                //l[i]->print();
                wstring wideStr(fp[i].begin(), fp[i].end());
                deleteFile(wideStr);
                return NULL;
            }

        }
        if (leaf == false)
        {
            child[i]->deletekey(val);
        }
        //cout << endl;
    }

    void deleteFile(const std::wstring& filePath) {
        if (DeleteFile(filePath.c_str())) {
            std::wcout << L"File deleted successfully: " << filePath << std::endl;
        }
        else {
            std::wcerr << L"Failed to delete file. Error code: " << GetLastError() << std::endl;
        }
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void deletion(BigInt val)
    {
        int i = 0;
        while (i < count && key[i] < val)
            i++;

        int n = i;

        if (n < count && key[n] == val)
        {
            /*if (key[n] == val && l[n] != NULL)
            {
                l[n]->remove(t val);
            }*/
            if (leaf == true)
            {
                removeleaf(n);
            }
            else
            {
                removeinternal(n);
            }
        }
        else
        {
            bool check = false;
            if (leaf == true)
            {
                cout << val << " doesn't exist in tree\n";
                return;
            }

            if (n == count)
            {
                check = true;
            }
            else
            {
                check = false;
            }

            if (child[n]->count < min)
            {
                fill(n);
            }
            if (check == true && n > count)
            {
                child[n - 1]->deletion(val);
            }
            else
            {
                child[n]->deletion(val);
            }
        }
        return;
    }

    void removeleaf(int v)
    {
        for (int i = v + 1; i < count; i++)
        {
            key[i - 1] = key[i];
            fp[i - 1] = fp[i];
            value[i - 1] = value[i];
            name[i - 1] = name[i];
        }
        count--;
        return;
    }

    // Delete from non leaf node
    void removeinternal(int v)
    {
        BigInt temp = key[v];

        if (child[v]->count >= min)
        {
            BigInt pre = getPredecessor(v);
            key[v] = pre;
            child[v]->deletion(pre);
        }

        else if (child[v + 1]->count >= min)
        {
            BigInt suc = getSuccessor(v);
            key[v] = suc;
            child[v + 1]->deletion(suc);
        }

        else
        {
            merge(v);
            child[v]->deletion(temp);
        }
        return;
    }

    BigInt getPredecessor(int v)
    {
        btnode* temp = child[v];
        while (!temp->leaf)
            temp = temp->child[temp->count];

        return temp->key[temp->count - 1];
    }

    BigInt getSuccessor(int v) {
        btnode* temp = child[v + 1];
        while (!temp->leaf)
            temp = temp->child[0];

        return temp->key[0];
    }

    void fill(int v) {
        if (v != 0 && child[v - 1]->count >= min)
            borrowFromPrev(v);

        else if (v != count && child[v + 1]->count >= min)
            borrowFromNext(v);

        else {
            if (v != count)
                merge(v);
            else
                merge(v - 1);
        }
        return;
    }

    // Borrow from previous
    void borrowFromPrev(int v)
    {
        btnode* c = child[v];
        btnode* s = child[v - 1];

        for (int i = c->count - 1; i >= 0; i--)
        {
            c->key[i + 1] = c->key[i];
            c->fp[i + 1] = c->fp[i];
            c->value[i + 1] = c->value[i];
            c->name[i + 1] = c->name[i];
        }
        if (!c->leaf) {
            for (int i = c->count; i >= 0; i--)
                c->child[i + 1] = c->child[i];
        }

        c->key[0] = key[v - 1];
        c->fp[0] = fp[v - 1];
        c->value[0] = value[v - 1];
        c->name[0] = name[v - 1];

        if (!c->leaf)
            c->child[0] = s->child[s->count];

        key[v - 1] = s->key[s->count - 1];
        fp[v - 1] = s->fp[s->count - 1];
        value[v - 1] = s->value[s->count - 1];
        name[v - 1] = s->name[s->count - 1];

        c->count += 1;
        s->count -= 1;

        return;
    }

    void borrowFromNext(int v) {
        btnode* c = child[v];
        btnode* s = child[v + 1];

        c->key[(c->count)] = key[v];

        if (!(c->leaf))
        {
            c->child[(c->count) + 1] = s->child[0];
        }

        key[v] = s->key[0];
        fp[v] = s->fp[0];
        value[v] = s->value[0];
        name[v] = s->name[0];

        for (int i = 1; i < s->count; i++)
        {
            s->key[i - 1] = s->key[i];
            s->fp[i - 1] = s->fp[i];
            s->value[i - 1] = s->value[i];
            s->name[i - 1] = s->name[i];
        }

        if (!s->leaf) {
            for (int i = 1; i <= s->count; i++)
            {
                s->child[i - 1] = s->child[i];
            }
        }

        c->count += 1;
        s->count -= 1;

        return;
    }

    //// Merge
    void merge(int v) {
        btnode* c = child[v];
        btnode* s = child[v + 1];

        c->key[min - 1] = key[v];
        c->fp[min - 1] = fp[v];
        c->value[min - 1] = value[v];
        c->name[min - 1] = name[v];

        for (int i = 0; i < s->count; i++)
        {
            c->key[i + min] = s->key[i];
            c->fp[i + min] = s->fp[i];
            c->value[i + min] = s->value[i];
            c->name[i + min] = s->name[i];
        }

        if (!c->leaf)
        {
            for (int i = 0; i <= s->count; i++)
            {
                c->child[i + min] = s->child[i];
            }
        }

        for (int i = v + 1; i < count; i++)
        {
            key[i - 1] = key[i];
            fp[i - 1] = fp[i];
            value[i - 1] = value[i];
            name[i - 1] = name[i];
        }
        for (int i = v + 2; i <= count; i++)
        {
            child[i - 1] = child[i];
        }
        c->count += s->count + 1;
        count--;

        delete (s);
        return;
    }

};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BTree
{
public:
    int min;
    btnode* root;
    int order;
    stack <string> btree_values;
    stack <string> btree_name;
    stack <string> btree_fp;

        BTree(int min = Bdegree)
    {
        this->min = min;
        root = NULL;
        this->order = min * 2;
    }

    void insert(BigInt k, string ff, string val, string nn)
    {
        btree_fp.push(ff);
        btree_name.push(nn);
        btree_values.push(val);


        if (root == NULL)
        {
            root = new btnode(min, true);
            root->key[0] = k;
            root->fp[0] = ff;
            root->value[0] = val;
            root->name[0] = nn;

            root->count = 1;
        }
        else {
            if (root->count == order - 1)
            {
                btnode* temp = new btnode(min, false);
                temp->child[0] = root;
                temp->schild(0, root, ff, val, nn);
                int i = 0;
                if (temp->key[0] < k)
                    i++;
                temp->child[i]->notfull(k, ff, val, nn);

                root = temp;
            }
            else
                root->notfull(k, ff, val, nn);
        }
    }

    ////search function
    btnode* search(BigInt k)
    {
        if (root == NULL)
        {
            return NULL;
        }
        else
        {
            btnode* result = root->search(k);
            if (result == NULL)
            {
                return NULL;
            }
            else
            {
                // Create a new BTree instance and set its root to the result
                return result;
            }
        }
    }




    BTree* deletekey(BigInt k)
    {
        if (root == NULL)
        {
            return NULL;
        }
        else
        {
            root->deletekey(k);
            return NULL;
        }
    }


    ////delete function in tree
    void deletion(BigInt k)
    {
        if (root == NULL)
        {
            cout << "The tree is empty" << endl;
            return;
        }
        else
        {
            root->deletion(k);

            if (root->count == 0)
            {
                btnode* temp = root;
                if (root->leaf == true)
                {
                    root = NULL;
                }
                else
                {
                    root = root->child[0];
                }
                delete temp;
            }
            return;
        }
    }


    void levelOrderPrint()
    {
        if (this == NULL)
            return;

        queue<btnode*> q;
        q.push(root);

        while (!q.empty())
        {
            int nodeCount = q.size();

            while (nodeCount > 0)
            {
                btnode* currentNode = q.front();
                if (currentNode == NULL) {
                    cout << "Tree is empty!\n";
                    return;
                }
                q.pop();
                cout << "[";
                for (int i = 0;currentNode && i < currentNode->count; ++i)
                {
                    cout << currentNode->key[i] << " ";
                }
                cout << "]  ";
                if (!currentNode->leaf)
                {
                    for (int i = 0; i <= currentNode->count; ++i)
                    {
                        if (currentNode->child[i] != NULL)
                            q.push(currentNode->child[i]);
                    }
                }

                --nodeCount;
            }

            cout << endl;
        }
    }
};

void fileextract(BTree& temp) {
    while (temp.root != NULL) {
        // Get the file path and value
        // Delete the BTree entry
        temp.deletion(temp.root->key[0]);
    }
    while (!temp.btree_fp.empty()) {
        string filePath = temp.btree_fp.top();
        temp.btree_fp.pop();
        // Delete the physical file
        if (remove(filePath.c_str()) != 0) {
            cerr << "Error deleting file: " << filePath << endl;
        }
        else {
            cout << "File deleted successfully: " << filePath << endl;
        }
    }
}
