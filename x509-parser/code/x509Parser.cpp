#include "x509Parser.h"

static const char to_base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz"
                                "0123456789+/";

static const unsigned char from_base64[] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62,  255,
    62,  255, 63,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  255, 255,
    255, 255, 255, 255, 255, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
    10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
    25,  255, 255, 255, 255, 63,  255, 26,  27,  28,  29,  30,  31,  32,  33,
    34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,  255, 255, 255, 255, 255};
// OID 映射

map<string, string> OID2Algorithm = {{"1.2.840.10040.4.1", "DSA"},
                                     {"1.2.840.10040.4.3", "sha1DSA"},
                                     {"1.2.840.113549.1.1.1", "RSA"},
                                     {"1.2.840.113549.1.1.2", "md2RSA"},
                                     {"1.2.840.113549.1.1.3", "md4RSA"},
                                     {"1.2.840.113549.1.1.4", "md5RSA"},
                                     {"1.2.840.113549.1.1.5", "sha1RSA"},
                                     {"1.3.14.3.2.29", "sha1RSA"},
                                     {"1.2.840.113549.1.1.13", "sha512RSA"},
                                     {"1.2.840.113549.1.1.11", "sha256RSA"}};

map<string, string> OID2Info = {{"2.5.4.6", "C"},  {"2.5.4.10", "O"},
                                {"2.5.4.3", "CN"}, {"2.5.4.8", "S"},
                                {"2.5.4.7", "L"},  {"2.5.4.6", "C"}};
map<string, string> OID2string = {
    {"1.3.6.1.5.5.7.3.1", "serverAuth"},
    {"1.3.6.1.5.5.7.3.2", "clientAuth"},
    {"2.5.29.37", "extKeyUsage"},
    {"2.5.29.31", "CRL Distribution Points"},
    {"1.2.840.10045.2.1", "EC Public Key"},
    {"2.5.29.17", "subjectAltName"},
    {"1.3.6.1.5.5.7.48.2", "id-ad-caIssuers"},
    {"1.3.6.1.5.5.7.48.1", "OCSP"},
    {"2.5.4.1", "aliasedEntryName"},
    {"1.3.6.1.5.5.7.3.1", "serverAuth"},
    {"2.5.29.15", "keyUsage"},
    {"2.5.29.14", "subjectKeyIdentifier"},
    {"2.5.29.15", "keyUsage"},
    {"2.5.29.16", "privateKeyUsagePeriod"},
    {"2.5.29.17", "subjectAltName"},
    {"2.5.29.18", "issuerAltName"},
    {"2.5.29.19", "basicConstraints"},
    {"2.5.29.20", "cRLNumber"},
    {"2.5.29.21", "cRLReason"},
    {"2.5.29.22", "expirationDate"},
    {"2.5.29.23", "instructionCode"},
    {"2.5.29.24", "invalidityDate"},
    {"2.5.29.25", "cRLDistributionPoints"},
    {"2.5.29.35", "authorityKeyIdentifier"},
    {"2.5.29.36", "policyConstraints"},
    {"2.5.29.37", "extKeyUsage"},
    {"2.5.29.37.0", "anyExtendedKeyUsage"},
    {"2.5.29.38", "authorityAttributeIdentifier"},
    {"2.5.29.39", "roleSpecCertIdentifier"},
    {"2.5.29.40", "cRLStreamIdentifier"},
    {"2.5.29.41", "basicAttConstraints"},
    {"2.5.29.42", "delegatedNameConstraints"},
    {"1.3.6.1.5.5.7.1.1", "authorityInfoAccess"},
    {"1.3.6.1.5.5.7.1.2", "biometricInfo"},
    {"1.3.6.1.5.5.7.1.3", "qcStatements"},
    {"1.3.6.1.5.5.7.1.4", "acAuditIdentity"},
    {"1.3.6.1.5.5.7.1.5", "acTargeting"},
    {"1.3.6.1.5.5.7.1.6", "acAaControls"},
    {"1.3.6.1.5.5.7.1.7", "ipAddrBlocks"},
    {"1.3.6.1.5.5.7.1.8", "autonomousSysIds"},
    {"1.3.6.1.5.5.7.1.9", "routerIdentifier"},
    {"1.3.6.1.5.5.7.1.10", "acProxying"},
    {"1.3.6.1.5.5.7.1.11", "subjectInfoAccess"},
    {"1.3.6.1.5.5.7.1.12", "logoType"},
    {"1.3.6.1.5.5.7.1.13", "wlanSSID"},
    {"1.3.6.1.5.5.7.48.2", "caIssuers"},
    {"1.3.6.1.5.5.7.48.3", "timeStamping"},
    {"1.3.6.1.5.5.7.48.4", "dvcs"},
    {"1.3.6.1.5.5.7.48.5", "caRepository"},
    {"1.3.6.1.5.5.7.48.7", "signedObjectRepository"},
    {"1.3.6.1.5.5.7.48.10", "rpkiManifest"},
    {"1.3.6.1.5.5.7.48.11", "signedObject"},
    {"2.5.29.27", "deltaCRLIndicator"},
    {"2.5.29.28", "issuingDistributionPoint"},
    {"2.5.29.29", "certificateIssuer"},
    {"2.5.29.30", "nameConstraints"},
    {"2.5.29.31", "cRLDistributionPoints"},
    {"2.5.29.32", "certificatePolicies"},
    {"2.5.29.32.0", "anyPolicy"},
    {"2.5.29.33", "policyMappings"},
    {"1.2.840.10045.3.1.3", "prime192v3"},
    {"1.2.840.10045.3.1.4", "prime239v1"},
    {"1.2.840.10045.3.1.5", "prime239v2"},
    {"1.2.840.10045.3.1.6", "prime239v3"},
    {"1.2.840.10045.3.1.7", "prime256v1"}

};
x509Parser::x509Parser(string filename) {
  ifstream in(filename);
  char buffer[1024];
  while (!in.eof()) {
    in.getline(buffer, 1024);
    if (string(buffer).find("END CERTIFICATE") != -1) {
      process();
    } else if (string(buffer).find("BEGIN CERTIFICATE") == -1) {
      rawData += string(buffer).substr(0, string(buffer).length());
    }
  }
}

