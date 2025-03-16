#include "Registro.h"
#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>

Registro::Registro() : id(0), titulo(""), autor(""), publicacao(0), genero("") {}

Registro::Registro(int id, const string& titulo, const string& autor, int publicacao, const string& categoria)
    : id(id), titulo(titulo), autor(autor), publicacao(publicacao), genero(categoria) {}

// Empacota os dados 
string Registro::packDescritor() const {
    string linha = to_string(id) + "|" + titulo + "|" + autor + "|" + to_string(publicacao) + "|" + genero + "\n";

    int tamanho = linha.size();
    string buffer(sizeof(int), '\0');
    memcpy(&buffer[0], &tamanho, sizeof(int));
    buffer += linha;
    return buffer;
}

void Registro::unpackDelimited(const string& buffer) {
    stringstream stream(buffer);
    string id_str;
    getline(stream, id_str, ';');
    id = stoi(id_str);
    getline(stream, autor, ';');
    string publicacao_str;
    getline(stream, publicacao_str, ';');
    publicacao = stoi(publicacao_str);
    getline(stream, genero, '\n');
}

void Registro::unpackDelimitedBinary(const string& buffer) {
    stringstream stream(buffer);
    string id_str;
    getline(stream, id_str, '|');
    id = stoi(id_str);
    getline(stream, titulo, '|');
    getline(stream, autor, '|');
    string publicacao_str;
    getline(stream, publicacao_str, '|');
    publicacao = stoi(publicacao_str);
    getline(stream, genero, '\n');
}

// Desempacota os dados 
void Registro::unpackDescritor(const string& buffer) {
    if (buffer.size() < sizeof(int)) {
        throw runtime_error("Buffer size is smaller than expected.");
    }

    int tamanho;
    memcpy(&tamanho, buffer.data(), sizeof(int));

    if (buffer.size() < sizeof(int) + tamanho) {
        cerr << "Buffer size: " << buffer.size() << ", Expected size: " << sizeof(int) + tamanho << endl;
        throw runtime_error("Buffer size is smaller than expected.");
    }

    unpackDelimitedBinary(buffer.substr(sizeof(int), tamanho));
}