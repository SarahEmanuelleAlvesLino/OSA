#include "Registro.h"
#include <cstring>
using namespace std;

string Registro::serialize() const {
    string data(30, '\0');               // Cria uma string de 30 bytes com '\0'
    strncpy(&data[0], nome.c_str(), 20); 
    memcpy(&data[20], &idade, sizeof(int));  
    return data;
}

void Registro::deserialize(const string& data) {
    nome = string(data.c_str(), strnlen(data.c_str(), 20));
    memcpy(&idade, &data[20], sizeof(int)); 
}
