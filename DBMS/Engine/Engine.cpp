#include <iostream>
#include <fstream>
#include <list>
#include "Engine.h"
#include "..\Parser\Parser.h"

Engine::Engine() {}

Engine::Engine(const Engine& engine) {
	// Only copy what is permanent
	this->relations = engine.relations;
}

Engine::~Engine() {}

void Engine::printRelations() {
	cout << "LIST OF RELATIONS:" << endl;

	for(int i = 0; i < relations.size(); i++) {
		cout << relations[i].getName() << endl;
	}

	cout << endl;
}

Relation Engine::getRelation(string relationName) {
	int index = searchRelation(relationName);
	
	if(index >= 0) return relations[index];
	else return Relation();
}

void Engine::setRelation(string oldRelation, Relation newRelation) {
	// Search in relations list
	int index = searchRelation(oldRelation);

	// If there is a relation with that name, reassign it with new relation
	if(index >= 0) {
		relations[index] = newRelation;
	}
}

void Engine::addRelation(Relation relation) { relations.push_back(relation); }

Engine Engine::operator=(Engine& engine) {
	this->relations.clear();
	this->relations = engine.relations;
	return *this;
}

void Engine::open(string fileName) {
	/*
	dbFile.open(fileName);

	if(dbFile.is_open()) {
		Parser p(*this);
		p.scan(dbFile);
	} else cerr << "File not found." << endl;
	*/
}

void Engine::close(string fileName) { 
	if(dbFile.is_open()) {
		dbFile.close();
	} else cout << "File is already closed." << endl;
}

void Engine::save(string relationName) { 
	// Search in relations list
	int index = searchRelation(relationName);

	if(index >= 0) {
		ofstream newFile("../Database/" + relationName + ".db", ofstream::trunc);
		
		newFile << "CREATE TABLE " << relationName << " (";
		
		// For eah attribute of the first tuple
		Tuple* labels = relations[index].getTuple(0);
		for (int i = 0; i < labels->getNumAttributes(); i++) {
			newFile << labels->getAttribute(i)->getName() << " ";
			newFile << labels->getAttribute(i)->getTypeString();
			if (labels->getAttribute(i)->getTypeString() == "VARCHAR")
				newFile << "(" << labels->getAttribute(i)->getCapacity() << ")";
			if (i != labels->getNumAttributes() - 1)
				newFile << ", ";
			}
			newFile << ") PRIMARY KEY(";
			for (int i = 0; i < labels->getNumAttributes(); i++) {
			newFile << labels->getAttribute(i)->getName();
			if (i != labels->getNumAttributes() - 1)
				newFile << ", ";
		}
		newFile << ");\n";

		// For each value tuple, INSERT
		for(int i = 1; i < relations[index].getHeight(); i++) {
			newFile << "INSERT INTO " << relationName
					<< " VALUES FROM (";

			// For each attribute, get string
			Tuple* t = relations[index].getTuple(i);
			for(int j = 0; j < t->getNumAttributes(); j++) {
				newFile << "\"" << t->getAttribute(j)->getName() << "\"";

				if(j != t->getNumAttributes() - 1)
					newFile << ", ";
			}
			newFile << ");\n";
		}
		newFile.close();
	} else cout << "There are no relations with this name." << endl;	
}



void Engine::create(string relationName, vector<Attribute> attributes) { 
	Relation r(relationName);

	Tuple t(attributes);
	r.addTuple(t);

	relations.push_back(r);

	//printRelations();
}

