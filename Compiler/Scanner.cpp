#pragma warning(disable : 4996)

#include "Scanner.h"


Scanner::Scanner()
{
	row_counter = 0;
	col_counter = 0;

	head = make_shared<struct tokenRecord>();
	rear = head;
}


Scanner::~Scanner()
{
	if (ifs && ifs.is_open()){
		ifs.close();
	}
	if (ofs && ofs.is_open()){
		ofs.close();
	}
}

void Scanner::OpenFile(string filename)
{
	ifs.open(filename);
	ofs.open("a.txt");
	if (!ifs.is_open()){
		cerr << "open " << filename << "failed!" << endl;
	}
	if (!ofs.is_open()){
		cerr << "error!" << endl;
	}
}

bool Scanner::ReadNewLine()
{
	if (!ifs.eof()){

		getline(ifs, buffer);
		return true;
	}
	else {
		return false;
	}
}

void Scanner::scanner()
{
	ReadNewLine();
	char ch;

	while (true){
		if (col_counter < buffer.size()){
			ch = buffer[col_counter];
			Sort(ch);
			++col_counter;
		}
		else{
			col_counter = 0;
			++row_counter;
			if (ReadNewLine()){
				ch = buffer[col_counter];
			}
			else{
				break;
			}
		}

	}
}

void Scanner::Sort(char ch)
{
	if (isdigit(ch)){
		Num(ch);
	}
	else if (isalpha(ch)){
		Identify(ch);
	}
	else if (ch == '\''){
		Ch(ch);
	}
	else if (ch == '\"'){
		String(ch);
	}
	else{
		switch (ch){
		case '_':
			newToken(UNDERLINE, '_');
			break;
		case '+':
			newToken(PLUS, '+');
			break;
		case '-':
			newToken(MINUS, '-');
			break;
		case '*':
			newToken(MULTI, '*');
			break;
		case '/':
			newToken(RDIV, '/');
			break;
		case '%':
			newToken(MOD, '%');
			break;
		case '(':
			newToken(LR_BRAC, '(');
			break;
		case ')':
			newToken(RR_BRAC, ')');
			break;
		case '[':
			newToken(LS_BRAC, '[');
			break;
		case ']':
			newToken(RS_BRAC, ']');
			break;
		case '{':
			newToken(LB_BRAC, '{');
			break;
		case '}':
			newToken(RB_BRAC, '}');
			break;
		case '?':
			newToken(QUES_MARK, '?');
			break;
		case ':':
			newToken(COLON, ':');
			break;
		case ';':
			newToken(SEMIC, ';');
			break;
		case ',':
			newToken(COMMA, ',');
			break;
		case '#':
			newToken(HASHTAG, '#');
			break;
		case '\\':
			newToken(BACKSLASH, '\\');
			break;
		case '=':
			if (++col_counter < buffer.size()){
				char tch = buffer[col_counter];
				if (tch == '=')
					newToken(EQ, "==");
				else{
					--col_counter;
					newToken(ASSIGN, '=');
				}
			}
			break;
		case '<':
			if (++col_counter < buffer.size()){
				char tch = buffer[col_counter];
				if (tch == '>')
					newToken(NE, "<>");
				else if (tch == '=')
					newToken(LE, "<=");
			}
			else{
				--col_counter;
				newToken(LT, '<');
			}
			break;
		case '>':
			if (++col_counter < buffer.size()){
				char tch = buffer[col_counter];
				if (tch == '=')
					newToken(GE, ">=");
			}
			else{
				--col_counter;
				newToken(GT, '>');
			}
			break;
		default:
			//这里应该有一个出错的提示或者处理
			break;
		}
		
	}
}

void Scanner::Num(char ch)
{
	char *numStr = new char[1024];

	int state = 1;
	size_t finalFlag = 0;
	size_t i;
	char c;

	numStr[0] = ch;
	for (i = 1; ++col_counter < buffer.size(); ++i){
		c = buffer[col_counter];
		if (state == 1 && isdigit(c)){
			state = 1;
			numStr[i] = c;
			finalFlag = i;
		}
		else if (state == 1 && c == '.'){
			state = 2;
			numStr[i] = c;
			finalFlag = i;
		}
		else if ((state == 1 || state == 2) && (c == 'E' || c == 'e')){
			state = 3;
			numStr[i] = c;
		}
		else if (state == 2 && isdigit(c)){
			state = 2;
			numStr[i] = c;
			finalFlag = i;
		}
		else if (state == 3 && isdigit(c)){
			state = 5;
			numStr[i] = c;
			finalFlag = i;
		}
		else if (state == 3 && (c == '+' || c == '-')){
			state = 4;
			numStr[i] = c;
		}
		else if (state == 4 && isdigit(c)){
			state = 5;
			numStr[i] = c;
			finalFlag = i;
		}
		else if (state == 5 && isdigit(c)){
			state = 5;
			numStr[i] = c;
			finalFlag = i;
		}
		else if (state == 1){
			numStr[i] = '\0';
			--col_counter;
			
			char* newStr = new char[i + 1];
			strcpy(newStr, numStr);
			newToken(INTEGER, newStr);
			delete newStr;

			break;
		}
		else if (state == 2 || state == 5){
			numStr[i] = '\0';
			--col_counter;

			char* newStr = new char[i + 1];
			strcpy(newStr, numStr);
			newToken(REAL, newStr);
			delete newStr;
			break;
		}
		else if (state == 3 || state == 4){
			col_counter -= (i - finalFlag);
			i = finalFlag + 1;
			numStr[i] = '\0';

			char* newStr = new char[i + 1];
			strcpy(newStr, numStr);
			newToken(REAL, newStr);
			delete newStr;

			break;
		}
	}
	if (col_counter >= buffer.size()){
		col_counter -= (i - finalFlag);
		i = finalFlag + 1;
		numStr[i] = '\0';

		char* newStr = new char[i + 1];
		strcpy(newStr, numStr);
		newToken(REAL, newStr);
		delete newStr;
	}
	
	delete numStr;
}

