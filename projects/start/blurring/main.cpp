//
//  main.cpp
//  blurring
//
//  Created by Dohyun Kim on 05/07/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void blur1(Mat src) {
    imshow("src", src);
    Mat blur33, blur99, blur9999;
    
    blur(src, blur33, Size(3, 3));
    String desc33 = format("Mean: %dx%d", 3, 3);
    putText(blur33, desc33, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
    imshow("blur 33", blur33);
    
    blur(src, blur99, Size(9, 9));
    String desc99 = format("Mean: %dx%d", 9, 9);
    putText(blur99, desc99, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
    imshow("blur 99", blur99);
    
    blur(src, blur9999, Size(99, 99));
    String desc9999 = format("Mean: %dx%d", 99, 99);
    putText(blur9999, desc9999, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
    imshow("blur 9999", blur9999);
    
    waitKey(0);
    destroyAllWindows();
}

void blur2(Mat src) {
    imshow("src", src);

    Mat blur1, blur3, blur5;
    int sigma1 = 1;
    int sigma3 = 3;
    int sigma5 = 5;
    
    GaussianBlur(src, blur1, Size(), (double)sigma1);
    String desc1 = format("sigma: %d", sigma1);
    putText(blur1, desc1, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
    imshow("blur 1", blur1);
    
    GaussianBlur(src, blur3, Size(), (double)sigma3);
    String desc3 = format("sigma: %d", sigma3);
    putText(blur3, desc3, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
    imshow("blur 3", blur3);
    
    GaussianBlur(src, blur5, Size(), (double)sigma5);
    String desc5 = format("sigma: %d", sigma5);
    putText(blur5, desc5, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
    imshow("blur 5", blur5);
    
    waitKey(0);
    destroyAllWindows();
}

void sharp(Mat src) {
    imshow("src", src);

    Mat sharp1, sharp3, sharp5;
    Mat blur1, blur3, blur5;
    int sigma1 = 1;
    int sigma3 = 3;
    int sigma5 = 5;
    float alpha = 1.f;
    
    GaussianBlur(src, blur1, Size(), (double)sigma1);
    sharp1 = (1 + alpha) * src - alpha * blur1;
    String desc1 = format("sigma: %d", sigma1);
    putText(sharp1, desc1, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
    imshow("sharp 1", sharp1);
    
    GaussianBlur(src, blur3, Size(), (double)sigma3);
    sharp3 = (1 + alpha) * src - alpha * blur3;
    String desc3 = format("sigma: %d", sigma3);
    putText(sharp3, desc3, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
    imshow("sharp 3", sharp3);
    
    GaussianBlur(src, blur5, Size(), (double)sigma5);
    sharp5 = (1 + alpha) * src - alpha * blur5;
    String desc5 = format("sigma: %d", sigma5);
    putText(sharp5, desc5, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
    imshow("sharp 5", sharp5);
    
    waitKey(0);
    destroyAllWindows();
}

void noisy(Mat src) {
    imshow("src", src);
    
    int stddev = 30;
    Mat noise(src.size(), CV_32SC1);
    randn(noise, 0, stddev);
    
    Mat dst;
    add(src, noise, dst, Mat(), CV_8U);
    
    String desc = format("stddev: %d", stddev);
    putText(dst, desc, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255), 1, LINE_AA);
    imshow("noise", dst);
    
    waitKey(0);
    destroyAllWindows();
}

void bilateral(Mat src) {
    Mat noise(src.size(), CV_32SC1);
    randn(noise, 0, 5);
    add(src, noise, src, Mat(), CV_8U);
    
    Mat dst;
    bilateralFilter(src, dst, -1, 10, 5);
    
    imshow("src", src);
    imshow("dst", dst);
    waitKey();
    destroyAllWindows();
}

void median(Mat src) {
    int num = (int)(src.total() * 0.1);
    for (int i = 0; i < num; i++) {
        int x = rand() % src.cols;
        int y = rand() % src.rows;
        src.at<uchar>(y, x) = (i % 2) * 255;
    }
    
    Mat dst;
    medianBlur(src, dst, 3);
    
    imshow("src", src);
    imshow("dst", dst);
    waitKey();
    destroyAllWindows();
}

int main(int argc, const char * argv[]) {
    
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return 0;
    }
    
//    blur1(src);
//    blur2(src);
//    sharp(src);
//    noisy(src);
    bilateral(src);
//    median(src);
    
    return 0;
}
