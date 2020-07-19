//
//  main.cpp
//  binarization
//
//  Created by Dohyun Kim on 19/07/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void on_threshold(int pos, void* userdata) {
    Mat src = *(Mat*)userdata;
    
    Mat dst;
    threshold(src, dst, pos, 255, THRESH_BINARY);
    
    imshow("dst", dst);
}

void binarization_1(Mat src) {
    imshow("src", src);
    
    namedWindow("dst");
    createTrackbar("Threshold", "dst", 0, 255, on_threshold, (void*)&src);
    setTrackbarPos("Threshold", "dst", 128); // 트랙바 시작 위치
    
    on_threshold(128, (void*)&src);
    
    waitKey(0);
    destroyAllWindows();
}

void binarization_2(Mat src) {
    
    Mat dst = src > 128;
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey(0);
    destroyAllWindows();
}

void on_adaptive(int pos, void* userdata) {
    Mat src = *(Mat*)userdata;
    
    int bsize = pos;
    if (bsize % 2 == 0) bsize--;
    if (bsize < 3) bsize = 3;
    
    Mat dst;
    adaptiveThreshold(src, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, bsize, 5);
    
    imshow("dst", dst);
}

void adaptive(Mat src) {
    imshow("src", src);
    
    namedWindow("dst");
    createTrackbar("Block Size", "dst", 0, 200, on_threshold, (void*)&src);
    setTrackbarPos("Block Size", "dst", 11); // 트랙바 시작 위치
    
    on_threshold(11, (void*)&src);
    
    waitKey(0);
    destroyAllWindows();
}

int main(int argc, const char * argv[]) {
    
    Mat src = imread("coins.png", IMREAD_GRAYSCALE);

    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    
//    binarization_1(src);
//    binarization_2(src);
    adaptive(src);
    
    return 0;
}
