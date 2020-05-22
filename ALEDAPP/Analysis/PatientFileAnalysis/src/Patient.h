#ifndef PATIENT_H
#define PATIENT_H

#include "Date.h"

class Patient
{
    private:
        // symptoms
        short * symptoms;
        int nbSym;

        // informations
        int age;
        int gender;
        int height;
        int weight;

        // allergy
        short * allergy;
        int nbAllergy;

        // genetic disease
        short * genetic;
        int nbGenetic;

        // picture of the patient
        std::string picture;

        // id
        int id_socdet;
        int id_medrec;
        int id_blgrp;

        // IMCI
        float imci;
        // IMG
		float img;

    public:
		// Constructor
        Patient() : symptoms(NULL), nbSym(0), age(-1), gender(-1),
					height(0), weight(0), allergy(NULL),  nbAllergy(0),
					genetic(NULL), nbGenetic(0), picture("None"),
					id_socdet(0), id_medrec(0), id_blgrp(0),
					imci(0.0), img(0.0) {}

		// Destructor
        virtual ~Patient() {
			if (symptoms != NULL){
				delete [] symptoms;
			}
			if (allergy != NULL){
				delete [] allergy;
			}
			if (genetic != NULL){
				delete [] genetic;
			}
		}

		// Symptoms
        short * getSymptoms() { return symptoms; }
        void setSymptoms(short * val, int nb) {
			if(symptoms == NULL)
				symptoms = new short[nb];

			for(int i(0); i < nb; i++)
				symptoms[i] = val[i];
		}

        int getNbSym() { return nbSym; }
        void setNbSym(int val) { nbSym = val; }

		// Age
        int getAge() { return age; }
        void setAge(int val) { age = val; }
		void computeAge(Date birthdate, Date today) {
            int year = today.getYear();
            int month = today.getMonth();
            int day = today.getDay();

            // if the day of the birthdate is higher than the day of today
            // then we soustract 1 to the number of months
            if (birthdate.getDay() > day) {
                    month --;
            }

            // if the month of birthdate is higher than the month of today
            // then we soustract 1 to the number of year
            if (birthdate.getMonth() > month) {
                year = year -1 ;
            }

            // Now we calculate the number of year doing the substraction of the current year with the calculate year
            age = year - birthdate.getYear();
        }

		// Gender
        int getGender() { return gender; }
        void setGender(int val) { gender = val; }

		// Height
        int getHeight() { return height; }
        void setHeight(int val) { height = val; }

		// Weight
        int getWeight() { return height; }
        void setWeight(int val) { weight = val; }

		// Allergy
        void getAllergy(short * all) {
        	if(allergy != NULL) {
        		if (all == NULL)
					all = new short[nbAllergy];

        		for(int i(0); i < nbAllergy; i++)
        			all[i] = allergy[i];
        	}
        }
        void setAllergy(short * val, int nb) {
			if (allergy == NULL)
				allergy = new short[nb];

			for (int i(0); i < nb; i++)
				allergy[i] = val[i];
		}

        int getNbAllergy() { return nbAllergy; }
        void setNbAllergy(int val) { nbAllergy = val; }

		// Genetic
        void getGenetic(short * gen) {
        	if(genetic != NULL) {
        		if (gen == NULL)
					gen = new short[nbGenetic];

        		for(int i(0); i < nbGenetic; i++)
        			gen[i] = genetic[i];
        	}
        }
        void setGenetic(short * val, int nb) {
			if (genetic == NULL)
				genetic = new short[nb];
			for (int i(0); i < nb; i++)
				genetic[i] = val[i];
		}

        int getNbGenetic() { return nbGenetic; }
        void setNbGenetic(int val) { nbGenetic = val; }

		// Picture
        std::string getPicture() { return picture; }
        void setPicture(std::string val) { picture = val; }

		// id_socdet
        int getIdSocdet() { return id_socdet; }
        void setIdSocdet(int val) { id_socdet = val; }

