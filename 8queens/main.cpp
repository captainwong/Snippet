#include <iostream>

bool chessboard[8][8] = { false };

void clear()
{
	memset(chessboard, false, sizeof(chessboard));
}

void print()
{
	std::cout << "printing chessboard" << std::endl;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			std::cout << (chessboard[i][j] ? '1' : '0') << ' ';
		}
		std::cout << std::endl;
	}
	std::cout << "-----------------------" << std::endl << std::endl;
}

bool test(int row, int col)
{
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
	for (int c = col - 1, r = row + 1; c >= 0, r < 8; c--, r++) {
		if (chessboard[r][c]) { return false; }
	}
	// right-up
	for (int c = col + 1, r = row - 1; c < 8, r >= 0; c++, r--) {
		if (chessboard[r][c]) { return false; }
	}

	// left-up to right-down
	// left-up
	for (int c = col - 1, r = row - 1; c >= 0, r >= 0; c--, r--) {
		if (chessboard[r][c]) { return false; }
	}
	// right-down
	for (int c = col + 1, r = row + 1; c < 8, r < 8; c++, r++) {
		if (chessboard[r][c]) { return false; }
	}

	return true;
}

int main()
{
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
			for (int rr = 0; rr < 8; rr++) {
				if (rr == r) { continue; }
				for (int cc = 0; cc < 8; cc++) {
					if (cc == c) { continue; }
				
					if (test(rr, cc)) {
						chessboard[rr][cc] = true;
						queens++;

						std::cout << "queens:" << queens << std::endl;
						print();
						break;
					}
				}
			}
			
			if (queens == 8) {
				total++;
			}
		}
	}

	std::cout << "total: " << total << std::endl;

	std::system("pause");
}
