#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

using namespace std;

typedef struct book {

  long long isbn;
  string autor = "";
  string title = "";

} book;

int buscaBinaria(book *books, long long isbn, int n, book *bookQuery) {

  int passos = 1;
  int i = 0 , j = n - 1;
  int p = ( i + j ) / 2;

  // Iterações de 1 -> k
  while ( j >= i && books[p].isbn != isbn ) {
    if( books[p].isbn > isbn ) {
      j = p - 1;
    } else {
      i = p + 1;
    }
    
    p = ( i + j ) / 2;
    passos += 1;

  }

  if (books[p].isbn == isbn)
  {
    bookQuery -> isbn = books[p].isbn;
    bookQuery -> autor = books[p].autor;
    bookQuery -> title = books[p].title;
  } else {
    bookQuery -> isbn = -1;
  }
  return passos;
}

int buscaInterpolada(book *booksL, long long isbn, int n) {

  int passos = 1;
  int i = 0 , j = n - 1;
  int p = i + ((booksL[j].isbn - booksL[i].isbn) % (j-i+1));

  // Iterações de 1 -> k
  while ( j >= i && booksL[p].isbn != isbn ) {
    if( booksL[p].isbn > isbn ) {
      j = p - 1;
    } else {
      i = p + 1;
    }
    
    if (j >= i && j >= 0 && i >= 0)
    {
      p = i + ((booksL[j].isbn - booksL[i].isbn) % (j-i+1));
      passos += 1;
    }

  }

  return passos;

}

int main(int argc, char *argv[])
{
  // Open files
	ifstream input;
	ofstream output;
	input.open(argv[1]);
	output.open(argv[2]);

  // Get books
  int booksNum;
  input >> booksNum; 
  book* booksList = new book[booksNum];
  string linha;

  for (int i = 0; i < booksNum; i++)
  {
    bool isAutor = true;
    input >> booksList[i].isbn;
    input.ignore();
    getline(input, linha);

    for(const auto& letter : linha) {
      if (letter == '&')
      {
        isAutor = false;
      } else
      {
        if (isAutor)
        {
          booksList[i].autor += letter;
        } else
        {
          booksList[i].title += letter;
        }   
      }
    }
  }

  int queriesNum;
  long long isbnToQuery;
  book *bookQuery = new book;
  int bPassos;
  int iPassos;
  int bSum = 0;
  int iSum = 0;
  int bVictory = 0;
  int iVictory = 0;

  input >> queriesNum;

  for (int i = 0; i < queriesNum; i++)
  {
    input >> isbnToQuery;
    bPassos = buscaBinaria(booksList, isbnToQuery, booksNum, bookQuery);
    iPassos = buscaInterpolada(booksList, isbnToQuery, booksNum);
    bSum += bPassos;
    iSum += iPassos;
    if (bookQuery->isbn == -1)
    {
      output << "[" << isbnToQuery << "]B=" << bPassos << ",I=" << iPassos << ":ISBN_NOT_FOUND" << endl;
    } else {
      output << "[" << isbnToQuery << "]B=" << bPassos << ",I=" << iPassos << ":Author:" << bookQuery->autor << ",Title:" << bookQuery->title << endl;
    }
    
    if (iPassos <= bPassos)
    {
      iVictory++;
    } else {
      bVictory++;
    }
    
  }
  output << "BINARY=" << bVictory << ":" << bSum/queriesNum << endl;
  output << "INTERPOLATION=" << iVictory << ":" << iSum/queriesNum << endl;


  // Close files
  input.close();
  output.close();


}