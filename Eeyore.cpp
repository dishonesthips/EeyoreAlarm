#include <iostream>
#include <string>
using namespace std;


int main(const int argc, const char* const args[]){
	bool exit = false;
	
	
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

		
	}
	cout<<"\n\tThanks for using Eeyore! Sweet Dreams!"<<endl;
	
	
	
}
