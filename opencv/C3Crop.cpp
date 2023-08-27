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
	Mat imgResize, imgCrop,imgCrop1;

	cout << img.size() << endl;
	resize(img, imgResize, Size(), 0.5, 0.5);

	Rect roi(200, 100, 300, 300);
	imgCrop = img(roi);
	Rect roi1(200, 150, 300, 300);
	imgCrop1 = img(roi1);

	imshow("Image", img);
	imshow("ImageResieze", imgResize);
	imshow("ImageCrop", imgCrop);
	imshow("ImageCrop1", imgCrop1);
	waitKey(0);

	return 0;
}
