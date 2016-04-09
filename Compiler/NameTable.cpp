#include "NameTable.h"


NameTable::NameTable()
{

}


NameTable::~NameTable()
{

}

shared_ptr<struct nameItem> NameTable::insert(string name)
{
	shared_ptr<struct nameItem> item = getItem(name);
	return item;
}

shared_ptr<struct nameItem> NameTable::getItem(string name)
{
	shared_ptr<struct nameItem> temp1 = tableHead.name_item, temp2;
	while (temp1){
		if (temp1->name == name){
			return temp1;
		}
		temp2 = temp1;
		temp1 = temp1->next;
	}

	shared_ptr<struct nameItem> newItem = make_shared<struct nameItem>();
	newItem->name = name;
	temp2->next = newItem;
	return newItem;
}