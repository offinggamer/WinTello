#pragma once

#include <iostream>
#include <string.h>
#include "Network.h"
#include <windows.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/opencv.hpp"
#include <sstream>

const char* const TELLO_STREAM_URL{ "udp://0.0.0.0:11111" };

using cv::CAP_FFMPEG;
using cv::imshow;
using cv::VideoCapture;
using cv::waitKey;
using namespace std;

float camera_matrix_Forward[3][3] = { {936.381996086254, 0, 496.20744875663297}, {0, 940.0142999256417, 317.543016377983}, {0, 0, 1} };
float dist_coeff_Forward[5] = { 0.02389703394583855, -0.3728288295235468, -0.019945325217444083, 0.00221201435709584, 1.0185154304683173 };

cv::Mat Mat_Camera_Forward(3, 3, CV_32F, camera_matrix_Forward);
cv::Mat Mat_Dist_Forward(5, 1, CV_32F, camera_matrix_Forward);

float camera_matrix_Downward[3][3] = { {936.381996086254, 0, 496.20744875663297}, {0, 940.0142999256417, 317.543016377983}, {0, 0, 1} };
float dist_coeff_Downward[5] = { 0.02389703394583855, -0.3728288295235468, -0.019945325217444083, 0.00221201435709584, 1.0185154304683173 };

cv::Mat Mat_Camera_Downward(3, 3, CV_32F, camera_matrix_Downward);
cv::Mat Mat_Dist_Downward(5, 1, CV_32F, camera_matrix_Downward);

static WSASession m_session;
static UDPSocket m_socket;

class Drone
{
protected:
	Drone();

	~Drone();

public:

	

	/*--------------------------------------------------------------------------------------------
	NAME: remoteControll
	Return Value: -
	Parameters:
				-fb: forward backward velocity(-100 - 100)
				-lr: left right velocity(-100 - 100)
				-ud: up down velocity(-100 - 100)
				-cwccw: turning velocity(-100 - 100)
	Description: Controlls the drone as it had a controller with 4 axes
	----------------------------------------------------------------------------------------------*/
	void remoteControll(int fb, int lr, int ud, int cwccw);

	
	/*--------------------------------------------------------------------------------------------
	NAME: remoteControllEasy
	Return Value: -                                                   0°
	Parameters:
				-alpha: direction of the manouver             270°  Drone   90°
				-speed: speed of the drone from 1 to 100
	Description: Easy controll for the drone                         180°
	----------------------------------------------------------------------------------------------*/
	void remoteControllEasy(int alpha, int speed);


	/*--------------------------------------------------------------------------------------------
	NAME: ascend
	Return Value: -
	Parameters:
				-distance: how many cm should the drone ascend (20-500)
	----------------------------------------------------------------------------------------------*/
	void ascend(int distance);


	/*--------------------------------------------------------------------------------------------
	NAME: descend
	Return Value: -
	Parameters:
				-distance: how many cm should the drone descend (20-500)
	----------------------------------------------------------------------------------------------*/
	void descend(int distance);


	/*--------------------------------------------------------------------------------------------
	NAME: connect
	Return Value: -
	Parameters:
				-Bitrate: set the bitrate of the video stream (1=1Mbps/2=2Mbps/3=3Mbps/4=4Mbps/5=5Mbps/0=Auto
				-VideoRes desidec what the resolution should be(480p=low/720p=high)
	Description: connects to the drone only after the wlan is already connected
	----------------------------------------------------------------------------------------------*/
	void connect(int Bitrate, string VideoRes);


	/*--------------------------------------------------------------------------------------------
	NAME: takeoff
	Return Value: -
	Parameters: -
	Description: Initiates Takeoff and hovers at a 50cm height
	----------------------------------------------------------------------------------------------*/
	void takeoff();


	/*--------------------------------------------------------------------------------------------
	NAME: land
	Return Value: -
	Parameters: -
	Description: Lands the drone
	----------------------------------------------------------------------------------------------*/
	void land();


	/*--------------------------------------------------------------------------------------------
	NAME: getSpeed
	Return Value: -
	Parameters: -
	Description: Get speed in m/s to m_speed
	----------------------------------------------------------------------------------------------*/
	void getSpeed();


	/*--------------------------------------------------------------------------------------------
	NAME: getHeight
	Return Value: height of the drone in cm
	Parameters: -
	Description: Get height in cm
	----------------------------------------------------------------------------------------------*/
	int getHeight() const;


