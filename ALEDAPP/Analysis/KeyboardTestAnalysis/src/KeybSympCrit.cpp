#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <mysql.h>
#include "KeybSympCrit.h"
#include "api.h"
#include "auth.h"

using namespace std;




//Destructor
KeybSympCrit::~KeybSympCrit()
{
	if(m_symptoms != NULL)
		delete [] m_symptoms;

	if(m_sympCrit != NULL)
		delete [] m_sympCrit;

}

//Builder
//This function analyzes the input and returns a list with symptoms and their criticality level: keybSympCrit.
KeybSympCrit::KeybSympCrit (double *tabSpeed, short* analyze)
{
	// Symptom that we can give in this function :
	// 1 : Anxiety - >  if answers are given too fast
	short anxiety(0);
	// 11. Fatigue  - >  if answers come to slow
	short fatigue(0);
	// 11. Fatigue / 14. Headache - >  if answers come with mistakes
	short headache(0);
	// 21. Tremor - >  if answers come with doubles letters and backspace
	short tremor(0);
	// 22. Vertigo - >  too many mistakes
	short vertigo(0);


	// analyze speed :
	// reminder speed structure:
	// on the first we put the numbers of words.
	// on the 1 to n blocks of the tab, the 1 to n speed in fct of the word.
	// on the last one we put the total speed adverage.

	// we cast the number of words from double to int
	int numberOfWords;
	numberOfWords = (int) tabSpeed[0];

	if (tabSpeed[numberOfWords + 1] >= 0 && tabSpeed[numberOfWords + 1] <= 1)
	{
		headache = headache + 3;
		fatigue = fatigue + 3;
		vertigo = vertigo + 3;
	}
	if (tabSpeed[numberOfWords + 1] > 1 && tabSpeed[numberOfWords + 1] <= 2)
	{
		headache = headache + 2;
		fatigue = fatigue + 2;
		vertigo = vertigo + 2;
	}
	if (tabSpeed[numberOfWords + 1] > 2 && tabSpeed[numberOfWords + 1] <= 4)
	{
		// all go well
	}
	if (tabSpeed[numberOfWords + 1] > 4 && tabSpeed[numberOfWords + 1] <= 5)
	{
		anxiety = anxiety + 2;
	}
	if (tabSpeed[numberOfWords + 1] > 5)
	{
		anxiety = anxiety + 4;
	}

	// analyze of analyzeMistakes
	// Reminder :
	//analyzeMistakes returns a list of short int between 0 and 10.
	//The first one represents the quantity of erased characters.
	//The second one is linked to the mistakes done by words.
	//The third one is linked to the mistakes of double consonants.

	if (analyze[0] >=0 && analyze[0] <= 2)
	{
	// all go well
	}
	if (analyze[0] > 2 && analyze[0] <= 5)
	{
		headache = headache + 2;
		fatigue = fatigue + 2;
		vertigo = vertigo + 2;
	}
	if (analyze[0] > 5 && analyze[0] <= 10)
	{
		headache = headache + 3;
		fatigue = fatigue + 3;
		vertigo = vertigo + 3;
	}


	if (analyze[0] >= 0 && analyze[0] <= 2)
	{
	// all go well
	}
	if (analyze[0] > 2 && analyze[0] <= 5)
	{
		headache = headache + 2;
		fatigue = fatigue + 2;
		vertigo = vertigo + 2;
	}
	if (analyze[0] > 5 && analyze[0] <= 10)
	{
		headache = headache + 3;
		fatigue = fatigue + 3;
		vertigo = vertigo + 3;
	}


	if (analyze[2] >= 0 && analyze[2] <= 1)
	{
	// all go well
	}
	if (analyze[2] > 1 && analyze[2] <= 5)
	{
		headache = headache + 2;
		fatigue = fatigue + 2;
		vertigo = vertigo + 2;
		tremor = tremor + 2;
	}
	if (analyze[2] > 5 && analyze[2] <= 10)
	{
		headache = headache + 3;
		fatigue = fatigue + 3;
		vertigo = vertigo + 3;
		tremor = tremor + 2;
	}

	// creation of the attributes
	m_symptoms = new short[5];

	m_symptoms[0] =(short)1;
	m_symptoms[1] =(short)11;
	m_symptoms[2] =(short)14;
	m_symptoms[3] =(short)21;
	m_symptoms[4] =(short)22;

	m_sympCrit = new short[5];
	m_sympCrit[0] = anxiety*10;
	m_sympCrit[1] = fatigue*10;
	m_sympCrit[2] = headache*10;
	m_sympCrit[3] = tremor*10;
	m_sympCrit[4] = vertigo*10;

}

