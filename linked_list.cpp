#include <iostream>
#include <fstream>
#include <string>

using namespace std;

typedef struct pessoa {
	
	string nome;
	
	// Ponteiro para anterior
	pessoa* amigoAnterior;
	
	// Ponteiro para próximo
	pessoa* amigoProximo;
 
} pessoa;

typedef struct  lista {

//  Ponteiro
	pessoa* head;
	pessoa* cauda;

} lista;

bool verificar(string nome, lista* pessoas) {

	pessoa *atual = pessoas -> head;

	if (pessoas -> head != 0) {
		while (true) {

			if (atual -> nome == nome) {
				return true;

			} else if (atual -> nome == pessoas -> cauda -> nome) {
				return false;

			} else {
				atual = atual -> amigoProximo;
			}

		}
	} else {
		return false;
	}
	
}

void addPessoa(string nome, lista* pessoas, ofstream* output) {

	// Verificar se já existe
	bool existe = false;

	existe = verificar(nome, pessoas);

	
	if (!existe) {

		// Declaração
		pessoa *novaPessoa = new pessoa();
		novaPessoa -> nome = nome;

		// Caso a lista esteja vazia
		if(pessoas -> head == 0) {

			// Adicionando pessoa como a cabeça e a cauda
			pessoas -> head = novaPessoa;
			pessoas -> cauda = novaPessoa;
			
			// Ligando o próximo e anterior para ele mesmo (único item)
			pessoas -> head -> amigoProximo = novaPessoa;
			pessoas -> head -> amigoAnterior = novaPessoa;


		// Caso a lista só tenha 1 elemento		
		} else if (pessoas -> head == pessoas -> cauda) {

			// Passando a cauda para o último
			pessoas -> cauda = novaPessoa;

			// Adicionando o próximo e o anterior da cabeça
			pessoas -> head -> amigoProximo = novaPessoa;
			pessoas -> head -> amigoAnterior = novaPessoa;

			// Adicionando o próximo e o anterior do novo elemento
			pessoas -> head -> amigoProximo -> amigoAnterior = pessoas -> head;
			pessoas -> head -> amigoProximo -> amigoProximo = pessoas -> head;


		// Caso a lista tenha 2 ou mais elementos
		} else {

			// Ligando o último elemento ao NOVO
			pessoas -> cauda -> amigoProximo = novaPessoa;

			// Ligando o último elemento como anterior do NOVO
			pessoas -> cauda -> amigoProximo -> amigoAnterior = pessoas -> cauda;

			// Passando a cauda para o NOVO elemento
			pessoas -> cauda = novaPessoa;

			// Ligando a cabeça como próximo do NOVO elemento
			pessoas -> cauda -> amigoProximo = pessoas -> head;

			// Ligando o NOVO elemento como anterior da cabeça
			pessoas -> head -> amigoAnterior = pessoas -> cauda;

		}

		*output << "[ OK  ] ADD " << nome << "\n";

	} else {
		*output << "[ERROR] ADD " << nome << "\n";
	}

}

void remover(string nome, lista* pessoas, ofstream* output) {

	if (pessoas -> head != 0) {
		pessoa *atual = pessoas -> head;

		while (true) {

			if (atual -> nome == nome) {

				// Se for a cabeça
				if(atual -> nome == pessoas -> head -> nome) {

					if(pessoas -> head -> amigoProximo -> nome != pessoas -> head -> nome) {
						
						// Passando a cabeça para o próximo
						pessoas -> head = pessoas -> head -> amigoProximo;

						// Ligando a cauda à nova cabeça
						pessoas -> cauda -> amigoProximo = pessoas -> head;

						// Ligando nova cabeça à cauda
						pessoas -> head -> amigoAnterior = pessoas -> cauda;
					
					} else {
						pessoas -> head = 0;
						pessoas -> cauda = 0;
					}

				// Se for a cauda	
				} else if (atual -> nome == pessoas -> cauda -> nome) {

					// Passando a cauda para o anterior
					pessoas -> cauda = pessoas -> cauda -> amigoAnterior;

					// Ligando nova cauda à cabeça
					pessoas -> cauda -> amigoProximo = pessoas -> head;

					// Ligando a cabeça à nova cauda
					pessoas -> head -> amigoAnterior = pessoas -> cauda;

				} else {

					// ligando anterior ao proximo
					atual -> amigoAnterior -> amigoProximo = atual -> amigoProximo;
					atual -> amigoProximo -> amigoAnterior = atual -> amigoAnterior;

				}

				*output << "[ OK  ] REMOVE " << nome << "\n";
				break;

			} else if (atual -> nome == pessoas -> cauda -> nome) {
				*output << "[ERROR] REMOVE " << nome << "\n";
				break;

			} else {
				atual = atual -> amigoProximo;
			}

		}
	} else {
		*output << "[ERROR] REMOVE " << nome << "\n";
	}

}

void show(string nome, lista* pessoas, ofstream* output) {

	if (pessoas -> head != 0) {
		pessoa *atual = pessoas -> head;

		while (true) {

			if (atual -> nome == nome) {
				*output << "[ OK  ] " << atual -> amigoAnterior -> nome << "<-" << atual -> nome << "->" << atual -> amigoProximo -> nome << "\n";
				break;

			} else if (atual -> nome == pessoas -> cauda -> nome) {
				*output << "[ERROR] ?<-" << nome << "->?" << "\n";
				break;

			} else {
				atual = atual -> amigoProximo;
			}

		}
	} else {
		*output << "[ERROR] ?<-" << nome << "->?" << "\n";
	}

}

int main(int argc, char* argv[]) {

	lista pessoas;
	pessoas.head = 0;
	pessoas.cauda = 0;

	// Abrindo arquivos
	ifstream input;
	ofstream output; 
	input.open(argv[1]);
	output.open(argv[2]);

	// Writing and Reading
	string linha;

	while(getline(input, linha)) {
		
		int spacePosi = linha.find(" ");
		string comando = linha.substr(0, spacePosi);
		string nome = linha.substr(spacePosi+1);

		if(comando == "ADD") {
			addPessoa(nome, &pessoas, &output);

		} else if(comando == "REMOVE") {
			remover(nome, &pessoas, &output);

		} else {
			show(nome, &pessoas, &output);
		}
	}

	// Fechando arquivos
	input.close();
	output.close();

	return 0;


}
