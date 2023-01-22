// lambda.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
using namespace std;

void testLambda()
{
	int id = 0;
	auto f = [id]() mutable {std::cout << "id: " << id << std::endl; ++id; };
	id = 42;
	f();
	f();
	f();
	std::cout << id << std::endl;
	f();
	std::cout << id << std::endl;
}


int main()
{
	testLambda();
	system("pause");
    return 0;
}