void x509Parser::process() {
  byteData = base642Byte(rawData);
  //解析 ASC.1
  parseAns1(byteData, 0, byteData.size());
  display();
}
//递归解析

void x509Parser::parseAns1(vector<BYTE> data, int begin, int end) {
  int p = begin;
  int tag;
  string value;
  int len = 0;
  int lenBytes = 0;
  while (p < end) {
    tag = data[p]; //获取 tag
    p++;
    len = 0;
    lenBytes = 1;
    if (data[p] & 0x80) /*判断最高是否为1*/
    {
      lenBytes = data[p] & 0x7f; //长度使用的字节数量
      p++;
      for (int j = 0; j < lenBytes; j++) {
        len <<= 8;
        // printf("%.2X ", int(data[p + j]));
        len += int(data[p + j]);
      }
    } else { //最高为0，直接取长度值
      len = int(data[p]) & 0x7f;
    }
    if (p + lenBytes <= end) {
      p += lenBytes;
    }
    //接下来解析数据,这里的tag 包含了第一字节的所有数据

    if (tag == 48 || tag == 49 || tag == 8) // SET SEQUENCE OCTET_STRING
    {
      parseAns1(data, p, p + len);
    } else if (tag == 1) // BOOLEAN
    {
      int val = 0xff;
      for (int i = 0; i < len; i++) {
        val &= data[i + p];
      }
      parseTlv.push_back({tag, len, val == 0 ? "False" : "True"});
    } else if (tag == 2 || tag == 128) {
      char tmp[3];
      long long num;
      for (size_t i = 0; i < len; i++) {
        sprintf(tmp, "%02x", int(data[i + p]));
        value += string(tmp);
      }
      parseTlv.push_back({tag, len, value});
    } else if (tag == 160) { // CONTEXT_SWPECIFIC 证书版本
      parseTlv.push_back({tag, len, "Version"});
      parseAns1(data, p, p + len);
    } else if (tag == 163) { // CONTEXT_SPECIFIC 用于拓展字段
      parseTlv.push_back({tag, len, "Extension"});
      parseAns1(data, p, p + len);
    } else if (tag == 6) { // OBJECT_IDENTIFIER OID 解析
      int firstval = min((data[p] & 0x7f) / 40, 2);
      int secondval = data[p] & 0x7f - 40 * firstval;
      value.append(to_string(firstval) + "." +
                   to_string(secondval)); //解析第一个 byte
      //读取接下来字节，并且根据最高位判断是否为结束块
      firstval = 0;
      for (size_t i = 1; i < len; i++) {
        firstval = (firstval << 7);
        firstval += int(data[i + p]) & 0x7f;
        if (!(int(data[p + i]) & 0x80)) //如果最高位是0 说明是一独立的块
        {
          value.append("." + to_string(firstval));
          firstval = 0;
        }
      }
      parseTlv.push_back({tag, len, value});
    } else if (tag == 23 || tag == 22 || tag == 19 || tag == 134 ||
               tag == 12) { // UTCTime IA5String  PrintableString
                            // CONTEXT_SPECIFIC (6) UTF8String
      if (tag == 23) {
        value = "UTCTime ";
      }
      for (size_t i = 0; i < len; i++) {
        value += (char)data[i + p];
      }
      parseTlv.push_back({tag, len, value});
    } else if (tag == 3) { // BIT_STRING
      char tmp[3];
      for (size_t i = 0; i < len - 1; i++) {
        sprintf(tmp, "%02x", int(data[i + p + 1]));
        value += string(tmp);
      }
      parseTlv.push_back({tag, len, value});
    }
    value = "";
    p += len;
  }
}

