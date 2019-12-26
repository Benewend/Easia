#include "parser.h"
#include <vector>
#include "Tokenizer.h"
#include<cmath>
using namespace std;


typedef std::vector<Token*> T_stream;
 tags Tags;
 int debug_var =0;
Out::Out(int i):type(i){

}
Out::~Out(){

}

NOut::NOut(int t, float val):Out(t),value(val){

}
NOut::~NOut(){

}

SOut::SOut():value(""),count(0),Out(Tags.STRING){

}
SOut::SOut(int t, std::string val):Out(t),value(val){

}

SOut::SOut( std::string val,int t):Out(Tags.STRING),value(val),count(t){

}
SOut::~SOut(){

}

Symbol::Symbol(Word* name, Out* vs):contained(name),ValType(vs){

}
Symbol::Symbol(Symbol* got):contained(got->contained),ValType(got->ValType){

}




Root::Root(int i):T(0){
	
}
Out* Root::visit(SymbolTable* st){
	return new Out(-2);
}
bool Root::typeCheck(int first,int second){
	if((first>=Tags.REAL)&&(second>Tags.REAL)){
		return true ;
	}else{
		return false;
	}
}


SymbolTable::SymbolTable(SymbolTable* prev):previous(prev){
	
}
SymbolTable::~SymbolTable(){

}
float SymbolTable::defineVar(Symbol* s){

	
	for(SymbolTable* t= this;t->previous!=NULL;t=t->previous){
		for(int i =0 ;i<t->Stable.size();i++){
			if(t->Stable[i]->contained->lexeme.compare(s->contained->lexeme)==0){
				t->Stable[i]=s; 
				return 1.0;
			}
		}
	}
	Stable.push_back(s);
	return 1.0;
}
Symbol* SymbolTable::Search(Word*v){
	for(SymbolTable* t= this;t!=NULL;t=t->previous){
		for(int i =0 ;i<t->Stable.size();i++){
			if(v->lexeme.compare(t->Stable[i]->contained->lexeme)==0){
				Symbol* temp = new Symbol(t->Stable[i]) ;
				return temp; 
			}
		}
	}
	return NULL;
}


std::string  SymbolTable::Tmatcher(int i){
	switch(i){
		case(284):{
			return "string";
		}break;
		case(293):{
			return "float";
		}break;
		case(294):{
			return "int";
		}break;
		case(295):{
			return "Bool";
		}break;



	}
}
void SymbolTable::printTable(){
	for(int i =0 ;i<Stable.size();i++){
		if(Stable[i]->ValType->type==Tags.REAL||Stable[i]->ValType->type==Tags.Int||Stable[i]->ValType->type==Tags.Bool){
			NOut * temp =static_cast<NOut*>(Stable[i]->ValType);
			cout<<"Variable "<<Stable[i]->contained->lexeme<<"->"<<temp->value<<" of type "
			<<Tmatcher(temp->type)<<endl;
		}else{
			SOut * temp =static_cast<SOut*>(Stable[i]->ValType);
			cout<<" Variable "<<Stable[i]->contained->lexeme<<"-> "<<temp->value<<" of type "
			<<Tmatcher(temp->type)<<endl;
		}
	}
}












Unary::Unary():sign(NULL),right(NULL),Root(){

}
Unary::~Unary(){

}
Unary::Unary(Token* S, Root*R):sign(S),right(R),Root(){

}


Out* Unary::visit(SymbolTable* st){
	Out* got =right->visit(st);
		if(sign->tag=='+'){
		return got;
		}else{
			NOut* Ngot =(NOut*)got;
			Ngot->value=-Ngot->value;
			return Ngot;
		}


	
	

	}

BinaryOP::BinaryOP():left(NULL),op(NULL),right(NULL),Root(Tags.binaryNode){

}
BinaryOP::BinaryOP(Root* L, Token* Opera, Root* R):left(L),op(Opera),right(R),Root(Tags.binaryNode){

}

