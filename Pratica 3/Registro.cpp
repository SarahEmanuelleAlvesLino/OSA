#include "Registro.h"
#include <sstream>
#include <cstring>
using namespace std;

// Coloca o registro usando delimitadores '|' e '\n'
string Registro::calocarDelimitador() const {
    return nome + "|" + sobrenome + "|" + telefone + "|" + data_nascimento + "\n";
}

// Desfaz o registro de uma string com delimitadores
void Registro::desfazDelimitador(const string& data) {
    istringstream stream(data);
    getline(stream, nome, '|');
    getline(stream, sobrenome, '|');
    getline(stream, telefone, '|');
    getline(stream, data_nascimento, '\n');
}

// Coloca o registro com um descritor de tamanho (binário)
string Registro::colocaDescritor() const {
    stringstream ss;
    string record = nome + "|" + sobrenome + "|" + telefone + "|" + data_nascimento;
    int tamanho = record.size();
    ss.write(reinterpret_cast<const char*>(&tamanho), sizeof(int));
    ss << record;
    return ss.str();
}

// Desfaz o registro com descritor de tamanho
void Registro::desfazDescritor(const string& data) {
    istringstream ss(data);
    int tamanho;
    ss.read(reinterpret_cast<char*>(&tamanho), sizeof(int));
    string record(tamanho, '\0');
    ss.read(&record[0], tamanho);

    istringstream recordStream(record);
    getline(recordStream, nome, '|');
    getline(recordStream, sobrenome, '|');
    getline(recordStream, telefone, '|');
    getline(recordStream, data_nascimento, '\n');
}
