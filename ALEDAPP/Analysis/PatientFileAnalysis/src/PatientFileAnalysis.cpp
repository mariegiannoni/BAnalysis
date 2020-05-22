#include <mysql.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <string>
#include <time.h>
#include "Patient.h"
#include "api.h"

std::string PATHTOALEDAPP;


// Analyse the IMC of a patient using its age and its gender
short imciAnalyse(float imci, int age, int gender);

// Do IMG Analysis
short imgAnalyse(float img, int gender);

// Compute allergy and genetic issues
void antecedents(MYSQL * con, Patient &patient);

// Create and fill an object of the class Patient
Patient createPatient(MYSQL * con, int id_socdet, int std);

// Creation of the file containing symptoms for patient file
void createOutputAnalysis(MYSQL * con, Patient &patient);


int main( int argc, char ** argv) {
	if (argc == 1) {
		std::cout << " 1str arg : '10' // 2nd arg : Patient ID // 3rd arg : PATHTOALEDAPP" << std::endl;
		std::cout << "This program will create an output in ALEDAPP/Analysis/FinalAnalysis/PreviousOutputs/Patient + PATIENTID\n" << std::endl;
	}

	// Classic mode
	if (argc == 4 && strcmp(argv[1], "10") == 0) {
		PATHTOALEDAPP = argv[3];
		std::string PatientID = argv[2];
		int id_socdet = atoi(PatientID.c_str());

		// PATIENT FILE

		MYSQL *con = mysql_init(NULL);

		if (con == NULL) {
			// The initialisation has failed
			std::cout << "Error : " << mysql_error(con) << std::endl;
			exit(1);
		}

		if (mysql_real_connect(con, "localhost", "root", "root", "BehaviorAnalysis", 0, NULL, 0) == NULL) {
			// The connection to the database has failed
			std::cout << "Error : " << mysql_error(con) << std::endl;
			mysql_close(con);
			exit(1);
		}

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

		// Creation and filling of an object from the class Patient
		Patient patient = createPatient(con, id_socdet, 0);

		// Compute allergies and genetic		
		if(patient.getNbGenetic() != 0 || patient.getNbAllergy() != 0)
			antecedents(con, patient);

		// Creation of the output analysis
		createOutputAnalysis(con, patient);

		// We close the connection
		mysql_close(con);

	}
	// Draft interface mode
	else if (argc == 4 && strcmp(argv[1], "1") == 0) {
		PATHTOALEDAPP = argv[3];
		std::string PatientID = argv[2];
		int id_socdet = atoi(PatientID.c_str());

		// PATIENT FILE

		MYSQL *con = mysql_init(NULL);

		if (con == NULL) {
			// The initialisation has failed
			std::cout << "Error : " << mysql_error(con) << std::endl;
			exit(1);
		}

		if (mysql_real_connect(con, "localhost", "root", "root", "BehaviorAnalysis", 0, NULL, 0) == NULL) {
			// The connection to the database has failed
			std::cout << "Error : " << mysql_error(con) << std::endl;
			mysql_close(con);
			exit(1);
		}

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

		// PATIENT FILE
		// Creation and filling of an object from the class Patient
		Patient patient = createPatient(con, id_socdet, 1);

		// Print the patient's file
		patient.printPatient();

		if(patient.getAge() != -1 && patient.getGender() != -1 && patient.getIMCI() != 0) {
			short _weight_imci = imciAnalyse(patient.getIMCI(), patient.getAge(), patient.getGender());
			std::cout << "_weight_imci represent the criticality linked to the IMCI of the patient" << std::endl;
			std::cout << "_weight_imci < 0 : underweight\n_weight_imci = 0 : normal weight\n_weight_imci > 0 : overweight" << std::endl;
			std::cout << "_weight_imci : " << _weight_imci << std::endl;
		}

		if(patient.getAge() != -1 && patient.getGender() != -1 && patient.getIMG() != 0) {
			short _weight_img = imgAnalyse(patient.getIMG(), patient.getGender());
			std::cout << "_weight_img represent the criticality linked to the IMG of the patient" << std::endl;
			std::cout << "_weight_img < 0 : underweight\n_weight_img = 0 : normal weight\n_weight_img > 0 : overweight" << std::endl;
			std::cout << "_weight_img : " << _weight_img << std::endl;
		}

		// We close the connection
		mysql_close(con);

	}
	return 0;
}

