#include "BTree.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

int main() {
    //Arquivos para salvar a árvore e os endereços no formato binário e txt, respectivamente
    string binFilename = "btree.bin";
    string txtFilename = "btree.txt"; 
    
    // Define a ordem da árvore
    BTree btree(2); // Ordem 2, mas pode ser alterado para arvore de ordem 1,3,...,n

    // Vetor de elementos a serem inseridos na árvore
    vector<int> elementos = {20, 40, 10, 30, 15, 35, 7, 26, 18, 22, 5, 42, 13, 46, 27, 8, 32, 38, 24, 45, 25};

     cout << "\n----------------------------------------------\n";
     cout << "        Inserção na Árvore B         \n";
     cout << "------------------------------------------------\n" << endl;

    //OBSERVAÇÂO: A forma de visualização é em indentação onde o nó mais a esquerda é a raiz e os pais das folhas que ficam mais a direita de cada elemento
     
    for (int chave : elementos) {
        cout << "- Insere  " << chave << " " << endl;
        btree.insert(chave, "End_" + to_string(chave));
        btree.print_tree();
        cout << "_____________________________________________" << endl;
    }
    
    
    btree.saveToFile(binFilename); //Salva a árvore em arquivo
    btree.convertBinToTxt(binFilename, txtFilename); //Converte o arquivo binário para txt
    
    cout << "Árvore salva em " << binFilename << " e " << txtFilename << endl;

    // Vetor de elementos a serem removidos da árvore
    vector<int> elementos2 = {25, 45, 24}; 

     cout << "\n----------------------------------------------\n";
     cout << "         Remoção na Árvore B         \n";
     cout << "-----------------------------------------------\n" << endl;
    
    // Remoção na mesma ordem das inserções
    for (int chave : elementos2) {
        cout << " - Remove  " << chave << "..." << endl;
        btree.remove(chave);
        btree.print_tree();
        cout << "_____________________________________________" << endl;
    }
    
    // Vetor de de elementos a serem buscados na árvore. OBS.: A busca é feita no arquivo binário
    vector<int> buscas = {3, 69, 15, 2};
   
    cout << "\n----------------------------------------------\n";
    cout << "          Busca na Árvore B          \n";
    cout << "------------------------------------------------\n" << endl;
    
    for (int chave : buscas) {
        auto resultado = btree.searchInFile(binFilename, chave);
        if(resultado.first) {
            cout << "Chave " << dec << chave << " encontrada no nó com endereço: 0x"
                << hex << resultado.second << dec << endl;
        } else {
            cout << " - Chave " << chave << " Não encontrada!" << endl;
        }
        cout << "____________________________________________" << endl;
    }

    return 0;
}
