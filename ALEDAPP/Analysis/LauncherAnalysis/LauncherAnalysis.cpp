// ANALYSIS LAUNCHER

#include <cstdio>
#include <stdio.h>
#include <cstdlib> // file access
#include <iostream> // file access
#include <fstream>
#include <string> // string
#include <cstring> // string
#include <thread> // thread
#include <unistd.h> // sleep
#include <vector>

#include <time.h>

#include <sys/types.h>
#include <dirent.h>

using namespace std;

#define PATHTOALEDAPP "/home/herrygabriel/ALEDAPP"


void launcherManager (string choice);
bool folderFinder (vector<string> &patientIDs);
void processAPatientFile(string patientID, string choice);
void threadDescription (int choice, string patientID, string pathToAled);
bool PresentInVector (vector<int> processedVector, int ID);


int main( int argc, char** argv )
{

    if (argc == 1)
    {
    	cout << "Classic mode :" << endl;
        cout << "1st arg : '10' " << endl;
        cout << "this program will create folder for answer and call : KeyboardTest -> MouseTest -> " << endl;
        cout << "KeyboardTest -> MouseTest -> EyesBlinking -> CardiacFrequency -> FormAnalysis -> Patient fileAnalysis -> FinalAnalysis" << endl;
        cout << "LauncherAnalysis erase input date and produced data in FinalAnalysis/PreviousOutputs" << endl;
        //cout << "It you don't want to erase inputs data launch the Launcher with '11' as the first argument." << endl;

        cout << endl;
    }
    else if ( (argc == 2) )
    {
       launcherManager (argv[1]);
    }
    return 0;
}



void launcherManager (string choice)
{
	vector<int> folderInProcess;

	while (true)  // check every two seconds if files are present in /ReceivedData
	{
		vector<string> patientsIDsInReceivedData;

		// update folderInProcess : if there are an ID in folderInProcess that is not in patientsIDsInReceivedData, that mean this folder was remove,
		// erase the ID in folderInProcess
		folderFinder(patientsIDsInReceivedData);
		for (int i = 0 ; i < (int)folderInProcess.size(); i++)
		{
			bool folderRemove = true;

			for (int j = 0 ; j < (int)patientsIDsInReceivedData.size(); j++)
			{
				
				if (folderInProcess[i] == stoi(patientsIDsInReceivedData[j]) )
					folderRemove = false;
			}

			if (folderRemove)
			{
				// remove this folder from folderInProcess
				//cout << "Remove ID : " << folderInProcess[i] << "from folderInProcess. " << endl;
				folderInProcess.erase(folderInProcess.begin()+i);
			}
		}


		if (folderFinder(patientsIDsInReceivedData))
		{
			// enter here if folders are in ReceivecData
			// check if there are in process of not
			for (int i = 0 ; i < (int)patientsIDsInReceivedData.size(); i++)
			{
				if ( !PresentInVector (folderInProcess, stoi(patientsIDsInReceivedData[i]) ) )
				{
					folderInProcess.push_back(stoi(patientsIDsInReceivedData[i]));
					thread (processAPatientFile, patientsIDsInReceivedData[i], choice).detach();
				}
			}

			if (folderInProcess.size()<=1)
				cout << "There is " << folderInProcess.size() << " folder in process. " << endl;
			else
				cout << "There are " << folderInProcess.size() << " folders in process. " << endl;

			sleep(2);
		}
		else
		{
			cout << "No Folder in ReceivedData" << endl;
			sleep(2);
		}
	}
}


bool folderFinder (vector<string> &patientIDs)
{
	patientIDs.clear();

    dirent* d;
    string pathToReceivedData = (string)PATHTOALEDAPP + "/ReceivedData";
    DIR* dir = opendir(pathToReceivedData.c_str());

    if (dir == NULL) 
    {
    	cout << "LauncherAnalysis can't open /ReceivedData folder." << endl;
    	cout << "Check PATHTOALEDAPP var." << endl;

    	exit(1);
    }

    string folderName = "";
    string patientID = "";

    while((d = readdir(dir))!=NULL) 
    {
    	if (strcmp((d->d_name),".") != 0 && strcmp((d->d_name),"..") != 0 )
    	{
    		folderName = d->d_name ;

		    // convert folderName toward PatientID
		    patientID = folderName.substr(7);
		    //cout << "Find a folder with patientID = " << patientID << endl;

		    patientIDs.push_back(patientID);


    	}
    	
    }
    closedir(dir);




    if (strcmp(folderName.c_str(),"") == 0 )
    {
    	return false;
    }
    else
    	return true;

}





