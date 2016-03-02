#ifndef EVALUATOR_H
#define EVALUTOR_H

#include <string>
#include <vector>

#include "..\Engine\Engine.h"

using namespace std;

class Evaluator {
private:
	/*----------VARIABLES---------- */
	// The control "pointer" to read the line
	int cur;

	// The line to be evaluated
	vector<string> tokens;

	// The engine from which to call functions
	Engine engine;

	/*----------FUNCTIONS---------- */

	bool isEndOfLine();
	// Verifies validity of a command

	Engine evalCommand();
	
	Engine evalOpen();
	Engine evalClose();
	Engine evalSave();
	Engine evalShow();
	Engine evalCreate();
	Engine evalUpdate();
	Engine evalInsert();
	Engine evalDelete();

	// Verifies validity of a query
	Engine evalQuery();

	Relation evalSelection();
	Relation evalProjection();
	Relation evalRenaming();

	Relation evalExpression();
	Relation evalAtomicExpression();
	
	Condition evalCondition();

	string evalOperator(string token);	
	string evalRelationName(string token);
	string evalAttributeName(string token);
	string evalIdentifier(string token);
	string evalInteger(string token);

	vector<string> evalVarchar();
	vector<string> evalType();
	vector<string> evalAttributeList();
	vector<Attribute> evalTypedAttributeList();

public:
	Evaluator(Engine engine, vector<string> tokens);
	~Evaluator();

	// Distinguishes if a line is a command or query
	Engine evalProgram();

};

#endif