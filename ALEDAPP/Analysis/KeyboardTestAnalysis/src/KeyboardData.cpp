#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include "KeyboardData.h"

using namespace std;

//Destructor
KeyboardData::~KeyboardData()
{
	if(m_askedSentence != NULL)
	delete [] m_askedSentence;

	if(m_patientWrittenChar != NULL)
	delete [] m_patientWrittenChar;

	if(m_timeForChar != NULL)
		delete [] m_timeForChar;

	if(m_tabSpeed != NULL)
		delete [] m_tabSpeed;

	if(m_mistakes != NULL)
		delete [] m_mistakes;
}


// classic builder

KeyboardData::KeyboardData(int choice, string fileAddress, string askedSentence )
{

	m_tabSpeed = NULL;
	m_mistakes = NULL;
	m_patientWrittenChar = NULL;
	m_timeForChar = NULL;




	// open file
    ifstream KBFile(fileAddress);
    string writtenCharAndTime;
    int lengthAllChar = 0;
    //char peekChar;

    if (KBFile.is_open())
    {
        //cout << endl << "Open KBFile OK" << endl;

        // -------------------------------------------------------------------------------------------------------------
        // first get ALL WRITTEN CHAR

        // ignore empty line
        getline (KBFile,writtenCharAndTime);

        while (writtenCharAndTime == "")
        {
        	getline (KBFile,writtenCharAndTime);
        }

        // find the number of written char in writtenCharAndTime
        // count the number of comma
        for (int i = 0; i < (int)writtenCharAndTime.size(); i++)
        {
        	if (writtenCharAndTime[i]==',')
        		lengthAllChar++;
        }


		// --------------------------------------------------------
		// Fill : m_patientWrittenChar - m_timeForChar

		stringstream SMwrittenCharAndTime(writtenCharAndTime);

		m_patientWrittenChar = new char[lengthAllChar + 1]; //dynamic allocation for m_patientWrittenChar
		m_timeForChar = new short[lengthAllChar]; // dynamic allocation for m_timeForChar

		short firstTime = 0;
		short currentTime = 0;
		char currentChar = ' ';

		for (int i = 0; i < lengthAllChar ; i++)
		{
			if (i == 0) // FIRST LOOP
			{

				SMwrittenCharAndTime >> firstTime;
				m_timeForChar[0] = 0;
			}
			else
			{
				SMwrittenCharAndTime  >> currentTime;
				m_timeForChar[i]= currentTime - firstTime;

				if (m_timeForChar[i] < 0)
				{
					cout << "ERROR in collecting information from Data file !!!" << endl;
					exit(0);
				}
			}

			SMwrittenCharAndTime.ignore(1,'/');

			SMwrittenCharAndTime >> currentChar;

			// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// SPECIAL CASE for some "currentChar"

			if (currentChar == ',') // That mean there is a  space  --> ' '
			{
				m_patientWrittenChar[i] = ' ';
			}
			else if (currentChar == 'S') // That mean there is the word Shift
			{
				// In this version we ignore it.
				SMwrittenCharAndTime.ignore(5,',');
				i--;
				m_patientWrittenChar[lengthAllChar] = '\0';
				lengthAllChar--;
			}
			else if (currentChar == 'B') // That mean there is the word Backspace, to erase a char, we remplace it by '$'
			{
				SMwrittenCharAndTime.ignore(10,',');
				m_patientWrittenChar[i] = '$';
			}
			else
			{
				m_patientWrittenChar[i] = currentChar;
				SMwrittenCharAndTime.ignore(1,',');

			}
		}

		m_patientWrittenChar[lengthAllChar] = '\0'; //add the \0


	/*	// TEST
		for (int i = 0; i < lengthAllChar ; i++)
		{
			cout << " char numb : " << i << "is : " << m_patientWrittenChar[i] << endl;
			cout << " time char numb : " << i << "is : " << m_timeForChar[i] << endl;
		}  */

  /*      // -------------------------------------------------------------------------------------------------------------
        // secondly, get ALL WRITTEN CHAR

        string testResultChar;
		getline (KBFile,testResultChar); // Jump 3 lines
		getline (KBFile,testResultChar);
		getline (KBFile,testResultChar);
		getline (KBFile,testResultChar); */

		//cout << testResultChar << endl;



	/*	short lengthResult = testResultChar.size();
		m_askedSentence = new char[lengthResult + 1]; //dynamic allocation for m_askedSentence

		for (int i = 0; i < (int)testResultChar.size(); i++)
		{
			m_askedSentence[i] = testResultChar[i];
		}

		m_askedSentence[lengthResult] = '\0'; //add the \0  */


		m_askedSentence = new char[askedSentence.size() + 1];

		for (int i = 0; i < (int)askedSentence.size(); i++)
		{
			m_askedSentence[i] = askedSentence[i];
		}

		m_askedSentence[askedSentence.size()] = '\0'; //add the \0  */



        KBFile.close();

    }


}







