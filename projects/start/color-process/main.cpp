//
//  main.cpp
//  color-process
//
//  Created by Dohyun Kim on 12/07/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int lower_hue = 40, upper_hue = 80; // 트랙바 위치
Mat src, src_hsv, mask;

void on_hue_changed(int, void*) {
    // 색상, 채도 100 ~ 255, 명도 0 ~ 255
    Scalar lowerb(lower_hue, 100, 0);
    Scalar upperb(upper_hue, 255, 255);
    inRange(src_hsv, lowerb, upperb, mask); // 마스크 영상 생성
    
    imshow("mask", mask);
}

void isinrange(Mat src) {
    cvtColor(src, src_hsv, COLOR_BGR2HSV); // 색 공간 변환
    imshow("src", src);
    
    namedWindow("mask");
    
    // 트랙 바 생성
    createTrackbar("Lower Hue", "mask", &lower_hue, 179, on_hue_changed);
    createTrackbar("Upper Hue", "mask", &upper_hue, 179, on_hue_changed);
    
    // 트랙바 강제 호출
    on_hue_changed(0, 0);
    
    waitKey();
}

void hist(Mat src) {
    
    Mat src_ycrcb;
    cvtColor(src, src_ycrcb, COLOR_BGR2YCrCb);
    
    vector<Mat> ycrcb_planes;
    split(src_ycrcb, ycrcb_planes);
    
    equalizeHist(ycrcb_planes[0], ycrcb_planes[0]); // Y channel
    
    Mat dst_ycrcb;
    merge(ycrcb_planes, dst_ycrcb);
    
    Mat dst;
    cvtColor(dst_ycrcb, dst, COLOR_YCrCb2BGR);
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey();
    destroyAllWindows();
}

void backprojection() {
    Mat ref_ycrcb;
    Mat ref = imread("ref.png", IMREAD_COLOR); // 기준 영상
    Mat mask = imread("mask.bmp", IMREAD_GRAYSCALE); // 피부색 위치 마스크
    cvtColor(ref, ref_ycrcb, COLOR_BGR2YCrCb); // 색 공간 변경
    
    // 피부색 영역을 CrCb 2차원 히스토그램으로 표현
    Mat hist;
    int channels[] = { 1, 2 };
    int cr_bins = 128;
    int cb_bins = 128;
    int histSize[] = { cr_bins, cb_bins };
    float cr_range[] = { 0, 256 };
    float cb_range[] = { 0, 256 };
    const float* ranges[] = { cr_range, cb_range };
    calcHist(&ref_ycrcb, 1, channels, mask, hist, 2, histSize, ranges);
    
    // 입력 영상 YCrCb 색공간
    Mat src, src_ycrcb;
    src = imread("kids.png", IMREAD_COLOR);
    cvtColor(src, src_ycrcb, COLOR_BGR2YCrCb);
    
    // 히스토그램 역투영 결과 저장
    Mat backproj;
    calcBackProject(&src_ycrcb, 1, channels, hist, backproj, ranges, 1, true);
    
    imshow("src", src);
    imshow("backproj", backproj);
    
    waitKey();
}

int main(int argc, const char * argv[]) {
    
    Mat src = imread("lenna.bmp", IMREAD_COLOR);

    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    
//    hist(src);
//    isinrange(src);
    backprojection();
    
    return 0;
}








































