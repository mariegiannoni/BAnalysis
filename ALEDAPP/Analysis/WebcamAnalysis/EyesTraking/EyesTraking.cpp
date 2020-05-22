 #include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include "opencv2/core/core.hpp"
 #include <cstdlib>

 #include <iostream>
 #include <stdio.h>

 using namespace std;
 using namespace cv;

 /** Global variable */
std::vector<cv::Point> centers;  // list of all the centers from getEyeball
std::vector<float> radiuss;      // list of all the radius from getEyeball

cv::Point mousePoint(960,540);
// this is to initialize the position of the mouse on the middle of the screen.

 /** Function Headers */
void detectEyes(cv::Mat &frame, cv::CascadeClassifier &faceCascade, cv::CascadeClassifier &eyeCascade);
// This is the main function of the program.
// This function detect the face and eye thanks to the classifier.
// It will call getLeftmostEye, HoughCircles, getEyeball, stabilizeCenter and stabilizeRadius,
// Draw a circle around the eyeball with "circle" and change the mouse position with changeMouse.

cv::Rect getLeftmostEye(std::vector<cv::Rect> &eyes);
// This function receive 2 eyes and return the most left one.

cv::Vec3f getEyeball(cv::Mat &eye, std::vector<cv::Vec3f> &circles);
// This function receive all the circles detected on eye with the function HoughCircles,
// and return the one that is the most black.

cv::Point stabilizeCenter(std::vector<cv::Point> &points, int n);
// This function receive a vector of points composed of all the centers of circle from getEyeball,
// and make an adverage of the n last one.

float stabilizeRadius(std::vector<float> &numbers, int n);
// This function receive a vector of floats composed of all the radius of circle from getEyeball,
// and make an adverage of the n last one.

void changeMouse(cv::Mat &cmEye, cv::Point &cmCenter);
// This function define the position of the mouse on the screen.
// This position depend of the position of the eyeball compare to the center of the eye mat.

 /** @function main */
int main( int argc, char** argv )
{

    if (argc == 1)
    {
    	cout << "Two way to use EYESTRAKING : " << endl;
    	cout << "The classic that is NOT operational because need correlation with mouse data during the questionary." << endl;
        cout << "Enter 10 as the first argument and the addresse of the video to analyse as the first argument.\n";
        cout << "this program will create an output in ALEDAPP/Analysis/FinalAnalysis/PreviousOutputs/Patient0\n\n";
        cout << endl;
        cout << "To use the debugg mode on current webcam Enter just 1 as the first argument.\n" ;
    }



    // debeug mode
    else if(argc == 2 && strcmp(argv[1],"1")==0)
	{
		// this first part check if classifiers are well upload.
		cv::CascadeClassifier faceCascade;
		cv::CascadeClassifier eyeCascade;
		if (!faceCascade.load("./haarcascade_frontalface_alt.xml"))
		{
			std::cerr << "Could not load face detector." << std::endl;
			return -1;
		}
		if (!eyeCascade.load("./haarcascade_eye_tree_eyeglasses.xml"))
		{
		    std::cerr << "Could not load eye detector." << std::endl;
		    return -1;
		}

		cv::VideoCapture cap(0); // this line log the program to the webcam
		if (!cap.isOpened())
		{
		    std::cerr << "Webcam not detected." << std::endl;
	    	return -1;
		}
		cv::Mat frame;

		while (1)
		{
		    cap >> frame; // convert the webcam stream to frame.

		    detectEyes(frame, faceCascade, eyeCascade); // transfer frame to detectEyes function

		    cv::imshow("Webcam", frame); // displays frame on screen

		    int c = cv::waitKey(30); // press "c" to stop the display of frame
		    if( (char)c == 'c' ) { break; }
		}
	}







	return 0;
}




void detectEyes(cv::Mat &frame, cv::CascadeClassifier &faceCascade, cv::CascadeClassifier &eyeCascade)
{
	cv::Mat grayscale;
	cv::cvtColor(frame, grayscale, CV_BGR2GRAY); // convert image to grayscale
	cv::equalizeHist(grayscale, grayscale); // improve image contrast
	std::vector<cv::Rect> faces;
	faceCascade.detectMultiScale(grayscale, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(150, 150)); // detect face

	if (faces.size() == 0) return; // none face was detected stop detectEyes
	cv::Mat face = grayscale(faces[0]); // crop the face
	rectangle(frame, faces[0].tl(), faces[0].br(), cv::Scalar(255, 0, 0), 2); // draw rectangle on frame around the face

	std::vector<cv::Rect> eyes;
	eyeCascade.detectMultiScale(face, eyes, 1.1, 4, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(50, 50)); // detect eyes


	if (eyes.size() != 2) return; // if both eyes were not detected stop detectEyes

	for (cv::Rect &pEye : eyes) // draw rectangle around eyes
	{
	  rectangle(frame, faces[0].tl() + pEye.tl(), faces[0].tl() + pEye.br(), cv::Scalar(0, 255, 0), 2);
	}

	cv::Rect eyeRect = getLeftmostEye(eyes);  // return the left most eye

	cv::Mat eye = face(eyeRect); // crop the left most eye
	cv::equalizeHist(eye, eye);  // improve image contrast

	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(eye, circles, CV_HOUGH_GRADIENT, 1, eye.cols / 8, 150, 15, eye.rows / 8, eye.rows / 3); // return vector of circles detected on eye mat


	if (circles.size() > 0)
	{
	  /* uncomment to draw all the circles from HoughCircles
	  for (size_t j = 0 ; j < circles.size() ; j++)
	  {
	    cv::Point centerJ(circles[j][0], circles[j][1]);
	    int radiusJ = circles[j][2];
	    cv::circle(frame, faces[0].tl() + eyeRect.tl() + centerJ, radiusJ, cv::Scalar(0, 0, 255), 2);
	  }*/

	  cv::Vec3f eyeball = getEyeball(eye, circles); // getEyeBall from eye mat
	  cv::Point center(eyeball[0], eyeball[1]);
	  int radius = eyeball[2];


	  centers.push_back(center); // put the new center from getEyeball in centers
	  radiuss.push_back(radius); // put the new radius from getEyeball in radiuss

	  center = stabilizeCenter(centers, 15); // return a center that is the adverage of the last 15 centers
	  radius = stabilizeRadius(radiuss, 15); // return a radius that is the adverage of the last 15 radius


	  cv::circle(frame, faces[0].tl() + eyeRect.tl() + center, radius, cv::Scalar(255, 0, 0), 2); // draw the optimized circle on the frame
	  cv::circle(eye, center, radius, cv::Scalar(255, 255, 255), 2); // draw the optimized circle on eye mat


	  changeMouse(eye, center); // change the mouse position by comparing the eyeball center and the eye mat center




	}
	cv::imshow("Eye", eye);
	cv::moveWindow("Eye", 960,0);
}

