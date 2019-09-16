#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h> 

using namespace std;

typedef struct no
{

  no *dad = nullptr;
	no *left = nullptr;
	no *right = nullptr;

  string word;
  string synonyms;
  int balance = 0;
  int height = 0;
  int side = 0;
  int rootSide = 2;

} no;

typedef struct tree
{
  no *root = nullptr;

  void addNo(no *newNo) {
    if (root == nullptr)
    {
      root = newNo;
    } else {
      no *aux = root;
      while(true) {
        if (aux->word.compare(newNo->word) < 0)
        {
          // SE NEWNO FOR MAIOR
          if (aux->right == nullptr)
          {
            aux->right = newNo;
            newNo->dad = aux;
            newNo->side = 1;
            
            break;
          } else
          {
            aux = aux->right;
          }

        } else if (aux->word.compare(newNo->word) > 0) {
          // SE NEWNO FOR MENOR
          if (aux->left == nullptr)
          {
            aux->left = newNo;
            newNo->dad = aux;
            newNo->side = 0;

            break;
          } else
          {
            aux = aux->left;
          }

        } else {
          // SE FOR IGUAL
          break;
        }
      }
      //updateHeightNo(newNo);
        //heightNo(root);
  
        //balancingNo(newNo);
  
      /**
      ordem(root);
      cout << endl;
      cout << root->balance << endl;
      */
    }
  }

  void search(string word, ofstream *output) {
    no *aux = root;
    *output << "[";
    while(true) {
      
      if (aux != nullptr)
      {
        if (aux->word.compare(word) < 0)
        {
          // SE WORD FOR MAIOR
          if (aux == root)
          {
            *output << aux->word;
          } else {
            *output << "->" << aux->word;
          }
          aux = aux->right;

        } else if (aux->word.compare(word) > 0)
        {
          // SE NEWNO FOR MENOR
          if (aux == root)
          {
            *output << aux->word;
          } else {
            *output << "->" << aux->word;
          }
          aux = aux->left;

        } else {
          // SE FOR IGUAL
          if (aux == root)
          {
            *output << aux->word << "]" << endl;
          } else
          {
            *output << "->" << aux->word << "]" << endl;
          }
          *output << aux->synonyms << endl;
          break;
        }
      } else
      {
        if (aux == root)
        {
          *output << "?]" << endl;
        }
        *output << "->?]" << endl;
        *output << "-" << endl;
        break;
      }
    }
  }
  
  int heightNo(no *newNo) {
    if (newNo == nullptr) {
      return -1; // altura da árvore vazia

    } else 
    {
      // BUG AQUI

      int leftHeight = heightNo(newNo->left);
      int rightHeight = heightNo(newNo->right);

      if (leftHeight > rightHeight)
      {
        int altura = leftHeight + 1;
        newNo->height = altura;
        return altura;
      } else {
        int altura2 = rightHeight + 1;
        newNo->height = altura2;
        return altura2;
      }
    }
  }

  /**
  void ordem(no* raiz){

    if(raiz!= 0){

    ordem(raiz->left);
    cout << raiz->word << " ";
    cout << raiz->balance << " pai: ";
    if (raiz!=root){
      cout << raiz->dad->word << endl; 
    }
    ordem(raiz->right);
   }
  }
  */

  void balancingNo(no *newNo) {
    no *aux = newNo;
    while (aux != nullptr)
    {

      // Balanceamento
      int rightH = -1;
      int leftH = -1;

      if(aux->right != nullptr) {
        rightH = aux->right->height;
      }
      if(aux->left != nullptr) {
        leftH = aux->left->height;
      }
      aux->balance = rightH - leftH;

      // Verificar balanceamento
      if(aux->balance == 2) {
        if(aux->right->balance >= 0) {
          rotation_L(aux);

          // Balancear
          heightNo(root);
          if(aux->right !=nullptr) {
            balancingNo(aux->right);
          }
          if(aux->left != nullptr) {
            balancingNo(aux->left);
          }
          if (aux->right == nullptr && aux->left == nullptr)
          {
            balancingNo(aux);
          }
          break;

        } else {
          rotation_R_L(aux);

          // Balancear
          heightNo(root);
          if(aux->right !=nullptr) {
            balancingNo(aux->right);
          }
          if(aux->left != nullptr) {
            balancingNo(aux->left);
          }
          if (aux->right == 0 && aux->left == 0)
          {
            balancingNo(aux);
          }
          break;
        }
      } else if(aux->balance == -2) {
        if(aux->left->balance <= 0) {
          rotation_R(aux);

          // Balancear
          heightNo(root);
          if(aux->right !=nullptr) {
            balancingNo(aux->right);
          }
          if(aux->left != nullptr) {
            balancingNo(aux->left);
          }
          if (aux->right == 0 && aux->left == 0)
          {
            balancingNo(aux);
          }
          break;
        } else {
          rotation_L_R(aux);

          // Balancear
          heightNo(root);
          if(aux->right !=nullptr) {
            balancingNo(aux->right);
          }
          if(aux->left != nullptr) {
            balancingNo(aux->left);
          }
          if (aux->right == 0 && aux->left == 0)
          {
            balancingNo(aux);
          }
          
          break;
        }
      }
      
      // Mudar Auxiliar
      aux = aux->dad;
    }
  }

  void rotation_L(no* raiz ) {
    no* eixo = raiz->right;
    raiz->right = eixo->left;
    if (raiz->right != nullptr)
    {
      raiz->right->side = 1;
      eixo->left->dad = raiz;
    }
    eixo->left = raiz;
    eixo->dad = raiz->dad;
    eixo->side = raiz->side;
    raiz->side = 0;
    raiz->dad = eixo;

    if (eixo->dad != nullptr)
    {  
      if(eixo->side == 0) {
        eixo->dad->left = eixo;
        //cout << "oi papai" << endl;
      } else {
        eixo->dad->right = eixo;
        //cout << "ou mamãe" << endl;
      
      }
    } else
    {
      root = eixo;
    }

    // raiz = eixo;

  }

  void rotation_R(no* raiz ) {
    no* eixo = raiz->left;
    raiz->left = eixo->right;
    if (raiz->left != nullptr)
    {
      raiz->left->side = 0;
       eixo->right->dad = raiz;
    }
    eixo->right = raiz;
    eixo->dad = raiz->dad;
    eixo->side = raiz->side;
    raiz->side = 1;
    raiz->dad = eixo;
    
    if (eixo->dad != nullptr)
    {  
      if(eixo->side == 0) {
        eixo->dad->left = eixo;
      } else {
        eixo->dad->right = eixo;
      }
    } else
    {
      root = eixo;
    }

    // raiz = eixo;
  }

  void rotation_L_R (no* raiz) {
    rotation_L(raiz->left) ;
    rotation_R(raiz);
  }

  void rotation_R_L (no* raiz) {
    rotation_R(raiz->right) ;
    rotation_L(raiz);
  }

} tree;


int main(int argc, char *argv[])
{

	// open files
	ifstream input;
	ofstream output;

	input.open(argv[1]);
	output.open(argv[2]);

  tree *searchTree = new tree;

  int wordsNum;
  int synonymsNum;
  string word;
  string synonym;

  // GET
  input >> wordsNum;
  //cout << wordsNum << endl;
  for (int i = 0; i < wordsNum; i++)
  {
    no *newNo = new no;
    input >> word >> synonymsNum;
    //cout << word << " " << synonymsNum << endl;
    string synonyms = "";
    for (int i = 0; i < synonymsNum; i++)
    {
      input >> synonym;
      //cout << synonym << " ";
      if (i == 0)
      {
        synonyms += synonym;
      } else
      {
        synonyms += ("," + synonym);
      }
    }
    //cout << endl;
    newNo->word = word;
    newNo->synonyms = synonyms;
    searchTree->addNo(newNo);
  }

  // SHOW
  int searchNum;
  string word2Search;
  input >> searchNum;
  //cout << searchNum << endl;

  for (int i = 0; i < searchNum; i++)
  {
    input >> word2Search;
    //cout << word2Search << endl;
    searchTree->search(word2Search, &output);
  }
  
  // close files
	input.close();
	output.close();
	
	return 0;
}