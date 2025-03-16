#ifndef BUFFER_H
#define BUFFER_H

#include "Registro.h"
#include "Arvore.h"
#include <fstream>
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Node
{
    public:
		int id;
        streampos endereco;
        
        bool operator>(const Node& other) const {
            return id > other.id; 
        }

        // Comparação '<'
        bool operator<(const Node& other) const {
            return id < other.id; 
        }

        // Comparação '=='
        bool operator==(const Node& other) const {
            return id == other.id;
        }
};

class Buffer {
public:
    string nomeArquivo;
    string nomeArquivoIndex;
    ArvoreBinaria<Node> arvoreIndices; 
    
    Buffer(const string& nomeArquivo, const string& nomeArquivoIndex);
    Buffer() : nomeArquivo(""), nomeArquivoIndex("") {} 

    void escreverDescritorVetor(const vector<Registro>& registros);
    Registro lerDescritor();
    int buscarEnd(int id);
    void buscarRegistro(int id);
    void adicionarRegistros(const vector<Registro>& registros);
    void criarIndiceInvertido(const vector<Registro>& registros, const string& arquivosStopwords, const string& arquivoPontuacao, const string& arquivoIndiceInvert);
    vector<int> buscarFrase(const string& frase, const string& arquivoIndiceInvert, const string& arquivosStopwords, const string& arquivosPontuacao);


private:
   unordered_map<string, vector<int>> indiceInvertido;
};


#endif
