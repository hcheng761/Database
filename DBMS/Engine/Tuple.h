#pragma once

#include "Attribute.h"
#include <vector>

class Tuple {
private:
	int num;						// Analogous to primary key

public:
	vector<Attribute> attributes;	// The list of attributes
	
	Tuple();
	Tuple(vector<Attribute> attributes);
	~Tuple();

	// Retrieves number of attributes
	int getNumAttributes();

	// Returns true if a tuple has no attributes
	bool isEmpty();

	// Retrieves a single attribute
	Attribute* getAttribute(int num);

	// Given a name, searches for the attribute index
	// at first occurence. Returns negative if not found
	int searchAttribute(string name);

	// Prints tuple siza and names and types of attributes
	void printMetadata();

	// Prints the names of attributes in a line
	void printAttributes();

	// Tuple assignment
	Tuple& operator=(Tuple& t);

	// Tuple concatenation
	Tuple operator+(Tuple& t);

	// Tuple comparison
	bool operator==(Tuple& t);
	bool operator!=(Tuple& t);

};