vector<BYTE> x509Parser::base642Byte(string base64str) {
  // padding
  while (base64str.size() % 4 != 0) {
    base64str.push_back('=');
  }
  size_t encoded_size = base64str.size();
  std::vector<unsigned char> ret;
  ret.reserve(3 * encoded_size / 4);

  for (size_t i = 0; i < encoded_size; i += 4) {
    unsigned char b4[4];
    b4[0] = from_base64[base64str[i + 0]];
    b4[1] = from_base64[base64str[i + 1]];
    b4[2] = from_base64[base64str[i + 2]];
    b4[3] = from_base64[base64str[i + 3]];

    unsigned char b3[3];
    b3[0] = ((b4[0] & 0x3f) << 2) + ((b4[1] & 0x30) >> 4);
    b3[1] = ((b4[1] & 0x0f) << 4) + ((b4[2] & 0x3c) >> 2);
    b3[2] = ((b4[2] & 0x03) << 6) + ((b4[3] & 0x3f));

    ret.push_back(b3[0]);
    ret.push_back(b3[1]);
    ret.push_back(b3[2]);
  }

  return ret;
}

void x509Parser::printTime(string str) {
  str = str.substr(8, str.length() - 9);
  cout << "20" << str[0] << str[1] << "-";
  cout << str[2] << str[3] << "-";
  cout << str[4] << str[5] << "-";
  cout << str[6] << str[7] << ":";
  cout << str[8] << str[9] << ":";
  cout << str[10] << str[11];
}

void x509Parser::display() {
  for (size_t i = 0; i < parseTlv.size(); i++) {
    tlv chunk = parseTlv[i];
    if (chunk.value == "Version") { //输出版本
      chunk = parseTlv[++i];
      if (chunk.tag == 2) {
        cout << "Version: " << std::stoi(chunk.value) + 1 << endl;
        chunk = parseTlv[++i];
        cout << "Serial Number  " << chunk.value << endl;
      } else {
        --i;
      }
    } else if (OID2Algorithm.find(chunk.value) !=
               OID2Algorithm.end()) //输出算法
    {
      if (parseTlv[i + 1].tag == 3) //有公钥的
      {
        cout << "Public Key Algorithm:" << OID2Algorithm[chunk.value] << endl;
        cout << "Public - Key : (2048 bit)" << endl;
        chunk = parseTlv[++i];
        cout << chunk.value << endl;
      } else {
        cout << "Signature Algorithm: " << OID2Algorithm[chunk.value] << endl;
        cout << "Issuer: ";
      }
    } else if (OID2Info.find(chunk.value) != OID2Info.end()) {
      cout << " " << OID2Info[chunk.value] << "=";
      chunk = parseTlv[++i];
      cout << chunk.value;
    } else if (OID2string.find(chunk.value) != OID2string.end()) {
      // if (chunk.value == "1.2.840.10045.2.1")
      //   cout << endl;
      cout << OID2string[chunk.value] << ":";
      chunk = parseTlv[++i];
      while (OID2string.find(chunk.value) != OID2string.end()) {
        cout << "\n    " << OID2string[chunk.value] << ":";
        chunk = parseTlv[++i];
      }
      cout << chunk.value << endl;
      ;
    } else if (chunk.value.find("UTCTime") != string::npos) { //输出有效期
      cout << "\nValidity" << endl;
      cout << "   Not Before: ";
      printTime(chunk.value);
      cout << endl;
      chunk = parseTlv[++i];
      cout << "   Not After :";
      printTime(chunk.value);
      cout << endl;
      cout << "Subject: ";
    }
  }
}
