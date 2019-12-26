#include "Tokenizer.h"
#include <iostream>
#include <fstream>
#include <string>
#include<sstream>
#include <fstream>

using namespace std;

tags Tag;//The object Tag contains all the tag we will be using at least those already encounter
typedef std::vector<Token*> T_stream; // the token stream



Vector extract(std::string file_name){//just extractinc the chars from the file this function will be 
	//improved in the next iteration of this specific file

	ifstream in(file_name);
	if(in.is_open()){
		Vector v ;
	 	char c;
	 	while(in.get(c)){
	 		//cout<<c;
			v.push_back(c);
	 	}
	 	return v;
	 }else{
	 	cout<<"ERROR: no such file in directory "<<endl;
	 	exit(1);
	 }
 }



Vector preprocess(Vector v){
 	int i=0;
 	Vector n_v;
	while(i<v.size()){
		switch (v.at(i)){
			case'#'://Taking out the one lines comments
				while((v.at(i)!='\n')&&(i<v.size()) ){
					i++;
				}
				n_v.push_back(v.at(i++)) ;
				break;
			case'<'://taking out the several lines comments
					if (v.at(i+1)=='<'){
						i+=2;
						while((v.at(i-1)!='>')&&(v.at(i-2)!='>')&&(i<v.size()) )
							i++;
							
							i+=1;
					}
					n_v.push_back(v.at(i++)) ;
					break;

			default: //appending the character that are not part of any type of comment
				n_v.push_back(v.at(i++))  ;
		
				break;
			}
		}
		
 	return n_v;
 }


 void printv(Vector v){//printing the vector of char(only for the testing of the two previous functions)
	for (int i = 0; i < v.size(); ++i){
		std::cout<<v.at(i);

	}
	std::cout<<std::endl;
}


Token::Token():line(0),tag(0){//constructor

	}

Token::Token(int t, int l):line(l),tag(t){//constructor

	}



std::string Token::toString(){ //matching some tokens to the their name instead fot their tags
	std::string go="";
		if((tag<128) and (tag!='\n') and (tag!='\t') ){
			go=static_cast<char>(tag);
		}else{
			switch(tag){
				case('\n'):
					go=" \\n ";
					break;
				case('\t'):
					go=" \\t";
					break;
				case(' '):
					go="SPACE";
					break;
				case(291):
					go="INDENT";
					break;

				case(292):
					go="DEDENT";
					break;
				default:
					go=to_string(tag);
			}

		}

	return  "("+go+") ";  //"||Token: "+go+" line:"+std::to_string(line)+" ||";

}
// implementation of tokens  constructors and destructors
Num::Num():value(0){

}


Num::Num(int val, int l):value(val),Token(Tag.NUM,l){
	
}


std::string Num::toString(){
	return "("+std::to_string(value)+") ";// "||Num:"+std::to_string(value)+" on line:"+std::to_string(line)+" ||";
}

Float::Float():value(0),Token(){//constructor

}

Float::Float(float val, int l):value(val),Token(Tag.REAL,l){//constructor

}

std::string Float::toString(){
	return " ("+std::to_string(value)+") ";//"||Float:"+std::to_string(value)+" on line:"+std::to_string(line)+" ||";
}

STR::STR(): count(0),val(""),Token(){//constructor
		
	}

STR::STR(std::string v, int l):Token(Tag.STRING,l),val(v),count(v.length()){
		
	}

std::string STR::toString(){
		return " (\""+val+"\") ";//"||string of"+std::to_string(count)+" char: "+val+" on line:"+std::to_string(line)+"||";
	}

Word::Word():lexeme(""),Token(){

}
Word::Word(Word*t,int line):lexeme(t->lexeme),Token(t->tag,line){

}
Word::Word(std::string lexi,int tag ,int l):lexeme(lexi),Token(tag,l){
 		
 	}
Word::Word(std::string lexi,int l):lexeme(lexi),Token(Tag.KEY_W,l){
 		
 	}
void Word::set(std::string lexi,int l){
	lexeme=lexi;
}

std::string Word::toString(){
		return "("+lexeme+") ";//"||Word: "+lexeme+" on line: "+std::to_string(line)+"||"+std::to_string(tag);
	}


//implementation of the class table (recall contains keywords)