//draft interface builder
KeyboardData::KeyboardData(char* patientName)
{
	m_tabSpeed = NULL;
	m_mistakes = NULL;


	int const LENGTHNAME(200);
	char fileName[LENGTHNAME];
	int valReturn = sprintf(fileName,"%s_keyboardata.bin",patientName);

	if(valReturn == -1){
		cout << "Error to create the name of the binary file." << endl;
		exit(1);
	}

	FILE* keyboardDataFile = NULL;
	keyboardDataFile = fopen(fileName, "rb");  // open the keyboard test binary file.

    if (keyboardDataFile == NULL)	//if we can't open the binary file
    {
        cout << "Impossible de lire le fichier " << fileName << endl;
        exit(1);
    }
    else 	//if we can open the file and read it
	{
		short lengthResult(0);
		short lengthAll(0);

		int readVal = fread( &lengthResult, sizeof(short), 1, keyboardDataFile); // we read the two first bytes and write in lengthresult

		if (readVal != 1) 	//if we read less than 1 value we exit the program
        {
            cout << "Value of lengthResult not read." << endl;
            exit(1);
        }

		readVal = fread( &lengthAll, sizeof(short), 1, keyboardDataFile); // we read the next two bytes and write in lengthall

		if (readVal != 1) 	//if we read less than 1 value we exit the program
        {
            cout << "Value of lengthAll not read." << endl;
            exit(1);
        }

        m_timeForChar = new short[lengthAll]; // dynamic allocation for m_timeForChar

		m_askedSentence = new char[lengthResult + 1]; //dynamic allocation for m_askedSentence

		m_patientWrittenChar = new char[lengthAll + 1]; //dynamic allocation for m_patientWrittenChar

        readVal = fread( m_askedSentence, sizeof(char), lengthResult, keyboardDataFile); // read and fill testresultchar

		if (readVal != lengthResult) 	//if we read less than lengthResult value we exit the program
            {
                cout << "Value of testResultChar not read." << endl;
                exit(1);
            }

		m_askedSentence[lengthResult] = '\0'; //add the \0

		readVal = fread( m_patientWrittenChar, sizeof(char), lengthAll, keyboardDataFile); // read and fill allWrittenChar

		if (readVal != lengthAll) 	//if we read less than lengthall value we exit the program
            {
                cout << "Value of allWrittenChar not read." << endl;
                exit(1);
            }

		m_patientWrittenChar[lengthAll] = '\0'; //add the \0

        readVal = fread( m_timeForChar, sizeof(short), lengthAll, keyboardDataFile); // read and fill allWrittenChar

		if (readVal != lengthAll) 	//if we read less than lengthAll value we exit the program
            {
                cout << "Value of timeforchar not read." << endl;
                exit(1);
            }

		fclose(keyboardDataFile);

	}
}


