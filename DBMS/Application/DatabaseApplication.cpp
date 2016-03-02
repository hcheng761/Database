#include <iostream>
#include "DatabaseApplication.h"

DatabaseApplication::DatabaseApplication()
{
	parselog.open("../Database/Command_Log", fstream::app);
	uselog.open("../Database/Input_Log", fstream::app);
	parse.scan("../Database/users.db");
	parse.scan("../Database/boards.db");
	parse.scan("../Database/groups.db");
}

void DatabaseApplication::run()
{
	banner();
	
	enterFourum();
}

void DatabaseApplication::banner()
{
	cout << "*****************************************" << endl;
	cout << "** Welcome to the Team Awesome Fourums **" << endl;
	cout << "**                                     **" << endl;
	cout << "**           enjoy your stay           **" << endl;
	cout << "*****************************************" << endl;
	cout << endl;
}

void DatabaseApplication::enterFourum()
{
	cout << "** Beware ye who enters here **"	<< endl;
	cout										<< endl;
	cout << "Choose Wisely"						<< endl;
	cout << "1 = Login"							<< endl;
	cout << "2 = Create Account"				<< endl;
	cout << "3 = Exit, cause I'm a quitter"		<< endl;
	cout										<< endl;
	cout << "Enter Choice: "						;
	
	string choice = "";
	cin >> choice;
	cin.clear();
	uselog << "enterFourum() choice: " << choice << endl;
	cout << endl;

	if (choice == "1")
	{
		login();
	}
	else if (choice == "2")
	{
		cout << "A wise choice" << endl << endl;
		createAccount();
	}
	else if (choice == "3")
	{
		cout << "L8r H8r :P" << endl;
		return;
	}
	else
	{
		cout << "You have chosen... poorly" << endl << endl;
		enterFourum();
	}
	return;
}

void DatabaseApplication::login()
{
	string temp_user;
	string temp_pass;

	cout << "** Lets see who you are **"	<< endl;
	cout									<< endl;
	cout << "Enter Username: "				;
	cin >> temp_user;
	cin.clear();
	cout << "Enter Password: "				;
	cin >> temp_pass;
	cin.clear();

	uselog << "login() username: " << temp_user << " password: " << temp_pass << endl;

	Engine tester = parse.getEngine();
	int i;
	for (i = 0; i < tester.getRelation("users").getHeight(); i++)
	{
		if (tester.getRelation("users").getTuple(i)->getAttribute(0)->getName() == temp_user)
		{
			break;
		}
	}
	if (i != tester.getRelation("users").getHeight())
	{
		if (tester.getRelation("users").getTuple(i)->getAttribute(1)->getName() == temp_pass)
		{
			activeUser = temp_user;
			cout << endl;
			mainMenu();
		}
		else
		{
			cout << endl << "Sorry, that user name and password combination does not exist" << endl;
			enterFourum();
		}
	}
	else
	{
		cout << endl << "Sorry, that user name and password combination does not exist" << endl;
		enterFourum();
	}
	
}

void DatabaseApplication::createAccount()
{
	string temp_user;
	string temp_pass;

	cout << "** Howdy new friend **" << endl;
	cout << endl;
	cout << "Enter Username: ";
	cin >> temp_user;
	cin.clear();
	cout << "Enter Password: ";
	cin >> temp_pass;
	cin.clear();

	for (int j = 1; j < parse.getEngine().getRelation("users").getHeight(); j++)
	{
		if (parse.getEngine().getRelation("users").getTuple(j)->getAttribute(0)->getName() == temp_user)
		{
			cout << "Sorry, that username is taken\n";
			enterFourum();
			return;
		}
	}

	uselog << "createAccount() username: " << temp_user << " password: " << temp_pass << endl;
	
	string parseCommand = "INSERT INTO users VALUES FROM (\"" + temp_user + "\", \"" + temp_pass + "\");";
	parselog << parseCommand << endl;
	parse.evaluate(parse.tokenize(parseCommand));
	parseCommand = "SAVE users;";
	parselog << parseCommand << endl;
	parse.evaluate(parse.tokenize(parseCommand));

	activeUser = temp_user;
	
	parseCommand = "CREATE TABLE " + activeUser + "messages (username VARCHAR(20), messages VARCHAR(3000)) PRIMARY KEY(username, messages);";
	parselog << parseCommand << endl;
	parse.evaluate(parse.tokenize(parseCommand));
	parseCommand = "SAVE " + activeUser + "messages;";
	parselog << parseCommand << endl;
	parse.evaluate(parse.tokenize(parseCommand));
	
	cout << endl;
	mainMenu();
}

