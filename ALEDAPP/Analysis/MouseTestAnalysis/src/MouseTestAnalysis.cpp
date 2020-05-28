#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <mysql.h>
#include "Patient.h"
#include "api.h"
#include "auth.h"

using namespace std;
using namespace cv;



string PATHTOALEDAPP ;



#define INPUT_SIZE 1000
#define PI 3.14159265359
#define DEGREEANGLE 90


void launcher ( int argc, char** argv );

string returnFolderAddress (int choice);

void launchDataBaseProcess (int choice, char *argv2);

void stringToPoints (string &stringCoordinates, vector<cv::Point> &pointsCoordinates, vector<int> &pointsTime);

bool fileToPoints (string fileAddress, vector<cv::Point> &pointsCoordinates, vector<int> &pointsTime);

bool FileAddressTodisplay (string fileAddress, Mat &displayMatFile);

int differenceWithBestTrack (string fileAddress);

void displayAllDataBase ();





// analysis functions

int userTime (string fileAddress);

int userTurnsNb (string fileAddress, double degree);

double angleFromThreePoints (Point b, Point a, Point c);

double euclDist (Point a, Point b);

vector<double> fileAnalysis(string fileAddress);
// return a vector with 3 values : 0] time 1] turnNb 2] dist from best track

void displayVectorFileAnalyze(vector<double> vectorFileAnalyze);

vector<double> analyzeDataBase ();

vector<double> fileAnalyzeWithDB (string fileAddress);
void displayFileAnalyzeWithDB (vector<double>  vectorFileAnalyzeWithDB);


void returnSymptomsFromAnalyis (string fileAddress);



// classic use : creation of output file

void createOutputAnalysis (string fileAddress, int id_socdet);



int main( int argc, char** argv )
{
    launcher(argc, argv);

	return 0;
}



void launcher ( int argc, char** argv )
{
    cout << endl;

    if( argc == 1) // let the user select choice 1 or 2
    {
        //info
        printf("\n\n\n");
        cout << "1st arg : '10' // 2nd arg : Patient ID // 3rd PATHTOALEDAPP \n";

        printf("If you want to save a mouse data:\n 1] Chosen number // 2] Data from webApp // 3] PATHTOALEDAPP \n");
        printf("healfy :'1'\n");
        printf("hesitation : '2' \n");
        printf("Tremor : '3' \n");
        printf("incoherent : '4' \n");
        printf("Patient file : '5' \n");
        printf("\n\n\n");


        printf("If you want to analyse and display a mouse test : 1] 'ana' // 2] the file address // 3] PATHTOALEDAPP \n");
        printf("If you want to see all test from the data base with analysis : 1] '1' // 2] '0' // 3]  PATHTOALEDAPP\n");
        printf("If you want to see the global results of data base analysis : 1] '2' // 2] '0' // 3]  PATHTOALEDAPP\n");

        exit(0);
    }
    else if ( (argc == 4) && strcmp(argv[1],"1")==0 && strcmp(argv[2],"0")==0)
    {
        PATHTOALEDAPP = argv[3];
        displayAllDataBase();
    }
    else if ( (argc == 4) && strcmp(argv[1],"2")==0 && strcmp(argv[2],"0")==0)
    {
        PATHTOALEDAPP = argv[3];
        vector<double> result = analyzeDataBase();
        for(int i = 0; i < 12; i=i+3)
        {
            cout << "result for section n° " <<  i/3+1 << endl;
            cout << "average user time is : " << result[i] << " milliseconds" << endl ;
            cout << "average turn number < "<< DEGREEANGLE << " is : " << result[i+1] << endl ;
            cout << "dist with best track is : " << result[i+2] << endl << endl ;
        }
    }
    else if ((argc == 4) && strcmp(argv[1],"ana")==0)
    {
        PATHTOALEDAPP = argv[3];
        Mat displayMatFile(1000,1000,CV_8UC3, Scalar(255,255,255));
        FileAddressTodisplay (argv[2], displayMatFile);

        // file analysis :
        vector<double> fileAnalyzeVector = fileAnalysis(argv[2]);
        displayVectorFileAnalyze(fileAnalyzeVector);

        // file analysis with the Data Base
        vector<double> vectorFileAnalyzeWithDB = fileAnalyzeWithDB (argv[2]);
        displayFileAnalyzeWithDB (vectorFileAnalyzeWithDB);

        // symptom :
        returnSymptomsFromAnalyis(argv[2]);

        namedWindow( "displayMatFile", WINDOW_AUTOSIZE );
        imshow("displayMatFile", displayMatFile);

        if (waitKey(0) > 0 )
        {
            destroyWindow("Picture in shades of grey");
            destroyWindow("Processed picture");
        }
    }

    else if ((argc == 4) && strcmp(argv[1],"10")== 0)
    {
        PATHTOALEDAPP = argv[3];
        string pathToInput = (string)PATHTOALEDAPP + "/ReceivedData/" + "Patient" + argv[2] + "/" + "Patient" + argv[2] + "MT.txt";
    	char patientId = stoi(argv[2]);
	
	FILE * f = NULL;
    	f = fopen(pathToInput.c_str(), "rt");
    	
    	if(f == NULL) {
		std::cout << "The file " << pathToInput << " is not present" << std::endl;
		exit(0);
	}
	else {
		fclose(f);
        	createOutputAnalysis (pathToInput, patientId);
	}
    }

    else if(argc == 3)
    {
        PATHTOALEDAPP = argv[3];
        int choice = argv[1][0] - '0';
        launchDataBaseProcess(choice, argv[2]);
    }
}


