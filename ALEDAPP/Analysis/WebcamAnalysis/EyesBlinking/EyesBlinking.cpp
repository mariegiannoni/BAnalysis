 #include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include "opencv2/core/core.hpp"

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <cmath>
#include <math.h>
#include <iostream>
#include <stdio.h>

using namespace dlib;
using namespace std;



string PATHTOALEDAPP ;




// calculation of the euclidian distance between 2 points
double eucl_dist(dlib::point a, dlib::point b);
//calcul the eye openness ratio
double eye_openness_ratio (dlib::full_object_detection shape);
// calcul the average of the n=averageNum last eye openness ratios
double eye_openness_average (std::vector<double> listRatios, int averageNum);

void eye_blinking_output (int blink10s, string PatientID);







int main( int argc, char** argv )
{

    if (argc == 1)
    {
        cout << "1st arg : '10' // 2nd arg : Patient ID // 3rd PATHTOALEDAPP \n";
        cout << "this program will create an output in ALEDAPP/Analysis/FinalAnalysis/PreviousOutputs/Patient+PATIENTID\n\n";
        cout << "Draft interface on current webcam : 1] '1' // 2] '0' // 3] PATHTOALEDAPP \n" ;
    }


    // Classic mode
    if(argc == 4 && strcmp(argv[1],"10")==0)
    {
        PATHTOALEDAPP = argv[3];
        // EYE BLINKING AND OPENING
        try
        {
            int blinkingNumb10s = 0;
            double blinkingNumb10sAverage = 2.5;
            clock_t start;
            clock_t currentTime;
            start = clock();
            string pathToInput = PATHTOALEDAPP + "/ReceivedData/" + "Patient" + argv[2] + "/" + "Patient" + argv[2] + "WC.mp4";
            cv::VideoCapture cap(pathToInput);
            if (!cap.isOpened())
            {
                cerr << "Unable to open the file in EyesBlinking" << endl;
                return 1;
            }

            dlib::image_window win;

            // Load face detection and pose estimation models.
            dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
            dlib::shape_predictor pose_model;
            dlib::deserialize(PATHTOALEDAPP + "/Analysis/WebcamAnalysis/EyesBlinking/shape_predictor_68_face_landmarks.dat") >> pose_model;

            // Grab and process frames until the main window is closed by the user.

            std::vector<double> eyeRatios;

            while(!win.is_closed())
            {
                // Grab a frame
                cv::Mat temp;
                if (!cap.read(temp))
                {
                    break;
                }
                dlib::cv_image<dlib::bgr_pixel> cimg(temp);

                // Detect faces
                std::vector<dlib::rectangle> faces = detector(cimg);

                // Find the pose of each face.
                //std::vector<full_object_detection> shapes;



                // if we detect at least one face, we work on the first one
                if ( (int)faces.size() > 0)
                {

                    dlib::full_object_detection shape = pose_model(cimg, faces[0]);

                        // calcul of eyeOpennessRatio and push it in the vector of ratios.
                    double lastEyeRatio = eye_openness_ratio (shape);
                    eyeRatios.push_back(lastEyeRatio);
                    double eyeRatioAverage = eye_openness_average(eyeRatios,10);


                    // detect blinking
                    if (lastEyeRatio < (0.7*eye_openness_average(eyeRatios,10)))
                    {
                        blinkingNumb10s++;
                    }
                }

                // enter in if every ten seconds :
                currentTime = clock();
                if ( (currentTime - start) > 10*CLOCKS_PER_SEC )
                {
                    start = clock();
                    //printf("number of blink after 10 seconds : %d\n", blinkingNumb10s  );
                    blinkingNumb10sAverage= (double)(blinkingNumb10s + blinkingNumb10sAverage) / 2.0 ;
                    blinkingNumb10s =0;



                }

            }

            // call eye_blinking_output at the video end
            eye_blinking_output (blinkingNumb10sAverage, argv[2]);

        }
        catch(dlib::serialization_error& e)
        {
            cout << "You need dlib's default face landmarking model file to run this example." << endl;
            cout << "You can get it from the following URL: " << endl;
            cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
            cout << endl << e.what() << endl;
        }
        catch(exception& e)
        {
            cout << e.what() << endl;
        }
    }


    // draft interface mode
    else if(argc == 4 && strcmp(argv[1],"1")==0)
    {
        PATHTOALEDAPP = argv[3];
        // EYE BLINKING AND OPENING
        try
        {
            int blinkingNumb10s = 0;
            double blinkingNumb10sAverage = 2.5;
            clock_t start;
            clock_t currentTime;
            start = clock();

            cv::VideoCapture cap(0);
            if (!cap.isOpened())
            {
                cerr << "Unable to open the file in EyesBlinking" << endl;
                return 1;
            }

            dlib::image_window win;

            // Load face detection and pose estimation models.
            dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
            dlib::shape_predictor pose_model;
            dlib::deserialize(PATHTOALEDAPP + (string)"/Analysis/WebcamAnalysis/EyesBlinking/shape_predictor_68_face_landmarks.dat") >> pose_model;

            // Grab and process frames until the main window is closed by the user.

            std::vector<double> eyeRatios;

            while(!win.is_closed())
            {
                // Grab a frame
                cv::Mat temp;
                if (!cap.read(temp))
                {
                    break;
                }
                dlib::cv_image<dlib::bgr_pixel> cimg(temp);

                // Detect faces
                std::vector<dlib::rectangle> faces = detector(cimg);

                // Find the pose of each face.
                //std::vector<full_object_detection> shapes;



                // if we detect at least one face, we work on the first one
                if ( (int)faces.size() > 0)
                {

                    dlib::full_object_detection shape = pose_model(cimg, faces[0]);

                        // calcul of eyeOpennessRatio and push it in the vector of ratios.
                    double lastEyeRatio = eye_openness_ratio (shape);
                    eyeRatios.push_back(lastEyeRatio);
                    double eyeRatioAverage = eye_openness_average(eyeRatios,10);


                    // detect blinking
                    if (lastEyeRatio < (0.7*eye_openness_average(eyeRatios,10)))
                    {
                        blinkingNumb10s++;
                    }


                    // ----------------------------------------------------------------
                    // use of a trick to write on "win"
                    // unwrap the cimg to cv::Mat
                    temp=toMat(cimg);
                    // write on temp

                    char buffer [50];
                    int n;
                    n=sprintf(buffer,"Moyenne 10 derniers ratio : %f", eyeRatioAverage );
                    cv::putText(temp, buffer, cv::Point(10,30) ,1 ,2 ,(0,0,255) , 2 , 8 , false);


                    n=sprintf(buffer,"          Dernier ratio est : %f", lastEyeRatio);
                    cv::putText(temp, buffer, cv::Point(10,60) ,1 ,2 ,(0,0,255) , 2 , 8 , false);


                    n=sprintf(buffer,"     Nombre de clignements : %d", blinkingNumb10s);
                    cv::putText(temp, buffer, cv::Point(10,83) ,1 ,2 ,(0,0,255) , 2 , 8 , false);





                    // return to cimg version 2
                    dlib::cv_image<dlib::bgr_pixel> cimg2(temp);
                    // ----------------------------------------------------------------


                    // Display it all on the screen
                    win.clear_overlay();
                    win.set_image(cimg2);
                    //render_face_detections add the lines between the 68 points
                    win.add_overlay(render_face_detections(shape));

                }
                else
                {
                    win.clear_overlay();
                    win.set_image(cimg);
                }

                // enter in if every ten seconds :
                currentTime = clock();
                if ( (currentTime - start) > 10*CLOCKS_PER_SEC )
                {
                    start = clock();
                    //printf("number of blink after 10 seconds : %d\n", blinkingNumb10s  );
                    blinkingNumb10sAverage= (double)(blinkingNumb10s + blinkingNumb10sAverage) / 2.0 ;
                    blinkingNumb10s =0;



                }

            }

            // call eye_blinking_output at the video end
            eye_blinking_output (blinkingNumb10sAverage, argv[2]);

        }
        catch(dlib::serialization_error& e)
        {
            cout << "You need dlib's default face landmarking model file to run this example." << endl;
            cout << "You can get it from the following URL: " << endl;
            cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
            cout << endl << e.what() << endl;
        }
        catch(exception& e)
        {
            cout << e.what() << endl;
        }
    }
    return 0;
}

