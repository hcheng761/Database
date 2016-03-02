#include <iostream>
#include <list>
#include "Relation.h"

using namespace std;

Relation::Relation() {
	width = 0;
}

Relation::Relation(string name) { 
	this->name = name;
	width = 0;
}

Relation::Relation(const Relation& r) {
	this->width = r.width;
	this->name = r.name;
	this->tuples = r.tuples;
}

Relation::~Relation() {}

void Relation::setName(string n)
{
	name = n;
}

string Relation::getName() { return name;}

int Relation::getWidth() { return width; }

int Relation::getHeight() { return tuples.size(); }

bool Relation::isEmpty() { return tuples.size() == 0; }

bool Relation::isVoid() { return name.empty() && tuples.size() == 0; }

Tuple* Relation::getLabelTuple() { 
	if(isEmpty())
		return NULL;
	else
		return &tuples[0]; 
}

Tuple* Relation::getTuple(int num) {
	if(isEmpty())
		return NULL;
	
	if(num < 0 || num > tuples.size()) {
		cerr << "Invalid tuple number. Unable to access." << endl;
		return NULL;
	} else return &tuples[num];
}

void Relation::copyTuples(const Relation& r) {
	this->width = r.width;
	this->tuples = r.tuples;
}

void Relation::printTuples() {
	for(int i = 0; i < tuples.size(); i++) {
		tuples[i].printAttributes();
	}
}

void Relation::printMetadata() {
	cout << "Table "<< name << " is " 
		 << getWidth() << " by " << getHeight()
		 << endl;
}

void Relation::addTuple(Tuple t) { 
	// If it's the first insertion, set table's width
	if(tuples.empty()) {
		width = t.getNumAttributes();
		tuples.push_back(t); 
	} else {
		// Check that inserted tuple is of same width
		if(t.getNumAttributes() == width) {
			
			// Check that attribute types are compatible
			for(int i = 0; i < t.getNumAttributes(); i++) {
				if(t.getAttribute(i)->getType() != tuples[0].getAttribute(i)->getType()) {
					cerr << "Incompatible attribute types. Tuple not inserted." << endl;
					return;
				}
			}

			tuples.push_back(t);

		} else cerr << "Incompatible number of attributes. Tuple not inserted." << endl;
	}
}

void Relation::removeTuple(int num) {
	if(num < 0 || num > tuples.size()) {
		cout << "Invalid tuple number. Tuple not removed" << endl;
		return;
	} else tuples.erase(tuples.begin() + num);
}

Relation Relation::operator=(Relation& r) {
	this->name = r.name;
	
	tuples.clear();	
	
	for(int i = 0; i < r.getHeight(); i++) {
		Tuple* t = r.getTuple(i);
		tuples.push_back((*t));
	}
	
	return *this;
}

/*----------------------------------------
 * RELATIONAL ALGEBRA OPERATIONS
 *----------------------------------------*/

bool isUnionCompatible(Relation r1, Relation r2) {
	if(r1.isEmpty()) {
		if(r2.isEmpty()) return true;
		else return false;
	} else if(r2.isEmpty()) {
		if(r1.isEmpty()) return true;
		else return false;
	} else {
		// Check that the two tables have the same number of columns
		bool sameWidth = r1.getWidth() == r2.getWidth();

		if(sameWidth) {
			// Check that correspondent columns are of same type
			// Only need to check first row (label tuple)
			for(int i = 0; i < r1.getWidth(); i++) {
				if(r1.getLabelTuple()->getAttribute(i)->getType() !=
					r2.getLabelTuple()->getAttribute(i)->getType())
					return false;
			}
			return true;
		} else return false;
	}
}

Relation rename(string viewName, vector<string> newAttrNames, Relation r) {
	// Copy relation
	Relation view(viewName);
	
	// Check that list of new names is the same size of width
	if(newAttrNames.size() != r.getWidth()) {
		cout << "rename_fail: ";
		cout << "Insufficient number of attribute-names given." << endl;
		return view;
	}
	
	view.copyTuples(r);

	// For every attribute's label, change the name while provided
	for (int i = 0; i < newAttrNames.size(); i++) {
		view.getLabelTuple()->getAttribute(i)->setName(newAttrNames[i]);
	}

	return view;
}