Table::Table(){
 		count=0;
		keyWords = new Word*[40];
		add(new Word("by",Tag.BY,0));
		add(new Word("as",Tag.AS,0));
		add(new Word("gives"));
		add(new Word("for",Tag.FOR,0));
		add(new Word("try"));
		add(new Word("long",Tag.Long,0));
		add(new Word("other",Tag.OTHER,0));
		add(new Word("going",Tag.GOING,0));
		add(new Word("do"));
		add(new Word("show",Tag.SHOW,0));
		add(new Word("bring"));
		add(new Word("from",Tag.FROM,0));
		add(new Word("del"));
		add(new Word("catch"));
		add(new Word("func"));
		add(new Word("to",Tag.TO,0));
		add(new Word("case"));
		add(new Word("in"));
		add(new Word("Break"));
		add(new Word("is"));
		add(new Word("when",Tag.WHEN,0));
		add(new Word("true",Tag.True,0));
		add(new Word("false",Tag.False,0));
		add(new Word("and",Tag.AND,0));
		add(new Word("or",Tag.OR,0));
		add(new Word("not",Tag.Not,0));//std::string lexi,int tag,int l

}



void Table::add(Word* w){//adds a word pointer inside the table

	keyWords[count++]=w;


	}

bool Table::get(std::string w){//checks if a word pointer is inside the table
	for (int i = 0; i < count; ++i){
		if(keyWords[i]->lexeme.compare(w)==0)

			return true;
	}
	return false;
	}

Word* Table::give(std::string s,int line){//return if a word pointer is inside the table and is only use if 
	//we are sure that the array contains the word's pointer
		for (int i = 0; i < count; ++i){
			if(keyWords[i]->lexeme.compare(s)==0){
				Word* togive = new Word(keyWords[i],line);
				return togive;
		}	}
		return NULL;
	}


void show(T_stream v){
	std::ostringstream carray;
	T_stream::iterator Titer;//display the tokens for testing sake
	int line =0;
	for (Titer=v.begin() ; Titer!=v.end() ; Titer++){
		if((*Titer)->line>line){
			carray<<"\n";
			line=(*Titer)->line;
		}
		
		carray<<(*Titer)->toString();

	
	}
	std::string out =carray.str();
	cout<<"show functions";
	cout<<out<<endl;
}

Lexer::Lexer(Vector cs):c(cs),t(){
		Stack.push_back(0);
		c.push_back('-');//addind this last character to avoid out of bound error
	}

void Lexer::error(std::string s,int li ){
		cerr<<"LEXICAL error:"<< s <<": "<< to_string(li)<<" "<<endl;
		exit(1);
	}

void Lexer::error(std::string s ){
		cerr<<"LEXICAL error:"<< s <<endl;
		exit(1);
	}


