#include "Buffer.h"
#include "Index.h"
#include "Arvore.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>

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
    if (endereco != -1) {
        cout << "ID = " << id << " | Endereço = " << endereco << endl;
        return endereco;
    } else {
        cout << "ID " << id << " não encontrado." << endl;
        throw runtime_error("Índice não encontrado.");
    }
    
}


void Buffer::buscarRegistro(int id){
   try {
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
           file.close();
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
   } catch (const runtime_error& e) {
       cerr << "Erro ao buscar registro: " << e.what() << endl;
   }
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
   // escreve no arquivo de índice para escrita
   ofstream arquivoIndex(nomeArquivoIndex, ios::binary | ios::app);
   if (!arquivoIndex.is_open())
       throw runtime_error("ERRO: Não foi possível abrir o arquivo de índice.");
   // Escreve todos os índices no arquivo
   for (const Index& index : indices) {
       arquivoIndex.write(reinterpret_cast<const char*>(&index.id), sizeof(index.id));
       arquivoIndex.write(reinterpret_cast<const char*>(&index.endereco), sizeof(index.endereco));
   }
   arquivoIndex.close();
}


// Função para carregar stopwords de um arquivo
unordered_set<string> carregarStopwords(const string& arquivoStopwords) {
   unordered_set<string> stopwords;
   ifstream arquivo(arquivoStopwords);
   if (!arquivo) {
       cerr << "Erro ao abrir o arquivo de stopwords: " << arquivoStopwords << endl;
       return stopwords;
   }
   string palavra;
   while (getline(arquivo, palavra)) {
       stopwords.insert(palavra);
   }
   arquivo.close();
   return stopwords;
}


// Função para carregar pontuações de um arquivo
unordered_set<char> carregarPontuacoes(const string& arquivosPontuacao) {
   unordered_set<char> pontuacoes;

   ifstream arquivo(arquivosPontuacao);

   if (!arquivo) {
       cerr << "Erro ao abrir o arquivo de pontuações: " << arquivosPontuacao << endl;
       return pontuacoes;
   }

   char pontuacao;

   while (arquivo.get(pontuacao)) {
       pontuacoes.insert(pontuacao);
   }

   arquivo.close();
   return pontuacoes;
}


// Função para processar o título, removendo pontuações e stopwords
vector<string> verTitulo(const string& titulo, const unordered_set<string>& stopwords, const unordered_set<char>& pontuacao) {
   vector<string> elementos;
   stringstream ss(titulo);
   string palavra;
   while (ss >> palavra) {
       // Remover pontuações
       palavra.erase(remove_if(palavra.begin(), palavra.end(), [&](char c) {
           return pontuacao.find(c) != pontuacao.end();
       }), palavra.end());


       // Converter para minúsculas
       transform(palavra.begin(), palavra.end(), palavra.begin(), ::tolower);


       // Verificar se não é uma stopword
       if (stopwords.find(palavra) == stopwords.end() && !palavra.empty()) {
           elementos.push_back(palavra);
       }
   }
   return elementos;
}


void Buffer::criarIndiceInvertido(const vector<Registro>& registros, const string& arquivosStopwords, const string& arquivosPontuacao, const string& arquivoIndiceInvert) {
   unordered_set<string> stopwords = carregarStopwords(arquivosStopwords);
   unordered_set<char> pontuacoes = carregarPontuacoes(arquivosPontuacao);

   for (const Registro& reg : registros) {
       vector<string> palavras = verTitulo(reg.titulo, stopwords, pontuacoes);
       for (const string& palavra : palavras) {
           indiceInvertido[palavra].push_back(reg.id);
       }
   }
   ofstream arquivoBinario(arquivoIndiceInvert, ios::binary);
   if (!arquivoBinario) {
       throw runtime_error("Erro ao abrir o arquivo de índice invertido para escrita.");
   }

   for (const auto& [palavra, ids] : indiceInvertido) {
       size_t palavraSize = palavra.size();
       arquivoBinario.write(reinterpret_cast<const char*>(&palavraSize), sizeof(palavraSize));
       arquivoBinario.write(palavra.data(), palavraSize);


       size_t idsSize = ids.size();
       arquivoBinario.write(reinterpret_cast<const char*>(&idsSize), sizeof(idsSize));
       arquivoBinario.write(reinterpret_cast<const char*>(ids.data()), idsSize * sizeof(int));
   }
   arquivoBinario.close();
}


vector<int> Buffer::buscarFrase(const string& frase, const string& arquivoIndiceInvert, const string& arquivosStopwords, const string& arquivoPontuacao) {
   unordered_set<string> stopwords = carregarStopwords(arquivosStopwords);
   unordered_set<char> pontuacoes = carregarPontuacoes(arquivoPontuacao);
   // Processa a frase para obter palavras significativas
   vector<string> tokens = verTitulo(frase, stopwords, pontuacoes);


   ifstream arquivoBinario(arquivoIndiceInvert, ios::binary);
   if (!arquivoBinario) {
       throw runtime_error("Erro ao abrir o índice invertido.");
   }
   // Para cada token, localiza IDs e faz interseção
   vector<int> res;
   bool primeiroToken = true;

   for (const auto& token : tokens) {
       // Reinicia leitura do arquivo
       vector<int> idsEncontrados;
       string palavra;
       size_t palavraSize, idsSize;
       arquivoBinario.clear();
       arquivoBinario.seekg(0, ios::beg);
       
       // Busca token no índice
       while (arquivoBinario.read(reinterpret_cast<char*>(&palavraSize), sizeof(palavraSize))) {
           palavra.resize(palavraSize);
           arquivoBinario.read(&palavra[0], palavraSize);

           arquivoBinario.read(reinterpret_cast<char*>(&idsSize), sizeof(idsSize));
           vector<int> ids(idsSize);
           arquivoBinario.read(reinterpret_cast<char*>(ids.data()), idsSize * sizeof(int));

           if (palavra == token) {
               idsEncontrados = ids;
               break;
           }
       }
       // Interseção
       if (primeiroToken) {
           res = idsEncontrados;
           primeiroToken = false;
       } else {
           vector<int> temp;
           sort(res.begin(), res.end());
           sort(idsEncontrados.begin(), idsEncontrados.end());
           set_intersection(res.begin(), res.end(), idsEncontrados.begin(), idsEncontrados.end(), back_inserter(temp));
           res = temp;
       }
   }

   arquivoBinario.close();
   return res;
}


