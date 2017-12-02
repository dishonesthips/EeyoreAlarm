#include <unistd.h>
#include <iostream>
#include <ugpio/ugpio.h>

using namespace std;

//gpio pin numbers
const int EXIT_PIN = 0;
const int TRIGGER_PIN = 1;
const int BUZZER_PIN = 11;

//pinMode == 0 for input, pinMode != 0 for output
int gpioSetup(const int pinNum, int &rq, const int pinMode) {
	int rv;
	// check if gpio is already exported
	if ((rq = gpio_is_requested(pinNum)) < 0) {
		cerr << "Error: GPIO pin " << pinNum << " is already in use."
			<< endl;
		return -1;
	}
	// export the gpio
	if (!rq) {
		if ((rv = gpio_request(pinNum, NULL)) < 0) {
			cerr << "Error: GPIO pin " << pinNum <<
				" could not be exported." << endl;
			return -1;
		}
	}
	// set to input direction
	if (pinMode == 0) {
		if ((rv = gpio_direction_input(pinNum)) < 0) {
			cerr << "Error: GPIO pin " << pinNum <<
				" could not be set as input." << endl;
			return -1;
		}
	} else {
		if ((rv = gpio_direction_output(pinNum, 1)) < 0) {
			cerr << "Error: GPIO pin " << pinNum <<
			       "could not be set as output." << endl;
			return -1;
		}
	}
	return 0;
}

int gpioRelease(const int pinNum, int &rq) {
	if (!rq) {
		if (gpio_free(pinNum) < 0) {
			cerr << "Error: Could not free GPIO pin " << pinNum <<
				endl;
			return -1;
		}
		return 0;
	}
	return 1;
}

int main() {
	int i;
	int exitVal;
	int rqExit;

	int retVal = gpioSetup(EXIT_PIN, rqExit, 0);
	if (retVal == -1) {
		return -1;
	}

	// read the gpio 20 times
	cout << "> begin reading GPIO" << EXIT_PIN << endl;
	for (i = 0; i < 20; i++) { 
		// read the gpio
		exitVal = gpio_get_value(EXIT_PIN);
		cout << "Exit: " << exitVal << endl;

		// pause between each read
		sleep(1);
	}

	// unexport the gpio
	gpioRelease(EXIT_PIN, rqExit);

	return 0;
}
