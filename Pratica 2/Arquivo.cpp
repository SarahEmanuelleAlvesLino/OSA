#include "Arquivo.h"
#include <fstream>
#include <sstream>
using namespace std;

vector<Registro> Arquivo::lerRegistrosCSV(const string& nomeArquivo) {
    vector<Registro> registros;
    ifstream file(nomeArquivo);
    string line;

    while (getline(file, line)) {
        istringstream ss(line);
        string nome;
        int idade;
        if (getline(ss, nome, ',') && ss >> idade) {
            registros.emplace_back(nome, idade);
        }
    }
    return registros;
}

void Arquivo::escreverRegistrosBinario(const string& nomeArquivo, const vector<Registro>& registros) {
    fstream file(nomeArquivo, ios::out | ios::binary);
    Buffer buffer(file);

    for (const Registro& reg : registros) {
        buffer.writeRegistro(reg);  // Escreve cada registro em bloco de 30 bytes
    }
}

vector<Registro> Arquivo::lerRegistrosBinario(const string& nomeArquivo) {
    vector<Registro> registros;
    fstream file(nomeArquivo, ios::in | ios::binary);
    Buffer buffer(file);

    while (file.peek() != EOF) {
        registros.push_back(buffer.readRegistro());  // Lê cada bloco de 30 bytes como um registro
    }
    return registros;
}

