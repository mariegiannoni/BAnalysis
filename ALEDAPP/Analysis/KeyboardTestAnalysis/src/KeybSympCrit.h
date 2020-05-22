#ifndef KEYBSYMPCRIT_H
#define KEYBSYMPCRIT_H

using namespace std;

// This class contains a list of symptoms and a list of their criticality level.
class KeybSympCrit {
	public:
	//Destructor
	~KeybSympCrit();

	//Builder
	KeybSympCrit(double* speeds, short* analyze);

	//Methods
	short* getSymptoms();
	short* getSympCrit();
	void printKeybSympCrit();
	void createOutputAnalysis(string PatientID, string PATHTOALEDAPP);

	//Attributes
	private:
	short* m_symptoms;
	short* m_sympCrit;
};

#endif // KEYBSYMPCRIT_H
