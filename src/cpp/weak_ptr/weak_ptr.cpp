// weak_ptr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <memory>
using namespace std;


namespace cannot_release_memory {
class a;
class b;

class a {
public:
	a() { std::cout << "a ctor" << std::endl; }
	~a() { cout << "a dtor" << endl; }
	std::shared_ptr<b> _b;
};

class b {
public:
	b() { std::cout << "b ctor" << std::endl; }
	~b() { cout << "b dtor" << endl; }
	std::shared_ptr<a> _a;
};


void test() {
	std::cout << "cannot_release_memory" << std::endl;
	auto aa = std::make_shared<a>();
	auto bb = std::make_shared<b>();
	aa->_b = bb;
	bb->_a = aa;
}
}


namespace can_release_memory {
class a;
class b;

class a {
public:
	a() { std::cout << "a ctor" << std::endl; }
	~a() { cout << "a dtor" << endl; }
	std::weak_ptr<b> _b;
};

class b {
public:
	b() { std::cout << "b ctor" << std::endl; }
	~b() { cout << "b dtor" << endl; }
	std::weak_ptr<a> _a;
};


void test() {
	std::cout << "can_release_memory" << std::endl;
	auto aa = std::make_shared<a>();
	auto bb = std::make_shared<b>();
	aa->_b = bb;
	bb->_a = aa;
}
}



int main()
{
	cannot_release_memory::test();
	can_release_memory::test();

    return 0;
}

