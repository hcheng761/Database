#include "../Parser/Parser.h"
#include "../Engine/Engine.h"

using namespace std;

/*
	This is the database apllication base class. end users only have access to commands made available through this

	The app is built as a tiered system with no functions returning anything but void. a return statement is tantamount to quitting
*/

class DatabaseApplication{

public:
	DatabaseApplication();		//Constructor
	
	void run();					//Runs the application

	void banner();				//Displays the app banner
	void enterFourum();
	
	void login();				//log in as user
	void createAccount();		//Create an account

	void mainMenu();			//Displays the main menu

	void shell();				//Admin direct access to parser, allows command input

	void accountMaintainance();	//User account options
	void boards();				//Shows available boards
	void instants();			//Shows instant messages
	void groups();				//Shows groups the user is a part of

	void viewGroup();			//shows the users in the groups

	void viewBoard();			//View the topics in a board
	void viewTopic();			//View a simgle topic thread

private:
	string activeUser;
	string activeGroup;
	string activeBoard;
	string activeTopic;
	string command;
	Parser parse;
	fstream uselog;				//inputs from user with functoin that caused it
	fstream parselog;			//commands sent to parser
};