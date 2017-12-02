#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class UserInfo {
	public:
		const string filename = "userinput.txt";
		UserInfo();	//declare constructor
		
		//declare functions
		int checkFileExists();
		string getName();
		string getEmail();
		void writeInfo();
		void readInfo();
		
	private:
		//declare private variables
		string name;
		string email;
		
		//error check methods do not require an instance of a class
		void capitalize();
		static int checkName(const string input);
		static int checkEmail(const string input);
};

//constructor
UserInfo::UserInfo() {
	name = "";
	email = "";
}

//write info to a file with paramters info
void UserInfo::writeInfo() {
	//declare strings to be written to file
	string nameFile;
	string emailFile;
	
	//get name
	cout << "What is your name? ";
	getline(cin, nameFile);
	while (UserInfo::checkName(nameFile)) {
		cout << "name: ";
		getline(cin, nameFile);
	}
	
	//get email
	cout << "What is your email? ";
	getline(cin, emailFile);
	while (UserInfo::checkEmail(emailFile)) {
		cout << "email: ";
		getline(cin, emailFile);
	}
	//write to file
	ofstream outfile;
	outfile.open(filename);
	outfile << nameFile << "," << emailFile << "\r\n";
	outfile.close();
}

//read info from a file and sets values of object UserInfo accordingly
void UserInfo::readInfo() {
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
	capitalize();
}

//check to see if text file already exists
int UserInfo::checkFileExists() {
	ifstream infile;
	infile.open(filename);
	if(infile.is_open()) {
		return 1;	//file exists
	}
	infile.close();
	return 0; //file does not exist (yet) :smirk:
}

//error check name
int UserInfo::checkName(const string input) {
	//check for empty string
	if (input.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -2;
	}
	
	//test print name COMMENT OUT OF FINAL COPY
	cout << "Recieved name: ";
	for (int i = 0; i < input.length(); i++) {
		cout << input[i] << " ";
	}
	cout << endl;
	
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

int UserInfo::checkEmail(const string input) {
	//check for empty string
	if (input.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -3;
	}
	
	int count = 0;
	
	//test print email COMMENT OUT OF FINAL COPY
	cout << "Recieved email: ";
	for (int i = 0; i < input.length() + 1; i++) {
		cout << input[i] << " ";
	}
	cout << endl;
	
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
		if (count > input.length()) {
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

//capitalize name for format
void UserInfo::capitalize() {
	
	//capitalize only lowercase letters that come after a space
	//set other letters to lowercase
	for (int i = 0; i < name.length(); i++) {
		if ((i == 0 || name[i - 1] == ' ' || name[i - 1] == '-' || name[i - 1] == '\'') && name[i] >= 'a' && name[i] <= 'z')
			name[i] = name[i] - 32;
		else if (name[i] >= 'A' && name[i] <= 'Z')
			name[i] = name[i] + 32;
	}
	
	//test print newName COMMENT OUT OF FINAL COPY
	cout << "Capitalized name: ";
	for (int i = 0; i < name.length(); i++) {
		cout << name[i] << " ";
	}
	cout << endl;
}

//getters
string UserInfo::getName() {
	return name;
}
string UserInfo::getEmail() {
	return email;
}

//sample usage of object UserInfo shown in main
int main() {
	UserInfo user1;
	
	if (user1.checkFileExists()) {
		user1.writeInfo();
		user1.readInfo();
		cout << user1.getName() << " " << user1.getEmail() << endl;
	}
	
	return 0;
}