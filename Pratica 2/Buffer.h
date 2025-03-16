#ifndef BUFFER_H
#define BUFFER_H

#include "Registro.h"
#include <fstream>
using namespace std;

class Buffer {
    fstream& stream;

public:
    Buffer(fstream& s) : stream(s) {}

    void writeRegistro(const Registro& reg);
    Registro readRegistro();
};

#endif
