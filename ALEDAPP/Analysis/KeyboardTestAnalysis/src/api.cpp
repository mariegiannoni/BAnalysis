#include <mysql.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <string>
#include <string>
#include <time.h>
#include "api.h"
#include "Patient.h"
#include "Date.h"

/* SYMPTOMS */

// To get the number of symptoms in the database
int retrieveNbSymp(MYSQL *con){
	// We execute the query
	if (mysql_query(con, "SELECT * FROM symptom")) {
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

	// We find the number of symptoms on the table using mysql_num_rows
	unsigned long nbSym = (unsigned long)mysql_num_rows(result);

	// We free the result set
	mysql_free_result(result);

	return (int)nbSym;
}

// To retrieve the id of a symptom in the database
short getIdSymptom(MYSQL *con, std::string symptom){
	// We execute the query
	std::string query = "SELECT id_symp FROM symptom WHERE name='" + symptom + "'";
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

	// We get the row that contain the result
	MYSQL_ROW row = NULL;
	unsigned int num_fields = 0;

	num_fields = mysql_num_fields(result);
	if (num_fields == 0) {
		// We have no result registered
		std::cout << "Error : " << mysql_error(con) << std::endl;
		mysql_close(con);
		exit(1);
	}

	// We only retrieve the first match row
	row = mysql_fetch_row(result);
	std::ostringstream out;
	int idSym = -1;

	// If the row is non NULL, we convert the result into a number
	if(row[0]) {
		out << row[0];
		std::string str = out.str();
		idSym = atoi(str.c_str());
	}

	// If the result is out of minimal bound
	if (idSym < 1)
		idSym = -1;

	// We free the result set
	mysql_free_result(result);

	return idSym - 1;
}


/* PATIENT FILE */

// Count the number of allergy
int retrieveNbAllergy(MYSQL *con, int id_medrec){
	// We execute the query
	std::ostringstream a;
    a << id_medrec;
	std::string query = "SELECT id_allergy FROM is_allergy WHERE id_medrec='" + a.str() + "'";

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

	// We find the number of symptoms on the table using mysql_num_rows
	unsigned long nbAllergy = (unsigned long)mysql_num_rows(result);

	// We free the result set
	mysql_free_result(result);

	return (int)nbAllergy;
}


// Count the number of genetic diseases
int retrieveNbGenetic(MYSQL *con, int id_medrec){
	// We execute the query
	std::ostringstream a;
    a << id_medrec;
	std::string query = "SELECT id_gen FROM is_genetic WHERE id_medrec='" + a.str() + "'";

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

	// We find the number of symptoms on the table using mysql_num_rows
	unsigned long nbGenetic = (unsigned long)mysql_num_rows(result);

	// We free the result set
	mysql_free_result(result);

	return (int)nbGenetic;
}

// retreive the gender of the patient
int retrieveGender(MYSQL *con, int id_socdet) {
	std::ostringstream a;
    a << id_socdet;
	// We execute the query
	std::string query = "SELECT gender FROM social_details WHERE id_socdet='" + a.str() + "'";
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

	// We get the row that contain the result
	MYSQL_ROW row = NULL;
	unsigned int num_fields = 0;

	num_fields = mysql_num_fields(result);
	if (num_fields == 0) {
		// We have no result registered
		std::cout << "Error : " << mysql_error(con) << std::endl;
		mysql_close(con);
		exit(1);
	}

	// We only retrieve the first match row
	row = mysql_fetch_row(result);
	std::ostringstream out;
	int gender = -1;

	// If the row is non NULL, we convert the result into a number
	if(row[0]) {
		out << row[0];
		std::string str = out.str();
		gender = atoi(str.c_str());
	}

	// If the result is out of minimal bound
	if (gender != 0 && gender != 1)
		gender = -1;

	// We free the result set
	mysql_free_result(result);

	return gender;
}

// retreive the path of the picture of the patient
std::string retrievePicturePath(MYSQL *con, int id_socdet) {
	std::ostringstream a;
    a << id_socdet;
	// We execute the query
	std::string query = "SELECT picture FROM social_details WHERE id_socdet='" + a.str() + "'";
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

	// We get the row that contain the result
	MYSQL_ROW row = NULL;
	unsigned int num_fields = 0;

	num_fields = mysql_num_fields(result);
	if (num_fields == 0) {
		// We have no result registered
		std::cout << "Error : " << mysql_error(con) << std::endl;
		mysql_close(con);
		exit(1);
	}

	// We only retrieve the first match row
	row = mysql_fetch_row(result);
	std::ostringstream out;
	std::string picture = "none";

	// If the row is non NULL, we convert the result into a number
	if(row[0]) {
		out << row[0];
		picture = out.str();
	}

	// If the result is out of minimal bound
	if (picture.compare("") == 0)
		picture = "none";

	// We free the result set
	mysql_free_result(result);

	return picture;
}

// retreive the id of the medical record of the patient
int retrieveIdMedrec(MYSQL *con, int id_socdet) {
	std::ostringstream a;
    a << id_socdet;
	// We execute the query
	std::string query = "SELECT id_medrec FROM social_details WHERE id_socdet='" + a.str() + "'";
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

	// We get the row that contain the result
	MYSQL_ROW row = NULL;
	unsigned int num_fields = 0;

	num_fields = mysql_num_fields(result);
	if (num_fields == 0) {
		// We have no result registered
		std::cout << "Error : " << mysql_error(con) << std::endl;
		mysql_close(con);
		exit(1);
	}

	// We only retrieve the first match row
	row = mysql_fetch_row(result);
	std::ostringstream out;
	int id_medrec = -1;

	// If the row is non NULL, we convert the result into a number
	if(row[0]) {
		out << row[0];
		std::string str = out.str();
		id_medrec = (short)atoi(str.c_str());
	}

	// If the result is out of minimal bound
	if (id_medrec <= 0)
		id_medrec = -1;

	// We free the result set
	mysql_free_result(result);

	return id_medrec;
}

// retrieve the height of the patient
int retrieveHeight(MYSQL *con, int id_medrec) {
	std::ostringstream a;
    a << id_medrec;
	// We execute the query
	std::string query = "SELECT height FROM patient_medical_record WHERE id_medrec='" + a.str() + "'";
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

	// We get the row that contain the result
	MYSQL_ROW row = NULL;
	unsigned int num_fields = 0;

	num_fields = mysql_num_fields(result);
	if (num_fields == 0) {
		// We have no result registered
		std::cout << "Error : " << mysql_error(con) << std::endl;
		mysql_close(con);
		exit(1);
	}

	// We only retrieve the first match row
	row = mysql_fetch_row(result);
	std::ostringstream out;
	int height = -1;

	// If the row is non NULL, we convert the result into a number
	if(row[0]) {
		out << row[0];
		std::string str = out.str();
		height = atoi(str.c_str());
	}

	// If the result is out of minimal bound
	if (height <= 0)
		height = -1;

	// We free the result set
	mysql_free_result(result);

	return height;
}

// retrieve the weight of the patient
int retrieveWeight(MYSQL *con, int id_medrec) {
	std::ostringstream a;
    a << id_medrec;
	// We execute the query
	std::string query = "SELECT weight FROM patient_medical_record WHERE id_medrec='" + a.str() + "'";
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

	// We get the row that contain the result
	MYSQL_ROW row = NULL;
	unsigned int num_fields = 0;

	num_fields = mysql_num_fields(result);
	if (num_fields == 0) {
		// We have no result registered
		std::cout << "Error : " << mysql_error(con) << std::endl;
		mysql_close(con);
		exit(1);
	}

	// We only retrieve the first match row
	row = mysql_fetch_row(result);
	std::ostringstream out;
	int weight = -1;

	// If the row is non NULL, we convert the result into a number
	if(row[0]) {
		out << row[0];
		std::string str = out.str();
		weight = atoi(str.c_str());
	}

	// If the result is out of minimal bound
	if (weight <= 0)
		weight = -1;

	// We free the result set
	mysql_free_result(result);

	return weight;
}


// retrieve the blood groop id of the patient
int retrieveBloodGroup(MYSQL *con, int id_medrec) {
	std::ostringstream a;
    a << id_medrec;
	// We execute the query
	std::string query = "SELECT id_blgrp FROM patient_medical_record WHERE id_medrec='" + a.str() + "'";
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

	// We get the row that contain the result
	MYSQL_ROW row = NULL;
	unsigned int num_fields = 0;

	num_fields = mysql_num_fields(result);
	if (num_fields == 0) {
		// We have no result registered
		std::cout << "Error : " << mysql_error(con) << std::endl;
		mysql_close(con);
		exit(1);
	}

	// We only retrieve the first match row
	row = mysql_fetch_row(result);
	std::ostringstream out;
	int id_blgrp = -1;

	// If the row is non NULL, we convert the result into a number
	if(row[0]) {
		out << row[0];
		std::string str = out.str();
		id_blgrp = atoi(str.c_str());
	}

	// If the result is out of minimal bound
	if (id_blgrp <= 0 || id_blgrp > 8)
		id_blgrp = -1;

	// We free the result set
	mysql_free_result(result);

	return id_blgrp;
}

// retrieve the id of the allergy
void retrieveAllergy(MYSQL *con, int id_medrec, int nbAllergy, short * allergy) {
	std::ostringstream a;
    a << id_medrec;
	// We execute the query
	std::string query = "SELECT id_allergy FROM is_allergy WHERE id_medrec='" + a.str() + "'";
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

	// We get the row that contain the result
	MYSQL_ROW row = NULL;
	unsigned int num_rows = 0;

	num_rows = mysql_num_rows(result);
	if (num_rows != (unsigned int)nbAllergy) {
		// We have no result registered
		std::cout << "Error : " << mysql_error(con) << std::endl;
		mysql_close(con);
		exit(1);
	}
	int i(0);
	// If the row is non NULL, we convert the result into a number
	while ((row = mysql_fetch_row(result)) && i < nbAllergy) {
		for(unsigned int j(0); j < num_rows; j++) {
			std::ostringstream out;
			out << row[j];
			std::string str = out.str();
			if(str.compare("") != 0) {
				allergy[i] = atoi(str.c_str());
				i++;
			}
		}
	}

	// We free the result set
	mysql_free_result(result);
}


// retrieve the id of the genetic disease
void retrieveGenetic(MYSQL *con, int id_medrec, int nbGenetic, short * genetic) {
	std::ostringstream a;
    a << id_medrec;
	// We execute the query
	std::string query = "SELECT id_gen FROM is_genetic WHERE id_medrec='" + a.str() + "'";
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

	// We get the row that contain the result
	MYSQL_ROW row = NULL;
	unsigned int num_rows = 0;

	num_rows = mysql_num_rows(result);
	if (num_rows != (unsigned int)nbGenetic) {
		// We have no result registered
		std::cout << "Error : " << mysql_error(con) << std::endl;
		mysql_close(con);
		exit(1);
	}

	int i(0);
	while ((row = mysql_fetch_row(result)) && i < nbGenetic) {
		for(unsigned int j(0); j < num_rows; j++) {
			std::ostringstream out;
			out << row[j];
			std::string str = out.str();
			if(str.compare("") != 0) {
				genetic[i] = atoi(str.c_str());
				i++;
			}
		}
	}

	// We free the result set
	mysql_free_result(result);
}


// retrieve the birthdate of the patient
std::string retrieveBirthdate(MYSQL *con, int id_socdet) {
	std::ostringstream a;
    a << id_socdet;
	// We execute the query
	std::string query = "SELECT birthdate FROM social_details WHERE id_socdet='" + a.str() + "'";
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

	// We get the row that contain the result
	MYSQL_ROW row = NULL;
	unsigned int num_fields = 0;

	num_fields = mysql_num_fields(result);
	if (num_fields == 0) {
		// We have no result registered
		std::cout << "Error : " << mysql_error(con) << std::endl;
		mysql_close(con);
		exit(1);
	}

	// We only retrieve the first match row
	row = mysql_fetch_row(result);
	std::ostringstream out;
	std::string date;

	// If the row is non NULL, we convert the result into a number
	if(row[0]) {
		out << row[0];
		date = out.str();
	}

	// If the result is out of empty
	if (date.compare("") == 0 || date.size() != 10)
		date = "none";

	// We free the result set
	mysql_free_result(result);

	return date;
}

// Convert a string containing a date with the format "aaaa-mm-dd" to an object of the class Date
Date stringToDate(std::string str_date){
	Date date;
	size_t size = str_date.size() + 1;

	char * cpy_str_date = new char[size];
	strncpy(cpy_str_date, str_date.c_str(), size);

	// year
	char * str_year = new char[5];
	str_year[0] = cpy_str_date[0];
	str_year[1] = cpy_str_date[1];
	str_year[2] = cpy_str_date[2];
	str_year[3] = cpy_str_date[3];
	str_year[4] = '\0';

	// month
	char * str_month = new char[3];
	str_month[0] = cpy_str_date[5];
	str_month[1] = cpy_str_date[6];
	str_month[3] = '\0';

	// year
	char * str_day = new char[3];
	str_day[0] = cpy_str_date[8];
	str_day[1] = cpy_str_date[9];
	str_day[3] ='\0';

	date.setYear(atoi(str_year));
	date.setMonth(atoi(str_month));
	date.setDay(atoi(str_day));

	delete [] cpy_str_date;
	delete [] str_year;
	delete [] str_month;
	delete [] str_day;

	return date;
}


/* FINAL ANALYSIS */

// Returns the number of symptoms detected
short nbSymptomNonNull(short * symptoms, int nbSym) {
	int nb = 0;
	for(int i(0); i < nbSym; i++) {
		if(symptoms[i] != 0)
			nb++;
	}
	return nb;
}


// Fill the symptoms with random values
int fillRandomly(short * symptoms, int nbSym) {
	int nb = 0;
	int finish = 0;
	float number = 0;
	short id = 0;
	short criticality = 0;
	srand(time(NULL));
	number = (float)rand() / (float)(nbSym - 1);

	nb = (int) number;

	if (nb == 0) {
		return 1;
	}

	while (finish < nb) {
		srand(time(NULL));
		number = (float)rand() / (float)(nbSym - 1);
		id = (int) number;

		srand(time(NULL));
		number = (float)rand() / 100.0;
		criticality = (int) number;

		if (symptoms[id] == 0) {
			symptoms[id] = criticality;
			finish ++;
		}
	}

	return 1;
}

// To read the final analyse file
// Return 0 if a problem occured, 1 otherwise
int readFileFinalAnalyse(int id_socdet, unsigned long timestamp, std::string pathToAled){
	FILE * f(NULL);

	std::cout << "Reading of the final_analyse file" << std::endl;

	// filename : Patient_id_socdet_timestamp_BehaviorAnalysis.bin
	// exemple : id_socdet = 310 and timestamp = 1589293516 then Patient_310_1589293516_BehaviorAnalysis.bin is the filename
	std::ostringstream a;
    a << id_socdet;
    std::ostringstream b;
    b << timestamp;
	std::string filename = pathToAled + "/EmittedData/" + "Patient_" + a.str() + "_" + b.str() + "_BehaviorAnalysis.bin";
	std::cout << filename << std::endl;

	// Opening of the binary file using the filename
	f = fopen(filename.c_str(), "r+b");

	if (f == NULL) {
		std::cout << "Impossible to open the file for reading" << std::endl;
		return 0;
	}

	// 100 (short)
	short hundred = 0;
	size_t size = fread(&hundred, sizeof(short), 1, f);
	if (size != 1) {
		return 0;
	}
	else {
		std::cout << "hundred : " << hundred << std::endl;
	}

	// timestamps(unsigned long)
	unsigned long timestamp_now = 0;
	size = fread(&timestamp_now, sizeof(unsigned long), 1, f);
	if (size != 1) {
		return 0;
	}
	else {
		std::cout << "timestamp :" << timestamp_now << std::endl;
	}

	// id_socdet(int)
	int id_soc = 0;
	size = fread(&id_soc, sizeof(int), 1, f);
	if (size != 1) {
		return 0;
	}
	else {
		std::cout << "id_socdet : " << id_soc << std::endl;
	}

	// global criticality(short)
	short globalCriticality = 0;
	size = fread(&globalCriticality, sizeof(short), 1, f);
	if (size != 1) {
		return 0;
	}
	else {
		std::cout << "global criticity : " << globalCriticality << std::endl;
	}

	// nb_symptoms(int)
	short nb = 0;
	size = fread(&nb, sizeof(short), 1, f);
	if (size != 1) {
		return 0;
	}
	else {
		std::cout << "nb : " << nb << std::endl;
	}

	// id_symptom(short) criticality(short)
	for(short i(0); i < nb; i++) {
		// id_symptom (short)
		short id = 0;
		size = fread(&id, sizeof(short), 1, f);
		if (size != 1) {
			return 0;
		}
		else {
			// criticality (short)
			short criticality = 0;
			size = fread(&criticality, sizeof(short), 1, f);
			if (size != 1) {
				return 0;
			}
			else {
				std::cout << "id : " << id << " criticality : " << criticality << std::endl;
			}
		}
	}

	// Closing of the file
	fclose(f);

	return 1;
}

// To read the analyse file : reading of the txt file for the intermediary test
// choice = 0 : mouse test
// choice = 1 : keyboard test
// choice = 2 : form
// choice = 3 : patient file
// choice = 4 : eye blinking
// choice = 5 : cardiac frequency
// return 1 if the file was correctly read
// return 0 if a problem occurred
int readFileAnalyse(short * symptoms, int nbSym, int id_socdet, int choice, std::string pathToAled) {

	std::string test = "";

	if (choice == 0) { // mouse test - MT
		test = "MT";
	}
	else if (choice == 1) { // keyboard - KB
		test = "KB";
	}
	else if (choice == 2) { // form - F
		test = "F";

	}
	else if (choice == 3) { // patient file - PF
		test = "PF";
	}
	else if (choice == 4) { // eye blinking - EB
		test = "EB";
	}
	else if (choice == 5) { // cardiac frequency - CF
		test = "CF";
	}
	else {
		return 0;
	}

	FILE * f(NULL);

	// filename : Patient_id_socdet_test.txt
	// exemple : id_socdet = 310 and test = "EB" then Patient_310_EB.txt is the filename
	std::ostringstream a;
    a << id_socdet;
	std::string filename = pathToAled + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + a.str() + "/" + "Patient_" + a.str() + "_" + test + ".txt";

	// Creation and opening of the file using the filename
	f = fopen(filename.c_str(), "rt");

	if (f == NULL) {
		std::cout << "Impossible to open the file for writing" << std::endl;
		return 0;
	}

	for(int i(0); i < nbSym; i++) {
		// id_symptom (short)
		short id = 0;
		// criticality (short)
		short criticality = 0;
		int read = fscanf(f, "%hd %hd\n", &id, &criticality);
		if (read != 2){
			return 0;
		}
		symptoms[i] = criticality;
	}

	fclose(f);

	return 1;
}

// To merge all the files together in order to obtain all the symptoms of the different analysis
// symptoms must be initialized to 0
// mt, kb, f, pf, eb, cf : when equal to 1, we merge the test related, otherwise we do not
// return 0 if a problem occured
// return 1 if everything went well
int mergeFile(short * symptoms, int nbSym, int id_socdet, int mt, int kb, int f, int pf, int eb, int cf, std::string pathToAled) {
	// intermediate symptoms array
	short symptomsIntermediate[nbSym];
	// To be sure that all the symptoms are equal to zero
	for(int i(0); i < nbSym; i++) {
		symptoms[i] = 0;
		symptomsIntermediate[i] = 0;
	}

	int read = 0;

	// if no element is equal to 1, we have nothing to merge
	if(mt == 0 && kb == 0 && f == 0 && pf == 0 && eb == 0 && cf == 0) {
		return 0;
	}
	if (mt != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 0, pathToAled);
		if (read == 0) {
			return 0;
		}
		for(int i(0); i < nbSym; i++) {
			symptoms[i] = symptoms[i] + symptomsIntermediate[i];
		}
	}

	if (kb != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 1, pathToAled);
		if (read == 0) {
			return 0;
		}
		for(int i(0); i < nbSym; i++) {
			symptoms[i] = symptoms[i] + symptomsIntermediate[i];
		}
	}

	if (f != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 2, pathToAled);
		if (read == 0) {
			return 0;
		}
		for(int i(0); i < nbSym; i++) {
			symptoms[i] = symptoms[i] + symptomsIntermediate[i];
		}
	}

	if (pf != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 3, pathToAled);
		if (read == 0) {
			return 0;
		}
		for(int i(0); i < nbSym; i++) {
			symptoms[i] = symptoms[i] + symptomsIntermediate[i];
		}
	}

	if (eb != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 4, pathToAled);
		if (read == 0) {
			return 0;
		}
		for(int i(0); i < nbSym; i++) {
			symptoms[i] = symptoms[i] + symptomsIntermediate[i];
		}
	}

	if (cf != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 5, pathToAled);
		if (read == 0) {
			return 0;
		}
		for(int i(0); i < nbSym; i++) {
			symptoms[i] = symptoms[i] + symptomsIntermediate[i];
		}
	}

	return 1;
}

