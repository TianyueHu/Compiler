#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#define t_len 43
using namespace std;
enum tokenType {
	EMPTY,
	ID,
	INTEGER,
	REAL,
	STRING,
	CH,
	ARRAY,
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
	shared_ptr<struct tokenRecord> next;	//指向下一个符号的指针

	//这里以后还要添加一个指向符号表的指针
	tokenRecord()
	{
		next = nullptr;
	}
};