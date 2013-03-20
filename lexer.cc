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
TYPE Token_type;
char lastChar = ' ';

TYPE gettok(){
	char ch;
	lexeme = " ";

	//cout << lastChar << endl;
	ch = lastChar;
	if(ch == EOF){
		Token_type = END_OF_FILE;
		return END_OF_FILE;
	}
	
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
		Token_type = IDENTIFIER;
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
		Token_type = INTEGER;
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
		Token_type = STRING;
		return STRING;
	}
	//commnent
	else if(ch == '/'){
		if(in.peek() != '/'){
			Token_type = OPERATOR;
			return OPERATOR;
		}

		ch = in.get();
		ch = in.get();
		while(ch != '\n'){
			lexeme += ch;
			ch = in.get();			
		}
		lastChar = ch;
		Token_type = COMMENT;
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
		Token_type = OPERATOR;
		return OPERATOR;
	}

	//BRACKET_OPEN
	else if(ch == '('){
		lexeme += ch;
		lastChar = in.get();
		Token_type = BRACKET_OPEN;
		return BRACKET_OPEN;
	}

	//BRACKET_CLOSE
	else if(ch == ')'){
		lexeme += ch;
		lastChar = in.get();
		Token_type = BRACKET_CLOSE;
		return BRACKET_CLOSE;
	}

	//SEMICOLON: [:]
	else if(ch == ';'){
		lexeme += ch;
		lastChar = in.get();
		Token_type = SEMICOLON;
		return SEMICOLON;
	}

	//COMMA: [,]
	else if(ch == ','){
		lexeme += ch;
		lastChar = in.get();
		Token_type = COMMA;
		return COMMA;
	}

	lastChar = ch;
	Token_type = ERROR;
	return ERROR;
}

/////////////////////
////AST NODES////////
/////////////////////

bool RN_FLAG = 0, VL_FLAG = 0, VB_FLAG = 0;

enum ExprType { E_NIL, E_DUMMY, E_INTEGER, E_IDENTIFIER, E_BOOLEAN, E_BINOP, E_UNARYOP, E_STRING, E_LET, E_LAMBDA, E_WHERE, E_TAU, E_AUG, E_COND, E_GAMMA, E_INFIX, E_WITHIN, E_AND, E_REC, E_ASSIGN, E_FUNC_FORM, E_PARENS, E_LIST, E_ERROR, E_VARLIST };

class ExprAST{
public:
	virtual ~ExprAST() {}
	virtual ExprType getType(){
		return E_ERROR;
	};
	virtual vector<ExprAST*> *getVector(){
		return NULL;
	}
};

ExprAST *D(); ExprAST *Dr(); ExprAST *Da(); ExprAST *Db(); ExprAST *E(); ExprAST *Ew(); ExprAST *T(); ExprAST *Ta(); ExprAST *Tc(); ExprAST *B(); ExprAST *Bt(); ExprAST *Bs(); ExprAST *Bp(); ExprAST *A(); ExprAST *At(); ExprAST *Af(); ExprAST *Ap(); ExprAST *R(); ExprAST *Rn(); ExprAST *Vb(); ExprAST *Vl();

class IntegerExprAST: public ExprAST{
public:
	ExprType type;
	int Val;
	IntegerExprAST(int val){
		type = E_INTEGER;
		Val = val;
	}
};

class IdentifierExprAST: public ExprAST{
public:
	ExprType type;
	string Val;
	IdentifierExprAST(string val){
		type = E_IDENTIFIER;
		Val = val;
	}

	ExprType getType(){
		return type;
	}
};

class StringExprAST: public ExprAST{
public:
	ExprType type;
	string Val;
	StringExprAST(string val){
		type = E_STRING;
		Val = val;
	}

	ExprType getType(){
		return type;
	}
};

class BooleanExprAST : public ExprAST{
public:
	ExprType type;
	bool Val;
	BooleanExprAST(bool val){
		type = E_BINOP;
		Val = val;
	}

	ExprType getType(){
		return type;
	}
};

class NilExprAST : public ExprAST{
public:
	ExprType type;
	NilExprAST(){
		type = E_NIL;
	}