void DatabaseApplication::mainMenu()
{
	cout << "** Welcome to your new home **"	<< endl;
	cout										<< endl;
	cout << "Active User: " << activeUser		<< endl;
	cout << "1 = Account Maintainance"			<< endl;
	cout << "2 = Boards"						<< endl;
	cout << "3 = IMs"							<< endl;
	cout << "4 = Groups"						<< endl;
	cout << "L = Logout"						<< endl;
	if (activeUser == "Admin")
	{
		cout << "C = Command Line" << endl;
	}
	cout										<< endl;
	cout << "Enter Choice: "					;

	string choice = "";
	cin >> choice;
	cin.clear();
	cout << endl;

	uselog << "mainMenu() choice: " << choice << endl;
	if (activeUser == "Admin" && (choice == "c" || choice == "C"))
	{
		cout << "Enter QUIT to leave the shell" << endl;
		cin.ignore();
		shell();
		return;
	}
	if (choice == "1")
	{
		accountMaintainance();
	}
	else if (choice == "2")
	{
		boards();		
	}
	else if (choice == "3")
	{
		instants();
	}
	else if (choice == "4")
	{
		groups();
	}
	else if (choice == "L" || choice == "l")
	{
		cout << "Bye bye" << endl;
		return;
	}
	else
	{
		cout << "You have chosen... poorly" << endl << endl;
		mainMenu();
	}
	return;
}

void DatabaseApplication::shell()			// Noemie/David log in with "Admin" and "1a" then start testing your commands
{
	cout << "Database > ";
	command = "";
	getline(cin, command);
	parselog << command << endl;
	if (command == "QUIT")
	{
		mainMenu();
		return;
	}
	parse.evaluate(parse.tokenize(command));
	shell();
}

void DatabaseApplication::accountMaintainance()
{
	cout << "** Account Maintainance **"	<< endl;
	cout									<< endl;
	cout << "1 = Change Password"			<< endl;
	if (activeUser != "Admin")
	{
		cout << "2 = Delete Account" << endl;
	}
	cout << "M = Return to Main Menu"		<< endl;
	cout << "L = Logout"					<< endl;
	cout									<< endl;
	cout << "Enter Choice: ";

	string choice = "";
	cin >> choice;
	cin.clear();
	cout << endl;

	uselog << "accountMaintanance() choice: " << choice << endl;

	if (choice == "1")
	{
		string newPass;
		string confirmPass;
		do
		{
			cout << "Please enter your new password, or x to exit: ";
			cin >> newPass;
			cin.clear();
			if (newPass == "x" || newPass == "X") break;
			cout << "Please renter your new password: ";
			cin >> confirmPass;
			cin.clear();
			uselog << "accountMaintainance() password change newPass: " << newPass << " confirmPass: " << confirmPass << endl;
			if (newPass != confirmPass)
			{
				cout << "Please enter matching passwords\n";
			}
		} while (newPass != confirmPass);
		if (newPass == "x" || newPass == "X")
				accountMaintainance();
		else
		{
			parse.engine.update("users", { "password" }, { newPass }, Condition("username", "==", activeUser));
			string parseCommand = "UPDATE users SET password = \"" + newPass + "\" WHERE username == \"" + activeUser + "\";";
			parselog << parseCommand << endl;
			//parse.evaluate(parse.tokenize(parseCommand));
			parseCommand = "SAVE users;";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			cout << "\nPassword Changed!\n\n";
			accountMaintainance();
		}
	}
	else if (choice == "2" && activeUser != "Admin")
	{
		do
		{
			cout << "Are you sure you want to delete your account?" << endl;
			cout << "1 = Yes" << endl;
			cout << "2 = No! Please let me stay!" << endl;
			cin >> choice;
			cin.clear();
			uselog << "accountMaintanance() delete account choice: " << choice << endl;
			if (choice != "1" && choice != "2")
			{
				cout << "Choice not recognized\n";
			}
		} while (choice != "1" && choice != "2");
		if (choice == "1")
		{
			parse.engine.deleteFrom("users", Condition("username", "==", activeUser));
			string parseCommand = "DELETE FROM users WHERE username == " + activeUser + ";";
			parselog << parseCommand << endl;
	//		parse.evaluate(parse.tokenize(parseCommand));
			parseCommand = "SAVE users;";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			cout << "Sad to see you go :'(\n";
			return;
		}
		else
		{
			cout << "Your wish is my command\n\n";
			accountMaintainance();
		}
	}
	else if (choice == "M" || choice == "m")
	{
		mainMenu();
	}
	else if (choice == "L" || choice == "l")
	{
		cout << "Bye bye" << endl;
		return;
	}
	else
	{
		cout << "You have chosen... poorly" << endl << endl;
		accountMaintainance();
	}
	return;
}

