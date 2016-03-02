#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "..\Engine\Engine.h"

using namespace std;

class Parser {
private:
	// The engine from which to call functions
	

public:
	Engine engine;
	Parser(Engine& engine);
	Parser();
	~Parser();
	
	// Reads a DB file line by line
	void scan(string filename);

	Engine getEngine();
	// Splits a line into words or tokens
	vector<string> tokenize(string line);
	
	// Interprets each token and calls engine functions
	void evaluate(vector<string> tokens);
};

#endif