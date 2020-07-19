//
//  main.cpp
//  morphology
//
//  Created by Dohyun Kim on 19/07/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void erode_dilate(Mat src) {
    Mat bin;
    threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);
    
    Mat dst1, dst2;
    erode(bin, dst1, Mat());
    dilate(bin, dst2, Mat());
    
    imshow("src", src);
    imshow("bin", bin);
    imshow("dst1", dst1);
    imshow("dst2", dst2);
    
    waitKey();
    destroyAllWindows();
}

void open_close(Mat src) {
    Mat bin;
    threshold(src, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);
    
    Mat dst1, dst2;
    morphologyEx(bin, dst1, MORPH_OPEN, Mat());
    morphologyEx(bin, dst2, MORPH_CLOSE, Mat());
    
    imshow("src", src);
    imshow("bin", bin);
    imshow("open", dst1);
    imshow("close", dst2);
    
    waitKey();
    destroyAllWindows();
}

int main(int argc, const char * argv[]) {
    
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    
//    erode_dilate(src);
    open_close(src);
    
    return 0;
}
