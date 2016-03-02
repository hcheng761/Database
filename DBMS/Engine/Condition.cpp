#include <iostream>
#include "Condition.h"

Condition::Condition() {}

Condition::Condition(string leftOp, string op, string rightOp) {
	leftOps.push_back(leftOp);
	ops.push_back(op);
	rightOps.push_back(rightOp);
}

Condition::Condition(vector<string> leftOps, vector<string> ops, 
	vector<string> rightOps, vector<string> conjunctions) {

	this->leftOps = leftOps;
	this->ops = ops;
	this->rightOps = rightOps;
	this->conjunctions = conjunctions;
}

Condition::~Condition() {}

int Condition::getNumConditions() { return leftOps.size(); }

void Condition::printConditions() {
	int numConds = leftOps.size();
	
	if(numConds == 0) return;
	else if(numConds == 1) {
		cout << leftOps[0] << " "
			 << ops[0] << " "
			 << rightOps[0] << endl;
	} else {
		for(int i = 0; i < numConds; i++) {
			cout << leftOps[i] << " "
				 << ops[i] << " "
				 << rightOps[i];
			
			if(i != numConds - 1)
				cout << " " << conjunctions[i] << " ";
			else cout << endl;
		}
	}
}

bool Condition::isEmpty() { return leftOps.size() == 0; }

bool Condition::isInteger(string s) {
	for(int i = 0; i < s.length(); i++) {
		if(!isdigit(s[i])) return false;
	}
	return true;
}

string Condition::getLeftOperand() { return leftOps[0]; }

string Condition::getLeftOperand(int num) { 
	string left;
	
	// Return empty string in case of out-of-range
	if(num < 0 || num > getNumConditions()) return left;
	else return leftOps[num];
}

string Condition::getRightOperand() { return rightOps[0]; }

string Condition::getRightOperand(int num) {
	string right;
	
	// Return empty string in case of out-of-range
	if(num < 0 || num > getNumConditions()) return right;
	else return rightOps[num];
}
	
string Condition::getOperand() { return ops[0]; }

string Condition::getOperand(int num) {
	string op;
	
	// Return empty string in case of out-of-range
	if(num < 0 || num > getNumConditions()) return op;
	else return ops[num];
}

string Condition::getConjunction(int num) {
	string conj;
	
	// Return empty string in case of out-of-range
	if(num < 0 || num > getNumConditions() - 1) return conj;
	else return conjunctions[num];
}

bool Condition::compareNames(string name1, string name2, string op) {
	if(op.compare("==") == 0) {
		return name1.compare(name2) == 0;
	} else if(op.compare("!=") == 0) {
		return name1.compare(name2) != 0;
	} else if(op.compare(">") == 0) {
		return name1.compare(name2) > 0;			
	} else if(op.compare("<") == 0) {
		return name1.compare(name2) < 0;
	} else if(op.compare(">=") == 0) {
		return name1.compare(name2) >= 0;
	} else if(op.compare("<=") == 0) {
		return name1.compare(name2) <= 0;
	}
}

bool Condition::compareNameLiteral(string name, string lit, string op) {
	// If comparing integers...
	if(isInteger(lit)) {
		int attrValue = stoi(name),
			litValue = stoi(lit);

		if(op.compare("==") == 0) {
			 return attrValue == litValue;
		} else if(op.compare("!=") == 0) {
			return attrValue != litValue;
		} else if(op.compare(">") == 0) {
			return attrValue > litValue;			
		} else if(op.compare("<") == 0) {
			return attrValue < litValue;
		} else if(op.compare(">=") == 0) {
			return attrValue >= litValue;
		} else if(op.compare("<=") == 0) {
			return attrValue <= litValue;
		}
	}
	
	// If comparing varchars...	
	else return compareNames(name, lit, op);
}

bool Condition::compareLiteralName(string lit, string name, string op) {
	return compareNameLiteral(lit, name, op);
}

bool Condition::compareLiterals(string lit1, string lit2, string op) {
	return compareNameLiteral(lit1, lit2, op);
}

Condition Condition::operator=(Condition& c) {
	leftOps = c.leftOps;
	ops = c.ops;
	rightOps = c.rightOps;
	conjunctions = c.conjunctions;

	return *this;
}