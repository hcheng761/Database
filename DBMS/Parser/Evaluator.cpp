/*--------------------------------------------------------------------------------
 * DESCRIPTION: The Evaluator is the core class in terms of interpreting the
 * grammar.
 *
 * DATE: September 2015
 *
 * NOTES:
 * 1. The pointer 'cur' behavior must be tightly controlled for correct reading.
 * Therefore, it is default that:
 * a) If a function calls another evaluating function, the caller moves the
 * pointer before calling;
 * b) An eval function always begins with the assumption that it's caller
 * has correctly set the pointer to begin reading.
 *
 * 2. Each evaluating function is responsible for printing error messages within
 * its own scope. E.g. The evalOpen function calls evalRelationName. If the latter
 * fails, it is the one to print an error message, not the first.
 *--------------------------------------------------------------------------------*/

#include <iostream>
#include "Evaluator.h"

using namespace std;

/*--------------------------------------------------
 * Incomplete error message
 *--------------------------------------------------*/
void printIncompleteError(string function, string description) {
	cerr << "Incomplete " << function << ". "
		 << description << endl;
}

/*--------------------------------------------------
 * Invalid error message
 *--------------------------------------------------*/
void printInvalidError(string function, string expected, string read) {
	cerr << "Invalid " << function << ". "
		 << "Expected: " << expected 
		 << " Read: " << read << endl;
}

/*--------------------------------------------------
 * Returns true if the cur pointer is at the last
 * token
 *--------------------------------------------------*/
bool Evaluator::isEndOfLine() {
	// Check if pointer is at or beyond last token
	bool eol = (cur >= tokens.size() - 1);
	
	// If true, print a message
	if(eol) {
		cout << "Pointer has reached the end of line." << endl;
	}

	return eol;
}

Evaluator::Evaluator(Engine engine, vector<string> tokens) {
	cur = 0;
	this->tokens = tokens;
	this->engine = engine;
}

Evaluator::~Evaluator() {}

/*--------------------------------------------------
 * A line starts with a command or a query.
 * The first token should be a word all in upper 
 * case or all in lower case. If not, no action is
 * performed. 
 * The pointer is not moved.
 *--------------------------------------------------*/
Engine Evaluator::evalProgram() {
	// Check that tokens isn't empty
	if(tokens.empty()) return engine;
	
	string firstTok = tokens[0];

	// If the first character is uppercase, possibly a commmand
	if(isupper(firstTok[0])) {
		// Check that all characters are uppercase
		for(int i = 1 ; i < firstTok.length() ; i++) {
			if(!isupper(firstTok[i])) {
				cerr << "Invalid command." << endl;
				return engine;
			}
		}

		engine = evalCommand();
	} 
	// If the first character is lowercase, possibly a query
	else {
		// Check that all characters are lowercase
		for(int i = 1 ; i < firstTok.length() ; i++) {
			if(isupper(firstTok[i])) {
				cerr << "Invalid query." << endl;
				return engine;
			}
		}
		engine = evalQuery();
	}
	return engine;
}

/*--------------------------------------------------
 * A command begins with the command name, followed
 * by its syntax.
 * The pointer is moved if the command name is valid.
 *--------------------------------------------------*/
Engine Evaluator::evalCommand() {
	string token = tokens[cur];
	
	// Check if pointer can move
	if(isEndOfLine()) {
		cerr << "Incomplete command." << endl;
		return engine;
	}
	
	string nextToken = tokens[cur + 1];
	
	if(token.compare("OPEN") == 0) {
		cur++;
		engine = evalOpen();
		return engine;
	}
	else if (token.compare("CLOSE") == 0) {
		cur++;
		engine = evalClose();
		return engine;
	}
	else if (token.compare("SAVE") == 0) {
		cur++;
		engine = evalSave();
		return engine;
	}
	else if (token.compare("EXIT") == 0) {
		if (nextToken == ";") return engine;
	}
	else if (token.compare("SHOW") == 0) {
		cur++;
		engine = evalShow();
		return engine;
	}
	else if (token.compare("UPDATE") == 0) {
		cur++;
		engine = evalUpdate();
		return engine;
	}
	else if (token.compare("CREATE") == 0 && nextToken.compare("TABLE") == 0) {
		cur+=2;
		engine = evalCreate();
		return engine;
	}
	else if (token.compare("INSERT") == 0 && nextToken.compare("INTO") == 0) {
		cur+=2;
		engine = evalInsert();
		return engine;
	}
	else if (token.compare("DELETE") == 0 && nextToken.compare("FROM") == 0) {
		cur+=2;
		engine = evalDelete();
		return engine;
	} else {
		cerr << "Invalid command." << endl;
		return engine;
	}
}

Engine Evaluator::evalOpen(){
	// Read current token: relation-name
	string openName = evalRelationName(tokens[cur]);
	// Check if relation name is valid
	if (openName.empty()) {
		return engine;
	} else {
		// Check if pointer can move
		if (isEndOfLine()) {
			printIncompleteError("open", "Missing ;");
			return engine;
		} else {
			// Move pointer to next token: semicolon
			cur++;
			string semiCheck = tokens[cur];
			if(semiCheck.compare(";") == 0)
				engine.open(openName);
			else printInvalidError("open", ";", semiCheck);
		}
	}
	return engine;
}