string returnFolderAddress (int choice)
{
    string address;
    switch (choice)
    {
        case 1 :
        address = PATHTOALEDAPP+"/Analysis/MouseTestAnalysis/DataBase/HealthyMT/HealthyMT";
        break;

        case 2 :
        address = PATHTOALEDAPP+"/Analysis/MouseTestAnalysis/DataBase/HesitationMT/HesitationMT";
        break;

        case 3 :
        address = PATHTOALEDAPP+"/Analysis/MouseTestAnalysis/DataBase/TremorMT/TremorMT";
        break;

        case 4 :
        address = PATHTOALEDAPP+"/Analysis/MouseTestAnalysis/DataBase/IncoherentMT/IncoherentMT";
        break;

        case 5 :
        address = PATHTOALEDAPP+"/Analysis/MouseTestAnalysis/DataBase/PatientMT/PatientMT";
        break;
    }

    return address;
}


void launchDataBaseProcess (int choice, char *argv2)
{
    // save in the collection
    // they will be named HealthyMT1 / HealftyMT2 / ...
    // they will be named HesitationMT1 / HesitationMT2 / ...
    // they will be named TremorMT1 / TremorMT2 / ...
    // they will be named IncoherentMT1 / IncoherentMT2 / ...


    string generic = returnFolderAddress (choice);


    // give the good name to the new and last one
    string fileName;
    bool foundLast = false;
    int i = -1;
    while(!foundLast)
    {
        i++;
        char buffer[10];
        sprintf(buffer, "%d", i);
        fileName=generic+buffer;
        fstream fileStream;
        fileStream.open(fileName);
        if (fileStream.fail()) {
            cout << "creation of : " << fileName << endl;
            foundLast = true;
        }
    }

    // create a file with the information of argv[2] = stringCoordinates
    ofstream newFile(fileName);
    newFile << argv2;
    newFile.close();
}





void stringToPoints (string &stringCoordinates, vector<cv::Point> &pointsCoordinates, vector<int> &pointsTime)
{

    cv::Point currentPoint;
    int currentTime;

    // convert to stringstream
    stringstream STCoordinates(stringCoordinates);

    // adapte to webApp output :
    bool process = true;
    while(!STCoordinates.eof() && process)
    {

        // check if this is the END or not
        string checkEND;
        STCoordinates >> checkEND;

       // cout << "checkEND=" << checkEND << endl;

        if (checkEND == "END")
        {
            process = false;
        }
        else
        {
            currentPoint.x = stoi(checkEND);

            STCoordinates.ignore(2, ',');

            STCoordinates >> currentPoint.y;
            STCoordinates.ignore(3, '=');

            STCoordinates >> currentTime;
            STCoordinates.ignore(1, ';');

            pointsCoordinates.push_back(currentPoint);
            pointsTime.push_back(currentTime);
        }

    }
}


bool fileToPoints (string fileAddress, vector<cv::Point> &pointsCoordinates, vector<int> &pointsTime)
{
    string stringCoordinates;
    // open file
    ifstream mtFile(fileAddress);

    if (mtFile.is_open())
    {
        //cout << "Open file : " << fileAddress << endl;
        getline (mtFile,stringCoordinates);
        mtFile.close();

        stringToPoints (stringCoordinates, pointsCoordinates, pointsTime);
        return true ;
    }
    else
    {
        cout << "can't open file :" << fileAddress << endl;
        return false;
    }
}