/*
	All of the following functions behave vary similarly, the only difference is the database being accessed
*/

void DatabaseApplication::boards()
{
	// This one is more technical. Need to know the raw commands for retrieving boards
	cout << "** Available Boards **" << endl;
	cout << "    Board Name\tActive Topics" << endl;
	int i = 0;
	for (i = 1; i < parse.getEngine().getRelation("boards").getHeight(); i++)
	{
		cout << i << " = " << parse.getEngine().getRelation("boards").getTuple(i)->getAttribute(0)->getName() << "\t" << parse.getEngine().getRelation("boards").getTuple(i)->getAttribute(1)->getName() << endl;
	}
	cout << endl << "**Other Options**" << endl;
	if (activeUser == "Admin")
	{
		cout << "A = Add Board" << endl;
		cout << "D = Delete Board" << endl;
	}
	cout << "M = Main Menu" << endl;
	cout << "L = Logout" << endl;
	cout << endl;
	cout << "Enter Choice: ";
	string choice;
	cin >> choice;
	uselog << "boards() choice: " << choice << endl;
	cin.clear();
	string op;
	if (activeUser == "Admin")
	{
		if (choice == "a" || choice == "A")
		{
			cin.ignore();
			cout << endl << "Enter the name of the new board: ";
			choice.clear();
			getline(cin, choice);
			uselog << "boards() addboard " << choice << endl;
			string parseCommand = "INSERT INTO boards VALUES FROM (\"" + choice + "\", 0);";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			parseCommand = "SAVE boards;";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			cout << "Board Added!\n\n";
			parseCommand = "CREATE TABLE b" + choice + " (topic VARCHAR(20), replies INTEGER) PRIMARY KEY(topic, replies);";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			parseCommand = "SAVE b" + choice + ";";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			boards();
			return;
		}
		if (choice == "D" || choice == "d")
		{
			cout << "    Board Name\tActive Topics" << endl;
			for (i = 1; i < parse.getEngine().getRelation("boards").getHeight(); i++)
			{
				cout << i << " = " << parse.getEngine().getRelation("boards").getTuple(i)->getAttribute(0)->getName() << "\t" << parse.getEngine().getRelation("boards").getTuple(i)->getAttribute(1)->getName() << endl;
			}
			cout << "Enter Delete Choice: ";
			string choice;
			cin >> choice;
			for (i = 1; i < parse.getEngine().getRelation("boards").getHeight(); i++)
			{
				op = to_string(i);
				if (choice == op)
				{
					activeBoard = parse.getEngine().getRelation("boards").getTuple(i)->getAttribute(0)->getName();
					break;
				}
			}
			uselog << "boards() delete board choice: " << choice << endl;
			cin.clear();
			do
			{
				cout << "Are you sure you want to delete this board?" << endl;
				cout << "1 = Yes" << endl;
				cout << "2 = No" << endl;
				cin >> choice;
				cin.clear();
				uselog << "boards() delete board choice: " << choice << endl;
				if (choice != "1" && choice != "2")
				{
					cout << "Choice not recognized\n";
				}
			} while (choice != "1" && choice != "2");
			if (choice == "1")
			{
				parse.engine.deleteFrom("boards", Condition("name", "==", activeBoard));
				string parseCommand = "DELETE FROM boards WHERE name == " + activeBoard + ";";
				parselog << parseCommand << endl;
				//		parse.evaluate(parse.tokenize(parseCommand));
				parseCommand = "SAVE boards;";
				parselog << parseCommand << endl;
				parse.evaluate(parse.tokenize(parseCommand));
				cout << "Bye Board!\n";
				boards();
				return;
			}
			else
			{
				cout << "Delete Canceled\n\n";
				boards();
				return;
			}
		}
	}
	for (i = 1; i < parse.getEngine().getRelation("boards").getHeight(); i++)
	{
		op = to_string(i);
		if (choice == op)
		{
			activeBoard = parse.getEngine().getRelation("boards").getTuple(i)->getAttribute(0)->getName();
			viewBoard();
			return;
		}
	}
	if (choice == "M" || choice == "m")
	{
		mainMenu();
		return;
	}
	else if (choice == "L" || choice == "l")
	{
		cout << "Bye Bye";
		return;
	}
	else
	{
		cout << "You have chosen... poorly" << endl << endl;
		boards();
	}
	return;
}

