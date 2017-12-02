#include <iostream>
#include <string>
#include <fstream>
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
//for pinMode zero is input, non-zero is output
int gpioSetup(const int pinNum, int &rq, const int pinMode) {
	//variables for ???
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
	return 0;
}

int main(const int argc, const char* const argv[]) {
	bool exit = false;
	
	cout << "exit pin: " << EXIT_PIN << endl;

	//set up GPIO pins, declare variables
	int rqExit;
	int rqTrigger;
	int rqBuzzer;
	int retVal = gpioSetup(EXIT_PIN, rqExit, 1);
	//gpioSetup(TRIGGER_PIN, rqTrigger, 1);
	//gpioSetup(BUZZER_PIN, rqBuzzer, 0);
	if (retVal == -1) {
		return -1;
	}
	int exitValue;
	/*while (true) {
		exit = true;
		exitValue = gpio_get_value(EXIT_PIN);
		cout << "read exit: " << exitValue << endl;
	}*/
	for (int i = 0; i < 20; i++) {
		exitValue = gpio_get_value(EXIT_PIN);
		cout << "Exit: " << exitValue << endl;
		sleep(1);
	}

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

		if(menuAnswer[0] == '1'){//Run Alarm
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

		else if(menuAnswer[0] == '2'){//Add an Alarm
		}
		else if(menuAnswer[0] == '3'){//Delete and Alarm
		}		
		else if(menuAnswer[0] == '4'){//Update User Info
		}
		else if(menuAnswer[0] == '5'){//View Statistics
		}
		else if(menuAnswer[0] == '6'){//Exit
			exit = true;
		}
		else{
			exit = true;
		}


	}

	//unexport the gpios
	if (!rqExit) {
		cout << "unexporting exit pin" << endl;
		if (gpio_free(EXIT_PIN) < 0) {
			cerr << "gpio_exit_free" << endl;
		}
	}

	cout<<"\n\tThanks for using Eeyore! Sweet Dreams!"<<endl;
	return 0;

}
