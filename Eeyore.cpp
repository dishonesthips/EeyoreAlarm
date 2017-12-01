#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const string logFileName = "log.txt";
const string uInfoFileName = "userInfo.txt";
const string statFileName = "stats.txt";
const string alarmFileName = "alarms.txt";
enum SEVERITY {TRACE, DEBUG, INFO, WARNING, FATAL};

int log(const string source, const string message, const SEVERITY severity);
int writeInfo(const string name, const string email);
int checkName(const string name);
int checkEmail(const string email);
string capitalize(const string name);

int log(const string source, const string message, const SEVERITY severity) {
	//initialize time and set to local
	time_t now = time(0);
	tm* ltm = localtime(&now);
	
	
	//declare filename to be written to
	const string filename = to_string(ltm->tm_year + 1900) + "-" + to_string(ltm->tm_mday) + "-" + to_string(ltm->tm_mon + 1) + "-" +
							to_string(ltm->tm_hour) + "-" + to_string(ltm->tm_min) + "-" + to_string(ltm->tm_sec) + ".log";
	
	ofstream logfile; // declare the file object
	logfile.open(filename, ios::app | ios::out); // open the file
	if (!logfile.is_open()) {
		cerr << "Unable to open file" << endl;
		return -1; // Unable to open file
	}
	
	logfile << asctime(ltm) << "\r\n" << "Source: " << source << "\r\n" << message << "\r\n" << severity << "\r\n\r\n";
	logfile.close();
	
	return 0;
}
int userInfo(bool update){
	string name;
	string email;
	if (!update){
		ifstream infile;
		infile.open(uInfoFileName);
		if(infile.is_open()) {
			cerr << "Error: File already exists" << endl;
			infile.close();
			return -1;//file exists
		}
		else{
			return 0;//file doesnt exist
		}
	}
	
	//get name
	cout << "\tWhat's your name? ";
	getline(cin, name);
	while (checkName(name)) {
		cout << "\tPlease enter a valid name: ";
		getline(cin, name);
	}
	
	//get email
	cout << "\tEnter your email: ";
	getline(cin, email);
	while (checkEmail(email)) {
		cout << "\tPlease enter a valid email: ";
		getline(cin, email);
	}
	
	//write to file
	cout << (writeInfo(name, email)) << endl;
	
	return 0;
}
int writeInfo(const string name, const string email) {
	//declare filename to be written to
	
	/*
	//check to see if the file already exists, error
	ifstream infile;
	infile.open(uInfoFileName);
	if(infile.is_open()) {
		cerr << "Error: File already exists" << endl;
		return -1;
	}
	*/ //now happens at start of userInfo
	
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
		if ((i == 0 || newName[i - 1] == ' ' || newName[i - 1] == '\'' || newName[i - 1] == '-') && newName[i] >= 'a' && newName[i] <= 'z')
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
int main(const int argc, const char* const args[]){
	bool exit = false;
	string name;
	string email;
	
	
	if (userInfo(false) == 0){ //will return 0 if the userInfo file doesnt exists
		cout <<"\n\n\tWelcome to Eeyore!\n\tWe don't recognize you...\n\n";
		userInfo(true);
	}
	//string* info = readInfo();
	//name = info[0];
	//email = info[1];
	
	cout <<"\n\tWelcome to Eeyore, <YOUR NAME HERE>!"<<endl;
	while (!exit){
		
		cout<<"\n\tMain Menu\n\n\t"
			<<"1. Run Alarm System\n\t"
			<<"2. Add an Alarm\n\t"
			<<"3. Delete an Alarm\n\t"
			<<"4. Update User Info\n\t"
			<<"5. View Statistics\n\t"
			<<"6. Exit\n\n\t"
			<<"Please enter a number corresponding to one of the options: ";
		
		string menuAnswer;
		getline(cin, menuAnswer);
		/*
		while (checkMenuAnswer(menuAnswer)) {
			cout << "Please enter a single digit in range [1,6]: ";
			getline(cin, menuAnswer);
		}
		*/
		
		if(menuAnswer[0] == '1'){ //Run Alarm
			
		}
		else if(menuAnswer[0] == '2'){//Add an Alarm
			
		}
		else if(menuAnswer[0] == '3'){//Delete and Alarm
			
		}		
		else if(menuAnswer[0] == '4'){//Update User Info
			userInfo(true);
			//string* info = readInfo();
			//name = info[0];
			//email = info[1];
		}
		else if(menuAnswer[0] == '5'){//View Statistics
			
		}
		else if(menuAnswer[0] == '6'){//Exit
			exit = true;
		}
		else{
			//log warning error with checkMenuAnswer code
			exit = true;
		}

	}
	cout<<"\n\tThanks for using Eeyore! Sweet Dreams!"<<endl;
}
