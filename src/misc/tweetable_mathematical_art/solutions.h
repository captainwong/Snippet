#pragma once

#include "tweetable_mathematical_art.h"
#include <time.h> // needed by JuliaSets2


struct KyleMcCormick : SolutionShort {
	virtual unsigned short RD(int i, int j) override {
		return (unsigned short)sqrt((double)(_sq(i - DIM / 2) * _sq(j - DIM / 2)) * 2.0);
	}

	virtual unsigned short GR(int i, int j) override {
		return (unsigned short)sqrt((double)(
			(_sq(i - DIM / 2) | _sq(j - DIM / 2)) *
			(_sq(i - DIM / 2) & _sq(j - DIM / 2))
			));
	}

	virtual unsigned short BL(int i, int j) override {
		return (unsigned short)sqrt((double)(_sq(i - DIM / 2) & _sq(j - DIM / 2)) * 2.0);
	}
};

struct KyleMcCormick2 : SolutionShort {
	virtual unsigned short RD(int i, int j) override {
		return (i && j) ? (i % j) & (j % i) : 0;
	}

	virtual unsigned short GR(int i, int j) override {
		return (i && j) ? (i % j) + (j % i) : 0;
	}

	virtual unsigned short BL(int i, int j) override {
		return (i && j) ? (i % j) | (j % i) : 0;
	}
};

struct Mandelbrot : SolutionChar {
	virtual unsigned char RD(int i, int j) override {
		double a = 0, b = 0, c, d, n = 0;
		while ((c = a * a) + (d = b * b) < 4 && n++ < 880) { b = 2 * a * b + j * 8e-9 - .645411; a = c - d + i * 8e-9 + .356888; }
		return 255 * pow((n - 80) / 800, 3.);
	}

	virtual unsigned char GR(int i, int j) override {
		double a = 0, b = 0, c, d, n = 0;
		while ((c = a * a) + (d = b * b) < 4 && n++ < 880) { b = 2 * a * b + j * 8e-9 - .645411; a = c - d + i * 8e-9 + .356888; }
		return 255 * pow((n - 80) / 800, .7);
	}

	virtual unsigned char BL(int i, int j) override {
		double a = 0, b = 0, c, d, n = 0;
		while ((c = a * a) + (d = b * b) < 4 && n++ < 880) { b = 2 * a * b + j * 8e-9 - .645411; a = c - d + i * 8e-9 + .356888; }
		return 255 * pow((n - 80) / 800, .5);
	}
};

struct Mandelbrot2 : SolutionChar {
	unsigned char RD(int i, int j) {
		double a = 0, b = 0, d, n = 0;
		for (; a * a + (d = b * b) < 4 && n++ < 8192; b = 2 * a * b + j / 5e4 + .06, a = a * a - d + i / 5e4 + .34);
		return n / 4;
	}
	unsigned char GR(int i, int j) {
		return 2 * RD(i, j);
	}
	unsigned char BL(int i, int j) {
		return 4 * RD(i, j);
	}
};

struct TableClothesFlat : SolutionChar {
	unsigned char RD(int i, int j) {
		float s = 3. / (j + 99);
		return (int((i + DIM) * s + j * s) % 2 + int((DIM * 2 - i) * s + j * s) % 2) * 127;
	}
	unsigned char GR(int i, int j) {
		float s = 3. / (j + 99);
		return (int((i + DIM) * s + j * s) % 2 + int((DIM * 2 - i) * s + j * s) % 2) * 127;
	}
	unsigned char BL(int i, int j) {
		float s = 3. / (j + 99);
		return (int((i + DIM) * s + j * s) % 2 + int((DIM * 2 - i) * s + j * s) % 2) * 127;
	}
};

struct TableClothesRipple : SolutionChar {
	unsigned char RD(int i, int j) {
		float s = 3. / (j + 99);
		float y = (j + sin((i * i + _sq(j - 700) * 5) / 100. / DIM) * 35) * s;
		return (int((i + DIM) * s + y) % 2 + int((DIM * 2 - i) * s + y) % 2) * 127;
	}
	unsigned char GR(int i, int j) {
		float s = 3. / (j + 99);
		float y = (j + sin((i * i + _sq(j - 700) * 5) / 100. / DIM) * 35) * s;
		return (int((i + DIM) * s + y) % 2 + int((DIM * 2 - i) * s + y) % 2) * 127;
	}
	unsigned char BL(int i, int j) {
		float s = 3. / (j + 99);
		float y = (j + sin((i * i + _sq(j - 700) * 5) / 100. / DIM) * 35) * s;
		return (int((i + DIM) * s + y) % 2 + int((DIM * 2 - i) * s + y) % 2) * 127;
	}
};

