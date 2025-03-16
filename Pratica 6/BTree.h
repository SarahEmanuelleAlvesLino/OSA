#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

// Define o tipo para os pares (chave, endereço)
using KeyValue = pair<int, string>;

class BTreeNode {
public:
    int m;                // mínimo de chaves em nós não‑raiz (m = ordem - 1)
    bool leaf;            // indica se é nó folha
    vector<KeyValue> keys;     // vetor de pares (chave, endereço)
    vector<BTreeNode*> children; // vetor de ponteiros para os filhos
    BTreeNode* parent;    // ponteiro para o nó pai

    // Construtor
    BTreeNode(int m, bool leaf = false, BTreeNode* parent = nullptr);

    // Métodos da árvore
    pair<BTreeNode*, int> search(int key);
    int find_key_index(int key);
    BTreeNode* insert_nonfull(int key, const string& address);
    void split_child(int i);

    void remove(int key);
    void remove_from_leaf(int idx);
    void remove_from_nonleaf(int idx);
    KeyValue get_predecessor(int idx);
    KeyValue get_successor(int idx);
    void fill(int idx);
    void borrow_from_prev(int idx);
    void borrow_from_next(int idx);
    void merge(int idx);
    void rotate_internal_left();
    void fix_deficiency_upwards();
    void print_node(int level = 0);
};

class BTree {
public:
    int m;            // mínimo de chaves em nós não‑raiz (m = ordem - 1)
    BTreeNode* root;  // ponteiro para a raiz da árvore

    // Construtor: 'ordem' é o valor informado (por exemplo, 3). Internamente m = ordem - 1.
    BTree(int ordem);
    pair<BTreeNode*, int> search(int key);
    void insert(int key, const string& address);
    void remove(int key);
    void print_tree();
};

#endif // BTREE_H
