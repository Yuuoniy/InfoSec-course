#include "md5.h"

unsigned int T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
    0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
    0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
    0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
    0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
    0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

const unsigned char s[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
             5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
             4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
             6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

const unsigned char X[4][16] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
                                                {1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12},
                                                {5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2},
                                                {0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9}
};



md5::md5(string _plain){
  //初始化寄存器
  A = 0x67452301;
  B = 0xEFCDAB89;
  C = 0x98BADCFE;
  D = 0x10325476;
  count = 0;
  for(int i = 0; i < _plain.size(); i++){
    plain.push_back(_plain[i]);
  }
  process();
}

void md5::process()
{
  while(plain.size()>=64){
    for(int i = 0; i < 16; i++)
    {//四个 bytes 组成 一个 int
      block[i] = ((unsigned int)plain[3] << 24)+((unsigned int)plain[2] << 16) + ((unsigned int)plain[1] << 8) + ((unsigned int)plain[0]);
      plain.erase(plain.begin(),plain.begin()+4);
      count +=32;
    }
    compress();
  }
  //多余的进行 padding
  count += plain.size()*8;
  padding();
  display();
}

void md5::padding(){
  if(plain.size()<56){
    plain.push_back(0x80); //填充 1
  }
  //填充 0 至 448 bit
  while(plain.size()<56){
    plain.push_back(0);
  }
  //附加 k 值的低64位
  for(int i=0;i<8;i++){
    plain.push_back((count>>i*8)%(1<<8));
  }

  for(int i=0;i<16;i++){
    block[i] = ((unsigned int)plain[3] << 24) + ((unsigned int)plain[2] << 16) + ((unsigned int)plain[1] << 8) + ((unsigned int)plain[0]);
    plain.erase(plain.begin(), plain.begin() + 4);
  }
  compress();
}

void md5::compress(){
  unsigned int cv[4];
  cv[0] = A; cv[1] = B; cv[2] = C; cv[3] = D;
  unsigned int a;
  //迭代
  for(int j=0;j<4;j++){
    for(int i=0;i<16;i++){
      a = A;
      a += block[X[j][i]] + T[j*16+i];
      switch (j){
        case 0:
          a += ((B & C) | ((~B) & D));
          break;
        case 1:
          a += (((B) & (D)) | ((C) & (~D)));
          break;
        case 2:
          a += (((B) ^ (C)) ^ (D));
          break;
        case 3:
          a += (C ^ (B | (~D)));
          break;
        default:
          break;
      }
      //循环左移
      a = (a << s[j * 16 + i] | a >> (32 - s[j * 16 + i])) + B;
      //循环轮换
      A = D;
      D = C;
      C = B;
      B = a;
    }
  }
  A += cv[0];
  B += cv[1];
  C += cv[2];
  D += cv[3];
}

void md5::display(){
  unsigned int result[16];
  for(int i = 0; i < 4; i++)
  {
    result[i] = (A>>i*8)&0xFF;
    result[i+4] = (B>>i*8)&0xFF;
    result[i+8] = (C>>i*8)&0xFF;
    result[i+12] = (D>>i*8)&0xFF;
  }
  std::ostringstream ss;
  ss << std::hex;
  for (int i = 0; i < 16; i++)
  {
    ss << (int)result[i];
  }
  cout << ss.str() << endl;
}
