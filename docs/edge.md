# 에지 검출

- 픽셀 값이 한쪽 방향으로 급격하게 바뀌는 부분
- 픽셀 값의 변화율을 이용한다
- 객체 판별을 위한 전처리롤 사용된다

## 미분

**derivative**: 함수의 순간 변화율

1차원 연속 함수 f(x)의 미분:

![f' = \frac{df}{dx} = \lim_{\Delta x \rightarrow 0}\frac{f(x + \Delta x) - f(x)}{\Delta x}](https://latex.codecogs.com/svg.latex?f%27%20%3D%20%5Cfrac%7Bdf%7D%7Bdx%7D%20%3D%20%5Clim_%7B%5CDelta%20x%20%5Crightarrow%200%7D%5Cfrac%7Bf%28x%20+%20%5CDelta%20x%29%20-%20f%28x%29%7D%7B%5CDelta%20x%7D)

전진 차분 forward difference:

![\frac{dI}{dx} \cong \frac{I(x + h) - I(x)}{h}](https://latex.codecogs.com/svg.latex?%5Cfrac%7BdI%7D%7Bdx%7D%20%5Ccong%20%5Cfrac%7BI%28x%20+%20h%29%20-%20I%28x%29%7D%7Bh%7D)

후진 차분 backward difference:

![\frac{dI}{dx} \cong \frac{I(x) - I(x - h)}{h}](https://latex.codecogs.com/svg.latex?%5Cfrac%7BdI%7D%7Bdx%7D%20%5Ccong%20%5Cfrac%7BI%28x%29%20-%20I%28x%20-%20h%29%7D%7Bh%7D)

**중앙 차분 centered difference**: 이론적으로 근사화 오류가 가장 적다

![\frac{dI}{dx} \cong \frac{I(x + h) - I(x - h)}{2h}](https://latex.codecogs.com/svg.latex?%5Cfrac%7BdI%7D%7Bdx%7D%20%5Ccong%20%5Cfrac%7BI%28x%20+%20h%29%20-%20I%28x%20-%20h%29%7D%7B2h%7D)

영상 특징:

1. 영상 = 2차원 평면에서 정의된 함수
2. 영상 = 정수 단위 좌표에 픽셀이 나열된 이산함수
3. 편미분 partial derivative: 가로, 세로 각각 미분

중앙 차분 방법으로 근사화한 편미분 방정식:

![
\begin{align*}
I_x = \frac{\delta I}{\delta x} \cong \frac{I(x + 1, y) - I(x - 1, y)}{2} 
I_y = \frac{\delta I}{\delta y} \cong \frac{I(x, y + 1) - I(x, y - 1)}{2}
\end{align*}
](https://latex.codecogs.com/svg.latex?%5Cbegin%7Balign*%7D%20I_x%20%3D%20%5Cfrac%7B%5Cdelta%20I%7D%7B%5Cdelta%20x%7D%20%5Ccong%20%5Cfrac%7BI%28x%20+%201%2C%20y%29%20-%20I%28x%20-%201%2C%20y%29%7D%7B2%7D%20%5C%5C%20I_y%20%3D%20%5Cfrac%7B%5Cdelta%20I%7D%7B%5Cdelta%20y%7D%20%5Ccong%20%5Cfrac%7BI%28x%2C%20y%20+%201%29%20-%20I%28x%2C%20y%20-%201%29%7D%7B2%7D%20%5Cend%7Balign*%7D)

필터 마스크:

편미분 근사 수식과 같이 그대로 1/2를 곱하지 않고  
단순하게 상용한다.  
(미분 값의 상대적 크기가 중요하기 때문이다)

- X축 1x3: [-1, 0, 1]
- Y축 3x1: [-1, 0, 1]

그래디언트 gradient:

2차원 함수 f(x, y)의 편미분을 한번에 벡터로 표현.  
두 편미분 (벡터)의 합.

![
\bigtriangledown f = \begin{bmatrix}
f_x \ 
f_y
\end{bmatrix} = f_x \mathbf{i} + f_y \mathbf{j}
](https://latex.codecogs.com/svg.latex?%5Cbigtriangledown%20f%20%3D%20%5Cbegin%7Bbmatrix%7D%20f_x%20%5C%5C%20f_y%20%5Cend%7Bbmatrix%7D%20%3D%20f_x%20%5Cmathbf%7Bi%7D%20+%20f_y%20%5Cmathbf%7Bj%7D)

벡터 크기: magnitude

![\left | \bigtriangledown f \right | = \sqrt{f_x^{2} + f_y^{2}}](https://latex.codecogs.com/svg.latex?%5Cleft%20%5C%7C%20%5Cbigtriangledown%20f%20%5Cright%20%5C%7C%20%3D%20%5Csqrt%7Bf_x%5E%7B2%7D%20+%20f_y%5E%7B2%7D%7D)

벡터 방향: phase

밝기가 가장 밝아지는 방향을 가리킨다.

![\theta = \textup{tan}^{-1}\left ( \frac{f_y}{f_x} \right )](https://latex.codecogs.com/svg.latex?%5Ctheta%20%3D%20%5Ctextup%7Btan%7D%5E%7B-1%7D%5Cleft%20%28%20%5Cfrac%7Bf_y%7D%7Bf_x%7D%20%5Cright%20%29)

에지 방향: 그래디언트 벡터와 수직

## 마스크 기반 에지 검출

마스크 크기 클수록 영상 잡음의 영향이 줄어든다.

**Sobel filter:**

![
\begin{bmatrix}
-1 & 0 & 1 \ 
-2 & 0 & 2 \ 
-1 & 0 & 1
\end{bmatrix}
](https://latex.codecogs.com/svg.latex?%5Cbegin%7Bbmatrix%7D%20-1%20%26%200%20%26%201%20%5C%5C%20-2%20%26%200%20%26%202%20%5C%5C%20-1%20%26%200%20%26%201%20%5Cend%7Bbmatrix%7D) ![
\begin{bmatrix}
-1 & -2 & -1 \ 
0 & 0 & 0 \ 
1 & 2 & 1
\end{bmatrix}
](https://latex.codecogs.com/svg.latex?%5Cbegin%7Bbmatrix%7D%20-1%20%26%20-2%20%26%20-1%20%5C%5C%200%20%26%200%20%26%200%20%5C%5C%201%20%26%202%20%26%201%20%5Cend%7Bbmatrix%7D)

### Sobel 미분

[Sobel](https://docs.opencv.org/master/d4/d86/group__imgproc__filter.html#gacea54f142e81b6758cb6f375ce782c8d)

![\texttt{dst} = \frac{\partial^{xorder+yorder} \texttt{src}}{\partial x^{xorder} \partial y^{yorder}}](https://latex.codecogs.com/svg.latex?%5Ctexttt%7Bdst%7D%20%3D%20%5Cfrac%7B%5Cpartial%5E%7Bxorder+yorder%7D%20%5Ctexttt%7Bsrc%7D%7D%7B%5Cpartial%20x%5E%7Bxorder%7D%20%5Cpartial%20y%5E%7Byorder%7D%7D)

파라미터:

- dx: x 방향 미분 차수
- dy: y 방향 미분 차수
- ksize: 소벨 커널의 크기
- scale: 필터링 연산 후 추가적으로 곱할 값
- delta: 필터링 연산 후 추가적으로 더할 값

c++:

```cpp
void cv::Sobel(InputArray src,
               OutputArray dst,
               int ddepth,
               int dx,
               int dy,
               int ksize = 3,
               double scale = 1,
               double delta = 0,
               int borderType = BORDER_DEFAULT)
```

python:

```python
dst = cv.Sobel(src, ddepth, dx, dy[, dst[, ksize[, scale[, delta[, borderType]]]]])
```

### Scharr 미분

[Scharr](https://docs.opencv.org/master/d4/d86/group__imgproc__filter.html#gaa13106761eedf14798f37aa2d60404c9)

c++:

```cpp
void cv::Scharr(InputArray src,
                OutputArray dst,
                int ddepth,
                int dx,
                int dy,
                double scale = 1,
                double delta = 0,
                int borderType = BORDER_DEFAULT)
```

python:

```python
dst = cv.Scharr(src, ddepth, dx, dy[, dst[, scale[, delta[, borderType]]]])
```

### magnitude 그래디언트 크기

[magnitude](https://docs.opencv.org/master/d2/de8/group__core__array.html#ga6d3b097586bca4409873d64a90fe64c3)

파라미터:

- x: 벡터의 x 좌표를 나타내는 실수 행렬 또는 벡터
- y: 벡터의 y 좌표를 나타내는 실수 행렬 또는 벡터
- magnitude: 벡터의 크기를 나타내는 실수 행렬 또는 벡터

c++:

```cpp
void cv::magnitude(InputArray x,
                   InputArray y,
                   OutputArray magnitude)
```

python:

```python
magnitude = cv.magnitude(x, y[, magnitude])
```

### phase 그래디언트 방향

[phase](https://docs.opencv.org/master/d2/de8/group__core__array.html#ga9db9ca9b4d81c3bde5677b8f64dc0137)

파라미터:

- x: 벡터의 x 좌표를 나타내는 실수 행렬 또는 벡터
- y: 벡터의 y 좌표를 나타내는 실수 행렬 또는 벡터
- angle: 벡터의 방향을 나타내는 실수 행렬 또는 벡터
- angleInDegrees: 기본값 라디안. 각도, 라디안.

c++:

```cpp
void cv::phase(InputArray x,
               InputArray y,
               OutputArray angle
               bool angleInDegrees = false)
```

python:

```python
angle = cv.phase(x, y[, angle[, angleInDegrees]])
```

![](images/edge.sobel.png)

## Canny 에지 검출기

좋은 에지 검출기 조건:

1. 정확한 검출: good detection
2. 정확한 위치: good localization
3. 단일 에지: single edge

연산 과정:

1. 가우시안 필터링: 영상 잡음 제거. 생략 가능
2. 그래디언트 계산: 소벨 마스크 필터링 후 그래디언트 크기, 방향 계산
   - L2 norm: ![\left | \bigtriangledown f \right | = \sqrt{f_x^{2} + f_y^{2}}](https://latex.codecogs.com/svg.latex?%5Cinline%20%5Cleft%20%5C%7C%20%5Cbigtriangledown%20f%20%5Cright%20%5C%7C%20%3D%20%5Csqrt%7Bf_x%5E%7B2%7D%20+%20f_y%5E%7B2%7D%7D)
   - **L1 norm**: ![\left | \bigtriangledown f \right | \approx \left | f_x \right | + \left | f_y \right |](https://latex.codecogs.com/svg.latex?%5Cinline%20%5Cleft%20%5C%7C%20%5Cbigtriangledown%20f%20%5Cright%20%5C%7C%20%5Capprox%20%5Cleft%20%7C%20f_x%20%5Cright%20%7C%20+%20%5Cleft%20%7C%20f_y%20%5Cright%20%7C)
3. 비최대 억제: non-maximum suppression.
   - 그래디언트 크기가 국지적 최대(local maximum)인 픽셀만 에지 픽셀로 설정
   - 특정 픽셀을 둘러싸고 있는 모든 픽셀 값을 검사하여 국지적 최대인지 판별
   - 그래디언트 벡터의 방향과 같은 방향에 있는 인접 픽셀끼리만 국지적 최대 검사 수행
   - 가장 변화율이 큰 위치의 픽셀만 에지로 검색됨
4. 이중 임계값을 이용한 히스테리시스 에지 트래킹
   - T<sub>High</sub>
     - < 픽셀 값: 에지 O
     - > 픽셀 값: 에지 X
   - T<sub>Low</sub> < 픽셀 값 < T<sub>High</sub> : 추가 검사
     - T<sub>High</sub>보다 큰 픽셀과 연결 O: 에지 O
     - T<sub>High</sub>보다 큰 픽셀과 연결 X: 에지 X

### Canny

[Canny](https://docs.opencv.org/master/dd/d1a/group__imgproc__feature.html#ga04723e007ed888ddf11d9ba04e2232de)

함수:

1. 일반 영상 입력
2. 미분 영상 입력

파라미터:

- threshold1: 낮은 임계값
- threshold2: 높은 임계값
- apertureSize: 소벨 마스크 크기
- L2gradient: 기본값 L1 norm

c++:

```cpp
void cv::Canny(InputArray image,
               OutputArray edges,
               double threshold1,
               double threshold2,
               int apertureSize = 3,
               bool L2gradient = false)
void cv::Canny(InputArray dx,
               InputArray dy,
               OutputArray edges,
               double threshold1,
               double threshold2,
               bool L2gradient = false)
```

python:

```python
edges = cv.Canny(image, threshold1, threshold2[, edges[, apertureSize[, L2gradient]]])
edges = cv.Canny(dx, dy, threshold1, threshold2[, edges[, L2gradient]])
```

![](images/edge.canny.png)
