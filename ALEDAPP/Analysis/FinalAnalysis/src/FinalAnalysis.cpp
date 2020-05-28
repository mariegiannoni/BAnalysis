#include <mysql.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <string>
#include <time.h>
#include "auth.h"
#include "Patient.h"
#include "api.h"

std::string PATHTOALEDAPP;


// Create the output file for the eye blinking test
int createOutputEB(MYSQL * con, int id_socdet) ;

// Create the output file for the cardiac frequency test
int createOutputCF(MYSQL * con, int age, int id_socdet);

// Compare the cardiac frequency with the frequency to know if it is higher (2), lower (1) or normal (0)
int cardiacFrequencyComparator(float reference, float precision, float value);

// Detection of abnormal cardiac frequency
int cardiacFrequencyAnalysis(float cardiacFrequency, int age);

// Compute the global criticality
short computeGlobalCriticality(MYSQL * con, short * symptoms, int nbSym, Patient &patient);


int main( int argc, char ** argv) {
	if (argc == 1) {
		std::cout << " 1str arg : '10' // 2nd arg : Patient ID // 3rd arg : PATHTOALEDAPP" << std::endl;
		std::cout << "This program will create an output in ALEDAPP/EmittedData/\n" << std::endl;
	}

	// Classic mode
	if (argc == 4 && strcmp(argv[1], "10") == 0) {
		PATHTOALEDAPP = argv[3];
		std::string PatientID = argv[2];
		int id_socdet = atoi(PatientID.c_str());

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

		if (result == NULL) {
			// We have no result registered
			std::cout << "Error : " << mysql_error(con) << std::endl;
			mysql_close(con);
			exit(1);
		}

		unsigned long nb_result = 0;
		nb_result = mysql_num_rows(result);
		if (nb_result != 1) {
			// We have no result registered
			std::cout << "The id is not in the database " << mysql_error(con) << std::endl;
			mysql_close(con);
			exit(1);
		}

		// We find the number of symptoms on the table using mysql_num_rows
		int nbSym = retrieveNbSymp(con);

		/* Create an array of short with the number of symptoms */
		short symptoms[(int)nbSym];

		// Initialisation of the array with 0
		for(int i(0); i < (int)nbSym; i++){
			symptoms[i] = 0;
		}

		// Creation and filling of an object from the class Patient
		Patient patient = createPatient(con, id_socdet, 0);
		
		// Creation of output analysis files
		// for cardiac frequency
		int create = createOutputCF(con, patient.getAge(), patient.getIdSocdet());
		
		// for eye blinking
		create = createOutputEB(con, patient.getIdSocdet());

		// To merge al the analysis
		int r = mergeFile(symptoms, nbSym, id_socdet, PATHTOALEDAPP);
		if (r == 0) {
			std::cout << "The merge was not done correctly\n" << std::endl;
			// We close the connection
			mysql_close(con);
			exit(1);
		}

		// Computation of the global criticality
		short globalCriticality = computeGlobalCriticality(con, symptoms, nbSym, patient);
		
		// We close the connection
		mysql_close(con);
		
		unsigned long write = writeFileFinalAnalyse(symptoms, nbSym, globalCriticality, id_socdet, PATHTOALEDAPP, 0);

		if (write == 0){
			std::cout << "Impossible to create file" << std::endl;
			exit(1);
		}
		else {
			std::ostringstream a;
			a << write;
			//std::cout << "creation of : " + PATHTOALEDAPP + "/EmittedData/Patient_" + PatientID + "_" + a.str() + "_BehaviorAnalysis.txt" << std::endl;
			std::cout << ">>>Patient"+PatientID+" --> Creation of : Patient_" + PatientID + "_BehaviorAnalysis.bin" << std::endl;
		}

	}
	// Draft interface mode
	else if (argc == 4 && strcmp(argv[1], "1") == 0) {
		PATHTOALEDAPP = argv[3];
		std::string PatientID = argv[2];
		int id_socdet = atoi(PatientID.c_str());

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

		if (result == NULL) {
			// We have no result registered
			std::cout << "Error : " << mysql_error(con) << std::endl;
			mysql_close(con);
			exit(1);
		}

		unsigned long nb_result = 0;
		nb_result = mysql_num_rows(result);
		if (nb_result != 1) {
			// We have no result registered
			std::cout << "The id is not in the database " << mysql_error(con) << std::endl;
			mysql_close(con);
			exit(1);
		}
		std::cout << "The patient " << PatientID << " is in the database" << std::endl;

		std::cout << "Press enter to continue" << std::endl;
		getchar();

		// We find the number of symptoms on the table using mysql_num_rows
		std::cout << "Creation of the Eye Blinking Analysis file" << std::endl;
		int nbSym = retrieveNbSymp(con);
		std::cout << "# symptoms in the database : " << nbSym << std::endl;

		/* Create an array of short with the number of symptoms */
		short symptoms[(int)nbSym];

		// Initialisation of the array with 0
		for(int i(0); i < (int)nbSym; i++){
			symptoms[i] = 0;
		}

		std::cout << "Press enter to continue" << std::endl;
		getchar();

		// Creation and filling of an object from the class Patient
		std::cout << "Creation of the Patient" << std::endl;
		Patient patient = createPatient(con, id_socdet, 0);

		std::cout << "We display the patient : " << std::endl;

		patient.printPatient();
		std::cout << "Press enter to continue" << std::endl;
		getchar();
		
		// Creation of output analysis files
		// for cardiac frequency
		std::cout << "Creation of the Cardiac Frequency Analysis file" << std::endl;
		int create = createOutputCF(con, patient.getAge(), patient.getIdSocdet());

		if(create == 1)
			std::cout << "The Cardiac Frequency Analysis file was written in a file correctly" << std::endl;

		std::cout << "Press enter to continue" << std::endl;
		getchar();
		
		// for eye blinking
		std::cout << "Creation of the Eye Blinking Analysis file" << std::endl;
		create = createOutputEB(con, patient.getIdSocdet());

		if(create == 1)
			std::cout << "The Eye Blinking Analysis file was written in a file correctly" << std::endl;

		std::cout << "Press enter to continue" << std::endl;
		getchar();

		// To merge al the analysis
		int r = mergeFile(symptoms, nbSym, id_socdet, PATHTOALEDAPP);
		if (r == 0) {
			std::cout << "The merge was not done correctly\n" << std::endl;
			// We close the connection
			mysql_close(con);
			exit(1);
		}

		std::cout << "The merge was done correctly" << std::endl;
		std::cout << "Press enter to continue" << std::endl;
		getchar();

		// Computation of the global criticality
		short globalCriticality = computeGlobalCriticality(con, symptoms, nbSym, patient);
		std::cout << "Global criticality : " << globalCriticality << std::endl;
		std::cout << "Press enter to continue" << std::endl;
		getchar();
		// We close the connection
		mysql_close(con);
		
		unsigned long write = writeFileFinalAnalyse(symptoms, nbSym, globalCriticality, id_socdet, PATHTOALEDAPP, 1);

		if (write == 0){
			std::cout << "Impossible to create file" << std::endl;
			exit(1);
		}
		else {
			std::ostringstream a;
			a << write;
			//std::cout << "creation of : " + PATHTOALEDAPP + "/EmittedData/Patient_" + PatientID + "_" + a.str() + "_BehaviorAnalysis.txt" << std::endl;
			std::cout << ">>>Patient"+PatientID+" --> Creation of : Patient_" + PatientID + "_BehaviorAnalysis.txt" << std::endl;
		}

	}
	return 0;
}

