#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string.h>

using namespace std;

// trim string from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim string from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim string from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

//enumeration types for token types
enum TYPE{
	IDENTIFIER, INTEGER, OPERATOR, STRING, BRACKET_OPEN, BRACKET_CLOSE, SEMICOLON, COMMA, END_OF_FILE, ERROR, COMMENT
};

//struct to hold data for a token
//not used in this program but will be used in next phase
struct token{
	TYPE type;
	string value;
};

//list to hold all tokens
//not used due to inefficient choice
struct list{
	token *data;
	list *next;
};

//input file declaration 
ifstream in;

//global variable for storing current value of Token and its type
string lexeme;
TYPE Token_type;

//character to hold current read character 
char lastChar = ' ';

//function to get a token with its type and return its type while storing both value
//and type in variables 'lexeme' and 'Token_type' respectively
TYPE gettok(){
	char ch;
	lexeme = " ";

	//read character
	ch = lastChar;

	//check if its EOF
	if(ch == EOF){
		Token_type = END_OF_FILE;
		//cout << "----------Token read:" << lexeme << endl;
		return END_OF_FILE;
	}
	
	//skip whitespace
	while(isspace(ch) || ch == '\n' || ch == '\t'){
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
		lexeme = trim(lexeme);
		lastChar = ch;
		Token_type = IDENTIFIER;
		//cout << "----------Token read:" << lexeme << endl;
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
		lexeme = trim(lexeme);
		lastChar = ch;
		Token_type = INTEGER;
		//cout << "----------Token read:" << lexeme << endl;
		return INTEGER;
	}
	//cout << ch << endl;

	//string
	else if(ch == '\''){
		ch = in.get();
		lexeme = trim(lexeme);
		while(ch != '\''){
			lexeme += ch;
			ch = in.get();
		}

		lastChar = in.get();
		Token_type = STRING;
		//cout << "----------Token read:" << lexeme << endl;
		return STRING;
	}
	//commnent
	else if(ch == '/'){
		if(in.peek() != '/'){
			lexeme = "/";
			lexeme = trim(lexeme);
			lastChar = in.get();
			Token_type = OPERATOR;
			//cout << "----------/Token read:" << lexeme << endl;
			return OPERATOR;
		}

		ch = in.get();
		while(ch != '\n'){
			lexeme += ch;
			ch = in.get();			
		}
		lexeme = trim(lexeme);
		lastChar = in.get();
		Token_type = COMMENT;
		//cout << "----------Token read:" << lexeme << endl;
		return gettok();
	}

	//operator
	else if(ch == '+' || ch == '-' || ch == '*' || ch == '<' || ch == '>' || ch == '&' || ch == '.' || ch == '@' || ch == '/' || ch == ':' || ch == '=' || ch == '~' || ch == '|' || ch == '$' || ch == '!' || ch == '#' || ch == '%' || ch == '^' || ch == '_' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '"' || ch == '`' || ch == '?'){
		lexeme += ch;
		ch = in.get();
		while(ch == '+' || ch == '-' || ch == '*' || ch == '<' || ch == '>' || ch == '&' || ch == '.' || ch == '@' || ch == '/' || ch == ':' || ch == '=' || ch == '~' || ch == '|' || ch == '$' || ch == '!' || ch == '#' || ch == '%' || ch == '^' || ch == '_' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '"' || ch == '`' || ch == '?'){
			lexeme += ch;
			ch = in.get();
		}
		lexeme = trim(lexeme);	
		lastChar = ch;
		Token_type = OPERATOR;
		//cout << "----------Token read:" << lexeme << endl;
		return OPERATOR;
	}

	//BRACKET_OPEN
	else if(ch == '('){
		lexeme += ch;
		lastChar = in.get();
		lexeme = trim(lexeme);
		Token_type = BRACKET_OPEN;
		//cout << "----------Token read:" << lexeme << endl;
		return BRACKET_OPEN;
	}

	//BRACKET_CLOSE
	else if(ch == ')'){
		lexeme += ch;
		lexeme = trim(lexeme);
		lastChar = in.get();
		Token_type = BRACKET_CLOSE;
		//cout << "----------Token read:" << lexeme << endl;
		return BRACKET_CLOSE;
	}

	//SEMICOLON: [:]
	else if(ch == ';'){
		lexeme += ch;
		lastChar = in.get();
		lexeme = trim(lexeme);
		Token_type = SEMICOLON;
		//cout << "----------Token read:" << lexeme << endl;
		return SEMICOLON;
	}

	//COMMA: [,]
	else if(ch == ','){
		lexeme += ch;
		lexeme = trim(lexeme);
		lastChar = in.get();
		Token_type = COMMA;
		//cout << "----------Token read:" << lexeme << endl;
		return COMMA;
	}
	lexeme = trim(lexeme);
	lastChar = ch;
	Token_type = ERROR;

	//cout << "----------Token read:" << lexeme << endl;
	//no match found
	return ERROR;
}