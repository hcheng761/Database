#pragma once

#include <string>
#include <vector>

using namespace std;

enum Datatype {	VARCHAR, INTEGER };

class Attribute {
private:
	string name;
	Datatype type;
	
	// The capacity of a VARCHAR attribute
	// If INTEGER, capacity is set to zero
	int capacity;

public:
	Attribute();
	Attribute(string name, Datatype type);					// for INTEGER type
	Attribute(string name, Datatype type, int capacity);	// for VARCHAR type
	~Attribute();
	
	// Prints the type's name
	void printType();

	// Prints label and type
	void printMetadata();

	// Returns the name of the attribute
	string getName();

	// Returns the attribute type's name
	string getTypeString();

	// Returns the attribute type's number
	Datatype getType();

	// Returns the attribute of type VARCHAR capacity
	int getCapacity();

	// Resets the name of the attribute
	void setName(string name);

	// Resets the type of the attribute
	void setType(Datatype type);

	// Comparison operators
	bool operator==(Attribute& a);
	bool operator!=(Attribute& a);
	bool operator>=(Attribute& a);
	bool operator<=(Attribute& a);
	bool operator>(Attribute& a);
	bool operator<(Attribute& a);
};