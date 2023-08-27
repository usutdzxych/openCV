#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img;
vector<vector<int>> newPoints;

vector<vector<int>> myColors{ {124, 48, 117, 143, 170, 255}, //purple
								{68, 72, 156, 102, 126, 255} }; //green

vector<Scalar> myColorValues{ {255, 0, 255}, //purple
								{0, 255, 0} }; //green

Point getContours(Mat imgDil) {

	vector<vector<Point>> contours; //��������
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //ͨ��Ԥ����Ķ�ֵͼ���ҵ���������contours
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2); //������������(���˳�����)
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]); //����ÿ����������
		cout << area << endl;

		if (area > 1000) //��������
		{
			//�������Ľ��ƶ���λ�����
			double peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]); //��ÿ���������ߵ���С�ϱ߽����
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;

			//drawContours(img, conPoly, i, Scalar(255, 0, 255), 2); //�����˳����������������
			//rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5); //���Ʊ߽��
		}
	}
	return myPoint; //���ؾ��ο��ϱ߽��е�����
}

vector<vector<int>> findColor(Mat img)
{
	Mat imgHSV, mask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		Point myPoint = getContours(mask); //����mask�õ���⵽��ǰ��ɫ���ο���ϱ߽��е�����

		if (myPoint.x != 0 && myPoint.y != 0)
		{
			newPoints.push_back({ myPoint.x, myPoint.y, i }); //�õ���ǰ֡�����ɫ��Ŀ���
		}
	}
	return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
	}
}

int main()
{
	VideoCapture cap(0);

	while (true)
	{
		cap.read(img);
		newPoints = findColor(img);
		drawOnCanvas(newPoints, myColorValues);

		imshow("Image", img);
		waitKey(1);
	}

	return 0;
}