Out* BinaryOP::visit(SymbolTable* st){
	Out* gotl= left->visit(st);
	Out* gotR= right->visit(st);
	if(typeCheck(gotl->type,gotR->type)){
		NOut* Lgot = (NOut*) gotl;
		NOut* Rgot = (NOut*) gotR;
		NOut* togive = new NOut(0,0);
		switch(op->tag){
			case('+'):{
				togive->type=Lgot->type;
				togive->value=Lgot->value+Rgot->value;
				return togive;
				}break;
			case('-'):{
				togive->type=Lgot->type;
				togive->value=Lgot->value-Rgot->value;
				return togive;
				}break;
			case('/'):{//need to handel division by zero error asap
				togive->type=Lgot->type;
				togive->value=Lgot->value/Rgot->value;
				return togive;
				}break;
			case('*'):{
				togive->type=Lgot->type;
				togive->value=Lgot->value*Rgot->value;
				return togive;
				}break;
			case('%'):{
				if(Lgot->type ==Tags.Int && Rgot->type==Tags.Int){
					togive->type =Lgot->type;
					togive->value=static_cast<int>(Lgot->value)%static_cast<int>(Rgot->value);
					return togive;
				}else{
					cout<<"ERROR you are trying to apply the modulo operation to a float"<<endl;
					exit(1);
				}
				}break;
			case(290):{//using the value of the tag for the power double star token cause of const exp error
				//cout<<"the power value"<<pow(left->visit(),right->visit())<<endl;
				togive->type =Lgot->type;
				togive->value=pow(Lgot->value,Rgot->value);
				return togive;
				}break;
			case('<'):{
				togive->type=Tags.Bool;
				togive->value=(Lgot->value < Rgot->value);
				//cout<<"\nthe value of 1 < 5 -> "<<Lgot->value<<"\n";
				//cout<<"\nthe value of  operation done using\" < \" -> "<<togive->value<<"\n";
				return togive;
				}break;
			case('>'):{
				togive->type=Tags.Bool;
				togive->value=Lgot->value>Rgot->value;
				//cout<<"\nthe value of  operation done using\" > \" -> "<<togive->value<<"\n";
				return togive;
				}break;
			case('='):{
				togive->type=Tags.Bool;
				togive->value=Lgot->value==Rgot->value;
				//cout<<"\nthe value of 1 = 5 -> "<<Lgot->value<<"\n";
				return togive;
				}break;
			case(267):{
				togive->type=Tags.Bool;
				togive->value=Lgot->value<=Rgot->value;
				//cout<<"\nthe value of 1<= 5 -> "<<Lgot->value<<"\n";
				return togive;
				}break;
			case(262):{
				togive->type=Tags.Bool;
				togive->value=Lgot->value>=Rgot->value;
				//cout<<"\nthe value of 1 >= 5 -> "<<Lgot->value<<"\n";
				return togive;
				}break;
			case(256):{
				togive->type=Tags.Bool;
				togive->value=Lgot->value && Rgot->value;
				//cout<<"\nthe value of 1 >= 5 -> "<<Lgot->value<<"\n";
				return togive;
				}break;
			case(275):{
				togive->type=Tags.Bool;
				togive->value=Lgot->value || Rgot->value;
				//cout<<"\nthe value of 1 >= 5 -> "<<Lgot->value<<"\n";
				return togive;
			}break;

		}
	}else{
		SOut* togive =new SOut();
		if(gotl->type==gotR->type){
			SOut* tempL=static_cast<SOut*>(gotl);
			SOut* tempR=static_cast<SOut*>(gotR);
			if(op->tag=='+'){
				togive->value=tempL->value+tempR->value;
				togive->count=tempL->count+tempR->count;
				return togive;
			}else{
				if(op->tag=='='){
					if(tempL->count==tempR->count){
						if(tempL->value.compare(tempR->value)==0){
							return new NOut(Tags.Bool,1);
						}
					}else{
						return new NOut(Tags.Bool,0);
					}
				}else{
					cout<<"ERROR: Only operation such as concatenation and equality is doable on string in Easia\n";
					exit(1);
				}
			}
		}else{
			if(gotl->type==Tags.STRING){
				SOut* tempStr = static_cast<SOut*>(gotl);
				NOut* tempNumb = static_cast<NOut*>(gotR);
				if(op->tag=='*'){
					for(int i=0; i< tempNumb->value;i++){
						togive->count+=tempStr->count;
						togive->value+=tempStr->value;
					}
					return togive;
				}else{
					if (op->tag=='+'){
						togive->value=tempStr->value+to_string(tempNumb->value);
						togive->count+=to_string(tempNumb->value).length();
						return togive;
					}else{
						cout<<"ERROR: Only operation such as multiplication and concatenation is doable between a strings and a numerical value Easia\n";
						exit(1);
					}
				}
			}else{
				if(gotR->type==Tags.STRING){
					SOut* tempStr = static_cast<SOut*>(gotR);
					NOut* tempNumb = static_cast<NOut*>(gotl);
					if(op->tag=='*'){
						for(int i=0; i< tempNumb->value;i++){
							togive->count+=tempStr->count;
							togive->value+=tempStr->value;
						}
						return togive;
					}else{
						if (op->tag=='+'){
							togive->value=tempStr->value+to_string(tempNumb->value);
							togive->count+=to_string(tempNumb->value).length();
							return togive;
						}else{
							cout<<"ERROR: Only operation such as multiplication and concatenation is doable between a strings and a numerical value Easia\n";
							exit(1);
						}
					}
				}

			}
		}
	}
	//cout<<"You are trying to perform operation on string this feature is not ready yet"<<endl;


	
}

