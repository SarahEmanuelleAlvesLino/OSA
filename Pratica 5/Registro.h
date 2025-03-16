#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>
#include <sstream>
#include <iostream>
#include <cstring>

using namespace std;

class Registro {
public:
    int id;
    string titulo;
    string autor;
    int publicacao;
    string genero;

    static const size_t tam_titulo = 100;
    static const size_t tam_autor = 100;
    static const size_t tam_genero = 50;

    Registro();
    Registro(int id, const string& titulo, const string& autor, int publicacao, const string& genero);
    
    string packDelimited() const;
    string packDelimitedbinary() const;
    string packDescritor() const;

    void unpackDelimited(const string& buffer);
    void unpackDelimitedBinary(const string& buffer);
    void unpackDescritor(const string& buffer);
};

#endif
