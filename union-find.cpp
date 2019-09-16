#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h> 

using namespace std;

// Função myrand
uint32_t myrand () {
  static uint32_t next = 1;
  next = next * 1103515245 + 12345;
  return next ;
}

int coord(int x) {
  return x + (-1 + myrand() % 3);
}

void infectar(int **matriz, int x, int y, int altura, int largura, ofstream *output) {
  long long total = (altura*largura)-1;
  matriz[x][y] = 1;

  int xNovo = x;
  int yNovo = y;
  int xTalvez, yTalvez;

  while (total > 0) {
    xTalvez = coord(xNovo);
    while(xTalvez >= altura or xTalvez < 0) {
      xTalvez = coord(xNovo);
    }
    xNovo = xTalvez;

    yTalvez = coord(yNovo);
    while(yTalvez >= largura or yTalvez < 0) {
      yTalvez = coord(yNovo);
    }
    yNovo = yTalvez;

    if(matriz[xNovo][yNovo] == 0) {
      *output << ";(" << xNovo << "," << yNovo << ")";
      matriz[xNovo][yNovo] = 1;
      total--;
    }
  }
  *output << endl;
}

int main(int argc, char *argv[])
{

	// open files
	ifstream input;
	ofstream output;

	input.open(argv[1]);
	output.open(argv[2]);

  int regioes;
  input >> regioes;

  int altura, largura, x, y;

  for (int i = 0; i < regioes; i++) {
    input >> altura >> largura >> x >> y;
    
    // Criando matriz e preenchendo com 0s
    int **matriz = new int*[altura]();
    for (int j = 0; j < altura; j++)
    {
      matriz[j] = new int[largura]();
    }
    output << i+1 << ":(" << x << "," << y << ")";
    infectar(matriz, x, y, altura, largura, &output);

  }
  
  
  // close files
	input.close();
	output.close();
	
	return 0;
}