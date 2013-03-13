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
	union {
		int num;
		char *str;
	} value;
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
		lastChar = in.get();
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

int main(){
	in.open("input");
	out.open("output");
	TYPE t;
	while((t = gettok()) != END_OF_FILE){
		if(t == IDENTIFIER){
			cout << "identifier: " << lexeme << endl;
		}
		else if(t == INTEGER){
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
	}

	in.close();
	out.close();

	return 0;
}