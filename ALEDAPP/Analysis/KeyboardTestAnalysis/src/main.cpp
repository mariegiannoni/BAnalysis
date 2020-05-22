#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <math.h>

// KEYBOARD TEST
#include "KeyboardData.h"
#include "KeyboardTest.h"
#include "KeybSympCrit.h"

string PATHTOALEDAPP;


#define ASKEDSENTENCE "Hello and welcome to the MedicHuber app! How are you?"


using namespace std;

int main( int argc, char** argv )
{
    if (argc == 1)
    {
    	cout << "Classic mode :" << endl;
        cout << "1st arg : '10' // 2nd arg : Patient ID // 3rd PATHTOALEDAPP \n";
        cout << "this program will create an output in ALEDAPP/Analysis/FinalAnalysis/PreviousOutputs/PatientPATIENTID\n\n";
        cout << endl;
        cout << "Draft interface on current webcam : 1] '1' // 2] '0' // 3] PATHTOALEDAPP \n" ;
    }

	// Classic mode
    else if(argc == 4 && strcmp(argv[1],"10")==0)
    {
    	PATHTOALEDAPP = argv[3];
    	// extraction of information from true input : ./ALEDAPP/ReceivedData/Patient+PATIENTID
    	// into a KeyboardData object

    	string pathToInput = PATHTOALEDAPP + "/ReceivedData/" + "Patient" + argv[2] + "/" + "Patient" + argv[2] + "KT.txt";

    	//std::cout << pathToInput << std::endl;

    	KeyboardData* myKeyboardData = new KeyboardData(10, pathToInput, ASKEDSENTENCE );

    	//cout << "asked sentence : " << myKeyboardData->getAskedSentence();
    	//cout << "my final sentence : " <<  myKeyboardData->getPatientWrittenChar() << endl;
    	myKeyboardData->listSpeed();
		myKeyboardData->analyzeMistakes(10);

		KeybSympCrit* myKeybSympCrit = new KeybSympCrit(myKeyboardData->getTabSpeed(), myKeyboardData->getMistakes());


		myKeybSympCrit->createOutputAnalysis(argv[2],PATHTOALEDAPP);

    }
        // draft interface mode
    else if(argc == 2 && strcmp(argv[1],"1")==0)
    {
    	PATHTOALEDAPP = argv[3];
		// Keyboard test
		cout << "Presentation of KeyboardTest :" << endl;
		cout << "This module is composed of 7 functions" << endl;
		cout << "createKeybData will create a binary file to simulate a input." << endl;
		cout << "We will do 2 test here : the first one will be for a healthy user and the second one for an unheathy." << endl;

		cout << "Enter Enter to continue, please" << endl;
		getchar();

		cout << "First one" << endl;
		cout << "Create the binary file" << endl;
		createKeyboardDataFile ( (char*)"Lucile_Peltier", (char*)"Hello and welcome to the MedicHuber app! How are you?", (char*)"Hello and welcome to the MedicHuber app! How are you?", 4);

		cout << "Enter Enter to continue, please" << endl;
		getchar();

		cout << "Transforme the binary file into a class" << endl;
		KeyboardData* myKeyboardData = new KeyboardData((char*)"Lucile_Peltier");
		cout << " ici " << endl;
		myKeyboardData->listSpeed();
		myKeyboardData->analyzeMistakes(0);

		//Print the information inside the class KeyboardDatato test
		myKeyboardData->printKeyboardData();

		cout << "And the last one that return a list of symptoms with their criticality level" << endl;
		KeybSympCrit* myKeybSympCrit = new KeybSympCrit(myKeyboardData->getTabSpeed(), myKeyboardData->getMistakes());

		//Print the information inside the class KeybSympCrit to test
		myKeybSympCrit->printKeybSympCrit();

		cout << "End of test. \n .................................." << endl;

		cout << "Enter Enter to continue, please" << endl;
		getchar();

		cout << "We delete the object and we free the memory" << endl;

		//Delete object and free memory
		myKeybSympCrit->~KeybSympCrit();
		myKeyboardData->~KeyboardData();
		free(myKeybSympCrit);
		free(myKeyboardData);

		cout << "Enter Enter to continue, please" << endl;

		cout << "Enter Enter to continue, please" << endl;
		getchar();

		cout << "Let's do a second test :" << endl;
		cout << "Create the binary file" << endl;
		createKeyboardDataFile ( (char*)"Arthur_LeBlanc", (char*)"Hello and welcome to the MedicHuber app! How are you?", (char*)"Helloo$and welcomme to the MedicHuber appp$! How are $ you?",2);

		cout << "Enter Enter to continue, please" << endl;
		getchar();

		cout << "Transforme the binary file into a structure" << endl;
		KeyboardData* myKeyboardData2 = new KeyboardData((char*)"Arthur_LeBlanc");
		myKeyboardData2->listSpeed();
		myKeyboardData2->analyzeMistakes(0);

		//Print the information inside the class KeyboardDatato test
		myKeyboardData2->printKeyboardData();

		cout << "And the last one that return a list of symptoms with their criticality level" << endl;
		KeybSympCrit* myKeybSympCrit2 = new KeybSympCrit(myKeyboardData2->getTabSpeed(), myKeyboardData2->getMistakes());

		//Print the information inside the class KeybSympCrit to test
		myKeybSympCrit2->printKeybSympCrit();

		cout << "End of test. \n .................................." << endl;

		cout << "Enter Enter to continue, please" << endl;
		getchar();

		cout << "We delete the object and we free the memory" << endl;

		//Delete object and free memory
		myKeybSympCrit2->~KeybSympCrit();
		myKeyboardData2->~KeyboardData();
		free(myKeybSympCrit2);
		free(myKeyboardData2);
	}

	return 0;
}