void Scanner::Identify(char ch)
{
	char* id = new char[1024];
	char c;
	id[0] = ch;
	size_t i;
	for (i = 1; ++col_counter < buffer.size(); ++i){
		if (isalnum(c = buffer[col_counter]) || c == '_'){
			id[i] = c;
		}
		else{
			break;
		}
	}
	id[i] = '\0';
	char* id1 = new char[i + 1];
	strcpy(id1, id);
	if (!strcmp("not", id1)){
		newToken(NOT, id1);
	}
	else if (!strcmp("and", id1)){
		newToken(AND, id1);
	}
	else if (!strcmp("or", id1)){
		newToken(OR, id1);
	}
	else if (!strcmp("int", id1)){
		newToken(INT, id1);
	}
	else if (!strcmp("float", id1)){
		newToken(FLOAT, id1);
	}
	else if (!strcmp("double", id1)){
		newToken(DOUBLE, id1);
	}
	else if (!strcmp("char", id1)){
		newToken(CHAR, id1);
	}
	else if (!strcmp("while", id1)){
		newToken(WHILE, id1);
	}
	else if (!strcmp("for", id1)){
		newToken(FOR, id1);
	}
	else if (!strcmp("if", id1)){
		newToken(IF, id1);
	}
	else if (!strcmp("else", id1)){
		newToken(ELSE, id1);
	}
	else if (!strcmp("switch", id1)){
		newToken(SWITCH, id1);
	}
	else if (!strcmp("case", id1)){
		newToken(CASE, id1);
	}
	else if (!strcmp("default", id1)){
		newToken(DEFAULT, id1);
	}
	else if (!strcmp("True", id1)){
		newToken(TRUE, id1);
	}
	else if (!strcmp("False", id1)){
		newToken(FALSE, id1);
	}
	else if (!strcmp("continue", id1)){
		newToken(CONTINUE, id1);
	}
	else if (!strcmp("break", id1)){
		newToken(BREAK, id1);
	}
	else if (!strcmp("define", id1)){
		newToken(DEFINE, id1);
	}
	else if (!strcmp("return", id1)){
		newToken(RETURN, id1);
	}
	else if (!strcmp("const", id1)){
		newToken(CONST, id1);
	}
	else if (!strcmp("Null", id1)){
		newToken(EMPTY, id1);
	}
	else if (col_counter < buffer.size() && buffer[col_counter] == '['){
		newToken(ARRAY, id1);
	}
	else if (col_counter < buffer.size() && buffer[col_counter] == '('){
		newToken(FUNC, id1);
	}
	else{
		newToken(ID, id1);
	}
	--col_counter;
	delete id, id1;

}

void Scanner::String(char ch)
{
	char* str = new char[1024];
	char c;
	size_t i;

	for (i = 0; ++col_counter < buffer.size() && isalpha(c = buffer[col_counter]); ++i){
		str[i] = c;
	}
	str[i] = '\0';
	if (col_counter < buffer.size() && buffer[col_counter] == '\"'){
		char* nStr = new char[i + 2];
		strcpy(nStr, str);
		newToken(STRING, nStr);
		delete nStr;
	}
	else{
		col_counter -= i;
		newToken(DOUBLE_QUOTE);
	}
	
	delete str;
}

void Scanner::Ch(char ch)
{
	char c;
	if (++col_counter < buffer.size() && isalpha(c = buffer[col_counter])){
		//对c做一个处理
		if (++col_counter < buffer.size() && buffer[col_counter] == '\''){
			newToken(CH, c);
		}
		else{
			col_counter -= 2;
			newToken(Q_MARK);
		}
	}
	else{
		newToken(Q_MARK);
		--col_counter;
	}
}

void Scanner::newToken(enum tokenType token)
{
	shared_ptr<struct tokenRecord> recordPtr = make_shared<struct tokenRecord>();
	recordPtr->token = token;
	rear->next = recordPtr;
	rear = rear->next;

	recordPtr->name_item = nullptr;
	ofs << token << " " << 0 << endl;
}

void Scanner::newToken(enum tokenType token, char ch)
{
	shared_ptr<struct tokenRecord> recordPtr = make_shared<struct tokenRecord>();
	recordPtr->token = token;
	rear->next = recordPtr;
	rear = rear->next;
	ofs << token << " " << ch << endl;
}

void Scanner::newToken(enum tokenType token, char* name)
{
	shared_ptr<struct tokenRecord> recordPtr = make_shared<struct tokenRecord>();
	recordPtr->token = token;
	rear->next = recordPtr;
	rear = rear->next;
	ofs << token << " " << name << endl;

}

void Scanner::SetRowCounter(size_t value)
{
	row_counter = value;
}

void Scanner::SetColCounter(size_t value)
{
	col_counter = value;
}

size_t Scanner::GetRowCounter()
{
	return row_counter;
}

size_t Scanner::GetColCounter()
{
	return col_counter;
}
