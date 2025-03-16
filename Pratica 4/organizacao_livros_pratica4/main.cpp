#include "Arvore.h"
#include "Buffer.h"
#include "Index.h"
#include "Registro.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int conta_aspas(const string& str);
bool le_arq(stringstream& ss, string& arq);

int main() {
    string arquivoCSV = "booksDataset.csv";
    string arquivoBinario = "dados_binario.bin";
    string arquivoIndex = "Index.txt";
    vector<Registro> registros;

    // Lê os registros do arquivo CSV e armazena em um vetor
    ifstream arquivoEntrada(arquivoCSV);
    if (!arquivoEntrada) {
        cerr << "ERRO: Não foi possível abrir o arquivo CSV para leitura.\n";
        return 1;
    }

    string linha;
    getline(arquivoEntrada, linha);
    while (getline(arquivoEntrada, linha)) {
        stringstream ss(linha);
        string id_str, titulo, autor, publicacao_str, genero;

        getline(ss, id_str, ';');
        if (!le_arq(ss, titulo)) {
            cerr << "ERRO: Não foi possível ler o título" << endl;
            continue;
        }
        getline(ss, autor, ';');
        getline(ss, publicacao_str, ';');
        getline(ss, genero);

        int id = stoi(id_str);
        int publicacao = stoi(publicacao_str);
        
        Registro reg(id, titulo, autor, publicacao, genero);
        registros.push_back(reg);
    }
    arquivoEntrada.close();
    

    // Salva os registros em binário com descritor de tamanho (dados_binario.bin)
    Buffer buffer(arquivoBinario, arquivoIndex);
    buffer.escreverDescritorVetor(registros);

    // Realiza as buscas por ID no arquivo
    try {
        cout << "----------------------------------------------------" << endl;
        cout << "-----------------ÍNDICES BUSCADOS-------------------" << endl;
        cout << "----------------------------------------------------\n" << endl;
        buffer.buscarRegistro(48134);
        buffer.buscarRegistro(71396);
        buffer.buscarRegistro(83615);
    } catch (const runtime_error& e) { 
        cerr << e.what() << endl;
    }

    // Adiciona novos registros
    vector<Registro> novosRegistros = {
        Registro(89675312, "A Journey Through Time", "Smith John", 2001, "Travel"),
        Registro(456852, "Exploring the Universe: A Beginner's Guide", "B.V. Alice", 2003, "Science"),
        Registro(951123, "The Art of Italian Cooking", "Rossi Marco", 1955, "Cooking"),
        Registro(8527416, "Digital Futures: Understanding AI", "Davis Claire", 2020, "Technology"),
        Registro(9876321, "Poetry of the Soul", "Anderson, Laura", 1985, "Poetry")
    };

    buffer.adicionarRegistros(novosRegistros);

    try {
        cout << "-----------------------------------------------------" << endl;
        cout << "-----------------LIVROS ADICIONADOS------------------" << endl;
        cout << "-----------------------------------------------------\n" << endl;
        for(auto reg : novosRegistros) {
            buffer.buscarRegistro(reg.id);
        }
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
    }

    cout << endl << "Finalizada a leitura e gravação!!!\n";
    
    return 0;
}
 

// Conta N° de aspas na string
int conta_aspas(const string& str) {
    return count(str.begin(), str.end(), '"');
}

// Lê um campo do arquivo e garante que as aspas sejam tratadas corretamente
bool le_arq(stringstream& ss, string& arq) {
    getline(ss, arq, ';');
    
    // Enquanto houver aspas não fechadas, continua lendo até encontrá-las
    while (conta_aspas(arq) % 2 != 0) {
        string aux;
        getline(ss, aux, ';');
        arq += ";" + aux;
    }

    return true;
}