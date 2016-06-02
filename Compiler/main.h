#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <map>

#define t_len 44
#define variSrt 50
using namespace std;
enum tokenType {
	EMPTY,
	ID,
	INTEGER,
	REAL,
	STRING,
	CH,
	ARRAY,
	FUNC,
	NOT,
	AND,
	OR,
	INT,
	FLOAT,
	DOUBLE,
	CHAR,
	WHILE,
	FOR,
	IF,
	ELSE,
	SWITCH,
	CASE,
	DEFAULT,
	TRUE,
	FALSE,
	DEFINE,
	RETURN,
	BREAK,
	CONTINUE,
	CONST,
	UNDERLINE,
	PLUS,
	MINUS,
	MULTI,
	RDIV,
	MOD,
	ASSIGN,
	EQ,
	LT,
	GT,
	NE,
	LE,
	GE,
	LR_BRAC,
	RR_BRAC,
	LS_BRAC,
	RS_BRAC,
	LB_BRAC,
	RB_BRAC,
	QUES_MARK,
	COLON,
	SEMIC,
	Q_MARK,
	DOUBLE_QUOTE,
	COMMA,
	HASHTAG,
	BACKSLASH,
};

struct variable
{
	size_t code;
	bool first[t_len];
	bool follow[t_len];
	shared_ptr<struct terminal> next;

	struct variable(){
		next = nullptr;
		for (size_t i = 0; i < t_len; ++i){
			first[i] = false;
			follow[i] = false;
		}
	}
};

struct terminal
{
	long long int code;
	bool first[t_len];
	shared_ptr<struct terminal> next;

	struct terminal(){
		next = nullptr;
		for (size_t i = 0; i < t_len; ++i){
			first[i] = false;
		}
	}
};

struct tokenRecord
{
	enum tokenType token;		//符号类型
	shared_ptr<struct nameItem> name_item;
	//常数值就记录在下面这些地方,方便输出中间代码
	string value;
	tokenRecord()
	{
		name_item = nullptr;
		token = EMPTY;
		
	}
};

struct nameItem
{
	string name;//每一个token的名字
	enum tokenType token_type;//token的类型
	enum tokenType type;//实际的类型 只能是char int等基础类型
	int* extendPtr;//扩展属性指针
	int address;
	int offset;
	//shared_ptr<struct nameItem> next;
	size_t row_counter;//行计数器
	size_t col_counter;//列计数器
	union{
		char ch;
		int integer;
		double rear;
		char* str;
	} value;
	
	//分配的内存地址
};

struct nameTableHead
{
	map<string, shared_ptr<struct nameItem>> name_items;
	vector<shared_ptr<struct nameTableHead>> childTable;
	shared_ptr<struct nameTableHead> parentTable;
	string funcName;
};

struct LR1ItemNode
{
	int headCode;
	long long int proCode;
	bool advancedCode[t_len];
	int point;//记录当前点的位置
	bool isVisited;

	int advanced;
	shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> gotoPtr;
	struct LR1ItemNode(){
		advanced = 0;
		gotoPtr = nullptr;
		isVisited = false;
		point = 0;
		for (size_t k = 0; k < t_len; ++k){
			advancedCode[k] = false;
		}
	}
};

struct itemSetNode
{
	short flag;
	shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> gotoPtr;
	
	int head;
	long long int code;
	itemSetNode()
	{
		flag = 0;
		gotoPtr = nullptr;
		head = 0;
		code = 0;
	}
};

struct variNode
{
	string name;
	int addr;//程序的offset
	int offset;//自己的一个属性，主要用于数组元素
	enum tokenType token_type;
	enum tokenType type;
	int width;//类型的宽度
	queue<string> que;//方便初始化
	double syn;// 综合属性，方便计算
	double inh;// 继承属性，单纯用来计算，最好是double型

	string op;					//操作符
	int instr;					//当前指令地址
	vector<int> truelist;	//布尔表达式真出口
	vector<int> falselist;	//布尔表达式假出口
	vector<int> nextlist;	//跳转指令出口

	variNode()
	{
		name = "";
		addr = 0;
		offset = 0;
		token_type = EMPTY;
		type = EMPTY;
	}
};