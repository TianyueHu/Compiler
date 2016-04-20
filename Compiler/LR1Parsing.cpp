#include "LR1Parsing.h"
#pragma warning(disable : 4996)

LR1Parsing::LR1Parsing()
{
	produceSet = new ProductionSet();
	
}

LR1Parsing::~LR1Parsing()
{
	if (ifs.is_open()){
		ifs.close();
	}
	delete produceSet;
	
	//这里要删除在map里新建的所有的数组
}

void LR1Parsing::Production()
{
	ifstream proSetFile;
	proSetFile.open("pro.txt");
	if (proSetFile.is_open()){
		string buffer;
		while (!proSetFile.eof()){
			getline(proSetFile, buffer);
			int head = (buffer[0] - '0') * 10 + (buffer[1] - '0');

			int i, j;
			char* numStr = new char[20];
			for (i = 3, j = 0; i <= buffer.size() && j < 20; ++i, ++j){

				if (isdigit(buffer[i]) && i < buffer.size()){
					numStr[j] = buffer[i];
				}
				else{
					numStr[j] = '\0';
					char* newStr = new char[j + 1]();
					strcpy(newStr, numStr);
					j = -1;
					produceSet->insert(head, atoll(newStr));
					delete newStr;
				}
			}
			delete numStr;
		}
	}
	else{
		cerr << "error in opening ProSetFile.txt" << endl;
	}
	//produceSet->printProList();
}

void LR1Parsing::OpenFile()
{
	ifs.open("a.txt");
	if (!ifs.is_open()){
		cerr << "error in LL1Parsing" << endl;
	}
}

int LR1Parsing::GetLength(long long int code)
{
	return ((int)log10l(code) + 1);
}

int LR1Parsing::Get2Code(long long int code, int n)
{
	int len = GetLength(code);
	if (len % 2 == 0){
		return (long long int)(code / pow(10, len - 2 * n)) % 100;
	}
	else {
		return (long long int)(code / pow(10, len - 2 * n + 1)) % 100;
	}

}

enum tokenType LR1Parsing::GetNewToken()
{
	string buffer;
	char* str = new char[1024]();
	size_t i;
	if (!ifs.eof()){
		getline(ifs, buffer);
		for (i = 0; i < buffer.size(); ++i){
			if (buffer[i] == ' '){
				break;
			}
			else{
				str[i] = buffer[i];
			}
		}
		str[i] = '\0';
	}
	else{
		delete str;
		return EMPTY;
	}

	char* str1 = new char[i + 1]();
	strcpy(str1, str);

	enum tokenType ret = enum tokenType(atoi(str1));

	delete str, str1;
	return ret;
}

void LR1Parsing::FisrtInit()
{
	for (size_t i = 0; i < produceSet->proList.size(); ++i){
		shared_ptr<struct variable> head = produceSet->proList[i];
		shared_ptr<struct terminal> ptr = head->next;
		while (ptr){
			if (ptr->code < 10){
				head->first[ptr->code] = true;
				//cout << "1 " << ptr->code << endl;
			}
			else{
				int code = Get2Code(ptr->code, 1);
				//cout <<"2 " << code << endl;
				if (code < t_len){
					head->first[code] = true;
				}
			}
			ptr = ptr->next;
		}
	}
}

void LR1Parsing::GetVariFst()
{
	bool flag = true;
	size_t size = produceSet->proList.size();

	while (flag){
		flag = false;
		for (size_t i = 0; i < size; ++i){
			shared_ptr<struct variable> head = produceSet->proList[i];
			shared_ptr<struct terminal> ptr = produceSet->proList[i]->next;
			while (ptr){
				for (size_t j = 1; j < (int)((GetLength(ptr->code) + 1) / 2) + 1; ++j){
					int code = 0;
					if (ptr->code < 10){
						head->first[ptr->code] = true;
						break;
					}
					else if ((code = Get2Code(ptr->code, j)) < t_len){
						if (!head->first[code]){
							flag = true;
							head->first[code] = true;
						}
						break;
					}
					else if (code > 49){
						shared_ptr<struct variable> fst = produceSet->proList[code - 50];
						for (size_t k = 0; k < t_len; ++k){
							if (fst->first[k] && !head->first[k]){
								flag = true;
								head->first[k] = true;
							}
						}
						if (!fst->first[0]){
							break;
						}
					}
				}
				ptr = ptr->next;
			}
		}
	}
}