void DatabaseApplication::instants()
{
	if (parse.engine.getRelation(activeUser + "messages").getName() != (activeUser + "messages"))
		parse.scan("../Database/" + activeUser + "messages.db");
	// This one is more technical. Need to know the raw commands for retrieving boards
	cout << "** Available Messages **" << endl;
	cout << "User Name\t\t Message" << endl;
	int i = 0;
	for (i = 2; i < parse.getEngine().getRelation(activeUser + "messages").getHeight(); i++)
	{
		cout << parse.getEngine().getRelation(activeUser + "messages").getTuple(i)->getAttribute(0)->getName() << "\t" << parse.getEngine().getRelation(activeUser + "messages").getTuple(i)->getAttribute(1)->getName() << endl;
	}
	cout << endl << "** Options **" << endl;
	cout << "N = New Message" << endl;
	cout << "M = Main Menu" << endl;
	cout << "L = Logout" << endl;
	cout << endl;
	cout << "Enter Choice: ";
	string choice;
	cin >> choice;
	uselog << "instants() choice: " << choice << endl;
	cin.clear();
	string op;
	if (choice == "n" || choice == "N")
	{
		cin.ignore();
		cout << endl << "Enter the username of the message recipient: ";
		choice.clear();
		cin >> choice;
		string recip = choice;
		uselog << "instants() new IM " << choice << endl;
		int j = 0;
		for (j = 1; j < parse.getEngine().getRelation("users").getHeight(); j++)
		{
			if (parse.getEngine().getRelation("users").getTuple(j)->getAttribute(0)->getName() == recip)
			{
				break;
			}
		}
		if (j < parse.getEngine().getRelation("users").getHeight())
		{
			if (parse.engine.getRelation(recip + "messages").getName() != (recip + "messages"))
				parse.scan("../Database/" + recip + "messages.db");
			cout << "What would you like to say? Press enter to send:\n";
			cin.ignore();
			cin.clear();
			getline(cin, choice);
			cin.clear();
			string parseCommand = "INSERT INTO " + activeUser + "messages VALUES FROM (\"" + recip + "\", \"" + choice + "\");";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			parseCommand = "SAVE " + activeUser + "messages;";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));

			parseCommand = "INSERT INTO " + recip + "messages VALUES FROM (\"" + activeUser + "\", \"" + choice + "\");";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			parseCommand = "SAVE " + recip + "messages;";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));

			cout << "Message Sent!\n\n";
			instants();
			return;
		}
		else
		{
			cout << "That user does not exist\n\n";
			instants();
			return;
		}
	}
	if (choice == "M" || choice == "m")
	{
		mainMenu();
		return;
	}
	else if (choice == "L" || choice == "l")
	{
		cout << "Bye Bye";
		return;
	}
	else
	{
		cout << "You have chosen... poorly" << endl << endl;
		instants();
	}
	return;
	
}

