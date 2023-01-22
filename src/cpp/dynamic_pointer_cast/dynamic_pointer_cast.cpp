// dynamic_pointer_cast.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <memory>
#include <cstdlib>
using namespace std;

class base {
public:
	virtual void make_me_polymorphic() = 0;
};

class derived : public base {
public:
	virtual void make_me_polymorphic() override {
		std::cout << "derived::make_me_polymorphic" << std::endl;
	}
};

typedef std::shared_ptr<base> baseptr;
typedef std::shared_ptr<derived> derivedptr;

int main()
{
	baseptr b = std::make_shared<derived>();
	derivedptr d = std::dynamic_pointer_cast<derived>(b);
	d->make_me_polymorphic();
	system("pause");
    return 0;
}

