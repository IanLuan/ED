#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h> 

using namespace std;


typedef struct no
{

  string codigo;
  int *numeros;
  int prioridade;

} no;

typedef struct tree {

  no **heapMax;
  no **heapMin;
  int *sorteados;
  int T;
  int premio;

  int esquerdo(int i) {
    return (2*i) + 1;
  }

  int direito(int i) {
    return (2*i) + 2;
  }

  void troca(int P, int i) {
    no *auxP = heapMax[P];
    no *auxi = heapMax[i];

    heapMax[P] = auxi;
    heapMax[i] = auxP;

  }

  void trocaMin(int P, int i) {
    no *auxP = heapMin[P];
    no *auxi = heapMin[i];

    heapMin[P] = auxi;
    heapMin[i] = auxP;

  }

  void heapifyMax ( int i ) {
    int P = i ;
    int E = esquerdo (i) ;
    int D = direito (i) ;
    if( E < T && heapMax[ E ]->prioridade > heapMax[ P ]->prioridade)
      P = E ;
    if( D < T && heapMax[ D ]->prioridade > heapMax[ P ]->prioridade)
      P = D ;
    if( P != i ) {
     troca (P, i) ;
     heapifyMax ( P ) ;
    }
  }

  void heapifyMin ( int i ) {
    int P = i ;
    int E = esquerdo (i) ;
    int D = direito (i) ;
    if( E < T && heapMin[ E ]->prioridade < heapMin[ P ]->prioridade)
      P = E ;
    if( D < T && heapMin[ D ]->prioridade < heapMin[ P ]->prioridade)
      P = D ;
    if( P != i ) {
     trocaMin (P, i) ;
     heapifyMin ( P ) ;
    }
  }

  void ganhadoresMax(ofstream *output) {

    int maxNumber = heapMax[0]->prioridade;
    int ganhadoresQtd = 0;

    for (int i = 0; i < T; i++) {
      if (heapMax[i]->prioridade == maxNumber) {
        ganhadoresQtd++;
      }
    }

    *output << "[" << ganhadoresQtd << ":" << maxNumber << ":" << (premio/2)/ganhadoresQtd << "]" << endl;

    /*
    for (int i = 0; i < T; i++)
    {
      if (heapMax[i]->prioridade == maxNumber)
      {
        *output << heapMax[i]->codigo << endl;
      }
      
    }
    */
   
    int tamanho = T;
    for (int i = 0; i < ganhadoresQtd; i++)
    {
      *output << heapMax[0]->codigo << endl;
      heapMax[0] = heapMax[T-1];
      T--;
      heapifyMax(0);
    }
    T = tamanho;
    
  }

  void ganhadoresMin(ofstream *output) {

    int minNumber = heapMin[0]->prioridade;
    int ganhadoresQtd = 0;

    for (int i = 0; i < T; i++) {
      if (heapMin[i]->prioridade == minNumber) {
        ganhadoresQtd++;
      }
    }

    *output << "[" << ganhadoresQtd << ":" << minNumber << ":" << (premio/2)/ganhadoresQtd << "]" << endl;

    /*
    for (int i = 0; i < T; i++)
    {
      if (heapMin[i]->prioridade == minNumber)
      {
        //cout << heapMin[i]->codigo << " " << heapMin[i]->prioridade << endl;
        *output << heapMin[i]->codigo << endl;
      }
      
    }
    */

    int tamanho = T;
    for (int i = 0; i < ganhadoresQtd; i++)
    {
      *output << heapMin[0]->codigo << endl;
      heapMin[0] = heapMin[T-1];
      T--;
      heapifyMin(0);
    }
    T = tamanho;
    
  
  }

} tree;


int main(int argc, char *argv[])
{

	// open files
	ifstream input;
	ofstream output;

	input.open(argv[1]);
	output.open(argv[2]);


  int premio;
  int qtd;
  input >> premio;
  input >> qtd;
  int *sorteados = new int[10];

  int sorteado;
  for (int i = 0; i < 10; i++) {
    input >> sorteado;
    sorteados[i] = sorteado;
  }

  tree *heapTree = new tree;
  heapTree->sorteados = sorteados;
  no **heapMax = new no*[qtd];
  no **heapMin = new no*[qtd];

  string codigo;
  for (int i = 0; i < qtd; ++i) {
    no *novoNo = new no;
    input >> codigo;

    int numero;
    int prioridade = 0;
    for (int j = 0; j < 15; j++) {
      input >> numero;
      for (int g = 0; g < 10; g++)
      {
        if (numero == sorteados[g])
        {
          prioridade++;
        }
      }
    }

    novoNo->codigo = codigo;
    novoNo->prioridade = prioridade;

    heapMax[i] = novoNo;
    heapMin[i] = novoNo;
    
  }
  
  heapTree->heapMax = heapMax;
  heapTree->heapMin = heapMin;
  heapTree->T = qtd;
  heapTree->premio = premio;


  for (int i = ((qtd-1)-1)/2; i >= 0; i--)
  {
    heapTree->heapifyMax(i);
    heapTree->heapifyMin(i);
  }
  
  heapTree->ganhadoresMax(&output);
  heapTree->ganhadoresMin(&output);

  // close files
	input.close();
	output.close();
	
	return 0;
}