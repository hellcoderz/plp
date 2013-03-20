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

#include "parser.cc"

using namespace std;

void Mainloop(){
	ExprAST *AST;

	switch(gettok()){
		case END_OF_FILE: exit(0); break;
		default: AST = E(); break;
	}

	print(AST, -1);
}

void print_prog(){
	char ch;
	ch = in.get();
	while(ch != EOF){
		cout << ch ;
		ch = in.get();
	}
}

int main(int argc,char **argv){
	
	cout << argc << endl;
	if(argc == 3){
		in.open(argv[2]);
		if(strcmp(argv[1],"-ast"))
			print_prog();
		else if(strcmp(argv[1],"-l"))
			Mainloop();
	}
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
			//cout << "Root is NULL" << endl;
			temp_list->data = new token;
			temp_list->data->type = t;
			temp_list->data->value = lexeme;
			temp_list->next = NULL;
			head = temp_list;
			root = head;
		}else{
			//cout << "Root is not NULL" << endl;
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
