#ifndef KEYBOARDDATA_H
#define KEYBOARDDATA_H

#include <string>

using namespace std;

// The class that collect information form the binary file send by the Apps groups.
class KeyboardData {
	public:
	//Destructor
	~KeyboardData();

	//Builder
	KeyboardData(int choice, string fileAddress, string askedSentence);
	KeyboardData(char* patientname);


	//Methods
	void listSpeed();
	void analyzeMistakes(int choice);
	char* getAskedSentence();
	char* getPatientWrittenChar();
	short* getTimeForChar();
	short* getMistakes();
	double* getTabSpeed();
	void printKeyboardData();

	//Attributes
	private:
	char* m_askedSentence;
	char* m_patientWrittenChar;
	short* m_timeForChar;
	double* m_tabSpeed;
	short* m_mistakes;
};

#endif // KEYBOARDDATA_H
