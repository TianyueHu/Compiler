#pragma once

/*词法分析器
读入程序代码
分析出Toke*/
#include "NameTable.h"
#include "main.h"

class Scanner
{
private:
	size_t row_counter;//行计数器
	size_t col_counter;//列计数器
	ifstream ifs;	//源代码文件
	ofstream ofs;	//token写出文件
	string buffer;	//行缓冲区
	
	
	//bool newProFlag;
	//string nowFuncName;
public:
	NameTable nameTable;
	vector<shared_ptr<struct tokenRecord>> tokenVector;

	Scanner();
	~Scanner();

	void OpenFile(string filename);
	bool ReadNewLine();
	void scanner();
	void Sort(char ch);		//单词分类
	void Num(char ch);		//数值处理函数
	void Identify(char ch);	//标识符处理函数
	void String(char ch);	//字符串处理函数
	void Ch(char ch);		//字符处理函数
	//void newToken(enum tokenType token);
	void newToken(enum tokenType token, char ch);
	void newToken(enum tokenType token, char* name);

	void SetRowCounter(size_t value);
	void SetColCounter(size_t value);

	size_t GetRowCounter();
	size_t GetColCounter();
};

