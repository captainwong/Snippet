#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <list>

struct Pos {
	int i = 0;
	int j = 0;
};

// 棋盘格子
struct Block {
	Pos pos{};
	bool is_queen = false;
	int being_attacked = 0; // 被多少皇后攻击着
};

struct Chessboard {
	Block block[8][8]{}; // 棋盘格子
	int queens = 0; // 已经放置的皇后数量
	int spaces = 64; // 可以放置棋子的格子数量

	Chessboard() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				block[i][j].pos.i = i;
				block[i][j].pos.j = j;
			}
		}
	}

	void clear() {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				block[i][j].is_queen = false;
				block[i][j].being_attacked = 0;
			}
		}
		queens = 0;
		spaces = 64;
	}

	void print() {
		printf("-----------------------\n");
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				if (block[i][j].is_queen) {
					printf("Q ");
				} else {
					printf("%d ", block[i][j].being_attacked);
				}
			}
			printf("\n");
		}
		printf("queens=%d, spaces=%d-------\n\n", queens, spaces);
	}
	
	// 放置皇后并更新攻击区域的被攻击数量
	bool place_queen(int i, int j) {
		if (block[i][j].is_queen || block[i][j].being_attacked) { return false; }

		// 攻击左上
		for (int ii = i - 1, jj = j - 1; ii >= 0 && jj >= 0; ii--, jj--) {
			if (block[ii][jj].being_attacked++ == 0) {
				spaces--;
			}
		}

		// 攻击右下
		for (int ii = i + 1, jj = j + 1; ii < 8 && jj < 8; ii++, jj++) {
			if (block[ii][jj].being_attacked++ == 0) {
				spaces--;
			}
		}

		// 攻击左下
		for (int ii = i - 1, jj = j + 1; ii >= 0 && jj < 8; ii--, jj++) {
			if (block[ii][jj].being_attacked++ == 0) {
				spaces--;
			}
		}

		// 攻击右上
		for (int ii = i + 1, jj = j - 1; ii < 8 && jj >= 0; ii++, jj--) {
			if (block[ii][jj].being_attacked++ == 0) {
				spaces--;
			}
		}

		// 攻击同行
		for (int ii = 0; ii < 8; ii++) {
			if (ii != i && block[ii][j].being_attacked++ == 0) {
				spaces--;
			}
		}

		// 攻击同列
		for (int jj = 0; jj < 8; jj++) {
			if (jj != j && block[i][jj].being_attacked++ == 0) {
				spaces--;
			}
		}

		// 放置皇后
		block[i][j].is_queen = true; spaces--; queens++;

		return true;
	}

	// 移除皇后
	void remove_queen(int i, int j) {
		// 取消攻击左上
		for (int ii = i - 1, jj = j - 1; ii >= 0 && jj >= 0; ii--, jj--) {
			if (--block[ii][jj].being_attacked == 0) {
				spaces++;
			}
		}

		// 取消攻击右下
		for (int ii = i + 1, jj = j + 1; ii < 8 && jj < 8; ii++, jj++) {
			if (--block[ii][jj].being_attacked == 0) {
				spaces++;
			}
		}

		// 取消攻击左下
		for (int ii = i - 1, jj = j + 1; ii >= 0 && jj < 8; ii--, jj++) {
			if (--block[ii][jj].being_attacked == 0) {
				spaces++;
			}
		}

		// 取消攻击右上
		for (int ii = i + 1, jj = j - 1; ii < 8 && jj >= 0; ii++, jj--) {
			if (--block[ii][jj].being_attacked == 0) {
				spaces++;
			}
		}

		// 取消攻击同行
		for (int ii = 0; ii < 8; ii++) {
			if (ii != i && --block[ii][j].being_attacked == 0) {
				spaces++;
			}
		}

		// 取消攻击同列
		for (int jj = 0; jj < 8; jj++) {
			if (jj != j && --block[i][jj].being_attacked == 0) {
				spaces++;
			}
		}

		// 取消放置皇后
		block[i][j].is_queen = false; spaces++; queens--;
	}

	// 找到下一个可以放置棋子的位置
	bool find_next_space(int i, int j, Pos& pos) {
		if (spaces < 1) { return false; }
		pos.i = i+1, pos.j = j;
		for (; pos.j < 8; pos.j++) {
			for (; pos.i < 8; pos.i++) {
				if (!block[pos.i][pos.j].is_queen && !block[pos.i][pos.j].being_attacked) { return true; }
			}
			pos.i = 0;
		}
		return false;
	}

	// 能否在第一个 Queen 放在 i, j 的情况下解决
	int can_solve(int i, int j) {
		//printf("trying place first queen on %d, %d\n", i, j);
		clear();
		place_queen(i, j);
		//print();
		
		std::list<Pos> qs; // 当前的试探序列
		qs.push_back(block[i][j].pos);

		int count = 0;
		Pos lastQ = qs.back();
		while (1) {
			Pos pos;
			if ((spaces >= 8 - (int)qs.size()) && find_next_space(lastQ.i, lastQ.j, pos)) { // 还有位置可以放置棋子
				lastQ = pos;

				if (place_queen(pos.i, pos.j)) { // 放置成功					
					qs.push_back(lastQ);
					if (queens == 2) {
						//print();
					}
				} else { // 放置失败，继续试探
					continue;
				}
			} else if (qs.size() > 1) { // 没有位置了，放置失败，尝试回溯
				lastQ = qs.back(); // 从上一个皇后再次尝试
				remove_queen(lastQ.i, lastQ.j); qs.pop_back(); // 移除上一个皇后
				if (queens == 1) {
					//print();
				}
				
			} else { // 已经回溯到第一个皇后了，失败
				break;
			}

			if (qs.size() == 8) { // 找到了一种解决方案，再次尝试回溯以找到更多解决方案
				printf("********** can place first queen on %d, %d, #%d solution !!!!!!!!!!!!!!\n", i, j, ++count);
				print();
				lastQ = qs.back(); // 从上一个皇后再次尝试
				remove_queen(lastQ.i, lastQ.j); qs.pop_back(); // 移除上一个皇后
			}
		}

		if (count == 0) {
			printf("xxxxxxxxxx can not place first queen on %d, %d\n", i, j);
		}
		
		return count;
	}

	void solve() {
		int count = 0;
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				count += can_solve(i, j);
			}
		}

		printf("%d solutions\n", count);
	}
};

int main()
{
	Chessboard().solve();
}