Engine Evaluator::evalClose() {
	// Read current token: relation-name
	string closeName = evalRelationName(tokens[cur]);
	
	// Check if relation name is valid
	if (closeName.empty()) {
		return engine;
	} else {
		// Check if pointer can move
		if (isEndOfLine()) {
			printIncompleteError("close", "Missing ;");
			return engine;
		} else {
			// Move pointer to next token: semicolon
			cur++;
			string semiCheck = tokens[cur];
			if (semiCheck.compare(";") == 0)
				engine.close(closeName);
			else printInvalidError("close", ";", semiCheck);
		}
	}
	return engine;
}

Engine Evaluator::evalSave() {
	// Read current token: relation-name
	string saveName = evalRelationName(tokens[cur]);
		
	// Check if relation name is valid
	if (saveName.empty()) {
		return engine;
	} else {
		// Check if pointer can move
		if (isEndOfLine()) {
			printIncompleteError("save", "Missing ;");
			return engine;
		} else {
			// Move pointer to next token: semicolon
			cur++;
			string semiCheck = tokens[cur];
			if(semiCheck.compare(";") == 0)
				engine.save(saveName);
			else printInvalidError("save", ";", semiCheck);
		}
	}
	return engine;
}

Engine Evaluator::evalShow() {
	// Read current token: relation-name
	string showName = evalRelationName(tokens[cur]);
	
	// Check if relation name is valid
	if (showName.empty()) {
		return engine;
	} else {
		// Check if pointer can move
		if (isEndOfLine()) {
			printIncompleteError("show", "Missing ;");
			return engine;
		} else {
			// Move pointer to next token: semicolon
			cur++;
			string semiCheck = tokens[cur];
			if(semiCheck.compare(";") == 0)
				engine.show(showName);
			else printInvalidError("show", ";", semiCheck);
		}
	}
	return engine;
}

Engine Evaluator::evalCreate() {
	string createName = evalRelationName(tokens[cur]);
	
	// Check if relation name is valid
	if (createName.empty()) return engine;
	
	// Check if pointer can move
	if(isEndOfLine()) {
		printIncompleteError("create", "Missing (");
		return engine;
	}
	
	// Move pointer to next token: (
	cur++;
	string parenthesisCheck = tokens[cur];
			
	if(tokens[cur].compare("(") == 0) {
		// Check if pointer can move		
		if (isEndOfLine()) {
			printIncompleteError("create", "Missing typed-attribute-list");
			return engine;
		}
		
		// Move pointer to next token: typed-attribute-list
		cur++;
		vector<Attribute> typedAttrList = evalTypedAttributeList();
					
		// Check that typed attribute list is valid
		if(typedAttrList.empty()) {
			return engine;
		}
		
		// Check if pointer can move
		if(isEndOfLine()) {
			printIncompleteError("create", "Missing )");
			return engine;
		}
		
		// Move pointer to next token: )
		//cur++;
		parenthesisCheck = tokens[cur];

		if(parenthesisCheck.compare(")") == 0) {
			// Check if pointer can move
			if (isEndOfLine()) {
				printIncompleteError("create", "Missing PRIMARY");
				return engine;
			}
			
			// Move pointer to next token: PRIMARY
			cur++;
			string primaryCheck = tokens[cur];

			if(primaryCheck.compare("PRIMARY") == 0) {
				// Check if pointer can move
				if (isEndOfLine()) {
					printIncompleteError("create", "Missing KEY");
					return engine;
				}
				// Move pointer to next token: KEY
				cur++;
				string keyCheck = tokens[cur];

				if(keyCheck.compare("KEY") == 0) {
					// Check if pointer can move
					if (isEndOfLine()) {
						printIncompleteError("create", "Missing (");
						return engine;
					}

					// Move pointer to next token: (
					cur++;
					parenthesisCheck = tokens[cur];

					if(parenthesisCheck.compare("(") == 0)	{
						// Check if pointer can move
						if (isEndOfLine()) {
							printIncompleteError("create", "Missing attribute-list");
							return engine;
						}
						
						// Move pointer to next token: attribute-list
						cur++;
						vector<string> attrList = evalAttributeList();
						
						// Check if attribute list is valid
						if(attrList.empty()) {
							return engine;
						}
						
						// Check if pointer can move
						if(isEndOfLine()) {
							printIncompleteError("create", "Missing )");
							return engine;
						}
						
						// Move pointer to next token: )
						cur++;
						parenthesisCheck = tokens[cur];

						if(tokens[cur].compare(")") == 0) {
							// Check if pointer can move
							if (isEndOfLine()) {
								printIncompleteError("create", "Missing ;");
								return engine;
							}
							
							// Move pointer to next token: ;
							cur++;
							string semiCheck = tokens[cur];
							
							if(semiCheck.compare(";") == 0)
								engine.create(createName, typedAttrList);
								
							else printInvalidError("create", ";", semiCheck);

						} else printInvalidError("create", ")", parenthesisCheck); // primary key
					} else printInvalidError("create", "(", parenthesisCheck);
				} else printInvalidError("create", "KEY", keyCheck);
			} else printInvalidError("create", "PRIMARY", primaryCheck);
		} else printInvalidError("create", ")", parenthesisCheck); // typed attribute list
	} else printInvalidError("create", "(", parenthesisCheck);
	return engine;
}