Relation select(string viewName, Condition condition, Relation r) {
	Relation view(viewName);

	// Check that passed condition isn't empty
	if (condition.isEmpty()) {
		cout << "select_fail: ";
		cout << "Empty condition given." << endl;
		return view;
	}

	// Check that passed relation isn't empty
	if (r.isEmpty()) {
		cout << "select_fail: ";
		cout << "Empty relation given." << endl;
		return view;
	}

	// Check the conditions tuple by tuple
	// Add tuple to view if condition is true
	for(int i = 1; i < r.getHeight(); i++) {
		list<bool> results;
		int numConditions = condition.getNumConditions();

		for (int j = 0; j < numConditions; j++) {
			string leftOp  = condition.getLeftOperand(j),
				   op	   = condition.getOperand(j),
				   rightOp = condition.getRightOperand(j);
			
			// Check cases for name or literal comparison
			// If the name is not found, consider to be a literal
			int indexLeft  = r.getLabelTuple()->searchAttribute(leftOp),
				indexRight = r.getLabelTuple()->searchAttribute(rightOp);

			// Case both are names
			if (indexLeft >= 0 && indexRight >= 0) {
				string leftName  = r.getTuple(i)->getAttribute(indexLeft)->getName(),
					   rightName = r.getTuple(i)->getAttribute(indexRight)->getName();

				results.push_back(condition.compareNames(leftName, rightName, op));
			}
			// Case left is name, right is literal
			else if (indexLeft >= 0 && indexRight < 0) {
				string leftName = r.getTuple(i)->getAttribute(indexLeft)->getName();

				results.push_back(condition.compareNameLiteral(leftName, rightOp, op));
			}
			// Case left is literal, right is name
			else if (indexLeft < 0 && indexRight >= 0) {
				string rightName = r.getTuple(i)->getAttribute(indexRight)->getName();

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

		// Finally, if the final result is true, add the tuple to the view
		if(finalResult) view.addTuple(*(r.getTuple(i)));
	}

	return view;
}

Relation project(string viewName, vector<string> attributes, Relation r) {
	Relation view(viewName);

	// Check that passed relation isn't empty
	if (r.isEmpty()) {
		cout << "project_fail: ";
		cout << "Empty relation given." << endl;
		return view;
	}
	
	// Search for the attribute names in the first tuple
	vector<int> indices;
	for(int i = 0; i < attributes.size(); i++) {
		for(int j = 0; j < r.getWidth(); j++) {
			string name = r.getLabelTuple()->getAttribute(j)->getName();

			// If that attribute exists, save the index
			if(attributes[i].compare(name) == 0) {
				indices.push_back(j);
				break;
			}
		}
	}

	// Check if the attribute names were found at all
	if(indices.size() != attributes.size()) {
		cout << "project_fail: ";
		cout << "Could not find all given attributes in given table." << endl;
		return view;
	}

	// Now we know which indices in table to copy in each tuple
	vector<Attribute> atts;
	for(int i = 0; i < r.getHeight(); i++) {
		// Directly access the attributes
		for(int j = 0; j < indices.size(); j++) {
			int index = indices[j];
			atts.push_back(*(r.getTuple(i)->getAttribute(index)));
		}

		// Create the tuple and add to table
		Tuple t(atts);
		view.addTuple(t);
		atts.clear();
	}

	return view;
}

Relation setUnion(string viewName, Relation r1, Relation r2) {
	Relation view(viewName);
	
	if(isUnionCompatible(r1, r2)) {
		// Copy all tuples of r1 to view
		view.copyTuples(r1);

		// Insert only the tuples of r2 that differ
		bool diff = false;
		for(int i = 1; i < r2.getHeight(); i++) {
			for(int j = 0; j < view.getHeight(); j++) {
				if(r2.getTuple(i) != view.getTuple(j))
					diff = true;
				else {
					diff = false;
					break;
				}
			}

			if(diff == true) view.addTuple(*(r2.getTuple(i)));
		}
		
		return view;
	} else {
		cout << "union_fail: ";
		cout << "The two relations are not compatible" << endl;
		return view;
	}
}

Relation setDifference(string viewName, Relation r1, Relation r2) {
	Relation view(viewName);
	if(isUnionCompatible(r1, r2)) {

		// If r1 is empty, the result is empty
		if(r1.isEmpty()) return view;

		// If r2 is empty, the result is r1
		if(r2.isEmpty()) {
			view = r1;
			return view;
		}

		view = r1;

		// Remove all equal tuples
		for(int i = 1; i < r2.getHeight(); i++) {
			for(int j = 1; j < view.getHeight(); j++) {
				Tuple t1 = *(r2.getTuple(i));
				Tuple t2 = *(view.getTuple(j));
				
				if(t1 == t2) view.removeTuple(j);
			}
		}

		return view;
	} else {
		cout << "difference_fail: ";
		cout << "The two relations are not compatible" << endl;
		return view;
	}
}

Relation crossProduct(string viewName, Relation r1, Relation r2) {
	Relation view(viewName);
	vector<Attribute> atts;
	vector<Tuple> tuples;

	// For each tuple of a, pair up with tuples of b
	for(int i = 0; i < r1.getHeight(); i++) {
		for(int j = 0; j < r2.getHeight(); j++) {
			Tuple t = *(r1.getTuple(i)) + *(r2.getTuple(j));
			view.addTuple(t);
		}
	}

	return view;
}