#include "main.h"
#include "Scanner.h"
#include "LL1Parsing.h"

int main()
{
	//Scanner *s = new Scanner();
	//s->OpenFile("e:\\test.txt");
	//s->scanner();
	LL1Parsing l;
	l.OpenFile();
	l.Production();

	l.FisrtInit();		//FIRST集合初始化
	//l.PrintVariFst();

	l.GetVariFst();		//求FIRST集
	//l.PrintVariFst();

	l.GetVariFol();		//求FOLLOW集
	//l.PrintVariFol();

	l.GetProFst();		//求产生式的FIRST集合
	//l.PrintProFst();

	l.GetFAATable();	//构造预测分析表
	//l.PrintFAATable();

	getchar();
	return 0;
}