// Analyse the IMC of a patient using its age and its gender
// Return 0 if the weight is normal
// Return a negative non null number if he is underweight
// Return a positive non null number if he is overweight
short imciAnalyse(float imci, int age, int gender) {
	short _weight;
	float a_1, a_2, a_3, a_4, a_5, a_6;

	// Definition of limits according to the age and gender
	if(age < 7) {
		a_1 = 13.0;
		a_2 = 14.0;
		a_3 = 16.0;
		a_4 = 18.0;
		a_5 = 19.0;
		a_6 = 20.0;
	}
	else if (age >= 7  && age < 15) {
		a_1 = 12.0;
		a_2 = 14.0;
		a_3 = 20.0;
		a_4 = 24.0;
		a_5 = 28.0;
		a_6 = 32.0;
	}
	else if (age >= 15  && age < 20) {
		if(gender == 0) {
			a_1 = 14.5;
			a_2 = 17.0;
			a_3 = 24.0;
			a_4 = 28.0;
			a_5 = 32.0;
			a_6 = 35.0;
		}
		else {
			a_1 = 13.5;
			a_2 = 16.0;
			a_3 = 26.0;
			a_4 = 29.0;
			a_5 = 33.0;
			a_6 = 36.0;
		}
	}
	else if (age >= 20  && age < 60) {
		if(gender == 0) {
			a_1 = 16.5;
			a_2 = 18.5;
			a_3 = 25.0;
			a_4 = 30.0;
			a_5 = 35.0;
			a_6 = 40.0;
		}
		else {
			a_1 = 15.5;
			a_2 = 17.5;
			a_3 = 27.0;
			a_4 = 32.0;
			a_5 = 37.0;
			a_6 = 41.0;
		}
	}
	else {
		if(gender == 0) {
			a_1 = 17.0;
			a_2 = 19.0;
			a_3 = 32.0;
			a_4 = 42.0;
			a_5 = 47.0;
			a_6 = 50.0;
		}
		else {
			a_1 = 16.0;
			a_2 = 18.0;
			a_3 = 35.0;
			a_4 = 45.0;
			a_5 = 50.0;
			a_6 = 54.0;
		}
	}

	if(imci < a_1) {
		_weight = -25; // denutrition, anorewia
	}
	else if(imci >= a_1 && imci < a_2) {
		_weight = -10; // underweight range
	}
	else if(imci >= a_2 && imci < a_3) {
		_weight = 0; // normal
	}
	else if(imci >= a_3 && imci < a_4) {
		_weight = 10; // overweight range
	}
	else if(imci >= a_4 && imci < a_5) {
		_weight = 25; // obese range
	}
	else if(imci >= a_5 && imci < a_6) {
		_weight = 50; // severe obese range
	}
	else {
		_weight = 75; // extrem obese range
	}

	return _weight;
}

// Do IMG Analysis
// return 0 if normal fat proportion
// return a non null negative number if underweigth
// return a non null positive number if overweigth
short imgAnalyse(float img, int gender) {
	short _weight = 0;
	float min, max;

	// different limit according to the gender
	if (gender == 0) {
		min = 15.0;
		max = 20.0;
	}
	else {
		min = 25.0;
		max = 30.0;
	}

	if (img < min) { // too skinny
		_weight = - abs(img - min);
		if(_weight < -100) _weight = -100;
	}
	else if (img > max) { // too fat
		_weight = abs(img - max);
		if(_weight > 100) _weight = 100;
	}
	return _weight;
}

