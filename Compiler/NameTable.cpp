#include "NameTable.h"
#include <cstdio>

NameTable::NameTable()
{
	tableHead = make_shared<struct nameTableHead>();
	tablePtrStack.push(tableHead);
}


NameTable::~NameTable()
{

}

shared_ptr<struct nameItem> NameTable::insert(shared_ptr<struct nameTableHead> head, string name)
{
	head->name_items[name] = make_shared<struct nameItem>();

	return head->name_items[name];
}

shared_ptr<struct nameItem> NameTable::getItem(shared_ptr<struct nameTableHead> head, string name)
{
	map<string, shared_ptr<struct nameItem>>::iterator iter = head->name_items.find(name);
	
	if (iter == head->name_items.end()){
		shared_ptr<struct nameItem> a = make_shared<struct nameItem>();
		int i = 0;
		head->name_items[name] = a;
	}
	return head->name_items[name];
}

void NameTable::newProduce()
{
	shared_ptr<struct nameTableHead> temp = make_shared<struct nameTableHead>();
	tablePtrStack.top()->childTable.push_back(temp);
	temp->parentTable = tablePtrStack.top();
	tablePtrStack.push(temp);
}


void NameTable::exitProduce()
{
	tablePtrStack.pop();
}

shared_ptr<struct nameItem> NameTable::getItem(string name)
{
	return getItem(tablePtrStack.top(), name);
}


shared_ptr<struct nameItem> NameTable::getItem()
{
	return make_shared<struct nameItem>();
}