Engine Evaluator::evalUpdate() {
	string updateName = evalRelationName(tokens[cur]);
	vector<string> oldNames;
	vector<string> newNames;
	Condition condition;
	bool whilebreak = false;	//for while loop later on

	// Check if relation name is valid
	if (updateName.empty()) return engine;
	
	// Check if pointer can move
	if(isEndOfLine()) {
		printIncompleteError("update", "Missing SET");
		return engine;
	}
	
	// Move pointer to next token: SET
	cur++;
	string setCheck = tokens[cur];
	
	if (setCheck.compare("SET") == 0) {
		// Check if pointer can move
		if (isEndOfLine()) {
			printIncompleteError("update", "Missing attribute-name");
			return engine;
		}
		
		// Move pointer to next token: attribute-name
		cur++;
		string nextToken;
		
		// Check for one or multiple attribute-name = literal
		do {
			string attrName = evalAttributeName(tokens[cur]);
		
			// Check if attribute name is valid
			if (attrName.empty()) return engine;
		
			// Push to old names list
			oldNames.push_back(attrName);
		
			// Check if pointer can move
			if (isEndOfLine()) {
				printIncompleteError("update", "Missing =");
				return engine;
			}

			// Move pointer to next token: =
			cur++;
			string equalsCheck = tokens[cur];

			if(equalsCheck.compare("=") == 0) {
				// Check if pointer can move
				if (isEndOfLine()) {
					printIncompleteError("update", "Missing literal");
					return engine;
				}
			
				// Move pointer to next token: literal
				cur++;
				string lit = tokens[cur];

				// Push literal to new names list
				newNames.push_back(tokens[cur]);
			
				// Check if pointer can move
				if (isEndOfLine()) {
					printIncompleteError("update", "Missing WHERE");
					return engine;
				}

				// Move pointer to next token: WHERE or ,
				cur++;
				nextToken = tokens[cur];

				if(isEndOfLine()) {
					printIncompleteError("update", "Missing attribute-name or condition");
					return engine;
				} else {
					// Move the pointer to the next token: attribute-name
					cur++;
				}
			} else {
				printInvalidError("update", "=", equalsCheck);
				return engine;
			}
		} while (nextToken.compare(",") == 0);

		// Here, nextToken found something different from ,
		// Either WHERE or something else
		// Note that pointer has moved already
		if(nextToken.compare("WHERE") == 0) {
			// Check if pointer can move
			if (isEndOfLine()) {
				printIncompleteError("update", "Missing condition");
				return engine;
			}
				
			Condition condition = evalCondition();
				
			// Check if condition is valid
			if (condition.isEmpty()) return engine;
				
			// Check if pointer can move
			if(isEndOfLine()) {
				printIncompleteError("update", "Missing ;");
				return engine;
			}

			// Move pointer to next token: ;
			cur++;
			string semiCheck = tokens[cur];
							
			if(semiCheck.compare(";") == 0)
				engine.update(updateName, oldNames, newNames, condition);
								
			else printInvalidError("update", ";", semiCheck);
				
		} else printInvalidError("update", "WHERE", nextToken);
	} else printInvalidError("update", "SET", setCheck);	
	return engine;
}

Engine Evaluator::evalInsert() {
	// Read current token: relation-name
	string relationName = tokens[cur];
	
	// Check if relation name is valid
	if (relationName.empty()) {
		return engine;
	} else {
		// Check if pointer can move
		if (isEndOfLine()) {
			printIncompleteError("insert", "Missing VALUES");
			return engine;
		} else {
			// Move pointer to next token: VALUES
			cur++;
			string valueCheck = tokens[cur];
			if(valueCheck.compare("VALUES") == 0) {
				// Check if pointer can move
				if (isEndOfLine()) {
					printIncompleteError("insert", "Missing FROM");
					return engine;
				} else {
					// Move pointer to next token: FROM
					cur++;
					string fromCheck = tokens[cur];
					if(fromCheck.compare("FROM") == 0) {
						// Check if pointer can move
						if (isEndOfLine()) {
							printIncompleteError("insert", "Missing literal or relation-name");
							return engine;
						} else {
							// Move pointer to next token: ( or RELATION
							cur++;
							string relationCheck = tokens[cur];

							// Case RELATION
							if(relationCheck.compare("RELATION") == 0) {
								// Check if pointer can move
								if (isEndOfLine()) {
									printIncompleteError("insert", "Missing expression");
									return engine;
								} else {
									// Move pointer to next token: expression
									cur++;
									Relation rel = evalExpression();
									cur--;
									// Check if valid relation
									if (rel.isVoid()) return engine;
									else {
										// Check if pointer can move
										if (isEndOfLine()) {
											printIncompleteError("insert", "Missing ;");
											return engine;
										} else {
											cur++;
											string semiCheck = tokens[cur]; 
											if((semiCheck.compare(";")) == 0) 
												engine.insertInto(relationName, rel);
											else printInvalidError("insert", ";", semiCheck);
										}										
									} 
								}
							} else if(relationCheck.compare("(") == 0) {
								// Check if pointer can move
								if (isEndOfLine()) {
									printIncompleteError("insert", "Missing literal");
									return engine;
								} else {
									// Move pointer to next token: literal
									cur++;
									string lit = tokens[cur];
									vector<string> literals;
									
									// While closing parenthesis isn't found and end of line not reached
									while(lit.compare(")") != 0) {
								//		cout << cur << "/" << tokens.size() << endl;
										if(lit.compare("\"") == 0 || lit.compare(",") == 0) {
											cur++;
											if(cur == tokens.size()) break;
											lit = tokens[cur];
										} else {
											literals.push_back(lit);
											cur++;
											if(cur == tokens.size()) break;
											lit = tokens[cur];
										}
									}

									// Current has moved in the while loop to either eol or )
									if(tokens[cur].compare(")") == 0) {
										// Check if pointer can move
										if (isEndOfLine()) {
											printIncompleteError("insert", "Missing ;");
											return engine;
										} else {
											// Move pointer to next token: literal
											cur++;
											string semiCheck = tokens[cur];
											if(semiCheck.compare(";") == 0)
												 engine.insertInto(relationName, literals);
											else printInvalidError("open", ";", semiCheck);
										}
									} else {
										printIncompleteError("insert", "Missing )");
									}
								}
							}
						}
					} else {
						printInvalidError("insert", "FROM", fromCheck);
						return engine;
					}
				}
			} else {
				printInvalidError("insert", "VALUES", valueCheck);
				return engine;
			}
		}
	}	
	return engine;
}