	ExprType getType(){
		return type;
	}
};

class DummyExprAST : public ExprAST{
public:
	ExprType type;
	DummyExprAST(){
		type = E_DUMMY;
	}

	ExprType getType(){
		return type;
	}
};

class TauExprAST: public ExprAST{
public:
	ExprType type;
	vector<ExprAST*> Ta;
	TauExprAST(vector<ExprAST*> ta){
		type = E_TAU;
		Ta = ta;
	}

	ExprType getType(){
		return type;
	}
};

class AugExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *Ta;
	ExprAST *Tc;
	AugExprAST(ExprAST *ta, ExprAST *tc){
		type = E_AUG;
		Ta = ta;
		Tc = tc;
	}

	ExprType getType(){
		return type;
	}
};

class CondExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *B;
	ExprAST *Tc_if;
	ExprAST *Tc_else;
	CondExprAST(ExprAST *b, ExprAST *tc_if, ExprAST *tc_else){
		type = E_COND;
		B = b;
		Tc_if = tc_if;
		Tc_else = tc_else;
	}

	ExprType getType(){
		return type;
	}
};


enum BINOP { B_PLUS, B_MINUS, B_MUL, B_DIV, B_NEG, B_OR, B_AND, B_NOT, B_GR, B_GE, B_LS, B_LE, B_EQ, B_NE, B_AUG, B_EXP};

enum UNARYOP { U_NOT, U_NEG };

class BinaryOpExprAST : public ExprAST{
public:
	ExprType type;
	ExprAST *Left;
	ExprAST *Right;
	BINOP OP;
	BinaryOpExprAST(ExprAST *left, ExprAST *right,BINOP op) {
		type = E_BINOP;
		Left = left;
		Right = right;
		OP = op;
	}

	ExprType getType(){
		return type;
	}
};

class UnaryOpExprAST : public ExprAST{
public:
	ExprType type;
	ExprAST *Node;
	UNARYOP OP;
	UnaryOpExprAST(ExprAST *node, UNARYOP op){
		type = E_UNARYOP;
		Node = node;
		OP = op;
	}

	ExprType getType(){
		return type;
	}
};

class LetExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *D;
	ExprAST *E;
	LetExprAST(ExprAST *d, ExprAST *e){
		type = E_LET;
		D = d;
		E = e;
	}

	ExprType getType(){
		return type;
	}
};

class LambdaExprAST: public ExprAST{
public:
	ExprType type;
	vector<ExprAST*> Vb;
	ExprAST *E;
	LambdaExprAST(vector<ExprAST*> vb, ExprAST *e){
		Vb = vb;
		E = e;
		type = E_LAMBDA;
	}

	ExprType getType(){
		return type;
	}
};

class WhereExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *T;
	ExprAST *Dr;
	WhereExprAST(ExprAST *t, ExprAST *dr){
		type = E_WHERE;
		T = t;
		Dr = dr;
	}

	ExprType getType(){
		return type;
	}
};

class InfixExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *A;
	ExprAST *Identifier;
	ExprAST *B;
	InfixExprAST(ExprAST *a,ExprAST *identifier, ExprAST *b){
		type = E_INFIX;
		A = a;
		Identifier = identifier;
		B = b;
	}

	ExprType getType(){
		return type;
	}
};

class WithinExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *Da;
	ExprAST *D;
	WithinExprAST(ExprAST *da, ExprAST *d){
		type = E_WITHIN;
		Da = da;
		D = d;
	}

	ExprType getType(){
		return type;
	}
};

class AndExprAST: public ExprAST{
public:
	vector<ExprAST*> Dr;
	ExprType type;
	AndExprAST(vector<ExprAST*> dr){
		Dr = dr;
		type = E_AND;
	}

	ExprType getType(){
		return type;
	}
};

class RecExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *Db;
	RecExprAST(ExprAST *db){
		type = E_REC;
		Db = db;
	}

	ExprType getType(){
		return type;
	}
};

class AssignExprAST: public ExprAST{
public:
	ExprAST *Vl;
	ExprAST *E;
	ExprType type;
	AssignExprAST(ExprAST *vl, ExprAST *e){
		Vl = vl;
		E = e;
		type = E_ASSIGN;
	}

