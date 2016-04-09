#include "LL1Parsing.h"
#pragma warning(disable : 4996)

LL1Parsing::LL1Parsing()
{
	produceSet = new ProductionSet();
	
}

LL1Parsing::~LL1Parsing()
{
	if (ifs.is_open()){
		ifs.close();
	}
	delete produceSet;
}

void LL1Parsing::Production()
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

void LL1Parsing::OpenFile()
{
	ifs.open("a.txt");
	if (!ifs.is_open()){
		cerr << "error in LL1Parsing" << endl;
	}
}

enum tokenType LL1Parsing::GetNewToken()
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

void LL1Parsing::FisrtInit()
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

void LL1Parsing::GetVariFst()
{
	bool flag = true;
	size_t size = produceSet->proList.size();
	
	while (flag){
		flag = false;
		for (size_t i = 0; i < size; ++i){
			shared_ptr<struct variable> head = produceSet->proList[i];
			shared_ptr<struct terminal> ptr = produceSet->proList[i]->next;
			while(ptr){
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

void LL1Parsing::GetVariFol()
{
	bool flag = true;
	size_t size = produceSet->proList.size();

	produceSet->proList[0]->follow[0] = true;

	while (flag){
		flag = false;
		for (size_t i = 0; i < size; ++i){
			shared_ptr<struct variable> head = produceSet->proList[i];
			shared_ptr<struct terminal> ptr = produceSet->proList[i]->next;
			while (ptr){
				//对每一个产生式
				for (size_t j = 1; j < (int)((GetLength(ptr->code) + 1) / 2) + 1; ++j){
					//对当前产生式中的每一个符号
					int code = 0;//当前符号的编码
					if (ptr->code < 10){
						break;
					}
					else if ((code = Get2Code(ptr->code, j)) < t_len){
						//这里是从后面开始获得的
						continue;
					}
					else if (code > t_len){
						//当前符号是非终结符

						if ((j + 1) < (int)((GetLength(ptr->code) + 1) / 2) + 1){
							//当前符号不是最后一个字符,这里将并上下一个字符的first集
							
							for (int offset = 1; (j + offset) < (int)((GetLength(ptr->code) + 1) / 2) + 1; ++offset){
								//当没有查找到最后一个字符前

								int nextCode = Get2Code(ptr->code, j + offset);
								//下一个符号的编码
								if (nextCode < t_len){
									//如果下一个字符是终结符
									if (!produceSet->proList[code - 50]->follow[nextCode]){
										produceSet->proList[code - 50]->follow[nextCode] = true;
										flag = true;
									}
									break;
								}
								else if (nextCode > t_len){
									//如果下一个字符是非终结符
									for (size_t k = 0; k < t_len; ++k){
										if (produceSet->proList[nextCode - 50]->first[k] && !produceSet->proList[code - 50]->follow[k]){
											flag = true;
											produceSet->proList[code - 50]->follow[k] = true;
										}
									}
									if (!produceSet->proList[nextCode - 50]->first[0]){
										break;
									}
								}
							}
							for (size_t k = 0; k < t_len; ++k){
								if (head->follow[k] && !produceSet->proList[code - 50]->follow[k]){
									flag = true;
									produceSet->proList[code - 50]->follow[k] = true;
								}
							}
						}
						else {
							//如果是最后一个字符，那么需要并上产生式前端的Follow集
							for (size_t k = 0; k < t_len; ++k){
								if (head->follow[k] && !produceSet->proList[code - 50]->follow[k]){
									flag = true;
									produceSet->proList[code - 50]->follow[k] = true;
								}
							}
						}
					}
				}
				ptr = ptr->next;
			}
		}
	}
}

void LL1Parsing::GetProFst()
{
	size_t size = produceSet->proList.size();

	for (size_t i = 0; i < size; ++i){
		shared_ptr<struct variable> head = produceSet->proList[i];
		shared_ptr<struct terminal> ptr = produceSet->proList[i]->next;
		while (ptr){
			//对每一个产生式
			for (size_t j = 1; j < (int)((GetLength(ptr->code) + 1) / 2) + 1; ++j){
				int code = 0;
				if (ptr->code < 10){
					ptr->first[ptr->code] = true;
					break;
				}
				else if ((code = Get2Code(ptr->code, j)) < t_len){
					ptr->first[code] = true;
					break;
				}
				else if (code > t_len){
					for (size_t k = 1; k < t_len; ++k){
						if (produceSet->proList[code - 50]->first[k]){
							ptr->first[k] = true;
						}
					}
					if (!produceSet->proList[code - 50]->first[0]){
						break;
					}
				}
			}
			ptr = ptr->next;
		}
	}
	
}

int LL1Parsing::GetLength(long long int code)
{
	return ((int)log10l(code) + 1);
}

int LL1Parsing::Get2Code(long long int code, int n)
{
	int len = GetLength(code);
	if (len % 2 == 0){
		return (long long int)(code / pow(10, len - 2 * n)) % 100;
	}
	else {
		return (long long int)(code / pow(10, len - 2 * n + 1)) % 100;
	}
	
}

void LL1Parsing::PrintVariFst()
{
	for (size_t i = 0; i < produceSet->proList.size(); ++i){
		shared_ptr<struct variable> head = produceSet->proList[i];
		cout << head->code << "--> ";
		for (size_t j = 0; j < t_len; ++j){
			if (head->first[j]){
				cout << j << " ";
			}
		}
		cout << endl;
	}
}

void LL1Parsing::PrintVariFol()
{
	for (size_t i = 0; i < produceSet->proList.size(); ++i){
		shared_ptr<struct variable> head = produceSet->proList[i];
		cout << head->code << "--> ";
		for (size_t j = 0; j < t_len; ++j){
			if (head->follow[j]){
				cout << j << " ";
			}
		}
		cout << endl;
	}
}

void LL1Parsing::PrintProFst()
{
	size_t size = produceSet->proList.size();

	for (size_t i = 0; i < size; ++i){
		shared_ptr<struct variable> head = produceSet->proList[i];
		cout << head->code << "--> ";
		shared_ptr<struct terminal> ptr = produceSet->proList[i]->next;
		while (ptr){
			//对每一个产生式
			cout << ptr->code << endl;
			cout << "     ";
			for (size_t j = 0; j < t_len; ++j){
				if (ptr->first[j]){
					cout << j << "|";
				}
			}
			cout << endl;
			ptr = ptr->next;
		}
	}
}

void LL1Parsing::GetFAATable()
{
	size_t size = produceSet->proList.size();
	for (size_t i = 0; i < size; ++i){
		shared_ptr<struct variable> head = produceSet->proList[i];
		shared_ptr<struct terminal> ptr = produceSet->proList[i]->next;
		while (ptr){

			bool flag1 = false, flag2 = false;
			if (ptr->first[0]){
				flag1 = true;
				if (head->follow[0]){
					flag2 = true;
				}
			}
			if (flag2){
				faaTable[head->code - 50][t_len].push_back(ptr->code);
			}
			for (size_t j = 0; j < t_len; ++j){
				if (ptr->first[j]){
					faaTable[head->code - 50][j].push_back(ptr->code);
				}
				if (flag1 && head->follow[j]){
					faaTable[head->code - 50][j].push_back(ptr->code);
				}
			}
			ptr = ptr->next;
		}
	}
}

void LL1Parsing::PrintFAATable()
{
	size_t size = produceSet->proList.size();
	for (size_t i = 0; i < size; ++i){
		shared_ptr<struct variable> head = produceSet->proList[i];
		shared_ptr<struct terminal> ptr = produceSet->proList[i]->next;
		while (ptr){
			cout << "head " << i + 50 << ":" << endl;
			for (size_t j = 1; j < t_len + 1; ++j){
				if (faaTable[i][j].size()){
					cout << "node " << j << ":";
					for (size_t k = 0; k < faaTable[i][j].size(); ++k){
						cout << faaTable[i][j][k] << "|";
					}
				}
			}
			cout << endl;
			ptr = ptr->next;
		}
	}
}

void LL1Parsing::LL1()
{
	stack<int> s;
	enum tokenType token;
	s.push(99);
	s.push(50);
	int top;
	token = GetNewToken();
	do{
		top = s.top();
		
		if (top < t_len || top == 99){
			if (top != 99 && top == GetCode(token)){
				cout << "pop " << top << endl;
				s.pop();
				token = GetNewToken();
			}
			else {
				cerr << "error" << endl;
			}
		}
		else{
			if (faaTable[top - 50][GetCode(token)].size()>0){
				long long int code = faaTable[top - 50][GetCode(token)][0];
				cout << top << "-->" << code << endl;
				s.pop();
				if (code > 0){
					for (size_t j = (int)((GetLength(code) + 1) / 2); j > 0; --j){
						s.push(Get2Code(code, j));
					}
				}
			}
			else {
				cerr << "error" << endl;
			}
		}
	} while (s.top() != 99 && !s.empty());
}

int LL1Parsing::GetCode(enum tokenType token)
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
	if (token == GT )
		return 32;
	if (token == LT)
		return 33;
	if (token == LE )
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