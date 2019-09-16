#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

using namespace std;

typedef struct server {

  int num = 0;
  string history = "";

} server;

int checksum(string *text) {
  int total = 0;

  for(const auto& letter : *text) {
    if (total == 0)
    {
      total = int(letter);
    } else {
      total = total ^ int(letter);
    }
    
  }
  return total;

};

int main(int argc, char *argv[])
{

	// open files
	ifstream input;
	ofstream output;
	input.open(argv[1]);
	output.open(argv[2]);

  // GET servers
  int serversNum;
  int serverMax;

  input >> serversNum >> serverMax;
  cout << serversNum << endl;
  server* serversList = new server[serversNum];

  // GET texts and operate it
  int qtd;
  int textCount;
  int total;
  uint32_t h1;
  uint32_t h2 = 100;
  uint32_t doubleH;
  uint32_t lastDoubleH;
  uint32_t i;
  string text;
  string textForHistory = "";

  input >> qtd;

  for (int g = 0; g < qtd; g++)
  {
    i = 0;
    input >> textCount;
    total = 0;
    textForHistory = "";

    for (int j = 0; j < textCount; j++)
    {
      input >> text;
      total = total ^ checksum(&text);
      textForHistory += (" " + text);
    }

    // DOUBLE HASHING CALCULATE
    h1 = (7919 * total);
    h2 = (104729 * total + 123);
    doubleH = (h1 + i*h2) % serversNum;
    lastDoubleH = doubleH;

    // COLISION WHILE
    while (serversList[doubleH].num == serverMax)
    {
      i++;
      doubleH = (h1 + i*h2) % serversNum;

    }

    if (i > 0)
    {
      output << "S" << lastDoubleH << "->S" << doubleH << endl;
    }
    
    serversList[doubleH].num += 1;


    // CONCAT
    if (serversList[doubleH].num == 1)
    {
      serversList[doubleH].history += textForHistory;

    } else {
      textForHistory = ","+textForHistory;
      serversList[doubleH].history += textForHistory;

      }

      output << "[S" << doubleH << "]" << serversList[doubleH].history << endl;
    }
  

	// close files
	input.close();
	output.close();
	
	return 0;
}