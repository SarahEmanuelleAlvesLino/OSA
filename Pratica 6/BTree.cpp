#include "BTree.h"

using namespace std;

BTreeNode::BTreeNode(int m, bool leaf, BTreeNode* parent)
    : m(m), leaf(leaf), parent(parent) { }

pair<BTreeNode*, int> BTreeNode::search(int key) {
    int i = 0;
    while (i < static_cast<int>(keys.size()) && key > keys[i].first)
        i++;
    if (i < static_cast<int>(keys.size()) && keys[i].first == key)
        return {this, i};
    if (leaf)
        return {nullptr, -1};
    return children[i]->search(key);
}

int BTreeNode::find_key_index(int key) {
    int idx = 0;
    while (idx < static_cast<int>(keys.size()) && keys[idx].first < key)
        idx++;
    return idx;
}

BTreeNode* BTreeNode::insert_nonfull(int key, const string& address) {
    if (leaf) {
        int i = keys.size() - 1;
        keys.push_back({0, ""}); // aloca espaço para a nova chave
        while (i >= 0 && key < keys[i].first) {
            keys[i+1] = keys[i];
            i--;
        }
        keys[i+1] = {key, address};
        if (static_cast<int>(keys.size()) > 2 * m) {
            if (parent != nullptr) {
                int idx = 0;
                for (int j = 0; j < static_cast<int>(parent->children.size()); j++) {
                    if (parent->children[j] == this) {
                        idx = j;
                        break;
                    }
                }
                parent->split_child(idx);
            } else {
                // Divisão da raiz
                BTreeNode* new_root = new BTreeNode(m, false);
                new_root->children.push_back(this);
                parent = new_root;
                new_root->split_child(0);
                return new_root;
            }
        }
        return nullptr;
    } else {
        int i = keys.size() - 1;
        while (i >= 0 && key < keys[i].first)
            i--;
        i++;
        if (static_cast<int>(children[i]->keys.size()) > 2 * m) {
            split_child(i);
            if (key > keys[i].first)
                i++;
        }
        BTreeNode* ret = children[i]->insert_nonfull(key, address);
        if (ret != nullptr)
            return ret;
        if (static_cast<int>(keys.size()) > 2 * m) {
            if (parent != nullptr) {
                int idx = 0;
                for (int j = 0; j < static_cast<int>(parent->children.size()); j++) {
                    if (parent->children[j] == this) {
                        idx = j;
                        break;
                    }
                }
                parent->split_child(idx);
            } else {
                BTreeNode* new_root = new BTreeNode(m, false);
                new_root->children.push_back(this);
                parent = new_root;
                new_root->split_child(0);
                return new_root;
            }
        }
        return nullptr;
    }
}

void BTreeNode::split_child(int i) {
    BTreeNode* y = children[i];                      // nó a ser dividido
    BTreeNode* z = new BTreeNode(m, y->leaf, this);    // novo nó que receberá as chaves finais
    KeyValue mid_key = y->keys[m];                   // chave mediana a ser promovida

    // z recebe as chaves de índice m+1 até o fim
    z->keys.assign(y->keys.begin() + m + 1, y->keys.end());
    // Se não for folha, transfere também os filhos
    if (!y->leaf) {
        z->children.assign(y->children.begin() + m + 1, y->children.end());
        for (auto child : z->children)
            child->parent = z;
        y->children.resize(m + 1);
    }
    // y mantém as chaves de índice 0 até m-1
    y->keys.resize(m);

    // Insere z como filho deste nó, logo após y, e promove a chave mediana
    children.insert(children.begin() + i + 1, z);
    keys.insert(keys.begin() + i, mid_key);
}

void BTreeNode::remove(int key) {
    int idx = find_key_index(key);
    if (idx < static_cast<int>(keys.size()) && keys[idx].first == key) {
        if (leaf) {
            remove_from_leaf(idx);
            if (parent != nullptr && static_cast<int>(keys.size()) < m) {
                int idx_p = 0;
                for (int j = 0; j < static_cast<int>(parent->children.size()); j++) {
                    if (parent->children[j] == this) {
                        idx_p = j;
                        break;
                    }
                }
                parent->fill(idx_p);
                parent->fix_deficiency_upwards();
            }
        } else {
            remove_from_nonleaf(idx);
        }
    } else {
        if (leaf)
            return; // chave não encontrada
        if (children[idx]->search(key).first == nullptr && static_cast<int>(children[idx]->keys.size()) == m)
            fill(idx);
        children[idx]->remove(key);
    }
}

void BTreeNode::remove_from_leaf(int idx) {
    keys.erase(keys.begin() + idx);
}

