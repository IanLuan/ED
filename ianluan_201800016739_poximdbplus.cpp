#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h> 

using namespace std;

typedef struct chave {
	string nome = "";
	int tamanho;
	string hash = "";
} chave;

typedef struct no {

	chave **chaves;
 	no **filhos;
	no *pai = 0;
 	int quantidade = 0;
	bool folha = true;
	no *proximo = 0;
	no *anterior = 0;

 } no;

 typedef struct tree {
	no *root;
	int *ordem;
	
	void add(chave *key) {
		no *aux = root;

    while(true) {

			for (int i = 0; i < *ordem; i++) {

				if(i == aux->quantidade && !aux->folha) {
						aux = aux->filhos[i];
						break;
				} else {

					if (aux->chaves[i] == 0 && aux->folha) {
						aux->chaves[i] = key;
						aux->quantidade++;
						goto dividir;

					} else if (aux->chaves[i]->hash.compare(key->hash) > 0 && i < aux->quantidade) {
						if (aux->folha) {

							if (aux->chaves[i]->hash == "")
							{
								aux->chaves[i] = key;
								aux->quantidade++;

							} else {
								// TRANSLOCAÇÃO
								int count = i;
								chave *antes = key;
								while (true) {
									chave *proximo = aux->chaves[count];
									aux->chaves[count] = antes;
									antes = proximo;
									if (antes == 0) {
										aux->quantidade += 1;
										goto dividir;
									}
									count++;
								}
								}		

							} else {
								aux = aux->filhos[i];
								break;
							}
					}
				}
			}
		}
		dividir:
		if (aux->quantidade == *ordem) {
			//cout << "Dividr nó";
			if(aux->folha) {
				divisao_no_folha(aux);
			} else {
				divisao_no(aux);
			}

			if (aux->pai != 0) {
				juntar_no(aux);
			}
			
			
		}

	}

	void divisao_no ( no * x ) {
		bool isRoot = false;
		bool isFolha = false;
		if (x->folha)
		{
			isFolha = true;
		}
		
		if (root == x)
		{
			isRoot = true;
		}
		
		uint32_t i = 0;
		no * y = criar_no(isFolha);
		no * z = criar_no(isFolha);
		for ( i = 0; i < *ordem / 2; i ++) {
 			y -> chaves [ i ] = x -> chaves [ i ];
			y -> filhos [ i ] = x -> filhos [ i ];
			if(y->filhos[i] != 0) {
				y->filhos[i]->pai = y;
			}
			y -> quantidade++;
 		} 
		y -> filhos [ i ] = x -> filhos [ i ];
		if(y->filhos[i] != 0) {
				y->filhos[i]->pai = y;
		}
 		for ( i = ( *ordem / 2) + 1; i < x -> quantidade; i ++) {
			z -> chaves [ i - ( *ordem / 2) - 1] = x -> chaves [ i ];
			z -> filhos [ i - ( *ordem / 2) - 1] = x -> filhos [ i ]; 
			if(z->filhos[i-(*ordem/2)-1] != 0) {
				z->filhos[i-(*ordem/2)-1]->pai = z;
			}
			z -> quantidade ++;
		}
		z -> filhos [ i - ( *ordem / 2) - 1] = x -> filhos [ i ];
		if(z->filhos[i-(*ordem/2)-1] != 0) {
				z->filhos[i-(*ordem/2)-1]->pai = z;
		}
		x -> chaves [0] = x -> chaves [ *ordem / 2];
		x -> filhos [0] = y;
		x -> filhos [1] = z;
		x -> filhos [0] -> pai = x;
		x -> filhos [1] -> pai = x;
		x -> folha = false;
		x -> quantidade = 1;
		if (isRoot)
		{
			root = x;
		}
		
	}

	void divisao_no_folha ( no * x ) {
		bool isRoot = false;
		bool isFolha = false;
		if (x->folha)
		{
			isFolha = true;
		}
		
		if (root == x)
		{
			isRoot = true;
		}
		
		uint32_t i = 0;
		no * y = criar_no(isFolha);
		no * z = criar_no(isFolha);
		for ( i = 0; i < *ordem / 2; i ++) {
 			y -> chaves [ i ] = x -> chaves [ i ];
			y -> filhos [ i ] = x -> filhos [ i ];
			if(y->filhos[i] != 0) {
				y->filhos[i]->pai = y;
			}
			y -> quantidade++;
 		} 
		y -> filhos [ i ] = x -> filhos [ i ];
		if(y->filhos[i] != 0) {
				y->filhos[i]->pai = y;
		}
 		for ( i = ( *ordem / 2); i < x -> quantidade; i ++) {
			
			z -> chaves [ i - ( *ordem / 2)] = x -> chaves [ i ];
			z -> filhos [ i - ( *ordem / 2)] = x -> filhos [ i ]; 
			if(z->filhos[i-(*ordem/2)] != 0) {
				z->filhos[i-(*ordem/2)]->pai = z;
			}
			z -> quantidade ++;
		}
		z -> filhos [ i - ( *ordem / 2)] = x -> filhos [ i ];
		if(z->filhos[i-(*ordem/2)] != 0) {
				z->filhos[i-(*ordem/2)]->pai = z;
		}
		y->proximo = z;
		z->anterior = y;
		if(x->anterior != 0) {
			x->anterior->proximo = y;
			y->anterior = x->anterior;
		}

		x -> chaves [0] = x -> chaves [ *ordem / 2];
		x -> filhos [0] = y;
		x -> filhos [1] = z;
		x -> filhos [0] -> pai = x;
		x -> filhos [1] -> pai = x;
		x -> folha = false;
		x -> quantidade = 1;
		if (isRoot)
		{
			root = x;
		}
		
	}

	no* criar_no(bool isFolha) {
		no *novoNo = new no();
		chave **chaves = new chave*[*ordem]();
		no **filhos = new no*[*ordem+1]();
		
		novoNo->chaves = chaves;
		novoNo->filhos = filhos;
		for (int i = 0; i < *ordem; i++)
		{
			novoNo->filhos[i] = 0;
		}
		
		novoNo->folha = isFolha;
		return novoNo;
	}

	void juntar_no(no *mediana) {
		no *pai = mediana->pai;
		for (int i = 0; i < *ordem; i++)	{
			if (i == pai->quantidade)
			{
				pai->chaves[i] = mediana->chaves[0];
				pai->quantidade++;
				pai->filhos[i] = mediana->filhos[0];
				pai->filhos[i+1] = mediana->filhos[1];
				pai->filhos[i]->pai = pai;
				pai->filhos[i+1]->pai = pai;
				break;

			} else if (pai->chaves[i]->hash.compare(mediana->chaves[0]->hash) > 0)	{

				// TRANSLOCAÇÃO
				int count = i;
				chave *antes = new chave();
				chave *proximo = new chave();
				antes = mediana->chaves[0];
				while (true) {
					proximo = pai->chaves[count];
					pai->chaves[count] = antes;
					antes = proximo;
					if (count >= pai->quantidade) {
						pai->quantidade += 1;
						break;
					}
					count++;
				}

				// TRANSLOCAÇÃO FILHOS
				// TRANSLOCAÇÃO
				count = i;
				no *antesF = pai->filhos[i];
				while (count < *ordem+1) {
					no *proximoF = pai->filhos[count];
					pai->filhos[count] = antesF;
					antesF = proximoF;
					count++;
				}

				pai->filhos[i] = mediana->filhos[0];
				pai->filhos[i+1] = mediana->filhos[1];
				pai->filhos[i]->pai = pai;
				pai->filhos[i+1]->pai = pai;
				break;

			}
		}
		if (pai->quantidade == *ordem) {
			if (pai->folha)
			{
				divisao_no_folha(pai);
			} else {
				divisao_no(pai);
			}

			if (pai->pai != 0) {
				juntar_no(pai);
			}
		}

	}

	// Função de busca na árvore
	no * busca ( no * x , string c ) {
		no * r = 0 ;
		if(x != 0 || x->quantidade) {
			uint32_t i = 0;
			while ( i < x -> quantidade && c.compare(x -> chaves[i]->hash) > 0)
				i ++;
			if( i < x -> quantidade && c.compare(x->chaves[ i ]->hash) == 0)
				if(x->folha) {
					r = x ;
				} else {
					r = busca (x -> filhos[ i+1 ] , c );
				}
			else {
				if (x->folha) {
					r = 0;
				} else {
					r = busca (x -> filhos[ i ] , c ) ;
				}
			}
		}
		return r ;
	}

	void select(string chave, ofstream *output) {
		no *encontrado = busca(root, chave);
		*output << "[" << chave << "]" << endl;
		if (encontrado == 0) {
			*output << "-" << endl;
		} else {
			for (int i = 0; i < encontrado->quantidade; i++) {
				*output << encontrado->chaves[i]->nome << ":size=" << encontrado->chaves[i]->tamanho << ",hash=" << encontrado->chaves[i]->hash << endl;
			}
		}
	}

	no * buscaRange ( no * x , string c ) {
		no * r = 0 ;
		if(x->folha) {
			r = x;
		} else {
			if(x != 0 || x->quantidade) {
				uint32_t i = 0;
				while ( i < x -> quantidade && c.compare(x -> chaves[i]->hash) > 0)
					i ++;
				if( i < x -> quantidade && c.compare(x->chaves[ i ]->hash) == 0)
					if(x->folha) {
						r = x;
					} else {
						r = buscaRange (x -> filhos[ i+1 ] , c );
					}
				else {
					if (x->folha) {
						r = x;
					} else {
						r = buscaRange (x -> filhos[ i ] , c ) ;
					}
				}
			}
		}
		return r;
	}

	void printar(no *encontrado, ofstream *output) {
		for (int i = 0; i < encontrado->quantidade; i++) {
			*output << encontrado->chaves[i]->nome << ":size=" << encontrado->chaves[i]->tamanho << ",hash=" << encontrado->chaves[i]->hash << endl;
		}	
	}

	void selectRange(string chave1, string chave2, ofstream *output) {

		bool encontrou = false;
		bool passou = false;
		no *encontrado = buscaRange(root, chave1);
		*output << "[" << chave1 << "," << chave2 << "]" << endl;

		for (int i = 0; i < encontrado->quantidade; i++)
		{
			if (encontrado->chaves[i]->hash.compare(chave1) >= 0 && encontrado->chaves[i]->hash.compare(chave2) <= 0) {
				printar(encontrado, output);
				encontrou = true;
			}
		}

		encontrado = encontrado->proximo;
		
		while(encontrado != 0 && ((encontrado->chaves[0]->hash).compare(chave2) <= 0)) {
			printar(encontrado, output);
			encontrado = encontrado->proximo;
		}

		/*
		while (encontrado != 0) {
			
			for (int i = 0; i < encontrado->quantidade; i++) {
				if( (encontrado->chaves[i]->hash).compare(chave1) >= 0 && encontrado->chaves[i]->hash.compare(chave2) <= 0) {
					printar(encontrado, output);
					encontrou = true;
					break;
				}

				if(encontrado->chaves[i]->hash.compare(chave2) > 0) {
					bool passou = true;
					break;
				}
			}
			if(passou) {
				break;
			} else {
				encontrado = encontrado->proximo;
			}
		}

		if(!encontrou) {
			*output << "-" << endl;
		} */

	}

 } tree;


