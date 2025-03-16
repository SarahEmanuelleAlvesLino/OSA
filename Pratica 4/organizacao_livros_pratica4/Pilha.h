/*****************************************************
 * IMPLEMENTA��O DO TAD - Aloca��o din�mica
 *****************************************************
 * TAD PILHA IMPLEMENTADO UTILIZANDO ENCADEAMENTO DE N�S
  *****************************************************/
// diretiva de pr�-compila��o para evitar a redefini��o do tipo
#ifndef PILHA_H
#define PILHA_H

#include <cstdlib> // para usar o apontador NULL
#include <cassert>

using namespace std;

// implementa��o de uma classe para representa��o dos n�s da PILHA
template <class T>
class NodePilha
{
    public:
        T item;
        NodePilha<T>* prox;
        // construtor default
        NodePilha(){ prox = NULL;}
        // construtor com um parametro
        NodePilha(T it){item =it; prox =NULL;}
};

// Declara��o da classe pilha, seus atributos e m�todos.
template <class TIPO>
class Pilha
{
    private:
        NodePilha<TIPO>* topo; // apontador para o topo da pilha
    public:
        Pilha(); // construtor do TAD pilha
        void Empilhar(TIPO); // m�todo para empilhar dados no topo da pilha
        TIPO  Desempilhar(); // m�todo para desempilhar dados do topo da pilha
        bool Vazia(); // m�todo para verificar se um pilha est� vazia
};

// construtor do TAD pilha
template <class T>
Pilha<T>::Pilha()
{
    topo = NULL;
}

// m�todo para empilhar dados no topo de uma pilha
template <class T>
void Pilha<T>::Empilhar(T item)
{
    // criar um novo n� contendo o item e apontando o prox para NULL (veja construtor do NodePilha)
    NodePilha<T>* aux = new NodePilha<T>(item);
    aux->prox = topo;
    topo = aux;
}

// m�todo para desempilhar dados do topo da pilha
template <class T>
T  Pilha<T>::Desempilhar()
{
    assert ( !Vazia() );

    T aux = topo->item; // recupera o elemento que est� no topo da pilha
    NodePilha<T>* tmp = topo; // armazena o endere�o do topo em tmp
    topo = topo->prox; // atualiza o topo para o pr�ximo da pilha
    delete tmp;   // remove da pilha o n� que estava no topo da pilha
    return aux;   // retorna o resultado

}

// m�todo para verificar se uma pilha est� vazia
template <class T>
bool Pilha<T>::Vazia()
{
    return topo == NULL;
}

#endif
