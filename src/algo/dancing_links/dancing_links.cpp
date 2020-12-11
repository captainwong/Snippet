// https://en.wikipedia.org/wiki/Dancing_Links
// https://www.cnblogs.com/grenet/p/3145800.html

#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <assert.h>
#include "ansi_color_codes.h"


// 给定一个由0-1组成的矩阵，是否能找到一个行的集合，使得集合中每一列都恰好包含一个1
class DancingLinks
{
public:
	struct Node;
	using PNode = Node*;
	struct Node {
		PNode left = nullptr;
		PNode right = nullptr;
		PNode up = nullptr;
		PNode down = nullptr;
		PNode header = nullptr; // column header
		int col = 0; // column index, 0 为超级节点的列
		int row = 0; // row index，0 为辅助行
		int val = 0; // 没什么意义，辅助打印
		bool covered = false; // 没什么意义，辅助打印
	};

	using Line = std::vector<PNode>;

	PNode root{}; // 超级节点
	Line headerLine{}; // 辅助行
	int rows = 0; // 行数，包括辅助行
	std::vector<int> ans{}; // 答案
	bool enable_print = false;
	std::vector<Line> lines; // 没什么意义，辅助打印
	int steps = 0; // 没什么意义，辅助打印

	explicit DancingLinks(bool enable_print = false) : enable_print(enable_print) {}
	~DancingLinks() { clear(); }

	void clear() {
		for (auto p : headerLine) {
			auto down = p->down;
			while (down && down != p) {
				auto t = down->down;
				delete down;
				down = t;
			}
			delete p;
		}
		root = nullptr;
		headerLine.clear();
		rows = 0;
		steps = 0;
		ans.clear();
		lines.clear();
	}

	//! 没他还真不好理解，感觉自己是个弱智。。。
	void print() {
		if (!enable_print) { return; }
		printf("--------------------------------------\n0  ");
		Line line1;
		auto right = root->right;
		while (right != root) {
			printf("%d  ", right->col); 
			line1.push_back(right->down);
			right = right->right;
		}
		printf("\n");
		Line line2;
		for(int row = 1; row < rows; row++){
			line2.clear();
			printf("%d  ", row);
			for (auto item : line1) {
				if (item->row == row) {					
					printf("%-2d ", item->val);
					line2.push_back(item->down);
				} else {
					line2.push_back(item);
					printf("   "); 
				}
			}
			printf("\n");
			line1 = line2;
		} 
		printf("--------------------------------------\n\n");
	}

	void print_all() {
		if (!enable_print) { return; }
		printf("--------------------------------------\n");
		printf("steps=%d, ans=[", ++steps);
		for (auto i : ans) { printf("%d,", i); }
		printf("]\n");
		printf("0  ");
		for (auto node : headerLine) {
			if (node->col == 0) { continue; }
			if (node->covered) {
				printf(RED("%d  "), node->col);
			} else {
				printf("%d  ", node->col);
			}
		}
		printf("\n");
		int row = 1;
		for (const auto& line : lines) {
			printf("%d  ", row++);
			for (auto item : line) {
				if (item) {
					if (item->covered) {
						printf(RED("%-2d "), item->val);
					} else {
						printf("%-2d ", item->val);
					}					
				} else {
					printf("   ");
				}
			}
			printf("\n");
		}
		printf("--------------------------------------\n\n");
	}

	//! 从辅助行节点 c 开始移除
	void remove(PNode c) {
		// 将辅助节点从辅助行内移除
		c->left->right = c->right;
		c->right->left = c->left;
		c->covered = true;
		auto down = c->down;
		while (down != c) { // 向下遍历同列节点
			down->covered = true;
			auto right = down->right; // 依次遍历该节点的同行节点
			while (right != down) { // 将同行节点从该节点的列内移除
				right->covered = true;
				right->up->down = right->down;
				right->down->up = right->up;
				right = right->right;
			}
			down = down->down;
		}

		// 最终效果是若从 root 开始遍历，已经不包含 c，c 的同列，以及每个 c 同列节点的同行节点
		// 但是！！！
		// 从 c 节点仍然可以遍历所有这些被移除的节点，以便恢复，妙啊
	}

