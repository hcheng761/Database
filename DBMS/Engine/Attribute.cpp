#include "Attribute.h"
#include <iostream>

Attribute::Attribute()
{}

Attribute::Attribute(string name, Datatype type) {
	this->name = name;
	this->type = type;
	capacity = 0;
}

Attribute::Attribute(string name, Datatype type, int capacity) {
	this->name = name;
	this->type = type;
	this->capacity = capacity;
}

Attribute::~Attribute() {}

void Attribute::printMetadata() {
	cout << "NAME: " << name << "\t"
		 << "TYPE: ";
	printType();
	cout << endl;
}

void Attribute::printType() {
	switch(type) {
	case VARCHAR:
		cout << "VARCHAR";
		break;
	case INTEGER: 
		cout << "INTEGER";
		break;
	}
}

string Attribute::getName() { return name; }

string Attribute::getTypeString() {
	switch(type) {
	case VARCHAR:
		return "VARCHAR";
		break;
	case INTEGER: 
		return "INTEGER";
		break;
	}
}

Datatype Attribute::getType() { return type; }

int Attribute::getCapacity() { return capacity; }

void Attribute::setName(string name) { this->name = name; }

void Attribute::setType(Datatype type) { this->type = type; }

bool Attribute::operator==(Attribute& a) {
	return (this->getName() == a.getName() && 
			this->getType() == a.getType());
}

bool Attribute::operator!=(Attribute& a) {
	return !(*this == a);
}

bool Attribute::operator>=(Attribute& a) {
	if(this->getType() != a.getType()) {
		cerr << "Cannot compare attributes of different types";
		return false;
	} else {
		if(a.getType() == Datatype::INTEGER)
			return stoi(this->getName()) >= stoi(a.getName());
		else
			return this->getName() >= a.getName();
	}
}

bool Attribute::operator<=(Attribute& a) {
	if(this->getType() != a.getType()) {
		cerr << "Cannot compare attributes of different types";
		return false;
	} else {
		if(a.getType() == Datatype::INTEGER)
			return stoi(this->getName()) <= stoi(a.getName());
		else
			return this->getName() <= a.getName();
	}
}

bool Attribute::operator>(Attribute& a) {
	if(this->getType() != a.getType()) {
		cerr << "Cannot compare attributes of different types";
		return false;
	} else {
		if(a.getType() == Datatype::INTEGER)
			return stoi(this->getName()) > stoi(a.getName());
		else
			return this->getName() > a.getName();
	}
}

bool Attribute::operator<(Attribute& a) {
	if(this->getType() != a.getType()) {
		cerr << "Cannot compare attributes of different types";
		return false;
	} else {
		if(a.getType() == Datatype::INTEGER)
			return stoi(this->getName()) < stoi(a.getName());
		else
			return this->getName() < a.getName();
	}
}
