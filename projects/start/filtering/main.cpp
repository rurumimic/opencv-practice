//
//  main.cpp
//  filtering
//
//  Created by Dohyun Kim on 05/07/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void display(Mat src, Mat dst) {
    imshow("src", src);
    imshow("dst", dst);
    waitKey();
    
    destroyAllWindows();
}

int main(int argc, const char * argv[]) {
    
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return 0;
    }
    
    float data[] = { -1, -1, 0, -1, 0, 1, 0, 1, 1 };
    Mat emboss(3, 3, CV_32FC1, data);
    
    Mat dst;
    filter2D(src, dst, -1, emboss, Point(-1, -1), 128);
    
    display(src, dst);
    
    return 0;
}