bool FileAddressTodisplay (string fileAddress, Mat &displayMatFile)
{

    // open file
    ifstream mtFile(fileAddress);
    string stringCoordinates;
    vector<Point> pointsCoordinates;
    vector<int> pointsTime;

    if (mtFile.is_open())
    {
        cout << "Open file : " << fileAddress << endl << endl;
        getline (mtFile,stringCoordinates);
        mtFile.close();

        stringToPoints (stringCoordinates, pointsCoordinates, pointsTime);

        int c1 = 0;
        //int c2 = 0;
        //int c3 = 0;
        bool upDown = true;
        for (int i=0; i < (int)pointsCoordinates.size(); i++)
        {
            circle( displayMatFile, pointsCoordinates[i] ,5 , Scalar( c1, 0, 0 ), FILLED, LINE_8 );

            if (upDown)
                c1++;
            else
                c1--;

            if (c1==255)
            {
                upDown = false;
                //c2=c2+5;
            }
            else if (c1==0)
            {
                upDown = true;
            }
        }

        return true;

    }
    else
    {
        cout << "Can't open file : " << fileAddress << endl;
        return false;
    }
}


void displayAllDataBase ()
{
    bool end = false;

    string fileAddress;

    int folderNb = 1;
    int fileNb = 0;


    printf("\n\nTo see next result select picture and press a key.\n\n");


    while (!end)
    {
        // see if file exist and display it is this order :
        // 1) HealthyMT /  2) HesitationMT / 3) TremorMT / 4) IncoherentMT

        fileAddress = returnFolderAddress (folderNb);

        Mat displayMatFile(1000,1000,CV_8UC3, Scalar(255,255,255));


        char buffer[10];
        sprintf(buffer, "%d", fileNb);
        fileAddress = fileAddress + buffer;

        if (FileAddressTodisplay(fileAddress,displayMatFile))
        {
            vector<double> fileAnalyzeVector = fileAnalysis(fileAddress);
            displayVectorFileAnalyze(fileAnalyzeVector);

            namedWindow( "displayMatFile", WINDOW_AUTOSIZE );
            imshow("displayMatFile", displayMatFile);
            fileNb++;
            cout << endl ;

            if (waitKey(0) > 0 )
            {
                destroyWindow("Picture in shades of grey");
                destroyWindow("Processed picture");
            }

        }
        else
        {
            cout << endl ;
            fileNb = 0;
            folderNb ++;
        }

        if(folderNb == 5)
            end = true;

    }
}

// --------------------------------------------------------------------------------------------------------
// ---------------------------------Analysis functions ----------------------------------------------------
// --------------------------------------------------------------------------------------------------------


int userTime (string fileAddress)
{
    vector<cv::Point> pointsCoordinates;
    vector<int> pointsTime;

    // collect info
    fileToPoints (fileAddress, pointsCoordinates, pointsTime);

    int userTime = pointsTime.back() - pointsTime.front();

    return userTime;
}



int userTurnsNb (string fileAddress, double degree)
{
    vector<cv::Point> pointsCoordinates;
    vector<int> pointsTime;
    int turnNb = 0;

    // collect info
    fileToPoints (fileAddress, pointsCoordinates, pointsTime);

    for (int i = 0; i < pointsCoordinates.size()-2; i++)
    {
        if (angleFromThreePoints(pointsCoordinates[i], pointsCoordinates[i+1], pointsCoordinates[i+2]) <= degree)
            turnNb++;
    }

    return turnNb;
}



double angleFromThreePoints (Point b, Point a, Point c)
{
    // we apply the scalar product  :   v(ab).v(ac) / ( ||ab|| * ||ac|| ) = cos(a)
    double result = ( (b.x - a.x) * (c.x - a.x) + (b.y - a.y)*(c.y - a.y) ) /
        sqrt(  ( (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y) ) *
            (    (c.x - a.x)*(c.x - a.x) + (c.y - a.y)*(c.y - a.y) )  + 0.0000000001 );
    result = acos (result) * 180.0 / PI;
    return result;

}