Engine Evaluator::evalDelete() {
	// Read current token: relation-name
	string deleteName = evalRelationName(tokens[cur]);

	if (deleteName.empty()) {
		return engine;
	} else {
		// Check if pointer can move
		if (isEndOfLine()) {
			printIncompleteError("delete", "Missing WHERE");
			return engine;
		} else {
			// Move pointer to next token: semicolon
			cur++;
			string whereCheck = tokens[cur];//check for proper syntax (WHERE)
			if(whereCheck.compare("WHERE") == 0) {
				// Check if pointer can move
				if (isEndOfLine()) {
					printIncompleteError("show", "Missing ;");
					return engine;
				} else {
					// Move pointer to next token: semicolon
					cur++;
					Condition condition = evalCondition();

					// Check if condition is valid
					if(condition.isEmpty()) {
						return engine;
					} else {
						engine.deleteFrom(deleteName, condition);
					}
				}
			} else printInvalidError("delete", "WHERE", whereCheck);
		}
	}
	return engine;
}

/*--------------------------------------------------
 * A query starts with a relation's name, followed 
 * by the relation assignment '<-' and an expression. 
 *--------------------------------------------------*/
Engine Evaluator::evalQuery() {
	string relationName = evalRelationName(tokens[cur]);

	// Check if relation-name is valid
	if(relationName.empty()) {
		return engine;
	} else {
		// Check if pointer can move
		if(isEndOfLine()) {
			printIncompleteError("query", "Missing <-");
			return engine;
		} else {
			// Move pointer to next token: <-
			cur++;
			string arrowCheck = tokens[cur];
			
			if(arrowCheck.compare("<-") == 0) {
				// Check if pointer can move
				if(isEndOfLine()) {
					printIncompleteError("query", "Missing expression");
					return engine;
				} else {
					// Move pointer to next token: expression
					cur++;
					
					Relation rel = evalExpression();
//					rel.printTuples();
					cur--;
					// Check if relation is valid
					if(rel.isVoid()) {
						return engine;
					} else {
						// Check if pointer can move
						if(isEndOfLine()) {
							printIncompleteError("query", "Missing ;");
							return engine;
						} else {
							// Move pointer to next token: ;
							cur++;
							string semiCheck = tokens[cur];
							//if(semiCheck.compare(";") == 0) {
								rel.setName(relationName);
								cout << rel.getName() << endl;
								//I changed this
								//rel.printTuples();								
								engine.addRelation(rel);
								//engine.show(relationName);
								//engine.setRelation(relationName, rel);

							//} else printInvalidError("open", ";", semiCheck);
						}
					}
				}
			} else printInvalidError("query", "<-", arrowCheck);		
		}
	}
	return engine;
}

Relation Evaluator::evalSelection() { 
	Relation rel;
	
	string parenthesisCheck = tokens[cur];
	
	// If the current token is "(" start finding conditions
	if (parenthesisCheck.compare("(") == 0) {
		// Check if pointer can move
		if(isEndOfLine()) {
			printIncompleteError("selection", "Missing condition");
			return Relation();
		} else {
			// Move pointer to next token: condition
			cur++;
			Condition selectCondition = evalCondition();

			// Check if condition is valid
			if(selectCondition.isEmpty()) {
				return Relation();
			} else {
				// Check if pointer can move
				if(isEndOfLine()) {
					printIncompleteError("selection", "Missing )");
					return Relation();
				} else {
					// Move pointer to next token: )
					cur++;
					parenthesisCheck = tokens[cur];

					if (parenthesisCheck.compare(")") == 0) {
						if(isEndOfLine()) {
							printIncompleteError("selection", "Missing atomic-expression");
							return Relation();
						} else {
							// Move pointer to next token: atomic-expression
							cur++;
							rel = evalAtomicExpression();
							// Check if relation is valid
							if(rel.isVoid()) {
								return Relation();
							} else {
								// Check if pointer can move
								if(isEndOfLine()) {
									printIncompleteError("selection", "Missing ;");
									return Relation();
								} else {
									// Move pointer to next token: atomic-expression
									cur++;
									string semiCheck = tokens[cur];
									
									//if (semiCheck.compare(";") == 0){

										rel = select("select_result", selectCondition, rel);
										return rel;
							//		}
							//		else {
							//			cout << ":(";
							//			printInvalidError("selection", ";", semiCheck);
							//			return Relation();
							//		}
								}
							}
						}
					} else {
						printInvalidError("selection", ")", parenthesisCheck);
						return Relation();
					}
				}
			}
		}
	} else {
		printInvalidError("selection", "(", parenthesisCheck);
		return Relation();
	}
}

