#include<vector>
#include <iostream>
#ifndef TOKENIZER_H
#define TOKENIZER_H 
#include <string>


typedef std::vector<char> Vector;


//function to copy carrater from the file and store it in a token stream in
//which we ca look ahead
Vector extract(std::string file_name);

//This function takes out all the comment :both many line an sigle line comments
Vector preprocess(Vector v);

//helping function that prints the token stream 
void printv(Vector v);

//this class contains all the constant that we will use in the program
class tags{
  public:
  	int AND=256, BASIC=257,BREAK=258, DO=259, AS=260,Long=261,GE=262,
  		EQ=263,ID=264,WHEN=265/*,INDEX=265*/,LE=267,MINUS=268,PLUS=269,TIME=270
  		,DIVIDE=271,NUM=272,NE=273,OTHER=274,OR=275,TEMP=277
  		,CASE=279,KEY_W=281,Id=282,ASSIGN=283,STRING=284,FUNCTION=284
  		,E_O_F=285,new_l=287,numNode=288,binaryNode=289,POWER=290,INDENT=291
  		,DEDENT=292,REAL=293,Int=294,Bool=295,Not=296,False=297,True=298,FOR=299,GOING=300
  		,FROM=301,TO=302,BY=303,SHOW=304;
  	tags(){}
  };


//Base class for the different tokens 
class Token{
 public:
 	int line,tag;
 	Token();
 	Token(int t,int l);
 	virtual std::string toString();
 	
 };
 //vector use for storing the tokens
typedef std::vector<Token*> T_stream;

//class for tokens numbers 
 class Num:public Token{
 	
 public:
 	float value;
 	Num();
 	Num(int val, int l);
 	std::string toString();
 	
 };
//class for float numbers
 class Float:public Token{
 	public:
 	float value;
 	Float();
 	Float(float val, int l);
 	std::string toString();
 };

 //class for strings not tested yet :)

class STR:public Token{
 public:
 	int count;
 	std::string val;
 	STR();
 	STR(std::string v,int l);
 	
 	std::string toString();
 	
 };
//class for both key words and ids
 class Word:public Token{
 public:
 	std::string lexeme;
 	Word();
 	Word(Word*t,int line);
 	Word(std::string lexi,int tag,int l);///
 	Word(std::string lexi,int l=0);
 	void set(std::string lexi,int l=0);
 	
 	std::string toString();
 	
 };



//contains the key words and returns them if they are encotered in the 
 //source file
class Table{
Word** keyWords; 
int count;
public:
	Table();
	void add(Word *w);
	bool get(std::string w);
	Word * give(std::string s,int line);
};
//helping method that shows the the different tokens
void show(T_stream v);


//this is the class that is actully carrying out all the job by calling functions
class Lexer{
	void error(std::string s);
	void error(std::string s,int li);
public:
	std::vector<int> Stack;
	Table t;
	Vector c;
	Lexer(Vector c);
	T_stream scan();
	
	
};









#endif