int differenceWithBestTrack (string fileAddress)
{
    vector<cv::Point> pointsCoordinates;
    vector<int> pointsTime;
    // collect info
    fileToPoints (fileAddress, pointsCoordinates, pointsTime);


    vector<cv::Point> bestPointsCoordinates;
    vector<int> bestPointsTime;
    int difference = 0;
    // collect info
    fileToPoints (PATHTOALEDAPP+"/Analysis/MouseTestAnalysis/DataBase/BestTrack/BestTrack0", bestPointsCoordinates, bestPointsTime);



    for (int i = 0; i < pointsCoordinates.size(); i++)
    {
        // shearch the closest point of bestTrack to pointsCoordinates[i]
        int currentMin = 1000;
        for (int j = 0; j < bestPointsCoordinates.size(); j++)
        {
            if(euclDist(pointsCoordinates[i], bestPointsCoordinates[j]) < currentMin)
                currentMin = euclDist(pointsCoordinates[i], bestPointsCoordinates[j]);
        }
       // cout << "best eucl dist with " << i << "point is : " <<  currentMin << endl;
        difference = difference + currentMin;
    }

    return difference;
}



double euclDist (Point a, Point b)
{
    return sqrt( (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) );
}


vector<double> fileAnalysis(string fileAddress)
{
    vector<double> analyzedData;
    analyzedData.push_back(userTime (fileAddress));
    analyzedData.push_back(userTurnsNb (fileAddress,DEGREEANGLE));
    analyzedData.push_back(differenceWithBestTrack(fileAddress));

    return analyzedData;
}

void displayVectorFileAnalyze(vector<double> vectorFileAnalyze)
{
    cout << "user time is " << vectorFileAnalyze[0] << " milliseconds" << endl ;
    cout << "turnNb < "<<DEGREEANGLE<<" is " << vectorFileAnalyze[1] << endl ;
    cout << "dist with best track is : " << vectorFileAnalyze[2] << " pixels" << endl << endl;
}



vector<double> analyzeDataBase ()
{
    // this function return a vector with 12 averages values :
    // from 0 to 2 : time/turns/dist for healfyMT
    // from 3 to 5 : time/turns/dist for hesitationMT
    // from 6 to 8 : time/turns/dist for TremorMT
    // from 9 to 11 : time/turns/dist for IncoherentMT
    vector<double> result;

    double timeAverage = 0;
    double turnAverage = 0;
    double distAverage = 0;

    // go through all data
    bool end = false;
    string fileAddress;
    int folderNb = 1;
    int fileNb = 0;

    while (!end)
    {

        fileAddress = returnFolderAddress (folderNb);

        char buffer[10];
        sprintf(buffer, "%d", fileNb);
        fileAddress = fileAddress + buffer;
        fstream fileStream;
        fileStream.open(fileAddress);

        if (!fileStream.fail())
        {

            timeAverage = timeAverage + userTime (fileAddress);
            turnAverage = turnAverage + userTurnsNb (fileAddress,DEGREEANGLE);
            distAverage = distAverage + differenceWithBestTrack(fileAddress);

            fileNb++;
        }
        else
        {
            timeAverage = timeAverage/fileNb;
            turnAverage = turnAverage/fileNb;
            distAverage = distAverage/fileNb;

            result.push_back(timeAverage);
            result.push_back(turnAverage);
            result.push_back(distAverage);

            timeAverage = 0;
            turnAverage = 0;
            distAverage = 0;

            fileNb = 0;
            folderNb ++;
        }

        if(folderNb == 5)
            end = true;

    }

    return result;
}



vector<double> fileAnalyzeWithDB (string fileAddress)
{
    // return 2 of 4 classes ( Healthy Hesitation Tremor Incoherent )
    // with a propability


    // get information
    double testTime = userTime (fileAddress);
    double testTurn = userTurnsNb (fileAddress,DEGREEANGLE);
    double testDist = differenceWithBestTrack(fileAddress);

    vector<double> DBResult = analyzeDataBase();

    // test closeness with the 4 class
    vector<double> testClosenessWithDB(12,0);

    for (int i = 0; i<12 ; i=i+3)
    {
        testClosenessWithDB[i]=    testTime / DBResult[i];
        testClosenessWithDB[i+1]= testTurn / DBResult[i+1];
        testClosenessWithDB[i+2]= testDist / DBResult[i+2];
    }

   // for (int i = 0; i<12 ; i++)
    //    cout << "info num " << i << " : " << testClosenessWithDB[i] << endl;

    // limit the result to [0, 2] -> -100% or + 100%
    for (int i = 0; i<12 ; i++)
        if(testClosenessWithDB[i]>2)
            testClosenessWithDB[i]=2;
        else if (testClosenessWithDB[i]<0)
            testClosenessWithDB[i]=0;

    // the difference average on the 3 values of each classe
    vector<double> finaltestClosenessWithDB(4,0);

    for (int i = 0; i<4 ; i++)
    {
        finaltestClosenessWithDB[i]= abs((testClosenessWithDB[i*3 + 0] + testClosenessWithDB[i*3 + 1] + testClosenessWithDB[i*3 + 2] - 3 ) / 3) ;
    }

    //for (int i = 0; i<4 ; i++)
    //cout << "average value " << i << " : " << finaltestClosenessWithDB[i] << endl;

    // finally the percentage :
    for (int i = 0; i<4 ; i++)
    {
        finaltestClosenessWithDB[i] = (1 - finaltestClosenessWithDB[i]) * 100;
    }




    return finaltestClosenessWithDB;
}

