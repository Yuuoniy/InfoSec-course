#ifndef _MD5_
#define _MD5_
#include <string>
#include <iostream>
#include <bitset>
#include <sstream>
#include <vector>
using namespace std;
//定义轮函数


class md5{
  public:
    md5(string _plain);
    void padding(); //填充
    void compress();//循环压缩
    void display();//显示结果
    void process();
  private:
    vector<unsigned char> plain;
    unsigned int A,B,C,D;//初始化的四个寄存器
    unsigned int block[16]; //当前处理的信息块 32*16 = 512 bit
    long long count;

};


#endif
