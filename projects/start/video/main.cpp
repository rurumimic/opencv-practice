//
//  main.cpp
//  video
//
//  Created by Dohyun Kim on 21/06/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void cameraIn();
void videoIn();
void cameraInVideoOut();

int main(int argc, const char * argv[]) {
    
    cameraIn();
    videoIn();
    cameraInVideoOut();
    
    return 0;
}

void cameraIn() {
    VideoCapture cap(0);
    
    if (!cap.isOpened()) {
        cerr << "Camera open failed!" << endl;
        return;
    }
    
    cout << "Frame width: " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;
    cout << "Frame height: " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;
    
    Mat frame, inversed;
    
    while(true) {
        cap >> frame;
        
        if (frame.empty()) break;
        
        inversed = ~frame;
        
        imshow("frame", frame);
        imshow("inversed", inversed);
        
        if (waitKey(10) == 27) break;
    }
    
    destroyAllWindows();
}

void videoIn() {
    VideoCapture cap("cafe.mov");
    
    if (!cap.isOpened()) {
        cerr << "Video open failed!" << endl;
        return;
    }
    
    cout << "Frame width: " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;
    cout << "Frame height: " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;
    cout << "Frame count: " << cvRound(cap.get(CAP_PROP_FRAME_COUNT)) << endl;
    
    double fps = cap.get(CAP_PROP_FPS);
    cout << "FPS: " << fps << endl;
    
    int delay = cvRound(1000 / fps);
    
    Mat frame, inversed;
    
    while(true) {
        cap >> frame;
        
        if (frame.empty()) break;
        
        inversed = ~frame;
        
        imshow("frame", frame);
        imshow("inversed", inversed);
        
        if (waitKey(delay) == 27) break;
    }
    
    destroyAllWindows();
}

void cameraInVideoOut() {
    VideoCapture cap(0);
    
    if (!cap.isOpened()) {
        cerr << "Camera open failed!" << endl;
        return;
    }
    
    int w = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
    int h = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);
    
    int fourcc = VideoWriter::fourcc('D', 'I', 'V', 'X');
    int delay = cvRound(1000 / fps);
    
    VideoWriter outputVideo("output.avi", fourcc, fps, Size(w, h));
    
    if (!outputVideo.isOpened()) {
        cout << "File open failed!" << endl;
        return;
    }
    
    Mat frame, inversed;
    
    while(true) {
        cap >> frame;
        
        if (frame.empty()) break;
        
        inversed = ~frame;
        outputVideo << inversed;
        
        imshow("frame", frame);
        imshow("inversed", inversed);
        
        if (waitKey(delay) == 27) break;
    }
    
    destroyAllWindows();
}
