#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>
using namespace std;

class Registro {
public:
    string nome;
    string sobrenome;
    string telefone;
    string data_nascimento;

    Registro() : nome(""), sobrenome(""), telefone(""), data_nascimento("") {}
    Registro(const string& n, const string& sn, const string& tel, const string& data)
        : nome(n), sobrenome(sn), telefone(tel), data_nascimento(data) {}

    string calocarDelimitador() const; // Para representação com delimitadores
    void desfazDelimitador(const string& data);

    string colocaDescritor() const; // Para representação com descritor de tamanho
    void desfazDescritor(const string& data);
};

#endif
