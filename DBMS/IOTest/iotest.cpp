//This is the main function where the I/O handling for the DBMS goes
#include <iostream>
#include <string>
#include "../Parser/Parser.h"
#include "../Engine/Engine.h"
#include "../Application/DatabaseApplication.h"

using namespace std;

int main()
{
	DatabaseApplication app;
	app.run();
	cout << "\n";
	cout << "Please type any character and press enter to close this program.";
	string i;
	cin >> i;

    /*//to run the parser functions
    Parser parse;
    //to hold the filename from user input
    string filename="";
	string preface = "../Database/";
    //to hold the tokenized line from user input
    vector<string> tokens;
    //to hold the command from user input
    string user_command="";
    //monitors user input:    
    int user_input=0;
    while(user_input!=3)
    {
        cout<<"Welcome to the DBMS! Please choose an option:\n";
        //if the user wants to read from the command line:
        cout<<"1.Enter a command or query\n";
        //if the user wants to read from a file:
        cout<<"2.Enter a filename to be read from\n";
        //if the user is finished using the program:
        cout<<"3.Exit\n";
        //read in user input:
        cin>>user_input;
        //check to make sure user input is valid:
        while(cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout<<"Not a valid type. Please enter an integer:\n";
            cin>>user_input;
        }
        switch(user_input)
        {
            case 1:
            //read from the command line
                cout<<"Please enter your command or query:\n";
				cin >> user_command;
				tokens = parse.tokenize(user_command);
                parse.evaluate(tokens);
                break;
            case 2:
            //read from a file
                cout<<"Please enter the name of the file you want to read from:\n";
                cin>>filename;
				preface.append(filename);
				preface.append(".db");
                parse.scan(preface);
				preface = "../Database/";
                break;
            case 3:
            //exit
                cout<<"Goodbye!";
                break;
            default:
                cout<<"Invalid choice, please try again:\n";
                break;
        }
    }*/
}