#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h> 

using namespace std;

typedef struct no
{

	no *left = 0;
	no *right = 0;

  string name;
  string type;
  long size;
  int index;

} no;

typedef struct tree
{
  no *root = 0;

  void addNo(no *newNo) {
    if (root == 0)
    {
      root = newNo;
    } else {
      no *aux = root;
      while(true) {
        if (aux->name.compare(newNo->name) < 0)
        {
          // SE NEWNO FOR MAIOR
          if (aux->right == 0)
          {
            aux->right = newNo;
            break;
          } else
          {
            aux = aux->right;
          }

        } else if (aux->name.compare(newNo->name) > 0) {
          // SE NEWNO FOR MENOR
          if (aux->left == 0)
          {
            aux->left = newNo;
            break;
          } else
          {
            aux = aux->left;
          }

        } else {
          // SE FOR IGUAL
          if (aux->type == "rw")
          {
            aux->type = newNo->type;
            aux->size = newNo->size;
            aux->index = newNo->index;
          }
          break;
        }
      }
    }
  }

} tree;

void ordem(no *root, ofstream *output){
  if(root != 0){

    ordem(root->left, output);

    if (root->size > 1)
    {
      *output << root->index << " " << root->name << " " << root->type << " " << root->size << " bytes" << endl;
    } else
    {
      *output << root->index << " " << root->name << " " << root->type << " " << root->size << " byte" << endl;
    }

    ordem(root->right, output);
  }
}

void preOrdem(no *root, ofstream *output){
  if(root != 0){

    if (root->size > 1)
    {
      *output << root->index << " " << root->name << " " << root->type << " " << root->size << " bytes" << endl;
    } else
    {
      *output << root->index << " " << root->name << " " << root->type << " " << root->size << " byte" << endl;
    }

    preOrdem(root->left, output);
    preOrdem(root->right, output);
  }
}

void posOrdem(no *root, ofstream *output){
  if(root != 0){

    posOrdem(root->left, output);
    posOrdem(root->right, output);

    if (root->size > 1)
    {
      *output << root->index << " " << root->name << " " << root->type << " " << root->size << " bytes" << endl;
    } else
    {
      *output << root->index << " " << root->name << " " << root->type << " " << root->size << " byte" << endl;
    }
  }
}


int main(int argc, char *argv[])
{

	// open files
	ifstream input;
	ofstream output;

	input.open(argv[1]);
	output.open(argv[2]);

  tree *searchTree = new tree;

  int filesNum;
  string fileName;
  string fileType;
  long fileSize;

  // GET
  input >> filesNum;
  for (int i = 0; i < filesNum; i++)
  {
    no *newNo = new no;
    input >> fileName >> fileType >> fileSize;
    newNo->name =fileName;
    newNo->type = fileType;
    newNo->size = fileSize;
    newNo->index = i;
    searchTree->addNo(newNo);
  }

  // SHOW
  output << "EPD:" << endl;
  ordem(searchTree->root, &output);

  output << "PED:" << endl;
  preOrdem(searchTree->root, &output);

  output << "EDP:" << endl;
  posOrdem(searchTree->root, &output);
  

  // close files
	input.close();
	output.close();
	
	return 0;
}