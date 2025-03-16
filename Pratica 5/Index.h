#ifndef INDEX_H
#define INDEX_H

#include <string>
#include <sstream>
#include <cstddef>
#include <stdint.h>

using namespace std;

class Index{
    public:
        int id;
        streampos endereco;
        string nomeArquivo;
        
        Index(const string& nomeArquivo); 
        Index(int id, streampos endereco,  const string& nomeArquivo);
        Index();

};
#endif