#include <stdio.h>
#include <math.h>



int main()
{
	// �뾶
	const int R = 1;
	
	// �����θ���
	int n = 6;

	// �����α߳�
	int a = R;

	// �����θ� = 1 - (a/2)^2
	long double h = sqrt(1 - pow(a / 2.0, 2));

	// ���������
	long double s = a * h / 2.0;

	// Բ�����������Σ������ ��Ϊ S = pi * r^2, r=1, ���� S = pi
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
