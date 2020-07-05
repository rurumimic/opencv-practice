# 블러링

blurring, smoothing

## 평균값 필터 mean filter

[blur](https://docs.opencv.org/master/d4/d86/group__imgproc__filter.html#ga8c45db9afe636703801b0b2e440fce37)

![\texttt{K} = \frac{1}{\texttt{ksize.width*ksize.height}} \begin{bmatrix} 1 & 1 & 1 & \cdots & 1 & 1 \\ 1 & 1 & 1 & \cdots & 1 & 1 \\ \hdotsfor{6} \\ 1 & 1 & 1 & \cdots & 1 & 1 \\ \end{bmatrix}](https://latex.codecogs.com/svg.latex?%5Ctexttt%7BK%7D%20%3D%20%5Cfrac%7B1%7D%7B%5Ctexttt%7Bksize.width*ksize.height%7D%7D%20%5Cbegin%7Bbmatrix%7D%201%20%26%201%20%26%201%20%26%20%5Ccdots%20%26%201%20%26%201%20%5C%5C%201%20%26%201%20%26%201%20%26%20%5Ccdots%20%26%201%20%26%201%20%5C%5C%20%5Chdotsfor%7B6%7D%20%5C%5C%201%20%26%201%20%26%201%20%26%20%5Ccdots%20%26%201%20%26%201%20%5C%5C%20%5Cend%7Bbmatrix%7D)

c++:

```cpp
void cv::blur(InputArray src, 
              OutputArray dst, 
              Size size, 
              Point anchor = Point(-1, -1), 
              int borderType = BORDER_DEFAULT);
```

python:

```python
dst = cv.blur(src, ksize[, dst[, anchor[, borderType]]])
```

![](images/blurring.png)

## 가우시안 필터 gaussian filter

가우시안 분포 Gaussian distribution 함수를 근사하여 생성한 필터 마스크

![G_{\sigma}(x) = \frac{1}{\sqrt{2\pi}\sigma}e^{-\frac{x^{2}}{2\sigma ^{2}}}](https://latex.codecogs.com/svg.latex?G_%7B%5Csigma%7D%28x%29%20%3D%20%5Cfrac%7B1%7D%7B%5Csqrt%7B2%5Cpi%7D%5Csigma%7De%5E%7B-%5Cfrac%7Bx%5E%7B2%7D%7D%7B2%5Csigma%20%5E%7B2%7D%7D%7D)

![
\begin{align*}
G_{\sigma_{x},\sigma_{y}}(x, y) &= \frac{1}{\sqrt{2\pi}\sigma_{x}\sigma_{y}}e^{-(\frac{x^{2}}{2\sigma_{x} ^{2}}+\frac{y^{2}}{2\sigma_{y} ^{2}})} \\
&= \frac{1}{\sqrt{2\pi}\sigma_{x}}e^{-\frac{x^{2}}{2\sigma_{x} ^{2}}} \times \frac{1}{\sqrt{2\pi}\sigma_{y}}e^{-\frac{y^{2}}{2\sigma_{y} ^{2}}} \\
&= G_{\sigma_{x}}(x) \cdot G_{\sigma_{y}}(y)
\end{align*}
](https://latex.codecogs.com/svg.latex?%5Cbegin%7Balign*%7D%20G_%7B%5Csigma_%7Bx%7D%2C%5Csigma_%7By%7D%7D%28x%2C%20y%29%20%26%3D%20%5Cfrac%7B1%7D%7B%5Csqrt%7B2%5Cpi%7D%5Csigma_%7Bx%7D%5Csigma_%7By%7D%7De%5E%7B-%28%5Cfrac%7Bx%5E%7B2%7D%7D%7B2%5Csigma_%7Bx%7D%20%5E%7B2%7D%7D&plus;%5Cfrac%7By%5E%7B2%7D%7D%7B2%5Csigma_%7By%7D%20%5E%7B2%7D%7D%29%7D%20%5C%5C%20%26%3D%20%5Cfrac%7B1%7D%7B%5Csqrt%7B2%5Cpi%7D%5Csigma_%7Bx%7D%7De%5E%7B-%5Cfrac%7Bx%5E%7B2%7D%7D%7B2%5Csigma_%7Bx%7D%20%5E%7B2%7D%7D%7D%20%5Ctimes%20%5Cfrac%7B1%7D%7B%5Csqrt%7B2%5Cpi%7D%5Csigma_%7By%7D%7De%5E%7B-%5Cfrac%7By%5E%7B2%7D%7D%7B2%5Csigma_%7By%7D%20%5E%7B2%7D%7D%7D%20%5C%5C%20%26%3D%20G_%7B%5Csigma_%7Bx%7D%7D%28x%29%20%5Ccdot%20G_%7B%5Csigma_%7By%7D%7D%28y%29%20%5Cend%7Balign*%7D)

표준 편차 σ가 작을 수록 뾰족한 그래프가 된다.

가우시안 필터 마스크 크기는 보통 (8σ + 1)이다.

[GaussianBlur](https://docs.opencv.org/master/d4/d86/group__imgproc__filter.html#gaabe8c836e97159a9193fb0b11ac52cf1)

c++:

```cpp
void cv::GaussianBlur(InputArray src, 
                      OutputArray dst, 
                      Size size, 
                      double sigmaX,
                      double sigmaY = 0,
                      int borderType = BORDER_DEFAULT);
```

python:

```python
dst = cv.GaussianBlur(src, ksize, sigmaX[, dst[, sigmaY[, borderType]]])
```

![](images/gaussian.blurring.png)

### Gaussian Kernel

![G_i= \alpha \cdot e^{-\frac{(i-( \texttt{ksize} -1)/2)^2}{2 \cdot \texttt{sigma}^2}}](https://latex.codecogs.com/svg.latex?G_i%3D%20%5Calpha%20%5Ccdot%20e%5E%7B-%5Cfrac%7B%28i-%28%20%5Ctexttt%7Bksize%7D%20-1%29/2%29%5E2%7D%7B2%20%5Ccdot%20%5Ctexttt%7Bsigma%7D%5E2%7D%7D)

- i: 0 ~ ksize-1
- α: ![\sum_{i}^{}G_i = 1](https://latex.codecogs.com/svg.latex?%5Cinline%20%5Csum_%7Bi%7D%5E%7B%7DG_i%20%3D%201)

where i=0..𝚔𝚜𝚒𝚣𝚎−1 and α is the scale factor chosen so that ∑iGi=1

[getGaussianKernel](https://docs.opencv.org/master/d4/d86/group__imgproc__filter.html#gac05a120c1ae92a6060dd0db190a61afa)

c++:

```cpp
Mat cv::getGaussianKernel(int ksize, double sigma, int ktype=CV_64F)
```

python:

```python
retval = cv.getGaussianKernel(ksize, sigma[, ktype])
```
