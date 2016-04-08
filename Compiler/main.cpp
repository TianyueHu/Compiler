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
	l.FisrtInit();
	//l.PrintVariFst();
	//cout << endl;
	l.GetVariFst();
	//l.PrintVariFst();

	l.GetVariFol();
	//l.PrintVariFol();
	l.GetProFst();
	l.PrintProFst();
	getchar();
	return 0;
}