void DatabaseApplication::groups()
{
	// This one is more technical. Need to know the raw commands for retrieving groups
	cout << "** Available Groups **" << endl;
	cout << "    Group Name\tActive Users" << endl;
	int i = 0;
	for (i = 1; i < parse.getEngine().getRelation("groups").getHeight(); i++)
	{
		cout << i << " = " << parse.getEngine().getRelation("groups").getTuple(i)->getAttribute(0)->getName() << "\t" << parse.getEngine().getRelation("groups").getTuple(i)->getAttribute(1)->getName() << endl;
	}
	cout << endl << "**Other Options**" << endl;
	if (activeUser == "Admin")
	{
		cout << "A = Add Group" << endl;
		cout << "D = Delete Group" << endl;
	}
	cout << "M = Main Menu" << endl;
	cout << "L = Logout" << endl;
	cout << endl;
	cout << "Enter Choice: ";
	string choice;
	cin >> choice;
	uselog << "groups() choice: " << choice << endl;
	cin.clear();
	string op;
	if (activeUser == "Admin")
	{
		if (choice == "a" || choice == "A")
		{
			cin.ignore();
			cout << endl << "Enter the name of the new group: ";
			choice.clear();
			getline(cin, choice);
			uselog << "groups() addgroup " << choice << endl;
			string parseCommand = "INSERT INTO groups VALUES FROM (\"" + choice + "\", 0);";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			parseCommand = "SAVE groups;";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			cout << "Group Added!\n\n";
			parseCommand = "CREATE TABLE g" + choice + " (members VARCHAR(20)) PRIMARY KEY(members);";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			parseCommand = "SAVE g" + choice + ";";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			groups();
			return;
		}
		if (choice == "D" || choice == "d")
		{
			cout << "    Group Name\tActive Users" << endl;
			for (i = 1; i < parse.getEngine().getRelation("groups").getHeight(); i++)
			{
				cout << i << " = " << parse.getEngine().getRelation("groups").getTuple(i)->getAttribute(0)->getName() << "\t" << parse.getEngine().getRelation("groups").getTuple(i)->getAttribute(1)->getName() << endl;
			}
			cout << "Enter Delete Choice: ";
			string choice;
			cin >> choice;
			for (i = 1; i < parse.getEngine().getRelation("groups").getHeight(); i++)
			{
				op = to_string(i);
				if (choice == op)
				{
					activeGroup = parse.getEngine().getRelation("groups").getTuple(i)->getAttribute(0)->getName();
					break;
				}
			}
			uselog << "groups() delete group choice: " << choice << endl;
			cin.clear();
			do
			{
				cout << "Are you sure you want to delete this group?" << endl;
				cout << "1 = Yes" << endl;
				cout << "2 = No" << endl;
				cin >> choice;
				cin.clear();
				uselog << "groups() delete group choice: " << choice << endl;
				if (choice != "1" && choice != "2")
				{
					cout << "Choice not recognized\n";
				}
			} while (choice != "1" && choice != "2");
			if (choice == "1")
			{
				parse.engine.deleteFrom("groups", Condition("name", "==", activeGroup));
				string parseCommand = "DELETE FROM groups WHERE name == " + activeGroup + ";";
				parselog << parseCommand << endl;
				//		parse.evaluate(parse.tokenize(parseCommand));
				parseCommand = "SAVE groups;";
				parselog << parseCommand << endl;
				parse.evaluate(parse.tokenize(parseCommand));
				cout << "Bye Group!\n";
				groups();
				return;
			}
			else
			{
				cout << "Delete Canceled\n\n";
				groups();
				return;
			}
		}
	}
	for (i = 1; i < parse.getEngine().getRelation("groups").getHeight(); i++)
	{
		op = to_string(i);
		if (choice == op)
		{
			activeGroup = parse.getEngine().getRelation("groups").getTuple(i)->getAttribute(0)->getName();
			viewGroup();
			return;
		}
	}
	if (choice == "M" || choice == "m")
	{
		mainMenu();
		return;
	}
	else if (choice == "L" || choice == "l")
	{
		cout << "Bye Bye";
		return;
	}
	else
	{
		cout << "You have chosen... poorly" << endl << endl;
		groups();
	}
	return;
}