// To write the analyse file : creation of the txt file for the intermediary test
// choice = 0 : mouse test
// choice = 1 : keyboard test
// choice = 2 : form
// choice = 3 : patient file
// choice = 4 : eye blinking
// choice = 5 : cardiac frequency
// return 1 if the file was correctly created
// return 0 if a problem occurred
int writeFileAnalyse(short * symptoms, int nbSym, int id_socdet, int choice, std::string pathToAled) {

	std::string test = "";

	if (choice == 0) { // mouse test - MT
		test = "MT";
	}
	else if (choice == 1) { // keyboard - KB
		test = "KB";
	}
	else if (choice == 2) { // form - F
		test = "F";

	}
	else if (choice == 3) { // patient file - PF
		test = "PF";
	}
	else if (choice == 4) { // eye blinking - EB
		test = "EB";
	}
	else if (choice == 5) { // cardiac frequency - CF
		test = "CF";
	}
	else {
		return 0;
	}

	FILE * f(NULL);

	// filename : Patient_id_socdet_test.txt
	// exemple : id_socdet = 310 and test = "EB" then Patient_310_EB.txt is the filename
	std::ostringstream a;
    a << id_socdet;
	std::string filename = pathToAled +  "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + a.str() + "/" + "Patient_" + a.str() + "_" + test + ".txt";

	// Creation and opening of the file using the filename
	f = fopen(filename.c_str(), "wt");

	if (f == NULL) {
		std::cout << "Impossible to open the file for writing" << std::endl;
		return 0;
	}

	for(int i(0); i < nbSym; i++) {
		// id_symptom (short)
		short id = i + 1;
		// criticality (short)
		short criticality = symptoms[i];
		fprintf(f, "%hd %hd\n", id, criticality);
	}

	fclose(f);

	return 1;
}

