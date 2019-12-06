#pragma once

#define _CRT_SECURE_NO_WARNINGS

// https://codegolf.stackexchange.com/questions/35569/tweetable-mathematical-art
// https://pastebin.com/uQkCQGhz

// NOTE: compile with g++ filename.cpp -std=c++11

#include <iostream>
#include <cmath>

#define DIM 1024
#define DM1 (DIM-1)
#define _sq(x) ((x)*(x))                           // square
#define _cb(x) abs((x)*(x)*(x))                    // absolute value of cube
#define _cr(x) (unsigned short)(pow((x),1.0/3.0))  // cube root

struct Solution {
	virtual void write(FILE* fp, int i, int j) = 0;
};

struct SolutionShort : Solution {
	virtual unsigned short RD(int i, int j) = 0;
	virtual unsigned short GR(int i, int j) = 0;
	virtual unsigned short BL(int i, int j) = 0;

	virtual void write(FILE* fp, int i, int j) {
		static unsigned short color[3];
		color[0] = RD(i, j) & DM1;
		color[1] = GR(i, j) & DM1;
		color[2] = BL(i, j) & DM1;
		fwrite(color, 2, 3, fp);
	}
};

struct SolutionChar : Solution
{
	virtual unsigned char RD(int i, int j) = 0;
	virtual unsigned char GR(int i, int j) = 0;
	virtual unsigned char BL(int i, int j) = 0;

	virtual void write(FILE* fp, int i, int j) {
		static unsigned char color[3];
		color[0] = RD(i, j) & 255;
		color[1] = GR(i, j) & 255;
		color[2] = BL(i, j) & 255;
		fwrite(color, 1, 3, fp);
	}
};

void run(const char* name, Solution* solution)
{
	FILE* fp;
	fp = fopen(name, "wb");
	fprintf(fp, "P6\n%d %d\n1023\n", DIM, DIM);
	for (int j = 0; j < DIM; j++)
		for (int i = 0; i < DIM; i++)
			solution->write(fp, i, j);
	fclose(fp);
	delete solution;
}

#define Run(name) printf("Running %s\n", #name##"..."); run(#name##".ppm", new name); printf("Done!\n\n");