void Engine::update(string relationName, vector<string> oldNames,
					vector<string> newNames, Condition condition) {
	
	// Check that old and new names have the same size
	if (oldNames.size() != newNames.size()) {
		cout << "update_fail: ";
		cout << "Different number of attr-names and literals." << endl;
		return;
	}

	// Check that passed condition isn't empty
	if (condition.isEmpty()) {
		cout << "update_fail: ";
		cout << "Empty condition given." << endl;
		return;
	}
						
	// Try to find the relation passed				
	int index = searchRelation(relationName);
	
	if(index < 0) {
		cout << "update_fail: ";
		cout << "There are no saved relations with this name." << endl;
		return;
	}
	
	Relation *updates = &relations[index];

	// For each of the matching attribute names, store the indexes for direct access
	vector<int> attrIndices;
	for(int i = 0; i < oldNames.size(); i++) {
		for(int j = 0; j < updates->getWidth(); j++) {
			if(updates->getLabelTuple()->getAttribute(j)->getName() == oldNames[i]) {
				attrIndices.push_back(j);
				break;
			}
		}
	}
	
	// Check if the attribute names were found at all
	if(attrIndices.size() != oldNames.size()) {
		cout << "update_fail: ";
		cout << "Could not find all given attributes in given table." << endl;
		return;
	}

	// Check the conditions tuple by tuple for the given attributes
	// Update value if condition is true
	for(int i = 1; i < updates->getHeight(); i++) {
		list<bool> results;
		int numConditions = condition.getNumConditions();
		
		for (int j = 0; j < numConditions; j++) {
			string leftOp  = condition.getLeftOperand(j),
				   op	   = condition.getOperand(j),
				   rightOp = condition.getRightOperand(j);
			
			// Check cases for name or literal comparison
			// If the name is not found, consider to be a literal
			int indexLeft  = updates->getLabelTuple()->searchAttribute(leftOp),
				indexRight = updates->getLabelTuple()->searchAttribute(rightOp);

			// Case both are names
			if (indexLeft >= 0 && indexRight >= 0) {
				string leftName  = updates->getTuple(i)->getAttribute(indexLeft)->getName(),
					   rightName = updates->getTuple(i)->getAttribute(indexRight)->getName();

				results.push_back(condition.compareNames(leftName, rightName, op));
			}
			// Case left is name, right is literal
			else if (indexLeft >= 0 && indexRight < 0) {
				string leftName = updates->getTuple(i)->getAttribute(indexLeft)->getName();

				results.push_back(condition.compareNameLiteral(leftName, rightOp, op));
			}
			// Case left is literal, right is name
			else if (indexLeft < 0 && indexRight >= 0) {
				string rightName = updates->getTuple(i)->getAttribute(indexRight)->getName();

				results.push_back(condition.compareLiteralName(leftOp, rightName, op));
			}
			// Case both are literals
			else {
				results.push_back(condition.compareLiterals(leftOp, rightOp, op));
			}
		}

		// Combine the results
		// Note: AND has precedence over OR

		// A pointer to the beginning of results list
		list<bool>::iterator it = results.begin();

		// Iterate through each conjunction
		for (int k = 0; k < numConditions - 1; k++) {
			// If OR, keep the result and move the pointer			
			if (condition.getConjunction(k).compare("||") == 0) {
				it++;
			}
			// If AND, merge results and don't move pointer
			else {
				bool left = *it, right = *(next(it,1));

				*it = left && right;
				results.erase(it++);
			}
		}

		// The new results list now holds the results of all AND operations
		// Iterate again to get result of OR
		it = results.begin();
		bool finalResult = *it;

		while(next(it,1) != results.end()) {
			finalResult = finalResult || *(next(it,1));
			it++;
		}

		// Finally, if the final result is true, update attributes with new names
		if (finalResult) {
			for (int k = 0; k < attrIndices.size(); k++) {	
				int index = attrIndices[k];
					
				updates->getTuple(i)->getAttribute(index)->setName(newNames[k]);
			}
		}
	}
}

void Engine::show(string relationName) {
	// Search in relations list
	int index = searchRelation(relationName);

	if(index >= 0)
		relations[index].printTuples();
	else 
		cout << "There are no relations with this name." << endl;
}

/*--------------------------------------------------------------------------------
 * This is a helper function for INSERT, to identify if a literal is an integer.
 *--------------------------------------------------------------------------------*/
bool isNumber(string s) {
	for (int i = 0; i < s.length(); i++) {
		if (!isdigit(s[i])) return false;
	}
	return true;
}

