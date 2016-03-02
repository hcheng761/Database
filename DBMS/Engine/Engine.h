#ifndef ENGINE_H
#define ENGINE_H

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include "Relation.h"
#include "Condition.h"

using namespace std;

class Engine {
public:
	// A list of temporay views (only when file is open)
	// NOTE: Temporarily made public
	vector<Relation> views;
	
	Engine();
	Engine(const Engine& engine);
	~Engine();

	// Prints the list of saved relations
	void printRelations();

	// Returns a relation
	Relation getRelation(string relationName);

	// Sets a relation with the content of a given relation
	void setRelation(string oldRelation, Relation newRelation);

	// Assignment operator
	Engine operator=(Engine& engine);
	
	/*----------------------------------------
	 *	COMMANDS
	 *----------------------------------------*/
	// Opens a database file for reading/editing
	void open(string fileName);

	// Closes an opened database file
	void close(string fileName);
	
	// Saves a relation to a file
	void save(string relationName);
	
	// Adds a new database to the list
	void create(string relationName, vector<Attribute> attributes);	

	// Updates a relation's attributes' names that satisfy a condition
	void update(string relationName, vector<string> oldNames,
				vector<string> newNames, Condition condition);
	
	// Prints the information stored in a relation
	void show(string relationName);

	// Inserts values from literals into relation
	void insertInto(string relationName, vector<string> literals);
	
	// Inserts values from another relation
	void insertInto(string relationName, Relation relation);

	// Deletes values from relation that satisfy a condition
	void deleteFrom(string relationName, Condition condition);

	void addRelation(Relation relation);
private:
	// The currently open file
	// The user can only edit one database at a time
	fstream dbFile;
	
	// A list of the saved relations
	vector<Relation> relations;

	// Given a name, finds the relation index in the list
	// If not found, returns negative
	int searchRelation(string relationName);

	// Given a name, finds the relation index in the list
	// If not found, returns negative
	int searchView(string relationName);
};

#endif