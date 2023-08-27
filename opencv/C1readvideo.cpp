#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	string path = "Resources/test_video.mp4";
	VideoCapture cap(path); //ÊÓÆµ²¶×½¶ÔÏó
	Mat img;
	while (true) {

		cap.read(img);
		try
		{
			imshow("Image", img);
			waitKey(2);
			
		}
		catch (const exception& e)
		{
			destroyAllWindows();
			break;
		}
		
	}
	return 0;
}
