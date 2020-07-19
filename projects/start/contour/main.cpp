//
//  main.cpp
//  contour
//
//  Created by Dohyun Kim on 19/07/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void contours_basic(Mat src) {
    Mat bin;
    threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);
    
    vector<vector<Point>> contours;
    findContours(bin, contours, RETR_LIST, CHAIN_APPROX_NONE);
    
    Mat dst;
    cvtColor(src, dst, COLOR_GRAY2BGR);
    
    for (int i = 0; i < contours.size(); i++) {
        Scalar c(rand() & 255, rand() & 255, rand() & 255);
        drawContours(dst, contours, i, c, 2);
    }
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey();
    destroyAllWindows();
}

void contours_hier(Mat src) {
    Mat bin;
    threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);
    
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(bin, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
    
    Mat dst;
    cvtColor(src, dst, COLOR_GRAY2BGR);
    
    for (int i = 0; i >= 0; i = hierarchy[i][0]) {
        Scalar c(rand() & 255, rand() & 255, rand() & 255);
        drawContours(dst, contours, i, c, -1, LINE_8, hierarchy);
    }
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey();
    destroyAllWindows();
}

// img 영상에서 pts 외곽선 주변에 바운딩 박스를 그리고 label 문자열 출력
void setLabel(Mat& img, const vector<Point>& pts, const String& label) {
    Rect rc = boundingRect(pts); // 외곽선을 감싸는 바운딩 박스
    rectangle(img, rc, Scalar(0, 0, 255), 1); // 주황색 표시
    putText(img, label, rc.tl(), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255)); // label 문자열 출력
}

void labeling(Mat src) {
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    
    Mat bin;
    threshold(gray, bin, 200, 255, THRESH_BINARY_INV | THRESH_OTSU);
    
    vector<vector<Point>> contours; // 외곽선 검출
    findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    
    for (vector<Point>& pts: contours) {
//        if (contourArea(pts) < 100) continue; // 면적이 400보다 작으면 잡음으로 무시한다
        
        vector<Point> approx; // 외곽선 근사화
        approxPolyDP(pts, approx, arcLength(pts, true) * 0.02, true);
        
        int vtc = (int) approx.size(); // 외곽선 점 개수 저장
        
        if (vtc == 3) {
            setLabel(src, pts, "TRI");
        } else if (vtc == 4) {
            setLabel(src, pts, "RECT");
        } else if (vtc > 4) {
            // 면적 대 길이 비율 조사하여 원에 가까운지 검사
            double len = arcLength(pts, true);
            double area = contourArea(pts);
            double ratio = 4. * CV_PI * area / (len * len);
            
            if (ratio > 0.8) {
                setLabel(src, pts, "CIR");
            }
        }
    }
    
    imshow("src", src);
    waitKey();
    destroyAllWindows();
}

int main(int argc, const char * argv[]) {
    Mat src = imread("card.bmp", IMREAD_GRAYSCALE);

    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    
//    contours_basic(src);
//    contours_hier(src);
    
    Mat img = imread("polygon.bmp", IMREAD_COLOR);
    if (img.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    
    labeling(img);
    
    return 0;
}