void LR1Parsing::GetClosureSet()
{
	queue<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>> que;

	shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> startItem = make_shared<vector<shared_ptr<struct LR1ItemNode>>>();
	shared_ptr<struct terminal> codePtr = produceSet->proList[0]->next;

	while (codePtr){
		shared_ptr<struct LR1ItemNode> startNode = make_shared<struct LR1ItemNode>();
		
		startNode->advancedCode[0] = true;
		startNode->headCode = variSrt;
		startNode->proCode = codePtr->code;
		startNode->point = 0;
		startItem->push_back(startNode);
		codePtr = codePtr->next;
	}
	itemSet.push_back(startItem);

	GetClosure(startItem);

	que.push(startItem);

	while (que.size()){
		shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> item = que.front();
		que.pop();
		
		//现在处理item 针对每一个产生式产生一个新的转移
		for (vector<shared_ptr<struct LR1ItemNode>>::iterator iter = item->begin(); iter != item->end(); ++iter){
			if ((*iter)->proCode < 10 && (*iter)->point == 0){
				//当前产生式还没有到规约的时候
				(*iter)->advanced = (*iter)->proCode;
				
			}
			else if ((*iter)->proCode > 9 && (*iter)->point < (GetLength((*iter)->proCode) + 1) / 2){
				//当前产生式还没有到规约的时候
				
				(*iter)->advanced = Get2Code((*iter)->proCode, (*iter)->point + 1);
				
			}
			else{
				continue;
			}
			shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> newItem = make_shared<vector<shared_ptr<struct LR1ItemNode>>>();
			shared_ptr<struct LR1ItemNode> newNode = make_shared<struct LR1ItemNode>();
			newNode->headCode = (*iter)->headCode;
			newNode->proCode = (*iter)->proCode;
			newNode->point = (*iter)->point + 1;
			for (size_t k = 0; k < t_len; ++k){
				if ((*iter)->advancedCode[k]){
					newNode->advancedCode[k] = true;
				}
			}

			
			newItem->push_back(newNode);
			GetClosure(newItem);
			bool tempFlag = false;
			vector<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>>::iterator iter1;
			for (iter1 = itemSet.begin(); iter1 != itemSet.end(); ++iter1){
				if (Compare2Item((*iter1), newItem)){
					tempFlag = true;
					(*iter)->gotoPtr = *iter1;
					break;
				}
			}
			if (!tempFlag){
				que.push(newItem);
				itemSet.push_back(newItem);
				(*iter)->gotoPtr = newItem;
			}
			
		}
	}
}