// compute distance between two points and return it
double eucl_dist (dlib::point a, dlib::point b) {
   // int ax = a.x;
   // int ay = a.y;

    int test = (a.x() - b.x());
    //printf("test = %d\n",test);
    test = (a.y() - b.y());
    //printf("test = %d\n",test);
    double result = pow((a.x() - b.x()),2) + pow((a.y() - b.y()),2);
    result = pow(result, 0.5);
    return result;
}


// compute eye openess ratio and return it
double eye_openness_ratio (full_object_detection shape) {
    // first we collect the 6 points of the left eye : number 36 to 41
    double ratio=0.0;
    double edVertical1 = eucl_dist (shape.part(37),shape.part(41));
    double edVertical2 = eucl_dist (shape.part(38),shape.part(40));
    double edHorizontal = eucl_dist (shape.part(36),shape.part(39));
    ratio = ( edVertical1 + edVertical2 ) / ( 2.0 * edHorizontal );
    //printf ("ratio = %f\n", ratio);

    return ratio;

}


// compute the average of the eye openess ratio on the last averageNum ones
double eye_openness_average (std::vector<double> listRatios, int averageNum) {

    double result = 0.0;
    for (size_t i = std::max(0, (int)(listRatios.size() - averageNum)); i < listRatios.size(); i++)
    {
        result = result + listRatios[i];
    }
    result = result / averageNum;

    return result;
}

