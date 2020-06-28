//
//  main.cpp
//  operation
//
//  Created by Dohyun Kim on 28/06/2020.
//  Copyright © 2020 rurumimic. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void await() {
    while(true) {
        if (waitKey(10) == 27) {
            destroyAllWindows();
            break;
        }
    }
}

int main(int argc, const char * argv[]) {
    Mat src1 = imread("lenna.bmp", IMREAD_GRAYSCALE);
    Mat src2 = imread("night.jpg", IMREAD_GRAYSCALE);
    Mat src3 = imread("lenna256.bmp", IMREAD_GRAYSCALE);
    Mat src4 = imread("square.bmp", IMREAD_GRAYSCALE);
    
    Mat dst0, dst1, dst2;
    
    dst0 = src1 + src2; // 간단한 덧셈
    addWeighted(src1, 0.7, src2, 0.3, 0, dst1); // 가중치 덧셈
    
    dst2 = src1 - src2; // 간단한 뺄셈
    
    imshow("add", dst0);
    imshow("add weighted", dst1);
    imshow("minus", dst2);
    
    await();
    
    Mat dst3, dst4, dst5, dst6, dst7, dst8, dst9;
    
    add(src3, src4, dst3);
    addWeighted(src3, 0.5, src4, 0.5, 0, dst4);
    
    subtract(src3, src4, dst5); // 뺄셈 연산
    subtract(src4, src3, dst6); // 뺄셈 연산
    
    absdiff(src3, src4, dst7); // 차이 연산
    
    multiply(src3, src4, dst8); // 곱셈 연산
    divide(src3, src4, dst9); // 나눗셈 연산
    
    imshow("add", dst3);
    imshow("add weighted", dst4);
    imshow("subtract 1 - 2", dst5);
    imshow("subtract 2 - 1", dst6);
    imshow("abs diff", dst7);
    imshow("multiply", dst8);
    imshow("divide", dst9);
    
    await();
    
    Mat dst10, dst11, dst12, dst13;
    
    bitwise_and(src3, src4, dst10); // 논리곱
    bitwise_or(src3, src4, dst11); // 논리합
    bitwise_xor(src3, src4, dst12); // 배타적 논리합
    bitwise_not(src3, dst13); // 부정
    
    imshow("and", dst10);
    imshow("or", dst11);
    imshow("xor", dst12);
    imshow("not", dst13);
    
    await();
    
    return 0;
}