T_stream Lexer::scan(){
	int i=0,indent=0, ival=0,decimal=0,strgcount=0, range=10,line=1 ;
	float fval=0;
	bool begin=false;
	T_stream Tokens ;
	Table t;
	std::ostringstream carray; 
	Tokens.push_back(new Token(Tag.INDENT,line));
	while(i<c.size()){
		if((c.at(i)=='\t')&&(begin)){
				i++; //taking out the tabulation that occuring after the line has started
			}
		if (c.at(i)==' '){
			if(!begin){
				error( "Take out the space at the begining of this line", line);
				//taking care of the space at the begining of the line according to easia rules
			}else{ i++;}
		}else{
			if((c.at(i)=='\"')){ //matching the strings 
					i++;
					int LetterCount=0;
					do{	
							LetterCount++;
						if(c.at(i)=='\\'){ ///in this block character are escaped
							i++;
							carray<<'\\';
							carray<<c.at(i);
							i++;
						}else{
							carray<<c.at(i);
							i++;
						}
						
						
					}while((c.at(i)!='\"')); 
					i++;
					STR* stemp = new STR(carray.str(),line);
					stemp->count =LetterCount;
					Tokens.push_back(stemp);
					carray.str("");
					carray.clear();
					begin=true;
			}else{
				if((isalpha(c.at(i)))||(c.at(i)=='_')){ //matching the id;
					do{
						carray<<c.at(i);
						i++;
					}while((isalpha(c.at(i)))||(isdigit(c.at(i)))||(c.at(i)=='_'));
					std::string out =carray.str();
					if(t.get(out)){ //if statement checks if it is a keyword 
						Tokens.push_back(t.give(out,line));
						carray.str("");
						carray.clear();
						begin=true;
					}else{
						Tokens.push_back(new Word(out,Tag.ID,line));//if not a simple word is pushed in the token stream 
						carray.str("");
						carray.clear();
						begin=true;
						}
				}else{
					if(isdigit(c.at(i))){//matching numbers and floats 

						
						ival+=c.at(i)-48;
						i++;
						while(isdigit(c.at(i))){
							ival*=10;
							ival+=c.at(i)-48;
							i++;
						}
						if(c.at(i)=='.'){
							i++;
							fval = ival;
							while(isdigit(c.at(i))){
								decimal=c.at(i)-48;
								decimal*=10;
								range*=10;
								i++;
							}
							fval+=(float)(decimal)/range;
							Tokens.push_back(new Float(fval,line));
							fval=0;
							ival=0;
							begin=true;
						}else{	
								Tokens.push_back(new Num(ival,line));
								begin=true;
								ival=0;
							}
					}else{
						switch(c.at(i)){
							case('<'):{//matching assignation and tokens starting by <
								if(c.at(i+1)=='-'){
									i+=2;
									Tokens.push_back(new Token(Tag.ASSIGN,line));
									begin=true;
								}else{
									if(c.at(i+1)=='='){
										i+=2;
										Tokens.push_back(new Token(Tag.LE,line));
										begin=true;
									}else{
										i++;
										Tokens.push_back(new Token('<',line));
										begin=true;
										}
									}
								}
								break;
							case('>'):{ //matching tokens starting by >
								if(c.at(i+1)=='='){
									i+=2;
									Tokens.push_back(new Token(Tag.GE,line));
									begin=true;
								}else{
									Tokens.push_back(new Token('>',line));
									begin=true;
									i++;
								}
								}break;
							case('\n'):{//taking care of new lines
								Tokens.push_back(new Token(Tag.new_l,line));
								begin=false;
								line++;
								i++;
								if(c.at(i)=='\t'){
									do{
										indent++;
										i++;
									}while(c.at(i)=='\t');
									int n =Stack.size()-1;
									if(indent>Stack[n]){
										Tokens.push_back(new Token(Tag.INDENT,line));
										Stack.push_back(indent);
										indent=0;
									}else{
										if(indent<Stack[n]){
											int count =0;
											int n = Stack.size()-1;
											do{
												count++;
												n--;
												Stack.pop_back();
											}while(indent<Stack[n]);
											if(indent!=Stack[n]){
												Stack.push_back(indent);
											}
											for(int i=0;i<count;i++){
												Tokens.push_back(new Token(Tag.DEDENT,line));
												Tokens.push_back(new Token(Tag.new_l,line));

											}
											indent=0;

										}else{
												indent=0;
											}
										
											
										
											
										}
									
									
								}else{
									int count =0;
									int n = Stack.size()-1;
										while(indent<Stack[n]){
											count++;
											n--;
											Stack.pop_back();
										}
										//taking care of virtual token Dedent tha is the delimiter of blocks
										for(int i=0;i<count;i++){
											Tokens.push_back(new Token(Tag.DEDENT,line));
											Tokens.push_back(new Token(Tag.new_l,line));
										}
										
										indent=0;

									}
								}
								break;
								//the rest of the code matches operator
							case('*'):{
								if(c.at(i+1)=='*'){
									i+=2;
									Tokens.push_back(new Token(Tag.POWER,line));
								}else{
									Tokens.push_back(new Token('*',line));
									i++;
								}
							}break;
							case('&'):{
								if(c.at(i+1)=='&'){
									i+=2;
									Tokens.push_back(new Token(Tag.AND,line));
								}else{
									Tokens.push_back(new Token('&',line));
									i++;
								}
							}break;
							case('|'):{
								if(c.at(i+1)=='|'){
									i+=2;
									Tokens.push_back(new Token(Tag.OR,line));
								}else{
									Tokens.push_back(new Token('|',line));
									i++;
								}
							}break;
							case('!'):{
								Tokens.push_back(new Token(Tag.Not,line));
								i++;
							}break;
							default:{
								Tokens.push_back(new Token(c.at(i++),line));
								begin=true;
							}
							break;

									
				}
		}

	}
	
}
}
}
Tokens.pop_back();
Tokens.push_back(new Token(Tag.DEDENT,line));
Tokens.push_back(new Token(Tag.E_O_F,line));
return Tokens;
}