	/*--------------------------------------------------------------------------------------------
	NAME: isDroneFlying
	Return Value: bool about the status of the drone(true = flying / false = not flying)
	Parameters: -
	Description: returns wether the drone is in the air or not
	----------------------------------------------------------------------------------------------*/
	bool isDroneFlying() const;


	/*--------------------------------------------------------------------------------------------
	NAME: startVideoStream
	Return Value: -
	Parameters: -
	Description: starts the udp server on the drone
	----------------------------------------------------------------------------------------------*/
	void startVideoStream();

	/*--------------------------------------------------------------------------------------------
	NAME: getBattery
	Return Value: Current capacity of the Battery in %
	Parameters: -
	Description: Get battery %
	----------------------------------------------------------------------------------------------*/
	int getBattery();

	/*--------------------------------------------------------------------------------------------
	NAME: flyForward
	Return Value: -
	Parameters: distance in cm 20 - 500
	Description: controlls the drone in the forward direction at the given distance
	----------------------------------------------------------------------------------------------*/
	void flyForward(int speed);

	/*--------------------------------------------------------------------------------------------
	NAME: flyBackward
	Return Value: -
	Parameters: distance in cm 20 - 500
	Description: controlls the drone in the forward direction at the given distance
	----------------------------------------------------------------------------------------------*/
	void flyBackward(int speed);

	/*--------------------------------------------------------------------------------------------
	NAME: flyRight
	Return Value: -
	Parameters: distance in cm 20 - 500
	Description: controlls the drone in the forward direction at the given distance
	----------------------------------------------------------------------------------------------*/
	void flyRight(int speed);

	/*--------------------------------------------------------------------------------------------
	NAME: flyLeft
	Return Value: -
	Parameters: distance in cm 20 - 500
	Description: controlls the drone in the forward direction at the given distance
	----------------------------------------------------------------------------------------------*/
	void flyLeft(int speed);

	/*--------------------------------------------------------------------------------------------
	NAME: getDistanceMarkerAndDrone
	Return Value: The Estimated Distance between the Drone and the Marker on the x axis
	Parameters: 
				- x: the distance between the center of the marker and the center of the picture on the x axis in px
				- distance: distance between marker and drone in cm
	Description: controlls the drone to the marker on the horizontal plane
	----------------------------------------------------------------------------------------------*/
	int getDistanceMarkerAndDroneX(int x, float Camera, float objectsize, float realsize);

	/*--------------------------------------------------------------------------------------------
	NAME: getDistanceMarkerAndDrone
	Return Value: The Estimated Distance between the Drone and the Marker on the y axis
	Parameters:
				- y: the distance between the center of the marker and the center of the picture on the y axis in px
				- distance: distance between marker and drone in cm
	Description: controlls the drone to the marker on the vertical plane
	----------------------------------------------------------------------------------------------*/
	int getDistanceMarkerAndDroneY(int x, float Camera, float objectsize, float realsize);

	const float Camera_Forward = Mat_Camera_Forward.at<double>(0, 0) * 0.5;
	const float Camera_Downward = Mat_Camera_Downward.at<double>(0, 0) * 0.5;

protected:

private:

	int m_speed;
	int m_battery;
	int m_height;
	bool m_flight = false;
	std::string m_ip = "192.168.10.1";
	int m_port = 8889;
};

class Tello : public Drone
{
public:
	Tello();


private:

};

class EDU : public Drone
{
public:
	EDU();

	/*--------------------------------------------------------------------------------------------
	NAME: setCameraDirection
	Return Value: -
	Parameters: the direction of the camera that should be used (Down/Up)
	Description: selects the camera for the video stream (downfacing or forward facing camera)
	----------------------------------------------------------------------------------------------*/
	bool setCameraDirection(string direction);

	string getCameraDirection();

private:
	string m_direction;

};


/*--------------------------------------------------------------------------------------------
NAME: SendCommandNoAnswer
Return Value:	-
Parameters:		message in string
Description:	send a message to the drone and returns no answer
----------------------------------------------------------------------------------------------*/
void SendCommandNoAnswer(string msg);


/*--------------------------------------------------------------------------------------------
NAME: SendCommand
Return Value:	the answer of the drone
Parameters:		message in string
Description:	send a message to the drone
----------------------------------------------------------------------------------------------*/
string SendCommand(string msg);



