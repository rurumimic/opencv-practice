//
//  main.cpp
//  histogram
//
//  Created by Dohyun Kim on 28/06/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void display(Mat src, Mat hist) {
    imshow("src", src);
    imshow("hist", hist);
    waitKey();
    
    destroyAllWindows();
}

Mat calcGrayHist(const Mat& img) {
    CV_Assert(img.type() == CV_8UC1);
    
    Mat hist;
    int channels[] = { 0 }; // 그레이스케일 채널 수 = 1, 0번 채널
    int dims = 1; // 채널 1개
    const int histSize[] = { 256 }; // bin 수
    float graylevel[] = { 0, 256 }; // 그레이스케일 값 범위
    const float* ranges[] = { graylevel }; // graylevel 배열 갖는 배열
    
    calcHist(&img, 1, channels, noArray(), hist, dims, histSize, ranges);
    
    return hist;
}

Mat getGrayHistImage(const Mat& hist) {
    // 히스토그램 검사
    CV_Assert(hist.type() == CV_32FC1);
    CV_Assert(hist.size() == Size(1, 256));
    
    double histMax; // hist 행렬 원소 최댓값
    minMaxLoc(hist, 0, &histMax); // 히스토그램, 최솟값 생략 0, 최댓값 저장 변수
    
    Mat imgHist(100, 256, CV_8UC1, Scalar(255)); // 흰색 256 x 100 영상 생성
    
    // 그래프 생성
    for (int i = 0; i < 256; i++) {
        line(imgHist, // 히스토그램
             Point(i, 100), // 그래프 최대 길이 100
             Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)), // 값 비율
             Scalar(0));
    }
    
    return imgHist;
}

void histogram_stretching(Mat src) {
    double gmin, gmax; // 최소, 최대
    minMaxLoc(src, &gmin, &gmax);
    
    Mat dst = (src - gmin) * 255 / (gmax - gmin); // 히스토그램 스트레칭
    
    display(src, getGrayHistImage(calcGrayHist(src)));
    display(dst, getGrayHistImage(calcGrayHist(dst)));
}

void histogram_equalization(Mat src) {
    Mat dst;
    equalizeHist(src, dst);
    
    display(src, getGrayHistImage(calcGrayHist(src)));
    display(dst, getGrayHistImage(calcGrayHist(dst)));
}

int main(int argc, const char * argv[]) {
    
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    Mat src2 = imread("hawkes.bmp", IMREAD_GRAYSCALE);
    Mat hist = calcGrayHist(src);
    Mat hist_img = getGrayHistImage(hist);
    
    
//    display(src, hist_img);
//    histogram_stretching(src);
    histogram_equalization(src2);
    
    return 0;
}
