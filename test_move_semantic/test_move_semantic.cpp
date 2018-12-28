// test_move_semantic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>


typedef std::vector<int> vint;

void output(const vint& v) {
	for (auto m : v) {
		std::cout << m << " ";
	}
	std::cout << std::endl;
}

class test {
private:
	vint m_ = {};

public:
	test() {
		m_ = { 1,2,3,4,5 };
	}

	vint && get() {
		return std::move(m_);
	}

	void output() {
		for (auto m : m_) {
			std::cout << m << " ";
		}
		std::cout << std::endl;
	}
};

int main()
{
	test t;

	t.output();

	auto r = t.get();

	output(r);

	t.output();

	system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
