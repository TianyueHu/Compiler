#include "main.h"
#include "Scanner.h"
#include "LL1Parsing.h"
#include "LR1Parsing.h"
int main()
{
	Scanner *s = new Scanner();
	s->OpenFile("e:\\test.txt");
	s->scanner();
	

	shared_ptr<struct nameTableHead> i = s->nameTable.tablePtrStack.top();
	cout << "FuncName" << i->funcName << endl;
	for (map<string, shared_ptr<struct nameItem>>::iterator iter = i->name_items.begin(); iter != i->name_items.end(); ++iter){
		cout << (*iter).first << " " << (*iter).second << endl;
	}
	s->nameTable.tablePtrStack.pop();
	i = s->nameTable.tablePtrStack.top();
	cout << "FuncName" << i->funcName << endl;
	for (map<string, shared_ptr<struct nameItem>>::iterator iter = i->name_items.begin(); iter != i->name_items.end(); ++iter){
		cout << (*iter).first << " " << (*iter).second << endl;
	}
	/*
	LL1Parsing ll1;
	ll1.LL1();			//LL1Óï·¨·ÖÎö
	*/

	LR1Parsing lr1;
	lr1.LR1();


	getchar();
	return 0;
}