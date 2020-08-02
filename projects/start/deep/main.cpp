//
//  main.cpp
//  deep
//
//  Created by Dohyun Kim on 02/08/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::dnn;
using namespace std;

/* mnist */

Point ptPrev(-1, -1);

void on_mouse(int event, int x, int y, int flags, void* userdata)
{
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

void mnist() {
    Net net = readNet("mnist_cnn.pb");

    if (net.empty()) {
        cerr << "Network load failed!" << endl;
        return;
    }

    Mat img = Mat::zeros(400, 400, CV_8UC1);

    imshow("img", img);
    setMouseCallback("img", on_mouse, (void*)&img);

    while (true) {
        int c = waitKey();

        if (c == 27) {
            break;
        } else if (c == ' ') {
            // 28x28 크기 행렬. 0 ~ 1 실수값
            // 크기: 1x1x28x28
            Mat blob = blobFromImage(img, 1/255.f, Size(28, 28));
            net.setInput(blob);
            
            // 크기: 10x1
            Mat prob = net.forward();

            double maxVal;
            Point maxLoc;
            minMaxLoc(prob, NULL, &maxVal, NULL, &maxLoc);
            int digit = maxLoc.x;

            cout << digit << " (" << maxVal * 100 << "%)" <<endl;

            img.setTo(0);
            imshow("img", img);
        }
    }
}

/* Classify */

void classify() {
//    Mat img = imread("space_shuttle.jpg", IMREAD_COLOR);
//    Mat img = imread("cup.jpg", IMREAD_COLOR);
//    Mat img = imread("beagle.jpg", IMREAD_COLOR);
//    Mat img = imread("pineapple.jpg", IMREAD_COLOR);
    Mat img = imread("scooter.jpg", IMREAD_COLOR);

    if (img.empty()) {
        cerr << "Image load failed!" << endl;
        return;
    }

    // 네트워크 불러오기

    Net net = readNet("bvlc_googlenet.caffemodel", "deploy.prototxt");

    if (net.empty()) {
        cerr << "Network load failed!" << endl;
        return;
    }

    // 클래스 이름 불러오기

    ifstream fp("classification_classes_ILSVRC2012.txt");

    if (!fp.is_open()) {
        cerr << "Class file load failed!" << endl;
        return;
    }

    vector<String> classNames;
    string name;
    while (!fp.eof()) {
        getline(fp, name);
        if (name.length())
            classNames.push_back(name);
    }

    fp.close();

    // 영상을 모델에 입력

    Mat inputBlob = blobFromImage(img, 1, Size(224, 224), Scalar(104, 117, 123));
    net.setInput(inputBlob, "data");
    Mat prob = net.forward();

    // 최댓값 위치 찾고 결과 출력

    double maxVal;
    Point maxLoc;
    minMaxLoc(prob, NULL, &maxVal, NULL, &maxLoc);

    String str = format("%s (%4.2lf%%)", classNames[maxLoc.x].c_str(), maxVal * 100);
    putText(img, str, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255));
    imshow("img", img);

    waitKey();
}

/* Face */

const String model = "res10_300x300_ssd_iter_140000_fp16.caffemodel";
const String config = "deploy.2.prototxt";
//const String model = "opencv_face_detector_uint8.pb";
//const String config = "opencv_face_detector.pbtxt";

void face() {
    VideoCapture cap(0);

    if (!cap.isOpened()) {
        cerr << "Camera open failed!" << endl;
        return;
    }

    Net net = readNet(model, config);

    if (net.empty()) {
        cerr << "Net open failed!" << endl;
        return;
    }

    Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty())
            break;

        // 1x3x300x300 크기
        Mat blob = blobFromImage(frame, 1, Size(300, 300), Scalar(104, 177, 123));
        net.setInput(blob);
        Mat res = net.forward(); // 1x1xNx7 크기. N: 얼굴 개수. 최대 200개

        // 3, 4차원으로 새로운 2차원 행렬 구성
        Mat detect(res.size[2], res.size[3], CV_32FC1, res.ptr<float>());

        for (int i = 0; i < detect.rows; i++) {
            // 0번 열: 0
            // 1번 열: 1
            // 2번 열: 얼굴 신뢰도 confidence
            float confidence = detect.at<float>(i, 2); // Nx7 크기
            if (confidence < 0.5)
                break;

            // 3 ~ 6번 열: 얼굴 영역 사각형 좌표
            int x1 = cvRound(detect.at<float>(i, 3) * frame.cols);
            int y1 = cvRound(detect.at<float>(i, 4) * frame.rows);
            int x2 = cvRound(detect.at<float>(i, 5) * frame.cols);
            int y2 = cvRound(detect.at<float>(i, 6) * frame.rows);

            rectangle(frame, Rect(Point(x1, y1), Point(x2, y2)), Scalar(0, 255, 0));

            String label = format("Face: %4.3f", confidence);
            putText(frame, label, Point(x1, y1 - 1), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 255, 0));
        }

        imshow("frame", frame);
        if (waitKey(1) == 27)
            break;
    }
}

int main(int argc, const char * argv[]) {
    
//    mnist();
//    classify();
    face();
    
    return 0;
}