// Compute the global criticality
// 1. Compute the average
// 2. Adjust the average accoridng to the age, the weight and the number of symptoms
short computeGlobalCriticality(MYSQL * con, short * symptoms, int nbSym, Patient &patient) {
	// number of non null symptoms
	short nb = nbSymptomNonNull(symptoms, nbSym);
	if(nb == 0) {
		return 0;
	}

	// mean of non null symptoms
	short mean = 0;
	
	for(int i(0); i < nbSym; i++) {
		if(symptoms[i] != 0) {
			mean = mean + symptoms[i];
		}
	}
	mean = mean / nb;

	// Influence of the age
	int age = patient.getAge();
	if(age < 10) {
		mean = mean + 0.1 * mean;
	}
	else if(age > 60) {
		mean = mean + 0.2 * mean;
	}

	// Influence of the genetic diseases
	int nbGen = patient.getNbGenetic();
	if(nbGen != 0) {
		mean = mean + nbGen;
	}

	// Influence of the allergies
	int nbAll = patient.getNbAllergy();
	if(nbAll != 0) {
		mean = mean + nbAll;
	}

	// Influence of the weight
	int id =  getIdSymptom(con, "overweight");
	if(id != -1) {
		if(symptoms[id] != 0)
			mean = mean + 0.15 * symptoms[id];
	}

	id =  getIdSymptom(con, "anorexia");
	if(id != -1) {
		if(symptoms[id] != 0)
			mean = mean + 0.15 * symptoms[id];
	}

	id =  getIdSymptom(con, "underweight");
	if(id != -1) {
		if(symptoms[id] != 0)
			mean = mean + 0.1 * symptoms[id];
	}

	// Influence of the number of symptoms
	if(nb >= 2  && nb < 5) {
		mean = mean + 0.05 * mean;
	}
	else if(nb >= 5  && nb < 10) {
		mean = mean + 0.1 * mean;
	}
	else if(nb >= 10  && nb < 20) {
		mean = mean + 0.15 * mean;
	}
	else if(nb >= 20 && nb < 30) {
		mean = mean + 0.2 * mean;
	}
	else {
		mean = mean + 0.25 * mean;
	}

	// we normalize the global criticality
	if(mean > 100) {
		mean = 100;
	}
	
	return mean;
}

