//
//  main.cpp
//  affine
//
//  Created by Dohyun Kim on 05/07/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void affine_transform(Mat src) {
    Point2f srcPts[3], dstPts[3];
    srcPts[0] = Point2f(0, 0);
    srcPts[1] = Point2f(src.cols - 1, 0);
    srcPts[2] = Point2f(src.cols - 1, src.rows - 1);
    dstPts[0] = Point2f(50, 50);
    dstPts[1] = Point2f(src.cols - 100, 100);
    dstPts[2] = Point2f(src.cols - 50, src.rows - 50);
    
    Mat M = getAffineTransform(srcPts, dstPts);
    
    Mat dst;
    warpAffine(src, dst, M, Size());
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey(0);
    destroyAllWindows();
}

void affine_translation(Mat src) {
    Mat M = Mat_<double>({ 2, 3 }, {1, 0, 150, 0, 1, 100});
    
    Mat dst;
    warpAffine(src, dst, M, Size());
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey(0);
    destroyAllWindows();
    
}

void affine_shear(Mat src) {
    double mx = 0.3;
    Mat M = Mat_<double>({ 2, 3 }, {1, mx, 0, 0, 1, 0});
    
    Mat dst;
    warpAffine(src, dst, M, Size(cvRound(src.cols + src.rows * mx), src.rows));
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey(0);
    destroyAllWindows();
    
}

void affine_scale(Mat src) {
    
    Mat dst;
//    resize(src, dst, Size(), 4, 4, INTER_NEAREST);
//    resize(src, dst, Size(1920, 1280));
//    resize(src, dst, Size(1920, 1280), 0, 0, INTER_CUBIC);
    resize(src, dst, Size(1920, 1280), 0, 0, INTER_LANCZOS4);
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey(0);
    destroyAllWindows();
    
}

void affine_rotation(Mat src) {
    Point2f cp(src.cols / 2.f, src.rows / 2.f);
    Mat M = getRotationMatrix2D(cp, 20, 1);
    
    Mat dst;
    warpAffine(src, dst, M, Size());
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey(0);
    destroyAllWindows();
    
}

void affine_flip(Mat src) {
    
    Mat dst;
    int flipCode[] = { 1, 0, -1 };
    
//    flip(src, dst, flipCode[0]);
//    flip(src, dst, flipCode[1]);
    flip(src, dst, flipCode[2]);
    
    imshow("src", src);
    imshow("dst", dst);
    
    waitKey(0);
    destroyAllWindows();
    
}


int main(int argc, const char * argv[]) {
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return 0;
    }
    
//    affine_transform(src);
//    affine_translation(src);
//    affine_shear(src);
//    affine_scale(src);
//    affine_rotation(src);
    affine_flip(src);
    
    return 0;
}