// Compute the patient antecedents
void antecedents(MYSQL * con, Patient &patient) {
	// We find the number of symptoms on the table using mysql_num_rows
	int nbSym = retrieveNbSymp(con);
	int nbAll = patient.getNbAllergy();
	int nbGen = patient.getNbGenetic();

	/* Create an array of short with the number of symptoms */
	short symptoms[(int)nbSym];

	// Initialisation of the array with 0
	for(int i(0); i < (int)nbSym; i++){
		symptoms[i] = 0;
	}
	
	// Getting the symptoms id
	int worry = getIdSymptom(con, "worry");
	int vertigo = getIdSymptom(con, "vertigo");
	int tremor = getIdSymptom(con, "tremor");
	int chills = getIdSymptom(con, "chills");
	int fatigue = getIdSymptom(con, "fatigue");
	int dizziness = getIdSymptom(con, "dizziness");
	int palpitation = getIdSymptom(con, "palpitation");
	int cardio = getIdSymptom(con, "cardiovascular_event");
	int backPain = getIdSymptom(con, "pain_back");
	int chestPain = getIdSymptom(con, "pain_chest");
	int shortBreath = getIdSymptom(con, "shortness_of_breath");
	int cough = getIdSymptom(con, "cough");
	int abdominalPain = getIdSymptom(con, "pain_abdominal");
	int headache = getIdSymptom(con, "headache");
	int flatulence = getIdSymptom(con, "flatulence");
	int nausea = getIdSymptom(con, "nausea");
	int distendedAbdomen = getIdSymptom(con, "distended_abdomen");
	int sweat = getIdSymptom(con, "sweat");
	int neckPain = getIdSymptom(con, "pain_neck");
	int fever = getIdSymptom(con, "fever");
	int earPain = getIdSymptom(con, "ear_pain");
	int bradycardia = getIdSymptom(con, "bradycardia");
	int thirst = getIdSymptom(con, "permanent_thirst");
	int incoherent = getIdSymptom(con, "incoherent");
	int difficulty = getIdSymptom(con, "difficulty");
	int anorexia = getIdSymptom(con, "anorexia");
	int underweight = getIdSymptom(con, "underweight");
	int nosebleed = getIdSymptom(con, "nosebleed");

	std::string strId = "";

	// we check if the patient has known allergies
	if (nbAll != 0) {
		short allergy [nbAll];
		patient.getAllergy(allergy);
		for (int i = 0; i < nbAll; i++) {
			strId = retrieveNameAllergy(con, allergy[i]);
			if (strId.compare("Egg") == 0 || strId.compare("Fish") == 0 || strId.compare("Fruits") == 0
			|| strId.compare("Garlic") == 0 || strId.compare("Peppers") == 0 || strId.compare("Maize") == 0
			|| strId.compare("Milk") == 0 || strId.compare("Peanut") == 0 || strId.compare("Poultry Meat") == 0
			|| strId.compare("Red Meat") == 0 || strId.compare("Rice") == 0 || strId.compare("Sesame") == 0
			|| strId.compare("Shellfish") == 0 || strId.compare("Soy") == 0 || strId.compare("Sulfites") == 0
			|| strId.compare("Tartrazine") == 0 || strId.compare("Wheat") == 0) {
				if(worry != -1)
					symptoms[worry] += 10;
					
				if(vertigo != -1)
					symptoms[vertigo] += 10;

				if(fatigue != -1)
					symptoms[fatigue] += 5;

				if(dizziness != -1)
					symptoms[dizziness] += 10;

				if(palpitation != -1)
					symptoms[palpitation] += 20;

				if(cardio != -1) 
					symptoms[cardio] += 10;

				if(backPain != -1)
					symptoms[backPain] += 5;

				if(chestPain != -1)
					symptoms[chestPain] += 5;

				if(shortBreath != -1)
					symptoms[shortBreath] += 20;

				if(cough != -1)
					symptoms[cough] += 15;

				if(abdominalPain != -1)
					symptoms[abdominalPain] += 5;

				if(headache != -1)
					symptoms[headache] += 15;

				if(flatulence != -1)
					symptoms[flatulence] += 5;

				if(nausea != -1)
					symptoms[nausea] += 20;
					
				if (strId.compare("Wheat")) {
					if(distendedAbdomen != -1)
						symptoms[distendedAbdomen] += 10; 
				}
				else {
					if(distendedAbdomen != -1)
						symptoms[distendedAbdomen] += 5; 
				}
			}
			else if (strId.compare("Balsam of Peru") == 0) {
				if(worry != -1)
					symptoms[worry] += 5;
					
				if(vertigo != -1)
					symptoms[vertigo] += 15;
					
				if(fatigue != -1)
					symptoms[fatigue] += 5;
					
				if(dizziness != -1)
					symptoms[dizziness] += 10;
					
				if(palpitation != -1)
					symptoms[palpitation] += 20;
					
				if(cardio != -1)
					symptoms[cardio] += 10;
					
				if(backPain != -1)
					symptoms[backPain] += 10;
					
				if(chestPain != -1) 
					symptoms[chestPain] += 10;
					
				if(shortBreath != -1)
					symptoms[shortBreath] += 20;
					
				if(abdominalPain!= -1)
					symptoms[abdominalPain] += 10;
					
				if(sweat != -1)
					symptoms[sweat] += 5;

				if(headache != -1)
					symptoms[headache] += 15;

				if(nausea != -1)
					symptoms[nausea] += 15;

				if(neckPain != -1)
					symptoms[neckPain] += 10; 
			}
			else if (strId.compare("Tetracycline") == 0 || strId.compare("Dilantin") == 0 || strId.compare("Tegretol") == 0
			|| strId.compare("Penicilin") == 0 || strId.compare("Cephalosporins") == 0|| strId.compare("Sulfonamides") == 0) {
				if(tremor != -1)
					symptoms[tremor] += 5;
					
				if(worry != -1)
					symptoms[worry] += 10;

				if(chills != -1)
					symptoms[chills] += 20;

				if(vertigo != -1)
					symptoms[vertigo] += 20;

				if(fatigue!= -1)
					symptoms[fatigue] += 15;

				if(dizziness != -1)
					symptoms[dizziness] += 10;

				if(fever != -1)
					symptoms[fever] += 20;

				if(palpitation != -1)
					symptoms[palpitation] += 20;

				if(cardio != -1)
					symptoms[cardio] += 10;

				if(backPain != -1)
					symptoms[backPain] += 10;

				if(chestPain != -1)
					symptoms[chestPain] += 10;

				if(shortBreath != -1)
					symptoms[shortBreath] += 20;

				if(abdominalPain != -1)
					symptoms[abdominalPain] += 10;

				if(sweat != -1)
					symptoms[sweat] += 5;

				if(headache != -1)
					symptoms[headache] += 15;

				if(nausea != -1)
					symptoms[nausea] += 15;
					
				if (strId.compare("Cephalosporins") != 0 || strId.compare("Sulfonamides") != 0) {
					if(neckPain != -1)
						symptoms[neckPain] += 10; 
				}
				else if (strId.compare("Cephalosporins") != 0) {
					if(distendedAbdomen != -1)
						symptoms[distendedAbdomen] += 10;
						
					if(flatulence != -1)
						symptoms[flatulence] += 10; 
				}
			}
			else if (strId.compare("Dog") == 0 || strId.compare("Cat") == 0 || strId.compare("Insect Sting") == 0) {
				if(worry != -1)
					symptoms[worry] += 5;

				if(vertigo != -1)
					symptoms[vertigo] += 20;

				if(fatigue != -1) 
					symptoms[fatigue] += 5;

				if(dizziness != -1)
					symptoms[dizziness] += 10;

				if(palpitation != -1)
					symptoms[palpitation] += 20;

				if(cardio != -1)
					symptoms[cardio] += 10;

				if(backPain != -1)
					symptoms[backPain] += 5;

				if(chestPain != -1)
					symptoms[chestPain] += 5;

				if(shortBreath != -1)
					symptoms[shortBreath] += 20;
					
				if (strId.compare("Insect Sting") == 0) {
					if(worry != -1)
						symptoms[worry] += 10;
						
					if(fever != -1)
						symptoms[fever] += 10; 
				}
			}
			else if (strId.compare("Mold") == 0) {
				if(worry != -1)
					symptoms[worry] += 10;

				if(vertigo != -1)
					symptoms[vertigo] += 10;

				if(fatigue != -1)
					symptoms[fatigue] += 5;

				if(dizziness != -1)
					symptoms[dizziness] += 10;

				if(palpitation != -1)
					symptoms[palpitation] += 20;

				if(cardio != -1) 
					symptoms[cardio] += 10;

				if(backPain != -1)
					symptoms[backPain] += 10;

				if(chestPain != -1)
					symptoms[chestPain] += 5;

				if(shortBreath != -1)
					symptoms[shortBreath] += 20;

				if(cough != -1)
					symptoms[cough] += 15;

				if(distendedAbdomen != -1)
					symptoms[distendedAbdomen] += 5;

				if(abdominalPain != -1)
					symptoms[abdominalPain] += 5;

				if(headache != -1)
					symptoms[headache] += 15;

				if(flatulence != -1)
					symptoms[flatulence] += 5;

				if(nausea != -1)
					symptoms[nausea] += 20; 
			}
			else if (strId.compare("Perfume") == 0 || strId.compare("Cosmetics") == 0 || strId.compare("Latex") == 0
			|| strId.compare("Nickel") == 0 || strId.compare("Gold") == 0 || strId.compare("Chromium") == 0 || strId.compare("Cobalts") == 0) {
				if(worry != -1)
					symptoms[worry] += 5;
					
				if(vertigo!= -1)
					symptoms[vertigo] += 5;

				if(fatigue != -1)
					symptoms[fatigue] += 5;

				if(dizziness != -1)
					symptoms[dizziness] += 10;

				if(palpitation != -1)
					symptoms[palpitation] += 20;

				if(cardio!= -1)
					symptoms[cardio] += 10;

				if(backPain != -1)
					symptoms[backPain] += 10;

				if(chestPain != -1)
					symptoms[chestPain] += 10;

				if(shortBreath != -1)
					symptoms[shortBreath] += 20;

				if(abdominalPain != -1)
					symptoms[abdominalPain] += 10;

				if(sweat != -1)
					symptoms[sweat] += 5;

				if(headache != -1)
					symptoms[headache] += 15;

				if(nausea!= -1)
					symptoms[nausea] += 15;

				if(neckPain != -1)
					symptoms[neckPain] += 10;
					
				if (strId.compare("Gold") == 0 || strId.compare("Nickel") == 0|| strId.compare("Chromium") == 0) {
					if(fatigue != -1)
						symptoms[fatigue] += 5; //+5 = 10
				}
			}
			else if (strId.compare("Semen") == 0 || strId.compare("Pollen") == 0 || strId.compare("Formaldehyde") == 0
			|| strId.compare("Fungicide") == 0 || strId.compare("Nail Polish") == 0 || strId.compare("Hair dye") == 0
			|| strId.compare("Paraphenylenediamine") == 0) {
				if(worry != -1)
					symptoms[worry] += 10;

				if(vertigo != -1)
					symptoms[vertigo] += 20;

				if(fatigue != -1) 
					symptoms[fatigue] += 5;

				if(dizziness!= -1)
					symptoms[dizziness] += 10;

				if(palpitation != -1)
					symptoms[palpitation] += 20;

				if(cardio != -1)
					symptoms[cardio] += 10;

				if(shortBreath!= -1)
					symptoms[shortBreath] += 20;

				if(abdominalPain != -1)
					symptoms[abdominalPain] += 10;
					
				if (strId.compare("Semen") == 0) {
					if(nausea != -1)
						symptoms[nausea] += 5; 
				}
				else if (strId.compare("Pollen") == 0) {
					if(fatigue != -1)
						symptoms[fatigue] += 5;
						
					if(cough != -1)
						symptoms[cough] += 15;
						
					if(headache != -1)
						symptoms[headache] += 10; 

					if(earPain != -1)
						symptoms[earPain] += 15; 
				}
				else if (strId.compare("Formaldehyde") == 0 || strId.compare("Fungicide") == 0 || strId.compare("Nail Polish") == 0
				|| strId.compare("Hair dye") == 0 || strId.compare("Paraphenylenediamine") == 0) {
					if(backPain != -1)
						symptoms[backPain] += 10;

					if(chestPain!= -1)
						symptoms[chestPain] += 10;

					if(cough != -1)
						symptoms[cough] += 15;

					if(sweat != -1) 
						symptoms[sweat] += 5;

					if(headache != -1)
						symptoms[headache] += 15;

					if(nausea != -1) 
						symptoms[nausea] += 15; 
				}
			}
		}
	}

	// We check if the patient has genetic issue
	if (nbGen != 0) {
		short genetic [nbGen];
		patient.getGenetic(genetic);
		for (int i = 0; i < nbGen; i++) {
			strId = retrieveNameGenetic(con, genetic[i]);
			if (strId.compare("Ehler-Danlos Syndrome") == 0) {
				if(worry != -1)
					symptoms[worry] += 15;

				if(backPain != -1)
					symptoms[backPain] += 10;

				if(chestPain != -1)
					symptoms[chestPain] += 10;

				if(distendedAbdomen != -1)
					symptoms[distendedAbdomen] += 10;

				if(abdominalPain != -1)
					symptoms[abdominalPain] += 15;

				if(flatulence != -1)
					symptoms[flatulence] += 5;

				if(neckPain != -1)
					symptoms[neckPain] += 10;
			}
			else if (strId.compare("PCC Deficiency") == 0) {
				if(worry != -1)
					symptoms[worry] += 20;

				if(palpitation != -1)
					symptoms[palpitation] += 10;

				if(cardio != -1)
					symptoms[cardio] += 15;

				if(bradycardia != -1)
					symptoms[bradycardia] += 10;

				if(backPain != -1)
					symptoms[backPain] += 10;

				if(chestPain != -1)
					symptoms[chestPain] += 10;

				if(headache != -1)
					symptoms[headache] += 10;

				if(nausea!= -1)
					symptoms[nausea] += 20;

				if(neckPain != -1)
					symptoms[neckPain] += 10;

				if(thirst != -1)
					symptoms[thirst] += 5;
			}
			else if (strId.compare("Pfeiffer Syndrome") == 0) {
				if(worry != -1)
					symptoms[worry] += 15;

				if(shortBreath != -1)
					symptoms[shortBreath] += 10;

				if(headache != -1)
					symptoms[headache] += 20;

				if(neckPain != -1)
					symptoms[neckPain] += 10;

				if(earPain != -1)
					symptoms[earPain] += 10;
			}
			else if (strId.compare("Brody myopathy") == 0) {
				if(worry != -1)
					symptoms[worry] += 10;
					
				if(backPain != -1)
					symptoms[backPain] += 10;

				if(chestPain != -1)
					symptoms[chestPain] += 10;

				if(abdominalPain != -1)
					symptoms[abdominalPain] += 5;

				if(headache != -1)
					symptoms[headache] += 10;

				if(neckPain != -1)
					symptoms[neckPain] += 10;
			}
			else if (strId.compare("Neurofibromatosis type II") == 0) {
				if(worry != -1)
					symptoms[worry] += 15;
			}
			else if (strId.compare("Neurofibromatosis type I") == 0) {
				if(worry != -1)
					symptoms[worry] += 10;
			}
			else if (strId.compare("Hemophilia") == 0) {
				if(worry != -1)
					symptoms[worry] += 10;

				if(backPain != -1)
					symptoms[backPain] += 5;

				if(chestPain != -1)
					symptoms[chestPain] += 10;

				if(abdominalPain != -1)
					symptoms[abdominalPain] += 15;

				if(headache != -1)
					symptoms[headache] += 10;

				if(neckPain != -1)
					symptoms[neckPain] += 15;

				if(nosebleed != -1)
					symptoms[nosebleed] += 10;
			}
			else if (strId.compare("Hemochromatosis") == 0) {
				if(worry != -1)
					symptoms[worry] += 15;

				if(fatigue!= -1)
					symptoms[fatigue] += 20;

				if(palpitation != -1)
					symptoms[palpitation] += 10;

				if(cardio != -1)
					symptoms[cardio] += 10;

				if(bradycardia != -1)
					symptoms[bradycardia] += 10;

				if(chestPain != -1)
					symptoms[chestPain] += 10;

				if(neckPain != -1)
					symptoms[neckPain] += 10;
			}
			else if (strId.compare("Alzheimer disease") == 0) {
				if(worry != -1)
					symptoms[worry] += 15;

				if(incoherent != -1)
					symptoms[incoherent] += 15;

				if(difficulty != -1)
					symptoms[difficulty] += 20;

				if(dizziness != -1)
					symptoms[dizziness] += 10;
			}
			else if (strId.compare("Crohn Disease") == 0) {
				if(worry != -1)
					symptoms[worry] += 15;

				if(fatigue != -1)
					symptoms[fatigue] += 20;

				if(anorexia != -1)
					symptoms[anorexia] += 5;

				if(underweight != -1)
					symptoms[underweight] += 10;

				if(abdominalPain != -1)
					symptoms[abdominalPain] += 20;

				if(flatulence != -1)
					symptoms[flatulence] += 15;
					
				if(nausea != -1)
					symptoms[nausea] += 10;
			}
			else if (strId.compare("Down Syndrome") == 0) {
				if(worry != -1)
					symptoms[worry] += 5;

				if(palpitation != -1)
					symptoms[palpitation] += 20;

				if(cardio != -1)
					symptoms[cardio] += 20;

				if(bradycardia != -1)
					symptoms[bradycardia] += 20;

				if(shortBreath != -1)
					symptoms[shortBreath] += 15;
					
				if(nosebleed != -1)
					symptoms[nosebleed] += 10;

				if(earPain != -1)
					symptoms[earPain] += 10;
			}
		}
	}

	for(int i(0); i < nbSym; i++) {
		if(symptoms[i] > 100)
			symptoms[i] = 100;
	}

	// Creation of the file
	// 6 => genetic
	std::ostringstream a;
    a << patient.getIdSocdet();
   	int w = writeFileAnalyse(symptoms, nbSym, patient.getIdSocdet(), 6, PATHTOALEDAPP);
	if (w == 0) {
		std::cout << "Patient file was not writing correctly\n" << std::endl;
	}
	else {
		//std::cout << "creation of : " + PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + a.str() + "/Patient_" + a.str() + "_GenAll.txt" << std::endl;
		std::cout << ">>>Patient"+ a.str() +" --> Creation of : Patient_" + a.str() + "_GenAll.txt" << std::endl;
	}
}

