#include "WinTello.h"

int main()
{
	cv::Mat Frame;
	cv::Mat Croped_Frame;
	cv::Mat Croped_Frame_Interpol;
    cv::Size size(720, 480);
	EDU Tello;
	cout << "trying to connect to drone..." << endl;
	Tello.connect(0, string("high"));
	cout << "connected" << endl;
	Tello.setCameraDirection("Down");
	Tello.startVideoStream();
	VideoCapture Capture{ TELLO_STREAM_URL, CAP_FFMPEG };
	while (1)
	{
		Capture >> Frame;
		if (!Frame.empty())
		{
			cv::Rect Roi (0, 0, 320,201);
			Frame(Roi).copyTo(Croped_Frame);
			cv::resize(Croped_Frame, Croped_Frame_Interpol, size, 0, 0, cv::INTER_LINEAR);
			imshow("Down Stream", Croped_Frame_Interpol);
		}
		if (waitKey(1) == 27)
		{
			return 0;
		}
	}
	return 0;
}

