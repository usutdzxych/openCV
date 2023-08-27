#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	string path = "resources/test.png";
	Mat img = imread(path);
	Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

	cvtColor(img, imgGray, COLOR_BGR2GRAY); //�ҶȻ�
	GaussianBlur(img, imgBlur, Size(3, 3), 3, 0); //��˹ģ��
	Canny(imgBlur, imgCanny, 25, 75); //��Ե���

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	erode(imgDil, imgErode, kernel);

	imshow("Image", img);
	imshow("ImageGray", imgGray);
	imshow("ImageBlur", imgBlur);
	imshow("ImageCanny", imgCanny);
	imshow("ImageDilation", imgDil);
	imshow("ImageErode", imgErode);
	waitKey(0);

	return 0;
}
