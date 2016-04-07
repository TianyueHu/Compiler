#pragma once
#include "main.h"


class ProductionSet
{
private:
	size_t t_lenth;//ÖÕ½á·û³¤¶È

public:
	ProductionSet();
	~ProductionSet();

	void printProList();
	shared_ptr<struct variable> getHead(size_t head);
	void insert(size_t head, long long int body);
	vector<shared_ptr<struct variable>> proList;
};

