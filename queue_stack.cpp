#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdio.h>  


using namespace std;

typedef struct doc
{

	string name;
	int pages;

	doc *next = 0;
	bool inStack = false;

} doc;

typedef struct docs
{

	doc *top = 0;
	int total = 0;

	void addDocToStack(doc *presentDoc)
	{
		if (top == 0)
		{
			top = presentDoc;
		} else {
			presentDoc->next = top;
			top = presentDoc;
		}

		total += presentDoc->pages;
		presentDoc->inStack = true;
		
	}

	void showStack(ofstream *output)
	{
		doc *aux = top;

		*output << total << "p" << endl;

		while (aux != 0)
		{
			*output << aux->name << "-" << aux->pages << "p" << endl;
			aux = aux->next;
		}
		
	}

} docs;


typedef struct impressora
{

	string name;
	string status = "";
	int pages = 0;

	impressora *next = 0;
	impressora *previous = 0;
	doc *previousDoc = 0;

} impressora;

typedef struct impressoras
{

	//  Ponteiro  inicial
	impressora *start = 0;

	//  Ponteiro  final
	impressora *end = 0;

	void addImpressora(string name)
	{

		impressora *nova = new impressora();
		nova->name = name;

		if (start == 0)
		{
			start = nova;
			end = nova;
			end->previous = start;
		}
		else
		{
			end->next = nova;
			nova->previous = end;
			end = nova;
		}
	}

	void addDoc(doc *presentDoc, impressora *presentImpressora, ofstream *output)
	{

		presentImpressora->pages += presentDoc->pages;

		if (presentImpressora->status == "")
		{
			presentImpressora->status = presentDoc->name + "-" + to_string(presentDoc->pages) + "p" + presentImpressora->status;
		}
		else
		{
			presentImpressora->status = presentDoc->name + "-" + to_string(presentDoc->pages) + "p" + ", " + presentImpressora->status;
		}

		*output << "[" << presentImpressora->name << "] " << presentImpressora->status << endl;
	
	}

	void addFirstDocs(int *countDoc, int *menor, ifstream *input, ofstream *output) {
		
		impressora *aux = new impressora;
		aux = start;
		bool isFirst = true;

		while (aux != 0 && *countDoc > 0)
		{
			doc *presentDoc = new doc;
			string name;
			int pages;

			*input >> name >> pages;

			presentDoc->name = name;
			presentDoc->pages = pages;
			addDoc(presentDoc, aux, output);
			aux->previousDoc = presentDoc;

			if (isFirst)
			{
				*menor = pages;
				isFirst = false;
			} else
			{
				if (pages < *menor)
				{
					*menor = pages;
				}
				
			}
			

			aux = aux->next;
			*countDoc -= 1;
		}
		
	}

	void addAllDocs(int *countDoc, int *menor, ifstream *input, ofstream *output, docs *docStack) {

		while (*countDoc > 0)
		{

			impressora *aux = new impressora;
			aux = start;
			int secondMenor = 0;

			while (aux != 0)
			{
				aux->pages -= *menor;

				if (aux->pages == 0)
				{
					doc *presentDoc = new doc;
					string name;
					int pages;

					*input >> name >> pages;

					if (name != "")
					{
						presentDoc->name = name;
						presentDoc->pages = pages;
						addDoc(presentDoc, aux, output);
						docStack->addDocToStack(aux->previousDoc);
						aux->previousDoc = presentDoc;
					}
					
				}

				if (secondMenor == 0)
				{
					secondMenor = aux->pages;

				} else if (aux->pages < secondMenor)
				{
					secondMenor = aux->pages;
				}
			
				aux = aux->next;
			}
			*countDoc -= 1;
			*menor = secondMenor;
			secondMenor = 0;
			
		}
	}

	void removerImpressora(impressora *presentImpressora) {
		if (presentImpressora == start)
		{
			start = start->next;
			
		} else if (presentImpressora == end)
		{
			end = end->previous;
			end->next = 0;
		} else {
			presentImpressora->previous->next = presentImpressora->next;
			presentImpressora->next->previous = presentImpressora->previous;
		}
	}

	void addRemainingDocsToStack(docs *docStack) {
		impressora *aux = new impressora;
		aux = start;

		while (aux != 0)
		{	
			if (aux->previousDoc != 0)
			{
				if (aux->previousDoc->inStack)
				{
					removerImpressora(aux);
				}
			} else {
				removerImpressora(aux);
			}
			aux = aux->next;
		}

		while (start != 0)
		{
			impressora *aux = new impressora;
			aux = start;
			impressora *menorImpr = start;

			while (aux != 0)
			{
				if (aux->pages < menorImpr->pages)
				{
					menorImpr = aux;
				}
				aux = aux->next;
			}
			docStack->addDocToStack(menorImpr->previousDoc);
			removerImpressora(menorImpr);
		}

	}

} impressoras;


int main(int argc, char *argv[])
{

	impressoras listImpressoras;
	docs docStack;
	int *menor = new int;

	// open files
	ifstream input;
	ofstream output;

	input.open(argv[1]);
	output.open(argv[2]);

	// get impressora's number
	int qtd;
	input >> qtd;
	int countImpr = qtd;

	string linha;

	while (countImpr != 0)
	{
		input >> linha;
		listImpressoras.addImpressora(linha);
		countImpr--;
	}

	// get doc's number
	input >> qtd;
	int *countDoc = new int;
	*countDoc = qtd;

	// add to all impressoras first
	listImpressoras.addFirstDocs(countDoc, menor, &input, &output);

	// add Docs
	listImpressoras.addAllDocs(countDoc, menor, &input, &output, &docStack);

	// Add Remaing
	listImpressoras.addRemainingDocsToStack(&docStack);

	// Show Stack
	docStack.showStack(&output);
	

	// close files
	input.close();
	output.close();
	
	return 0;
}