struct TableClothesColour : SolutionChar {
	unsigned char RD(int i, int j) {
		float s = 3. / (j + 99);
		float y = (j + sin((i * i + _sq(j - 700) * 5) / 100. / DIM) * 35) * s;
		return (int((i + DIM) * s + y) % 2 + int((DIM * 2 - i) * s + y) % 2) * 127;
	}
	unsigned char GR(int i, int j) {
		float s = 3. / (j + 99);
		float y = (j + sin((i * i + _sq(j - 700) * 5) / 100. / DIM) * 35) * s;
		return (int(5 * ((i + DIM) * s + y)) % 2 + int(5 * ((DIM * 2 - i) * s + y)) % 2) * 127;
	}
	unsigned char BL(int i, int j) {
		float s = 3. / (j + 99);
		float y = (j + sin((i * i + _sq(j - 700) * 5) / 100. / DIM) * 35) * s;
		return (int(29 * ((i + DIM) * s + y)) % 2 + int(29 * ((DIM * 2 - i) * s + y)) % 2) * 127;
	}
};

struct TableClothesInMotion : SolutionChar {
	unsigned char RD(int i, int j) {
#define P 6.03
		float s = 3. / (j + 250), y = (j + sin((i * i + _sq(j - 700) * 5) / 100. / DIM + P) * 15) * s; return (int((i + DIM) * s + y) % 2 + int((DIM * 2 - i) * s + y) % 2) * 127;
	}

	unsigned char GR(int i, int j) {
		float s = 3. / (j + 250);
		float y = (j + sin((i * i + _sq(j - 700) * 5) / 100. / DIM + P) * 15) * s;
		return (int(5 * ((i + DIM) * s + y)) % 2 + int(5 * ((DIM * 2 - i) * s + y)) % 2) * 127;
	}

	unsigned char BL(int i, int j) {
		float s = 3. / (j + 250);
		float y = (j + sin((i * i + _sq(j - 700) * 5) / 100. / DIM + P) * 15) * s;
		return (int(29 * ((i + DIM) * s + y)) % 2 + int(29 * ((DIM * 2 - i) * s + y)) % 2) * 127;
	}
#undef P
};

struct RandomPainter : SolutionChar {
	unsigned char RD(int i, int j) {
#define r(n)(rand()%n)
		static char c[1024][1024]; return!c[i][j] ? c[i][j] = !r(999) ? r(256) : RD((i + r(2)) % 1024, (j + r(2)) % 1024) : c[i][j];
	}
	unsigned char GR(int i, int j) {
		static char c[1024][1024]; return!c[i][j] ? c[i][j] = !r(999) ? r(256) : GR((i + r(2)) % 1024, (j + r(2)) % 1024) : c[i][j];
	}
	unsigned char BL(int i, int j) {
		static char c[1024][1024]; return!c[i][j] ? c[i][j] = !r(999) ? r(256) : BL((i + r(2)) % 1024, (j + r(2)) % 1024) : c[i][j];
	}
#undef r
};

struct RandomPainterIsotropic : SolutionChar {
	unsigned char RD(int i, int j) {
#define r(n)(rand()%n)
		static char c[1024][1024]; return!c[i][j] ? c[i][j] = r(999) ? RD((i + r(5) + 1022) % 1024, (j + r(5) + 1022) % 1024) : r(256) : c[i][j];
	}
	unsigned char GR(int i, int j) {
		static char c[1024][1024]; return!c[i][j] ? c[i][j] = r(999) ? GR((i + r(5) + 1022) % 1024, (j + r(5) + 1022) % 1024) : r(256) : c[i][j];
	}
	unsigned char BL(int i, int j) {
		static char c[1024][1024]; return!c[i][j] ? c[i][j] = r(999) ? BL((i + r(5) + 1022) % 1024, (j + r(5) + 1022) % 1024) : r(256) : c[i][j];
	}
#undef r
};

