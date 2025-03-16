#include "Buffer.h"
#include "Index.h"
#include "Arvore.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

Buffer::Buffer(const string& nomeArquivo, const string& nomeArquivoIndex) 
    : nomeArquivo(nomeArquivo), nomeArquivoIndex(nomeArquivoIndex) {}

void Buffer::escreverDescritorVetor(const vector<Registro>& registros) {
    ofstream arquivoDados(nomeArquivo, ios::trunc | ios::binary);
    if (!arquivoDados)
        throw runtime_error("ERRO: Não foi possível abrir o arquivo para escrita.");

    // Vetor de índices
    vector<Index> indices;

    for (const Registro& registro : registros) {
        // Empacota o registro dos livros
        string bufferRegistro = registro.packDescritor();

        // Antes de escrever salva a posição
        streampos salva_pos = arquivoDados.tellp();

        // Escreve o registro no arquivo de dados
        arquivoDados.write(bufferRegistro.data(), bufferRegistro.size());

        // Cria o objeto Index
        Index novoIndex(registro.id, salva_pos, nomeArquivoIndex);
        
        Node n = {registro.id, salva_pos};
        arvoreIndices.Inserir(n);
        
        // Adiciona o índice no vetor de registros
        indices.push_back(novoIndex);
    }
  
    arquivoDados.close();
    ofstream arquivoIndex(nomeArquivoIndex, ios::trunc);
    if (!arquivoIndex.is_open())
        throw runtime_error("ERRO: Não foi possível abrir o arquivo de índice.");

    // Escreve todos os índices no arquivo
    for (const Index& index : indices) {
        arquivoIndex << index.id << "|" << index.endereco << "\n";
    }
    arquivoIndex.close();
}

Registro Buffer::lerDescritor() {
    ifstream arquivoEntrada(nomeArquivo, ios::binary);
    if (!arquivoEntrada)
        throw runtime_error("ERRO: Não foi possível abrir o arquivo para leitura.");

    int tam; //Indica o tamanho
    arquivoEntrada.read(reinterpret_cast<char*>(&tam), sizeof(int));
    if (!arquivoEntrada)
        throw runtime_error("ERRO: Não foi possível ler o descritor de tamanho.");

    string buffer(tam, '\0');
    arquivoEntrada.read(&buffer[0], tam);
    if (!arquivoEntrada)
        throw runtime_error("ERRO: Não foi possível ler o registro com descritor.");

    Registro reg;
    reg.unpackDescritor(buffer);
    return reg;
}


int Buffer::buscarEnd(int id) {
    
    //É feita uma pesquisa na árvore binária para a busca do ID
    int endereco = arvoreIndices.Busca(id);
    
    // Verifica se o endereço foi encontrado
    if (endereco != 0) {
        cout << "ID = " << id << " | Endereço = " << endereco << endl;
        return endereco;
    } else {
        cout << "ID " << id << " não encontrado." << endl;
        throw runtime_error("Índice não encontrado.");
    }
    
}

void Buffer::buscarRegistro(int id){
    
    int encontrado = buscarEnd(id); // Procura o índice com ID = id

    ifstream file(nomeArquivo, ios::binary);
    file.seekg(static_cast<streampos>(encontrado));
    string buffer;
    char c;
    // Fica em um loop até ler e encontrar o delimitador final '\n'
    while (file.get(c)) {
        buffer += c;
        if (c == '\n') {
            break;
        }
    }
    if (buffer.empty()) {
        cerr << "ERRO: Não foi possível ler nenhum dado!" << endl;
        return;
    }
    Registro registro;
    registro.unpackDescritor(buffer);
    
    cout << " " << endl;
    cout << "ID: " << registro.id << endl;
    cout << "Título: " << registro.titulo << endl;
    cout << "Autor: " << registro.autor << endl;
    cout << "Ano de Publicação: " << registro.publicacao << endl;
    cout << "Gênero: " << registro.genero << endl;
    cout << " "<< endl;
    file.close();
}

void Buffer::adicionarRegistros(const vector<Registro>& registros) {
    ofstream arquivoSaida(nomeArquivo, ios::app | ios::binary);
    if (!arquivoSaida)
        throw runtime_error("ERRO: Não foi possível abrir o arquivo para escrita.");

    // Vetor de índices
    vector<Index> indices;

    for (const Registro& reg : registros) {
        string buffer = reg.packDescritor();

        streampos posicao = arquivoSaida.tellp();

        arquivoSaida.write(buffer.data(), buffer.size());

        Index index(reg.id, posicao, nomeArquivoIndex);

        Node n = {reg.id, posicao};
        arvoreIndices.Inserir(n);

        indices.push_back(index);
    }
  
    arquivoSaida.close();
    // Abre o arquivo de índice para escrever
    ofstream arquivoIndex(nomeArquivoIndex, ios::app);
    if (!arquivoIndex.is_open())
        throw runtime_error("ERRO: Não foi possível abrir o arquivo de índice.");

    // Escreve todos os índices no arquivo
    for (const Index& index : indices) {
        arquivoIndex << index.id << "|" << index.endereco << "\n";
    }
    arquivoIndex.close();
}
