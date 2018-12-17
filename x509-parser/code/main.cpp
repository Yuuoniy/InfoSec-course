#include <iostream>
#include "x509Parser.h"
using namespace std;
int main(int argc, char const *argv[])
{
  string filename;
  if (argc > 1)
  {
    filename = string(argv[1]);
  }else{
    cout << "Please input filename" << endl;
    cin >> filename;
  }
  x509Parser parser(filename);
}