// This function returns the average speed of the test in characters/second.
// It also returns the average speed on each word of the test.
// in the return we have : in [0] the adverage speed / after, the adverage per words
void KeyboardData::listSpeed ()
{
	// calcul of the speed adverage :
	double speedAdverage(0);
	double totalTimeInS = m_timeForChar[strlen(m_patientWrittenChar)-1]/100; //  length m_patientWrittenChar = length timeForChar
	speedAdverage = (strlen(m_patientWrittenChar)-1) / totalTimeInS;

	// calcul of the speed per words :
	int numberOfWords(1);                                          // +1 because the last word is not count.
	for (int i(1); i < (int)strlen(m_patientWrittenChar); i++)
	{
		if(m_patientWrittenChar[i-1] != ' ' &&  m_patientWrittenChar[i] == ' ' && m_patientWrittenChar[i+1] != ' ')
			numberOfWords ++;
	}
	// initialze the tab
	m_tabSpeed = new double[numberOfWords + 2]; // on the first we put the number of words
													  // on the 1 to n blocks of the tab, the 1 to n speed in fct of the word.
													  // on the last one we put the total speed adverage

	//On the first we put the number of words
	m_tabSpeed[0] = numberOfWords;

	//On the last, we put the average speed
	m_tabSpeed[numberOfWords + 1] = speedAdverage;

	// adverage per words
	int indiceBeginning(0);
	int indiceEnd(0);
	int wordNumber(1);

	for (int i(1); i < (int)strlen(m_patientWrittenChar); i++)
	{
		if(m_patientWrittenChar[i-1] != ' ' &&  m_patientWrittenChar[i] == ' ' && m_patientWrittenChar[i+1] != ' ')
		{
			//cout << "my indice is : " << i << endl;
			indiceBeginning = indiceEnd;
			indiceEnd = i-1;

			//calcul the speedAdverage on each words
			speedAdverage = (indiceEnd - indiceBeginning)*100/(m_timeForChar[indiceEnd] - m_timeForChar[indiceBeginning]);

			//cout << "my speedaverage " << speedAdverage << endl;
			m_tabSpeed[wordNumber] = speedAdverage;

			wordNumber++;
		}
	}

	// calcul for the last word :
	indiceBeginning = indiceEnd;
	indiceEnd = strlen(m_patientWrittenChar)-2;
	speedAdverage = (indiceEnd - indiceBeginning)*100/(m_timeForChar[indiceEnd] - m_timeForChar[indiceBeginning]);

	//printf("my speedaverage %f\n",speedAdverage);
	m_tabSpeed[wordNumber] = speedAdverage;

}

//This function returns a list of short int between 0 and 10.
//The first one represents the quantity of erased characters.
//The second one is linked to the mistakes done by words.
//The third one is linked to the mistakes of double letter.

// if choice = 10 there is no display on the console