// Create the output file analysis
void createOutputAnalysis(MYSQL * con, Patient &patient) {
    /* CONNEXION TO THE API TO GET THE ID OF THE SYMPTOMS */

	// We find the number of symptoms on the table using mysql_num_rows
	int nbSym = retrieveNbSymp(con);

	/* Create an array of short with the number of symptoms */
	short symptoms[(int)nbSym];

	// Initialisation of the array with 0
	for(int i(0); i < (int)nbSym; i++){
		symptoms[i] = 0;
	}

	short id = 0;
	short _weight = 0;
	// Filling of the symptoms

	// IMCI
	if(patient.getAge() != -1 && patient.getGender() != -1 && patient.getIMCI() != 0) {
		_weight = imciAnalyse(patient.getIMCI(), patient.getAge(), patient.getGender());

		if(_weight > 0) {
			id =  getIdSymptom(con, "overweight");
			if(id != -1)
				symptoms[id] = _weight;
		}
		else if (_weight < 0) {
			if( _weight == -25) {
				id =  getIdSymptom(con, "anorexia");
				if(id != -1)
					symptoms[id] = - _weight;
			}

			id =  getIdSymptom(con, "underweight");
			if(id != -1)
				symptoms[id] = - _weight;
		}
	}

	// IMG
	if(patient.getAge() != -1 && patient.getGender() != -1 && patient.getIMG() != 0) {
		_weight = imgAnalyse(patient.getIMG(), patient.getGender());
		if (_weight < 0) {
			id =  getIdSymptom(con, "underweight");
			if(id != -1)
				symptoms[id] = symptoms[id] - _weight;
		}
		else if (_weight > 0){
			id =  getIdSymptom(con, "overweight");
			if(id != -1)
				symptoms[id] = symptoms[id] + _weight;
		}
	}

    // Creation of the file
	// 3 => patient file
	std::ostringstream a;
    a << patient.getIdSocdet();
   	int w = writeFileAnalyse(symptoms, nbSym, patient.getIdSocdet(), 3, PATHTOALEDAPP);
	if (w == 0) {
		std::cout << "Patient file was not writing correctly\n" << std::endl;
	}
	else {
		//std::cout << "creation of : " + PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + a.str() + "/Patient_" + a.str() + "_PF.txt" << std::endl;
		std::cout << ">>>Patient"+ a.str() +" --> Creation of : Patient_" + a.str() + "_PF.txt" << std::endl;
	}
}
