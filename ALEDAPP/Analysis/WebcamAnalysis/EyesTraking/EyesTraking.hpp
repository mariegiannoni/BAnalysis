#ifndef _EYESTRAKING
#define _EYESTRAKING


// This is the main function of the program.
// This function detect the face and eye thanks to the classifier.
// It will call getLeftmostEye, HoughCircles, getEyeball, stabilizeCenter and stabilizeRadius,
// Draw a circle around the eyeball with "circle" and change the mouse position with changeMouse.
void detectEyes(cv::Mat &frame, cv::CascadeClassifier &faceCascade, cv::CascadeClassifier &eyeCascade);

// This function receive 2 eyes and return the most left one.
cv::Rect getLeftmostEye(std::vector<cv::Rect> &eyes);

// This function receive all the circles detected on eye with the function HoughCircles,
// and return the one that is the most black.
cv::Vec3f getEyeball(cv::Mat &eye, std::vector<cv::Vec3f> &circles);

// This function receive a vector of points composed of all the centers of circle from getEyeball,
// and make an adverage of the n last one.
cv::Point stabilizeCenter(std::vector<cv::Point> &points, int n);

// This function receive a vector of floats composed of all the radius of circle from getEyeball,
// and make an adverage of the n last one.
float stabilizeRadius(std::vector<float> &numbers, int n);

// This function define the position of the mouse on the screen.
// This position depend of the position of the eyeball compare to the center of the eye mat.
void changeMouse(cv::Mat &cmEye, cv::Point &cmCenter);

#endif
