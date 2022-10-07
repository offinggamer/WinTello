#include "WinTello.h"

int main()
{
	cv::Mat Frame;
	Drone Tello;
	cout << "trying to connect to drone..." << endl;
	Tello.connect(0, string ("high"));
	cout << "connected" << endl;
	Tello.startVideoStream();
	VideoCapture Capture{ TELLO_STREAM_URL, CAP_FFMPEG };
	while (1)
	{
		Capture >> Frame;
		if (!Frame.empty())
		{
			imshow("CTello Stream", Frame);
		}
		if (waitKey(1) == 27)
		{
			return 0;
		}
	}
	return 0;
}