// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to find frontal human faces in an image and
    estimate their pose.  The pose takes the form of 68 landmarks.  These are
    points on the face such as the corners of the mouth, along the eyebrows, on
    the eyes, and so forth.


    This example is essentially just a version of the face_landmark_detection_ex.cpp
    example modified to use OpenCV's VideoCapture object to read from a camera instead
    of files.


    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most current machines.
*/

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

#include <cmath>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace dlib;
using namespace std;
using namespace cv;

/* Cardiac frequency :
 * 1. We retrieve the principal points of the face's edges (dlib)
 * 2. We retrieve the area of the face (opencv)
 * 3. We will compute the mean color of the region between the points retreive at point 1
 * We will retrieve the principal point of the face's edges using dlib and we will check the color between the points of the face
 *
 */

// Compute points min max
int computeMaxX(dlib::point a, dlib::point b);
int computeMaxY(dlib::point a, dlib::point b);
int computeMinX(dlib::point a, dlib::point b);
int computeMinY(dlib::point a, dlib::point b);

// Compute the mean color of the face frame by frame
float computeMeanFace(cv::Mat &frame, full_object_detection shape, cv::CascadeClassifier &faceCascade, int draftMode);

// Compute the cardiac frequency on the last 60 seconds
float computeCardiacFrequency (std::vector<float> meanColorFace, double videoDuration);

// Compute the difference between the normal color and the mean color
int compareMeanColorToNormalColor(float meanColorFace, float normalColor);

// Detection of yellow or red eyes
int yellowRedEye(float meanColorEye, float normalColor);

// Compare the cardiac frequency with the referencial frequency
int cardiacFrequencyComparator(float reference, float precision, float value);

// Detection of abnormal cardiac frequency
int cardiacFrequencyAnalysis(float cardiacFrequency, float age);

Mat RGBtoHSV (Mat originalImage);

void CardiacFrequencyOutput (float cardiacFrequency, string PatientID);




string PATHTOALEDAPP ;



