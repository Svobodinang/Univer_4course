#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

Mat get_hist (const Mat& img)
{
    int hist_size = 256;
    float range[] = {0, 255};
    const float *ranges[] = { range };

    Mat hist;
    calcHist(&img, 1, 0, Mat(), hist, 1, &hist_size, ranges, true, false);


    int hist_w = 500;
    int hist_h = 500;
    int step_w = (double) hist_w / hist_size;

    Mat hist_img(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
    normalize(hist, hist, 0, hist_img.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < hist_size; i++)
    {
        line( hist_img, Point( step_w * (i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
              Point( step_w * (i), hist_h - cvRound(hist.at<float>(i)) ),
              Scalar( 0, 0, 0), 2, 8, 0  );
    }

    return hist_img;
}


int main ()
{

    Mat bgr[3];
    bool is_color_img {true};

    string color_img = "color.jpeg";
    string wb_img = "wb.jpeg";
    string img_name;

    if (is_color_img)
        img_name = color_img;
    else
        img_name = wb_img;

    Mat img = imread("../../data/" + img_name, is_color_img);


    namedWindow("Input image", CV_WINDOW_NORMAL);
    imshow("Input image", img);


    if (is_color_img)
    {
        split(img, bgr);

        namedWindow("Blue channel hist", CV_WINDOW_NORMAL);
        imshow("Blue channel hist", get_hist(bgr[0]));

        namedWindow("Green channel hist", CV_WINDOW_NORMAL);
        imshow("Green channel hist", get_hist(bgr[1]));

        namedWindow("Red channel hist", CV_WINDOW_NORMAL);
        imshow("Red channel hist", get_hist(bgr[2]));
    }
    else
    {
        namedWindow("Black-White image hist", CV_WINDOW_NORMAL);
        imshow("Black-White image hist", get_hist(img));
    }

    waitKey(0);
}