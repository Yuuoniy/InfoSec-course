#include "md5.h"
#include <iostream>

using namespace std;

int main(){
  string plain;
  cout << "Please input the string:" << endl;
  cin >> plain;
  md5 m(plain);
  return 0;
}
