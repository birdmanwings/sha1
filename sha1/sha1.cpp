#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

// 循环左移,处理32位的数据块
unsigned circleShift(const unsigned& word, const int& bits) {
	return (word << bits) | ((word) >> (32 - bits));  
}

// 非线性函数
unsigned sha1Fun(const unsigned& B, const unsigned& C, const unsigned& D, const unsigned& t) {

	switch (t / 20) {
	case 0:     return (B & C) | ((~B) & D);
	case 2:     return (B & C) | (B & D) | (C & D);
	case 1:
	case 3:     return B ^ C^ D;
	}

	return t;
}

string sha1(const string& strRaw) {

	string str(strRaw);
	//cout << str;
	// 补一个1后面填0
	str += (unsigned char)(0x80);
	cout << str;
	// 每个字节8位,所以要乘8,左移3位
	while (str.size() << 3 % 512 != 448) {
		str += (char)0;
	}
	cout << str.size();

	// 写入原始数据长度,只有当i等于0的时候，才是左移三位即乘8后的长度，前面都是补0,注意这里strRaw.size()返回的是size_t，是32位的，这里最高移动了56位，所以要将其转换为64位
	for (int i(56); i >= 0; i -= 8) {
		str += (unsigned char)((((unsigned __int64)strRaw.size()) << 3) >> i);
	}

	const unsigned K[4] = { 0x5a827999,0x6ed9eba1,0x8f1bbcdc,0xca62c1d6 };//参数K
	unsigned A(0x67452301), B(0xefcdab89), C(0x98badcfe), D(0x10325476), E(0xc3d2e1f0), T(0);//参数ABCDE
	unsigned W[80] = { 0 };//缓冲区

	// 每次处理64字节,共512位
	for (unsigned i(0); i != str.size(); i += 64) {
		// 前16个字为原始数据
		for (unsigned t(0); t != 16; ++t) {
			// 将4个8位数据放入一个32位变量中
			W[t] = ((unsigned)str[i + 4 * t] & 0xff) << 24 |
				((unsigned)str[i + 4 * t + 1] & 0xff) << 16 |
				((unsigned)str[i + 4 * t + 2] & 0xff) << 8 |
				((unsigned)str[i + 4 * t + 3] & 0xff);
		}

		// 将16块填充到80块
		for (unsigned t(16); t != 80; ++t) {
			W[t] = circleShift(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
		}

		// 进行80轮计算
		for (unsigned t(0); t != 80; ++t) {
			T = circleShift(A, 5) + sha1Fun(B, C, D, t) + E + W[t] + K[t / 20];
			E = D;
			D = C;
			C = circleShift(B, 30);
			B = A;
			A = T;
		}

		A += 0x67452301;
		B += 0xefcdab89;
		C += 0x98badcfe;
		D += 0x10325476;
		E += 0xc3d2e1f0;
	}

	stringstream ss;
	ss << setw(8) << setfill('0') << hex << A << B << C << D << E;
	ss >> str;

	return str;
}

int main(int argc, char* argv[]) {

	string str("");

	cout << "in put a string :" << endl;
	getline(cin, str);

	cout << "raw  string: " << str << endl
		<< "sha1 encode: " << sha1(str) << endl;

	return 0;
}
