//
//  main.cpp
//  knn
//
//  Created by Dohyun Kim on 01/08/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::ml;
using namespace std;

/* KNN */

Mat img;
Mat train, label;
Ptr<KNearest> knn;
int k_value = 1;

void trainAndDisplay() {
    knn->train(train, ROW_SAMPLE, label); // 데이터 훈련

    // 분류 결과 표현
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; ++j) {
            Mat sample = (Mat_<float>(1, 2) << j, i);

            Mat res;
            knn->findNearest(sample, k_value, res); // kNN 알고리즘 실행

            int response = cvRound(res.at<float>(0, 0)); // 분류 값
            if (response == 0) {
                img.at<Vec3b>(i, j) = Vec3b(128, 128, 255); // R
            } else if (response == 1) {
                img.at<Vec3b>(i, j) = Vec3b(128, 255, 128); // G
            } else if (response == 2) {
                img.at<Vec3b>(i, j) = Vec3b(255, 128, 128); // B
            }
        }
    }

    // 훈련 데이터 및 레이블 표현
    for (int i = 0; i < train.rows; i++) {
        int x = cvRound(train.at<float>(i, 0));
        int y = cvRound(train.at<float>(i, 1));
        int l = label.at<int>(i, 0);

        if (l == 0) {
            circle(img, Point(x, y), 5, Scalar(0, 0, 128), -1, LINE_AA);
        } else if (l == 1) {
            circle(img, Point(x, y), 5, Scalar(0, 128, 0), -1, LINE_AA);
        } else if (l == 2) {
            circle(img, Point(x, y), 5, Scalar(128, 0, 0), -1, LINE_AA);
        }
    }

    imshow("knn", img);
}

void on_k_changed(int, void*) {
    if (k_value < 1) k_value = 1;
    trainAndDisplay();
}

void addPoint(const Point& pt, int cls) {
    Mat new_sample = (Mat_<float>(1, 2) << pt.x, pt.y); // 실수 값
    train.push_back(new_sample); // 훈련 데이터

    Mat new_label = (Mat_<int>(1, 1) << cls); // 정수 값
    label.push_back(new_label); // 클래스
}

void knnDist() {
    img = Mat::zeros(Size(500, 500), CV_8UC3);
    knn = KNearest::create();

    namedWindow("knn");
    createTrackbar("k", "knn", &k_value, 5, on_k_changed);

    const int NUM = 30;
    Mat rn(NUM, 2, CV_32SC1);

    // (150, 150) 좌표 중심: 0번 클래스. 30개 점 생성
    randn(rn, 0, 50); // 표준 편차 50 가우시안 분포
    for (int i = 0; i < NUM; i++) {
        addPoint(Point(rn.at<int>(i, 0) + 150, rn.at<int>(i, 1) + 150), 0);
    }

    // (350, 150) 좌표 중심: 1번 클래스. 30개 점 생성
    randn(rn, 0, 50); // 표준 편차 50 가우시안 분포
    for (int i = 0; i < NUM; i++) {
        addPoint(Point(rn.at<int>(i, 0) + 350, rn.at<int>(i, 1) + 150), 1);
    }

    // (250, 400) 좌표 중심: 2번 클래스. 30개 점 생성
    randn(rn, 0, 70); // 표준 편차 70 가우시안 분포
    for (int i = 0; i < NUM; i++) {
        addPoint(Point(rn.at<int>(i, 0) + 250, rn.at<int>(i, 1) + 400), 2);
    }

    trainAndDisplay();
    waitKey();
}

/* Digits */

Ptr<KNearest> train_knn() {
    Mat digits = imread("digits.png", IMREAD_GRAYSCALE);
    
    if (digits.empty()) {
        cerr << "Image load failed!" << endl;
        return 0;
    }
    
    Mat train_images, train_labels;
    for (int j = 0; j < 50; j++) { // 세로 50
        for (int i = 0; i < 100; i++) { // 가로 100
            Mat roi, roi_float, roi_flatten;
            roi = digits(Rect(i * 20, j * 20, 20, 20));
            roi.convertTo(roi_float, CV_32F); // 자료형을 실수형으로 변환
            roi_flatten = roi_float.reshape(1, 1); // 영상 크기 변환 400x1
            
            train_images.push_back(roi_flatten);
            train_labels.push_back(j / 5); // 숫자 500개 마다 레이블 설정
        }
    }
    
    Ptr<KNearest> knn = KNearest::create();
    knn->train(train_images, ROW_SAMPLE, train_labels);
    
    return knn;
}

Point ptPrev(-1, -1);

void on_mouse(int event, int x, int y, int flags, void* userdata) {
    Mat img = *(Mat*)userdata;
    
    if (event == EVENT_LBUTTONDOWN) {
        ptPrev = Point(x, y);
    } else if (event == EVENT_LBUTTONUP) {
        ptPrev = Point(-1, -1);
    } else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON)) {
        line(img, ptPrev, Point(x, y), Scalar::all(255), 40, LINE_AA, 0);
        ptPrev = Point(x, y);
        
        imshow("img", img);
    }
}

void knnDigits() {
    Ptr<KNearest> knn = train_knn();
    
    if (knn.empty()) {
        cerr << "Training failed!" << endl;
        return;
    }
    
    Mat img = Mat::zeros(400, 400, CV_8U);
    
    imshow("img", img);
    setMouseCallback("img", on_mouse, (void*)&img);
    
    while (true) {
        int c = waitKey();
        
        if (c == 27) { // ESC 프로그램 종료
            break;
        } else if (c == ' ') { // Space 필기체 숫자 인식
            Mat img_resize, img_float, img_flatten, res;
            
            // 숫자가 쓰여진 img 영상 20x20 크기로 변환
            resize(img, img_resize, Size(20, 20), 0, 0, INTER_AREA);
            // 실수형 자료형으로 변환
            img_resize.convertTo(img_float, CV_32F);
            // 400x1 크기 영상으로 변환
            img_flatten = img_float.reshape(1, 1);
            
            knn->findNearest(img_flatten, 3, res); // 알고리즘 분류
            cout << cvRound(res.at<float>(0, 0)) << endl;
            
            img.setTo(0);
            imshow("img", img);
        }
    }
}

int main(int argc, const char * argv[]) {

//    knnDist();
    knnDigits();

    return 0;
}
