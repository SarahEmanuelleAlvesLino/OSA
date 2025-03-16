#include "Buffer.h"
#include <iostream>
using namespace std;

Buffer::Buffer(const string& arqNome, bool binario) : Bin(binario) {
    ios::openmode mode = ios::in | ios::out | ios::app | (binario ? ios::binary : ios::openmode(0));
    stream.open(arqNome, mode);
    if (!stream) cerr << "Erro ao abrir o arquivo.\n";
}

Buffer::~Buffer() {
    if (stream.is_open()) stream.close();
}

void Buffer::escreveRegistro(const Registro& reg, bool useDelimitador) {
    string data = useDelimitador ? reg.calocarDelimitador() : reg.colocaDescritor();
    if (stream) {
        stream.write(data.c_str(), data.size());
    } else {
        cerr << "Erro ao escrever no arquivo.\n";
    }
}

Registro Buffer::readRegistro(bool useDelimitador) {
    Registro reg;
    if (!stream) return reg;

    if (useDelimitador) {
        string line;
        if (getline(stream, line)) reg.desfazDelimitador(line);
    } else {
        int tamanho;
        if (stream.read(reinterpret_cast<char*>(&tamanho), sizeof(int))) {
            string data(tamanho, '\0');
            if (stream.read(&data[0], tamanho)) reg.desfazDescritor(data);
        }
    }
    return reg;
}

vector<Registro> Buffer::carregaRegistroTexto(const string& arqNome) {
    ifstream inputFile(arqNome);
    vector<Registro> registros;

    if (!inputFile) {
        cerr << "Erro ao abrir o arquivo de entrada 'dados.txt'.\n";
        return registros;
    }

    string line;
    while (getline(inputFile, line)) {
        Registro reg;
        reg.desfazDelimitador(line);
        registros.push_back(reg);
    }

    return registros;
}


