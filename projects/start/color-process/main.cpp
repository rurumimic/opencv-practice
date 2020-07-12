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
    int channels[] = { 1, 2 }; // 히스토그램을 구할 채널을 나타내는 정수형 배열. Cr Cb
    int cr_bins = 128;
    int cb_bins = 128;
    int histSize[] = { cr_bins, cb_bins }; // 각 차원의 히스토그램 배열 크기를 나타내는 배열. 각 차원의 히스토그램 bin 개수를 나타내는 배열
    float cr_range[] = { 0, 256 };
    float cb_range[] = { 0, 256 };
    const float* ranges[] = { cr_range, cb_range }; // 각 차원의 히스토그램 범위
    
    calcHist(&ref_ycrcb, // images: 입력 영상의 배열
             1,          // nimages: 입력 영상 개수 1개
             channels,   // channels: 히스토그램을 구할 채널을 나타내는 정수형 배열. Cr Cb
             mask,       // mask: 마스크 영상
             hist,       // hist: 출력 히스토그램
             2,          // dims 2: 출력 히스토그램 차원 수
             histSize,   // 각 차원의 히스토그램 bin 개수를 나타내는 배열
             ranges);    // 각 차원의 히스토그램 범위
    
    // 입력 영상 YCrCb 색공간
    Mat src, src_ycrcb;
    src = imread("kids.png", IMREAD_COLOR);
    cvtColor(src, src_ycrcb, COLOR_BGR2YCrCb);
    
    // 히스토그램 역투영 결과 저장
    Mat backproj;
    calcBackProject(&src_ycrcb, // images: 입력 영상의 배열 또는 입력 영상의 주소
                    1,          // nimages: 입력 영상 개수
                    channels,   // channels: 역투영 계산 시 사용할 채널 번호 배열
                    hist,       // hist: 입력 히스토그램
                    backproj,   // backProject: 출력 히스토그램 역투영 영상. 1채널 영상
                    ranges,     // ranges: 각 차원의 히스토그램 빈 범위를 나타내는 배열의 배열
                    1,          // scale: 히스토그램 역투영 값에 추가적으로 곱할 값
                    true);      // uniform: 히스토그램 빈의 간격이 균등한지를 나타내는 플래그

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








































