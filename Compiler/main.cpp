#include "main.h"
#include "Scanner.h"
#include "LL1Parsing.h"
#include "LR1Parsing.h"
int main()
{
	Scanner *s = new Scanner();
	s->OpenFile("e:\\test.txt");
	s->scanner();
	
	/*
	LL1Parsing ll1;
	ll1.LL1();			//LL1”Ô∑®∑÷Œˆ
	*/

	LR1Parsing lr1;
	lr1.OpenFile();
	lr1.Production();
	lr1.FisrtInit();
	lr1.GetVariFst();
	lr1.GetClosureSet();
	lr1.GetLR1ParsingTable();
	lr1.LR1();


	getchar();
	return 0;
}