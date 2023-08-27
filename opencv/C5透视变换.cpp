#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

float w = 250, h = 350;
Mat matrix, imgWarp;

int main()
{
	// 读取图片
	string path = "Resources/cards.jpg";
	Mat img = imread(path);
	// 四个点位置
	Point2f src[4] = { {529, 142}, {771, 190}, {405, 395}, {674, 457} };
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

	// 获取透视变换矩阵
	matrix = getPerspectiveTransform(src, dst);
	// 透视变换
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	// 显示
	/*for (int i = 0; i < 4; i++) {
		circle(img, src[i], 10, Scalar(0, 0, 255), FILLED);
	}*/

	imshow("Image", img);
	imshow("ImageWarp", imgWarp);
	waitKey(0);

	return 0;
}
