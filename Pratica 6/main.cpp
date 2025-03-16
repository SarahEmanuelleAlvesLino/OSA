#include "BTree.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

int main() {
    BTree btree(2); 
    // Podemos escolher o btree(2) como o btree(4) também

    // Vetor de elementos a serem inseridos na árvore
    vector<int> elementos = {20, 40, 10, 30, 15, 35, 7, 26, 18, 22, 5, 42, 13, 46, 27, 8, 32, 38, 24, 45, 25};

     cout << "\n----------------------------------------------\n";
     cout << "        Inseri na Árvore B         \n";
     cout << "------------------------------------------------\n" << endl;

    for (int chave : elementos) {
        cout << "- Insere  " << chave << " " << endl;
        btree.insert(chave, "End_" + to_string(chave));
        btree.print_tree();
        cout << "_____________________________________________" << endl;
    }
    //A forma de visualização é em indentação onde os nó mais a esquerda é a raiz e os pais das folhas que ficam mais a direita de cada elemento

    // Vetor de chaves para busca
    vector<int> buscas = {3, 69, 15, 2};
    // Cabeçalho para a busca
    cout << "\n----------------------------------------------\n";
    cout << "          Busca da Árvore B          \n";
    cout << "------------------------------------------------\n" << endl;
    for (int chave : buscas) {
        auto resultado = btree.search(chave);
        if(resultado.first != nullptr) {
            cout << "- Chave " << chave << " Foi encontrada!" << endl;
        } else {
            cout << " - Chave " << chave << " Não encontrada!" << endl;
        }
        cout << "____________________________________________" << endl;
    }

    // Vetor de elementos a serem removidos da árvore
    vector<int> elementos2 = {25, 45, 24};  
     cout << "\n----------------------------------------------\n";
     cout << "         Removendo da Árvore B         \n";
     cout << "-----------------------------------------------\n" << endl;
    // Removendo na mesma ordem das inserções
    for (int chave : elementos2) {
        cout << " - Remove  " << chave << "..." << endl;
        btree.remove(chave);
        btree.print_tree();
        cout << "_____________________________________________" << endl;
    }

    return 0;
}
