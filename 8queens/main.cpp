#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <deque>
#include <sstream>

#define WIN32
#include "G:/dev_libs/jlib/jlib/log2.h"

using namespace jlib;

//#undef JLOG_INFO
//#define JLOG_INFO(...)

bool chessboard[8][8] = { false };

void clear()
{
	memset(chessboard, false, sizeof(chessboard));
}

void print()
{
	JLOG_INFO("printing chessboard");
	std::stringstream ss;
	ss << std::endl;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			ss << (chessboard[i][j] ? '1' : '0') << ' ';
		}
		ss << std::endl;
	}
	ss << "-----------------------" << std::endl << std::endl;
	JLOG_INFO(ss.str());
}

bool test(int row, int col)
{
	if (chessboard[row][col])return false;
	// same collumn
	for (int r = 0; r < 8; r++) {
		if (r != row && chessboard[r][col]) { return false; }
	}

	// same row
	for (int c = 0; c < 8; c++) {
		if (c != col && chessboard[row][c]) { return false; }
	}

	// left-down to right-up
	// left-down
	for (int c = col - 1, r = row + 1; c >= 0 && r < 8; c--, r++) {
		if (chessboard[r][c]) { return false; }
	}
	// right-up
	for (int c = col + 1, r = row - 1; c < 8 && r >= 0; c++, r--) {
		if (chessboard[r][c]) { return false; }
	}

	// left-up to right-down
	// left-up
	for (int c = col - 1, r = row - 1; c >= 0 && r >= 0; c--, r--) {
		if (chessboard[r][c]) { return false; }
	}
	// right-down
	for (int c = col + 1, r = row + 1; c < 8 && r < 8; c++, r++) {
		if (chessboard[r][c]) { return false; }
	}

	return true;
}

typedef std::pair<int, int> position;

void print(std::deque<position>& queen_poses)
{
	std::stringstream ss;
	ss << "other queens are: " << std::endl;
	for (auto q : queen_poses) {
		ss << "[" << q.first << "," << q.second << "] ";
	}
	ss << std::endl;
	JLOG_INFO(ss.str());
	print();
}

bool search(std::deque<position>& queen_poses, int queens, int row, int col)
{
	bool searched = false;
	for (int r = row, c = col; (r * 8) + c < 64; ) {
		if (++c == 8) {
			c = 0;
			if (++r == 8) {
				break;
			}
		}

		if (test(r, c)) {
			chessboard[r][c] = true;
			queens++;
			//std::cout << "queens:" << queens << std::endl;
			//print();
			searched = true;

			queen_poses.emplace_back(r, c);
			JLOG_INFO("add {},{}", r, c);
			print(queen_poses);

			if (queens == 8) {
				return true;
			} else {
				return search(queen_poses, queens, r, c);
			}
		}
	}

	if (!searched && !queen_poses.empty()) {
		auto q = queen_poses.back();
		queen_poses.pop_back();
		queens--;
		chessboard[q.first][q.second] = false;
		JLOG_INFO("remove {},{}", q.first, q.second);
		print(queen_poses);
		return search(queen_poses, queens, q.first, q.second);
	}

	return false;
}

int main()
{
	{
		jlib::init_logger(L"8queens");
	}

	int total = 0;
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			clear();
			int queens = 0;
			int try_times = 0;
			std::cout << "try first queen on [" << r << "," << c << "]" << std::endl;
			chessboard[r][c] = true;
			queens++; 
			print();

			std::deque<position> queen_poses;
				
			if (search(queen_poses, queens, r, c)) {
				print(queen_poses);
				total++;
			}

			break;
		}

		break;
	}

	std::cout << "total: " << total << std::endl;

	std::system("pause");
}