void Engine::insertInto(string relationName, vector<string> literals) {
	int r = searchRelation(relationName);
	
	if (r < 0) {
		cout << "There are no relations with this name." << endl;
		return;
	}
	
	else {
		vector<Attribute> attlist;
		for (int i = 0; i < literals.size(); i++) {
			//check type of literal
			string lit = literals[i];
			Attribute att;
			att.setName(lit);

			if (isNumber(lit))
			{
				att.setType(Datatype::INTEGER);
			}
			else
			{
				att.setType(Datatype::VARCHAR);
			}
			attlist.push_back(att);
		}

		Tuple atttuple(attlist);
		relations[r].addTuple(atttuple);
	}
}
	
void Engine::insertInto(string relationName, Relation relation) {
	int r = searchRelation(relationName);
	
	if (r < 0)
		cout << "There are no relations with this name." << endl;
	else
	{
		//skips first tuple to ignore labels
		for (int i = 1; i < relation.getHeight(); i++)
		{
			relations[r].addTuple(*(relation.getTuple(i)));
		}
	}
}

void Engine::deleteFrom(string relationName, Condition condition) {
	
	// Check that passed condition isn't empty
	if (condition.isEmpty()) {
		cout << "delete_fail: ";
		cout << "Empty condition given." << endl;
		return;
	}
	
	// Try to find the relation passed
	int index = searchRelation(relationName);

	if(index < 0) {
		cout << "delete_fail: ";
		cout << "There are no saved relations with this name." << endl;
		return;
	}

	// Create a pointer to the relation
	Relation* r = &relations[index];

	// For each tuple of r, check each comparison
	for(int i = 1; i < r->getHeight();) {
		list<bool> results;
		int numConditions = condition.getNumConditions();
		
		for (int j = 0; j < numConditions; j++) {
			string leftOp  = condition.getLeftOperand(j),
				   op	   = condition.getOperand(j),
				   rightOp = condition.getRightOperand(j);

			// Check cases for name or literal comparison
			// If the name is not found, consider to be a literal
			int indexLeft  = r->getLabelTuple()->searchAttribute(leftOp),
				indexRight = r->getLabelTuple()->searchAttribute(rightOp);

			// Case both are names
			if (indexLeft >= 0 && indexRight >= 0) {
				string leftName  = r->getTuple(i)->getAttribute(indexLeft)->getName(),
					   rightName = r->getTuple(i)->getAttribute(indexRight)->getName();

				results.push_back(condition.compareNames(leftName, rightName, op));
			}
			// Case left is name, right is literal
			else if (indexLeft >= 0 && indexRight < 0) {
				string leftName = r->getTuple(i)->getAttribute(indexLeft)->getName();

				results.push_back(condition.compareNameLiteral(leftName, rightOp, op));
			}
			// Case left is literal, right is name
			else if (indexLeft < 0 && indexRight >= 0) {
				string rightName = r->getTuple(i)->getAttribute(indexRight)->getName();

				results.push_back(condition.compareLiteralName(leftOp, rightName, op));
			}
			// Case both are literals
			else {
				results.push_back(condition.compareLiterals(leftOp, rightOp, op));
			}
		}

		// Combine the results
		// Note: AND has precedence over OR

		// A pointer to the beginning of results list
		list<bool>::iterator it = results.begin();

		// Iterate through each conjunction
		for (int k = 0; k < numConditions - 1; k++) {
			// If OR, keep the result and move the pointer			
			if (condition.getConjunction(k).compare("||") == 0) {
				it++;
			}
			// If AND, merge results and don't move pointer
			else {
				bool left = *it, right = *(next(it,1));

				*it = left && right;
				results.erase(it++);
			}
		}

		// The new results list now holds the results of all AND operations
		// Iterate again to get result of OR
		it = results.begin();
		bool finalResult = *it;

		while(next(it,1) != results.end()) {
			finalResult = finalResult || *(next(it,1));
			it++;
		}

		// Finally, if the final result is true, delete tuple from relation
		// If tuple is deleted, don't move to next tuple
		if(finalResult) {
			r->removeTuple(i);
		} else i++;
	}
}

int Engine::searchRelation(string relationName) {
	for(int i = 0; i < relations.size(); i++) {
		string name = relations[i].getName();

		if(name.compare(relationName) == 0) 
			return i;
	}
	return -1;
}