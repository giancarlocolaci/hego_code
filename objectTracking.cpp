//objectTracking.cpp

#include<sstream>
#include<string>
#include<iostream>
#include<opencv\highgui.h>
#include<opencv\cv.h>
#include<opencv2\opencv.hpp>
#include<vector>
#include<fstream>
#include<Windows.h>
#include<thread>

using namespace std;
using namespace cv;

//initial min and max HSV values of the filter
/*110
121
126
153
106
181
*/

int H_MIN = 0;
int H_MAX = 259;
int S_MIN = 0;
int S_MAX = 259;
int V_MIN = 0;
int V_MAX = 259;
//variable that limits the rotation
int posCam = 2;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 5;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 15 * 15;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH / 1.5;
//window's names for camera1
const String window1Name = "(1) Original Image";
const String window1Name1 = "(1) HSV Image";
const String window1Name2 = "(1) Thresholded Image";
const String window2Name = "(2) Original Image 1";
const String window2Name1 = "(2) HSV Image 1";
const String window2Name2 = "(2) Thresholded Image 1";
const String window3Name = "(3) Original Image 2";
const String window3Name1 = "(3) HSV Image 2";
const String window3Name2 = "(3) Thresholded Image 2";
const String windowName3 = "After Morphological Operations";
const String trackbarWindowName = "Trackbars";


void on_trackbar(int, void*)
{
	//Called when a trackbar position is changed
}


String intToString(int number){
	std::stringstream ss;
	ss << number;
	return ss.str();
}


void createTrackbars(){
	//create window for trackbars
	namedWindow(trackbarWindowName, 0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf_s(TrackbarName, "H_MIN", H_MIN);
	sprintf_s(TrackbarName, "H_MAX", H_MAX);
	sprintf_s(TrackbarName, "S_MIN", S_MIN);
	sprintf_s(TrackbarName, "S_MAX", S_MAX);
	sprintf_s(TrackbarName, "V_MIN", V_MIN);
	sprintf_s(TrackbarName, "V_MAX", V_MAX);
	//create trackbars      
	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);
}


void drawObject1(int x, int y, Mat &frame){
	//Draw crosshairs on the tracked image
	circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);
	if (y - 25>0)
		line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
	if (y + 25<FRAME_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
	if (x - 25>0)
		line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
	if (x + 25<FRAME_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);
	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);
}


void drawObject2(int k, int x, int y, Mat &frame){
	//Draw crosshairs on the tracked image
	circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);
	if (y - 25>0)
		line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
	if (y + 25<FRAME_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
	if (x - 25>0)
		line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
	if (x + 25<FRAME_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);
	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);
}

void drawObject3(int z, int x, int y, Mat &frame){
	//Draw crosshairs on the tracked image
	circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);
	if (y - 25>0)
		line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
	if (y + 25<FRAME_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
	if (x - 25>0)
		line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
	if (x + 25<FRAME_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);
	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);
}

void morphOps(Mat &thresh){
	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 2px by 2px rectangle
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(2, 2));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(9, 9));
	erode(thresh, thresh, erodeElement);
	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);
	erode(thresh, thresh, erodeElement);
}


void trackFilteredObject1(int &x, int &y, Mat threshold, Mat &cameraFeed, vector< vector<Point> > &contours, vector<Vec4i> &hierarchy, HANDLE hSerial, char outputChars[], DWORD btsIO){
	Mat temp;
	threshold.copyTo(temp);
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				if (area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
				}
				else objectFound = false;
			}
			if (objectFound == true){
				putText(cameraFeed, "Tracking", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
				//draw object location on screen
				drawObject1(x, y, cameraFeed);
				if (posCam == 2 && x > 0 && x < 640){
					outputChars[0] = 'x';
					WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
					printf("Muovo sinistra\n");
					//system("C:\\Users\\Giancarlo_Colaci\\Documents\\SportCam\\ptz_shell -com 3 -dir l -t 3");
					posCam = 1;
				}
				else if (posCam == 3 && x > 0 && x < 640){
					outputChars[0] = 'x';
					WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
					printf("Muovo sinistra\n");
					//system("C:\\Users\\Giancarlo_Colaci\\Documents\\SportCam\\ptz_shell -com 3 -dir l -t 6");
					posCam = 1;
				}
			}
		}
		else putText(cameraFeed, "Change Filters", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
}


void trackFilteredObject2(int &k, int &y, Mat threshold, Mat &cameraFeed, vector< vector<Point> > &contours, vector<Vec4i> &hierarchy, HANDLE hSerial, char outputChars[], DWORD btsIO){
	int x = 0;
	Mat temp;
	threshold.copyTo(temp);
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				if (area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					k = FRAME_WIDTH + (moment.m10 / area);
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
				}
				else objectFound = false;
			}
			if (objectFound == true){
				putText(cameraFeed, "Tracking", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
				//draw object location on screen
				drawObject2(k, x, y, cameraFeed);
				if (posCam == 1 && k > 640 && k < 1280){
					outputChars[0] = 'c';
					WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
					printf("Muovo centro\n");
					//system("C:\\Users\\Giancarlo_Colaci\\Documents\\SportCam\\ptz_shell -com 3 -dir r -t 3");
					posCam = 2;
				}
				else if (posCam == 3 && k > 640 && k < 1280){
					outputChars[0] = 'c';
					WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
					printf("Muovo centro\n");
					//system("C:\\Users\\Giancarlo_Colaci\\Documents\\SportCam\\ptz_shell -com 3 -dir l -t 3");
					posCam = 2;
				}
			}
		}
		else putText(cameraFeed, "Change Filters", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
}


