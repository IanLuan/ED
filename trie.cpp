#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
//#include <time.h>

using namespace std;

typedef struct no {

  char valor;
  bool ePai = false;
  no **filhos;
  bool palavra = false;
  string palavraValor;
  string palavraAtual;
  int codigoPalavra = 0;

} no;

typedef struct arvore {

  no *raiz;
  bool first;
  bool encontrado;
  bool alternar = true;

  int index(char letra) {
    return int(letra)-97;
  }

  no *criarNo(char letra) {
    no *novoNo = new no;
    novoNo->valor = letra;
    novoNo->filhos = new no*[26]();
    return novoNo;
  }

  void inserirPalavra(string palavra, int tamanho) {
    no *aux = raiz;

    for (int i = 0; i < tamanho; i++) {
      if(aux->filhos[index(palavra[i])] == 0) {
        aux->filhos[index(palavra[i])] = criarNo(palavra[i]);
        aux->ePai = true;
      }
      aux = aux->filhos[index(palavra[i])];
    }

    aux->palavra = true;
    aux->palavraValor = palavra;
  }


  void inundar(no *atual, int camada,ofstream *output, string palavraAgora, int codigo) {
    if (camada == 0 || !atual->ePai)
    {
      return;
    }

      for (int i = 0; i < 26; i++)
      {
        if(atual->filhos[i] != 0) {
          if(atual->filhos[i]->palavra) {
            if(atual->filhos[i]->palavraAtual != palavraAgora || (atual->filhos[i]->palavraAtual == palavraAgora && atual->filhos[i]->codigoPalavra != codigo)){
              if(first) {
                *output << atual->filhos[i]->palavraValor;
                atual->filhos[i]->palavraAtual = palavraAgora;
                atual->filhos[i]->codigoPalavra = codigo;
                first = false;
                encontrado = true;
              } else {
                *output << "," << atual->filhos[i]->palavraValor;
                atual->filhos[i]->palavraAtual = palavraAgora;
                atual->filhos[i]->codigoPalavra = codigo;
              }
            }
          }
          inundar(atual->filhos[i], camada-1 ,output, palavraAgora, codigo);
        }
      }

    
  }

  void autocompletar(string palavra, int tamanho, ofstream *output, no *aux, int camada, int ind, int codigo) {
    // encontrar max
    //string pedaco = "";

    if(aux == 0 || camada > tamanho) {
      return;
    }

      if(aux->palavra) {
        if(aux->palavraAtual != palavra || (aux->palavraAtual==palavra && aux->codigoPalavra != codigo)) {
          *output << aux->palavraValor;
          first = false;
          encontrado = true;
          aux->palavraAtual = palavra;
          aux->codigoPalavra = codigo;
        }
      }

    inundar(aux, camada, output, palavra, codigo);
    autocompletar(palavra, tamanho, output, aux->filhos[index(palavra[ind+1])], camada+1, ind+1, codigo);

    /*
    if(!encontrado) {
      *output << "-";
    }
    */
    
  }



} arvore;


int main(int argc, char *argv[])
{

  //clock_t tempo_inicial, tempo_final;
  //tempo_inicial = clock();
	// open files
	ifstream input;
	ofstream output;

	input.open(argv[1]);
	output.open(argv[2]);

  arvore *arvorePrefixo = new arvore;
  no *raiz = new no;
  raiz->filhos = new no*[26]();
  arvorePrefixo->raiz = raiz;

  int addQtd;
  input >> addQtd;

  string palavra;
  for (int i = 0; i < addQtd; i++)
  {
    input >> palavra;
    arvorePrefixo->inserirPalavra(palavra, palavra.length());
  }
  
  int buscaQtd;
  input >> buscaQtd;

  string palavraBuscada;

  for (int i = 0; i < buscaQtd; i++)
  {
    input >> palavraBuscada;
    output << palavraBuscada << ":";
    arvorePrefixo->first = true;
    arvorePrefixo->encontrado = false;
    arvorePrefixo->autocompletar(palavraBuscada, palavraBuscada.length(), &output, arvorePrefixo->raiz->filhos[arvorePrefixo->index(palavraBuscada[0])], 1, 0, i+1);
    arvorePrefixo->alternar = !arvorePrefixo->alternar;
    if(!arvorePrefixo->encontrado) {
      output << "-";
    }
    output << endl;
  }

  //tempo_final = clock();
  //printf("%f\n", (tempo_final - tempo_inicial) / (double)CLOCKS_PER_SEC);

  // close files
	input.close();
	output.close();
	
	return 0;
}