	ExprType getType(){
		return type;
	}
};

class FuncExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *Identifier;
	vector<ExprAST*> Vb;
	ExprAST *E;
	FuncExprAST(ExprAST *identifier, vector<ExprAST*> vb, ExprAST *e){
		type = E_FUNC_FORM;
		Identifier = identifier;
		Vb = vb;
		E = e;
	}

	ExprType getType(){
		return type;
	}

};

class ParensExprAST: public ExprAST{
public:
	ExprType type;
	ParensExprAST(){
		type = E_PARENS;
	}

	ExprType getType(){
		return type;
	}
};

class GammaExprAST: public ExprAST{
public:
	ExprType type;
	ExprAST *Left;
	ExprAST *Right;
	GammaExprAST(ExprAST *left,ExprAST *right){
		type = E_GAMMA;
		Left = left;
		Right = right;
	}

	ExprType getType(){
		return type;
	}
};

class VariableExprAST: public ExprAST{
public:
	ExprType type;
	vector<ExprAST*> List;
	VariableExprAST(vector<ExprAST*> list){
		type = E_VARLIST;
		List = list;
	}

	ExprType getType(){
		return type;
	}

	vector<ExprAST*> *getVector(){
		return &List;
	}
};

class ErrorExprAST: public ExprAST{
	ExprType type;
public:
	ErrorExprAST() {
		type = E_ERROR;
	}

	ExprType getType(){
		return type;
	}
};

//class 

////////////////////////
/////AST GENERATE///////
////////////////////////

void p_error(string st){
	cout << "Parse Error: " << st << endl;
	exit(0);
}

void Read(string st){
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
			gettok();
			do{
				AST_Vb.push_back(Vb());
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
	}while(lexeme == ",");

	return (new TauExprAST(AST_Ta));
}

ExprAST *Ta(){
	ExprAST *AST_Ta;
	ExprAST *AST_Tc;
	ExprAST *AST_Left;

	AST_Ta = Tc();
	AST_Left = AST_Ta;
	while(lexeme == "aug"){
		gettok();
		AST_Tc = Tc();
		AST_Left = new AugExprAST(AST_Left,AST_Tc);
	}
	return AST_Left;
}

ExprAST *Tc(){
	ExprAST *AST_B;
	ExprAST *AST_Tc_if;
	ExprAST *AST_Tc_else;

	AST_B = B();
	if(lexeme == "->"){
		gettok();
		AST_Tc_if = Tc();
		Read("|");
		AST_Tc_else = Tc();
		return (new CondExprAST(AST_B,AST_Tc_if,AST_Tc_else));
	}

	return AST_B;
}

ExprAST *B(){
	ExprAST *AST_B;
	ExprAST *AST_Bt;

	AST_B = Bt();
	while(lexeme == "or"){
		gettok();
		AST_Bt = Bt();
		AST_B = new BinaryOpExprAST(AST_B,AST_Bt,B_OR);
	}

	return AST_B;
}

ExprAST *Bt(){
	ExprAST *AST_Bt;
	ExprAST *AST_Bs;

	AST_Bt = Bs();
	while(lexeme == "&"){
		gettok();
		AST_Bs = Bs();
		AST_Bt = new BinaryOpExprAST(AST_Bt,AST_Bs,B_AND);
	}

	return AST_Bt;
}

ExprAST *Bs(){
	ExprAST *AST_Bp;

	if(lexeme == "not"){
		AST_Bp = Bp(); 
		return (new UnaryOpExprAST(AST_Bp,U_NOT));
	}else{
		return Bp();
	}
}

ExprAST *Bp(){
	ExprAST *AST_Left;
	ExprAST *AST_Right;

	AST_Left = A();
	if(lexeme == "gr" && lexeme == ">"){
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_GR));
	}else if(lexeme == "ge" && lexeme == ">="){
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_GE));
	}else if(lexeme == "ls" && lexeme == "<"){
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_LS));
	}else if(lexeme == "le" && lexeme == "<="){
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_LE));
	}else if(lexeme == "eq"){
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_EQ));
	}else if(lexeme == "ne"){
		AST_Right = A();
		return (new BinaryOpExprAST(AST_Left,AST_Right,B_NE));
	}

	return AST_Left;
}

