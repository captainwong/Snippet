#include <stdio.h>
#include <math.h>



int main()
{
	// 半径
	const int R = 1;
	
	// 三角形个数
	int n = 6;

	// 三角形边长
	int a = R;

	// 三角形高 = 1 - (a/2)^2
	long double h = sqrt(1 - pow(a / 2.0, 2));

	// 三角形面积
	long double s = a * h / 2.0;

	// 圆（所有三角形）的面积 因为 S = pi * r^2, r=1, 所以 S = pi
	long double pi = 6 * s;

	printf("n=%d, pi=%lf\n", n, pi);

	while (true) {
		n;
		h = R - h;
		s = a * h / 2.0;
		pi += n * s;
		n *= 2;
		a = sqrt(h * h * (a / 2.0) * (a / 2.0));
		h
		printf("n=%d, pi=%lf\n", n, pi);
	}
}
