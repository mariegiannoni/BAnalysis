#include <mysql.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <string>
#include <time.h>
#include "auth.h"
#include "Patient.h"
#include "api.h"

using namespace std;

std::string PATHTOALEDAPP;

// Create the output file for the eye blinking test
int createOutputForm(MYSQL * con, int id_socdet, int std) ;

int main( int argc, char ** argv) {
	if (argc == 1) {
		std::cout << " 1str arg : '10' // 2nd arg : Patient ID // 3rd arg : PATHTOALEDAPP" << std::endl;
		std::cout << "This program will create an output in ALEDAPP/Analysis/FinalAnalysis/PreviousOutputs\n" << std::endl;
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

		// Creation of output analysis files for the form
		int create = createOutputForm(con, id_socdet, 0);
		
		// We close the connection
		mysql_close(con);
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

		// Creation of output analysis files for the form
		int create = createOutputForm(con, id_socdet, 1);
		
		// We close the connection
		mysql_close(con);
	}
	return 0;
}

int createOutputForm(MYSQL * con, int id_socdet, int std) {

	std::ostringstream a;
	a << id_socdet;
	std::string filename = PATHTOALEDAPP + "/ReceivedData/Patient" + a.str() + "/Patient" + a.str() + "F.txt";
    ifstream form (filename);
    //std::cout << filename << std::endl;

    FILE * f(NULL);

    f = fopen(filename.c_str(), "rt");

	if (f == NULL) {
		std::cout << "Impossible to open the file " << filename << " for writing" << std::endl;
		return 0;
	}
	
	int symptom1 = 0;
	int symptom2 = 0;
	int counter = 1;
	int value = 0;
	
	// We find the number of symptoms on the table using mysql_num_rows
	int nbSym = retrieveNbSymp(con);

	/* Create an array of short with the number of symptoms */
	short symptoms[(int)nbSym];

	// Initialisation of the array with 0
	for(int i(0); i < (int)nbSym; i++){
		symptoms[i] = 0;
	}

	while(!feof(f)) //While we are not at the end of the file, we keep reading
	{
		int read = fscanf(f, "%d\n", &value);
		if(read != 1)
			return 0;

		if(std == 1)
			std::cout << "value " <<  value  << " counter "  << counter << std::endl;
			
		counter = counter + 1;
		if (counter == 3){
			symptom1 = value;
		}
		else if (counter == 9){
			symptom2 = value;
		}
		else if (counter > 2 && counter < 9){
			symptoms[symptom1 -1] = symptoms[symptom1 -1] + value; // add criticality
		}
		else if (counter > 9){
			symptoms[symptom2 -1] = symptoms[symptom2 -1] + value; // add criticality
		}
	}
	if(std == 1) {
		for (int i(0); i < nbSym; i++)
		{
			if(symptoms[i] != 0)
				cout << "Symptom num " << i << " has crit : " << symptoms[i] << endl;
		}
	}

	// Creation of the file for the form
	// 2 => form
	int w = writeFileAnalyse(symptoms, nbSym, id_socdet, 2, PATHTOALEDAPP);
	if (w == 0) {
		std::cout << "Form file was not writing correctly\n" << std::endl;
		return 0;
	}
	else {
		//std::cout << "creation of : " + PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + a.str() + "/Patient_" + a.str() + "_F.txt" << std::endl;
		std::cout << ">>>Patient"+ a.str() +" --> Creation of : Patient_" + a.str() + "F.txt" << std::endl;
	}
	
	return 1;
}
