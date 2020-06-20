//
//  main.cpp
//  image
//
//  Created by Dohyun Kim on 20/06/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
    
    Mat img;
    img = imread("lenna.bmp");
    
    if (img.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }
    
    namedWindow("image");
    imshow("image", img);
    
    waitKey();
    
    return 0;
}
