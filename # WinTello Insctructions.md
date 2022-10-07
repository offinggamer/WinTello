#  WinTello Insctructions
as the name indicates this library can only be used on Windows systems. It is not compatible with other operating systems, because the socket management varies between other OS-s and Windows systems.
<!---
## How to install the Tello library
**NOTE**: OpenCV Library should be preinstalled!
--->

## How to Use
### Functions:

#### remoteControll
##### Return Value:
-
##### Parameters:
			-fb: forward backward velocity(-100 - 100)
			-lr: left right velocity(-100 - 100)
			-ud: up down velocity(-100 - 100)
			-cwccw: turning velocity(-100 - 100)
##### Description:
Controlls the drone as it had a controller with 4 axes

#### remoteControllEasy

#### ascend
##### Return Value:
-
##### Parameters:
			-distance: how many cm should the drone ascend (20-500)

#### descend
##### Return Value:
-
##### Parameters:
			-distance: how many cm should the drone descend (20-500)

#### connect
##### Return Value:
-
##### Parameters:
			-Bitrate: set the bitrate of the video stream (1=1Mbps/2=2Mbps/3=3Mbps/4=4Mbps/5=5Mbps/0=Auto
			-VideoRes desidec what the resolution should be(480p=low/720p=high)
##### Description: 
connects to the drone only after the wlan is already connected

#### takeoff
##### Return Value:
-
##### Parameters:
-
##### Description:
Initiates Takeoff and hovers at a 50cm height

#### land
##### Return Value:
-
##### Parameters:
-
##### Description:
Lands the drone

#### getVideoStream
##### Return Value:
1 frame
##### Parameters:
-
##### Description:
captures and decodes a frame from the drone

#### getSpeed
##### Return Value:
-
##### Parameters:
-
##### Description:
Get speed in m/s to m_speed

#### getBattery
##### Return Value:
-
##### Parameters:
-
##### Description:
Get battery level in % to m_battery

#### getHeight
##### Return Value:
height of the drone in cm
##### Parameters:
-
##### Description:
Get height in cm