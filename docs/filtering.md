# 필터링

- 필터링: 영상에서 원하는 정보만 통과시키고 원치 낳는 정보는 걸러 내는 작업
- 마스크 mask: 필터링의 성격을 정의하는 행렬. 커널 kernel. 윈도우 window. 필터.
  - 고정점 anchor point: 대부분 행렬의 정중앙

## 마스크 연산

![
dst (x,y) \\
= \sum _{ \substack{0\leq x' < kernel.cols \\
{0\leq y' < kernel.rows}}} kernel (x',y')* src(x+x'- anchor.x ,y + y'- anchor.y)
](https://latex.codecogs.com/svg.latex?dst%20%28x%2Cy%29%20%5C%5C%20%3D%20%5Csum%20_%7B%20%5Csubstack%7B0%5Cleq%20x%27%20%3C%20kernel.cols%20%5C%5C%20%7B0%5Cleq%20y%27%20%3C%20kernel.rows%7D%7D%7D%20kernel%20%28x%27%2Cy%27%29*%20src%28x&plus;x%27-%20anchor.x%20%2Cy%20&plus;%20y%27-%20anchor.y%29)

![
\begin{align*}
g(x, y) &= m(0, 0)f(x-1, y-1) + m(1, 0)f(x, y-1) +  m(2, 0)f(x+1, y-1) \\
&+ m(0, 1)f(x-1, y) \;\;\;\;\;\; + m(1, 1)f(x, y) \;\;\;\;\;\; + m(2, 1)f(x+1, y) \\
&+ m(0, 2)f(x-1, y+1) + m(1, 2)f(x, y+1) + m(2, 2)f(x+1, y+1)
\end{align*}
](https://latex.codecogs.com/svg.latex?%5Cbegin%7Balign*%7D%20g%28x%2C%20y%29%20%26%3D%20m%280%2C%200%29f%28x-1%2C%20y-1%29%20&plus;%20m%281%2C%200%29f%28x%2C%20y-1%29%20&plus;%20m%282%2C%200%29f%28x&plus;1%2C%20y-1%29%20%5C%5C%20%26&plus;%20m%280%2C%201%29f%28x-1%2C%20y%29%20%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%20&plus;%20m%281%2C%201%29f%28x%2C%20y%29%20%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%5C%3B%20&plus;%20m%282%2C%201%29f%28x&plus;1%2C%20y%29%20%5C%5C%20%26&plus;%20m%280%2C%202%29f%28x-1%2C%20y&plus;1%29%20&plus;%20m%281%2C%202%29f%28x%2C%20y&plus;1%29%20&plus;%20m%282%2C%202%29f%28x&plus;1%2C%20y&plus;1%29%20%5Cend%7Balign*%7D)

### 가장자리 픽셀 확장 방법

[BorderTypes](https://docs.opencv.org/master/d2/de8/group__core__array.html#ga209f2f4869e304c82d07739337eae7c5)

대부분 실제 영상의 픽셀 값을 대칭 형태로 가상의 픽셀을 만든 후 연산한다.

### 함수

[filter2D](https://docs.opencv.org/master/d4/d86/group__imgproc__filter.html#ga27c049795ce870216ddfb366086b5a04)

파라미터:

- ddepth: 결과 영상의 깊이
- kernel: 필터링 커널. 1채널 실수형 행렬
- anchor: 고정점 좌표. Point(-1, -1)을 지정하면 커널 중심을 고정점으로 사용합니다.
- delta: 필터링 연산 후 추가적으로 더할 값
- borderType: 가장자리 픽셀 확장 방식

c++:

```cpp
void filter2D(InputArray src, 
              OutputArray dst, 
              int ddepth, 
              InputArray kernel, 
              Point anchor = Point(-1, -1), 
              double delta = 0, 
              int borderType = BORDER_DEFAULT);
```

python:

```python
dst = cv.filter2D(src, ddepth, kernel[, dst[, anchor[, delta[, borderType]]]])
```

## 엠보싱 필터링

|     |     |     |
| --- | --- | --- |
| -1  | -1  | 0   |
| -1  | 0   | 1   |
| 0   | 1   | 1   |

![](images/filtering.emboss.png)