BinaryOP::~BinaryOP(){
	
}

Numb::Numb():token(NULL),value(0),Root(Tags.numNode){

}

Numb::Numb(Token* t):token(t),Root(Tags.numNode){
	value = static_cast<Num*>(t)->value;

}

Out* Numb::visit(SymbolTable* st){
	NOut* togive = new NOut(Tags.Int, value); 
	return togive;
}
	
Numb::~Numb(){
	
}

Bool::Bool(Token* t, float val):token(t),value(val){

}

Bool::~Bool(){

}
Out* Bool::visit(SymbolTable* st){
	NOut* togive = new NOut(Tags.Bool,value);
	return togive;
}
FLoat::FLoat(Token* t):token(t){
	value = static_cast<Float*>(t)->value;
}

FLoat::~FLoat(){

}
Out* FLoat::visit(SymbolTable* st){
	NOut* togive= new NOut(Tags.REAL,value);
	return togive;
}


String::String(Token* t){
	token = static_cast<STR*>(t);
}

String::~String(){

}

Out* String::visit(SymbolTable* st){
	SOut* togive = new SOut(token->val,token->count);
	return togive;
}



BLOCK::BLOCK():Root(){
	
}

BLOCK::~BLOCK(){

}

Out* BLOCK::visit(SymbolTable* st){
	SymbolTable* s = new SymbolTable(st);
	for(int i=0;i<Statements.size();i++){
		Statements[i]->visit(s);
	}
	//s->printTable();
	return new Out(-1);

}

Assignation::Assignation(Word* l, Token* t, Root* r):left(l),Assign(t),right(r){

}
Assignation::Assignation(Word* l, Root* r):left(l),Assign(NULL),right(r){

}
Assignation::~Assignation(){

}

Out* Assignation::visit(SymbolTable* st){
	Symbol* toSave = new Symbol(left, right->visit( st));
	st->defineVar(toSave);
	return new Out(-5);
}


Variable::Variable(Word* t ):Root(),token(t),value(token->lexeme){

}

Variable::~Variable(){
	
}

Out* Variable::visit(SymbolTable* st){
	Symbol* toGive= st->Search(token);
	if(toGive!=NULL){
		return  toGive->ValType;
	}else{
		cout<<"you are trying to read a Variable not define "
		<< "Easia doesn't know "+token->lexeme+"  "<<endl;
		exit(1);
	}
}

Negation::Negation(Token* t , Root* r):token(t),right(r){

}
Negation::~Negation(){

}
Out* Negation::visit(SymbolTable* st){
	NOut* togive = (NOut*)right->visit(st);
	togive->type=Tags.Bool;
	togive->value = !togive;
	return togive;
}

Null::Null():Root(){

}
Null::~Null(){

}

Out* Null::visit(SymbolTable* st){
	return new Out(-3);
}


When::When(Root*con,Root* b,Root* ot):condition(con),Blck(b),other(ot),Root(){

}
When::When(Root*con,Root* b):condition(con),Blck(b),other(NULL),Root(){

}

When::~When(){

}

Out* When::visit(SymbolTable* st){
	Out* temp =condition->visit(st);
	NOut* got = static_cast<NOut*>(temp);
	if(got->value>0){
		Blck->visit(st);
		return new Out(1);
	}else{
		if(other){
			other->visit(st);
		}
		return new Out(1);
	}
}


For::For(Word* id, Root* strt, Root* nd,Root* blck,Root* stp):start(strt),end(nd),Block(blck),Root(){
	counter = new Assignation(id,strt);
	if(stp){
		step=stp;
	}else{
		step= new Numb( new Num(1,0));

	}
}
//Assignation(Word* var, Token* as, Root* right);

For::~For(){

}