ExprAST *A(){
	ExprAST *AST_A;
	ExprAST *AST_At;

	if(lexeme == "+" || lexeme == "-"){
		if(lexeme == "+"){
			return At();
		}else{
			AST_At = At();
			return (new UnaryOpExprAST(AST_At,U_NEG)); 
		}
	}else{
		AST_A = At();
		while(lexeme == "+" || lexeme == "-"){
			if(lexeme == "+"){
				AST_At = At();
				AST_A = new BinaryOpExprAST(AST_A,AST_At,B_PLUS);
			}else{
				AST_At = At();
				AST_A = new BinaryOpExprAST(AST_A,AST_At,B_MINUS);
			}
		}

		return AST_A;
	}
}

ExprAST *At(){
	ExprAST *AST_At;
	ExprAST *AST_Af;

	AST_At = Af();
	while(lexeme == "*" || lexeme == "/"){
		if(lexeme == "/"){
			AST_Af = Af();
			AST_At = new BinaryOpExprAST(AST_At,AST_Af,B_MUL);
		}else{
			AST_Af = Af();
			AST_At = new BinaryOpExprAST(AST_At,AST_Af,B_DIV);
		}
	}

	return AST_At;	
}

ExprAST *Af(){
	ExprAST *AST_Ap;
	ExprAST *AST_Af;

	AST_Ap = Ap();
	if(lexeme == "**"){
		AST_Af = Af();
		AST_Ap = new BinaryOpExprAST(AST_Ap,AST_Af,B_EXP);
	}

	return AST_Ap;
}

ExprAST *Ap(){
	ExprAST *AST_Ap;
	ExprAST *AST_R;
	ExprAST *AST_Id;

	AST_Ap = R();
	while(lexeme == "@"){
		if(gettok() == IDENTIFIER){
			AST_Id = new IdentifierExprAST(lexeme);
			gettok();
			AST_R = R();
			AST_Ap = new InfixExprAST(AST_Ap,AST_Id,AST_R);
		}
	}

	return AST_Ap;
}

ExprAST *R(){
	ExprAST *AST_R;
	ExprAST *AST_Rn;

	AST_R = Rn(); 
	if(RN_FLAG == 1){
		RN_FLAG = 0;
		AST_Rn = Rn();
		while(RN_FLAG == 1){
			RN_FLAG = 0;
			AST_R = new GammaExprAST(AST_R,AST_Rn);
			AST_Rn = Rn();
		}
	}
	RN_FLAG = 0;
	return AST_R;
}

ExprAST *Rn(){
	ExprAST *AST_E;

	switch(gettok()){
		case IDENTIFIER: RN_FLAG = 1; 
						 if(lexeme == "true" || lexeme == "false"){
						 	if(lexeme == "true"){
						 		gettok();
						 		return (new BooleanExprAST(true));
						 	}else{
						 		gettok();
						 		return (new BooleanExprAST(true));
						 	}
						 }else if(lexeme == "nil"){
						 	gettok();
						 	return (new NilExprAST());
						 }else if(lexeme == "dummy"){
						 	gettok();
						 	return (new DummyExprAST());
						 }else{
						 	gettok();
						 	return (new IdentifierExprAST(lexeme)); break;
						 }
		case INTEGER: RN_FLAG = 1; gettok(); return (new IntegerExprAST(atoi(lexeme.c_str()))); break;
		case STRING: RN_FLAG = 1; gettok(); return (new StringExprAST(lexeme)); break;
		case BRACKET_OPEN: RN_FLAG = 1;
						   gettok();
						   AST_E = E();
						   if(Token_type != BRACKET_CLOSE){
						   		p_error(")");
						   }
						   gettok();
						   return AST_E;
						   break;

		default : RN_FLAG = 0; gettok();
	}
	return (new ErrorExprAST());
}