// To write the final analyse file
// Return 0 if a problem occured, the timestamp otherwise
unsigned long writeFileFinalAnalyse(short * symptoms, int nbSym, int globalCriticality, int id_socdet, std::string pathToAled) {
	FILE * f(NULL);

	std::cout << "Writing of the final_analyse file" << std::endl;

	// timstamp
	unsigned long timestamp = time(NULL);

	// filename : Patient_id_socdet_timestamp_BehaviorAnalysis.bin
	// exemple : id_socdet = 310 and timestamp = 1589293516 then Patient_310_1589293516_BehaviorAnalysis.bin is the filename
	std::ostringstream a;
    a << id_socdet;
    std::ostringstream b;
    b << timestamp;
	std::string filename = pathToAled +  "/EmittedData/" + "Patient_" + a.str() + "_" + b.str() + "_BehaviorAnalysis.bin";
	std::cout << filename << std::endl;

	// Creation and opening of the file using the filename
	f = fopen(filename.c_str(), "w+b");

	if (f == NULL) {
		std::cout << "Impossible to open the file for writing" << std::endl;
		return 0;
	}

	// 100 (short)
	short hundred = 100;
	fwrite(&hundred, sizeof(short), 1, f);

	// timestamps(unsigned long)
	std::cout << "timestamp:" << timestamp << std::endl;
	fwrite(&timestamp, sizeof(unsigned long), 1, f);

	// id_socdet(int)
	fwrite(&id_socdet, sizeof(int), 1, f);

	// global criticality(short)
	fwrite(&globalCriticality, sizeof(short), 1, f);

	// nb_symptoms(int)
	short nb = nbSymptomNonNull(symptoms, nbSym);
	fwrite(&nb, sizeof(short), 1, f);
	std::cout << "nb : " << nb << std::endl;

	// id_symptom(short) criticality(short)
	if(nb > 0) {
		for(int i(0); i < nbSym; i++) {
			if(symptoms[i] != 0) {
				// id_symptom (short)
				short id = i + 1;
				fwrite(&id, sizeof(short), 1, f);
				// criticality (short)
				short criticality = symptoms[i];
				fwrite(&criticality, sizeof(short), 1, f);
				std::cout << "id : " << id << " criticality : " << criticality << std::endl;
			}
		}
	}

	// We close the file
	fclose(f);

	return timestamp;
}

