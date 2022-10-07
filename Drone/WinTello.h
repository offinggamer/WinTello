#pragma once

#include <iostream>
#include <string.h>
#include "Network.h"
#include <windows.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <sstream>

const char* const TELLO_STREAM_URL{ "udp://0.0.0.0:11111" };

using cv::CAP_FFMPEG;
using cv::imshow;
using cv::VideoCapture;
using cv::waitKey;
using namespace std;

static WSASession m_session;
static UDPSocket m_socket;

class Drone
{
public:
	Drone();

	~Drone();

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

	
	
private:

	int m_speed;
	int m_battery;
	int m_height;
	bool m_flight = false;
	std::string m_ip = "192.168.10.1";
	int m_port = 8889;
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