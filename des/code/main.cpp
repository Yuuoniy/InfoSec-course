#include "des.h"

int main(){
  vector<unsigned char>data,key;
  string dataStr,keyStr;
  int mode = 1;
  dataStr = "helloworld";
  keyStr = "sysucsas";
  // cout << "Please input your data:" << endl;
  // cin >> dataStr;
  // cout << "Please input your key:" << endl;
  // cin >> keyStr;
  // cout << "Please input your mode(0 for decrypt,1 for encrypt)" << endl;
  // cin >> mode;
  for(int i = 0; i < dataStr.length(); i++)
  {
    data.push_back(dataStr[i]);
  }
  
  for(int i = 0; i < keyStr.length(); i++)
  {
    key.push_back(keyStr[i]);
  }
des desOject;
vector<unsigned char>cipher =  desOject.desProcess(data, key, 1);
cout << "encrypt result:";
for (int i = 0; cipher[i] != '\0' && i < cipher.size(); i++)
  cout << cipher[i];
cout << endl;

vector<unsigned char>plain =  desOject.desProcess(cipher, key, 0);

cout  <<"decrypt result:";
for (int i = 0; plain[i] != '\0' && i < plain.size(); i++)
  cout << plain[i];
cout << endl;
return 0;
}