Relation Evaluator::evalProjection() {
	Relation rel;
	string parenthesisCheck = tokens[cur];

	if(parenthesisCheck.compare("(") == 0) {
		// Check if pointer can move
		if(isEndOfLine()) {
			printIncompleteError("projection", "Missing attribute-list");
			return Relation();
		} else {
			// Move pointer to next token: attribute-list
			cur++;
			vector<string> attrList = evalAttributeList();

			// Check if attribute list is valid
			if(attrList.empty()) {
				return Relation();
			} else {
				// Check if pointer can move
				if(isEndOfLine()) {
					printIncompleteError("projection", "Missing )");
					return Relation();
				} else {
					// Move pointer to next token: )
					cur++;
					parenthesisCheck = tokens[cur];

					if(parenthesisCheck.compare(")") == 0) {
						if(isEndOfLine()) {
							printIncompleteError("projection", "Missing atomic-expression");
							return Relation();
						} else {
							// Move pointer to next token: atomic-expression
							cur++;
							rel = evalAtomicExpression();
							// Check if relation is valid
							if(rel.isVoid()) {
								return Relation();
							} else {
								// Check if pointer can move
								if(isEndOfLine()) {
									printIncompleteError("projection", "Missing ;");
									return Relation();
								} else {
									// Move pointer to next token: atomic-expression
									cur++;
									string semiCheck = tokens[cur];
									if (semiCheck.compare(";") == 0)
										return project("project_result", attrList, rel);
									else {
										printInvalidError("projection", ";", semiCheck);
										return Relation();
									}
								}
							}
						}
					} else {
						printInvalidError("projection", ")", parenthesisCheck);
						return Relation();
					}
				}
			}
		}
	} else {
		printInvalidError("projection", "(", parenthesisCheck);
		return Relation();
	}
}

Relation Evaluator::evalRenaming() { 
	Relation rel;
	string parenthesisCheck = tokens[cur];

	if(parenthesisCheck.compare("(") == 0) {
		// Check if pointer can move
		if(isEndOfLine()) {
			printIncompleteError("renaming", "Missing attribute-list");
			return Relation();
		} else {
			// Move pointer to next token: attribute-list
			cur++;
			vector<string> attrList = evalAttributeList();

			// Check if attribute list is valid
			if(attrList.empty()) {
				return Relation();
			} else {
				// Check if pointer can move
				if(isEndOfLine()) {
					printIncompleteError("renaming", "Missing )");
					return Relation();
				} else {
					// Move pointer to next token: )
					cur++;
					parenthesisCheck = tokens[cur];

					if(parenthesisCheck.compare(")") == 0) {
						if(isEndOfLine()) {
							printIncompleteError("renaming", "Missing atomic-expression");
							return Relation();
						} else {
							// Move pointer to next token: atomic-expression
							cur++;
							rel = evalAtomicExpression();

							// Check if relation is valid
							if(rel.isVoid()) {
								return Relation();
							} else {
								// Check if pointer can move
								if(isEndOfLine()) {
									printIncompleteError("renaming", "Missing ;");
									return Relation();
								} else {
									// Move pointer to next token: atomic-expression
									cur++;
									string semiCheck = tokens[cur];
									
									if(semiCheck.compare(";") == 0)
										return rename("rename_result", attrList, rel);
									else {
										printInvalidError("renaming", ";", semiCheck);
										return Relation();
									}
								}
							}
						}
					} else {
						printInvalidError("renaming", ")", parenthesisCheck);
						return Relation();
					}
				}
			}
		}
	} else {
		printInvalidError("renaming", "(", parenthesisCheck);
		return Relation();
	}
}

/*--------------------------------------------------
 * An expression is an atomic expression or one of
 * the query operations.
 * If fails, returns empty relation.
 *--------------------------------------------------*/
Relation Evaluator::evalExpression() {
	Relation rel;
	string expr = tokens[cur];
	
	if(expr.compare("select") == 0) {
		// Check if pointer can move
		if(isEndOfLine()) {
			printIncompleteError("expression", "Missing (");
			return Relation();
		} else {
			cur++;
			return evalSelection();
		}
	} else if(expr.compare("project") == 0) {
		// Check if pointer can move
		if(isEndOfLine()) {
			printIncompleteError("expression", "Missing (");
			return Relation();
		} else {
			cur++;
			return evalProjection();
		}
	} else if(expr.compare("rename") == 0) {
		// Check if pointer can move
		if(isEndOfLine()) {
			printIncompleteError("expression", "Missing (");
			return Relation();
		} else {
			cur++;
			return evalRenaming();
		}
	} else {
		// Current token should be an atomic-expression
		rel = evalAtomicExpression();

		// Check that relation is valid
		if(rel.isVoid()) {
			return Relation();
		} else {
			// If the relation is valid and it's the last token, return relation
			if(isEndOfLine()) {
				return rel;
			} else {
				// Move pointer to next token: +, -, * or ;
				cur++;
				string op = tokens[cur];

				// If it's an operator, there should be another atomic-expression
				if(op.compare("+") == 0 ||
				   op.compare("-") == 0 ||
				   op.compare("*") == 0) {
					
					// Check if pointer can move
					if(isEndOfLine()) {
						printIncompleteError("expression", "Missing atomic-expression");
						return Relation();
					} else {
						// Move pointer to next token: atomic-expression
						cur++;
						Relation rel2 = evalAtomicExpression();

						// Check if relation is valid
						if(rel2.isVoid()) {
							return Relation();
						} else {
							// Case union
							if(op.compare("+") == 0) 
								return setUnion("union_result", rel, rel2);
							// Case difference
							else if(op.compare("-") == 0)
								return setDifference("diff_result", rel, rel2);
							// Case product
							else
								return crossProduct("product_result", rel, rel2);
						}
					}
				} 
			}
		}
	} 
}

/*--------------------------------------------------
 * An atomic expression is an relation name or a
 * parenthesized expression.
 * If fails, returns empty relation.
 *--------------------------------------------------*/
