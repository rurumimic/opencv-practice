//
//  main.cpp
//  svm
//
//  Created by Dohyun Kim on 01/08/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::ml;
using namespace std;

/* Dist */

void svm_basic() {
    // 8x2 크기 행렬
    Mat train = Mat_<float>({ 8, 2 }, {
        150, 200, 200, 250, 100, 250, 150, 300,
        350, 100, 400, 200, 400, 300, 350, 400 });
    // 8x1 크기 행렬
    Mat label = Mat_<int>({ 8, 1 }, { 0, 0, 0, 0, 1, 1, 1, 1 });

    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->trainAuto(train, ROW_SAMPLE, label); // 자동 학습
    
    Mat img = Mat::zeros(Size(500, 500), CV_8UC3); // 분류 결과 영상
    
    // 모든 픽셀 좌표를 빨간색 또는 녹색으로 표현
    for (int j = 0; j < img.rows; j++) {
        for (int i = 0; i < img.cols; i++) {
            Mat test = Mat_<float>({ 1, 2 }, { (float)i, (float)j });
            int res = cvRound(svm->predict(test));

            if (res == 0)
                img.at<Vec3b>(j, i) = Vec3b(128, 128, 255); // R
            else
                img.at<Vec3b>(j, i) = Vec3b(128, 255, 128); // G
        }
    }

    // 훈련 데이터 점을 반지름 5인 원으로 표시
    for (int i = 0; i < train.rows; i++) {
        int x = cvRound(train.at<float>(i, 0));
        int y = cvRound(train.at<float>(i, 1));
        int l = label.at<int>(i, 0);

        if (l == 0)
            circle(img, Point(x, y), 5, Scalar(0, 0, 128), -1, LINE_AA); // R
        else
            circle(img, Point(x, y), 5, Scalar(0, 128, 0), -1, LINE_AA); // G
    }

    imshow("svm", img);

    waitKey();
}

/* Digits */

Ptr<SVM> train_hog_svm(const HOGDescriptor& hog)
{
    Mat digits = imread("digits.png", IMREAD_GRAYSCALE);

    if (digits.empty()) {
        cerr << "Image load failed!" << endl;
        return 0;
    }

    Mat train_hog, train_labels;
    
    // 숫자마다 HOG 특징 벡터 추출
    for (int j = 0; j < 50; j++) {
        for (int i = 0; i < 100; i++) {
            Mat roi = digits(Rect(i * 20, j * 20, 20, 20));

            vector<float> desc;
            hog.compute(roi, desc);

            Mat desc_mat(desc);
            train_hog.push_back(desc_mat.t());
            train_labels.push_back(j / 5);
        }
    }

    // 모델 생성
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::Types::C_SVC);
    svm->setKernel(SVM::KernelTypes::RBF);
    svm->setC(2.5);
    svm->setGamma(0.50625);
    svm->train(train_hog, ROW_SAMPLE, train_labels);

    return svm;
}

Point ptPrev(-1, -1);

void on_mouse(int event, int x, int y, int flags, void* userdata)
{
    Mat img = *(Mat*)userdata;

    if (event == EVENT_LBUTTONDOWN)
        ptPrev = Point(x, y);
    else if (event == EVENT_LBUTTONUP)
        ptPrev = Point(-1, -1);
    else if (event == EVENT_MOUSEMOVE && (flags & EVENT_FLAG_LBUTTON))
    {
        line(img, ptPrev, Point(x, y), Scalar::all(255), 40, LINE_AA, 0);
        ptPrev = Point(x, y);

        imshow("img", img);
    }
}


void svm_digits() {
    HOGDescriptor hog(Size(20, 20), Size(10, 10), Size(5, 5), Size(5, 5), 9);
    
    // 모델 생성
    Ptr<SVM> svm = train_hog_svm(hog);
    
    Mat img = Mat::zeros(400, 400, CV_8U);

    imshow("img", img);
    setMouseCallback("img", on_mouse, (void*)&img);
    
    while (true) {
        int c = waitKey();

        if (c == 27) {
            break;
        } else if (c == ' ') {
            Mat img_resize;
            resize(img, img_resize, Size(20, 20), 0, 0, INTER_AREA);

            vector<float> desc;
            hog.compute(img_resize, desc); // HOG 특징 벡터 추출

            Mat desc_mat(desc);
            int res = cvRound(svm->predict(desc_mat.t())); // 예측
            cout << res << endl;

            img.setTo(0);
            imshow("img", img);
        }
    }
}

int main(int argc, const char * argv[]) {
    
//    svm_basic();
    svm_digits();
    
    return 0;
}
