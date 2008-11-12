#ifndef AVL_HPP_INCLUDED
#define AVL_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <assert.h>


using namespace std;


template <class T_TYPE> struct Tnode
{
    T_TYPE key;
    struct Tnode<T_TYPE> * left;
    struct Tnode<T_TYPE> * right;
    struct Tnode<T_TYPE> * next;
    struct Tnode<T_TYPE> * prev;
    int bf;
};


template <class T_TYPE> class AVL
{
    public :
        typedef struct Tnode<T_TYPE> TNODE;

        AVL();
        virtual ~AVL();

        inline void print() { print(root, 0); }
        void print_node(TNODE * node);

        void insert(T_TYPE kkey);
        TNODE * find(T_TYPE kkey);
        void remove(T_TYPE kkey);

        inline struct Tnode<T_TYPE> * get_min () { return get_min(root); }


    private :
        TNODE * root;

        TNODE * alloc_new_node(T_TYPE kkey);
        //void print_node(TNODE * node);
        void print(TNODE * node, int tabs);
        TNODE * get_min (TNODE * node);

        inline void check_parent(TNODE * parent, TNODE * y, TNODE * node);
        inline void left_rot1(TNODE * y, TNODE * z, TNODE * parent, bool zero = false);
        inline void left_rot2(TNODE * y, TNODE * z, TNODE * t, TNODE * parent);
        inline void right_rot1(TNODE * y, TNODE * z, TNODE * parent, bool zero = false);
        inline void right_rot2(TNODE * y, TNODE * z, TNODE * t, TNODE * parent);
};



template <class T_TYPE>
void
AVL<T_TYPE>::remove (T_TYPE kkey)
{
    // * Determine path to leaf *
    vector<TNODE *> path;
    path.push_back(NULL);
    TNODE * node = root;
    while (node) {
        if (node->key == kkey) break;
        path.push_back(node);
        node = (kkey < node->key) ? node->left : node->right;
    }
    assert(node);

    // * Removing node from tree *
    TNODE * parent = path.back();
    bool lson;
    if (node->right == NULL) {
        if (parent) {
            lson = (parent->left == node) ? true : false;
            ((parent->left == node) ? parent->left : parent->right) = node->left;
        }
        else {
            root = node->left;
        }
        if (node->prev)  node->prev->next = node->next;
        if (node->next)  node->next->prev = node->prev;
    }
    else if (node->left == NULL) {
        if (parent) {
            lson = (parent->left == node) ? true : false;
            ((parent->left == node) ? parent->left : parent->right) = node->right;
        }
        else {
            root = node->right;
        }
        if (node->prev)  node->prev->next = node->next;
        if (node->next)  node->next->prev = node->prev;
    }
    else {
        int i_rmin = path.size();
        path.push_back(NULL);

        TNODE * prev = node;
        TNODE * rmin = node->right;
        while (rmin->left) {
            path.push_back(rmin);
            prev = rmin;
            rmin = rmin->left;
        }
        path[i_rmin] = rmin;
        //path.pop_back();
        lson = (node->right != rmin) ? true : false;

        //printNode(prev);
        //printNode(rmin);
        rmin->bf = node->bf;

        if (node->prev) node->prev->next = rmin;
        rmin->prev = node->prev;

        if (prev != node) prev->left = rmin->right;
        rmin->left = node->left;
        if (node->right != rmin) rmin->right = node->right;

        check_parent(parent, node, rmin);
    }
    delete node;

    // * Correct tree *
    for(int ii=1; ii<path.size(); ii++)  cout << " " << path[ii]->key;  cout << endl;
    //return;
    while (path.size() > 1)
    {
        node = path.back();  path.pop_back();
        parent = path.back();

        cout << " " << node->key << "{" << ((parent) ? parent->key : -1) << "}";
        cout << "    " << " Is left son? " << ((lson) ? "TAK" : "NIE");
        cout << endl;
        //*
        if (lson) {
            node->bf--;
            if (node->bf == -1) { return; }
            else if (node->bf == -2) {
                cout << " ROTACJA PRAWA " << endl;
                if (node->right->bf < 0)  { right_rot1(node, node->right, parent); }
                else if (node->right->bf > 0)  { right_rot2(node, node->right, node->right->left, parent); }
                else if (node->right->bf == 0)  {
                    right_rot1(node, node->right, parent, true);
                    return;
                }
                else assert(false);
            }
        }
        else {
            node->bf++;
            if (node->bf == 1) { return; }
            else if (node->bf == 2) {
                cout << " ROTACJA LEWA " << endl;
                if (node->left->bf > 0)  { left_rot1(node, node->left, parent); }
                else if (node->left->bf < 0)  { left_rot2(node, node->left, node->left->right, parent); }
                else if (node->left->bf == 0) {
                    left_rot1(node, node->left, parent, true);
                    return;
                }
                else assert(false);
                return;
            }
        }

        if (parent)  lson = (parent->left == node) ? true : false;
        //*/
    }
}


template <class T_TYPE>
struct Tnode<T_TYPE> *
AVL<T_TYPE>::get_min(TNODE * node)
{
    while (node->left)  node = node->left;
    return node;
}


template <class T_TYPE>
struct Tnode<T_TYPE> *
AVL<T_TYPE>::find(T_TYPE kkey)
{
    TNODE * node = root;
    while (node) {
        if (node->key == kkey) break;
        node = (kkey < node->key) ? node->left : node->right;
    }
    return node;
}