Out* For::visit(SymbolTable* st){
	NOut* tempStrt = static_cast<NOut*>(start->visit(st));
	NOut* tempEnd = static_cast<NOut*>(end->visit(st));
	NOut* tempStep = static_cast<NOut*>(step->visit(st));
	counter->visit(st);
	Root* var = new Variable(counter->left);
	Root* right = new BinaryOP(var,new Token('+',0),step);
	Root* node= new Assignation(counter->left,right);
	BLOCK* temp = static_cast<BLOCK*>(Block);
	temp->Statements.push_back(node);
	for(int i =tempStrt->value;i<tempEnd->value;i+=tempStep->value){
		temp->visit(st);
	}
	st->Stable.pop_back();
	return new Out(1);
}

AsLoop::AsLoop(Root* con,Root* Blck):condition(con),Block(Blck),Root(){

}

AsLoop::~AsLoop(){

}


Out* AsLoop::visit(SymbolTable*st){
	NOut* tempCond = static_cast<NOut*>(condition->visit(st));
	while(tempCond->value!=0){
		Block->visit(st);
		tempCond = static_cast<NOut*>(condition->visit(st));
	}
}


Show::Show(Token* t ):Root(),token(t),value(""){

}
Show::~Show(){

}
Out* Show::visit(SymbolTable* st){
	if (token->tag==Tags.ID){
		Word* temp= static_cast<Word*>(token);
		Symbol* Give= st->Search(temp);
		//cout<<temp->tag<<endl;
		if(Give!=NULL){
				
				if(Give->ValType->type>=Tags.REAL){
					NOut* toGive =static_cast<NOut*>(Give->ValType);
					std::string give =(toGive->type==Tags.REAL)? to_string(toGive->value) : to_string((int)toGive->value);
					value =temp->lexeme;
					cout<<"> "<<give<<endl;
			 	}else{
			 		SOut* toGive =static_cast<SOut*>(Give->ValType);
			 		value =temp->lexeme;
					cout<<">"<<toGive->value<<endl;
			 	
			 	}
			 return new Out(1);
		}else{
			cout<<"you are trying to read a Variable not define:\t"
			<< "Easia doesn't know "+temp->lexeme+" \n "<<endl;
			exit(1);
		}
	

	}

	/*cout<<"just before the printing"<<endl;
	NOut* temp= static_cast<NOut*>(toShow->visit(st));
	cout<<"->"<<temp->value<<"\n";*/
}


std::string  Show::Tmatcher(int i){
	switch(i){
		case(284):{
			return "string";
		}break;
		case(293):{
			return "float";
		}break;
		case(294):{
			return "int";
		}break;
		case(295):{
			return "Bool";
		}break;



	}
}



Parser::Parser(T_stream Ts):Tokens(Ts),Current(Ts.at(0)),index(0){


}




void Parser::error(){
	cout<< "syntaxical error on line: "+to_string(Current->line)<<endl;
	exit(1);
}
void Parser::error(std::string m){
	cout<< m<<endl;
	exit(1);
}
bool Parser::precedence(int tag, int pre){
	std::vector<int> vi;
	if(pre==1){
		vi.push_back((int)'*');
		vi.push_back((int)'/');
		vi.push_back((int)'%');
		vi.push_back((int)'<');
		vi.push_back((int)'>');
		vi.push_back((int)'=');
		vi.push_back(Tags.LE);
		vi.push_back(Tags.GE);
		for(int i=0;i<vi.size();i++){
			if(vi.at(i)==tag){
				return true;
			}
		}
		return false;
	}else{
		vi.push_back((int)'-');
		vi.push_back((int)'+');
		vi.push_back(Tags.OR);
		vi.push_back(Tags.AND);
		for(int i=0;i<vi.size();i++){
			if(vi.at(i)==tag){
				return true;
			}
		}
		return false;

	}
}

void Parser::eat(int tok,std::string function){

	if(Current->tag==tok){
		if( index<Tokens.size()){
		//cout<<"have just eaten "+to_string(tok)<<'\n';
		Current = Tokens[++index];
		//cout<<"\t the index->"+to_string(index)<<'\n';
		}else{
			error("your code is incomplete");
		}
	}else{
		error("bad token got  on line "+to_string(Current->line)+"  should have got "+MatchToken(tok)+" but have got "+MatchToken(Current->tag)+" "+function);
	}
}

