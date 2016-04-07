#include "ProductionSet.h"


ProductionSet::ProductionSet()
{

}


ProductionSet::~ProductionSet()
{

}

void ProductionSet::insert(size_t head, long long int body)
{
	shared_ptr<struct variable> ptr1 = getHead(head);
	shared_ptr<struct terminal> ptr;
	if (ptr1->next){
		ptr = ptr1->next;
		while (ptr->next){
			ptr = ptr->next;
		}
		shared_ptr<struct terminal> newNode = make_shared<struct terminal>();
		newNode->code = body;
		ptr->next = newNode;
	}
	else{
		shared_ptr<struct terminal> newNode = make_shared<struct terminal>();
		newNode->code = body;
		ptr1->next = newNode;
	}
}

shared_ptr<struct variable> ProductionSet::getHead(size_t head)
{
	for (size_t i = 0; i < proList.size(); ++i){
		if (proList[i]->code == head){
			return proList[i];
		}
	}
	shared_ptr<struct variable> newHead = make_shared<struct variable>();
	newHead->code = head;
	proList.push_back(newHead);
	return newHead;
}

void ProductionSet::printProList()
{
	for (size_t i = 0; i < proList.size(); ++i){
		cout << proList[i]->code << "--> ";
		shared_ptr<struct terminal> temp = proList[i]->next;
		while (temp){
			cout << temp->code << " | ";
			temp = temp->next;
		}
		cout << endl;
	}
}

