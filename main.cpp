#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void main(){
	Mat img, hsv, mask, blur;
	VideoCapture cap(0);

	//selektor warna kuning
	int h_low = 0, s_low = 127, v_low = 140;
	int h_hi = 26, s_hi = 255, v_hi = 255;

	//slider untuk mengatur range nilai HSV
	namedWindow("slider");
	createTrackbar("hue low : ", "slider", &h_low, 255);
	createTrackbar("sat low : ", "slider", &s_low, 255);
	createTrackbar("val low : ", "slider", &v_low, 255);
	createTrackbar("hue hi : ", "slider", &h_hi, 255);
	createTrackbar("sat hi : ", "slider", &s_hi, 255);
	createTrackbar("val hi : ", "slider", &v_hi, 255);

	while(true){
		Scalar low(h_low, s_low, v_low);
		Scalar hi(h_hi, s_hi, v_hi);
		cap.read(img);
		cvtColor(img, hsv, COLOR_BGR2HSV);
		inRange(hsv, low, hi, mask);
		Canny(mask, mask, 30, 50, 3);
		GaussianBlur(mask, blur, Size(11, 11), 0);
		
		vector<Vec3f> circles;
		HoughCircles(blur, circles, HOUGH_GRADIENT, 1.25 , 100, 100, 30, 1, 500);

		for (size_t i = 0; i < circles.size(); i++) {
			Vec3i c = circles[i];
			Point cntr = Point(c[0], c[1]);
			int r = c[2];
			circle(img, cntr, 5, Scalar(0, 0, 255), 10, LINE_8);
			circle(img, cntr, r, Scalar(0, 255, 0), 3, LINE_8);
			cout << "posisi : (" << c[0] << " ," << c[1] << " ) dan radius bola : " << c[2] << endl;
		}
		
		imshow("image ori", img);
		imshow("image edge", mask);
		waitKey(1);
	}
}