	//! 从辅助行节点 c 开始恢复
	void resume(PNode c) {
		// 将辅助节点重新置入辅助行内，而位置没变，妙啊
		c->left->right = c;
		c->right->left = c;
		c->covered = false;

		// 由于 remove 是从上到下遍历同列节点并移除了每个同列节点的同行节点，
		// 这里就要按照相反的顺序从下到上恢复每一行
		auto up = c->up; // 逆向遍历同列节点
		while (up != c) {
			up->covered = false;
			auto right = up->right;
			while (right != up) { // 重新将同列节点的同行节点恢复到同行节点原本的列内，妙啊
				right->covered = false;
				right->up->down = right;
				right->down->up = right;
				right = right->right;
			}
			up = up->up;
		}
	}

	//! 递归跳动
	bool dance(int depth) {
		// 超级节点没有后继了，说明已经没有节点了，跳跃结束，算法结束
		auto c1 = root->right;
		if (c1 == root) { return true; }

		// 移除辅助节点
		remove(c1);
		print_all();

		// 遍历辅助节点的列
		for (auto down = c1->down; down != c1; down = down->down) {
			// 选择一个同列节点，将列号 row 加入答案数组
			// 这里是顺序选择同列节点，看资料应该选择该节点的同行内 1 最少的节点会更快，这里暂时没有尝试
			ans.resize(depth + 1);
			ans[depth] = down->row;

			// 依次移除同列节点的同行节点的辅助行头
			auto right = down->right;
			while (right != down) {
				remove(right->header);
				print_all();
				right = right->right;
			}

			// 递归跳动，如果选择这个 down 节点就成功了，算法就结束了
			if (dance(depth + 1)) { return true; }

			// 没有成功，需要选择其他同列节点继续尝试，但在这之前要回复之前移除的节点
			// 由于之前是从左到右移除的同行节点的辅助行头，这里就逆序回复每一个辅助行头
			auto left = down->left;
			while (left != down) {
				resume(left->header);
				print_all();
				left = left->left;
			}
		}

		// 恢复辅助节点
		resume(c1);
		print_all();
		return false;
	}


	std::vector<int> solve(std::vector<std::vector<int>>& matrix) {
		clear();

		// check input
		size_t cols = 0; // 一行的元素个数
		do {
			if (matrix.empty()) { return ans; }
			cols = matrix[0].size();
			if (cols == 0) { return ans; }
			for (size_t i = 1; i < matrix.size(); i++) {
				if (cols != matrix[i].size()) { return ans; }
			}
		} while (0);

		// init root & header
		root = new Node();
		root->left = root->right = root;
		headerLine.push_back(root);
		//lines.push_back(headerLine);
		int nodecount = 0;
		{
			auto prev = root;
			for (size_t i = 0; i < cols; i++) {
				auto node = new Node();
				node->left = prev;
				node->right = root;
				node->col = i + 1;
				node->header = node->up = node->down = node;	
				headerLine.push_back(node);
				prev->right = node;
				prev = node;
			}
			rows++;
		}

		print_all();

		// construct matrix
		{
			for (auto& mline : matrix) {
				Line line;
				PNode first = nullptr, prev = nullptr;
				for (size_t i = 0; i < mline.size(); i++) {
					if (mline[i] == 1) {
						auto node = new Node();
						node->val = ++nodecount;
						node->left = prev;
						node->row = rows;
						node->col = i + 1;

						auto header = headerLine[i + 1];
						node->header = header;
						node->down = header;
						header->up = node;
						auto bottom = header;
						while (bottom->down != header) { bottom = bottom->down; }
						bottom->down = node;
						node->up = bottom;

						if (!first) {
							first = node;
						}
						if (prev) {
							prev->right = node;
						}
						prev = node;
						line.push_back(node);
					} else {
						line.push_back(nullptr);
					}
				}
				if (prev) {
					prev->right = first;
				}				
				if (first) {
					first->left = prev;
				}
				rows++;
				lines.push_back(line);
			}
		}

		print_all();

		// solve
		dance(0);
		std::sort(ans.begin(), ans.end());
		return ans;
	}
};

int main()
{
	using namespace std;
	vector<vector<int>> matrix = {
		{0, 0, 1, 0, 1, 1, 0},
		{1, 0, 0, 1, 0, 0, 1},
		{0, 1, 1, 0, 0, 1, 0},
		{1, 0, 0, 1, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 1},
		{0, 0, 0, 1, 1, 0, 1},
	};

	vector<int> ans = { 1, 4, 5 };

	assert(DancingLinks(true).solve(matrix) == ans);
}