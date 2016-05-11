#pragma once
#include "main.h"

class NameTable
{
public:
	shared_ptr<struct nameTableHead> tableHead;
	stack<shared_ptr<struct nameTableHead>> tablePtrStack;

	NameTable();
	~NameTable();
	shared_ptr<struct nameItem> insert(shared_ptr<struct nameTableHead> head, string name);
	shared_ptr<struct nameItem> getItem();
	shared_ptr<struct nameItem> getItem(string name);
	shared_ptr<struct nameItem> getItem(shared_ptr<struct nameTableHead> head, string name);
	void newProduce();

	void exitProduce();

};