/*
void eye_blinking_output (int blink10s, string PatientID)
{
    // 1 : Anxiety -> ( blink > 20 )
    short anxiety=0;
    // 7. Delusions -> ( blink > 20 )
    short delusion=0;
    // 11. Fatigue  -> ( blink < 10 )
    short fatigue=0;
    // 12. Fever  ->  ( blink < 10 )
    short fever = 0;
    // 21. Tremor -> ( blink > 20 )
    short tremor=0;
    // 22. Vertigo -> ( blink < 10 )
    short vertigo=0;


    // creation of outputfile
    string pathToOutput = PATHTOALEDAPP + (string)"/Analysis/FinalAnalysis/PreviousOutputs/Patient"+PatientID+"/Patient"+PatientID+"EyeBlinking.txt";
    FILE * f = fopen(pathToOutput.c_str(), "wt");

    if (f == NULL) {
		std::cout << "Impossible to open the file for writing" << std::endl;
	}

    int symptomsCriticality [24][2];

    for (int i = 0 ; i < 24 ; i++)
    {
        symptomsCriticality[i][0]=i;
        symptomsCriticality[i][1]=0;
    }



    if (blink10s < 1.6)
    {
        fatigue=100;
        fever=100;
        vertigo=100;

        symptomsCriticality[11][1] = fatigue;
        symptomsCriticality[12][1] = fever;
        symptomsCriticality[22][1] = vertigo;

    }
    else if (blink10s > 3.3)
    {
        anxiety=100;
        delusion=100;
        tremor=100;

        symptomsCriticality[1][1] = anxiety;
        symptomsCriticality[7][1] = delusion;
        symptomsCriticality[21][1] = tremor;

    }

    // writing : symptom '-' criticity 'semicolon'  symptom .....

    for (int i = 0; i < 24; i++)
    {
    	short id = symptomsCriticality[i][0];
    	short criticality = symptomsCriticality[i][1];
    	fprintf(f, "%hd %hd\n", id, criticality);
    }

	fprintf(f, "%d", blink10s);
    fclose(f);

    cout << "creation of : " +  pathToOutput << endl;

}
*/


void eye_blinking_output (int blink10s, string PatientID)
{
	// write the cardiac frequency in a file with only the cardiac frequency
	string pathToOutput = PATHTOALEDAPP + (string)"/Analysis/FinalAnalysis/PreviousOutputs/Patient"+PatientID+"/Patient_"+PatientID+"_BlinkFrequency.txt";

	FILE * f = fopen(pathToOutput.c_str(), "wt");

	if (f == NULL) {
		std::cout << "Impossible to open the file for writing" << std::endl;
	}

	fprintf(f, "%d", blink10s);
	fclose(f);

   //cout << "creation of : " +  pathToOutput << endl;
    std::cout << ">>>Patient"+PatientID+" --> Creation of : Patient_" + PatientID + "_BlinkFrequency.txt" << std::endl;
}