/** @function main */
int main( int argc, char** argv ) {


	if (argc == 1)
    {
    	cout << "Classic use\n";
        cout << "1st arg : '10' // 2nd arg : Patient ID // 3rd PATHTOALEDAPP \n";
        cout << "this program will create an output in ALEDAPP/Analysis/FinalAnalysis/PreviousOutputs/Patient+PATIENTID\n\n";
        cout << "Draft interface on current webcam : 1] '1' // 2] '0' // 3] PATHTOALEDAPP \n" ;
    }

    // Classic mode
    if(argc == 4 && strcmp(argv[1],"10")==0)
    {
    	PATHTOALEDAPP = argv[3];
    	string pathToInput = PATHTOALEDAPP + "/ReceivedData/" + "Patient" + argv[2] + "/" + "Patient" + argv[2] + "WC.mp4";
		try {
			// Face and eye detection
			cv::CascadeClassifier faceCascade;
			cv::CascadeClassifier eyeCascade;

			// Cardiac frequency and mean color
			float cardiacFrequency = 0.0;
			float meanColor = 0.0;

			// We check tha twe can load the face and eye detector
			if (!faceCascade.load(PATHTOALEDAPP + "/Analysis/WebcamAnalysis/CardiacFrequency/haarcascade_frontalface_alt.xml")) {
				 std::cerr << "Could not load face detector." << std::endl;
				 exit(1);
			}

			// We load the program to the webcam
			cv::VideoCapture cap(pathToInput);
			if (!cap.isOpened()) {
				std::cerr << "Imput not detected." << std::endl;
				exit(1);
			}


			dlib::image_window win;

			//Average color of the face
			std::vector<float> meanColorFace;

			// Load face detection - dlib
			dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
			dlib::shape_predictor pose_model;
			dlib::deserialize(PATHTOALEDAPP + "/Analysis/WebcamAnalysis/EyesBlinking/shape_predictor_68_face_landmarks.dat") >> pose_model;

			//std::cout << "Beginning of the cardiac frequency process" << std::endl;

			// while the window is not closed, we continue processing frames
			while (!win.is_closed()) {
				// We convert the webcam video into a picture
				cv::Mat temp;
				cv::Mat frame;
				cap >> frame;

				if(!cap.read(frame)||!cap.read(temp)) {
					break;
				}

				// conversion of the Mat of OpenCV into dlib object for image : cv_image<bgr_pixel>
				dlib::cv_image<dlib::bgr_pixel> cimg(temp);

				//We detect the faces using dlib
				std::vector<dlib::rectangle> faces = detector(cimg);

				// if we detect at least one face
				if ((int)faces.size() > 0) {
					full_object_detection shape = pose_model(cimg, faces[0]);


					// we detect the face mean color
					meanColor = computeMeanFace(frame, shape, faceCascade,0);

					// We print the mean color
	                //printf("mean color %f\n", meanColor);

	                //We keep the color in a vector
					meanColorFace.push_back(meanColor);



					// Conversion from a dlib objet to a open cv object
					temp = toMat(cimg);

					char buffer[50];
					int n;

					n = sprintf(buffer, "Color Average : %f", meanColor);
					putText(temp, buffer, cv::Point(10,60) ,1 ,2 ,(0,0,255) , 2 , 8 , false);

					// conversion of OpenCV image to dlib image
					dlib::cv_image<dlib::bgr_pixel> cimg2(temp);

					// We display frame on screen
					//win.clear_overlay();
					//win.set_image(cimg2);
					//win.add_overlay(render_face_detections(shape));
				}
				else {
					//win.clear_overlay();
					//win.set_image(cimg);
				}
			}
			// We compute the cardiac frequency on the whole sequence


			// GET TIME OF THE VIDEO
			cap.set(CAP_PROP_POS_AVI_RATIO, 1);
			float numberOfFrame = cap.get(CAP_PROP_POS_FRAMES);
			double fps = cap.get(CAP_PROP_FPS);
			double videoDuration = numberOfFrame / fps;
			cardiacFrequency = computeCardiacFrequency (meanColorFace, videoDuration);
			//cout << "video Duration = " << videoDuration << endl;
			//cout << "cardiacFrequency = " << cardiacFrequency << endl;


			CardiacFrequencyOutput (cardiacFrequency, argv[2]);


		}
		catch(dlib::serialization_error& e) {
	        std::cout << "You need dlib's default face landmarking model file to run this example." << std::endl;
	        std::cout << "You can get it from the following URL: " << std::endl;
	        std::cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << std::endl;
	        std::cout << std::endl << e.what() << std::endl;
	    }
	    catch(exception& e) {
	        std::cout << e.what() << std::endl;
	    }



	}


    // draft interface mode
    else if(argc == 4 && strcmp(argv[1],"1")==0)
    {
    	PATHTOALEDAPP = argv[3];
    	string pathToInput = PATHTOALEDAPP + "/ReceivedData/" + "Patient" + argv[2] + "/" + "Patient" + argv[2] + "WC.mp4";
		try {
			// Face and eye detection
			cv::CascadeClassifier faceCascade;
			cv::CascadeClassifier eyeCascade;

			// Cardiac frequency and mean color
			float cardiacFrequency = 0.0;
			float meanColor = 0.0;

			// We check tha twe can load the face and eye detector
			if (!faceCascade.load(PATHTOALEDAPP + "/Analysis/WebcamAnalysis/CardiacFrequency/haarcascade_frontalface_alt.xml")) {
				 std::cerr << "Could not load face detector." << std::endl;
				 exit(1);
			}

			// We load the program to the webcam
			cv::VideoCapture cap(pathToInput);
			if (!cap.isOpened()) {
				std::cerr << "Imput not detected." << std::endl;
				exit(1);
			}


			dlib::image_window win;

			//Average color of the face
			std::vector<float> meanColorFace;

			// Load face detection - dlib
			dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
			dlib::shape_predictor pose_model;
			dlib::deserialize(PATHTOALEDAPP + "/Analysis/WebcamAnalysis/EyesBlinking/shape_predictor_68_face_landmarks.dat") >> pose_model;

			// while the window is not closed, we continue processing frames
			while (!win.is_closed()) {
				// We convert the webcam video into a picture
				cv::Mat temp;
				cv::Mat frame;
				cap >> frame;

				if(!cap.read(frame)||!cap.read(temp)) {
					break;
				}

				// conversion of the Mat of OpenCV into dlib object for image : cv_image<bgr_pixel>
				dlib::cv_image<dlib::bgr_pixel> cimg(temp);

				//We detect the faces using dlib
				std::vector<dlib::rectangle> faces = detector(cimg);

				// if we detect at least one face
				if ((int)faces.size() > 0) {
					full_object_detection shape = pose_model(cimg, faces[0]);


					// we detect the face mean color
					meanColor = computeMeanFace(frame, shape, faceCascade,1);

					// We print the mean color
	                printf("mean color %f\n", meanColor);

	                //We keep the color in a vector
					meanColorFace.push_back(meanColor);



					// Conversion from a dlib objet to a open cv object
					temp = toMat(cimg);

					char buffer[50];
					int n;

					n = sprintf(buffer, "Color Average : %f", meanColor);
					putText(temp, buffer, cv::Point(10,60) ,1 ,2 ,(0,0,255) , 2 , 8 , false);

					// conversion of OpenCV image to dlib image
					dlib::cv_image<dlib::bgr_pixel> cimg2(temp);

					// We display frame on screen
					win.clear_overlay();
					win.set_image(cimg2);
					win.add_overlay(render_face_detections(shape));
				}
				else {
					win.clear_overlay();
					win.set_image(cimg);
				}
			}
			// We compute the cardiac frequency on the whole sequence


			// GET TIME OF THE VIDEO
			cap.set(CAP_PROP_POS_AVI_RATIO, 1);
			float numberOfFrame = cap.get(CAP_PROP_POS_FRAMES);
			double fps = cap.get(CAP_PROP_FPS);
			double videoDuration = numberOfFrame / fps;
			cardiacFrequency = computeCardiacFrequency (meanColorFace, videoDuration);
			cout << "video Duration = " << videoDuration << endl;
			cout << "cardiacFrequency = " << cardiacFrequency << endl;



		}
		catch(dlib::serialization_error& e) {
	        std::cout << "You need dlib's default face landmarking model file to run this example." << std::endl;
	        std::cout << "You can get it from the following URL: " << std::endl;
	        std::cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << std::endl;
	        std::cout << std::endl << e.what() << std::endl;
	    }
	    catch(exception& e) {
	        std::cout << e.what() << std::endl;
	    }
    }


	return 0;
}

