#include <mysql.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <string>
#include <string>
#include <time.h>
#include "Date.h"
#include "Patient.h"
#include "api.h"

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

	unsigned long nb_result = 0;
	nb_result = mysql_num_rows(result);
	if (nb_result != 1) {
		// We have no result registered
		std::cout << "The symptom is not in the database " << mysql_error(con) << std::endl;
		return -1;
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
	int idSym = 0;

	// If the row is non NULL, we convert the result into a number
	if(row[0]) {
		out << row[0];
		std::string str = out.str();
		idSym = atoi(str.c_str());
	}

	// If the result is out of minimal bound
	if (idSym < 1)
		idSym = 0;

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

	unsigned long nb_result = 0;
	nb_result = mysql_num_rows(result);
	if (nb_result != 1) {
		// We have no result registered
		std::cout << "The gender is not in the database " << mysql_error(con) << std::endl;
		return -1;
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

	unsigned long nb_result = 0;
	nb_result = mysql_num_rows(result);
	if (nb_result != 1) {
		// We have no result registered
		std::cout << "The picture is not in the database " << mysql_error(con) << std::endl;
		return "none";
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

	unsigned long nb_result = 0;
	nb_result = mysql_num_rows(result);
	if (nb_result != 1) {
		// We have no result registered
		std::cout << "The id of the medical record is not in the database " << mysql_error(con) << std::endl;
		return -1;
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

	unsigned long nb_result = 0;
	nb_result = mysql_num_rows(result);
	if (nb_result != 1) {
		// We have no result registered
		std::cout << "The height is not in the database " << mysql_error(con) << std::endl;
		return -1;
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

	unsigned long nb_result = 0;
	nb_result = mysql_num_rows(result);
	if (nb_result != 1) {
		// We have no result registered
		std::cout << "The weight is not in the database " << mysql_error(con) << std::endl;
		return -1;
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

	unsigned long nb_result = 0;
	nb_result = mysql_num_rows(result);
	if (nb_result != 1) {
		// We have no result registered
		std::cout << "The blood group is not in the database " << mysql_error(con) << std::endl;
		return -1;
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

	unsigned long nb_result = 0;
	nb_result = mysql_num_rows(result);
	if (nb_result != 1) {
		// We have no result registered
		std::cout << "The birthdate is not in the database " << mysql_error(con) << std::endl;
		return "none";
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

// retreive the name of the genetic diseases
std::string retrieveNameGenetic(MYSQL *con, int id_gen) {
	std::ostringstream a;
    a << id_gen;
	// We execute the query
	std::string query = "SELECT name FROM genetic WHERE id_gen='" + a.str() + "'";
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
		std::cout << "The genetic disease is not in the database " << mysql_error(con) << std::endl;
		return "none";
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
	std::string genetic = "none";

	// If the row is non NULL, we convert the result into a string
	if(row[0]) {
		out << row[0];
		genetic = out.str();
	}

	// If the result is out of minimal bound
	if (genetic.compare("") == 0)
		genetic = "none";

	// We free the result set
	mysql_free_result(result);

	return genetic;
}

// retrieve the name of the allergy
std::string retrieveNameAllergy(MYSQL *con, int id_allergy) {
	std::ostringstream a;
    a << id_allergy;
	// We execute the query
	std::string query = "SELECT name FROM allergy WHERE id_allergy='" + a.str() + "'";
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
		std::cout << "The allergy is not in the database " << mysql_error(con) << std::endl;
		return "none";
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
	std::string allergy = "none";

	// If the row is non NULL, we convert the result into a string
	if(row[0]) {
		out << row[0];
		allergy = out.str();
	}

	// If the result is out of minimal bound
	if (allergy.compare("") == 0)
		allergy = "none";

	// We free the result set
	mysql_free_result(result);

	return allergy;
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

// Create and fill an object of the class Patient for the patient of the id id_socdet
// if std = 1, it will display all the patient information during the filling of the object
Patient createPatient(MYSQL * con, int id_socdet, int std) {
	Patient patient;
	patient.setIdSocdet(id_socdet);

	// Gender
	int gender = retrieveGender(con, id_socdet);
	if (gender == -1) {
		if(std == 1)
			std::cout << "The patient has no gender" << std::endl;
	}
	else {
		patient.setGender(gender);
		if(std == 1)
			std::cout << "Gender " << gender << std::endl;
	}

	// birthdate
	std::string str_date = retrieveBirthdate(con, id_socdet);
	if (str_date.compare("none") == 0) {
		if(std == 1)
			std::cout << "The patient has no birthdate" << std::endl;
	}
	else {
		if(std == 1)
			std::cout << "Date " << str_date<< std::endl;
		// class Date to contain the birthdate of the patient
		Date birthdate = stringToDate(str_date);
		if(std == 1)
			birthdate.printDate();

		// We use the time library to retrieve the current date
		Date today;
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		today.setDay(tm.tm_mday);
		today.setMonth(tm.tm_mon + 1);
		today.setYear(tm.tm_year + 1900);

		// We compute the age
		patient.computeAge(birthdate, today);
	}

	// picture
	std::string picture = retrievePicturePath(con, id_socdet);
	if (picture.compare("none") == 0) {
		if(std == 1)
			std::cout << "The patient has no picture" << std::endl;
	}
	else {
		patient.setPicture(picture);
		if(std == 1)
			std::cout << "Picture " << picture << std::endl;
	}

	// id_medrec
	int id_medrec = retrieveIdMedrec(con, id_socdet);
	if (id_medrec == -1) {
		if(std == 1)
			std::cout << "The patient has no medical record" << std::endl;
	}
	else {
		patient.setIdMedrec(id_medrec);
		// height
		int height = retrieveHeight(con, id_medrec);
		if(height == -1) {
			if(std == 1)
				std::cout << "The patient has no height" << std::endl;
		}
		else {
			patient.setHeight(height);
			if(std == 1)
				std::cout << "Height " << height << std::endl;
		}

		// weight
		int weight = retrieveWeight(con, id_medrec);
		if(weight == -1) {
			if(std == 1)
				std::cout << "The patient has no weight" << std::endl;
		}
		else {
			patient.setWeight(weight);
			if(std == 1)
				std::cout << "Weight " << weight << std::endl;
		}

		if(weight != -1 && height != -1) {
			patient.computeIMCI();
			if (str_date.compare("none") != 0 && gender != -1) {
				patient.computeIMG();
			}
		}

		// id_blgrp
		int id_blgrp = retrieveBloodGroup(con, id_medrec);
		if (id_blgrp == -1) {
			if(std == 1)
				std::cout << "The patient has no blood type" << std::endl;
		}
		else {
			patient.setIdBlgrp(id_blgrp);
			if(std == 1)
				std::cout << "Blood group " << id_blgrp << std::endl;
		}

		// allergy
		int nbAllergy = retrieveNbAllergy(con, id_medrec);
		if (nbAllergy == 0) {
			if(std == 1)
				std::cout << "The patient has no allergy" << std::endl;
		}
		else {
			if(std == 1)
				std::cout << "NbAllergy " << nbAllergy << std::endl;
			patient.setNbAllergy(nbAllergy);
			short allergy[nbAllergy];
			retrieveAllergy(con, id_medrec, nbAllergy, allergy);
			patient.setAllergy(allergy, nbAllergy);
		}

		// genetic
		int nbGenetic = retrieveNbGenetic(con, id_medrec);
		if (nbGenetic == 0) {
			if(std == 1)
				std::cout << "The patient has no genetic diseases" << std::endl;
		}
		else {
			if(std == 1)
				std::cout << "NbGenetic " << nbGenetic << std::endl;
			patient.setNbGenetic(nbGenetic);
			short genetic[nbGenetic];
			retrieveGenetic(con, id_medrec, nbGenetic, genetic);
			patient.setGenetic(genetic, nbGenetic);
		}

	}

	return patient;
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
int readFileFinalAnalyse(int id_socdet, unsigned long timestamp, std::string pathToAled, int std){
	FILE * f(NULL);

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
		std::cout << "Impossible to open the file " << filename << " for reading" << std::endl;
		return 0;
	}

	// 100 (short)
	short hundred = 0;
	size_t size = fread(&hundred, sizeof(short), 1, f);
	if (size != 1) {
		return 0;
	}
	else {
		if(std == 1)
			std::cout << "hundred : " << hundred << std::endl;
	}

	// timestamps(unsigned long)
	unsigned long timestamp_now = 0;
	size = fread(&timestamp_now, sizeof(unsigned long), 1, f);
	if (size != 1) {
		return 0;
	}
	else {
		if(std == 1)
			std::cout << "timestamp :" << timestamp_now << std::endl;
	}

	// id_socdet(int)
	int id_soc = 0;
	size = fread(&id_soc, sizeof(int), 1, f);
	if (size != 1) {
		return 0;
	}
	else {
		if(std == 1)
			std::cout << "id_socdet : " << id_soc << std::endl;
	}

	// global criticality(short)
	short globalCriticality = 0;
	size = fread(&globalCriticality, sizeof(short), 1, f);
	if (size != 1) {
		return 0;
	}
	else {
		if(std == 1)
			std::cout << "global criticality : " << globalCriticality << std::endl;
	}

	// nb_symptoms(int)
	short nb = 0;
	size = fread(&nb, sizeof(short), 1, f);
	if (size != 1) {
		return 0;
	}
	else {
		if(std == 1)
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
				if(std == 1)
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
// choice = 6 : genetic and allergy
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
	else if (choice == 6) { // genetic and allergy - GenAll
		test = "GenAll";
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
		std::cout << "Impossible to open the file " << filename << " for reading" << std::endl;
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

// If the file is present in the directory of the patient, we return 1, otherwise, we return 0
int isPresent(std::string test, std::string PatientID, std::string pathToAled) {
	// Reading of the cardiac frequency
	std::string pathToOutput = pathToAled + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + PatientID + "/Patient_" + PatientID + "_" + test + ".txt";

	FILE * f = fopen(pathToOutput.c_str(), "rt");

	if (f == NULL) {
		//std::cout << "Impossible to open the file " << pathToOutput << " for reading" << std::endl;
		return 0;
	}

	fclose(f);
	return 1;
}

// To merge all the files together in order to obtain all the symptoms of the different analysis
// symptoms must be initialized to 0
// return 0 if a problem occured
// return 1 if everything went well
int mergeFile(short * symptoms, int nbSym, int id_socdet, std::string pathToAled) {
	// intermediate symptoms array
	short symptomsIntermediate[nbSym];

	// To be sure that all the symptoms are equal to zero
	for(int i(0); i < nbSym; i++) {
		symptoms[i] = 0;
		symptomsIntermediate[i] = 0;
	}

	int read = 0;

	// mt, kb, f, pf, eb, cf : when equal to 1, we merge the test related, otherwise we do not
	std::ostringstream a;
    a << id_socdet;
	int mt = isPresent("MT", a.str(), pathToAled);
	int kb = isPresent("KB", a.str(), pathToAled);
	int f = isPresent("F", a.str(), pathToAled);
	int pf = isPresent("PF", a.str(), pathToAled);
	int eb = isPresent("EB", a.str(), pathToAled);
	int cf = isPresent("CF", a.str(), pathToAled);
	int genall = isPresent("GenAll", a.str(), pathToAled);

	if(mt == 0 && kb == 0 && f == 0 && pf == 0 && eb == 0 && cf == 0) {
		return 0;
	}
	if (mt != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 0, pathToAled);
		if (read != 0) {
			for(int i(0); i < nbSym; i++) {
				symptoms[i] = symptoms[i] + symptomsIntermediate[i];
			}
		}
	}

	if (kb != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 1, pathToAled);
		if (read != 0) {
			for(int i(0); i < nbSym; i++) {
				symptoms[i] = symptoms[i] + symptomsIntermediate[i];
			}
		}
	}

	if (f != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 2, pathToAled);
		if (read != 0) {
			for(int i(0); i < nbSym; i++) {
				symptoms[i] = symptoms[i] + symptomsIntermediate[i];
			}
		}
	}

	if (pf != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 3, pathToAled);
		if (read != 0) {
			for(int i(0); i < nbSym; i++) {
				symptoms[i] = symptoms[i] + symptomsIntermediate[i];
			}
		}
	}

	if (eb != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 4, pathToAled);
		if (read != 0) {
			for(int i(0); i < nbSym; i++) {
				symptoms[i] = symptoms[i] + symptomsIntermediate[i];
			}
		}
	}

	if (cf != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 5, pathToAled);
		if (read != 0) {
			for(int i(0); i < nbSym; i++) {
				symptoms[i] = symptoms[i] + symptomsIntermediate[i];
			}
		}
	}

	if (genall != 0) {
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 6, pathToAled);
		read = readFileAnalyse(symptomsIntermediate, nbSym, id_socdet, 6, pathToAled);
		if (read != 0) {
			for(int i(0); i < nbSym; i++) {
				if(symptoms[i] != 0) {
					symptoms[i] = symptoms[i] + symptomsIntermediate[i];
				}
			}
		}
	}

	for(int i(0); i < nbSym; i++) {
		if(symptoms[i] > 100) {
			symptoms[i] = 100;
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
// choice = 6 : genetic and allergy
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
	else if (choice == 6) { // genetic and allergy - GenAll
		test = "GenAll";
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
		std::cout << "Impossible to open the file " << filename << " for writing" << std::endl;
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
// when std = 1 : write the information
unsigned long writeFileFinalAnalyse(short * symptoms, int nbSym, int globalCriticality, int id_socdet, std::string pathToAled, int std) {
	FILE * f(NULL);

	// timstamp
	unsigned long timestamp = time(NULL);

	// filename : Patient_id_socdet_timestamp_BehaviorAnalysis.bin
	// exemple : id_socdet = 310 and timestamp = 1589293516 then Patient_310_1589293516_BehaviorAnalysis.bin is the filename
	std::ostringstream a;
    a << id_socdet;
    std::ostringstream b;
    b << timestamp;
	std::string filename = pathToAled +  "/EmittedData/" + "Patient_" + a.str() + "_" + b.str() + "_BehaviorAnalysis.bin";
	if(std == 1)
		std::cout << filename << std::endl;

	// Creation and opening of the file using the filename
	f = fopen(filename.c_str(), "w+b");

	if (f == NULL) {
		std::cout << "Impossible to open the file " << filename << " for writing" << std::endl;
		return 0;
	}

	// 100 (short)
	short hundred = 100;
	fwrite(&hundred, sizeof(short), 1, f);

	// timestamps(unsigned long)
	if(std == 1)
		std::cout << "timestamp :" << timestamp << std::endl;
	fwrite(&timestamp, sizeof(unsigned long), 1, f);

	// id_socdet(int)
	if(std == 1)
		std::cout << "id_socdet :" << id_socdet << std::endl;
	fwrite(&id_socdet, sizeof(int), 1, f);

	// global criticality(short)
	if(std == 1)
		std::cout << "global criticality :" << globalCriticality << std::endl;
	fwrite(&globalCriticality, sizeof(short), 1, f);

	// nb_symptoms(int)
	short nb = nbSymptomNonNull(symptoms, nbSym);
	fwrite(&nb, sizeof(short), 1, f);
	if(std == 1)
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
				if(std == 1)
					std::cout << "id : " << id << " criticality : " << criticality << std::endl;
			}
		}
	}

	// We close the file
	fclose(f);

	return timestamp;
}
