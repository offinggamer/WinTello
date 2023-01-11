#include <iostream>
#include <opencv2/opencv.hpp>
#include "WinTello.h"

using namespace std;
using namespace cv;

int main()
{
    Mat Frame;
    Mat Croped_Frame;
    Mat Croped_Frame_Interpol;
    Size size(720, 480);
    EDU Tello;
    cout << "trying to connect to drone..." << endl;
    Tello.connect(0, string("high"));
    cout << "connected" << endl;
    Tello.setCameraDirection("Down");
    Tello.startVideoStream();
    VideoCapture Capture{ TELLO_STREAM_URL, CAP_FFMPEG };
    while (true) {

        Capture >> Frame;
        if (!Frame.empty())
        {
            cv::Rect Roi(0, 0, 320, 201);
            Frame(Roi).copyTo(Croped_Frame);
            cv::resize(Croped_Frame, Croped_Frame_Interpol, size, 0, 0, cv::INTER_LINEAR);
            Mat undistorted;
            //undistort image using the camera matrix and distortion coefficients
            undistort(Croped_Frame_Interpol, undistorted, Mat_Camera_Downward, Mat_Dist_Downward);

            // Blur the image to reduce noise
            GaussianBlur(undistorted, undistorted, Size(5, 5), 0);

            // Convert the image to grayscale
            cvtColor(undistorted, undistorted, COLOR_BGR2GRAY);

            // Use HoughCircles to detect circles in the image
            vector<Vec3f> circles;
            HoughCircles(undistorted, circles, HOUGH_GRADIENT, 1, undistorted.rows / 64, 200, 100, 0, 0);

            for (size_t i = 0; i < circles.size(); i++)
            {
                Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
                // Draw the circle center
                circle(undistorted, center, 3, Scalar(0, 255, 0), -1, 8, 0);
                // Draw the circle outline
                circle(undistorted, center, radius, Scalar(0, 0, 255), 3, 8, 0);

                // Get the estimated distance

                float x = Tello.getDistanceMarkerAndDroneX(center.x - 360, Tello.Camera_Downward, radius * 2, 10);
                float y = Tello.getDistanceMarkerAndDroneY(center.y - 240, Tello.Camera_Downward, radius * 2, 10);

                if (x < 0)
                {
                    Tello.flyLeft(x);
                }
                else {
                    Tello.flyRight(x);
                }

                if (y > 0)
                {
                    Tello.flyForward(y);
                }
                else {
                    Tello.flyBackward(y);
                }
            }

            imshow("Down Stream", undistorted);
        }
        if (waitKey(1) == 27)
        {
            return 0;
        }

    }

    destroyAllWindows();
    return 0;
}