Relation Evaluator::evalAtomicExpression() {
	string token = tokens[cur];
	// Check if it's a parenthesis
	if(token.compare("(") == 0) {
		// Check if pointer can move
		if(isEndOfLine()) {
			printIncompleteError("atomic expression", "Missing expression");
			return Relation();
		} else {
			// Move pointer to next token: expression
			cur++;
			Relation rel = evalExpression();
			if(rel.isVoid()) {
				return Relation();
			} else {
				// Check if pointer can move
				if(isEndOfLine()) {
					printIncompleteError("atomic expression", "Missing )");
					return Relation();
				} else {
					// Move pointer to next token: )
					//cur++;
					string parenthesisCheck = tokens[cur];
					if(parenthesisCheck.compare(")") == 0) {
						return rel;
					}
					else printInvalidError("atomic expression", ")", parenthesisCheck);
				}
			}
		}
	} 
	// Check if it's a relation name
	else {
		string relationName = evalRelationName(token);

		// Check if name is valid
		if(relationName.empty()) {
			return Relation();
		} else {
			return engine.getRelation(relationName);
		}
	}
}

/*--------------------------------------------------
 * A condition is of format operand op operand, and
 * can be followed by other conditions after the
 * conjunction operators && or ||.
 * If fails, returns an empty condition.
 *--------------------------------------------------*//*
Condition Evaluator::evalCondition() {
	vector<string> leftOps, ops, rightOps, conjs;
	string nextToken, leftOp, op, rightOp;

	do {
		// First it reads an operand, which can be an attribute-name or literal
		// Literals are wrapped by "
		
		// Case left operand is a literal
		string quoteCheck = tokens[cur];
		if(quoteCheck.compare("\"") == 0) {
			// Check if the pointer can move
			if(isEndOfLine()) {
				printIncompleteError("condition", "Missing literal");
				return Condition();
			}

			// Move the pointer to the next token: literal
			cur++;
			string lit = tokens[cur];

			// Check if the pointer can move
			if(isEndOfLine()) {
				printIncompleteError("condition", "Missing \"");
				return Condition();
			}

			// Move the pointer to the next token: closing "
			cur++;
			quoteCheck = tokens[cur];

			if(quoteCheck.compare("\"") == 0) {
				leftOp = lit;
			} else {
				printInvalidError("condition", "\"", quoteCheck);
				return Condition();
			}
		}
		// Case left operand is an attribute-name
		else {
			leftOp = evalAttributeName(tokens[cur]);

			// Check if name is valid
			if(leftOp.empty()) return Condition();
		}
		
		// Check if the pointer can move
		if(isEndOfLine()) {
			printIncompleteError("condition", "Missing operator");
			return Condition();
		} 
		
		// Move the pointer to the next token: operator
		cur++;
		op = evalOperator(tokens[cur]);

		// Check if operator is valid
		if(op.empty()) return Condition();

		// Check if the pointer can move
		if(isEndOfLine()) {
			printIncompleteError("condition", "Missing right operand");
			return Condition();
		}

		// Move the pointer to the next token: right operand
		cur++;
		
		// Case right operand is a literal
		quoteCheck = tokens[cur];
		if(quoteCheck.compare("\"") == 0) {
			// Check if the pointer can move
			if(isEndOfLine()) {
				printIncompleteError("condition", "Missing literal");
				return Condition();
			}

			// Move the pointer to the next token: literal
			cur++;
			string lit = tokens[cur];

			// Check if the pointer can move
			if(isEndOfLine()) {
				printIncompleteError("condition", "Missing \"");
				return Condition();
			}

			// Move the pointer to the next token: closing "
			cur++;
			quoteCheck = tokens[cur];

			if(quoteCheck.compare("\"") == 0) {
				rightOp = lit;
			} else {
				printInvalidError("condition", "\"", quoteCheck);
				return Condition();
			}
		}
		// Case right operand is an attribute-name
		else {
			rightOp = evalAttributeName(tokens[cur]);

			// Check if name is valid
			if(rightOp.empty()) return Condition();
		}

		// Now verify if there's more conditions to read
		if(isEndOfLine()) {
			leftOps.push_back(leftOp);
			ops.push_back(op);
			rightOps.push_back(rightOp);
			break;
		}

		// Move the pointer to the next token: && or ||
		cur++;
		nextToken = tokens[cur];

		if(nextToken.compare("&&") == 0 || nextToken.compare("||") == 0) {
			if(isEndOfLine()) {
				printIncompleteError("condition", "Missing left operand");
				return Condition();
			} else {
				// Add the elements to their vectors
				leftOps.push_back(leftOp);
				ops.push_back(op);
				rightOps.push_back(rightOp);
				conjs.push_back(nextToken);

				// Move the pointer to the next token: left operand
				cur++;
			}
		} else {
			printInvalidError("condition", "&& or ||", nextToken);
			return Condition();
		}
	} while(nextToken.compare("&&") == 0 || nextToken.compare("||") == 0);

	return Condition(leftOps, ops, rightOps, conjs);
}
*/