struct SwirlyPointyThing : SolutionShort {
	unsigned short RD(int i, int j) {
		return(sqrt(_sq(73. - i) + _sq(609 - j)) + 1) / (sqrt(abs(sin((sqrt(_sq(860. - i) + _sq(162 - j))) / 115.0))) + 1) / 200;
	}
	unsigned short GR(int i, int j) {
		return(sqrt(_sq(160. - i) + _sq(60 - j)) + 1) / (sqrt(abs(sin((sqrt(_sq(86. - i) + _sq(860 - j))) / 115.0))) + 1) / 200;
	}
	unsigned short BL(int i, int j) {
		return(sqrt(_sq(844. - i) + _sq(200 - j)) + 1) / (sqrt(abs(sin((sqrt(_sq(250. - i) + _sq(20 - j))) / 115.0))) + 1) / 200;
	}
};

struct SwirlyPointyThing2 : SolutionShort {
	unsigned short RD(int i, int j) {
		return(sqrt(_sq(148. - i) + _sq(1000 - j)) + 1) / (sqrt(abs(sin((sqrt(_sq(500. - i) + _sq(400 - j))) / 115.0))) + 1) / 200;
	}
	unsigned short GR(int i, int j) {
		return(sqrt(_sq(610. - i) + _sq(60 - j)) + 1) / (sqrt(abs(sin((sqrt(_sq(864. - i) + _sq(860 - j))) / 115.0))) + 1) / 200;
	}
	unsigned short BL(int i, int j) {
		return(sqrt(_sq(180. - i) + _sq(100 - j)) + 1) / (sqrt(abs(sin((sqrt(_sq(503. - i) + _sq(103 - j))) / 115.0))) + 1) / 200;
	}
};

struct MandelbrotSubmission : SolutionChar {
	unsigned char RD(int i, int j) {
		float x = 0, y = 0; int k;
		for (k = 0; k++ < 256;) {
			float a = x * x - y * y + (i - 768.0) / 512;
			y = 2 * x * y + (j - 512.0) / 512;
			x = a;
			if (x * x + y * y > 4)break;
		}
		return k > 31 ? 256 : k * 8;
	}
	unsigned char GR(int i, int j) {
		float x = 0, y = 0;
		int k;
		for (k = 0; k++ < 256;) {
			float a = x * x - y * y + (i - 768.0) / 512;
			y = 2 * x * y + (j - 512.0) / 512;
			x = a;
			if (x * x + y * y > 4)break;
		}
		return k > 63 ? 256 : k * 4;
	}
	unsigned char BL(int i, int j) {
		float x = 0, y = 0;
		int k;
		for (k = 0; k++ < 256;) {
			float a = x * x - y * y + (i - 768.0) / 512;
			y = 2 * x * y + (j - 512.0) / 512;
			x = a;
			if (x * x + y * y > 4)break;
		}
		return k;
	}
};

struct MandelbrotSubmission2 : SolutionChar {
	unsigned char RD(int i, int j) {
		float x = 0, y = 0; int k;
		for (k = 0; k++ < 256;) {
			float a = x * x - y * y + (i - 768.0) / 512;
			y = 2 * x * y + (j - 512.0) / 512;
			x = a;
			if (x * x + y * y > 4)break;
		}
		return log(k) * 47;
	}
	unsigned char GR(int i, int j) {
		float x = 0, y = 0;
		int k;
		for (k = 0; k++ < 256;) {
			float a = x * x - y * y + (i - 768.0) / 512;
			y = 2 * x * y + (j - 512.0) / 512;
			x = a;
			if (x * x + y * y > 4)break;
		}
		return log(k) * 47;
	}
	unsigned char BL(int i, int j) {
		float x = 0, y = 0;
		int k;
		for (k = 0; k++ < 256;) {
			float a = x * x - y * y + (i - 768.0) / 512;
			y = 2 * x * y + (j - 512.0) / 512;
			x = a;
			if (x * x + y * y > 4)break;
		}
		return 128 - log(k) * 23;
	}
};

