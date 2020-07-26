//
//  main.cpp
//  object
//
//  Created by Dohyun Kim on 26/07/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void template_matching() {
    Mat src = imread("lenna.bmp", IMREAD_COLOR);
    Mat templ = imread("lenna.eye.png", IMREAD_COLOR);

    if (src.empty() || templ.empty()) {
        cerr << "Image load failed!" << endl;
        return;
    }
    
    src = src + Scalar(50, 50, 50); // 밝기 50 증가
    
    // 표준편차 10 가우시안 잡음 추가
    Mat noise(src.size(), CV_32SC3); // 영상 크기 지정
    randn(noise, 0, 10); // 잡음 생성
    add(src, noise, src, Mat(), CV_8UC3); // 잡음 추가
    
    Mat res, res_norm;
    matchTemplate(src, templ, res, TM_CCOEFF_NORMED); // 템플릿 매칭
    normalize(res, res_norm, 0, 255, NORM_MINMAX, CV_8U); // 결과값 0 ~ 255 정규화
    
    double maxv;  // 최댓값
    Point maxloc; // 최댓값 위치
    minMaxLoc(res, 0, &maxv, 0, &maxloc);
    cout << "maxv: " << maxv << endl;
    
    // 매칭 위치 표시
    rectangle(src, Rect(maxloc.x, maxloc.y, templ.cols, templ.rows), Scalar(0, 0, 255), 2);
    
    imshow("templ", templ);
    imshow("res_norm", res_norm);
    imshow("src", src);
    
    waitKey(0);
    destroyAllWindows();
}

void detect_face() {
    Mat src = imread("lenna.bmp", IMREAD_COLOR);
    
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return;
    }
    
    CascadeClassifier face_classifier("haarcascade_frontalface_default.xml");
    CascadeClassifier eye_classifier("haarcascade_eye.xml");
    
    if (face_classifier.empty() || eye_classifier.empty()) {
        cerr << "XML load failed!" << endl;
        return;
    }
    
    vector<Rect> faces;
    face_classifier.detectMultiScale(src, faces); // 얼굴 검출
    
    for (Rect face: faces) { // 검출된 얼굴에 대하여
        rectangle(src, face, Scalar(255, 0, 255), 2);
        
        Mat faceROI = src(face); // 얼굴 부분 영상 추출. Region of Interest 관심 영역
        vector<Rect> eyes;
        eye_classifier.detectMultiScale(faceROI, eyes); // 눈 검출
        
        for (Rect eye: eyes) {
            Point center(eye.x + eye.width / 2, eye.y + eye.height / 2);
            circle(faceROI, center, eye.width / 2, Scalar(255, 0, 0), 2, LINE_AA);
        }
    }
    
    imshow("src", src);
    
    waitKey(0);
    destroyAllWindows();
}

void detect_people() {
    VideoCapture cap("vtest.avi");
    
    if (!cap.isOpened()) {
        cerr << "Video open failed" << endl;
        return;
    }
    
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    
    Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        
        vector<Rect> detected;
        hog.detectMultiScale(frame, detected);
        
        for (Rect r: detected) {
            Scalar c = Scalar(rand() % 256, rand() % 256, rand() % 256);
            rectangle(frame, r, c, 3);
        }
        
        imshow("frame", frame);
        
        if (waitKey(10) == 27) break;
    }
}

void decode_qrcode() {
    VideoCapture cap(0);
    
    if (!cap.isOpened()) {
        cerr << "Camera open failed" << endl;
        return;
    }
    
    QRCodeDetector detector;
    
    Mat frame;
    while (true) {
        cap >> frame;
        
        if (frame.empty()) {
            cerr << "Frame load failed" << endl;
            return;
        }
        
        vector<Point> points;
        String info = detector.detectAndDecode(frame, points);
        
        if (!info.empty()) {
            polylines(frame, points, true, Scalar(0, 0, 255), 2);
            putText(frame, info, Point(10, 30), FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255));
        }
        
        imshow("frame", frame);
        if (waitKey(100) == 27) break;
    }
}

int main(int argc, const char * argv[]) {
    
    template_matching();
//    detect_face();
//    detect_people();
//    decode_qrcode();
    
    return 0;
}
