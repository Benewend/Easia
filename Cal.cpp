#include <iostream>
#include "Tokenizer.h"
#include "parser.h"
#include <string>

using namespace std;



int main(int argc, char const *argv[]){
	if(argc>=2){
		Vector  Letter_stream = extract(argv[1]);
		Letter_stream = preprocess(Letter_stream);
		Lexer lexi =Lexer(Letter_stream);
		T_stream Tokens;
		Tokens =lexi.scan();
		Parser* prs= new Parser(Tokens);
		Root* AST =prs->parse();
		SymbolTable* st = new SymbolTable(NULL);
		AST->visit(st);
	}else{
		cout<<"ERROR: no  file name argument was provided"<<endl;
		exit(1);
	}
	return 0;
}