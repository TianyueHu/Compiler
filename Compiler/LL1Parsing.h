#pragma once
#include "main.h"
#include "ProductionSet.h"

/*
空 00
not 01
and 02
or 03
标识符 04
数组名 05
函数名 06
字符串 07
整数常量 08
小数常量 09
( 10
) 11
char 12
int 13
float 14
double 15
, 16
[ 17
] 18
; 19
= 20
{ 21
} 22
if 23
else 24
while 25
for 26
continue 27
break 28
return 29
? 30
: 31
> 32
< 33
<= 34
>= 35
<> 36
== 37
+ 38
- 39
/ 40
* 41
% 42


程序 50
扩展声明 51
函数定义 52
类型定义 53
形参列表 54
形参声明 55
声明序列 56
初始化声明 57
直接声明体 58
初始化程序 59
初始化序列 60
初始化序列1 61
复合语句 62
复合语句体 63
语句序列 64
语句 65
表达语句 66
分支语句 67
循环语句 68
跳转语句 69
表达式 70
条件表达式 71
三目运算式 72
逻辑表达式 73
逻辑表达式1 74
逻辑符 75
关系表达式 76
关系表达式1 77
关系操作符 78
因式 79
因式1 80
项 81
后缀表达式 82
基本表达式 83
实参列表 84
加减操作符 85
乘除操作符 86
*/



class LL1Parsing
{
private:
	ifstream ifs;
	ProductionSet* produceSet;
	enum tokenType GetNewToken();
	int GetLength(long long int code);
	int Get2Code(long long int code, int n);

public: 
	
	LL1Parsing();
	~LL1Parsing();
	void FisrtInit();
	void OpenFile();
	void Production();

	void GetVariFst();
	void GetVariFol();
	void GetProFst();

	void PrintVariFst();
	void PrintVariFol();
	void PrintProFst();
};

