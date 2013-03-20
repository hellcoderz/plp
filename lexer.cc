#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>

#define DEBUG

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

/////////////////////
////AST NODES////////
/////////////////////

enum ExprType { E_NIL, E_DUMMY, E_INTEGER, E_IDENTIFIER, E_BOOLEAN, E_BINOP, E_UNARYOP, E_STRING, E_LET, E_LAMBDA, E_WHERE, E_TAU, E_AUG, E_COND, E_GAMMA, E_INFIX, E_WITHIN, E_AND, E_REC, E_ASSIGN, E_FUNC_FORM, E_PARENS, E_LIST };

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

class TauExprAST: public ExprAST{
	ExprType type;
	vector<ExprAST*> Ta;
public:
	TauExprAST(vector<ExprAST*> ta){
		type = E_TAU;
		Ta = ta;
	}
};

class AugExprAST: public ExprAST{
	ExprType type;
	ExprAST *Ta;
	ExprAST *Tc;
public:
	AugExprAST(ExprAST *ta, ExprAST *tc){
		type = E_AUG;
		Ta = ta;
		Tc = tc;
	}
};

class CondExprAST: public ExprAST{
	ExprType type;
	ExprAST *B;
	ExprAST *Tc_if;
	ExprAST *Tc_else;
public:
	CondExprAST(ExprAST *b, ExprAST *tc_if, ExprAST *tc_else){
		type = E_COND;
		B = b;
		Tc_if = tc_if;
		Tc_else = tc_else;
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

class LambdaExprAST: public ExprAST{
	ExprType type;
	vector<ExprAST*> Vb;
	ExprAST *E;
public:
	LambdaExprAST(vector<ExprAST*> vb, ExprAST *e){
		Vb = vb;
		E = e;
		type = E_LAMBDA;
	}
};

class WhereExprAST: public ExprAST{
	ExprType type;
	ExprAST *T;
	ExprAST *Dr;
public:
	WhereExprAST(ExprAST *t, ExprAST *dr){
		type = E_WHERE;
		T = t;
		Dr = dr;
	}
};

class InfixExprAST: public ExprAST{
	ExprType type;
	ExprAST *A;
	ExprAST *Identifier;
	ExprAST *B;
public:
	InfixExprAST(ExprAST *a,ExprAST *identifier, ExprAST *b){
		type = E_INFIX;
		A = a;
		Identifier = identifier;
		B = b;
	}
};

class WithinExprAST: public ExprAST{
	ExprType type;
	ExprAST *Da;
	ExprAST *D;
public:
	WithinExprAST(ExprAST *da, ExprAST *d){
		type = E_WITHIN;
		Da = da;
		D = d;
	}
};

class AndExprAST: public ExprAST{
	vector<ExprAST*> Dr;
	ExprType type;
public:
	AndExprAST(vector<ExprAST*> dr){
		Dr = dr;
		type = E_AND;
	}
};

class RecExprAST: public ExprAST{
	ExprType type;
	ExprAST *Db;
public:
	RecExprAST(ExprAST *db){
		type = E_REC;
		Db = db;
	}
};

class AssignExprAST: public ExprAST{
	ExprAST *Vl;
	ExprType type;
public:
	AssignExprAST(ExprAST *vl){
		Vl = vl;
		type = E_ASSIGN;
	}
};

class FuncExprAST: public ExprAST{
	ExprType type;
	ExprAST *Identifier;
	vector<ExprAST*> Vb;
	ExprAST *E;
public:
	FuncExprAST(ExprAST *identifier, vector<ExprAST*> vb, ExprAST *e){
		type = E_FUNC_FORM;
		Identifier = identifier;
		Vb = vb;
		E = e;
	}
};

class ParensExprAST: public ExprAST{
	ExprType type;
public:
	ParensExprAST(){
		type = E_PARENS;
	}
};


//class 

////////////////////////
/////AST GENERATE///////
////////////////////////

void Read(string st){
	gettok();
	if(lexeme != st){
		cout << "Error reading " << st << endl;
		exit(0);
	}
	gettok();
}

ExprAST *E(){
	ExprAST *AST_D;
	vector<ExprAST*> AST_Vb;
	ExprAST *AST_E;

	if(lexeme == "let" || lexeme == "fn"){
		if(lexeme == "let"){
			AST_D = D();
			Read("in");
			AST_E = E();
			return (new LetExprAST(AST_D,AST_E));
		}else{
			do{
				AST_Vb.push_back(Vb());
				gettok();
			}while(lexeme != ".");
			Read(".");
			AST_E = E();
			return (new LambdaExprAST(AST_Vb,AST_E));			
		}
	}
	return Ew();
}

ExprAST *Ew(){
	ExprAST *AST_T;
	ExprAST *AST_Dr;

	AST_T = T();
	gettok()
	if(lexeme == "where"){
		AST_Dr = Dr();
		return (new WhereExprAST(AST_T,AST_Dr));
	}

	return AST_T;
}

ExprAST *T(){
	vector<ExprAST*> AST_Ta;

	do{
		AST_Ta.push_back(Ta());
		gettok();
	}while(lexeme == ",");

	return (new TauExprAST(AST_Ta));
}

ExprAST *Ta(){
	ExprAST *AST_Ta;
	ExprAST *AST_Tc;
	ExprAST *AST_Left

	AST_Ta = Tc();
	gettok();
	AST_Left = AST_Tc;
	while(lexeme == "aug"){
		gettok();
		AST_Tc = Tc();
		AST_Left = new TauExprAST(AST_Left,AST_Tc);
		gettok();
	}
	return AST_Left;
}

ExprAST *Tc(){
	ExprAST *AST_B;
	ExprAST *AST_Tc_if;
	ExprAST *AST_Tc_else;

	AST_B = B();
	gettok();
	if(lexeme == "->"){
		gettok();
		AST_Tc_if = Tc();
		Read("|");
		AST_Tc_else = Tc();
		return (new CondExprAST(AST_B,AST_Tc_if,AST_Tc_else));
	}

	return (new CondExprAST(AST_B,AST_Tc_if,AST_Tc_else));
}

ExprAST *B(){
	
}


void Mainloop(){
	ExprAST *AST;
	switch(gettok()){
		case END_OF_FILE: exit(0); break;
		default: AST = E(); break
	}

	//print(AST);
}

int main(){
	in.open("input");
	out.open("output");
	TYPE t;

	list *root, *head;
	root = NULL;
	token temp_token;
	list *temp_list;
/*
	while((t = gettok()) != END_OF_FILE){
		temp_list = new list;
		
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
*/
	in.close();
	out.close();

	return 0;
}