//Getter
short* KeybSympCrit::getSymptoms()
{
	return m_symptoms;
}

short* KeybSympCrit::getSympCrit()
{
	return m_sympCrit;
}

void KeybSympCrit::printKeybSympCrit()
{
	for (int i(0); i < 5; i++)
    {
    	cout << "The symptom number " << m_symptoms[i] << " has a criticality level of : " << m_sympCrit[i] << endl;
    }

}

void KeybSympCrit::createOutputAnalysis(string PatientID, string PATHTOALEDAPP)
{
	/* CONNEXION TO THE API TO GET THE ID OF THE SYMPTOMS */

    MYSQL *con = mysql_init(NULL);

	if (con == NULL) {
		// The initialisation has failed
		std::cout << "Error : " << mysql_error(con) << std::endl;
		exit(1);
	}

	// We read the password in the binary file
		char *password = readPassword("/home/isen/project_big_data/CAD/CAD_project/data/auth/key.bin");

		if(mysql_real_connect(con, "localhost", "non-root", password, "projet", 0, NULL, 0) == NULL) {			// The connection to the database has failed
			std::cout << "Error : " << mysql_error(con) << std::endl;
			mysql_close(con);
			exit(1);
		}

		// We clean and free the password
		cleanAndFreePassword(&password);

		// We check that the id of the patient is in the bdd
		std::string query = "SELECT * FROM social_details WHERE id_socdet='" + PatientID + "'";
		if (mysql_query(con, query.c_str())) {
			// We can't retrieve the information we want from the db
			std::cout << "Error : " << mysql_error(con) << std::endl;
			mysql_close(con);
			exit(1);
		}

		// We get the result set
		MYSQL_RES * result = mysql_store_result(con);

	// We find the number of symptoms on the table using mysql_num_rows
	int nbSym = retrieveNbSymp(con);

	/* Create an array of short with the number of symptoms */
	short symptoms[(int)nbSym];

	// Initialisation of the array with 0
	for(int i(0); i < (int)nbSym; i++){
		symptoms[i] = 0;
	}

	short id = 0;

	// anxiety -> worry
	id =  getIdSymptom(con, "worry");
		if(id != -1)
			symptoms[id] = m_sympCrit[0];

	// fatigue
	id =  getIdSymptom(con, "fatigue");
		if(id != -1)
			symptoms[id] = m_sympCrit[1];

	// headache -> chest_pain and incoherent
	id =  getIdSymptom(con, "incoherent");
		if(id != -1)
			symptoms[id] = m_sympCrit[3];

	id =  getIdSymptom(con, "pain_chest");
		if(id != -1)
			symptoms[id] = m_sympCrit[3];

	// tremor
	id =  getIdSymptom(con, "tremor");
		if(id != -1)
			symptoms[id] = m_sympCrit[3];

	// vertigo
	id =  getIdSymptom(con, "vertigo");
		if(id != -1)
			symptoms[id] = m_sympCrit[3];

	// We close the connection
	mysql_close(con);

	// If the directory does not exit, we create it
    // Creation of the file for the mouse test
	// 0 => mouse test
	int id_socdet = atoi(PatientID.c_str());
   	int w = writeFileAnalyse(symptoms, nbSym, id_socdet, 1, PATHTOALEDAPP);
	if (w == 0) {
		std::cout << "Keyboard test file was not writing correctly\n" << std::endl;
	}
	else {
		std::cout << ">>>Patient"+ PatientID +" --> Creation of : Patient_" + PatientID + "_KB.txt" << std::endl;
		//std::cout << "creation of : " + PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + PatientID + "/Patient_" + PatientID + "_KB.txt" << std::endl;
	}

}
