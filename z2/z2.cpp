#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;


const double MAX_VAL = 100;
Mat inp, out;


double fun(double x, double a)
{
	if (a == 0)
		return x;
	else
		return max(255.0, x / a + a * 2 / x);
}

void trackbar_callback(int value, void *user_data)
{
	for (int i = 0; i < inp.rows; i++)
	{
		for (int j = 0; j < inp.cols; j++)
		{
			out.at<float>(i, j) = fun(inp.at<float>(i, j), (double)value / MAX_VAL);
		}
	}
}

Mat res;

int main()
{

	inp = imread("../../data/wb.jpeg", CV_32FC1);
	out = inp.clone();

	namedWindow("Track_bar", CV_WINDOW_NORMAL);

	int cur_pos = 0;
	createTrackbar("parameter", "Track_bar", &cur_pos, MAX_VAL, trackbar_callback);


	while (1)
	{
		hconcat(inp, out, res);
		imshow("Track_bar", res);
		int key = waitKey(30);
		if (key == ' ')
			break;

	}
}