void DatabaseApplication::viewGroup()
{
	if (parse.engine.getRelation("g" + activeGroup).getName() != ("g" + activeGroup))
		parse.scan("../Database/g" + activeGroup + ".db");
	cout << "** " << activeGroup << " **" << endl;
	cout << "    Members" << endl;
	int i = 0;
	for (i = 1; i < parse.getEngine().getRelation("g" + activeGroup).getHeight(); i++)
	{
		cout << parse.getEngine().getRelation("g" + activeGroup).getTuple(i)->getAttribute(0)->getName() << endl;
	}
	cout << endl << "**Other Options**" << endl;
	bool isMember = false;
	for (int j = 1; j < parse.getEngine().getRelation("g"+activeGroup).getHeight(); j++)
	{
		if (parse.getEngine().getRelation("g"+activeGroup).getTuple(j)->getAttribute(0)->getName() == activeUser)
		{
			isMember = true;
		}
	}
	if (!isMember)
		cout << "J = Join Group" << endl;
	cout << "G = Groups" << endl;
	cout << "Q = Quit Group" << endl;
	cout << "M = Main Menu" << endl;
	cout << "L = Logout" << endl;
	cout << endl;
	cout << "Enter Choice: ";
	string choice;
	cin >> choice;
	uselog << "viewGroup() choice: " << choice << endl;
	cin.clear();
	string op;
	for (i = 1; i < parse.getEngine().getRelation("g" + activeGroup).getHeight(); i++)
	{
		op = to_string(i);
		if (choice == op)
		{
			activeGroup = parse.getEngine().getRelation("g" + activeGroup).getTuple(i)->getAttribute(0)->getName();
			viewGroup();
			return;
		}
	}
	if (choice == "Q" || choice == "q")
	{
		do
		{
			cout << "Are you sure you want to leave this group?" << endl;
			cout << "1 = Yes" << endl;
			cout << "2 = No" << endl;
			cin >> choice;
			cin.clear();
			uselog << "viewGroup() leave group choice: " << choice << endl;
			if (choice != "1" && choice != "2")
			{
				cout << "Choice not recognized\n";
			}
		} while (choice != "1" && choice != "2");
		if (choice == "1")
		{
			parse.engine.deleteFrom("g" + activeGroup, Condition("members", "==", activeUser));
			string parseCommand = "DELETE FROM g" + activeGroup + " WHERE members == " + activeUser + ";";
			parselog << parseCommand << endl;
			cout << endl;
			cout << parseCommand << endl;
			cout << endl;
			//		parse.evaluate(parse.tokenize(parseCommand));
			parseCommand = "SAVE g" + activeGroup + ";";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			cout << "Bye!\n";
			int j = 0;
			for (j = 1; j < parse.getEngine().getRelation("groups").getHeight(); j++)
			{
				if (parse.getEngine().getRelation("groups").getTuple(j)->getAttribute(0)->getName() == activeGroup)
				{
					break;
				}
			}
			parse.engine.update("groups", { "members" }, { to_string(stoi(parse.getEngine().getRelation("groups").getTuple(j)->getAttribute(1)->getName()) - 1) }, Condition("name", "==", activeGroup));
			parseCommand = "SAVE groups;";
			parselog << parseCommand << endl;
			parse.evaluate(parse.tokenize(parseCommand));
			groups();
			return;
		}
		else
		{
			cout << "Delete Canceled\n\n";
			groups();
			return;
			}
	}
	if (choice == "j" || choice == "J")
	{
		uselog << "viewGroup() add user " << activeUser << endl;
		string parseCommand = "INSERT INTO g" + activeGroup + " VALUES FROM (\"" + activeUser + "\");";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));
		parseCommand = "SAVE g" + activeGroup + ";";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));
		cout << "Group Joined!\n\n";

		int j = 0;
		for (j = 1; j < parse.getEngine().getRelation("groups").getHeight(); j++)
		{
			if (parse.getEngine().getRelation("groups").getTuple(j)->getAttribute(0)->getName() == activeGroup)
			{
				break;
			}
		}
		parse.engine.update("groups", { "members" }, { to_string(stoi(parse.getEngine().getRelation("groups").getTuple(j)->getAttribute(1)->getName()) + 1) }, Condition("name", "==", activeGroup));
		parseCommand = "SAVE groups;";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));
		viewGroup();
		return;
	}
	if (choice == "g" || choice == "G")
	{
		groups();
		return;
	}
	else if (choice == "m" || choice == "M")
	{
		mainMenu();
		return;
	}
	else if (choice == "l" || choice == "L")
	{
		return;
	}
	else
	{
		cout << "You have chosen... poorly" << endl << endl;
		viewGroup();
	}
	return;
}