std::string Parser::MatchToken(int t){
	if(t<=255){
		std::string s(1, (char) t);
		return s;
	}else{
		switch(t){
			case(256):{
				return "AND or &&";
			}break;
			case(259):{
				return "do";
			}break;
			case(260):{
				return "as";
			}break;
			case(261):{
				return "long";
			}break;
			case(264):{
				return "identifier";
			}break;
			case(265):{
				return "when";
			}break;
			case(274):{
				return "other";
			}break;
			case(275):{
				return "or";
			}break;
			case(281):{
				return " Key word";
			}break;
			case(282):{
				return "id";
			}break;
			case(283):{
				return " <- ";
			}break;
			case(284):{
				return "string";
			}break;
			case(285):{
				return "end of the source file ";
			}break;
			case(287):{
				return "new line expected";
			}break;
			case(290):{
				return "power";
			}break;
			case(291):{
				return "indent";
			}break;
			case(292):{
				return "dedent";
			}break;
			case(299):{
				return "for";
			}break;
			case(300):{
				return "going";
			}break;
			case(301):{
				return "from";
			}break;
			case(302):{
				return "to";
			}break;
			case(303):{
				return "by";
			}break;
			case(304):{
				return "304";
			}break;
			default:{
				return "bad token";
			}break;


		}
	}
}



Root* Parser::factor(){
	Token* token = Current;
	if(token->tag==Tags.NUM){
		eat(Tags.NUM);
		if(Current->tag==Tags.POWER){
			Token*Power=Current;
			eat(Tags.POWER);
			return new  BinaryOP(new Numb(token),Power,factor());
		}
		return new Numb(token);
	}else{
		if(token->tag=='('){
			eat('(');
			Root*  node = expr();
			eat(')');
			return node;
		}else{
			if(token->tag=='-'||token->tag=='+'){
				char toEat=(token->tag=='-')? '-':'+';
				eat(toEat);
				Root* node =new Unary(token,factor());
				return node;
			}else{
				if(token->tag==Tags.ID){
					eat(Tags.ID);
					if(Current->tag==Tags.POWER){
						Token*Power=Current;
						eat(Tags.POWER);
						return new  BinaryOP(new Variable((Word*)token),Power,factor());
					}

					Root* node = new Variable((Word*)token);
					return node;
				}else{
					if(token->tag==Tags.True){
						eat(Tags.True);
						Root* node = new Bool(token,1);
						return node;
					}else{
						if(token->tag==Tags.False){
						eat(Tags.False);
						Root* node = new Bool(token,0);
						return node;
						}else{
							if(token->tag==Tags.REAL){
								eat(Tags.REAL);
								Root* node = new FLoat(token);
								return node;
							}else{
								if (token->tag==Tags.STRING){
								
									eat(Tags.STRING);
									Root* node = new String(token);
									return node;

								}else{
									if (token->tag==Tags.Not){
										eat(Tags.Not);
										Root* node= new Negation(token,factor());
										return node;
									}
									return new Null();
								}
							}
						}

					}
					
				}

				
				
			

			}
		}
	}
}
Root* Parser::term(){
	Root* node = factor();
	while(precedence(Current->tag,1)){
		Token* token = Current;


		if (token->tag=='*'){
			eat('*');
		}else{
			if(token->tag=='/'){
				eat('/');
			}else{
				if(token->tag=='<'){
					eat('<');
				}else{
					if(token->tag=='>'){
					eat('>');
					}else{
						if(token->tag==Tags.GE){
							eat(Tags.GE);
						}else{
							if(token->tag==Tags.LE){
								eat(Tags.LE);
							}else{
								if(token->tag=='='){
									eat('=');
								}else{
									eat('%');
								}
			
							}
		
						}
					}
				}
			}
		}
		node= new BinaryOP(node,token,factor());
	}
	return node;
}
Root* Parser::expr(){
	Root* node = term();
	while(precedence(Current->tag,2)){
		Token *token =Current;


		if(token->tag=='+'){
			eat('+');
		}else{
			if(token->tag=='-'){
				eat('-');
			}else{
				if(token->tag==Tags.AND){
					eat(Tags.AND);
				}else{
					if(token->tag==Tags.OR){
						eat(Tags.OR);
					}
				}

			}
	}	node = new BinaryOP(node,token,term());
	}
	return node;
}



Root* Parser::parse(){
	Root* got = code();

	if(Current->tag!=Tags.E_O_F){
		cout<<"\nerror on line "+to_string(Current->line)+"\n should have got E_O_F but have got "+to_string(Current->tag)+"\n";
		exit(1);
	}
	return got;
	}