struct MandelbrotSubmission3 : SolutionChar {
	unsigned char RD(int i, int j) {
		float x = 0, y = 0, k = 0, X, Y;
		while (k++ < 256e2 && (X = x * x) + (Y = y * y) < 4)
			y = 2 * x * y + (j - 89500) / 102400., x = X - Y + (i - 14680) / 102400.;
		return log(k) / 10.15 * 256;
	}
	unsigned char GR(int i, int j) {
		float x = 0, y = 0, k = 0, X, Y;
		while (k++ < 256e2 && (X = x * x) + (Y = y * y) < 4)
			y = 2 * x * y + (j - 89500) / 102400., x = X - Y + (i - 14680) / 102400.;
		return log(k) / 10.15 * 256;
	}
	unsigned char BL(int i, int j) {
		float x = 0, y = 0, k = 0, X, Y;
		while (k++ < 256e2 && (X = x * x) + (Y = y * y) < 4)
			y = 2 * x * y + (j - 89500) / 102400., x = X - Y + (i - 14680) / 102400.;
		return 128 - k / 200;
	}
};

struct JuliaSets : SolutionShort {
	unsigned short RD(int i, int j) {
#define D(x) (x-DIM/2.)/(DIM/2.)
		float x = D(i), y = D(j), X, Y, n = 0; while (n++ < 200 && (X = x * x) + (Y = y * y) < 4) { x = X - Y + .36237; y = 2 * x * y + .32; }return log(n) * 256;
	}

	unsigned short GR(int i, int j) {
		float x = D(i), y = D(j), X, Y, n = 0; while (n++ < 200 && (x * x + y * y) < 4) { X = x; Y = y; x = X * X - Y * Y + -.7; y = 2 * X * Y + .27015; }return log(n) * 128;
	}

	unsigned short BL(int i, int j) {
		float x = D(i), y = D(j), X, Y, n = 0; while (n++ < 600 && (x * x + y * y) < 4) { X = x; Y = y; x = X * X - Y * Y + .36237; y = 2 * X * Y + .32; }return log(n) * 128;
	}
#undef D
};

struct JuliaSets2 : SolutionShort {
	unsigned short RD(int i, int j) {
		static int n = 1; if (n) { --n; srand(time(0)); }
#define R rand()/16384.-1
#define S static float r=R,k=R;float
		return _cb(i ^ j);
	}

	unsigned short GR(int i, int j) {
#define D(x) (x-DIM/2.)/(DIM/2.),
		S x = D(i)y = D(j)X, Y; int n = 0; while (n++ < 200 && (X = x) * x + (Y = y) * y < 4) { x = X * X - Y * Y + r; y = 2 * X * Y + k; }return log(n) * 512;
	}

	unsigned short BL(int i, int j) {
		S x = D(i)y = D(j)X, Y; int n = 0; while (n++ < 200 && (X = x) * x + (Y = y) * y < 4) { x = X * X - Y * Y + r; y = 2 * X * Y + k; }return log(n) * 512;
	}
#undef R
#undef S
#undef D
};

struct ManuelKastenSolution : SolutionChar {
	unsigned char RD(int i, int j) {
		static double k; k += rand() / 1. / RAND_MAX; int l = k; l %= 512; return l > 255 ? 511 - l : l;
	}
	unsigned char GR(int i, int j) {
		static double k; k += rand() / 1. / RAND_MAX; int l = k; l %= 512; return l > 255 ? 511 - l : l;
	}
	unsigned char BL(int i, int j) {
		static double k; k += rand() / 1. / RAND_MAX; int l = k; l %= 512; return l > 255 ? 511 - l : l;
	}
};

struct FaubiSolution : SolutionShort {
	unsigned short RD(int i, int j) {
		int a = (j ? i % j : i) * 4; int b = i - 32; int c = j - 32; return _sq(abs(i - 512)) + _sq(abs(j - 512)) > _sq(384) ? a : int(sqrt((b + c) / 2)) ^ _cb((b - c) * 2);
	}
	unsigned short GR(int i, int j) {
		int a = (j ? i % j : i) * 4; return _sq(abs(i - 512)) + _sq(abs(j - 512)) > _sq(384) ? a : int(sqrt((i + j) / 2)) ^ _cb((i - j) * 2);
	}
	unsigned short BL(int i, int j) {
		int a = (j ? i % j : i) * 4; int b = i + 32; int c = j + 32; return _sq(abs(i - 512)) + _sq(abs(j - 512)) > _sq(384) ? a : int(sqrt((b + c) / 2)) ^ _cb((b - c) * 2);
	}
};

