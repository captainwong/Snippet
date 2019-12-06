// test_move_semantic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

typedef std::vector<int> vint;

void output_v(const vint& v, const std::string& tag = "") {
	if (!tag.empty()) {
		std::cout << tag << std::endl;
	}

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
		std::cout << "test.m_" << std::endl;
		for (auto m : m_) {
			std::cout << m << " ";
		}
		std::cout << std::endl;
	}
};

void test_class() 
{
	test t;
	t.output();
	auto r = t.get();
	output_v(r, "test_class: r");
	t.output();
}

vint&& init_1()
{
	return std::move(vint{ 1,2,3,4,5,6 });
}

vint&& init_2()
{
	return vint{ 1, 2, 3, 4, 5 };
}

vint init3()
{
	return vint{ 1, 2, 3, 4, 5 };
}

vint test_direct_return()
{
	return init3();
}

vint&& test_return_local()
{
	auto v = init3();
	return std::move(v);
}

void test_func()
{
	auto v1 = init3();
	output_v(v1, "test_func:v1");
	
	auto v2 = test_direct_return();
	output_v(v2, "test_func:v2");

	auto v3 = test_return_local();
	output_v(v3, "test_func:v3");

}

int main()
{
	test_class();
	test_func();

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
