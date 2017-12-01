#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ugpio/ugpio.h>

using namespace std;

//GPIO pin numbers
const int EXIT_PIN = 0;
const int TRIGGER_PIN = 1;
const int BUZZER_PIN = 11;

//function for setting up GPIO pins
//for pinMode zero is output, non-zero is input
int gpioSetup(const int pinNum, const int pinMode) {
	//variables for ???
	int rq;
	int rv;
	//check if pin is already in use
	if ((rq = gpio_is_requested(pinNum)) < 0) {
		cerr << "Error: GPIO pin " << pinNum << " already in use." << endl;
		//log something here
		return -1;
	}
	//export pin
	if (!rq) {
		if ((rv = gpio_request(pinNum, NULL)) < 0) {
			cerr << "Error: GPIO pin " << pinNum << 
				" could not be exported." << endl;
			return -1;
			//log here
		}
	}
	//set pin to input or output
	if (pinMode == 0) {
		if ((rv = gpio_direction_input(pinNum)) < 0) {
			cerr << "Error: GPIO pin " << pinNum << " could not be set as input." << endl;
			//log something here
			return -1;
		}
	} else {
		if ((rv = gpio_direction_output(pinNum, 1)) < 0) {
			cerr << "Error: GPIO pin " << pinNum << " could not be set as output." << endl;
			//log something here
			return -1;
		}
	}
}

int main(const int argc, const char* const args[]){
	bool exit = false;

	//set up GPIO pins
	gpioSetup(EXIT_PIN, 1);
	gpioSetup(TRIGGER_PIN, 1);
	gpioSetup(BUZZER_PIN, 0);

	while (!exit){
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
			while (true) {
				cout << "Loop 1" << endl;
				//check button statuses
				int triggerVal = gpio_get_value(TRIGGER_PIN);
				int exitVal = gpio_get_value(EXIT_PIN);
				if (triggerVal == 1) {
					//turn off alarm
					gpio_set_value(TRIGGER_PIN, 0);
				}
				if (exitVal == 1) {
					//exit
					break; //YOLO
				}
			}
		}

		else if(menuAnswer[0] == '2'){
			
		}
		else if(menuAnswer[0] == '3'){
			
		}		
		else if(menuAnswer[0] == '4'){
			
		}		
		else if(menuAnswer[0] == '5'){
			
		}
		else if(menuAnswer[0] == '6'){
			exit = true;
		}
		
		//check for exit


	}
	cout<<"\n\tThanks for using Eeyore! Sweet Dreams!"<<endl;
	return 0;

}
