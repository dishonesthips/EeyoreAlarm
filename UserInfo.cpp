#include "UserInfo.hpp"
#include "Log.hpp"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

//UserInfo member function definitions
UserInfo::UserInfo(Log log){ //constructor

	name = "";
	email = "";
	logger = log;
}

void UserInfo::writeInfo(){//write info to a file with paramters info
	//declare strings to be written to file
	string nameFile;
	string emailFile;
	
	//get name
	cout << "\tWhat is your name? ";
	getline(cin, nameFile);
	while (UserInfo::checkName(nameFile)) {
		cout << "\tPlease enter a valid name: ";
		getline(cin, nameFile);
	}
	nameFile = capitalize(nameFile);

	//get email
	cout << "\tWhat is your email? ";
	getline(cin, emailFile);
	while (UserInfo::checkEmail(emailFile)) {
		cout << "\tPlease enter a valid email: ";
		getline(cin, emailFile);
	}
	//write to file
	ofstream outfile;
	outfile.open(filename);
	outfile << nameFile << "," << emailFile << "\r\n";
	outfile.close();
}

void UserInfo::readInfo(){//read info from a file and sets values of object UserInfo accordingly

	//declare variable
	string line;
	
	//reset string name and email to empty string since they will be appended to
	name = "";
	email = "";
	
	//read line from text file
	ifstream infile;
	infile.open(filename);
	getline(infile, line);
	infile.close();
	
	//loop through individual characters of the line, appending to name and email accordingly
	int i = 0;
	while (line[i] != ',') {
		name += line[i];
		i++;
	}
	i++;
	while (line[i]) {
		email += line[i];
		i++;
	}
}

bool UserInfo::fileNotExist(){//check to see if text file already exists
	ifstream infile;
	infile.open(filename);
	if(infile.is_open()) {
		infile.close();
		return false;	//file exists
	}
	infile.close();
	return true; //file does not exist (yet) :smirk:
}

int UserInfo::checkName(const string input){//static error check valid name
	//check for empty string
	if (input.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -2;
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -2;
		}
	}	
	//valid name
	return 0;
}

int UserInfo::checkEmail(const string input){//static error check valid email
	//check for empty string
	if (input.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -3;
	}
	
	int count = 0;

	//error checking for email name validity
	if (input[count] == '.') {
		cerr << "Error: Invalid dot usage" << endl;
		return -3;	//email cannot begin with a '.'
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < input.length(); i++) {
		if (input[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -2;
		}
	}
	
	//first find an '@'; if cannot find within length of email characters, error
	//the local-part of an email can only include certain characters; if invalid character, error
	while (input[count] != '@') {
		if (count == input.length()-1) {
			cerr << "Error: Invalid email. Cannot find '@'" << endl;
			return -3;
		}
		if ((input[count] != '!' && (input[count] < '#' || input[count] > 39) && 
		input[count] != '*' && input[count] != '+' && (input[count] < '-' || input[count] > '/') && 
		input[count] != '=' && input[count] != '?' && (input[count] < '^' || input[count] > '~') &&
		(input[count] > '{' || input[count] < '~')) && (input[count] < 'A' || input[count] > 'Z') &&
		(input[count] < '0' || input[count] > '9') && input[count]) {
			cerr << "Error: Invalid character" << endl;
			return -3;
		}
		if (input[count] == '.' && (input[count - 1] == '.' || input[count + 1] == '.')) {			//cannot have two dots in a row
			cerr << "Error: Invalid dot usage" << endl;
			return -3;
		}
		count++;
	}
	count++;	//can only have one '@', increment count once after the single @ is found
	
	//the domain part of an email has a separate set of valid characters
	//at least one '.' is required for a valid domain
	while (input[count] != '.') {
		if (((input[count] < 'A' || input[count] > 'Z') && (input[count] < 'a' || input[count] > 'z') &&
		(input[count] < '0' || input[count] > '9') && input[count] != '-')) {
			cerr << "Error: Invalid character" << endl;
			return -3;
		}
		count++;
	}
	
	//the rest of the domain can be any valid characters, however cannot end in a '.'
	while (input[count]) {
		if (((input[count] < 'A' || input[count] > 'Z') && (input[count] < 'a' || input[count] > 'z') &&
		(input[count] < '0' || input[count] > '9') && input[count] != '-') && input[count] != '.') {
			cerr << "Error: Invalid character" << endl;
			return -3;
		}
		if (input[count] == '.' && (input[count - 1] == '.' || input[count + 1] == '.' || input[count + 1] == 0)) {
			cerr << "Error: Invalid dot usage" << endl;
			return -3;
		}
		count++;
	}
	
	//valid email
	return 0;
}

string UserInfo::capitalize(string name){//capitalize name for format
	//declare new name to be capitalized
	string newName = name;
	
	//capitalize only lowercase letters that come after a space
	//set other letters to lowercase
	for (int i = 0; i < newName.length(); i++) {
		if ((i == 0 || newName[i - 1] == ' ' || newName[i - 1] == '-' || newName[i - 1] == '\'') && newName[i] >= 'a' && newName[i] <= 'z')
			newName[i] = newName[i] - 32;
		else if (newName[i] >= 'A' && newName[i] <= 'Z' && (i != 0 && newName[i - 1] != ' ' && newName[i - 1] != '-' && newName[i - 1] != '\''))
			newName[i] = newName[i] + 32;
	}
	//return capitalized name
	return newName;
}

string UserInfo::getName(){ //getter name
	return name;
}

string UserInfo::getEmail(){//getter email
	return email;
}
