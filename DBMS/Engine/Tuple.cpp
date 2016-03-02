#include <iostream>
#include "Tuple.h"

using namespace std;

Tuple::Tuple() {}

Tuple::Tuple(vector<Attribute> attributes) {
	for(int i = 0; i < attributes.size(); i++) {
		this->attributes.push_back(attributes[i]);
	}
}

Tuple::~Tuple() {}

int Tuple::getNumAttributes() {	return attributes.size(); }

bool Tuple::isEmpty() { return attributes.size() == 0; }

Attribute* Tuple::getAttribute(int num) {
	if(isEmpty()) return NULL;
	
	if(num < 0 || num > attributes.size()) {
		cerr << "Invalid attribute number. Unable to access." << endl;
		return NULL;
	} else return &attributes[num];
}

int Tuple::searchAttribute(string name) {
	for(int i = 0; i < attributes.size(); i++) {
		if(attributes[i].getName().compare(name) == 0) 
			return i;
	}
	return -1;
}

void Tuple::printMetadata() {
	cout << "This tuple has " << getNumAttributes() << " attributes." << endl;
	
	for(int i = 0; i < attributes.size(); i++) {
		cout << attributes[i].getName() << "\t";
		attributes[i].printType();
		cout << endl;
	}
}

void Tuple::printAttributes() {
	for(int i = 0; i < attributes.size(); i++) {
		cout << attributes[i].getName() << "\t| ";
	}
	cout << endl;
}

Tuple& Tuple::operator=(Tuple& t) {
	attributes.clear();
	
	for(int i = 0; i < t.getNumAttributes(); i++) {
		attributes.push_back(*(t.getAttribute(i)));
	}
	
	return *this;
}

Tuple Tuple::operator+(Tuple& t) {
	vector<Attribute> attrs;

	// Put all attributes in one same vector
	for(int i = 0; i < this->getNumAttributes(); i++) {
		attrs.push_back(*(this->getAttribute(i)));
	}

	for(int i = 0; i < t.getNumAttributes(); i++) {
		attrs.push_back(*(t.getAttribute(i)));
	}

	Tuple tuple(attrs);
	return tuple;
}

bool Tuple::operator==(Tuple& t) {
	// First check that two tuples are of the same size
	if(this->getNumAttributes() != t.getNumAttributes()) return false;
	
	for(int i = 0; i < t.getNumAttributes(); i++) {
		if(*(this->getAttribute(i)) != *(t.getAttribute(i)))
			return false;
	}
	return true;
}

bool Tuple::operator!=(Tuple& t) {
	return !(*this == t);
}