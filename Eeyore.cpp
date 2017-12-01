#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const string statFileName = "stats.txt";
const string alarmFileName = "alarms.txt";
enum SEVERITY {TRACE, DEBUG, INFO, WARNING, FATAL};

//class declarations
class UserInfo {
	public:
		const string filename = "userInfo.txt";
		UserInfo();	//declare constructor
		
		//declare functions
		bool fileNotExist();
		string getName();
		string getEmail();
		void writeInfo();
		void readInfo();
		
	private:
		//declare private variables
		string name;
		string email;
		
		//error check methods do not require an instance of a class
		string capitalize(string name);
		static int checkName(const string input);
		static int checkEmail(const string input);
};
//UserInfo member function declarations
UserInfo::UserInfo() { //constructor

	name = "";
	email = "";
}
void UserInfo::writeInfo() {//write info to a file with paramters info
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
void UserInfo::readInfo() {//read info from a file and sets values of object UserInfo accordingly

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
bool UserInfo::fileNotExist() {//check to see if text file already exists
	ifstream infile;
	infile.open(filename);
	if(infile.is_open()) {
		infile.close();
		return false;	//file exists
	}
	infile.close();
	return true; //file does not exist (yet) :smirk:
}
int UserInfo::checkName(const string input) {//static error check name
	//check for empty string
	if (input.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -2;
	}
	/*
	//test print name COMMENT OUT OF FINAL COPY
	cout << "Recieved name: ";
	for (int i = 0; i < input.length(); i++) {
		cout << input[i] << " ";
	}
	cout << endl;
	*/
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
int UserInfo::checkEmail(const string input) {//static error check email
	//check for empty string
	if (input.empty()) {
		cerr << "Error: Empty string!" << endl;
		return -3;
	}
	
	int count = 0;
	/*
	//test print email COMMENT OUT OF FINAL COPY
	cout << "Recieved email: ";
	for (int i = 0; i < input.length() + 1; i++) {
		cout << input[i] << " ";
	}
	cout << endl;
	*/
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
string UserInfo::capitalize(string name) {//capitalize name for format
	
	//declare new name to be capitalized
	string newName = name;
	
	//capitalize only lowercase letters that come after a space
	//set other letters to lowercase
	for (int i = 0; i < newName.length(); i++) {
		if ((i == 0 || newName[i - 1] == ' ' || newName[i - 1] == '-' || newName[i - 1] == '\'') && newName[i] >= 'a' && newName[i] <= 'z')
			newName[i] = newName[i] - 32;
		else if (newName[i] >= 'A' && newName[i] <= 'Z')
			newName[i] = newName[i] + 32;
	}
	/*
	//test print newName COMMENT OUT OF FINAL COPY
	cout << "Capitalized name: ";
	for (int i = 0; i < newName.length(); i++) {
		cout << newName[i] << " ";
	}
	cout << endl;
	*/
	//return capitalized name
	return newName;
}
string UserInfo::getName() {
	return name;
}
string UserInfo::getEmail() {
	return email;
}


//function declarations
int log(const string message, const string severity);

int log(const string severity, const string message) {
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
	
	logfile << (ltm->tm_year + 1900) << "-" << (ltm->tm_mon + 1) << "-" << ltm->tm_mday << "\t" 
			<< ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << "\t(" << severity << "):\t " << message << "\r\n";
	logfile.close();
	
	return 0;
}


int main(const int argc, const char* const args[]){
	bool exit = false;
	UserInfo user;
	
	if (user.fileNotExist()) { 
		cout<<"\n\tWelcome to Eeyore! Is this your first time?\n\tI don't recognize you...\n\n";
		user.writeInfo();
		user.readInfo();
		cout << user.getName() << " " << user.getEmail() << endl;
	}
	
	while (!exit){
		cout<<"\n\tWelcome to Eeyore, "<<user.getName()<<"\n\n\t"
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
		
	

		if(menuAnswer[0] == '1'){//Run Alarm
		}

		else if(menuAnswer[0] == '2'){//Add an Alarm
			
		}
		else if(menuAnswer[0] == '3'){//Delete and Alarm
			
		}		
		else if(menuAnswer[0] == '4'){//Update User Info
			user.writeInfo();
			user.readInfo();
			cout << user.getName() << " " << user.getEmail() << endl;
		}
		else if(menuAnswer[0] == '5'){//View Statistics
			
		}
		else if(menuAnswer[0] == '6'){//Exit
			log("TRACE","Manual request to exit program");
			exit = true;
		}
		else{
			log("WARNING","Error checking menuAnswer went wrong, treated as if exit request;");
			exit = true;
		}


	}
	cout<<"\n\tThanks for using Eeyore! Sweet Dreams!"<<endl;
	return 0;

}