// Create the output of the eye blinking
int createOutputEB(MYSQL * con, int id_socdet) {
	std::ostringstream a;
    a << id_socdet;
    
	// Reading 
	std::string pathToOutput = PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + a.str() + "/Patient_" + a.str() + "_BlinkFrequency.txt";

	FILE * f = fopen(pathToOutput.c_str(), "rt");


	if (f == NULL) {
		std::cout << "BlinkFrequency.txt for Patient" << id_socdet << " not available." << std::endl;
		return 0;
	}
	
	int blinkFrequency = 0.0;
	int read = fscanf(f, "%d", &blinkFrequency);
	fclose(f);

	if (read != 1){
		return 0;
	}
	
	// We find the number of symptoms on the table using mysql_num_rows
	int nbSym = retrieveNbSymp(con);

	/* Create an array of short with the number of symptoms */
	short symptoms[(int)nbSym];

	// Initialisation of the array with 0
	for(int i(0); i < (int)nbSym; i++){
		symptoms[i] = 0;
	}

	short id = 0;

	if (blinkFrequency < 1.6)
	{
		id = getIdSymptom(con, "fatigue");
		if(id != -1)
			symptoms[id] = 15;

		id = getIdSymptom(con, "fever");
		if(id != -1)
			symptoms[id] = 15;

		id = getIdSymptom(con, "vertigo");
		if(id != -1)
			symptoms[id] = 15;
	}
	else if (blinkFrequency > 3.3)
	{
		id = getIdSymptom(con, "worry");
		if(id != -1)
			symptoms[id] = 15;

		id = getIdSymptom(con, "dizziness");
		if(id != -1)
			symptoms[id] = 15;

		id = getIdSymptom(con, "tremor");
		if(id != -1)
			symptoms[id] = 15;

	}

	// Creation of the file for the blink frequency
	// 4 => blink frequency
	int w = writeFileAnalyse(symptoms, nbSym, id_socdet, 4, PATHTOALEDAPP);
	if (w == 0) {
		std::cout << "Patient file was not writing correctly\n" << std::endl;
		return 0;
	}
	else {
		//std::cout << "creation of : " + PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + a.str() + "/Patient_" + a.str() + "_EB.txt" << std::endl;
		std::cout << ">>>Patient"+a.str() +" --> Creation of : Patient_" + a.str()  + "_EB.txt" << std::endl;
	}
	return 1;
}