void displayFileAnalyzeWithDB (vector<double>  vectorFileAnalyzeWithDB)
{

    cout << "Closeness percentage with HealthyMT : " << vectorFileAnalyzeWithDB[0] << " %" <<endl;
    cout << "Closeness percentage with Hesitation : " << vectorFileAnalyzeWithDB[1] << " %" <<endl;
    cout << "Closeness percentage with TremorMT : " << vectorFileAnalyzeWithDB[2] << " %" <<endl;
    cout << "Closeness percentage with IncoherentMT : " << vectorFileAnalyzeWithDB[3] << " %" <<endl;

}


// ---------------------------------------------------------------------------------
// -------------------------------- FINAL OUTPUT -----------------------------------
// ---------------------------------------------------------------------------------


void returnSymptomsFromAnalyis (string fileAddress)
{
    vector<double> vectorFileAnalyzeWithDB = fileAnalyzeWithDB (fileAddress);

    // 1. anxiety
    // 5. chill
    // 7. Delusions
    //11. Fatigue
    //14. Headache
    //21. Tremor
    //22. vertigo

    int symptomsCriticality [7][2] = {{1,0},{5,0},{7,0},{11,0},{14,0},{21,0},{22,0}};


    if (vectorFileAnalyzeWithDB[0] >= vectorFileAnalyzeWithDB[1] &&
        vectorFileAnalyzeWithDB[0] >= vectorFileAnalyzeWithDB[2] &&
        vectorFileAnalyzeWithDB[0] >= vectorFileAnalyzeWithDB[3])
        // if the main proba is healfy
    {
        // all go well, do nothing
    }


    else if (vectorFileAnalyzeWithDB[1] >= vectorFileAnalyzeWithDB[0] &&
        vectorFileAnalyzeWithDB[1] >= vectorFileAnalyzeWithDB[2] &&
        vectorFileAnalyzeWithDB[1] >= vectorFileAnalyzeWithDB[3])
        // if the main proba is hesitation
    {
        symptomsCriticality[3][1] = vectorFileAnalyzeWithDB[1];
        symptomsCriticality[4][1] = vectorFileAnalyzeWithDB[1];
        symptomsCriticality[6][1] = vectorFileAnalyzeWithDB[1];
    }


    else if (vectorFileAnalyzeWithDB[2] >= vectorFileAnalyzeWithDB[0] &&
        vectorFileAnalyzeWithDB[2] >= vectorFileAnalyzeWithDB[1] &&
        vectorFileAnalyzeWithDB[2] >= vectorFileAnalyzeWithDB[3])
        // if the main proba is Tremor
    {
        symptomsCriticality[0][1] = vectorFileAnalyzeWithDB[2];
        symptomsCriticality[1][1] = vectorFileAnalyzeWithDB[2];
        symptomsCriticality[5][1] = 100;
        symptomsCriticality[4][1] = vectorFileAnalyzeWithDB[2];
    }


    else if (vectorFileAnalyzeWithDB[3] >= vectorFileAnalyzeWithDB[0] &&
        vectorFileAnalyzeWithDB[3] >= vectorFileAnalyzeWithDB[1] &&
        vectorFileAnalyzeWithDB[3] >= vectorFileAnalyzeWithDB[2])
        // if the main proba is incoherent
    {
        symptomsCriticality[0][1] = vectorFileAnalyzeWithDB[3];
        symptomsCriticality[2][1] = vectorFileAnalyzeWithDB[3];
        symptomsCriticality[6][1] = vectorFileAnalyzeWithDB[3];
    }

    for (int i = 0; i<7; i++)
    {
        cout << "lvl of symptom number " << symptomsCriticality[i][0] << " is : " << symptomsCriticality[i][1] << endl;
    }

}



