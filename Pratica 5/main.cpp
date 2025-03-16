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
   // string arquivoIndex = "Index.txt";
    string arquivoIndiceInvert = "IndexInvert.bin";
    string arquivosStopwords = "Stopwords.txt";
    string arquivoPontuacao = "Pontuacao.txt";
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
    Buffer buffer(arquivoBinario, arquivoIndiceInvert);
    buffer.escreverDescritorVetor(registros);
    buffer.criarIndiceInvertido(registros, arquivosStopwords, arquivoPontuacao, arquivoIndiceInvert);


    cout << "Qual Livro deseja achar? Escreva o titulo ou palavra chave: ";
    string frase;
    getline(cin, frase);


    vector<int> ids = buffer.buscarFrase(frase, arquivoIndiceInvert, arquivosStopwords, arquivoPontuacao);
    if (!ids.empty()) {
       cout << endl;
       for (int id : ids) {
           buffer.buscarRegistro(id);
       }
    cout << "\nQuantidade de livros achados: " << ids.size() << endl;
    } else {
       cout << "Nenhum livro encontrado!" << endl;
       cout << "\nTotal de livros encontrados: " << ids.size() << endl;
    }


    cout << endl << "Achados com Sucesso =) !\n";

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


 