ExprAST *D(){
	ExprAST *AST_Da;
	ExprAST *AST_D;

	AST_Da = Da();
	if(lexeme == "within"){
		AST_D = D();
		AST_Da = new WithinExprAST(AST_Da, AST_D);
	}

	return AST_Da;
}

ExprAST *Da(){
	vector<ExprAST*> AST_Dr;

	do{
		AST_Dr.push_back(Dr());
	}while(lexeme == "and");

	return (new AndExprAST(AST_Dr));
}

ExprAST *Dr(){
	ExprAST *AST_Dr;

	if(lexeme == "rec"){
		AST_Dr = Dr();
		return (new RecExprAST(AST_Dr));
	}
	AST_Dr = Dr();

	return AST_Dr;
}

ExprAST *Db(){
	ExprAST *AST_Vl;
	ExprAST *AST_E;
	vector<ExprAST*> AST_Vb;
	ExprAST *AST_D;

	AST_Vl = Vl();
	if(VL_FLAG == 1){	//left
		gettok();
		if(lexeme == "="){
			AST_E = E();
			return (new AssignExprAST(AST_Vl,AST_E));
		}
		gettok();
		do{
			AST_Vb.push_back(Vb());
		}while(lexeme != "=");
		AST_E = E();
		return (new FuncExprAST(AST_Vl,AST_Vb,AST_E));
	}
	if(Token_type == BRACKET_OPEN){
		gettok();
		AST_D = D();
		if(Token_type != BRACKET_CLOSE) p_error(")");
		return AST_D;
	}
	p_error("Nothing to parse");
	VL_FLAG = 0;
	VB_FLAG = 0;
	return (new ErrorExprAST());
}

ExprAST *Vl(){
	ExprAST *AST_Id;
	vector<ExprAST*> AST_List;

	//cout << lexeme << " : " << Token_type << endl;

	if(Token_type == IDENTIFIER){
		AST_List.push_back(new IdentifierExprAST(lexeme));
		while(gettok() == COMMA){
			if(gettok() != IDENTIFIER) p_error("no identifier found");
			AST_List.push_back(new IdentifierExprAST(lexeme));
		}
		VL_FLAG = 1;
		//cout << AST_List.size() << endl;
		if(AST_List.size() < 2)
			return (AST_List[0]);
		else
			return (new VariableExprAST(AST_List));
	}
	VL_FLAG = 0;
	return (new ErrorExprAST());
}

ExprAST *Vb(){
	ExprAST *AST_Vl;

	switch(Token_type){
		case IDENTIFIER: VB_FLAG = 1; return (new IdentifierExprAST(lexeme)); break;
		case BRACKET_OPEN: gettok(); AST_Vl = Vl(); if(VL_FLAG == 1){
								if(Token_type == BRACKET_OPEN) p_error(")");
								gettok();
								return AST_Vl;
		          		   }
		          		   if(Token_type != BRACKET_CLOSE) p_error(")");
		          		   gettok();
		          		   VB_FLAG = 1;
		          		   return (new ParensExprAST()); break;
		default: VB_FLAG = 0; return (new ErrorExprAST());
	}

}


void print(ExprAST *AST, int DEPTH){
	DEPTH++;
	ExprType type = AST->getType();
	for(int i=0; i<DEPTH;i++)
			cout << "*" ;
	//cout << AST->getType() << endl;
	if( type == E_VARLIST){
		//cout << "Variable List" << endl; 
				
		VariableExprAST *list = (VariableExprAST*)AST; 
		cout << "," << endl;
		for(int i = 0;i<list->List.size();i++){
			print(list->List[i], DEPTH);
		}
	}else if(type == E_IDENTIFIER){
		IdentifierExprAST *id = (IdentifierExprAST*)AST;
		cout << id->Val;
	}else if(type == E_PARENS){
		cout << "()";
	}

	cout << endl;
}

void Mainloop(){
	ExprAST *AST;
	switch(gettok()){
		case END_OF_FILE: exit(0); break;
		default: AST = Vb(); break;
	}

	print(AST, -1);
}

int main(){
	in.open("test");
	out.open("output");
	TYPE t;

	list *root, *head;
	root = NULL;
	token temp_token;
	list *temp_list;

	Mainloop();
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
