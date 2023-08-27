#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	string path = "Resources/test.png";
	Mat img = imread(path);

	CascadeClassifier faceCascade;
	faceCascade.load("Resources/haarcascade_frontalface_default.xml");

	if (faceCascade.empty()) { cout << "XML file not loaded" << endl; }

	vector<Rect> faces; //用于存储检测到的面部区域。每个元素都是一个表示面部区域的矩形对象
	faceCascade.detectMultiScale(img, faces,1.1, 10);

	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3);
	}

	imshow("Image", img);
	waitKey(0);

	return 0;
}
