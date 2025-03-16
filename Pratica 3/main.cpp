#include "Buffer.h"
#include <iostream>
using namespace std;

int main() {
    string ArmazenaNome = "Dados.txt";
    string ArmazenaComDelimitador = "registros_delimitador.txt";
    string ArmazenaComDescritor = "registros_descritor.bin";

    vector<Registro> registros = Buffer::carregaRegistroTexto(ArmazenaNome);

    Buffer GravaREGDelimitador(ArmazenaComDelimitador);
    for (const Registro& reg : registros) {
        GravaREGDelimitador.escreveRegistro(reg, true);
    }

    Buffer GravaRegDescritor(ArmazenaComDescritor, true);
    for (const Registro& reg : registros) {
        GravaRegDescritor.escreveRegistro(reg, false);
    }

    return 0;
}
