//
//  main.cpp
//  color
//
//  Created by Dohyun Kim on 12/07/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void inverse1(Mat src) {
    Mat dst(src.rows, src.cols, src.type());
    
    for (int j = 0; j < src.rows; j++) {
        for (int i = 0; i < src.cols; i++) {
            /*
            Vec3b& p1 = src.at<Vec3b>(j, i);
            Vec3b& p2 = dst.at<Vec3b>(j, i);
            
            p2[0] = 255 - p1[0]; // B
            p2[1] = 255 - p1[1]; // G
            p2[2] = 255 - p1[2]; // R
            */
            
            dst.at<Vec3b>(j, i) = Vec3b(255, 255, 255) - src.at<Vec3b>(j, i);
        }
    }
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey();
    destroyAllWindows();
}

void inverse2(Mat src) {
    Mat dst = Scalar(255, 255, 255) - src;
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey();
    destroyAllWindows();
}

void grayscale(Mat src) {
    Mat dst;
    cvtColor(src, dst, COLOR_BGR2GRAY);
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey();
    destroyAllWindows();
}

void splits(Mat src) {
    vector<Mat> bgr_planes;
    split(src, bgr_planes);
    
    imshow("src", src);
    imshow("B_plane", bgr_planes[0]);
    imshow("G_plane", bgr_planes[1]);
    imshow("R_plane", bgr_planes[2]);
    
    waitKey();
    destroyAllWindows();
}

int main(int argc, const char * argv[]) {
    
    Mat src = imread("lenna.bmp", IMREAD_COLOR);

    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    
//    inverse1(src);
//    inverse2(src);
//    grayscale(src);
    splits(src);
    
    return 0;
}
