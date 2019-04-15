#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


int sqr_side = 200;
int input_w = sqr_side * 3;
int input_h = sqr_side * 2;




vector <Scalar> colors = {Scalar(0,0,0), Scalar(127,127,127), Scalar(255,255,255)};

vector <vector <Scalar> > sqr_color_map {{colors[1], colors[0]},
                                         {colors[2], colors[1]},
                                         {colors[0], colors[2]}};

vector <vector <Scalar> > cirlce_color_map{{colors[0], colors[2]},
                                          {colors[1], colors[0]},
                                          {colors[2], colors[1]}};


int main ()
{
    Mat input (input_w, input_h, CV_8UC1);

    for (int i = 0; i < sqr_color_map.size(); i++)
    {
        for (int j = 0; j < sqr_color_map[i].size(); j++)
        {
            int a = j * sqr_side;
            int b = i * sqr_side;


            rectangle(input, Rect(a, b, a + sqr_side, b + sqr_side), sqr_color_map[i][j], CV_FILLED);
            circle(input, Point(a + sqr_side / 2, b + sqr_side / 2), sqr_side / 2,  cirlce_color_map[i][j], CV_FILLED);
        }
    }


    imshow("input", input);

    Mat res1, res2;
    Mat kernel(2, 2, CV_32FC1);
    kernel.at<float>(0, 0) = -1;
    kernel.at<float>(0, 1) = 0;
    kernel.at<float>(1, 0) = 0;
    kernel.at<float>(1, 1) = 1;

    Mat kernel2(2, 2, CV_32FC1);
    kernel2.at<float>(0, 0) = 0;
    kernel2.at<float>(0, 1) = 1;
    kernel2.at<float>(1, 0) = -1;
    kernel2.at<float>(1, 1) = 0;


    filter2D(input,res1, -1, kernel, Point(-1, -1), 128, BORDER_REPLICATE);
    filter2D(input,res2, -1, kernel2, Point(-1, -1), 128, BORDER_REPLICATE);

    imshow("res1", res1);
    imshow("res2", res2);


    Scalar m_inp = mean(input);
    Scalar m_res1 = mean(res1);
    Scalar m_res2 = mean(res2);

    cout << "sqrt(res1_mean^2 - inp_mean^2) : " << sqrtf(m_res1[0] * m_res1[0] - m_inp[0] * m_inp[0]) << endl;
    cout << "sqrt(res2_mean^2 - inp_mean^2) : " << sqrtf(m_res2[0] * m_res2[0] - m_inp[0] * m_inp[0]) << endl;
    cout << "sqrt(res1_mean^2 - res2_mean^2): " << sqrtf(m_res1[0] * m_res1[0] - m_res2[0] * m_res2[0]) << endl;


    waitKey(0);
}