void BTreeNode::remove_from_nonleaf(int idx) {
    int key_val = keys[idx].first;
    if (static_cast<int>(children[idx]->keys.size()) >= m) {
        KeyValue pred = get_predecessor(idx);
        keys[idx] = pred;
        children[idx]->remove(pred.first);
    } else if (static_cast<int>(children[idx+1]->keys.size()) >= m) {
        KeyValue succ = get_successor(idx);
        keys[idx] = succ;
        children[idx+1]->remove(succ.first);
    } else {
        merge(idx);
        children[idx]->remove(key_val);
    }
}

KeyValue BTreeNode::get_predecessor(int idx) {
    BTreeNode* cur = children[idx];
    while (!cur->leaf)
        cur = cur->children.back();
    return cur->keys.back();
}

KeyValue BTreeNode::get_successor(int idx) {
    BTreeNode* cur = children[idx+1];
    while (!cur->leaf)
        cur = cur->children.front();
    return cur->keys.front();
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && static_cast<int>(children[idx-1]->keys.size()) > m)
        borrow_from_prev(idx);
    else if (idx != static_cast<int>(keys.size()) && static_cast<int>(children[idx+1]->keys.size()) > m)
        borrow_from_next(idx);
    else {
        if (idx != 0)
            merge(idx-1);
        else
            merge(idx);
    }
}

void BTreeNode::borrow_from_prev(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx-1];
    child->keys.insert(child->keys.begin(), keys[idx-1]);
    if (!child->leaf) {
        child->children.insert(child->children.begin(), sibling->children.back());
        child->children.front()->parent = child;
        sibling->children.pop_back();
    }
    keys[idx-1] = sibling->keys.back();
    sibling->keys.pop_back();
}

void BTreeNode::borrow_from_next(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx+1];
    child->keys.push_back(keys[idx]);
    if (!child->leaf) {
        child->children.push_back(sibling->children.front());
        child->children.back()->parent = child;
        sibling->children.erase(sibling->children.begin());
    }
    keys[idx] = sibling->keys.front();
    sibling->keys.erase(sibling->keys.begin());
}

void BTreeNode::merge(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx+1];
    child->keys.push_back(keys[idx]);
    child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());
    if (!child->leaf) {
        child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());
        for (auto c : sibling->children)
            c->parent = child;
    }
    keys.erase(keys.begin() + idx);
    children.erase(children.begin() + idx + 1);
    delete sibling;
}

void BTreeNode::rotate_internal_left() {
    if (children.size() < 2 || children[0]->keys.empty())
        return;
    BTreeNode* left_child = children[0];
    BTreeNode* right_child = children[1];
    KeyValue temp = left_child->keys.back();
    left_child->keys.pop_back();
    KeyValue old_key = keys[0];
    keys[0] = temp;
    right_child->keys.insert(right_child->keys.begin(), old_key);
    if (!left_child->leaf && !left_child->children.empty()) {
        BTreeNode* child_temp = left_child->children.back();
        left_child->children.pop_back();
        right_child->children.insert(right_child->children.begin(), child_temp);
        child_temp->parent = right_child;
    }
}

void BTreeNode::fix_deficiency_upwards() {
    BTreeNode* current = this;
    while (current->parent != nullptr && static_cast<int>(current->keys.size()) < current->m) {
        BTreeNode* parent_node = current->parent;
        int idx = 0;
        for (int j = 0; j < static_cast<int>(parent_node->children.size()); j++) {
            if (parent_node->children[j] == current) {
                idx = j;
                break;
            }
        }
        parent_node->fill(idx);
        current = parent_node;
    }
}

void BTreeNode::print_node(int level) {
    string indent(level * 4, ' ');
    cout << indent << "[";
    for (size_t i = 0; i < keys.size(); i++) {
        cout << keys[i].first;
        if (i < keys.size() - 1)
            cout << ", ";
    }
    cout << "]\n";
    if (!leaf) {
        for (auto child : children)
            child->print_node(level + 1);
    }
}

BTree::BTree(int ordem) : m(ordem) {
    root = new BTreeNode(m, true);
}

pair<BTreeNode*, int> BTree::search(int key) {
    if (root)
        return root->search(key);
    return {nullptr, -1};
}

void BTree::insert(int key, const string& address) {
    if (static_cast<int>(root->keys.size()) > 2 * m) {
        BTreeNode* s = new BTreeNode(m, false);
        s->children.push_back(root);
        root->parent = s;
        s->split_child(0);
        root = s;
    }
    BTreeNode* new_root = root->insert_nonfull(key, address);
    if (new_root != nullptr)
        root = new_root;
}

void BTree::remove(int key) {
    if (!root) {
        cout << "Árvore vazia.\n";
        return;
    }
    root->remove(key);
    if (root && root->keys.empty()) {
        if (!root->leaf) {
            BTreeNode* tmp = root->children[0];
            tmp->parent = nullptr;
            delete root;
            root = tmp;
        } else {
            delete root;
            root = nullptr;
        }
    }
}

void BTree::print_tree() {
    if (root)
        root->print_node();
    else
        cout << "A Árvore B está vazia.\n";
}