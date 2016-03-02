#include <iostream>
#include <fstream>
#include <locale>

#include "Parser.h"
#include "Evaluator.h"

using namespace std;

Parser::Parser(Engine& engine) { this->engine = engine; }

Parser::Parser() {}

Parser::~Parser() {}

void Parser::scan(string filename)
{
	ifstream inFile;
	string line;

	inFile.open(filename.c_str(), ios::in);
	if (!inFile.is_open()) {
		cout << "File not found.\n";
		return;
	}
	
	while (getline(inFile, line)) {
		vector<string> tokens = tokenize(line);
		evaluate(tokens);
//		engine.printRelations();
	}
}

vector<string> Parser::tokenize(string line) {
	vector<string> tokens;
	string accumulator="";
	
	if(line.empty()) return tokens;

	for( ; ; ) {
		switch(line[0])	{
		case '(':
			if(accumulator.size() != 0) {
				tokens.push_back(accumulator);
				accumulator="";
			}
			
			tokens.push_back("(");
			line.erase(0,1);
			break;
		case ')':
			if(accumulator.size() != 0) {
				tokens.push_back(accumulator);
				accumulator="";
			}

			tokens.push_back(")");
			line.erase(0,1);
			break;
		case '"':
			if(accumulator.size() != 0) { //check for words
				tokens.push_back(accumulator);
				accumulator="";
			}
			tokens.push_back("\""); //push back the first quotes
			line.erase(0,1); //erase first quotes
            while(line[0]!='\"') //while not hitting a quote
            {
                accumulator += line.substr(0,1); //add to accumulator
                line.erase(0,1); //erase from string
            }
            if(accumulator.size() != 0) { //check for words
				tokens.push_back(accumulator); //push back words
				accumulator=""; //reset accumulator
			}
            tokens.push_back("\""); //push back the second quotes
            line.erase(0,1); //erase second quotes
			break;
		case ',':
			if(accumulator.size()!=0) {
				tokens.push_back(accumulator);
				accumulator="";
			}
			
			tokens.push_back(",");
			line.erase(0,1);
			break;
		case ';':
			if(accumulator.size()!=0) {
				tokens.push_back(accumulator);
				accumulator="";
			}

			tokens.push_back(";");
			line.erase(0,1);
			break;
		case ' ':
			if(accumulator.size() != 0) {
				tokens.push_back(accumulator);
				accumulator="";
			}
			line.erase(0,1);
			break;
        case '\n':
            if(accumulator.size() != 0) {
				tokens.push_back(accumulator);
				accumulator="";
			}
			line.erase(0,1);
			break;
		default:
			accumulator += line.substr(0,1);
			line.erase(0,1);
			break;
		}
		
		if(line.size() == 0) {
			if(accumulator.size() != 0) 
            {
				tokens.push_back(accumulator);
            }
			break;
		}		
	}
	/* 
    Use this for testing:
    
	for(int i = 0; i != tokens.size(); ++i)
		cout <<"Token: "<< tokens[i] << endl;

	*/
	return tokens;
}

Engine Parser::getEngine()
{
	return engine;
}
	
void Parser::evaluate(vector<string> tokens) {
	Evaluator e(engine, tokens);
	
	engine = e.evalProgram();
}