int main(int argc, char *argv[])
{
	// open files
	ifstream input;
	ofstream output;

	input.open(argv[1]);
	output.open(argv[2]);

  int ordem;
	int qtdFiles;

	input >> ordem;
	input >> qtdFiles;

	no *raiz = new no();
	chave **chaves = new chave*[ordem]();
	no **filhos = new no*[ordem+1]();
	raiz->chaves = chaves;
	raiz->filhos = filhos;
	raiz->folha = true;
	tree *arvore = new tree();
	arvore->root = raiz;

	arvore->ordem = &ordem;
	

	for (int i = 0; i < qtdFiles; i++) {
		string nome;

		int tamanho;
		string hash;
		input >> nome;
		input >> tamanho;
		input >> hash;
		chave *key = new chave();
		key->nome = nome;
		key->tamanho = tamanho;
		key->hash = hash;
		arvore->add(key);
	}

	int opNum;
	input >> opNum;
	for (int i = 0; i < opNum; i++) {
		string op;
		input >> op;
		if (op == "INSERT")
		{
			string nome;
			int tamanho;
			string hash;
			input >> nome;
			input >> tamanho;
			input >> hash;
			chave *key = new chave();
			key->nome = nome;
			key->tamanho = tamanho;
			key->hash = hash;
			arvore->add(key);
		} else if(op == "SELECT") {
			string secondOp;
			input >> secondOp;
			
			if(secondOp == "RANGE") {
				string chaveBusca1;
				string chaveBusca2;
				input >> chaveBusca1;
				input >> chaveBusca2;
				if(chaveBusca1.compare(chaveBusca2) >= 0) {
					arvore->selectRange(chaveBusca2, chaveBusca1, &output);
				} else {
					arvore->selectRange(chaveBusca1, chaveBusca2, &output);
				}
			} else {
				string chaveBusca;
				chaveBusca = secondOp;
				arvore->select(chaveBusca, &output);
			}
		}
		
	}
	

  // close files
	input.close();
	output.close();
	
	return 0;
	return 0;
}