// To compute min and max coordintates
int computeMaxX(dlib::point a, dlib::point b) {
	int x;
	a.x() > b.x() ? x = a.x() : x = b.x();
	return x;
}

int computeMaxY(dlib::point a, dlib::point b) {
	int y;
	a.y() > b.y() ? y = a.y() : y = b.y();
	return y;
}

int computeMinX(dlib::point a, dlib::point b) {
	int x;
	a.x() < b.x() ? x = a.x() : x = b.x();
	return x;
}

int computeMinY(dlib::point a, dlib::point b) {
	int y;
	a.y() < b.y() ? y = a.y() : y = b.y();
	return y;
}

// To compute the mean color of the face
// return the mean color of the face (jaws)
float computeMeanFace(cv::Mat &frame, full_object_detection shape, cv::CascadeClassifier &faceCascade, int draftMode) {
	float meanColor = 0;

	// Face detection
	// We use the image in greyscale
	cv::Mat grayscale;
	cv::cvtColor(frame, grayscale, cv::COLOR_BGR2GRAY);

	// We improve the contrast
	cv::equalizeHist(grayscale, grayscale);

	// We detect the face
	std::vector<cv::Rect> faces;
	faceCascade.detectMultiScale(grayscale, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(150, 150));

	if (faces.size() == 0) return 0.0;

	// Since we have a face, we convert RGB into HSV

	//cv::cvtColor(bright, brightHSV, cv::COLOR_BGR2HSV);
	//cv::cvtColor(dark, darkHSV, cv::COLOR_BGR2HSV);

	Mat hsv = RGBtoHSV (frame);

	// Computation of mean value

	/*We only check the jaws of the face, so we only use the point 1 to 17 of dlib facial landmarks
	 * The points 8, 9 and 10 won't be taken into account
	 * We will use rectangle formed with four points of the jaws, two from each side
	 * Example : first rectangle is (1, 2, 16, 17)
	 * We will have an error caused by the pixel that we take and should not be taken
	 * This error is not a problem when we work with variation of color
	 * It could cause issues when we will look for abnormal colors.*/

	// We will look for the xmax, xmin, ymax, ymin for the fourt points to have a real rectangle
	// We take the largest rectangle inscribed in order to have less non face as possible
	dlib::point pmin, pmax;
	float nb = 0;
	float sum = 0;
	for(int right = 0; right < 7; right ++) {
		for(int left = 16; left > 9; left --) {
			if(right + left == 16) {
				//std::cout << "\nPoints\nright point : " << shape.part(right).x() << " " << shape.part(right).y() << std::endl;
				//std::cout << "right + 1 point : " << shape.part(right + 1).x() << " " << shape.part(right + 1).y() << std::endl;
				//std::cout << "left point : " << shape.part(left).x() << " " << shape.part(left).y() << std::endl;
				//std::cout << "left - 1 point : " << shape.part(left - 1).x() << " " << shape.part(left - 1).y() << std::endl;

				pmin.x() = computeMinX(shape.part(right+1), shape.part(left));
				pmin.y() = computeMinY(shape.part(right + 1), shape.part(right));
				pmax.x() = computeMaxX(shape.part(right + 1), shape.part(left - 1));
				pmax.y() = computeMaxY(shape.part(left - 1), shape.part(left));

				//std::cout << "\nPoint min " << pmin.x() << " " << pmin.y() << std::endl;
				//std::cout << "Point max " << pmax.x() << " " << pmax.y() << std::endl;



				if (draftMode==1)
					cv::rectangle(frame,cv::Point(pmax.x(),pmax.y()),cv::Point(pmin.x(),pmin.y()),Scalar(0,0,0),5,8,0);


				for(int x = pmin.x() ; x < pmax.x() + 1; x++) {
					for (int y = pmin.y(); y < pmax.y() + 1; y++) {
						Vec3f HSVvect = hsv.at<Vec3f>(y, x);
						float hue = HSVvect.val[0];
						sum = sum + hue;
						nb ++;
						//std::cout << "\nDouble for x, y\nhue" << hue << " sum " << sum << " nb " << nb << std::endl;
					}
				}
			}
		}
	}
	// we compute the mean color
	if (nb != 0) {
		meanColor = sum / nb;
		//cout << "meanColor = " << sum << " / " << nb << " = " << meanColor << endl;
	}
	else {
		cout << "nb is null" << nb << endl;
		meanColor = 0;
	}

	if(draftMode==1)
	{
		cv::imshow("Webcam", frame); // displays frame on screen
		if( cv::waitKey(0)>=0 ) { destroyWindow("Webcam"); }
	}


	return meanColor;
}

