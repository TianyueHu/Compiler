#pragma once
#include "ProductionSet.h"
#include "Scanner.h"
class LR1Parsing
{
private:
	ifstream ifs;
	ofstream ofs;
	vector<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>> itemSet;
	ProductionSet* produceSet;
	map<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>, struct itemSetNode*> itemTable;

	int GetLength(long long int code);
	int Get2Code(long long int code, int n);
	void GetClosure(shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> item);
	enum tokenType GetNewToken();
	shared_ptr<struct tokenRecord> LR1Parsing::GetNexTokenRecord();
	bool LR1Parsing::Compare2Item(shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> vec1,
		shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> vec2);
	int GetCode(enum tokenType token);
	string PrintID(int code);
	shared_ptr<struct tokenRecord> GetNextTokenRecord();

	shared_ptr<struct variNode> genCode(int head, long long int production);
	stack<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>> itemStack;
	vector<shared_ptr<struct variNode>> variStack;
	vector<int> symbolStack;
	vector<shared_ptr<struct tokenRecord>> tokenRecordStack;

	Scanner *s;

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
	void genCode();
};

