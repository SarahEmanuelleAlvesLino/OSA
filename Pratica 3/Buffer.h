#ifndef BUFFER_H
#define BUFFER_H

#include "Registro.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Buffer {
    fstream stream;
    bool Bin;


public:
    Buffer(const string& arqNome, bool binario = false);
    ~Buffer();

    void escreveRegistro(const Registro& reg, bool useDelimitador);
    Registro readRegistro(bool useDelimitador);

    //carregar registros de dados.txt
    static vector<Registro> carregaRegistroTexto(const string& arqNome);
};

#endif