		// id_medrec
        int getIdMedrec() { return id_medrec; }
        void setIdMedrec(int val) { id_medrec = val; }

		// id_blgrp
        int getIdBlgrp() { return id_blgrp; }
        void setIdBlgrp(int val) { id_blgrp = val; }

        // IMCI
        void computeIMCI(){
        	if(weight != 0 && height != 0) {
        		imci = (float) ((float) weight / (float) ((float) height/100 * (float) height/100));
        	}
        }
        float getIMCI() { return imci; }
        void setIMCI(float val) { imci = val; }

        // IMG
        void computeIMG(){
        	if(imci != 0 && age != -1 && gender != -1) {
        		if( age > 15 ) {
        			if(gender == 0) {
        				img = 1.2 * imci + 0.23 * age - 16.2;
        			}
        			else {
        				img = 1.2 * imci + 0.23 * age - 5.4;
        			}
        		}
        		else {
        			if(gender == 0) {
        				img = 1.51 * imci + 0.7 * age - 2.2;
        			}
        			else {
        				img = 1.51 * imci + 0.7 * age + 1.4;
        			}
        		}
        	}
        }
        float getIMG() { return img; }
        void setIMG(float val) { img = val; }

        // Print the information about the patient
        void printPatient() {
			std::cout << "PATIENT" << std::endl;
			if (id_socdet != 0) {
				std::cout << "Id_socdet : " << id_socdet << std::endl;

				if(age != -1) {
					std::cout << "Age : " << age << std::endl;
				}
				else {
					std::cout << "The age is not filled" << std::endl;
				}

				if(gender != -1) {
					std::cout << "Gender : " << gender << std::endl;
				}
				else {
					std::cout << "The gender is not filled" << std::endl;
				}

				if (picture.compare("None") != 0) {
					std::cout << "picture path : " << picture << std::endl;
				}
				else {
					std::cout << "The picture is not filled" << std::endl;
				}

				if (id_medrec != 0) {
					if (id_blgrp != 0) {
						std::string str;

						if (id_blgrp == 1)
							str = "A-";
						else if (id_blgrp == 2)
							str = "B-";
						else if (id_blgrp == 3)
							str = "0-";
						else if (id_blgrp == 4)
							str = "AB-";
						else if (id_blgrp == 5)
							str = "AB+";
						else if (id_blgrp == 6)
							str = "0+";
						else if (id_blgrp == 7)
							str = "B+";
						else if (id_blgrp == 8)
							str = "A+";

						std::cout << "Id_blgrp : " << str << std::endl;
					}
					else {
						std::cout << "The blood group is no filled" << std::endl;
					}

					if (height != 0) {
						std::cout << "Height : " << height << std::endl;
					}
					else {
						std::cout << "The height is not filled" << std::endl;
					}

					if (weight != 0) {
						std::cout << "Weight : " << weight << std::endl;
					}
					else {
						std::cout << "The weight is not filled" << std::endl;
					}

					std::cout << "# of allergy : " << nbAllergy << std::endl;
					if (nbAllergy != 0) {
						for (int i(0); i < nbAllergy; i++) {
							std::cout << "allergy # " << i + 1 << " : " << allergy[i] << std::endl;
						}
					}
					std::cout << "# of genetic diseases : " << nbGenetic << std::endl;
					if (nbGenetic != 0) {
						for (int i(0); i < nbGenetic; i++) {
							std::cout << "genetic # " << i + 1 << " : " << genetic[i] << std::endl;
						}
					}
					if(imci != 0) {
						std::cout << "IMCI : " << imci << std::endl;
					}
					if(img != 0) {
						std::cout << "IMG : " << imci << std::endl;
					}
				}
				else {
					std::cout << "The patient has no medical record" << std::endl;
				}

			}
			else
				std::cout << "\nPatient not in the database" << std::endl;
		}
};

#endif // PATIENT_H