// Create the output file for the cardiac frequency test
int createOutputCF(MYSQL * con, int age, int id_socdet) {
	std::ostringstream a;
    a << id_socdet;

	// Reading of the cardiac frequency
	std::string pathToOutput = PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + a.str() + "/Patient_" + a.str() + "_CardiacFrequency.txt";

	FILE * f = fopen(pathToOutput.c_str(), "rt");

	if (f == NULL) {
		std::cout << "CardiacFrequency.txt for Patient" << id_socdet << " not available." << std::endl;
		return 0;
	}
		float cardiacFrequency = 0.0;
		int read = fscanf(f, "%f", &cardiacFrequency);
		fclose(f);
		
		if (read != 1){
			return 0;
		}
	// Analysis of cardiac frequency
	if(cardiacFrequency != 0.0) {
		int result = cardiacFrequencyAnalysis(cardiacFrequency, age);

		// We find the number of symptoms on the table using mysql_num_rows
		int nbSym = retrieveNbSymp(con);

		/* Create an array of short with the number of symptoms */
		short symptoms[(int)nbSym];

		// Initialisation of the array with 0
		for(int i(0); i < (int)nbSym; i++){
			symptoms[i] = 0;
		}
		
		short id = 0;
		short result2 = 0;
		if (result == 1) {
			if(age < 5) {
				result2 = 25; // 25
			}
			else if (age >= 5 && age < 25) {
				result2 = 10; // 10
			}
			else if (age >= 25 && age < 40) {
				result2 = 15; // 15
			}
			else if (age >= 40 && age < 60) {
				result2 = 20; // 20
			}
			else if (age >= 60 && age < 80) {
				result2 = 50; // 50
			}
			else if(age >= 80) {
				result2 = 60; // 60
			}
			id = getIdSymptom(con, "bradycardia");
			if(id != -1)
				symptoms[id] = result2;
		}
		else if (result == 2) {
			if(age < 5) {
				result2 = 30; // 30
			}
			else if (age >= 5 && age < 25) {
				result2 = 5; // 5
			}
			else if (age >= 25 && age < 40) {
				result2 = 10; // 10
			}
			else if (age >= 40 && age < 60) {
				result2 = 20; // 20
			}
			else if (age >= 60 && age < 80) {
				result2 = 40; // 40
			}
			else if(age >= 80) {
				result2 = 55; // 55
			}
			id = getIdSymptom(con, "palpitation");
			if(id != -1)
				symptoms[id] = result2;
		}
		else if (result == 3) {
			if(age < 5) {
				result2 = 40; // 40
			}
			else if (age >= 5 && age < 25) {
				result2 = 15; // 15
			}
			else if (age >= 25 && age < 40) {
				result2 = 20; // 20
			}
			else if (age >= 40 && age < 60) {
				result2 = 25; // 25
			}
			else if (age >= 60 && age < 80) {
				result2 = 60; // 60
			}
			else if(age >= 80) {
				result2 = 80; // 80
			}

			id = getIdSymptom(con, "palpitation");
			if(id != -1)
				symptoms[id] = result2;

			id = getIdSymptom(con, "cardiovascular_event");
			if(id != -1)
				symptoms[id] = result2 / 2;
		}

		// Creation of the file for the cardiac frequency
		// 5 => cardiac frequency
		int w = writeFileAnalyse(symptoms, nbSym, id_socdet, 5, PATHTOALEDAPP);
		if (w == 0) {
			std::cout << "Patient file was not writing correctly\n" << std::endl;
			return 0;
		}
		else {
			//std::cout << "creation of : " + PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + a.str() + "/Patient_" + a.str() + "_CF.txt" << std::endl;
			std::cout << ">>>Patient"+a.str()+" --> Creation of : Patient_" + a.str() + "_CF.txt" << std::endl;
		}
	}
	return 1;
}

// Compare the cardiac frequency with the frequency to know if it is higher (2), lower (1) or normal (0)
int cardiacFrequencyComparator(float reference, float precision, float value){
	if (value > reference + precision)
		return 1;
	else if (value < reference - precision)
		return 2;
	else
		return 0;
}

// Detection of abnormal cardiac frequency
// extremal tachycardia : 3
// tachycardia : 2
// bradycardia : 1
// no arrythmia (normal): 0
int cardiacFrequencyAnalysis(float cardiacFrequency, int age) {
	int result = 0;

	if (cardiacFrequency >= 220 - age) {
		// the cardiac frequency is equal or higher to the maximal cardiac frequency that is allowed
		result = 3;
	}
	else {
		// the reference are mean rest cardiac frequency, the precision is the standard variation divided by 2
		if (age < 1) {
			result = cardiacFrequencyComparator(140, 50, cardiacFrequency);
		}
		else if (age >= 1 && age <= 2) {
			result = cardiacFrequencyComparator(110, 40, cardiacFrequency);
		}
		else if (age >= 3 && age <= 5) {
			result = cardiacFrequencyComparator(105, 35, cardiacFrequency);
		}
		else if (age >= 6 && age <= 12) {
			result = cardiacFrequencyComparator(95, 30, cardiacFrequency);
		}
		else if (age >= 13 && age <= 60) {
			result = cardiacFrequencyComparator(80, 25, cardiacFrequency);
		}
		else if (age > 60) {
			result = cardiacFrequencyComparator(65, 20, cardiacFrequency);
		}
	}
	return result;
}
