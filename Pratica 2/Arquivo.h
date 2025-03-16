#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "Registro.h"
#include "Buffer.h"
#include <vector>
#include <string>
using namespace std;

class Arquivo {
public:
    static vector<Registro> lerRegistrosCSV(const string& nomeArquivo);
    static void escreverRegistrosBinario(const string& nomeArquivo, const vector<Registro>& registros);
    static vector<Registro> lerRegistrosBinario(const string& nomeArquivo);
};

#endif
