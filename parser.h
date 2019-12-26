#ifndef PARSER_H
#define PARSER_H 

#include<vector>
#include <iostream>
#include <string>
#include "Tokenizer.h"

typedef std::vector<Token*> T_stream;
//this class is the base class of the warper of the AST node 
class Out{
public:
	int type;
	Out(int t);
	~Out();
};
//the warper of the AST node that are dealing with int and float
class NOut:public Out{
public:
	float value;
	NOut(int t, float val);
	~NOut();
	
};
//the warper of the AST node that are dealing with strings
class SOut:public Out{
public:
	std::string value;
	int count;
	SOut();
	SOut(int t, std::string val);
	SOut(std::string s,int  count);
	~SOut();
	
};
//This unit use to fill the symbolTable
class Symbol{
public:
	Word* contained;
	Out* ValType;
	Symbol(Word* name, Out* vs);
	Symbol(Symbol* got);
	~Symbol();
	
};

//The symbole table that contains all the the symbols- information about all of the declareted variable
class SymbolTable{
		std::string Tmatcher(int i);
public:
	std::vector<Symbol*> Stable;
	SymbolTable* previous ;
	SymbolTable(SymbolTable* prev=NULL);
	~SymbolTable();
	float defineVar(Symbol* s);
	Symbol* Search(Word*v);
	void printTable();
};

//Base class for all the node of the AST

class Root{
public:
	int T;/*BinaryOP=1 Numb =2 */
	Root(int i =0);
	virtual Out* visit(SymbolTable* st);
	bool typeCheck(int first,int second=293);
};

//Node for the unary operation
class Unary:public Root{
public:
	Token* sign;
	Root*right;
	Unary();
	Unary(Token* S, Root* R);
	Out* visit(SymbolTable* st);
	~Unary();
};


//node for the the binary operator
class BinaryOP:public Root{
	Root* left;
	Token*op;
	Root * right; 
public:
	BinaryOP();
	BinaryOP(Root* L, Token *Opera, Root * R);
	Out* visit(SymbolTable* st);
	~BinaryOP();
	
};
//node for number ints 
class Numb:public Root{
public:
	Token* token;
	float  value;
	Numb();
	Numb(Token* t);
	Out* visit(SymbolTable* st);
	~Numb();
	
};

//node for booleans 
class Bool:public Root{
public:
	Token* token;
	float value;
	Bool(Token* t, float val);
	~Bool();
	Out* visit(SymbolTable* st);
	
};

//node for floats
class FLoat:public Root{
public:
	Token* token;
	float value;
	FLoat(Token* t);
	~FLoat();
	Out* visit(SymbolTable* st);
	
};
//node for strings

class String:public Root {
public:
	STR* token;
	String(Token*t);
	~String();
	Out* visit(SymbolTable* st);
};
//node for the blocks
class BLOCK:public Root{
public:
	std::vector<Root*> Statements;
	BLOCK();
	~BLOCK();
	Out* visit(SymbolTable* st=NULL);
};

//node for assignation

class Assignation:public Root{
public:
	Word* left;
	Token* Assign;
	Root* right;
	Assignation(Word* var, Token* as, Root* right);
	Assignation(Word* l, Root* r);
	~Assignation();
	Out* visit(SymbolTable* st);
};
//node for negation
class Negation:public Root{
public:
	Token* token;
	Root* right;
	Negation(Token* t , Root* r);
	~Negation();
	Out* visit(SymbolTable*st);
	
};

//node for variable 
class Variable:public Root{
public:
	Word* token;
	std::string value;
	Variable(Word* t);
	~Variable();
	Out*visit(SymbolTable* st);
	
};
//node for null statements 
class Null:public Root{
public:
	Null();
	~Null();
	Out* visit(SymbolTable* st);
};

//node for when statement equivalent to the if statement in c like languages
class When:public Root{
public:
	Root* condition;
	Root* Blck;
	Root* other;
	When(Root*con,Root* b,Root* ot);
	When(Root*con,Root* b);
	~When();
	Out* visit(SymbolTable* st);
	
};

//node for for statement
class For:public Root{
public:
	Assignation* counter ;
	Root* start,*end,* step,*Block;
	For(Word* i, Root* st, Root* nd,Root* blck,Root* stp=NULL);
	~For();
	Out* visit(SymbolTable* st);
};
//node for "as long as" statement equivalent to the while statement in c like languages

class AsLoop:public Root{
public:
	Root* condition,*Block;
	AsLoop(Root* con,Root* Blck);
	~AsLoop();
	Out* visit(SymbolTable* st);
	
};
//show supposed to be the equivalent of the print in python or the cout in c++ but doesn't work yet

class Show:public Root {
	std::string Tmatcher(int i); //matching tags to types
public:
	Token* token;
	std::string value;
	Show(Token *r);
	~Show();
	Out* visit(SymbolTable* st);
};


//the class that is taking care of all the AST building
class Parser{
	T_stream Tokens;
	Token*Current;
	int index;
	std::string MatchToken(int t);
public:
	Parser(T_stream Ts);
	void error();
	void error(std::string m);
	void eat(int token,std::string function="");
	bool precedence(int tag, int pre);
	Root* factor();
	Root* term();
	Root* expr();
	Root* parse();
	Root* code();
	Root* block();
	std::vector<Root*> statement_list();
	Root* statement();
	Root* assign();
	Word* varName();
	Root* when();
	Root* ForLoop();
	Root* Shows();
	Root* other();
	Root* ASLong();
	~Parser();
	
};


#endif