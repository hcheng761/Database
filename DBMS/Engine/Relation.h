#ifndef RELATION_H
#define RELATION_H

#include "Tuple.h"
#include "Condition.h"

class Relation {
private:
	// Table's width must remain constant
	int width;
	string name;
	vector<Tuple> tuples;

public:
	Relation();
	Relation(string name);
	Relation(const Relation& r);
	~Relation();

	// Retrieves relation's name
	string getName();

	// Changes the relation's name to given name
	void setName(string name);

	// Retrieves the number of attributes per tuple
	int getWidth();

	// Retrieves the number of tuples
	int getHeight();

	// Returns true if there are no value tuples
	bool isEmpty();

	// Returns true if a relation has no name or content
	bool isVoid();

	// Returns a copy of the relation's label tuple (the first tuple)
	Tuple* getLabelTuple();

	// Returns a reference to a tuple
	Tuple* getTuple(int num);

	// Copies all the tuples from the given relation
	void copyTuples(const Relation& r);

	// Prints all tuples in a relation
	void printTuples();	

	// Prints relation's name and dimensions
	void printMetadata();
	
	// Adds tuple to the relation
	void addTuple(Tuple t);

	// Removes a tuple from the relation
	void removeTuple(int num);

	// Assignment operator
	Relation operator=(Relation& r);
};

/*----------------------------------------
 * RELATIONAL ALGEBRA OPERATIONS
 *----------------------------------------*/
// Verifies that two relations are union-compatible
// Two tables are union-compatible if they have the same number
// of columns and correspondent domains
bool isUnionCompatible(Relation r1, Relation r2);

// Returns a new table with renamed attribute names
Relation rename(string viewName, vector<string> newAttrNames, Relation r);	

// Returns a subset of rows that satisfy a condition
Relation select(string viewName, vector<string> leftOp,	vector<string> op,
	vector<string> rightOp, vector<string> conjunction,	Relation r);

Relation select(string viewName, Condition condition, Relation r);

// Returns a subset of columns with the gives names
Relation project(string viewName, vector<string> attrNames, Relation r);

// Return a new table with combination of two union-compatible tables
Relation setUnion(string viewName, Relation r1, Relation r2);
	
// Returns a new table with the tuples in a not present in b
// where a and b are union-compatible
Relation setDifference(string viewName, Relation r1, Relation r2);
	
// Returns a new table with the combination of information
// from both tables
Relation crossProduct(string viewName, Relation r1, Relation r2);

#endif