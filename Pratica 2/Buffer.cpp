#include "Buffer.h"
#include <iostream>
using namespace std;

void Buffer::writeRegistro(const Registro& reg) {
    string data = reg.serialize();  
    stream.write(data.c_str(), 30);  
    stream.put('\n');                // Adiciona uma nova linha para separar registros
}

Registro Buffer::readRegistro() {
    string data(30, '\0');           
    if (stream.read(&data[0], 30)) { 
        if (stream.get() != '\n') {  
            cerr << "Erro: formato de registro incorreto." << endl;
        }
        Registro reg;
        reg.deserialize(data);       
        return reg;
    }
    return Registro();  // Retorna um registro vazio se a leitura falhar
}
