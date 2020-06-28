//
//  main.cpp
//  contrast
//
//  Created by Dohyun Kim on 28/06/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 트랙바 위치, 사용자 데이터 포인터 값
void on_contrast(int pos, void* userdata) {
    float alpha = 0.05f * pos - 0.999f; // alpha > -1
    
    Mat src = *(Mat*)userdata;
    Mat dst = src + (src - 128) * alpha;
    
    imshow("dst", dst);
}

void display(Mat src, Mat dst) {
    imshow("src", src);
    imshow("dst", dst);
    waitKey();
    
    destroyAllWindows();
}

void contrast1(Mat src) {
    float s = 2.f;
    Mat dst = s * src;
    
    display(src, dst);
}

void contrast2(Mat src) {
    float alpha = 1.f;
    Mat dst = src + (src - 128) * alpha;
    
    display(src, dst);
}

void contrast3(Mat src) {
    Mat dst(src.rows, src.cols, src.type());
    
    float alpha = 1.f;
    float average = mean(src)[0]; // 밝기 평균: [124.049, 0, 0, 0]
    
    for (int j = 0; j < src.rows; j++) {
        for (int i = 0; i < src.cols; i++) {
            int v = src.at<uchar>(j, i) + (src.at<uchar>(j, i) - average) * alpha; // 명암 조절
            dst.at<uchar>(j, i) = saturate_cast<uchar>(v); // 포화 연산
        }
    }

    display(src, dst);
}

void contrast4(Mat src) {
    namedWindow("dst"); // 창 생성

    // 트랙바 이름, 생성 창 이름
    // 트랙바 위치를 받을 정수형 변수의 주소, 트랙바 최대 위치
    // 콜백 함수, 콜백 함수 파라미터
    // 반환: 정상 1, 실패 0
    createTrackbar("Contrast", "dst", 0, 100, on_contrast, (void*)&src);

    // 창 실행
    on_contrast(0, (void*)&src);

    waitKey();
    destroyAllWindows();
}

int main(int argc, const char * argv[]) {
    
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return 0;
    }
    
//    contrast1(src);
//    contrast2(src);
//    contrast3(src);
    contrast4(src);
    
    return 0;
}
