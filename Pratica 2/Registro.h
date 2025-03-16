#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>
using namespace std;

class Registro {
public:
    string nome;
    int idade;

    Registro() : nome(""), idade(0) {}
    Registro(const string& n, int i) : nome(n), idade(i) {}

    string serialize() const;
    void deserialize(const string& data);
};

#endif
