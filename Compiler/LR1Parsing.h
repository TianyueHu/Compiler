#pragma once
#include "ProductionSet.h"

class LR1Parsing
{
private:
	ifstream ifs;
	vector<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>> itemSet;
	ProductionSet* produceSet;
	map<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>, struct itemSetNode*> itemTable;
	int GetLength(long long int code);
	int Get2Code(long long int code, int n);
	void GetClosure(shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> item);
	enum tokenType GetNewToken();
	bool LR1Parsing::Compare2Item(shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> vec1,
		shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> vec2);
	int GetCode(enum tokenType token);

public:
	LR1Parsing();
	~LR1Parsing();

	
	void OpenFile();
	void Production();
	
	void GetVariFst();
	void FisrtInit();

	void GetClosureSet();
	void GetLR1ParsingTable();

	void LR1();
	
	//计算闭包的函数
};

