#pragma once
#include "main.h"


class ProductionSet
{

public:
	ProductionSet();
	~ProductionSet();

	void printProList();
	shared_ptr<struct variable> getHead(size_t head);
	void insert(size_t head, long long int body);
	vector<shared_ptr<struct variable>> proList;
};

