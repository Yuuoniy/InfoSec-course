#ifndef DES
#define DES
#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
using namespace std;

class des{
  private:
  vector<unsigned char> data;
  vector<unsigned char> result;
  vector<unsigned char> inputKey;
  bitset<64> key;
  int mode;
  int len;
  const static unsigned char PC1[];
  const static unsigned char PC2[];
  const static unsigned char IP[];
  const static unsigned char IpInverse[];
  const static unsigned char sBox[8][4][16];
  const static unsigned char eBox[];
  const static unsigned char pBox[];
  bitset<48> subKeys[16];

public:
  des();
  vector<unsigned char> desProcess(vector<unsigned char> data, vector<unsigned char> key, int mode);
  bitset<64> TIteration(bitset<64> &dataBit); //T 迭代
  void subKeyGenerated();    //子密钥生成
  bitset<32> sBoxTransform(bitset<48> &R); //s 盒变换
  void PKCS5Padding();       //字节填充
  bitset<32> feistel(bitset<32> R, bitset<48> key); // F 轮函数
  void leftShift(); //左移
  bitset<64> encrypt(bitset<64> &plain); // 加密
  void decrypt(); // 解密
  void transform(bitset<64>& data, int originSize, const int resSize, const unsigned char *table);
  void charToBits(vector<unsigned char> &str, bitset<64> &bits);
  void bitsToChar(bitset<64> &bits, vector<unsigned char> &str);
};

#endif 