Condition Evaluator::evalCondition() {
	vector<string> leftOps, ops, rightOps, conjs;
	string nextToken, leftOp, op, rightOp;
	do {
		// First it reads an operand, which can be an attribute-name or literal
		// Literals are wrapped by "

		// Case left operand is a literal
		
		string quoteCheck = tokens[cur];
		if (quoteCheck.compare("\"") == 0) {
			// Check if the pointer can move
			if (isEndOfLine()) {
				printIncompleteError("condition", "Missing literal");
				return Condition();
			}

			// Move the pointer to the next token: literal
			cur++;
			string lit = tokens[cur];

			// Check if the pointer can move
			if (isEndOfLine()) {
				printIncompleteError("condition", "Missing \"");
				return Condition();
			}

			// Move the pointer to the next token: closing "
			cur++;
			quoteCheck = tokens[cur];

			if (quoteCheck.compare("\"") == 0) {
				leftOp = lit;
			}
			else {
				printInvalidError("condition", "\"", quoteCheck);
				return Condition();
			}
		}
		// Case left operand is an attribute-name
		else {
			leftOp = evalAttributeName(tokens[cur]);

			// Check if name is valid
			if (leftOp.empty()) return Condition();
		}

		// Check if the pointer can move
		if (isEndOfLine()) {
			printIncompleteError("condition", "Missing operator");
			return Condition();
		}

		// Move the pointer to the next token: operator
		cur++;
		op = evalOperator(tokens[cur]);

		// Check if operator is valid
		if (op.empty()) return Condition();

		// Check if the pointer can move
		if (isEndOfLine()) {
			printIncompleteError("condition", "Missing right operand");
			return Condition();
		}

		// Move the pointer to the next token: right operand
		cur++;

		// Case right operand is a literal
		quoteCheck = tokens[cur];
		if (quoteCheck.compare("\"") == 0) {
			// Check if the pointer can move
			if (isEndOfLine()) {
				printIncompleteError("condition", "Missing literal");
				return Condition();
			}

			// Move the pointer to the next token: literal
			cur++;
			string lit = tokens[cur];

			// Check if the pointer can move
			if (isEndOfLine()) {
				printIncompleteError("condition", "Missing \"");
				return Condition();
			}

			// Move the pointer to the next token: closing "
			cur++;
			quoteCheck = tokens[cur];

			if (quoteCheck.compare("\"") == 0) {
				rightOp = lit;
			}
			else {
				printInvalidError("condition", "\"", quoteCheck);
				return Condition();
			}
		}
		// Case right operand is an attribute-name
		else {
			rightOp = evalAttributeName(tokens[cur]);

			// Check if name is valid
			if (rightOp.empty()) return Condition();
		}

		// Now verify if there's more conditions to read
		if (isEndOfLine()) {
			leftOps.push_back(leftOp);
			ops.push_back(op);
			rightOps.push_back(rightOp);
			break;
		}

		// Peek at next token
		nextToken = tokens[cur + 1];

		// If it's ')', it can also be the end of reading
		if (nextToken.compare(")") == 0) {
			leftOps.push_back(leftOp);
			ops.push_back(op);
			rightOps.push_back(rightOp);
			break;
		}

		// Else, actually move the pointer to the next token: && or ||
		cur++;
		nextToken = tokens[cur];

		if (nextToken.compare("&&") == 0 || nextToken.compare("||") == 0) {
			if (isEndOfLine()) {
				printIncompleteError("condition", "Missing left operand");
				return Condition();
			}
			else {
				// Add the elements to their vectors
				leftOps.push_back(leftOp);
				ops.push_back(op);
				rightOps.push_back(rightOp);
				conjs.push_back(nextToken);

				// Move the pointer to the next token: left operand
				cur++;
			}
		}
		else {
			printInvalidError("condition", "&& or ||", nextToken);
			return Condition();
		}
	} while (nextToken.compare("&&") == 0 || nextToken.compare("||") == 0);

	return Condition(leftOps, ops, rightOps, conjs);
}

/*--------------------------------------------------
 * An operator is any of the comparison operators.
 * If fails, returns an empty string.
 *--------------------------------------------------*/
string Evaluator::evalOperator(string token) {
	string emptyTok;

	if(token.compare("==") == 0 ||
	   token.compare("!=") == 0 ||
	   token.compare(">") == 0 ||
	   token.compare("<") == 0 ||
	   token.compare(">=") == 0 ||
	   token.compare("<=") == 0)
	   return token;
	else {
		cerr << "Invalid operator." << endl;
		return emptyTok;
	}

}

/*--------------------------------------------------
 * A relation name is an identifier.
 * If fails, returns an empty string.
 *--------------------------------------------------*/
string Evaluator::evalRelationName(string token) {
	return evalIdentifier(token);
}

/*--------------------------------------------------
 * An attribute name is an identifier.
 * If fails, returns an empty string.
 *--------------------------------------------------*/
string Evaluator::evalAttributeName(string token) {
	return evalIdentifier(token);
}

/*--------------------------------------------------
 * An identifier must start with an alpha and can
 * be followed by other alphanumeric characters or _.
 * If fails, returns an empty string.
 *--------------------------------------------------*/
string Evaluator::evalIdentifier(string token) {
	string emptyTok;
	
	if(isalpha(token[0])) {
		for(int i = 1; i < token.length(); i++) {
			if(!(isalnum(token[i])) && !(token[i] == '_')) {
				string c(1, token[i]);
				printInvalidError("identifier", "alphanum or _", c);
				return emptyTok;
			}
		}
		return token;
	} else {
		string c(1, token[0]);
		printInvalidError("identifier", "alpha", c);
		return emptyTok;
	}
}

/*--------------------------------------------------
 * An integer is any of the natural numbers.
 * If fails, returns an empty string.
 *--------------------------------------------------*/
string Evaluator::evalInteger(string token) {	
	string emptyTok;
	
	for(int i = 0; i < token.length(); i++) {
		if(!(isdigit(token[i]))) {
			string c(1, token[i]);
			printInvalidError("integer", "digit", c);
			return emptyTok;
		}
	}
	return token;
}

