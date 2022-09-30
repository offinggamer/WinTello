//Tello library

#include <iostream>
#include <string.h>
#include "Network.h"
#include <windows.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"

const char* const TELLO_STREAM_URL{ "udp://0.0.0.0:11111" };

using cv::CAP_FFMPEG;
using cv::imshow;
using cv::VideoCapture;
using cv::waitKey;
using namespace std;

class Drone
{
public:
	Drone();

	~Drone();

	void remoteControll(int fb, int lr, int ud, int cwccw);

	void remoteControllEasy(int alpha, int speed);

	void ascend(int distance);

	void descend(int distance);

	void connect(int Bitrate, string VideoRes);

	void takeoff();

	void land();

	cv::Mat getVideoStream();

	void getSpeed();

	void getBattery();

	int getHeight() const;

	static WSASession m_session;
	static UDPSocket m_socket;

private:

	int m_speed;
	int m_battery;
	int m_height;
	bool m_flight=false;
	std::string m_ip = "192.168.10.1";
	int m_port = 8889;
	cv::Mat m_frame;
};

Drone::Drone()
{
	m_socket.Bind(m_port);
}

Drone::~Drone()
{
	SendCommandNoAnswer(string("streamoff"));
	SendCommandNoAnswer(string("land"));
	m_flight = false;
}

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
void Drone::remoteControll(int fb, int lr, int ud, int cwccw)
{
	SendCommandNoAnswer(string("rc %d %d %d %d", &lr, &fb, &ud, &cwccw));
}

/*--------------------------------------------------------------------------------------------
NAME: remoteControllEasy
Return Value: -                                                   0°
Parameters:
			-alpha: direction of the manouver             270°  Drone   90°
			-speed: speed of the drone from 1 to 100
Description: Easy controll for the drone                         180°
----------------------------------------------------------------------------------------------*/
void Drone::remoteControllEasy(int alpha, int speed)
{
	int fb;
	int lr;
	fb = speed * sin(alpha);
	lr = speed * cos(alpha);

	SendCommandNoAnswer(string("rc %d %d 0 0", &lr, &fb));
}

/*--------------------------------------------------------------------------------------------
NAME: ascend
Return Value: -
Parameters:
			-distance: how many cm should the drone ascend (20-500)
----------------------------------------------------------------------------------------------*/
void Drone::ascend(int distance)
{
	SendCommandNoAnswer(string("up %d", &distance));
	m_height += distance;
}

/*--------------------------------------------------------------------------------------------
NAME: descend
Return Value: -
Parameters:
			-distance: how many cm should the drone descend (20-500)
----------------------------------------------------------------------------------------------*/
void Drone::descend(int distance)
{
	SendCommandNoAnswer(string("down % d", & distance));
	m_height -= distance;
}

/*--------------------------------------------------------------------------------------------
NAME: connect
Return Value: -
Parameters:
			-Bitrate: set the bitrate of the video stream (1=1Mbps/2=2Mbps/3=3Mbps/4=4Mbps/5=5Mbps/0=Auto
			-VideoRes desidec what the resolution should be(480p=low/720p=high)
Description: connects to the drone only after the wlan is already connected
----------------------------------------------------------------------------------------------*/
void Drone::connect(int Bitrate, string VideoRes)
{
	SendCommandNoAnswer(string("command"));

	SendCommandNoAnswer(string("setbitrate %d", &Bitrate));

	SendCommandNoAnswer(string("setresolution %s", &VideoRes));
}

/*--------------------------------------------------------------------------------------------
NAME: takeoff
Return Value: -
Parameters: -
Description: Initiates Takeoff and hovers at a 50cm height
----------------------------------------------------------------------------------------------*/
void Drone::takeoff()
{
	SendCommandNoAnswer(string("takeoff"));
	m_flight = true;
	m_height = 50;
}

/*--------------------------------------------------------------------------------------------
NAME: land
Return Value: -
Parameters: -
Description: Lands the drone
----------------------------------------------------------------------------------------------*/
void Drone::land()
{
	SendCommandNoAnswer(string("land"));
	m_flight = false;
	m_height = 0;
}

/*--------------------------------------------------------------------------------------------
NAME: getVideoStream
Return Value: 1 frame
Parameters: -
Description: captures and decodes a frame from the drone
----------------------------------------------------------------------------------------------*/
cv::Mat Drone::getVideoStream()
{
	SendCommandNoAnswer(string("streamon"));
	VideoCapture capture{ TELLO_STREAM_URL, CAP_FFMPEG };
	capture >> m_frame;
	if (!m_frame.empty())
	{
		return m_frame;
	}
}

/*--------------------------------------------------------------------------------------------
NAME: getSpeed
Return Value: -
Parameters: -
Description: Get speed in m/s to m_speed
----------------------------------------------------------------------------------------------*/
void Drone::getSpeed()
{
	m_speed = stoi(SendCommand(string("speed?")));
}

/*--------------------------------------------------------------------------------------------
NAME: getBattery
Return Value: -
Parameters: -
Description: Get battery level in % to m_battery
----------------------------------------------------------------------------------------------*/
void Drone::getBattery()
{
	m_battery = stoi(SendCommand(string("battery?")));
}

/*--------------------------------------------------------------------------------------------
NAME: getHeight
Return Value: height of the drone in cm
Parameters: -
Description: Get height in cm
----------------------------------------------------------------------------------------------*/
int Drone::getHeight() const
{
	return m_height;
}

/*--------------------------------------------------------------------------------------------
NAME: SendCommand
Return Value:	the answer of the drone
Parameters:		message in string
Description:	send a message to the drone
----------------------------------------------------------------------------------------------*/
string SendCommand(string msg)
{
	char buffer[100];
	Drone::m_socket.SendTo("192.168.10.1", 8889, msg.c_str(), msg.size());
	sockaddr_in add = Drone::m_socket.RecvFrom(buffer, sizeof(buffer));
	int i = 0;
	do {
		i++;
		Drone::m_socket.SendTo("192.168.10.1", 8889, msg.c_str(), msg.size());
		sockaddr_in add = Drone::m_socket.RecvFrom(buffer, sizeof(buffer));

	} while (buffer == "error" && i <= 5);

	return buffer;
}

/*--------------------------------------------------------------------------------------------
NAME: SendCommandNoAnswer
Return Value:	-
Parameters:		message in string
Description:	send a message to the drone and returns no answer
----------------------------------------------------------------------------------------------*/
void SendCommandNoAnswer(string msg)
{
	
	char buffer[100];
	Drone::m_socket.SendTo("192.168.10.1", 8889, msg.c_str(), msg.size());
	sockaddr_in add = Drone::m_socket.RecvFrom(buffer, sizeof(buffer));
	int i = 0;
	do {
		i++;
		Drone::m_socket.SendTo("192.168.10.1", 8889, msg.c_str(), msg.size());
		sockaddr_in add = Drone::m_socket.RecvFrom(buffer, sizeof(buffer));

	} while (buffer != "ok" && i <= 5);
}

void main()
{
	Drone Tello;
	Tello.connect(0, string ("high"));
	Sleep(2000);
	Tello.takeoff();
	Sleep(10000);
	Tello.land();
}