cv::Point stabilizeCenter(std::vector<cv::Point> &points, int n)
{
  float sumX = 0;
  float sumY = 0;
  int count = 0;
  for (size_t i = std::max(0, (int)(points.size() - n)); i < points.size(); i++) // sum the last n points
  {
      sumX += points[i].x;
      sumY += points[i].y;
      ++count;
  }
  if (count > 0) // take the adverage of points
  {
      sumX /= count;
      sumY /= count;
  }
  return cv::Point(sumX, sumY);
}


float stabilizeRadius(std::vector<float> &numbers, int n)
{
  float sum = 0;
  int count = 0;
  for (size_t i = std::max(0, (int)(numbers.size() - n)); i < numbers.size(); i++) // sum the last n radius
  {
      sum += numbers[i];
      ++count;
  }
  if (count > 0) // take the adverage of radius
  {
      sum /= count;
  }
  return (sum);
}



cv::Rect getLeftmostEye(std::vector<cv::Rect> &eyes)
{
  int leftmost = 10000; // the number of pixels on a screen rows is usualy 1920
  int leftmostIndex = -1;
  for (size_t  i = 0; i < eyes.size(); i++)
  {
      if (eyes[i].tl().x < leftmost) // find the most left eye
      {
          leftmost = eyes[i].tl().x;
          leftmostIndex = i;
      }
  }
  return eyes[leftmostIndex];
}



cv::Vec3f getEyeball(cv::Mat &eye, std::vector<cv::Vec3f> &circles)
{
  std::vector<int> sums(circles.size(), 0);
  for (int y = 0; y < eye.rows; y++)
  {
      uchar *ptr = eye.ptr<uchar>(y); // .ptr apply on row y of eye will return the pixel value in RGB
      for (int x = 0; x < eye.cols; x++)
      {
        int value = static_cast<int>(*ptr); // put and cast in value of a pixel,
                                            // in black and white R=G=B

          for (size_t i = 0; i < circles.size(); i++)
          {
              cv::Point center((int)std::round(circles[i][0]), (int)std::round(circles[i][1]));
              int radius = (int)std::round(circles[i][2]);
              if (std::pow(x - center.x, 2) + std::pow(y - center.y, 2) < std::pow(radius, 2))
              {
                  sums[i] += value;      // test if we are in the circle, if yes sum the value of the pixel
              }
          }
          ++ptr;
      }
  }
  int smallestSum = 9999999;
  int smallestSumIndex = -1;
  for (size_t i = 0; i < circles.size(); i++)
  {
      if (sums[i] < smallestSum) // find the smallest sum, closer to 0 <=> closer to black
      {
          smallestSum = sums[i];
          smallestSumIndex = i;
      }
  }
  return circles[smallestSumIndex];
}


void changeMouse(cv::Mat &cmEye, cv::Point &cmCenter)
{
  // in this algorithm we change the position of the mouse point
  // depending of the position of the eyeball compare to the center of the eye mat.
  if (cmCenter.x - cmEye.cols/2 > 4)
  {
    mousePoint.x = mousePoint.x + 10;
  }
  else if (cmCenter.x - cmEye.cols/2 < -4)
  {
    mousePoint.x = mousePoint.x - 10;
  }
  else // return toward the middle if the cmCenter.x is close to cmEye middle
  {
    if (mousePoint.x < 960)
      mousePoint.x = mousePoint.x + 10;
    else
      mousePoint.x = mousePoint.x - 10;
  }

  if (cmCenter.y - cmEye.rows/2 > 4)
  {
    mousePoint.y = mousePoint.y + 10;
  }
  else if (cmCenter.y - cmEye.rows/2 < -4)
  {
    mousePoint.y = mousePoint.y - 10;
  }
  else // return toward the middle if the cmCenter.y is close to cmEye middle
  {
    if (mousePoint.y < 540)
      mousePoint.y = mousePoint.y + 10;
    else
      mousePoint.y = mousePoint.y - 10;
  }

  /* uncomment to see the value used by changeMouse
  printf("cmEye.cols/2 = %d et cmCenter.x = %d \n",cmEye.cols/2 , cmCenter.x);
  printf("cmEye.rows/2 = %d et cmCenter.y = %d \n",cmEye.rows/2 , cmCenter.y); */

  system(("xdotool mousemove " + std::to_string(mousePoint.x) + " " + std::to_string(mousePoint.y)).c_str());
}
