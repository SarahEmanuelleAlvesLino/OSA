#include "Index.h"
#include <cstring>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

Index::Index(const string& nomeArquivo) 
    : nomeArquivo(nomeArquivo) {}

Index::Index(int id, streampos endereco,  const string& nomeArquivo)
    : id(id), endereco(endereco), nomeArquivo(nomeArquivo) {}

Index::Index() {}

void Index::escreveIndex() {
    // Abre o arquivo
    ofstream arquivoIndex(nomeArquivo, ios:: app);
    if (!arquivoIndex.is_open()) {
        cerr << "ERRO: Não foi possível abrir o arquivo de índice: " << nomeArquivo << endl;
        throw runtime_error("ERRO: Não foi possível abrir o arquivo de índice para escrita.");
    }

    arquivoIndex << id << "|" << endereco << "\n";
}

// Comparação de '>'
bool Index::operator>(const Index& other) const {
    return id > other.id; 
}

// Comparação '<'
bool Index::operator<(const Index& other) const {
    return id < other.id; 
}

// Comparação '=='
bool Index::operator==(const Index& other) const {
    return id == other.id;
}

