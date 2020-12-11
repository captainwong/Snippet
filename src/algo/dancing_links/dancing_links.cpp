// https://en.wikipedia.org/wiki/Dancing_Links
// https://www.cnblogs.com/grenet/p/3145800.html

#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <assert.h>


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
	};

	using Line = std::vector<PNode>;

	PNode root{}; // 超级节点
	int rows = 0; // 行数，包括辅助行
	std::vector<int> ans{}; // 答案
	bool enable_print = false;

	explicit DancingLinks(bool enable_print = false) : enable_print(enable_print) {}
	~DancingLinks() { clear(); }

	void clear() {
		auto p = root;
		while (p) {
			auto down = p->down;
			while (down != p) {
				auto t = down->down;
				delete down;
				down = t;
			}

			auto right = p->right;
			delete p;
			p = right;
		}
		root = nullptr;
		rows = 0;
		ans.clear();
	}

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

	//! 移除辅助行的节点c
	void remove(PNode c) {
		// 将辅助节点从辅助行内移除
		c->left->right = c->right;
		c->right->left = c->left;
		auto down = c->down;
		while (down != c) { // 向下遍历同列节点
			auto right = down->right;
			while (right != down) {
				// 将同列节点的同行节点从该节点的列内移除
				right->up->down = right->down;
				right->down->up = right->up;
				right = right->right;
			}
			down = down->down;
		}

		// 最终效果是若从 root 开始遍历，已经不包含 c，c 的同列，以及每个 c 同列节点的同行节点
		// 但是！
		// 从 c 节点仍然可以遍历所有这些被移除的节点，以便恢复，妙啊
	}

	//! 恢复辅助行的节点 c
	void resume(PNode c) {
		// 将辅助节点重新置入辅助行内，而位置没变，妙啊
		c->left->right = c;
		c->right->left = c;

		// 由于 remove 是从上到下遍历同列节点并移除了每个同列节点的行，
		// 这里就要按照相反的顺序从下到上恢复每一行
		auto up = c->up; // 逆向遍历同列节点
		while (up != c) {
			auto right = up->right;
			while (right != up) { // 重新将同列节点的同行节点恢复到同行节点原本的列内，妙啊
				right->up->down = right;
				right->down->up = right;
				right = right->right;
			}
			up = up->up;
		}
	}

	bool dance(int depth) {
		// 超级节点没有后继了，说明已经没有节点了，跳跃结束，算法结束
		auto c1 = root->right;
		if (c1 == root) { return true; }

		// 移除辅助节点
		remove(c1);
		print();

		// 遍历辅助节点的列
		for (auto down = c1->down; down != c1; down = down->down) {
			// 选择一个同列节点，将列号 row 加入答案数组
			ans.resize(depth + 1);
			ans[depth] = down->row;

			// 依次移除同列节点的同行节点的辅助行头
			auto right = down->right;
			while (right != down) {
				remove(right->header);
				print();
				right = right->right;
			}

			// 递归跳动，如果选择这个 down 节点就成功了，算法就结束了
			if (dance(depth + 1)) { return true; }

			// 没有成功，需要选择其他同列节点继续尝试，但在这之前要回复之前移除的节点
			// 由于之前是从左到右移除的同行节点的辅助行头，这里就逆序回复每一个辅助行头
			auto left = down->left;
			while (left != down) {
				resume(left->header);
				print();
				left = left->left;
			}
		}

		// 恢复辅助节点
		resume(c1);
		print();
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
		Line headerLine; headerLine.push_back(root);
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

		print();

		// construct matrix
		{
			for (auto& mline : matrix) {
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
					}
				}
				if (prev) {
					prev->right = first;
				}				
				if (first) {
					first->left = prev;
				}
				rows++;
			}
		}

		print();

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

	assert(DancingLinks().solve(matrix) == ans);
}