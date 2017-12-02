#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
using namespace std;

int writeInfo(const string name, const string email);
int checkName(const string name);
int checkEmail(const string email);
string capitalize(const string name);

int writeInfo(const string name, const string email) {
	//declare filename to be written to
	
	//check to see if the file already exists, error
	ifstream infile;
	infile.open(uInfoFileName);
	if(infile.is_open()) {
		cerr << "Error: File already exists" << endl;
		return -1;
	}
	
	//create a new name string to be edited
	string newName = capitalize(name);
	
	ofstream outfile; // declare the file object
	outfile.open(uInfoFileName); // open the file
	if (!outfile.is_open()) {
		cerr << "Unable to open file" << endl;
		return -1; // Unable to open file
	}
	
	outfile << newName << "," << email << "\n";
	
	outfile.close();
	return 0;
}

int checkName(const string name) {
	//check for empty string
	if (name.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -2;
	}
	
	//test print name COMMENT OUT OF FINAL COPY
	cout << "Recieved name: ";
	for (int i = 0; i < name.length(); i++) {
		cout << name[i] << " ";
	}
	cout << endl;
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < name.length(); i++) {
		if (name[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -2;
		}
	}
	
	//valid name
	return 0;
}

int checkEmail(const string email) {
	//check for empty string
	if (email.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -3;
	}
	
	int count = 0;
	
	//test print email COMMENT OUT OF FINAL COPY
	cout << "Recieved email: ";
	for (int i = 0; i < email.length() + 1; i++) {
		cout << email[i] << " ";
	}
	cout << endl;
	
	//error checking for email name validity
	if (email[count] == '.') {
		cerr << "Error: Invalid dot usage" << endl;
		return -3;	//email cannot begin with a '.'
	}
	
	//check for  (weird bug involving cin operator)
	for (int i = 0; i < email.length(); i++) {
		if (email[i] == '') {
			cerr << "Don't use arrow keys!" << endl;
			return -2;
		}
	}
	
	//first first an '@'; if cannot find within length of email characters, error
	//the local-part of an email can only include certain characters; if invalid character, error
	while (email[count] != '@') {
		if (count > email.length()) {
			cerr << "Error: Invalid email. Cannot find '@'" << endl;
			return -3;
		}
		if ((email[count] != '!' && (email[count] < '#' || email[count] > 39) && 
		email[count] != '*' && email[count] != '+' && (email[count] < '-' || email[count] > '/') && 
		email[count] != '=' && email[count] != '?' && (email[count] < '^' || email[count] > '~') &&
		(email[count] > '{' || email[count] < '~')) && (email[count] < 'A' || email[count] > 'Z') &&
		(email[count] < '0' || email[count] > '9') && email[count]) {
			cerr << "Error: Invalid character" << endl;
			return -3;
		}
		if (email[count] == '.' && (email[count - 1] == '.' || email[count + 1] == '.')) {			//cannot have two dots in a row
			cerr << "Error: Invalid dot usage" << endl;
			return -3;
		}
		count++;
	}
	count++;	//can only have one '@', increment count once after the single @ is found
	
	//the domain part of an email has a separate set of valid characters
	//at least one '.' is required for a valid domain
	while (email[count] != '.') {
		if (((email[count] < 'A' || email[count] > 'Z') && (email[count] < 'a' || email[count] > 'z') &&
		(email[count] < '0' || email[count] > '9') && email[count] != '-')) {
			cerr << "Error: Invalid character" << endl;
			return -3;
		}
		count++;
	}
	
	//the rest of the domain can be any valid characters, however cannot end in a '.'
	while (email[count]) {
		if (((email[count] < 'A' || email[count] > 'Z') && (email[count] < 'a' || email[count] > 'z') &&
		(email[count] < '0' || email[count] > '9') && email[count] != '-') && email[count] != '.') {
			cerr << "Error: Invalid character" << endl;
			return -3;
		}
		if (email[count] == '.' && (email[count - 1] == '.' || email[count + 1] == '.' || email[count + 1] == 0)) {
			cerr << "Error: Invalid dot usage" << endl;
			return -3;
		}
		count++;
	}
	
	//valid email
	return 0;
}

string capitalize(const string name) {
	//declare new name to be capitalized
	string newName = name;
	
	//capitalize only lowercase letters that come after a space
	//set other letters to lowercase
	for (int i = 0; i < newName.length(); i++) {
		if ((i == 0 || newName[i - 1] == ' ') && newName[i] >= 'a' && newName[i] <= 'z')
			newName[i] = newName[i] - 32;
		else if (newName[i] >= 'A' && newName[i] <= 'Z')
			newName[i] = newName[i] + 32;
	}
	
	//test print newName COMMENT OUT OF FINAL COPY
	cout << "Capitalized name: ";
	for (int i = 0; i < newName.length(); i++) {
		cout << newName[i] << " ";
	}
	cout << endl;
	
	//return capitalized name
	return newName;
}

int main() {
	//declare variables
	string name;
	string email;
	
	//get name
	cout << "Enter a name: ";
	getline(cin, name);
	while (checkName(name)) {
		cout << "Please enter a valid name: ";
		getline(cin, name);
	}
	
	//get email
	cout << "Enter an email: ";
	getline(cin, email);
	while (checkEmail(email)) {
		cout << "Please enter a valid email: ";
		getline(cin, email);
	}
	
	//write to file
	cout << (writeInfo(name, email)) << endl;
	
	//good job! ^__^
	return 0;
}