#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ugpio/ugpio.h>

using namespace std;

//the pin used for the exit button
const int EXIT_PIN = 0;

int main(const int argc, const char* const args[]){
	bool exit = false;
	//gpio pin variables
	int rq;
	int rv;

	//set up gpio pin
	if ((rq = gpio_is_requested(EXIT_PIN)) < 0) {
		cerr << "Error: GPIO pin " << EXIT_PIN << " already in use." << endl;
		//log something here
		return -1;
	}

	//export gpio pin
	if (!rq) {
		if ((rv = gpio_request(EXIT_PIN, NULL)) < 0) {
			cerr << "Error: GPIO pin " << EXIT_PIN << 
				" could not be exported." << endl;
			return -1;
			//log here
		}
	}

	//set exit button as input
	if ((rv = gpio_direction_input(EXIT_PIN)) < 0) {
		cerr << "Error: GPIO pin " << EXIT_PIN << " could not be set as output." << endl;
	}

	while (!exit){
		//get exit button status
		int exitVal = gpio_get_value(EXIT_PIN);

		cout<<"\n\tWelcome to Eeyore, <YOUR NAME HERE>,\n\n\t"
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

		if(menuAnswer[0] == '1'){

		}

		else if(menuAnswer[0] == '2'){
			
		}
		else if(menuAnswer[0] == '3'){
			
		}		
		else if(menuAnswer[0] == '4'){
			
		}		
		else if(menuAnswer[0] == '5'){
			
		}
		else if(menuAnswer[0] == '6' || exitVal == 1){
			exit = true;
		}
		
		//check for exit


	}
	cout<<"\n\tThanks for using Eeyore! Sweet Dreams!"<<endl;


}