// Compute the cardiac frequency on the whole time
// return the cardiac frequency
float computeCardiacFrequency (std::vector<float> meanColorFace, double videoDuration) {

    float nb_beat = 0.0;
    float meanColorDifferenceAverage = 0.0;

    for(int i(1); i < meanColorFace.size(); i++) {
		// When the meanColorFace changes enough, we have a beat

		//cout << "abs(meanColorFace[i] - meanColorFace[i-1]) = " << abs(meanColorFace[i] - meanColorFace[i-1]) << endl;

		if (!isnan(abs(meanColorFace[i] - meanColorFace[i-1]))) {
			meanColorDifferenceAverage = meanColorDifferenceAverage + abs(meanColorFace[i] - meanColorFace[i-1]);
		}
	}

	meanColorDifferenceAverage = meanColorDifferenceAverage / meanColorFace.size() ;

	//cout << "meanColorDifferenceAverage = " << meanColorDifferenceAverage << endl;

	//count nb of beat

	for(int i(1); i < meanColorFace.size(); i++) {
		if (abs(meanColorFace[i] - meanColorFace[i-1]) >= meanColorDifferenceAverage*2.2){
				nb_beat ++;
		}
	}

	//cout << "nb_beat = " << nb_beat << endl;


	// We compute the frequency
	// we have X beats 				for S seconds
	// We have X/S beats/s

	nb_beat = 60 * nb_beat / videoDuration;
    return nb_beat;
}

