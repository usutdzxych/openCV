#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat imgOriginal, imgGray, imgBlur, imgCanny, imgThre, imgDil, imgErode, imgWarp, imgCrop;
vector<Point> initialPoints, docPoints;

float w = 420, h = 596;


Mat preProcessing(Mat img)
{
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	//erode(imgDil, imgErode, kernel);
	return imgDil;
}

vector<Point> getContours(Mat imgDil) {

	vector<vector<Point>> contours; //轮廓数据
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //通过预处理的二值图像找到所有轮廓contours
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2); //绘制所有轮廓(不滤除噪声)
	vector<vector<Point>> conPoly(contours.size());
	vector<Point> biggest;
	int maxArea = 0;

	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]); //计算每个轮廓区域
		cout << area << endl;

		if (area > 1000) //过滤噪声
		{
			//找轮廓的近似多边形或曲线
			double peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			if (area > maxArea && conPoly[i].size() == 4) {

				//drawContours(imgOriginal, conPoly, i, Scalar(255, 0, 255), 5); //绘制滤除噪声后的所有轮廓
				biggest = { conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3] };
				maxArea = area;

			}
		}
	}
	return biggest; //返回最大轮廓四个点的坐标
}

void drawPoints(vector<Point> points, Scalar color)
{
	for (int i = 0; i < points.size(); i++)
	{
		circle(imgOriginal, points[i], 10, color, FILLED);
		putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 4, color, 4);
	}
}

vector<Point> reorder(vector<Point> points)
{
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;

	for (int i = 0; i < 4; i++)
	{
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}

	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); //0
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); //1
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); //2
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); //3

	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points, float w, float h)
{
	Point2f src[4] = { points[0], points[1], points[2], points[3] };
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));
	return imgWarp;
}


int main()
{
	string path = "Resources/paper.jpg";
	imgOriginal = imread(path);
	//resize(imgOriginal, imgOriginal, Size(), 0.5, 0.5);

	//Preprocessing
	imgThre = preProcessing(imgOriginal);
	//Get Contours - Biggest
	initialPoints = getContours(imgThre);
	//drawPoints(initialPoints, Scalar(0, 0, 255));
	docPoints = reorder(initialPoints);
	//drawPoints(docPoints, Scalar(0, 255, 0));

	//Warp
	imgWarp = getWarp(imgOriginal, docPoints, w, h);

	//Crop
	int cropValue = 5;
	Rect roi(cropValue, cropValue, w - (2 * cropValue), h - (2 * cropValue));
	imgCrop = imgWarp(roi);

	imshow("Image", imgOriginal);
	imshow("Image Dilation", imgThre);
	imshow("Image Warp", imgWarp);
	imshow("Image Crop", imgCrop);
	waitKey(0);

	return 0;
}