template <class T_TYPE>
void
AVL<T_TYPE>::insert(T_TYPE kkey)
{
    TNODE * nnode = alloc_new_node(kkey);
    if (root == NULL) { root = nnode; return; }

    // * Determine path to leaf *
    vector<TNODE *> path;
    path.push_back(NULL);
    TNODE * node = root;
    while (node) {
        path.push_back(node);
        node = (nnode->key < node->key) ? node->left : node->right;
    }

    // * Add new node *
    node = path.back();
    if (nnode->key < node->key)
    {
        node->left = nnode;

        nnode->prev = node->prev;
        nnode->next = node;
        if (node->prev) node->prev->next = nnode;
        node->prev = nnode;
    }
    else
    {
        node->right = nnode;

        nnode->prev = node;
        nnode->next = node->next;
        if (node->next) node->next->prev = nnode;
        node->next = nnode;
    }
    TNODE * child = nnode;

    // * Correct tree *
    while (path.size() > 1)
    {
        node = path.back();  path.pop_back();
        TNODE * parent = path.back();

        //cout << " " << node->key << "{" << ((parent) ? parent->key : -1) << "}";  cout << endl;
        if (node->left == child) {
            node->bf++;
            if (node->bf > 1) {
                //cout << " ROTACJA LEWA " << endl;
                if (node->left->bf > 0)  { left_rot1(node, node->left, parent); }
                else if (node->left->bf < 0)  { left_rot2(node, node->left, node->left->right, parent); }
                else assert(false);
                return;
            }
            else if (node->bf == 0) return;
        }
        else {
            node->bf--;
            if (node->bf < -1) {
                //cout << " ROTACJA PRAWA " << endl;
                if (node->right->bf < 0)  { right_rot1(node, node->right, parent); }
                else if (node->right->bf > 0)  { right_rot2(node, node->right, node->right->left, parent); }
                else assert(false);
                return;
            }
            else if (node->bf == 0) return;
        }
        child = node;
    }
}


template <class T_TYPE>
inline void
AVL<T_TYPE>::check_parent(TNODE * parent, TNODE * y, TNODE * node)
{
    if (parent)
    {
        if (parent->left == y) parent->left = node;
        else parent->right = node;
    }
    else root = node;
}


template <class T_TYPE>
inline void
AVL<T_TYPE>::left_rot1(TNODE * y, TNODE * z, TNODE * parent, bool zero)
{
    if (zero)  z->bf = - (y->bf = 1);
    else  y->bf = z->bf = 0;
    y->left = z->right;
    z->right = y;

    check_parent(parent, y, z);
}


template <class T_TYPE>
inline void
AVL<T_TYPE>::left_rot2(TNODE * y, TNODE * z, TNODE * t, TNODE * parent)
{
    if (t->bf < 0) {
        z->bf = 1;
        y->bf = t->bf = 0;
    }
    else if (t->bf > 0) {
        y->bf = -1;
        z->bf = t->bf = 0;
    }
    else
        y->bf = z->bf = t->bf = 0;
    y->left = t->right;
    z->right = t->left;
    t->left = z;
    t->right = y;

    check_parent(parent, y, t);
}


template <class T_TYPE>
inline void
AVL<T_TYPE>::right_rot1(TNODE * y, TNODE * z, TNODE * parent, bool zero)
{
    if (zero)  y->bf = - (z->bf = 1);
    else  y->bf = z->bf = 0;
    y->right = z->left;
    z->left = y;

    check_parent(parent, y, z);
}


template <class T_TYPE>
inline void
AVL<T_TYPE>::right_rot2(TNODE * y, TNODE * z, TNODE * t, TNODE * parent)
{
    if (t->bf > 0) {
        z->bf = -1;
        y->bf = t->bf = 0;
    }
    else if (t->bf > 0) {
        y->bf = 1;
        z->bf = t->bf = 0;
    }
    else
        y->bf = z->bf = t->bf = 0;
    y->right = t->left;
    z->left = t->right;
    t->right = z;
    t->left = y;

    check_parent(parent, y, t);
}


template <class T_TYPE>
void
AVL<T_TYPE>::print (TNODE * node, int tabs)
{
    if (node == root) cout << "[ AVL ] : " << endl;
    if (node->right) print(node->right, tabs+1);
    for (int i = 0; i < tabs; i++) cout << "\t.";
    print_node(node);
    if (node->left) print(node->left, tabs+1);
    if (node == root) cout << endl;
}


template <class T_TYPE>
void
AVL<T_TYPE>::print_node (TNODE * node)
{
    if (node) {
        cout << " " << node->key;
        cout << "{" << node->bf << "}";
        cout << "[" << (node->prev ? node->prev->key : -1) << ",";
        cout << (node->next ? node->next->key : -1) << "]";
    }
    else  cout << " (NULL)";
    cout << endl;
}


template <class T_TYPE>
struct Tnode<T_TYPE> *
AVL<T_TYPE>::alloc_new_node (T_TYPE kkey)
{
    TNODE * nnode = new TNODE;
    nnode->key = kkey;
    nnode->left = NULL;
    nnode->right = NULL;
    nnode->next = NULL;
    nnode->prev = NULL;
    nnode->bf = 0;
    return nnode;
};


template <class T_TYPE>
AVL<T_TYPE>::AVL ()
{
    root = NULL;
	std::cout << "[ AVL ]  created" << std::endl;
};


template <class T_TYPE>
AVL<T_TYPE>::~AVL ()
{
	std::cout << "[ AVL ]  destroyed" << std::endl;
};


#endif // AVL_HPP_INCLUDED
