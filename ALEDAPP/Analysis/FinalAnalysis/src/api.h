#ifndef API_H
#define API_H

/* SYMPTOMS */

// To get the number of symptoms in the database
int retrieveNbSymp(MYSQL *con);

// To get the id of a symptom in the database
short getIdSymptom(MYSQL *con, std::string symptom);


/* PATIENT FILE */

// Count the number of allergy
int retrieveNbAllergy(MYSQL *con, int id_medrec);

// Count the number of genetic diseases
int retrieveNbGenetic(MYSQL *con, int id_medrec);

// retreive the gender of the patient
int retrieveGender(MYSQL *con, int id_socdet);

// retreive the path of the picture of the patient
std::string retrievePicturePath(MYSQL *con, int id_socdet);

// retreive the id of the medical record of the patient
int retrieveIdMedrec(MYSQL *con, int id_socdet);

// retrieve the height of the patient
int retrieveHeight(MYSQL *con, int id_medrec);

// retrieve the weight of the patient
int retrieveWeight(MYSQL *con, int id_medrec);

// retrieve the blood groop id of the patient
int retrieveBloodGroup(MYSQL *con, int id_medrec);

// retrieve all the allergies of the patient
void retrieveAllergy(MYSQL *con, int id_medrec, int nbAllergy, short * allergy);

// retrieve all the genetic disease of the patient
void retrieveGenetic(MYSQL *con, int id_medrec, int nbGenetic, short * genetic);

// retrieve the birthdate of the patient
std::string retrieveBirthdate(MYSQL *con, int id_socdet);

// retreive the name of the genetic diseases
std::string retrieveNameGenetic(MYSQL *con, int id_gen);

// retreive the name of the allergy
std::string retrieveNameAllergy(MYSQL *con, int id_allergy);

//Convert a string into a date
Date stringToDate(std::string str_date);

// Create and fill an object of the class Patient for the patient of the id id_socdet
Patient createPatient(MYSQL * con, int id_socdet, int std);


/* FINAL ANALYSIS */

// Returns the number of symptoms detected
short nbSymptomNonNull(short * symptoms, int nbSym);

// Fill the symptoms with random values
int fillRandomly(short * symptoms, int nbSym);

// To read the final analyse file
int readFileFinalAnalyse(int id_socdet, unsigned long timestamp, std::string pathToAled, int std);

// To read the analyse file : reading of the txt file for the intermediary test
int readFileAnalyse(short * symptoms, int nbSym, int id_socdet, int choice, std::string pathToAled);

// Check if a file is in a directory
int isPresent(std::string test, std::string pathToAled);

// To merge all the files together in order to obtain all the symptoms of the different analysis
int mergeFile(short * symptoms, int nbSym, int id_socdet, std::string pathToAled);

// To write the analyse file : creation of the txt file for the intermediary test
int writeFileAnalyse(short * symptoms, int nbSym, int id_socdet, int choice, std::string pathToAled);

// To write the final analyse file
unsigned long writeFileFinalAnalyse(short * symptoms, int nbSym, int globalCriticality, int id_socdet, std::string pathToAled, int std);

#endif //API_H