void DatabaseApplication::viewBoard()
{
	if (parse.engine.getRelation("b" + activeBoard).getName() != ("b" + activeBoard))
		parse.scan("../Database/b"+activeBoard+".db");
	cout << "** " << activeBoard << " **" << endl;
	cout << "    Topic\tReplies" << endl;
	int i = 0;
	for (i = 1; i < parse.getEngine().getRelation("b"+activeBoard).getHeight(); i++)
	{
		cout << i << " = " << parse.getEngine().getRelation("b" + activeBoard).getTuple(i)->getAttribute(0)->getName() << "\t\t" << parse.getEngine().getRelation("b" + activeBoard).getTuple(i)->getAttribute(1)->getName() << endl;
	}
	cout << endl << "**Other Options**" << endl;
	cout << "A = Add Topic" << endl;
	cout << "B = Boards" << endl;
	if (activeUser == "Admin")
	{
		cout << "D = Delete Topic" << endl;
	}
	cout << "M = Main Menu" << endl;
	cout << "L = Logout" << endl;
	cout << endl;
	cout << "Enter Choice: ";
	string choice;
	cin >> choice;
	uselog << "viewBoard() choice: " << choice << endl;
	cin.clear();
	string op;
	for (i = 1; i < parse.getEngine().getRelation("b" + activeBoard).getHeight(); i++)
	{
		op = to_string(i);
		if (choice == op)
		{
			activeTopic = parse.getEngine().getRelation("b" + activeBoard).getTuple(i)->getAttribute(0)->getName();
			viewTopic();
			return;
		}
	}
	if (activeUser == "Admin")
	{
		if (choice == "D" || choice == "d")
		{
			cout << "    Topic\tReplies" << endl;
			for (i = 1; i < parse.getEngine().getRelation("b" + activeBoard).getHeight(); i++)
			{
				cout << i << " = " << parse.getEngine().getRelation("b" + activeBoard).getTuple(i)->getAttribute(0)->getName() << "\t" << parse.getEngine().getRelation("b" + activeBoard).getTuple(i)->getAttribute(1)->getName() << endl;
			}
			cout << "Enter Delete Choice: ";
			string choice;
			cin >> choice;
			for (i = 1; i < parse.getEngine().getRelation("b" + activeBoard).getHeight(); i++)
			{
				op = to_string(i);
				if (choice == op)
				{
					activeTopic = parse.getEngine().getRelation("b" + activeBoard).getTuple(i)->getAttribute(0)->getName();
					break;
				}
			}
			uselog << "viewBoard() delete topic choice: " << choice << endl;
			cin.clear();
			do
			{
				cout << "Are you sure you want to delete this topic?" << endl;
				cout << "1 = Yes" << endl;
				cout << "2 = No" << endl;
				cin >> choice;
				cin.clear();
				uselog << "viewBoard() delete topic choice: " << choice << endl;
				if (choice != "1" && choice != "2")
				{
					cout << "Choice not recognized\n";
				}
			} while (choice != "1" && choice != "2");
			if (choice == "1")
			{
				parse.engine.deleteFrom("b" + activeBoard, Condition("topic", "==", activeTopic));
				string parseCommand = "DELETE FROM b" + activeBoard + " WHERE name == " + activeTopic + ";";
				parselog << parseCommand << endl;
				//		parse.evaluate(parse.tokenize(parseCommand));
				parseCommand = "SAVE b" + activeBoard + ";";
				parselog << parseCommand << endl;
				parse.evaluate(parse.tokenize(parseCommand));
				cout << "Bye Topic!\n";
				int j = 0;
				for (j = 1; j < parse.getEngine().getRelation("boards").getHeight(); j++)
				{
					if (parse.getEngine().getRelation("boards").getTuple(j)->getAttribute(0)->getName() == activeBoard)
					{
						break;
					}
				}
				parse.engine.update("boards", { "topics" }, { to_string(stoi(parse.getEngine().getRelation("boards").getTuple(j)->getAttribute(1)->getName()) - 1) }, Condition("name", "==", activeBoard));
				parseCommand = "SAVE boards;";
				parselog << parseCommand << endl;
				parse.evaluate(parse.tokenize(parseCommand));
				boards();
				return;
			}
			else
			{
				cout << "Delete Canceled\n\n";
				boards();
				return;
			}
		}
	}
	if (choice == "a" || choice == "A")
	{
		cin.ignore();
		cout << endl << "Enter the name of the new topic (no spaces): ";
		choice.clear();
		getline(cin, choice);
		uselog << "viewBoard() add topic " << choice << endl;
		string parseCommand = "INSERT INTO b" + activeBoard + " VALUES FROM (\"" + choice + "\", 1);";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));
		parseCommand = "SAVE b" + activeBoard + ";";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));
		cout << "Topic Added!\n\n";
		parseCommand = "CREATE TABLE t" + choice + " (user VARCHAR(20), reply VARCHAR(3000)) PRIMARY KEY(user, reply);";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));

		activeTopic = choice;

		cout << endl << "What would you like to say about it? Press enter to submit:\n";
		choice.clear();
		getline(cin, choice);

		uselog << "viewBoard() " << activeUser << "says " << choice << endl;
		parseCommand = "INSERT INTO t" + activeTopic + " VALUES FROM (\"" + activeUser + "\", \"" + choice + "\");";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));

		parseCommand = "SAVE t" + activeTopic + ";";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));

		int j = 0;
		for (j = 1; j < parse.getEngine().getRelation("boards").getHeight(); j++)
		{
			if (parse.getEngine().getRelation("boards").getTuple(j)->getAttribute(0)->getName() == activeBoard)
			{
				break;
			}
		}
		parse.engine.update("boards", { "topics" }, { to_string(stoi(parse.getEngine().getRelation("boards").getTuple(j)->getAttribute(1)->getName()) + 1) }, Condition("name", "==", activeBoard));
		parseCommand = "SAVE boards;";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));
		viewBoard();
		return;
	}
	if (choice == "b" || choice == "B")
	{
		boards();
		return;
	}
	else if (choice == "m" || choice == "M")
	{
		mainMenu();
		return;
	}
	else if (choice == "l" || choice == "L")
	{
		return;
	}
	else
	{
		cout << "You have chosen... poorly" << endl << endl;
		viewBoard();
	}
	return;
}

