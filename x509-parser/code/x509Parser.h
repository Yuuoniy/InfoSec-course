#ifndef _PARSER_
#define _PARSER_
#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <fstream>
typedef unsigned char BYTE;
using namespace std;
//数据库
typedef struct
{
  int tag;
  int length;
  string value;
} tlv;

class x509Parser{
  private:
    string rawData;
    vector<BYTE> byteData;
    vector<tlv> parseTlv;//解析的结果 tlv 结构
    public: 
     x509Parser(string filename);
    vector<BYTE> base642Byte(string base64str);
    void display();//输出最终结果
    void process();
    void parseAns1(vector<BYTE> data,int begin,int end);
    void OIDpraser();
    void printTime(string time);
};
#endif
