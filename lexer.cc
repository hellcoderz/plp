#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

enum TYPE{
	IDENTIFIER, INTEGER, OPERATOR, STRING, BRACKET_OPEN, BRACKET_CLOSE, SEMICOLON, COMMA, END_OF_FILE, ERROR, COMMENT
};

struct token{
	TYPE type;
	string value;
};

struct list{
	token *data;
	list *next;
};

ifstream in;
ofstream out;
string lexeme;
char lastChar = ' ';

TYPE gettok(){
	char ch;
	lexeme = " ";

	//cout << lastChar << endl;
	ch = lastChar;
	if(ch == EOF)
		return END_OF_FILE;
	
	//skip whitespace
	while(isspace(ch)){
		ch = in.get();
	}

	//identifier: letter[letter|digit|'_']+
	if(isalpha(ch)){
		lexeme += ch;
		ch = in.get();
		while(isalnum(ch) || ch == '_'){
			lexeme += ch;
			ch = in.get();
		}	
		lastChar = ch;
		return IDENTIFIER;
	}

	//integer: [0-9]+
	else if(isdigit(ch)){
		lexeme += ch;
		ch = in.get();
		while(isdigit(ch)){
			lexeme += ch;
			ch = in.get();
		}	
		lastChar = ch;
		return INTEGER;
	}
	//cout << ch << endl;

	//string
	else if(ch == '\''){
		ch = in.get();
		while(ch != '\''){
			lexeme += ch;
			ch = in.get();
		}	
		lastChar = in.get();
		return STRING;
	}
	//commnent
	else if(ch == '/'){
		if(in.peek() != '/')
			return OPERATOR;

		ch = in.get();
		ch = in.get();
		while(ch != '\n'){
			lexeme += ch;
			ch = in.get();			
		}
		lastChar = ch;
		return COMMENT;
	}

	//operator
	else if(ch == '+' || ch == '-' || ch == '*' || ch == '<' || ch == '>' || ch == '&' || ch == '.' || ch == '@' || ch == '/' || ch == ':' || ch == '=' || ch == '~' || ch == '|' || ch == '$' || ch == '!' || ch == '#' || ch == '%' || ch == '^' || ch == '_' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '"' || ch == '`' || ch == '?'){
		lexeme += ch;
		ch = in.get();
		while(ch == '+' || ch == '-' || ch == '*' || ch == '<' || ch == '>' || ch == '&' || ch == '.' || ch == '@' || ch == '/' || ch == ':' || ch == '=' || ch == '~' || ch == '|' || ch == '$' || ch == '!' || ch == '#' || ch == '%' || ch == '^' || ch == '_' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '"' || ch == '`' || ch == '?'){
			lexeme += ch;
			ch = in.get();
		}	
		lastChar = ch;
		return OPERATOR;
	}

	//BRACKET_OPEN
	else if(ch == '('){
		lexeme += ch;
		lastChar = in.get();
		return BRACKET_OPEN;
	}

	//BRACKET_CLOSE
	else if(ch == ')'){
		lexeme += ch;
		lastChar = in.get();
		return BRACKET_CLOSE;
	}

	//SEMICOLON: [:]
	else if(ch == ';'){
		lexeme += ch;
		lastChar = in.get();
		return SEMICOLON;
	}

	//COMMA: [,]
	else if(ch == ','){
		lexeme += ch;
		lastChar = in.get();
		return COMMA;
	}

	lastChar = ch;
	return ERROR;
}

enum ExprType { E_NIL, E_DUMMY, E_INTEGER, E_IDENTIFIER, E_BOOLEAN, E_BINOP, E_UNARYOP, E_STRING, E_LET, E_LAMBDA, E_WHERE, E_TAU, E_AUG, E_IFTHEN, E_GAMMA };

class ExprAST{
public:
	virtual ~ExprAST() {}
};

class IntegerExprAST: public ExprAST{
	ExprType type;
	int Val;
public:
	IntegerExprAST(int val){
		type = E_INTEGER;
		Val = val;
	}
};

class IdentifierExprAST: public ExprAST{
	ExprType type;
	string Val;
public:
	IdentifierExprAST(string val){
		type = E_IDENTIFIER;
		Val = val;
	}
};

class StringExprAST: public ExprAST{
	ExprType type;
	string Val;
public:
	StringExprAST(string val){
		type = E_STRING;
		Val = val;
	}
};

class BooleanExprAST : public ExprAST{
	ExprType type;
	bool Val;
public:
	BooleanExprAST(bool val){
		type = E_BINOP;
		Val = val;
	}
};

class NilExprAST : public ExprAST{
	ExprType type;
public:
	NilExprAST(){
		type = E_NIL;
	}
};

class DummyExprAST : public ExprAST{
	ExprType type;
public:
	DummyExprAST(){
		type = E_DUMMY;
	}
};

enum BINOP { B_PLUS, B_MINUS, B_MUL, B_DIV, B_NEG, B_OR, B_AND, B_NOT, B_GR, B_GE, B_LS, B_LE, B_EQ, B_NE, B_AUG};

class BinaryOpExprAST : public ExprAST{
	ExprType type;
	ExprAST *Left;
	ExprAST *Right;
	BINOP OP;
public:
	BinaryOpExprAST(ExprAST *left, ExprAST *right,BINOP op) {
		type = E_BINOP;
		Left = left;
		Right = right;
		OP = op;
	}
};

class LetExprAST: public ExprAST{
	ExprType type;
	ExprAST *D;
	ExprAST *E;
public:
	LetExprAST(ExprAST *d, ExprAST *e){
		type = E_LET;
		D = d;
		E = e;
	}
};

//class LambdaExprAST: public ExprAST{
//	ExprAST *
//};

int main(){
	in.open("input");
	out.open("output");
	TYPE t;

	list *root, *head;
	root = NULL;
	token temp_token;
	list *temp_list;

	while((t = gettok()) != END_OF_FILE){
		temp_list = new list;
		/*
		if(t == IDENTIFIER){
			cout << "identifier: " << lexeme << endl;
		}else if(t == INTEGER){
			cout << "number: " << lexeme << endl;
		}else if(t == COMMENT){
			cout << "commnent: " << lexeme << endl;
		}else if(t == STRING){
			cout << "string: " << lexeme << endl;
		}else if(t == OPERATOR){
			cout << "operator: " << lexeme << endl;
		}else if(t == COMMA){
			cout << "comma: " << lexeme << endl;
		}else if(t == SEMICOLON){
			cout << "semicolon: " << lexeme << endl;
		}else if(t == BRACKET_OPEN){
			cout << "bracket_open: " << lexeme << endl;
		}else if(t == BRACKET_CLOSE){
			cout << "bracket_close: " << lexeme << endl;
		}else if(t == ERROR){
			cout << "ERROR" << endl;
			exit(0);
		}
		*/

		if(root == NULL){
			cout << "Root is NULL" << endl;
			temp_list->data = new token;
			temp_list->data->type = t;
			temp_list->data->value = lexeme;
			temp_list->next = NULL;
			head = temp_list;
			root = head;
		}else{
			cout << "Root is not NULL" << endl;
			temp_list->data = new token;
			temp_list->data->type = t;
			temp_list->data->value = lexeme;
			temp_list->next = NULL;
			head->next = temp_list;
			head = head->next;

		}

	}

	list *temp;
	temp = root;
	while(temp->next != NULL){
		cout << temp->data->value << endl;
		temp = temp->next;
	}

	in.close();
	out.close();

	return 0;
}