/*--------------------------------------------------
 * A varchar is a string with a capacity of the
 * format VARCHAR(capacity).
 * If fails, returns an empty vector of strings.
 *--------------------------------------------------*/
vector<string> Evaluator::evalVarchar() {
	vector<string> varchar;
	
	string varcharCheck = tokens[cur];
	
	// Check if the current token reads VARCHAR
	if (varcharCheck.compare("VARCHAR") == 0) {
		varchar.push_back(varcharCheck);
		
		// Check if pointer can move
		if (isEndOfLine()) {
			printIncompleteError("varchar", "Missing (");
			varchar.clear();
			return varchar;
		} else {
			// Move pointer to next token: (
			cur++;
			
			// If it's a '(', push
			string parenthesisCheck = tokens[cur];
			if (parenthesisCheck.compare("(") == 0) {
				varchar.push_back(parenthesisCheck);
				
				// Check if pointer can move
				if (isEndOfLine()) {
					printIncompleteError("varchar", "Missing integer");
					varchar.clear();
					return varchar;
				} else {
					// Move pointer to next token: integer
					cur++;
					string integer = evalInteger(tokens[cur]);
					
					if (integer.empty()) {
						varchar.clear();
						return varchar;
					} else {
						varchar.push_back(integer);
						
						// Check if pointer can move
						if(isEndOfLine()) {
							printIncompleteError("varchar", "Missing )");
							varchar.clear();
							return varchar;
						} else {
							// Move pointer to next token: )
							cur++;
							parenthesisCheck = tokens[cur];

							if (parenthesisCheck.compare(")") == 0) {
								varchar.push_back(parenthesisCheck);
								return varchar;
							} else {
								printInvalidError("varchar", ")", parenthesisCheck);
								varchar.clear();
								return varchar;
							}
						}
					}
				}
			} else {
				printInvalidError("varchar", "(", parenthesisCheck);
				varchar.clear();
				return varchar;
			}
		}
	} else {
		printInvalidError("varchar", "VARCHAR", varcharCheck);
		return varchar;
	}
}

/*--------------------------------------------------
 * A type can be a VARCHAR or integer.
 * If fails, returns an empty vector of strings.
 *--------------------------------------------------*/
vector<string> Evaluator::evalType() {
	vector<string>type;

	string typeName = tokens[cur];

	// Check if it's integer
	if(typeName.compare("INTEGER") == 0) {
		type.push_back("INTEGER");
		return type;
	} else {
		type = evalVarchar();
		return type;
	}
}

/*--------------------------------------------------
 * An attribute list is one or more attribute names.
 * If fails, returns an empty list.
 *--------------------------------------------------*/
vector<string> Evaluator::evalAttributeList() {
	vector<string>list;
	
	string nextToken;

	do {
		// First it reads an attribute name
		string name = evalAttributeName(tokens[cur]);

		// If the name's invalid, print error message and return empty list
		if(name.empty()) return list;
		
		// Now verify if there's more attributes to read
		// If it's the last attribute, return the list
		if(isEndOfLine()) {
//		if () {
			list.push_back(name);
			return list;
		} else {
			// Move the pointer to the next token: ,
			cur++;
			nextToken = tokens[cur];
			if (nextToken.compare(")") == 0) {
				cur--;
				list.push_back(name);
				return list;
			}
			if(nextToken.compare(",") == 0) {
				list.push_back(name);

				if(isEndOfLine()) { 
					printIncompleteError("attribute list", "Missing attribute-name");
					list.clear();
					return list;
				} else {
					// Move the pointer to the next token: attribute-name
					cur++;
				}
			} else {
				printInvalidError("attribute list", ",", nextToken);
				list.clear();
				return list;
			}
		}
	} while(nextToken.compare(",") == 0);
}

/*--------------------------------------------------
 * A typed attribute list is a collection of attribute 
 * names followed by types.
 * If fails, returns an empty list.
 *--------------------------------------------------*/
vector<Attribute> Evaluator::evalTypedAttributeList() {
	vector<Attribute> list;
	string nextToken;

	do {
		// First it reads an attribute name
		string name = evalAttributeName(tokens[cur]);

		// If the name's invalid, print error message and return empty list
		if(name.empty()) return list;
		
		// Check if pointer can move
		if(isEndOfLine()) {
			printIncompleteError("typed attribute list", "Missing type");
			return list;
		} else {
			// Move pointer to next token: type
			cur++;
			vector<string> type = evalType();

			// Verify that type is valid
			if(type.empty()) return list;

			// Verify which type
			else {
				if(type[0].compare("INTEGER") == 0) {
					Attribute attr(name, Datatype::INTEGER);
					list.push_back(attr);
				} else {
					// Get capacity before instantiating
					// VARCHAR(capacity)
					int capacity = stoi(type[2]);
					Attribute attr(name, Datatype::VARCHAR, capacity);
					list.push_back(attr);
				}
			}

			// Now verify if there's more attributes to read
			// If it's the last attribute, return the list
			if(isEndOfLine()) { 
				return list;
			} else {
				// Move the pointer to the next token: ,
				cur++;
				nextToken = tokens[cur];

				if(nextToken.compare(",") == 0) {
					if(isEndOfLine()) { 
						cur--;
						return list;
					} else {
						// Move the pointer to the next token: attribute-name
						cur++;
					}
				} else return list;
			}
		}
	} while(nextToken.compare(",") == 0);
}


