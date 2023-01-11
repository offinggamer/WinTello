#include "WinTello.h"



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




void Drone::remoteControll(int fb, int lr, int ud, int cwccw)
{
	string command = "rc ";
	command = command + to_string(lr) + " " + to_string(fb) + " " + to_string(ud) + " " + to_string(cwccw);
	m_socket.SendTo("192.168.10.1", 8889, command.c_str(), command.size());
}


void Drone::remoteControllEasy(int alpha, int speed)
{
	int fb;
	int lr;
	fb = speed * cos(alpha * 3.14159 / 180);
	lr = speed * sin(alpha * 3.14159 / 180);

	string command = "rc ";
	command = command + to_string(lr) + " " + to_string(fb) + " 0 0";
	m_socket.SendTo("192.168.10.1", 8889, command.c_str(), command.size());
}


void Drone::ascend(int distance)
{
	string command = "up ";
	command += to_string(distance);
	SendCommandNoAnswer(command);
	m_height += distance;
}


void Drone::descend(int distance)
{
	string command = "down ";
	command += to_string(distance);
	SendCommandNoAnswer(command);
	m_height -= distance;
}


void Drone::connect(int Bitrate, string VideoRes)
{
	SendCommandNoAnswer(string("command"));

	string command = "setbitrate ";
	command += to_string(Bitrate);
	SendCommandNoAnswer(command);

	command = "setresolution ";
	command += VideoRes;
	SendCommandNoAnswer(command);
}


void Drone::takeoff()
{
	SendCommandNoAnswer(string("takeoff"));
	m_flight = true;
	m_height = 50;
}


void Drone::land()
{
	SendCommandNoAnswer(string("land"));
	m_flight = false;
	m_height = 0;
}


void Drone::startVideoStream()
{
	SendCommandNoAnswer(string("streamon"));
}

void Drone::getSpeed()
{
	m_speed = stoi(SendCommand(string("speed?")));
}

void Drone::flyForward(int speed)
{
	string command = "forward ";
	command += to_string(speed);
	SendCommandNoAnswer(command);
}

void Drone::flyBackward(int speed)
{
	string command = "back";
	command += to_string(speed);
	SendCommandNoAnswer(command);
}

void Drone::flyRight(int speed)
{
	string command = "right";
	command += to_string(speed);
	SendCommandNoAnswer(command);
}

void Drone::flyLeft(int speed)
{
	string command = "left";
	command += to_string(speed);
	SendCommandNoAnswer(command);
}


int Drone::getHeight() const
{
	return m_height;
}


bool Drone::isDroneFlying() const
{
	return m_flight;
}

int Drone::getBattery()
{
	m_battery = stoi(SendCommand(string("battery?")));
	return m_battery;
}

int Drone::getDistanceMarkerAndDroneX(int x, float Camera, float objectsize, float realsize)
{
	float distance = realsize * Camera * objectsize;
	return distance / (Camera_Forward * x);
}

int Drone::getDistanceMarkerAndDroneY(int y, float Camera, float objectsize, float realsize)
{
	float distance = realsize * Camera * objectsize;
	return distance / (Camera_Forward * y);
}

bool EDU::setCameraDirection(string direction = "Up")
{
	m_direction = direction;
	if (direction == "Up")
	{
		string command = "downvision 0";
		m_socket.SendTo("192.168.10.1", 8889, command.c_str(), command.size());
	}
	else if (direction == "Down")
	{
		//string command = "downvision 1";
		//m_socket.SendTo("192.168.10.1", 8889, command.c_str(), command.size());
		//printf("downvision");
		cout << SendCommand(string("downvision 1")) << endl;
	}
	else
	{
		return false;
	}
	return true;
}


string EDU::getCameraDirection()
{
	return m_direction;
}

Tello::Tello() : Drone()
{}

EDU::EDU() : Drone()
{}


string SendCommand(string msg)
{
	char buffer[100];
	m_socket.SendTo("192.168.10.1", 8889, msg.c_str(), msg.size());
	sockaddr_in add = m_socket.RecvFrom(buffer, sizeof(buffer));
	int i = 0;
	do {
		i++;
		m_socket.SendTo("192.168.10.1", 8889, msg.c_str(), msg.size());
		sockaddr_in add = m_socket.RecvFrom(buffer, sizeof(buffer));

	} while (buffer == "error" && i <= 5);

	return buffer;
}


void SendCommandNoAnswer(string msg)
{

	char buffer[100];
	int i = 0;
	do {
		i++;
		m_socket.SendTo("192.168.10.1", 8889, msg.c_str(), msg.size());
		sockaddr_in add = m_socket.RecvFrom(buffer, sizeof(buffer));

	} while (buffer == "error" && i <= 5);
}