void LR1Parsing::GetClosure(shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> item)
{
	//展示不考虑重复的问题
	vector<shared_ptr<struct LR1ItemNode>> tempSet;
	bool flag = true;
	

	while (flag){
		flag = false;
		for (vector<shared_ptr<struct LR1ItemNode>>::iterator iter = item->begin(); iter != item->end(); ++iter){
			bool advancedList[t_len];
			for (size_t i = 0; i < t_len; ++i){
				advancedList[i] = false;
			}
			if (!(*iter)->isVisited){
				(*iter)->isVisited = true;
				long long int code = (*iter)->proCode;//当前编码
				//得到向前看序列针对，当前闭包的状态的产生式
				if (code == 0){
					(*iter)->point = 1;
				}
				if (code > 9 && (*iter)->point + 2 < (GetLength(code) + 1) / 2 + 1 && Get2Code(code, (*iter)->point + 1) >= variSrt){
					//不是最后一个字符
					
					int nowCode = Get2Code(code, (*iter)->point + 1);//点符号后面一个字符
					int count = (*iter)->point + 2;
					while (count < (GetLength(code) + 1) / 2 + 1){
						int co = 0;
						if ((co = Get2Code(code, count)) >= variSrt){
							for (size_t k = 1; k < t_len; ++k){
								if (produceSet->proList[co - variSrt]->first[k]){
									advancedList[k] = true;
								}
							}
							if (!produceSet->proList[co - variSrt]->first[0]){
								break;
							}
						}
						else{
							advancedList[co] = true;
							break;
						}
						++count;
					}
					if (count >= (GetLength(code) + 1) / 2 + 1){
						//说明产生式的后部可以推导出空产生式
						//所以要并上推导出这个产生式的向前看符号
						for (size_t k = 0; k < t_len; ++k){
							if ((*iter)->advancedCode[k]){
								advancedList[k] = true;
							}
						}
					}
				}
				else if (code > 9 && (*iter)->point + 1 < (GetLength(code) + 1) / 2 + 1 && Get2Code(code, (*iter)->point + 1) >= variSrt){
					//如果是最后一个字符则继承
					
					for (size_t k = 0; k < t_len; ++k){
						if ((*iter)->advancedCode[k]){
							advancedList[k] = true;
						}
					}
				}
				
				if (code > 9 && (*iter)->point + 1 < (GetLength(code) + 1) / 2 + 1 && Get2Code(code, (*iter)->point + 1) >= variSrt){
					//如果不是终结符就跳过
					//将所有推出的产生式并入当前的集合
					shared_ptr<struct variable> head = produceSet->proList[Get2Code(code, (*iter)->point + 1) - variSrt];
					shared_ptr<struct terminal> body = head->next;

					while (body){
						shared_ptr<struct LR1ItemNode> newNode = make_shared<struct LR1ItemNode>();
						newNode->point = 0;
						newNode->headCode = head->code;
						newNode->isVisited = false;
						newNode->proCode = body->code;
						body = body->next;
						for (size_t k = 0; k < t_len; ++k){
							if (advancedList[k]){
								newNode->advancedCode[k] = true;
							}
						}
						tempSet.push_back(newNode);
					}
				}
			}
		}

		//这里把刚刚压入临时栈的itemNode压入闭包栈
		if (tempSet.size()){
			for (vector<shared_ptr<struct LR1ItemNode>>::iterator iter = tempSet.begin(); iter != tempSet.end(); ++iter){
				item->push_back((*iter));
			}
			tempSet.clear();
			flag = true;
		}
	}
}

void LR1Parsing::GetLR1ParsingTable()
{
	for (vector<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>>::iterator iter = itemSet.begin(); iter != itemSet.end(); ++iter){

		itemTable[(*iter)] = new struct itemSetNode[90]();//前面t_len个是转移或者归约表，variSrt以后是GOTO表

	}

	for (vector<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>>::iterator iter = itemSet.begin(); iter != itemSet.end(); ++iter){
		//对于每一个项
		shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> item = (*iter);
		for (vector<shared_ptr<struct LR1ItemNode>>::iterator itemIter = item->begin(); itemIter != item->end(); ++itemIter){
			//对于项里每一个产生式
			shared_ptr<struct LR1ItemNode> itemNode = (*itemIter);
			if (itemNode->gotoPtr){
				//说明状态点不在产生式的最后
				int gotoCode = itemNode->advanced;
				if (gotoCode < t_len){
					//转移的符号是终结符
					itemTable[item][gotoCode].flag = 1;
					itemTable[item][gotoCode].gotoPtr = itemNode->gotoPtr;
				}
				else{
					//转移的符号不是终结符 应该填入GOTO表当中
					itemTable[item][gotoCode].flag = 3;
					itemTable[item][gotoCode].gotoPtr = itemNode->gotoPtr;
				}
			}

			else{
				//产生式的规约
				for (size_t k = 0; k < t_len; ++k){
					if (itemNode->advancedCode[k]){
						itemTable[item][k].head = itemNode->headCode;
						itemTable[item][k].code = itemNode->proCode;
						itemTable[item][k].flag = 2;
					}
				}
			}
		}
	}
}

