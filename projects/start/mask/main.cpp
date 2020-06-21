//
//  main.cpp
//  mask
//
//  Created by Dohyun Kim on 21/06/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void mask_setTo();

int main(int argc, const char * argv[]) {
    mask_setTo();
    return 0;
}

void mask_setTo() {
  Mat src = imread("lenna.bmp", IMREAD_COLOR);
  Mat mask = imread("mask_smile.bmp", IMREAD_GRAYSCALE);

  if (src.empty() || mask.empty()){
    cerr << "Image load failed!" << endl;
    return;
  }

  src.setTo(Scalar(0, 255, 255), mask);

  imshow("src",src);
  imshow("mask", mask);

  waitKey(0);
  destroyAllWindows();
}
