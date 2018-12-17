#include "des.h"

const unsigned char des::IP[] = {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36,
                                 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64,
                                 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17,
                                 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45,
                                 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};

const unsigned char des::IpInverse[] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15,
                                        55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37,
                                        5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20,
                                        60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42,
                                        10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};

const unsigned char des::PC1[] = {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
                                  10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63,
                                  55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6,
                                  61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};

const unsigned char des::PC2[] = {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
                                  23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41,
                                  52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49,
                                  39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

const unsigned char des::eBox[] = {32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
                                   8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16,
                                   17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25,
                                   26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
const unsigned char des::pBox[] = {16, 7, 20, 21, 29, 12, 28, 17, 1, 15,
                                   23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32,
                                   27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};

const unsigned char des::sBox[8][4][16]{
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 15, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
    },
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {12, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
    },
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
    },
};

des::des()
{
}

vector<unsigned char> des::desProcess(vector<unsigned char> _data, vector<unsigned char> _key, int _mode){
  this->data = _data;
  this->inputKey = _key;
  this->mode = _mode;
  //判断 key 长度是否合法
  if (_key.size() != 8)
  {
    cout << "Invalid key" << endl;
    return result;
  }
  if (mode == 1)
  { //encrypt
    len = data.size() + 8 - data.size() % 8;
    PKCS5Padding();
  }
  else
  {
    len = data.size();
  }
  result.resize(len);
  // 生成16个子密钥
  charToBits(inputKey, this->key);
  subKeyGenerated(); //生成子密钥
  //padding
  int times = this->data.size() / 8;
  //分块进行那个加密，每块8字节
  for (int i = 0; i < times; i++)
  {
    vector<unsigned char> rawData(8);
    for (int j = 0; j < 8; j++)
      rawData[j] = data[i * 8 + j];
    bitset<64> dataBit,processBit,cipher;
    charToBits(rawData, dataBit); //转为64 位
    //IP 变换
    for (int i = 0; i < 64; ++i)
      processBit[63 - i] = dataBit[64 - IP[i]];
    //T 迭代 和 W 置换
    TIteration(processBit);
    // IP 逆置换
    cipher = processBit;
    for (int i = 0; i < 64; ++i)
      cipher[63 - i] = processBit[64 - IpInverse[i]];

    //从位转化为字节
    bitsToChar(cipher, rawData);
    // for_each(rawData.begin(), rawData.end(), fun);
    for (int j = 0; j < 8; j++)
      result[i * 8 + j] = rawData[j];
  }
  //填充删除PKCS5填充
  if (mode == 0)
  {
    result[len - result[len - 1]] = '\0';
  }
  return result;
}

void des::transform(bitset<64> &data, int originSize, const int resSize, const unsigned char *table)
{
  bitset<64> dataCopy = data;
      // memcpy(dataCopy, data, (originSize + 1) * sizeof(unsigned char));
      // memset(data, 0, originSize * sizeof(resSize));
  for (int i = 0; i < resSize; i++)
  {
    data[resSize-1-i] = dataCopy[originSize-table[i]];
  }
}


void des::PKCS5Padding(){
  int originLen = data.size();
  int count = len-originLen; //需要填充的字节数
  //填充
  for (int i = 0; i < count; i++)
  {
    data.push_back(count);
  }
}

void des::subKeyGenerated(){
  bitset<56> realKey;
  // PC1 置换
  for(int i =0;i<56;i++)
    realKey[55-i] = key[64]-PC1[i];
  //生成子密钥
  for (int i = 0; i < 16; i++)
  {
    if (i != 0 && i != 1 && i != 8 && i != 15)
      leftShift();
    leftShift();
    bitset<64> keyCopy;
    for(int i=0;i<64;i++)
      keyCopy[i] = key[i];
    // PC2 变换
    for(int i=0;i<48;i++){
      key[47 - i] = keyCopy[56 - PC2[i]];
    }
    for (int j = 1; j <= 48; j++)
      subKeys[i][j] = key[j];
    key = keyCopy;
  }
}

// key 左移，分为前后两部分 
void des::leftShift(){
 unsigned char chead = key[0],dhead = key[28];
 for(int i = 0; i < 27; i++)
 {
   key[i] = key[i+1];
   key[i+28] = key[i+28+1];
 }
 key[27] = chead;
 key[55] = dhead;
}

bitset<64> des::TIteration(bitset<64>&dataBit){
  bitset<32> left;
  bitset<32> right;
  bitset<32> newLeft;
  bitset<64> reslut;
  for (int i = 32; i < 64; ++i)
    left[i - 32] = dataBit[i];
  for (int i = 0; i < 32; ++i)
    right[i] = dataBit[i];
  for(int i = 0; i < 16; i++)
  {
    newLeft = right;
    if(mode==1)//加密
      right = left ^ feistel(right, subKeys[i]);
    else if(mode==0)
      right = left^feistel(right,subKeys[15-i]);
    left = newLeft;
  }
  // 合并L16 和 R16
  for(int i = 0; i < 32; i++)
  {
    reslut[i]=left[i];
    reslut[i+32] = right[i];
  }
  dataBit = reslut;
}


void des::charToBits(vector<unsigned char>& str, bitset<64>& bits){
  for (int i = 0; i < 8; ++i)
  {
    for (int j = 0; j < 8; ++j)
    {
      bits[i * 8 + j] = ((str[i] >> j) & 1);
    }
  }
}

void des::bitsToChar(bitset<64> &bits, vector<unsigned char> &str)
{
  for (int i = 0; i < 8; ++i)
  {
    str[i] = 0;
    for (int j = 0; j < 8; ++j)
    {
      str[i] += bits[i*8+j]  << j;
    }
  }
}

bitset<32> des::feistel(bitset<32> R, bitset<48> key)
{
  bitset<48> expandR;
  //E 拓展
  for(int i=0;i<48;i++){
    expandR[47-i] = R[32-eBox[i]];
  }
 //异或
  expandR = expandR^key;
 // s 盒变换返回32位的串
  bitset<32> res = sBoxTransform(expandR);
  bitset<32> tmp = res;
  for(int i=0;i<32;i++)
    res[31-i] = tmp[32-pBox[i]];
  return res;
  // p 置换
}

// R平均分成8个分组，每个分组长度6位。各个 分组分别经过8个不同的 S - 盒进行 6 - 4 转换，得到8个长度分别 为4位的分组；
bitset<32> des::sBoxTransform(bitset<48> &src)
{
  bitset<32> result;
  for(int i =0;i<8;i++){
    int begin = 6*i,row = src[begin]*5+src[begin+5],
    col = src[begin+1]*8+src[begin+2]*4+src[begin+3]*2+src[begin+4];
   int  s = sBox[i][row][col];
    //转化为二进制
    bitset<4> bin(s);
    for(int j=0;j<4;j++){
      result[i*4+4-j] = bin[j];
    }
  }
  return result;
}
