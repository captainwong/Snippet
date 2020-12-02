#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <list>

struct Pos {
	int i = 0;
	int j = 0;
};

// ���̸���
struct Block {
	Pos pos{};
	bool is_queen = false;
	int being_attacked = 0; // �����ٻʺ󹥻���
};

struct Chessboard {
	Block block[8][8]{}; // ���̸���
	int queens = 0; // �Ѿ����õĻʺ�����
	int spaces = 64; // ���Է������ӵĸ�������

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
	
	// ���ûʺ󲢸��¹�������ı���������
	bool place_queen(int i, int j) {
		if (block[i][j].is_queen || block[i][j].being_attacked) { return false; }

		// ��������
		for (int ii = i - 1, jj = j - 1; ii >= 0 && jj >= 0; ii--, jj--) {
			if (block[ii][jj].being_attacked++ == 0) {
				spaces--;
			}
		}

		// ��������
		for (int ii = i + 1, jj = j + 1; ii < 8 && jj < 8; ii++, jj++) {
			if (block[ii][jj].being_attacked++ == 0) {
				spaces--;
			}
		}

		// ��������
		for (int ii = i - 1, jj = j + 1; ii >= 0 && jj < 8; ii--, jj++) {
			if (block[ii][jj].being_attacked++ == 0) {
				spaces--;
			}
		}

		// ��������
		for (int ii = i + 1, jj = j - 1; ii < 8 && jj >= 0; ii++, jj--) {
			if (block[ii][jj].being_attacked++ == 0) {
				spaces--;
			}
		}

		// ����ͬ��
		for (int ii = 0; ii < 8; ii++) {
			if (ii != i && block[ii][j].being_attacked++ == 0) {
				spaces--;
			}
		}

		// ����ͬ��
		for (int jj = 0; jj < 8; jj++) {
			if (jj != j && block[i][jj].being_attacked++ == 0) {
				spaces--;
			}
		}

		// ���ûʺ�
		block[i][j].is_queen = true; spaces--; queens++;

		return true;
	}

	// �Ƴ��ʺ�
	void remove_queen(int i, int j) {
		// ȡ����������
		for (int ii = i - 1, jj = j - 1; ii >= 0 && jj >= 0; ii--, jj--) {
			if (--block[ii][jj].being_attacked == 0) {
				spaces++;
			}
		}

		// ȡ����������
		for (int ii = i + 1, jj = j + 1; ii < 8 && jj < 8; ii++, jj++) {
			if (--block[ii][jj].being_attacked == 0) {
				spaces++;
			}
		}

		// ȡ����������
		for (int ii = i - 1, jj = j + 1; ii >= 0 && jj < 8; ii--, jj++) {
			if (--block[ii][jj].being_attacked == 0) {
				spaces++;
			}
		}

		// ȡ����������
		for (int ii = i + 1, jj = j - 1; ii < 8 && jj >= 0; ii++, jj--) {
			if (--block[ii][jj].being_attacked == 0) {
				spaces++;
			}
		}

		// ȡ������ͬ��
		for (int ii = 0; ii < 8; ii++) {
			if (ii != i && --block[ii][j].being_attacked == 0) {
				spaces++;
			}
		}

		// ȡ������ͬ��
		for (int jj = 0; jj < 8; jj++) {
			if (jj != j && --block[i][jj].being_attacked == 0) {
				spaces++;
			}
		}

		// ȡ�����ûʺ�
		block[i][j].is_queen = false; spaces++; queens--;
	}

	// �ҵ���һ�����Է������ӵ�λ��
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

	// �ܷ��ڵ�һ�� Queen ���� i, j ������½��
	int can_solve(int i, int j) {
		//printf("trying place first queen on %d, %d\n", i, j);
		clear();
		place_queen(i, j);
		//print();
		
		std::list<Pos> qs; // ��ǰ����̽����
		qs.push_back(block[i][j].pos);

		int count = 0;
		Pos lastQ = qs.back();
		while (1) {
			Pos pos;
			if ((spaces >= 8 - (int)qs.size()) && find_next_space(lastQ.i, lastQ.j, pos)) { // ����λ�ÿ��Է�������
				lastQ = pos;

				if (place_queen(pos.i, pos.j)) { // ���óɹ�					
					qs.push_back(lastQ);
					if (queens == 2) {
						//print();
					}
				} else { // ����ʧ�ܣ�������̽
					continue;
				}
			} else if (qs.size() > 1) { // û��λ���ˣ�����ʧ�ܣ����Ի���
				lastQ = qs.back(); // ����һ���ʺ��ٴγ���
				remove_queen(lastQ.i, lastQ.j); qs.pop_back(); // �Ƴ���һ���ʺ�
				if (queens == 1) {
					//print();
				}
				
			} else { // �Ѿ����ݵ���һ���ʺ��ˣ�ʧ��
				break;
			}

			if (qs.size() == 8) { // �ҵ���һ�ֽ���������ٴγ��Ի������ҵ�����������
				printf("********** can place first queen on %d, %d, #%d solution !!!!!!!!!!!!!!\n", i, j, ++count);
				print();
				lastQ = qs.back(); // ����һ���ʺ��ٴγ���
				remove_queen(lastQ.i, lastQ.j); qs.pop_back(); // �Ƴ���һ���ʺ�
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