bool LR1Parsing::Compare2Item(shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> vector1, 
	shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> vector2)
{
	bool flag = true;
	if (vector1->size() == vector2->size()){
		vector<shared_ptr<struct LR1ItemNode>> vec1(*vector1);
		vector<shared_ptr<struct LR1ItemNode>> vec2(*vector2);

		for (vector<shared_ptr<struct LR1ItemNode>>::iterator iter1 = vec1.begin(); iter1 != vec1.end(); ++iter1){

			for (vector<shared_ptr<struct LR1ItemNode>>::iterator iter2 = vec2.begin(); iter2 != vec2.end(); ++iter2){

				if ((*iter1)->headCode == (*iter2)->headCode
					&& (*iter1)->proCode == (*iter2)->proCode
					&& (*iter1)->point == (*iter2)->point){
					
					for (size_t k = 0; k < t_len; ++k){
						if ((*iter1)->advancedCode[k] != (*iter2)->advancedCode[k]){
							flag = false;
							return flag;
						}
					}
					vec2.erase(iter2);
					break;
				}
				else if (iter2 + 1 == vec2.end()){
					flag = false;
					return flag;
				}
			}
		}
	}
	else{
		flag = false;
	}
	return flag;
}

void LR1Parsing::LR1()
{
	shared_ptr<vector<shared_ptr<struct LR1ItemNode>>> item = itemSet.front();
	int symbol = GetCode(GetNewToken());
	stack<shared_ptr<vector<shared_ptr<struct LR1ItemNode>>>> itemStack;
	stack<int> symbolStack;
	itemStack.push(item);
	while (true)
	{
		
		struct itemSetNode itemNode = itemTable[itemStack.top()][symbol];
		if (symbol && itemNode.flag == 1){//不是规约
			itemStack.push(itemNode.gotoPtr);
			symbolStack.push(symbol);
			cout << "state:" << itemNode.head << "-->" << itemNode.code << endl;
			cout << "ip:" << symbol << endl;
			symbol = GetCode(GetNewToken());
		}
		else if (symbol && itemNode.flag == 2){//规约
			
			int length = (GetLength(itemNode.code) + 1 ) / 2;
			while (length > 0){
				cout << "pop:" << symbolStack.top() << endl;
				itemStack.pop();
				symbolStack.pop();
				--length;
			}
			
			symbolStack.push(itemNode.head);
			itemStack.push(itemTable[itemStack.top()][itemNode.head].gotoPtr);
		}
		else{
			cout << "error" << endl;
			return;
		}
	}
}

int LR1Parsing::GetCode(enum tokenType token)
{
	if (token == NOT)
		return 1;
	if (token == AND)
		return 2;
	if (token == OR)
		return 3;
	if (token == ID)
		return 4;
	if (token == ARRAY)
		return 5;
	if (token == FUNC)
		return 6;
	if (token == STRING)
		return 7;
	if (token == INTEGER)
		return 8;
	if (token == REAL)
		return 9;
	if (token == LR_BRAC)
		return 10;
	if (token == RR_BRAC)
		return 11;
	if (token == CHAR)
		return 12;
	if (token == INT)
		return 13;
	if (token == FLOAT)
		return 14;
	if (token == DOUBLE)
		return 15;
	if (token == COMMA)
		return 16;
	if (token == LS_BRAC)
		return 17;
	if (token == RS_BRAC)
		return 18;
	if (token == SEMIC)
		return 19;
	if (token == ASSIGN)
		return 20;
	if (token == LB_BRAC)
		return 21;
	if (token == RB_BRAC)
		return 22;
	if (token == IF)
		return 23;
	if (token == ELSE)
		return 24;
	if (token == WHILE)
		return 25;
	if (token == FOR)
		return 26;
	if (token == CONTINUE)
		return 27;
	if (token == BREAK)
		return 28;
	if (token == RETURN)
		return 29;
	if (token == QUES_MARK)
		return 30;
	if (token == COLON)
		return 31;
	if (token == GT)
		return 32;
	if (token == LT)
		return 33;
	if (token == LE)
		return 34;
	if (token == GE)
		return 35;
	if (token == NE)
		return 36;
	if (token == EQ)
		return 37;
	if (token == PLUS)
		return 38;
	if (token == MINUS)
		return 39;
	if (token == RDIV)
		return 40;
	if (token == MULTI)
		return 41;
	if (token == MOD)
		return 42;
	if (token == CH)
		return 43;
	return 0;
}