Parser::~Parser(){

}



Root* Parser::code(){
	BLOCK* got=static_cast<BLOCK*>(block());
	//cout<<"at the end of code"<<endl;
	// while(Current->tag==Tags.new_l and Tokens.at(index+1)->tag!=Tags.E_O_F){
	// 	eat(Tags.new_l);
	// 	// mod-> got->Statements.push_back(statement());
	// 	got->Statements.push_back(block());
	// }
	// if (Current->tag==Tags.new_l){
	// 	eat(Tags.new_l);
	// }if (Current->tag==Tags.DEDENT){
	// 	eat(Tags.DEDENT);
	// }
	return got;

}

Root* Parser::block(){
	eat(Tags.INDENT);
	std::vector<Root*> got = statement_list();
	BLOCK* root =new BLOCK();
	for(int i=0; i< got.size(); i++){
		root->Statements.push_back(got[i]);
	}
	// if(Current->tag=Tags.DEDENT){
	//cout<<"\tfirst eater of dedent end of block()\n";
	eat(Tags.DEDENT);
	//}
	return root;

}



std::vector<Root*> Parser::statement_list(){
	Root* node = statement();
	std::vector<Root*> got;
	got.push_back(node);
	while(Current->tag==Tags.new_l && Tokens.at(index+1)->tag!=Tags.E_O_F){
		eat(Tags.new_l);
		got.push_back(statement());
	}
	return got;
}


Root* Parser::statement(){
	Root* got;
	if(Current->tag == Tags.INDENT){
		got=block();
	}else{
		if(Current->tag==Tags.ID){

			got= assign();
			//}
		}else{
			if(Current->tag==Tags.WHEN){
				got=when();
			}else{
				if(Current->tag==Tags.FOR){
					got=ForLoop();
				}else{
					
						if(Current->tag==Tags.AS){
							got=ASLong();
						}else{
							if(Current->tag==Tags.SHOW){
								got=Shows();
							}else{
								got=new  Null();
							}
							//eat(Tags.new_l);
						}
					
					
				}
				
			}
			
		}
	}
	
	return got;
}

Root *Parser::assign(){
	Word* l=varName();
	Token* op =Current;
	eat(Tags.ASSIGN);
	Root* r = expr();
	return new Assignation(l,op,r);
}

Word* Parser::varName(){
	Token* token =Current;
	eat(Tags.ID);
	Word* got = (Word*)token;
	

	return got;
}


Root* Parser::when(){
	eat(Tags.WHEN);
	Root* con =expr();
	eat(':');
	eat(Tags.new_l);
	Root* b =block();
	if(Current->tag==Tags.OTHER){
		Root* ot=other();
		When* togive=new When(con,b,ot);
		return togive;
	}else{
		Root* togive=static_cast<Root*>(new When(con,b));
		return togive;
	}

}

Root* Parser::other(){
	eat(Tags.OTHER);
	eat(':');
	eat(Tags.new_l);
	return block();
}


Root* Parser::ForLoop(){
	eat(Tags.FOR);
	Token* TempId= Current;
	eat(Tags.ID);
	Word* id =static_cast<Word*>(TempId);
	eat(Tags.GOING);
	eat(Tags.FROM);
	Root* st=expr();
	eat(Tags.TO);
	Root*nd=expr();
	if(Current->tag==':'){
		eat(':');
		eat(Tags.new_l);
		Root* blck=block();
		For* tempFor=new For(id,st,nd,blck);
		Root* togive = static_cast<Root*>(tempFor);
		return togive;

	}else{
		eat(Tags.BY);
		Root* st =expr();
		eat(':');
		eat(Tags.new_l);
		Root* blck=block();
		For* tempFor=new For(id,st,nd,blck,st);
		Root* togive = static_cast<Root*>(tempFor);
		return togive;
	}
}


Root* Parser::Shows(){
	eat(Tags.SHOW);
	eat('(');
	Token* tok = Current;
	Word* temp =static_cast<Word*>(tok);
	eat(Tags.ID);
	eat(')');
	//eat(Tags.new_l);
	return new Show(temp);
}


Root* Parser::ASLong(){
	eat(Tags.AS);
	eat(Tags.Long);
	eat(Tags.AS);
	Root* con =expr();
	eat(':');
	eat(Tags.new_l);
	Root* Blck=block();
	AsLoop* temp = new AsLoop(con, Blck);
	Root* togive = static_cast<Root*>(temp);
	return togive;
}