struct Buddhabrot : SolutionShort {
	unsigned short RD(int i, int j) {
#define f(a,b)for(a=0;++a<b;)
#define D float x=0,y=0
		static int z, m, n; if (!z) { z = 1; f(m, 4096)f(n, 4096)BL(m - 4096, n - 4096); }; return GR(i, j);
	}
	unsigned short GR(int i, int j) {
#define R a=x*x-y*y+i/1024.+2;y=2*x*y+j/1024.+2
		static float c[DIM][DIM], p; if (i >= 0)return(p = c[i][j]) > DM1 ? DM1 : p; c[j + DIM][i / 2 + DIM] += i % 2 * 2 + 1;
	}
	unsigned short BL(int i, int j) {
		D, a, k, p = 0; if (i < 0)f(k, 5e5) { R; x = a; if (x * x > 4 || y * y > 4)break; GR(int((x - 2) * 256) * 2 - p, (y - 2) * 256); if (!p && k == 5e5 - 1) { x = y = k = 0; p = 1; } } else { return GR(i, j); }
	}
#undef f
#undef D
#undef R
};

struct Antibuddhabrot : SolutionShort {
	unsigned short RD(int i, int j) {
#define f(a)for(a=0;++a<DIM;)
		static int z; float x = 0, y = 0, m, n, k; if (!z) { z = 1; f(m)f(n)GR(m - DIM, n - DIM); }; return BL(i, j);
	}
	unsigned short GR(int i, int j) {
		float x = 0, y = 0, a, k; if (i < 0)f(k) { a = x * x - y * y + (i + 256.0) / 512; y = 2 * x * y + (j + 512.0) / 512; x = a; if (x * x + y * y > 4)break; BL((x - .6) * 512, (y - 1) * 512); }return BL(i, j);
	}
	unsigned short BL(int i, int j) {
		static float c[DIM][DIM]; if (i<0 && i>-DIM - 1 && j<0 && j>-DIM - 1)c[j + DIM][i + DIM]++; else if (i > 0 && i < DIM && j>0 && j < DIM)return log(c[i][j]) * 110;
	}
#undef f
};

struct SierpinskiPentagon : SolutionChar
{
	unsigned char RD(int i, int j) {
#define A int x=0,y=0,p[10]={512,9,0,381,196,981,827,981,DM1,381}
		auto s = 99. / (j + 99); return GR(i, j) ? 0 : abs(53 - int((3e3 - i) * s + j * s) % 107);
	}

	unsigned char GR(int i, int j) { static int c[DIM][DIM]; if (i + j < 1) { A; for (int n = 0; n < 2e7; n++) { int v = (rand() % 11 + 1) % 5 * 2; x += p[v]; x /= 2; y += p[v + 1]; y /= 2; c[x][y]++; } }return c[i][j]; }

	unsigned char BL(int i, int j) { static int c[DIM][DIM]; if (i + j < 1) { A; for (int n = 0; n < 3e7; n++) { int v = (rand() % 11 + 4) % 5 * 2; x += p[v]; x /= 2; y += p[v + 1]; y /= 2; c[x][y]++; } }return c[i][j]; }

#undef A
};

struct SheetMusic : SolutionShort {
	unsigned short RD(int i, int j) {
		return ((int)(100 * sin((i + 400) * (j + 100) / 11115))) & i;
	}
	unsigned short GR(int i, int j) {
		return RD(i, j);
	}
	unsigned short BL(int i, int j) {
		return RD(i, j);
	}
};

struct SheetMusicFullResolution : SolutionShort {
	unsigned short RD(int i, int j) {
		return ((int)(100 * sin((i + 400) * (j + 100) / 11115.0)));
	}
	unsigned short GR(int i, int j) {
		return RD(i, j);
	}
	unsigned short BL(int i, int j) {
		return RD(i, j);
	}
};