void KeyboardData::analyzeMistakes (int choice)
{
	// alloc dynamic for the return value
	m_mistakes = new short[3];

	//The first return value represents the quantity of erased characters.
	// in the table ascii the number of "Backspace key" is 8
	// For the beta version we will use the symbole '$'
	short numBS(0);
	for (int i(0); i < (int)strlen(m_patientWrittenChar); i++)
	{
		if (m_patientWrittenChar[i] == '$')
		{
			numBS++;
		}
	}
	if (numBS > 10)
	{
		numBS = 10;
	}
	m_mistakes[0] = numBS;

	// ------------------------------------------------------------------------------------------------
	// The second analysis :
	// return the number of word with a mistake

	// separate all words of the first sentence in a list of Strings
	char listOriginalWords[50][30];

	//int indiceWordBeginning = 0;
	int indiceForEachWord = 0;
	int originalWordsNumber = 0;


	for (int i(0); i < (int)strlen(m_askedSentence) + 1; i++)
	{
		// if it is a char : write the char
		if (m_askedSentence[i] != ' ' && m_askedSentence[i] != '$')
		{
			listOriginalWords[originalWordsNumber][indiceForEachWord] = m_askedSentence[i];
			indiceForEachWord ++;
		}
		// debut d'un nouveau mot
		if(m_askedSentence[i-1] != ' ' &&  m_askedSentence[i] == ' ' && m_askedSentence[i+1] != ' ')
		{
			listOriginalWords[originalWordsNumber][indiceForEachWord] = '\0';
			originalWordsNumber ++;
			indiceForEachWord = 0;
		}
		// at the end of original sentence :
		if (m_patientWrittenChar[i] == '\0')
		{
			listOriginalWords[originalWordsNumber][indiceForEachWord] ='\0';
		}
	}

	if (choice != 10)
	{
		for (int i(0); i <= originalWordsNumber; i++)
		{
			cout << "Word number : " << i << " is : " << listOriginalWords[i] << endl;
		}
	}


	// separate all words of the second sentence in a list of Strings
	char listPatientWords[50][30];

	//int indiceWordBeginning = 0;
	indiceForEachWord = 0;
	originalWordsNumber = 0;


	for (int i(0); i < (int)strlen(m_patientWrittenChar) + 1; i++)
	{
		// if it is a char : write the char
		if (m_patientWrittenChar[i] != ' ' && m_patientWrittenChar[i] != '$')
		{
			listPatientWords[originalWordsNumber][indiceForEachWord] = m_patientWrittenChar[i];
			indiceForEachWord ++;

		}
		// debut d'un nouveau mot
		if(m_patientWrittenChar[i-1] != ' ' &&  m_patientWrittenChar[i] == ' ' && m_patientWrittenChar[i+1] != ' ')
		{
			listPatientWords[originalWordsNumber][indiceForEachWord] = '\0';
			originalWordsNumber ++;
			indiceForEachWord = 0;
		}
		// at the end of original sentence :
		if (m_patientWrittenChar[i] == '\0')
		{
			listPatientWords[originalWordsNumber][indiceForEachWord] = '\0';
		}
	}

	if (choice != 10)
	{
		for (int i(0); i <= originalWordsNumber; i++)
		{
			cout << "2 : Word number : " << i << " is : " << listPatientWords[i] << endl;
		}
	}

	// compare the words
	int wordsWithMistakes = 0;

	for (int i(0); i <= originalWordsNumber; i++)
	{
		bool withoutMistake = false;

		for (int j(0); j <= originalWordsNumber; j++)
		{
			if(strcmp(listPatientWords[i], listOriginalWords[j]) == 0)
			{
				withoutMistake = true;
			}
		}

		if(withoutMistake == false)
		{
			wordsWithMistakes ++;
		}
	}
	if (choice != 10)
	{
		cout << "number of words with mistake = " << wordsWithMistakes << endl;
	}

	m_mistakes[1] = wordsWithMistakes;

	// --------------------------------------------------------------------------------------------------
	//The third one is linked to the mistakes of double letter.

	// // first part we take in memory the double letter of testresultchar :
	char initialDoubleLetter[20];
	int numDL = 0;

	for (int i(0); i < (int)strlen(m_askedSentence); i++)
	{
		if (m_askedSentence[i] == m_askedSentence[i+1])
		{
			initialDoubleLetter[numDL] = m_askedSentence[i];
			numDL++;
		}
	}

	initialDoubleLetter[numDL]='\0';

	// when we encounter a double letter in the allWrittenChar we check if this double letter is in initialDoubleLetter
	// if not there is a mistake

	int numMistake = 0;
	bool DL;

	for (int i(0); i < (int)strlen(m_patientWrittenChar); i++)
	{
		if (m_patientWrittenChar[i] == m_patientWrittenChar[i+1])
		{
			// we check in initialDoubleLetter
			// if no similar double letter are found there is a mistakes
			DL = true;
			for(int j(0); j < (int)strlen(initialDoubleLetter);j++)
			{
				if (m_patientWrittenChar[i] == initialDoubleLetter[j] )
				{
					//all go well
					DL = false;
				}
			}
			if (DL == true)
			{
				numMistake++;
			}
		}
	}
		if (numMistake > 10)
	{
		numMistake = 10;
	}
	m_mistakes[2] = numMistake;

}

char* KeyboardData::getAskedSentence()
{
	return m_askedSentence;
}

char* KeyboardData::getPatientWrittenChar()
{
	return m_patientWrittenChar;
}

short* KeyboardData::getTimeForChar()
{
	return m_timeForChar;
}

short* KeyboardData::getMistakes()
{
	return m_mistakes;
}

double* KeyboardData::getTabSpeed()
{
	return m_tabSpeed;
}

void KeyboardData::printKeyboardData()
{
	cout << "Verification that we have the data in our class" << endl;
	cout << "In myKeyboardData : Initial sentence " <<  m_askedSentence << endl;
	cout << "In myKeyboardData : All written char " <<  m_patientWrittenChar << endl;

	cout << "In myKeyboardData : Time Between char " << m_timeForChar[1] << " hundredths of a second." <<  endl;
	getchar();

	cout << "Now we apply the two analysis functions" << endl;

	// show tabSpeed
	cout << "the number of words is : " << m_tabSpeed[0] << endl;

	for (int i(1); i <= m_tabSpeed[0]; i++)
	{
		cout << "the word number : " << i << " has a speed of: " << m_tabSpeed[i] << " carac/s" << endl;
	}
	cout << "the adverage speed is : " << m_tabSpeed[(int)(m_tabSpeed[0] + 1)] << endl;

	getchar();

	// show tabAnalyze
	cout << "Number of erased characters : " << m_mistakes[0] << endl;
	cout << "Number of mistakes done at end of words : " << m_mistakes[1] << endl;
	cout << "Number of letter written twice in a row by mistake " << m_mistakes[2] << endl;
	getchar();
}

