#ifndef CONDITION_H
#define CONDITION_H

#include <string>
#include <vector>

using namespace std;

class Condition {
private:
	vector<string> leftOps;
	vector<string> ops;
	vector<string> rightOps;
	vector<string> conjunctions;

	bool isInteger(string s);

public:
	/*--------------------------------------------------
	 * Notes:
	 * Operators 'op' are any of the comparison operators.
	 * Conjunctions can be the AND or OR operators.
	 *--------------------------------------------------*/
	// Default constructor
	Condition();

	// Constructor for a single condition
	Condition(string leftOp, string op, string rightOp);
	
	// Constructor for multiple conditions
	Condition(vector<string> leftOps, vector<string> ops, 
		vector<string> rightOps, vector<string> conjunctions);
	
	// Destructor
	~Condition();
	
	// Returns the number of conditions
	int getNumConditions();

	// Prints the condition statement
	void printConditions();
	
	// Checks if a condition object is empty (zero conditions)
	bool isEmpty();

	// In the case of single condition, returns the left operand
	string getLeftOperand();

	// In the case of multiple conditions, returns a left operand at a given index
	string getLeftOperand(int num);

	// In the case of single condition, returns the right operand
	string getRightOperand();

	// In the case of multiple conditions, returns a right operand at a given index
	string getRightOperand(int num);
	
	// In the case of single condition, returns the operand
	string getOperand();

	// In the case of multiple conditions, returns an operand at a given index
	string getOperand(int num);

	// In the case of multiple conditions, returns a conjunction at a given index
	string getConjunction(int num);

	// Compares two attribute names according to operator
	bool compareNames(string name1, string name2, string op);
	
	// Compares an attribute name to a literal according to operator
	bool compareNameLiteral(string name, string lit, string op);
	
	// Compares a literal to an attribute name according to operator
	bool compareLiteralName(string lit, string name, string op);
	
	// Compares two literals according to operator
	bool compareLiterals(string lit1, string lit2, string op);

	// Assignment operator
	Condition operator=(Condition& c);

};

#endif