//
//  main.cpp
//  brightness
//
//  Created by Dohyun Kim on 28/06/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 트랙바 위치, 사용자 데이터 포인터 값
void on_brightness(int pos, void* userdata) {
    Mat src = *(Mat*)userdata;
    Mat dst = src + pos;
    
    imshow("dst", dst);
}

void display(Mat src, Mat dst) {
    imshow("src", src);
    imshow("dst", dst);
    waitKey();
    
    destroyAllWindows();
}

void brightness1(Mat src) {
    // Mat dst = src + 100; // 밝기 조절 & 포화 연산
    
    Mat dst;
    subtract(src, 100, dst); // 함수 사용
    
    display(src, dst);
}

void brightness2(Mat src) {
    Mat dst(src.rows, src.cols, src.type());
    
    for (int j = 0; j < src.rows; j++) {
        for (int i = 0; i < src.cols; i++) {
            int v = src.at<uchar>(j, i) + 100; // 밝기 조절
            dst.at<uchar>(j, i) = saturate_cast<uchar>(v); // 포화 연산
        }
    }
    
    display(src, dst);
}

void brightness3(Mat src) {
    namedWindow("dst"); // 창 생성
    
    // 트랙바 이름, 생성 창 이름
    // 트랙바 위치를 받을 정수형 변수의 주소, 트랙바 최대 위치
    // 콜백 함수, 콜백 함수 파라미터
    // 반환: 정상 1, 실패 0
    createTrackbar("Brightness", "dst", 0, 100, on_brightness, (void*)&src);
    
    // 창 실행
    on_brightness(0, (void*)&src);
    
    waitKey();
    destroyAllWindows();
}

int main(int argc, const char * argv[]) {
    
    // Mat origin = imread("lenna.bmp", IMREAD_COLOR);
    Mat grayscale = imread("lenna.bmp", IMREAD_GRAYSCALE);
    
    // 컬러 → 그레이스케일
    // Mat color2Gray;
    // cvtColor(origin, color2Gray, COLOR_BGR2GRAY);
    
    // 런타임에서 그레이스케일 생성
    // Mat emptyGrayScale(480, 640, CV_8UC1, Scalar(0));
    
    if (grayscale.empty()) {
        cerr << "Image load failed!" << endl;
        return 0;
    }
    
//    brightness1(grayscale);
//    brightness2(grayscale);
    brightness3(grayscale);
    
    return 0;
}
