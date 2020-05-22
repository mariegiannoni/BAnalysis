#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include "KeyboardTest.h"

using namespace std;

//This function returns a list of short int between 0 and 10.
//The first one represents the quantity of erased characters.
//The second one is linked to the mistakes done at the ends of words.
//The third one count mistakes where a letter is written twice in a row.
// This is a function created to TEST our code
// Create a binary file with the results of keyboard data

void createKeyboardDataFile (char* patientName, char* testResultChar, char* allWrittenChar, double speedCharSec)
{
	int const LENGTHNAME(200);
	
	char fileName[LENGTHNAME];
	int valReturn = sprintf(fileName,"%s_keyboardata.bin",patientName);

	if(valReturn == -1){
		cout << "Error to create the name of the binary file." << endl;
		exit(1);
	}

	// creation of timeForChar array
	short timeForChar[strlen(allWrittenChar)];

	for (int i = 0; i < strlen(allWrittenChar); i++)
	{
		timeForChar[i] = (short) i*(100/speedCharSec);
	}


	FILE* keyboardDataFile = NULL;
	keyboardDataFile = fopen(fileName, "wb");

    if (keyboardDataFile == NULL)
    {
		cout << "Impossible de lire le fichier " << fileName << endl;
        exit(1);
	}

    short tab[2];																								 // tab contains 2 shorts : length of testResult, length of allWrittenChar
    tab[0] = (short) strlen(testResultChar);
    tab[1] = (short) strlen(allWrittenChar);

    int valWrote = fwrite ( tab, sizeof(short), 2, keyboardDataFile );                                          // write tab in keyboardDataFile
	
    if (valWrote != 2)
    {
		cout << "Writing error for lengthResult and lengthAll. valWrote " << valWrote << endl;
		exit(1);
	}
	
    valWrote = fwrite ( testResultChar, sizeof(char), strlen(testResultChar), keyboardDataFile );              // writting of char * testResultChar (just the chars, not the /0 at the end)

    if (valWrote != strlen(testResultChar))
	{
		cout << "Writing error for testResultChar string. valWrote " << valWrote<< endl;
	    exit(1);
	}
	
    valWrote = fwrite ( allWrittenChar, sizeof(char), strlen(allWrittenChar), keyboardDataFile );              // writting of char * allWrittenChar (just the chars, not the /0 at the end)

    if (valWrote != strlen(allWrittenChar))
    {
		cout << "Writing error for allWrittenChar string. valWrote " << valWrote << endl;
	    exit(1);
	}
	
    valWrote = fwrite ( timeForChar, sizeof(short), strlen(allWrittenChar), keyboardDataFile );              // writting of short* timeForChar

    if (valWrote != strlen(allWrittenChar))
    {
		cout << "Writing error for timeForChar array. valWrote " << valWrote << endl;
	    exit(1);
	}
	
    fclose( keyboardDataFile ) ;
}