void trackFilteredObject3(int &z, int &y, Mat threshold, Mat &cameraFeed, vector< vector<Point> > &contours, vector<Vec4i> &hierarchy, HANDLE hSerial, char outputChars[], DWORD btsIO){
	int x = 0;
	Mat temp;
	threshold.copyTo(temp);
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {
				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;
				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				if (area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					z = (2*FRAME_WIDTH) + (moment.m10 / area);
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
				}
				else objectFound = false;
			}
			if (objectFound == true){
				putText(cameraFeed, "Tracking", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
				//draw object location on screen
				drawObject3(z, x, y, cameraFeed);
				if (posCam == 1 && z > 1280 && z < 1920){
					outputChars[0] = 'v';
					WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
					printf("Muovo destra\n");
					//system("C:\\Users\\Giancarlo_Colaci\\Documents\\SportCam\\ptz_shell -com 3 -dir r -t 6");
					posCam = 3;
				}
				else if (posCam == 2 && z > 640 && z < 1280){
					outputChars[0] = 'v';
					WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
					printf("Muovo destra\n");
					//system("C:\\Users\\Giancarlo_Colaci\\Documents\\SportCam\\ptz_shell -com 3 -dir r -t 3");
					posCam = 3;
				}
			}
		}
		else putText(cameraFeed, "Change Filters", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
}


int main(int argc, char* argv[])
{
	//Setting the code for Arduino
	//Open a comunication on the COM5
	HANDLE hSerial = CreateFile(L"COM8", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hSerial != INVALID_HANDLE_VALUE)
	{
		printf("Port opened! \n");

		DCB dcbSerialParams;
		GetCommState(hSerial, &dcbSerialParams);

		dcbSerialParams.BaudRate = CBR_9600;
		dcbSerialParams.ByteSize = 8;
		dcbSerialParams.Parity = NOPARITY;
		dcbSerialParams.StopBits = ONESTOPBIT;

		SetCommState(hSerial, &dcbSerialParams);
	}
	else
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("Serial port doesn't exist! \n");
		}

		printf("Error while setting up serial port! \n");
	}
	//Variable in which i can save the instruction for Arduino: L left, R right, C centre
	char outputChars[] = "c";
	DWORD btsIO = 0;
	WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector <Mat> channel;
	bool trackObjects = true;
	bool useMorphOps = true;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed1;
	//Mat cameraFeed2;
	//Mat cameraFeed3;
	//matrix storage for HSV image
	Mat HSV1;
	Mat HSV2;
	Mat HSV3;
	//matrix storage for binary threshold image
	Mat threshold1;
	//Mat threshold2;
	//Mat threshold3;
	//x and y values for the location of the object
	int x = 0, y = 0, k = 0, z = 0;
	//create slider bars for HSV filtering
	createTrackbars();
	//video capture object to acquire webcam feed
	VideoCapture capture1;
	//VideoCapture capture2;
	//VideoCapture capture3;
	//open capture object at location zero (default location for webcam), 1 and 2
	capture1.open(0);
	//capture2.open(2);
	//capture3.open(1);

	//set height and width of capture frame
	capture1.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture1.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	//capture2.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	//capture2.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	//capture3.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	//capture3.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	while (1){
		//store image to matrix
		capture1.read(cameraFeed1);
		//capture2.read(cameraFeed2);
		//capture3.read(cameraFeed3);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed1, HSV1, COLOR_BGR2HSV);
		//cvtColor(cameraFeed2, HSV2, COLOR_BGR2HSV);
		//cvtColor(cameraFeed3, HSV3, COLOR_BGR2HSV);
		//filter HSV image between values and store filtered image to threshold matrix
		split(HSV1, channel);
		channel.pop_back();
		Mat ch2;
		merge(channel, ch2);
		inRange(ch2, Scalar(H_MIN, S_MIN/*, V_MIN*/), Scalar(H_MAX, S_MAX/*, V_MAX*/), threshold1);
		//inRange(HSV2, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold2);
		//inRange(HSV3, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold3);
		//perform morphological operations on thresholded image to eliminate noise and emphasize the filtered object(s)
		if (useMorphOps)
		{
			morphOps(threshold1);
			//morphOps(threshold2);
			//morphOps(threshold3);
		}
		//this function will return the x and y coordinates of the filtered object
		if (trackObjects) {
			trackFilteredObject1(x, y, threshold1, cameraFeed1, contours, hierarchy, hSerial, outputChars, btsIO);
			//trackFilteredObject2(k, y, threshold2, cameraFeed2, contours, hierarchy, hSerial, outputChars, btsIO);
			//trackFilteredObject3(z, y, threshold3, cameraFeed3, contours, hierarchy, hSerial, outputChars, btsIO);
		}
		//show frames 
		imshow(window1Name, cameraFeed1);
		imshow(window1Name1, HSV1);
		imshow(window1Name2, threshold1);
		//imshow(window2Name, cameraFeed2);
		//imshow(window2Name1, HSV2);
		//imshow(window2Name2, threshold2);
		//imshow(window3Name, cameraFeed3);
		//imshow(window3Name1, HSV3);
		//imshow(window3Name2, threshold3);
		//delay 30ms so that screen can refresh.
		waitKey(30);
	}
	return 0;
}

