# 샤프닝

unsharp mask filter: unsharp 영상을 이용하여 날카로운 영상 생성하는 필터

![
\begin{align*}
h(x, y) &= f(x, y) + \alpha(f(x, y) - \bar f(x, y)) \\
&= (1 + \alpha)f(x, y) - \alpha \cdot \bar f(x, y)
\end{align*}
](https://latex.codecogs.com/svg.latex?%5Cbegin%7Balign*%7D%20h%28x%2C%20y%29%20%26%3D%20f%28x%2C%20y%29%20&plus;%20%5Calpha%28f%28x%2C%20y%29%20-%20%5Cbar%20f%28x%2C%20y%29%29%20%5C%5C%20%26%3D%20%281%20&plus;%20%5Calpha%29f%28x%2C%20y%29%20-%20%5Calpha%20%5Ccdot%20%5Cbar%20f%28x%2C%20y%29%20%5Cend%7Balign*%7D)

![](images/sharp.png)