// Compute the difference between the normal color and the mean color
// return the amount of deviation color
// 1 if hue difference is between 10 and 19
// 2 if it is between 20 and 29
// etc.
int compareMeanColorToNormalColor (float meanColorFace, float normalColor) {
	int result = 0;

	// Detection of abnormal color face : if the patient has a face color under 30, to hue goes up, otherwise the hue goes down
	if(abs(meanColorFace - normalColor) > 10)
		result = abs(meanColorFace - normalColor)/10;

	return result;
}

// Detection of yellow or red eyes - retrun the abnormal color detected
// normal : 0
// red : 1
// pink : 2
// yellow : 3
int yellowRedEye(float meanColorEye, float normalColor) {
	int result = 0;

	// Detection of abnormal color face : if the patient has a face color under 30, to hue goes up, otherwise the hue goes down
	if(abs(meanColorEye - normalColor) > 10){
		if (meanColorEye > 170 || meanColorEye < 10)
			result = 1; // the eye is red
		if (meanColorEye > 140 && meanColorEye < 160)
			result = 2; // the eye is pink
		if (meanColorEye > 50 && meanColorEye < 70)
			result = 3; // the eye is yellow
	}
	return result;
}


Mat RGBtoHSV (Mat originalImage)
{
    Mat HSVMat = Mat(originalImage.rows,originalImage.cols,CV_32FC3);

    for(int i = 0; i < originalImage.cols; i++)
    {
        for(int j = 0; j < originalImage.rows; j++)
        {
            Vec3b intensity = originalImage.at<Vec3b>(j, i);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];

            float hue = 0;
            float saturation = 0;
            float value = 0;

            value = max(max(blue,green),red);
            saturation = ( value - min(min(blue,green),red) ) / value ;

            if (value == red)
            {
                hue = (green - blue) / (value - min(min(blue,green),red) );
            }
            if (value == green)
            {
                hue = 2 + (blue - red) / (value - min(min(blue,green),red) );
            }
            if (value == blue)
            {
                hue = 4 + (red - green) / (value - min(min(blue,green),red) );
            }

            Vec3f HSVvect;
            HSVvect.val[0] = hue;
            HSVvect.val[1] = saturation;
            HSVvect.val[2] = value;

            HSVMat.at<Vec3f>(j, i) = HSVvect;

        }
    }

    return HSVMat;
}


void CardiacFrequencyOutput (float cardiacFrequency, string PatientID)
{
	// write the cardiac frequency in a file with only the cardiac frequency
	string pathToOutput = PATHTOALEDAPP + (string)"/Analysis/FinalAnalysis/PreviousOutputs/Patient"+PatientID+"/Patient_"+PatientID+"_CardiacFrequency.txt";

	FILE * f = fopen(pathToOutput.c_str(), "wt");

	if (f == NULL) {
		std::cout << "Impossible to open the file for writing" << std::endl;
	}

	fprintf(f, "%f", cardiacFrequency);
	fclose(f);

    //cout << "creation of : " +  pathToOutput << endl;
    std::cout << ">>>Patient"+PatientID+" --> Creation of : Patient_" + PatientID + "_CardiacFrequency.txt" << std::endl;
}
