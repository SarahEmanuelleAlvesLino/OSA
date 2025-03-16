#include "Arquivo.h"
#include <iostream>
using namespace std;

int main() {
    string csvFile = "Nomes_Idades_1.csv";
    string binFile = "pessoas.bin";

    vector<Registro> registrosCSV = Arquivo::lerRegistrosCSV(csvFile);
    Arquivo::escreverRegistrosBinario(binFile, registrosCSV);

    vector<Registro> registrosBinario = Arquivo::lerRegistrosBinario(binFile);

    cout << "Registros lidos do arquivo binário:\n";
    for (const Registro& reg : registrosBinario) {
        cout << "Nome: " << reg.nome << ", Idade: " << reg.idade << endl;
    }

    return 0;
}