void DatabaseApplication::viewTopic()
{
	if (parse.engine.getRelation("t" + activeTopic).getName() != ("t" + activeTopic))
		parse.scan("../Database/t" + activeTopic + ".db");
	
	cout << "** " << activeTopic << " **" << endl;
	cout << "User\t\tSays" << endl;
	int i = 0;
	for (i = 1; i < parse.getEngine().getRelation("t" + activeTopic).getHeight(); i++)
	{
		cout << parse.getEngine().getRelation("t" + activeTopic).getTuple(i)->getAttribute(0)->getName() << "\t\t" << parse.getEngine().getRelation("t" + activeTopic).getTuple(i)->getAttribute(1)->getName() << endl;
	}

	cout << endl;
	cout << "1 = Reply" << endl;
	cout << "2 = Back" << endl;
	cout << "B = Boards" << endl;
	cout << "M = Main Menu" << endl;
	cout << "L - Logout" << endl;

	cout << "Enter Choice: ";
	string choice;
	cin >> choice;
	uselog << "viewTopic() choice: " << choice << endl;
	cin.clear();

	if (choice == "B" || choice == "b")
	{
		boards();
		return;
	}
	else if (choice == "M" || choice == "m")
	{
		mainMenu();
		return;
	}
	else if (choice == "L" || choice == "l")
	{
		cout << "Bye Bye";
		return;
	}
	else if (choice == "1")
	{
		cout << "What would you like to say? Press enter to submit:\n";
		cin.ignore();
		choice.clear();
		getline(cin, choice);
		string parseCommand = "INSERT INTO t" + activeTopic + " VALUES FROM(\"" + activeUser + "\", \"" + choice + "\" );";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));
		parseCommand = "SAVE t" + activeTopic + ";";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));
		int j = 0;
		for (j = 1; j < parse.getEngine().getRelation("b"+activeBoard).getHeight(); j++)
		{
			if (parse.getEngine().getRelation("b"+activeBoard).getTuple(j)->getAttribute(0)->getName() == activeTopic)
			{
				break;
			}
		}
		parse.engine.update("b"+activeBoard, { "replies" }, { to_string(stoi(parse.getEngine().getRelation("b"+activeBoard).getTuple(j)->getAttribute(1)->getName()) + 1) }, Condition("topic", "==", activeTopic));
		parseCommand = "SAVE b" + activeBoard + ";";
		parselog << parseCommand << endl;
		parse.evaluate(parse.tokenize(parseCommand));
		viewTopic();
	}
	else if (choice == "2")
	{
		viewBoard();
		return;
	}
	else
	{
		cout << "You have chosen... poorly" << endl << endl;
		viewTopic();
	}
	return;
}