void createOutputAnalysis (string fileAddress, int id_socdet)
{

    	vector<double> vectorFileAnalyzeWithDB = fileAnalyzeWithDB (fileAddress);
	
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

	// We find the number of symptoms on the table using mysql_num_rows
	int nbSym = retrieveNbSymp(con);

	/* Create an array of short with the number of symptoms */
	short symptoms[(int)nbSym];

	// Initialisation of the array with 0
	for(int i(0); i < (int)nbSym; i++){
		symptoms[i] = 0;
	}

	short id = 0;

	// 0. no symptom -> no_symptom
	// 1. anxiety -> worry
    	// 5. chill -> chill
    	// 7. Delusions -> visual_hallucinations
    	//11. Fatigue -> fatigue
    	//14. Headache -> pain_chest
    	//21. Tremor -> tremor
    	//22. vertigo -> vertigo

    	if (vectorFileAnalyzeWithDB[0] >= vectorFileAnalyzeWithDB[1] &&
        	vectorFileAnalyzeWithDB[0] >= vectorFileAnalyzeWithDB[2] &&
        	vectorFileAnalyzeWithDB[0] >= vectorFileAnalyzeWithDB[3])
        	// if the main proba is healfy
    	{
        	// all go well, do nothing
    	}

    	else if (vectorFileAnalyzeWithDB[1] >= vectorFileAnalyzeWithDB[0] &&
        	vectorFileAnalyzeWithDB[1] >= vectorFileAnalyzeWithDB[2] &&
        	vectorFileAnalyzeWithDB[1] >= vectorFileAnalyzeWithDB[3])
        	// if the main proba is hesitation
    	{
		// fatigue
		id =  getIdSymptom(con, "fatigue");
		if(id != -1)
			symptoms[id] = vectorFileAnalyzeWithDB[1];

		// difficulty
		id =  getIdSymptom(con, "difficulty");
		if(id != -1)
			symptoms[id] = vectorFileAnalyzeWithDB[1];

		// vertigo
		id =  getIdSymptom(con, "vertigo");
		if(id != -1)
			symptoms[id] = vectorFileAnalyzeWithDB[1];

    	}


    	else if (vectorFileAnalyzeWithDB[2] >= vectorFileAnalyzeWithDB[0] &&
        	vectorFileAnalyzeWithDB[2] >= vectorFileAnalyzeWithDB[1] &&
        	vectorFileAnalyzeWithDB[2] >= vectorFileAnalyzeWithDB[3])
        	// if the main proba is Tremor
    	{
		// worry
		id =  getIdSymptom(con, "worry");
		if(id != -1)
			symptoms[id] = vectorFileAnalyzeWithDB[2];
		// chill
		id =  getIdSymptom(con, "chill");
		if(id != -1)
			symptoms[id] = vectorFileAnalyzeWithDB[2];
		// tremor
		id =  getIdSymptom(con, "tremor");
		if(id != -1)
			symptoms[id] = 100;
    	}


    	else if (vectorFileAnalyzeWithDB[3] >= vectorFileAnalyzeWithDB[0] &&
        	vectorFileAnalyzeWithDB[3] >= vectorFileAnalyzeWithDB[1] &&
        	vectorFileAnalyzeWithDB[3] >= vectorFileAnalyzeWithDB[2])
        	// if the main proba is incoherent
    	{
		// worry
		id =  getIdSymptom(con, "worry");
		if(id != -1)
			symptoms[id] = vectorFileAnalyzeWithDB[3];
		// visual_hallucinations
		id =  getIdSymptom(con, "visual_hallucinations");
		if(id != -1)
			symptoms[id] = vectorFileAnalyzeWithDB[3];
		// incoherent
		id =  getIdSymptom(con, "incoherent");
		if(id != -1)
			symptoms[id] = vectorFileAnalyzeWithDB[3];
		// vertigo
        	id =  getIdSymptom(con, "vertigo");
		if(id != -1)
			symptoms[id] = vectorFileAnalyzeWithDB[3];
    	}

	// We close the connection
	mysql_close(con);

    	// Creation of the file for the mouse test
	// 0 => mouse test
	std::ostringstream a;
    	a << id_socdet;
   	int w = writeFileAnalyse(symptoms, nbSym, id_socdet, 0, PATHTOALEDAPP);
	if (w == 0) {
		std::cout << "Mouse test file was not writing correctly\n" << std::endl;
	}
	else {
		//std::cout << "creation of : " + PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + a.str() + "/Patient_" + a.str() + "_MT.txt" << std::endl;
        std::cout << ">>>Patient"+a.str()+" --> Creation of : Patient_" + a.str() + "_MT.txt" << std::endl;
	}
}


