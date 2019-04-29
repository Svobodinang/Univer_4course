#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

const double MAX_VAL = 100;
Mat image, gray, gray_blur, cant, closed;

string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

void getMoneySvertka(string path, int blur, int gr) 
{
	image = imread(path, CV_32FC3);

	cvtColor(image, gray, COLOR_BGR2GRAY); //Преобразуем в чб
	Mat res1;

	//Свертка
	Mat kernel(3, 3, CV_32FC1);
	kernel.at<float>(0, 0) = -1;  kernel.at<float>(0, 1) = 0;  kernel.at<float>(0, 2) = 1;
	kernel.at<float>(1, 0) = -2;   kernel.at<float>(1, 1) = 0;  kernel.at<float>(1, 2) = 2;
	kernel.at<float>(2, 0) = -1;   kernel.at<float>(2, 1) = 0;  kernel.at<float>(2, 2) = 1;
	//Применяем свертку
	filter2D(gray, res1, -1, kernel, Point(-1, -1), 0, BORDER_REPLICATE);

	threshold(res1, res1, gr, 255, CV_THRESH_BINARY);

	vector<vector<Point>> cont;
	findContours(res1, cont, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //Находим контуры
	int total = 0;

	vector<vector<Point> > res;
	cout << cont.size();
	//Цикл по контурам
	for (int i = 0; i < cont.size(); i++)
	{
		double peri = arcLength(cont[i], true); //Сглаживаем контур
		vector<Point> approx;
		approxPolyDP(cont[i], approx, 0.02 * peri, true);

		if (approx.size() == 4) //Если у контура 4 вершина, то это купюра
		{
			double s = fabs(approx[1].y - approx[0].y)*fabs(approx[3].x - approx[0].x);
			if (s > 100)
			{
				cout << endl;
				res.push_back(approx);
				total += 1;
			}
		}
	}
	drawContours(image, res, -1, (0, 255, 0), 4); //Отрисовываем контуры денег на исхожном изображении

	imshow("good", res1);
	imshow("start", image);
	waitKey(0);
}

void getMoneySobel(String path, int blur, int gr)
{
	image = imread(path, CV_32FC3);

	Mat imageDX, imageDY;
	Mat imageX, imageY;
	//производные картинок по x, y
	Sobel(image, imageDX, CV_32F, 1, 0, 3, 1 / 255.0);
	Sobel(image, imageDY, CV_32F, 0, 1, 3, 1 / 255.0);

	//Преобразуем цветную в чб
	cvtColor(imageDX, imageX, CV_BGR2GRAY);
	cvtColor(imageDY, imageY, CV_BGR2GRAY);

	//каждый пиксель возводим в квадраты
	pow(imageX, 2, imageX);
	pow(imageY, 2, imageY);

	//суммируем пиксели по x, y
	Mat contImg = imageX + imageY;
	sqrt(contImg, contImg);
	Mat temp, binary, binary2;

	contImg.convertTo(temp, CV_8UC3, 255.0);

	threshold(temp, binary, gr, 255, CV_THRESH_BINARY);
	GaussianBlur(binary, binary, Size(blur, blur), 0); //Блюр, чтобы убрать шум

	vector<vector<Point>> cont;
	findContours(binary, cont, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //Находим контуры
	int total = 0;

	vector<vector<Point> > res;

	//Цикл по контурам
	for (int i = 0; i < cont.size(); i++)
	{
		double peri = arcLength(cont[i], true); //Сглаживаем контур
		vector<Point> approx;
		approxPolyDP(cont[i], approx, 0.02 * peri, true);

		if (approx.size() == 4) //Если у контура 4 вершина, то это купюра
		{
			double s = fabs(approx[1].y - approx[0].y)*fabs(approx[3].x - approx[0].x);
			if (s > 100)
			{
				cout << endl;
				res.push_back(approx);
				total += 1;
			}
		}
	}
	drawContours(image, res, -1, (0, 255, 0), 4); //Отрисовываем контуры денег на исхожном изображении

	imshow("t", binary);
	imshow("start", image);
	waitKey(0);
}


void getMoneyCant(string path, int blur, int close)
{

	image = imread(path, CV_32FC3);

	image = 1.3 * image + 10; //Делаем картинку светлее

	cvtColor(image, gray, COLOR_BGR2GRAY); //Преобразуем в чб
	GaussianBlur(gray, gray_blur, Size(blur, blur), 0); //Блюр, чтобы убрать шум
	Canny(gray_blur, cant, 10, 150); //Выделяем края

	Mat ker = getStructuringElement(MORPH_RECT, Point(close, close));
	morphologyEx(cant, closed, MORPH_CLOSE, ker); //Создаем и применяем закрытие,чтобы не было пустот

	vector<vector<Point>> cont;
	findContours(closed, cont, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //Находим контуры
	int total = 0;

	vector<vector<Point> > res;

	//Цикл по контурам
	for (int i = 0; i < cont.size(); i++)
	{
		double peri = arcLength(cont[i], true); //Сглаживаем контур
		vector<Point> approx;
		approxPolyDP(cont[i], approx, 0.02 * peri, true);

		if (approx.size() == 4) //Если у контура 4 вершина, то это купюра
		{
			double s = fabs(approx[1].y - approx[0].y)*fabs(approx[3].x - approx[0].x);
			if (s > 100)
			{
				cout << endl;
				res.push_back(approx);
				total += 1;
			}
		}
	}
	drawContours(image, res, -1, (0, 255, 0), 4); //Отрисовываем контуры денег на исхожном изображении

	imshow("good", closed);
	imshow("start", image);
	waitKey(0);
}


void getMoneyCantBlur(string path, int blur1, int blur2) {

	image = imread(path, CV_32FC3);

	image = 1.3 * image + 10; //Делаем картинку светлее

	cvtColor(image, gray, COLOR_BGR2GRAY); //Преобразуем в чб
	GaussianBlur(gray, gray_blur, Size(blur1, blur1), 0); //Блюр, чтобы убрать шум
	Canny(gray_blur, cant, 10, 150); //Выделяем края

	GaussianBlur(cant, cant, Size(blur2, blur2), 0); //Еще раз добавляем блюр, для того, чтобы сгладить края
	
	vector<vector<Point>> cont;
	findContours(cant, cont, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //Находим контуры
	int total = 0;

	vector<vector<Point> > res;

	//Цикл по контурам
	for (int i = 0; i < cont.size(); i++)
	{
		double peri = arcLength(cont[i], true); //Сглаживаем контур
		vector<Point> approx;
		approxPolyDP(cont[i], approx, 0.02 * peri, true);

		if (approx.size() == 4) //Если у контура 4 вершина, то это купюра
		{
			double s = fabs(approx[1].y - approx[0].y)*fabs(approx[3].x - approx[0].x);
			if (s > 100)
			{
				cout << endl;
				res.push_back(approx);
				total += 1;
			}
		}
	}
	drawContours(image, res, -1, (0, 255, 0), 4); //Отрисовываем контуры денег на исхожном изображении

	imshow("good", cant);
	imshow("start", image);
	waitKey(0);
}

int main() {

	//getMoneySvertka("../../data/money-1.jpg", 5, 10);

	//getMoneySobel("../../data/money-5.jpg", 3, 50);
	//getMoneySobel("../../data/money-2.jpg", 3, 70);
	//getMoneySobel("../../data/money-3.jpg", 5, 70);
	//getMoneySobel("../../data/money-3.jpg", 5, 80);
	//getMoneySobel("../../data/money-4.jpg", 3, 50);
	//getMoneySobel("../../data/money-5.jpg", 3, 50);

	//getMoneyCant("../../data/money-1.jpg", 5, 3);
	//getMoneyCant("../../data/money-2.jpg", 5, 2);
	//getMoneyCant("../../data/money-3.jpg", 3, 7);
	//getMoneyCant("../../data/money-4.jpg", 5, 7);
	//getMoneyCant("../../data/money-5.jpg", 3, 7);

	getMoneyCantBlur("../../data/money-1.jpg", 5, 3);
	getMoneyCantBlur("../../data/money-2.jpg", 5, 5);
	getMoneyCantBlur("../../data/money-3.jpg", 5, 3);
	getMoneyCantBlur("../../data/money-4.jpg", 5, 3);
	getMoneyCantBlur("../../data/money-5.jpg", 3, 5);


	return 0;
}