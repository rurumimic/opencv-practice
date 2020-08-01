# 머신 러닝

## 용어

- 학습: 데이터로부터 규칙을 찾아내는 과정
  - 지도 학습: 정답(label)이 있는 데이터 학습
  - 비지도 학습: 데이터 자체로 학습
- 모델: 학습으로 만든 규칙
- 예측, 추론: 데이터를 모델에 입력하여 결과를 판단하는 과정
- 이상치: outlier. 잡음. 데이터의 평균과 크게 다른 데이터. 
  - 모델 성능에 영향을 준다.
  - 제거하기 전에 데이터 분석이 필요하다.

### 지도 학습

1. 학습 단계: 
  1. 훈련 영상 특징 추출
  2. 훈련 레이블
  3. 학습 모델 생성
2. 예측 단계:
   1. 입력 영상 특징 추출
   2. 학습 모델 입력
   3. 예측 결과

- 회귀 regression: 연속된 수치 값 예측
- 분류 classification: 이산 수치 값 예측

### 비지도 학습

- 군집화 clustering: 집합의 서로 구분되는 특징을 구별

### k-폴드 교차 검증

- 기존 방법 1
  - 데이터셋: Training, Test
  - 단점: Test 데이터셋으로 테스트 후 모델 검증을 하게 되면 Test 데이터셋에 과대적합 overfitting
- 기존 방법 2
   - 데이터셋: Training, Validation, Test
   - 단점: 훈련 데이터가 적어 모델 성능이 떨어짐. 과소적합 underfitting

![](https://upload.wikimedia.org/wikipedia/commons/b/b5/K-fold_cross_validation_EN.svg)

K-폴드 교차 검증

1. 전체 데이터셋을 K개의 subset으로 나눈다.
2. 1개의 subset을 Test 데이터셋으로 사용한다.
3. 모델 생성, 예측, 평가 지표 추출
4. Test 데이터셋을 바꿔가며 위 과정을 K번 반복
5. 모델의 성능을 비교하여 최적의 모델 선택

- 장점: 
  - 모든 데이터셋 활용
  - 데이터 편중 방지
  - 정확도 향상
- 단점:
  - 오래 걸린다.

## OpenCV 머신 러닝 클래스

모듈: [`cv::ml`](https://docs.opencv.org/master/d3/d29/ml_8hpp.html)

### StateModel 클래스

통계적 모델: [`cv::ml::StatModel`](https://docs.opencv.org/master/db/d7d/classcv_1_1ml_1_1StatModel.html)

![](https://docs.opencv.org/master/db/d7d/classcv_1_1ml_1_1StatModel.png)

하위 클래스들은 다음 메소드를 재정의한다:

- [`StatModel::train()`](https://docs.opencv.org/master/db/d7d/classcv_1_1ml_1_1StatModel.html#af96a0e04f1677a835cc25263c7db3c0c)
- [`StatModel::predict()`](https://docs.opencv.org/master/db/d7d/classcv_1_1ml_1_1StatModel.html#a1a7e49e1febd10392452727498771bc1)

#### train

파라미터:

- samples: 훈련 데이터 행렬
- layout: 훈련 데이터 배치 방법. 
  - [SampleTypes](https://docs.opencv.org/master/dd/ded/group__ml.html#ga9c57a2b823008dda53d2c7f7059a8710)
    - `ROW_SAMPLE`: 각 훈련 데이터가 samples 행렬에 행 단위로 저장되어 있음
    - `COL_SAMPLE`: 각 훈련 데이터가 samples 행렬에 열 단위로 저장되어 있음
- responses: 각 훈련 데이터에 대응되는 응답(레이블) 행렬
- 반환값: 학습 완료 true
- trainData: OpenCV의 훈련 데이터 클래스

c++:

```cpp
virtual bool cv::ml::StatModel::train(const Ptr< TrainData > & trainData, int flags = 0)
virtual bool cv::ml::StatModel::train(InputArray samples, int layout, InputArray responses)
static Ptr<_Tp> cv::ml::StatModel::train(const Ptr< TrainData > & data, int flags = 0)
```

python:

```python
retval = cv.ml_StatModel.train(trainData[, flags])
retval = cv.ml_StatModel.train(samples, layout, responses)
```

#### predict

파라미터:

- samples: 입력 벡터가 행 단위로 저장된 행렬. CV_32F
- results: 각 입력 샘플에 대한 예측 결과가 저장된 행렬
- flags: 추가적인 플래그 상수. [cv::ml::StatModel::Flags
](https://docs.opencv.org/master/db/d7d/classcv_1_1ml_1_1StatModel.html#af1ea864e1c19796e6264ebb3950c0b9a)
- 반환값: 입력 벡터가 하나인 경우에 대한 응답이 반환됨

c++:

```cpp
virtual float cv::ml::StatModel::predic(InputArray samples, OutputArray results = noArray(), int flags = 0) const
```

python:

```python
retval, results = cv.ml_StatModel.predict(samples[, results[, flags]])
```

### 머신 러닝 클래스 종류

- ANN_MLP: 인공 신경망 다층 퍼셉트론. 여러 개의 은닉층을 포함한 신경망을 학습시킬 수 있고, 입력 데이터에 대한 결과 예측 가능
- DTrees: 이진 의사 결정 트리 알고리즘.
  - Boost: 부스팅 알고리즘. 다수의 약한 분류기에 적절한 가중치를 부여하여 성능이 좋은 분류기를 만듦
  - RTree: 랜덤 트리 또는 랜덤 포레스트 알고리즘. 입력 특징 벡터를 다수의 트리로 예측하고, 그 결과를 취합하여 분류 또는 회귀를 수행
- EM: 기댓값 최대화. 가우시안 혼합 모델을 이용한 군집화 알고리즘
- KNearest: k 최근접 이웃 알고리즘. 인접한 k개의 훈련 데이터를 찾고, 이 중 가장 많은 개수에 해당하는 클래스를 샘플 데이터 클래스로 지정
- LogisticRegression: 로지스틱 회귀. 이진 분류 알고리즘
- NormalBayesClassifier: 정규 베이즈 분류기. 각 클래스의 특징 벡터가 정규 분포를 따른다고 가정한다. 전체 데이터 분포를 혼합 모델로 표현 가능. 학습 데이터로부터 각 클래스의 평균 벡터와 공분산 행렬을 계산하고, 이를 예측에 사용
- SVM: 서포트 벡터 머신 알고리즘. 두 클래스의 데이터를 가장 여유 있게 분리하는 초평면을 구한다. 커널 기법을 이용하여 비선형 데이터 분류에도 사용 가능. 다중 클래스 분류 및 회귀에도 적용 가능
- SVMSDG: 통계적 그래디언트 하향 SVM. 대용량 데이터도 빠른 학습이 가능