void processAPatientFile(string patientID, string choice)
{
	cout << ">>>Patient"+patientID+" --> Start to process the folder" << endl;

    string commandLine = "mkdir " + (string)PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + patientID;
    system(commandLine.c_str());
    thread t1(threadDescription,1,patientID,PATHTOALEDAPP);
    sleep(1);
    thread t2(threadDescription,2,patientID,PATHTOALEDAPP);
    sleep(1);
    thread t3(threadDescription,3,patientID,PATHTOALEDAPP);
    sleep(1);
    thread t4(threadDescription,4,patientID,PATHTOALEDAPP);
    sleep(1);
    thread t5(threadDescription,5,patientID,PATHTOALEDAPP);
    sleep(1);
    thread t6(threadDescription,6,patientID,PATHTOALEDAPP);


    t1.join(); 
    t2.join(); 
    t3.join(); 
    t4.join(); 
    t5.join(); 
    t6.join(); 

    
    sleep(1);
    thread t7(threadDescription,7,patientID,PATHTOALEDAPP);


    t7.join(); 

    cout << "End of process for folder : Patient" << patientID << endl;

	if (strcmp(choice.c_str(),"10") == 0 ) // erase input data
	{
		cout << ">>>Delete of folder : Patient" + patientID << endl;
		system( ("rm -f -r " + (string)PATHTOALEDAPP + "/ReceivedData/Patient" + patientID).c_str()  );
        cout << ">>>Delete of folder : FinalAnalysis/PreviousOutputs/Patient" + patientID << endl;
        system( ("rm -f -r " + (string)PATHTOALEDAPP + "/Analysis/FinalAnalysis/PreviousOutputs/Patient" + patientID).c_str()  );

	}
}


void threadDescription (int choice, string patientID, string pathToAled)
{
    string pathToInput;
    if (choice == 1) // Run KeyboardTestAnalysis
        pathToInput = (string)PATHTOALEDAPP + "/ReceivedData/" + "Patient" + patientID + "/" + "Patient" + patientID + "KT.txt";
    else if (choice == 2) // Run MouseTest
        pathToInput = (string)PATHTOALEDAPP + "/ReceivedData/" + "Patient" + patientID + "/" + "Patient" + patientID + "MT.txt";
    else if (choice == 3) // Run EyesBlinking
        pathToInput = (string)PATHTOALEDAPP + "/ReceivedData/" + "Patient" + patientID + "/" + "Patient" + patientID + "WC.mp4"; 
    else if (choice == 4) // Run CardiacFrequency
        pathToInput = (string)PATHTOALEDAPP + "/ReceivedData/" + "Patient" + patientID + "/" + "Patient" + patientID + "WC.mp4"; 
    else if (choice == 5) // Run FormAnalysis
        pathToInput = (string)PATHTOALEDAPP + "/ReceivedData/" + "Patient" + patientID + "/" + "Patient" + patientID + "F.txt"; 
    else if (choice == 6)
        cout << ">>>PROCESS for Patient"+patientID + " --> Launch thread n° " << choice << endl;
    else if (choice == 7)
        cout << ">>>PROCESS for Patient"+patientID + " --> Launch thread n° " << choice << endl;


    if ( choice != 6 && choice != 7)
    {

        // F.txt is mandatory but MT / KT / WC are not mandatory so wait max 10 s for them
        // Verification if input file are present :
        clock_t start;
        double duration = 0;

        if (choice >= 1 && choice <= 5)
        {
            start = time(NULL);
        }

        bool inputFound = false;

        ifstream inputFile(pathToInput);
        while ( !inputFound )
        {

            if (choice >= 1 && choice <= 5)
            {
                duration = difftime( time(NULL) ,start ) ; 
                if (duration > 10)
                {
                    cout << ">>>Patient"<< patientID <<" --> No imput for thread n° " << choice << " after 10s, thread " << choice << " closed." << endl;
                    return ;
                }
            }



            inputFound = inputFile.is_open();
            if (inputFound)
                cout << ">>>Patient"+patientID + " --> Launch thread n° " << choice << endl;
            else
            {
                // If File not found : wait 1 second and try again
                cout << ">>>Patient"+patientID + " --> FILE not found for thread n° " << choice << endl;
                sleep(2);
            }
        }
    }
        
    


    // When the file is found, launch analysis on it :
    string commandLine;
    if ( choice == 1)
        commandLine = (string)PATHTOALEDAPP+"/Analysis/KeyboardTestAnalysis/KeyboardAnalysis 10 "+ patientID + " " + pathToAled;
    else if (choice == 2)
        commandLine = (string)PATHTOALEDAPP+"/Analysis/MouseTestAnalysis/MouseTestAnalysis 10 "+ patientID + " " + pathToAled;
    else if (choice == 3)
        commandLine = (string)PATHTOALEDAPP+"/Analysis/WebcamAnalysis/EyesBlinking/build/EyesBlinking 10 "+ patientID + " " + pathToAled;
    else if (choice == 4)
        commandLine = (string)PATHTOALEDAPP+"/Analysis/WebcamAnalysis/CardiacFrequency/build/CardiacFrequency 10 "+ patientID + " " + pathToAled;
    else if (choice == 5)
        commandLine = (string)PATHTOALEDAPP+"/Analysis/FormAnalysis/FormAnalysis 10 "+ patientID + " " + pathToAled;
    else if (choice == 6)
        commandLine = (string)PATHTOALEDAPP+"/Analysis/PatientFileAnalysis/PatientFileAnalysis 10 "+ patientID + " " + pathToAled;
    else if (choice == 7)
        commandLine = (string)PATHTOALEDAPP+"/Analysis/FinalAnalysis/FinalAnalysis 10 "+ patientID + " " + pathToAled;

    system(commandLine.c_str());

}


bool PresentInVector (vector<int> processedVector, int ID)
{
	for (int i = 0; i<(int)processedVector.size(); i++)
	{
		if (processedVector[i] == ID)
			return true;
	}
	return false;
}