#pragma once
#include "main.h"

class NameTable
{
private:
	shared_ptr<struct nameItem> getItem(string name);

public:
	struct nameTableHead tableHead;

	NameTable();
	~NameTable();
	shared_ptr<struct nameItem> insert(string name);
	
	
};

