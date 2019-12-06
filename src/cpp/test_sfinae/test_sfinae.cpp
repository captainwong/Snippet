// https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error
/*
Substitution failure is not an error (SFINAE) refers to a situation in C++
where an invalid substitution of template parameters is not in itself an error.
David Vandevoorde first introduced the acronym SFINAE to describe related programming techniques.
*/

#include <stdio.h>
#include <iostream>
#include <type_traits> // cpp11_simplification


namespace basic_test
{
struct Test {
	typedef int foo;
};

template <typename T>
void f(typename T::foo) {
	printf("f #1\n");
}  // Definition #1

template <typename T>
void f(T) {
	printf("f #2\n");
}  // Definition #2

void test() {
	printf("-----------basic test-----------\n");
	f<Test>(10);  // Call #1.
	f<int>(10);   // Call #2. Without error (even though there is no int::foo)
				  // thanks to SFINAE.
}

}


// determine if a type contains a certain typedef
namespace advanced_test
{

template <typename T>
struct has_typedef_foobar {
	// Types "yes" and "no" are guaranteed to have different sizes,
	// specifically sizeof(yes) == 1 and sizeof(no) == 2.
	typedef char yes[1];
	typedef char no[2];

	template <typename C>
	static yes& test(typename C::foobar*);

	template <typename>
	static no& test(...);

	// If the "sizeof" of the result of calling test<T>(nullptr) is equal to
	// sizeof(yes), the first overload worked and T has a nested type named
	// foobar.
	static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

struct foo {
	typedef float foobar;
};

void test() {
	printf("-----------advanced test-----------\n");
	std::cout << std::boolalpha;
	std::cout << has_typedef_foobar<int>::value << std::endl;  // false
	std::cout << has_typedef_foobar<foo>::value << std::endl;  // true
}

}


namespace cpp11_simplification
{

template <typename... Ts>
using void_t = void;

template <typename T, typename = void>
struct has_typedef_foobar : std::false_type {};

template <typename T>
struct has_typedef_foobar<T, void_t<typename T::foobar>> : std::true_type {};

struct foo {
	using foobar = float;
};

void test() {
	printf("-----------cpp11_simplification test-----------\n");
	std::cout << std::boolalpha;
	std::cout << has_typedef_foobar<int>::value << std::endl; // false
	std::cout << has_typedef_foobar<foo>::value << std::endl; // true
}

}

//
//namespace library_fundamental_v2_n4562
//{
//
//template <typename T>
//using has_typedef_foobar_t = typename T::foobar;
//
//struct foo {
//	using foobar = float;
//};
//
//void test() {
//	std::cout << std::boolalpha;
//	std::cout << std::is_detected<has_typedef_foobar_t, int>::value << std::endl;
//	std::cout << std::is_detected<has_typedef_foobar_t, foo>::value << std::endl;
//}
//
//}


namespace determin_member_exists
{

// limitation: cannot specify what the type of x is.
template <typename T>
struct has_x
{
	struct Fallback { int x; };
	struct Derived : T, Fallback {};
	template <typename U, U> struct Check;

	template <typename C> static char test(Check<int Fallback::*, &C::x>*);
	template <typename C> static int test(...);

	static constexpr bool value = sizeof(test<Derived>(0)) == sizeof(int);
};

struct A {
	int x;
};

struct B {

};

struct C {
	double x;
};

void test()
{
	printf("-----------determin_member_exists test-----------\n");
	std::cout << std::boolalpha;
	std::cout << has_x<A>::value << std::endl; // true
	std::cout << has_x<B>::value << std::endl; // false
	std::cout << has_x<C>::value << std::endl; // true
}

}


namespace determin_member_exists2
{

// limitation: cannot specify what the type of x is
template <typename T>
struct has_x
{
	struct Fallback { char* x; };
	struct Derived : T, Fallback {};
	template <typename U, U> struct Check;

	template <typename C> static char test(Check<char* Fallback::*, &C::x>*);
	template <typename C> static int test(...);

	static constexpr bool value = sizeof(test<Derived>(0)) == sizeof(int);
};

struct A {
	int x;
};

struct B {
	char* c;
};

struct C {
	double x;
};

void test()
{
	printf("-----------determin_member_exists2 test-----------\n");
	std::cout << std::boolalpha;
	std::cout << has_x<A>::value << std::endl; // true
	std::cout << has_x<B>::value << std::endl; // false
	std::cout << has_x<C>::value << std::endl; // true
}

}


namespace determin_member_exists3
{

// limitation: cannot specify what the type of x is
template <typename T>
struct has_x_y
{
	struct Fallback { int x; int y; };
	struct Derived : T, Fallback {};

	template <typename U, U>
	struct Check;

	template <typename C>
	static char test(Check<int Fallback::*, &C::x>*, Check<int Fallback::*, &C::y>*);

	template <typename C>
	static int test(...);

	static constexpr bool value = sizeof(test<Derived>(0, 0)) == sizeof(int);
};

struct A {
	int x;
	int y;
};

struct B {
	char* x;
};

struct C {
	float y;
};

void test()
{
	printf("-----------determin_member_exists3 test-----------\n");
	std::cout << std::boolalpha;
	std::cout << has_x_y<A>::value << std::endl; // true
	std::cout << has_x_y<B>::value << std::endl; // true, won't work
	std::cout << has_x_y<C>::value << std::endl; // true, won't work
}


}

namespace determin_member_exists4
{


// limitation: cannot specify what the type of x is
template <typename T>
struct has_x_y
{
	struct Fallback { int x; int y; };
	struct Derived : T, Fallback {};

	template <typename U, U>
	struct Check;

	template <typename C>
	static char test_x(Check<int Fallback::*, &C::x>*);

	template <typename C>
	static int test_x(...);

	template <typename C>
	static char test_y(Check<int Fallback::*, &C::y>*);

	template <typename C>
	static int test_y(...);

	static constexpr bool value = sizeof(test_x<Derived>(0)) == sizeof(int) && sizeof(test_y<Derived>(0)) == sizeof(int);
};

struct A {
	int x;
	int y;
};

struct B {
	char* x;
};

struct C {
	float y;
};

void test()
{
	printf("-----------determin_member_exists4 test-----------\n");
	std::cout << std::boolalpha;
	std::cout << has_x_y<A>::value << std::endl; // true
	std::cout << has_x_y<B>::value << std::endl; // false
	std::cout << has_x_y<C>::value << std::endl; // false
}
}


namespace determin_member_exists5
{

// limitation: cannot specify what the type of x is

// https://stackoverflow.com/questions/1005476/how-to-detect-whether-there-is-a-specific-member-variable-in-class/16000226#16000226

template <typename T, typename = int>
struct HasX : std::false_type { };

template <typename T>
struct HasX <T, decltype((void)T::x, 0)> : std::true_type { };

struct A {
	int x;
	int y;
};

struct B {
	char* x;
};

struct C {
	float y;
};

void test()
{
	printf("-----------determin_member_exists5 test-----------\n");
	std::cout << std::boolalpha;
	std::cout << HasX<A>::value << std::endl; // true
	std::cout << HasX<B>::value << std::endl; // true
	std::cout << HasX<C>::value << std::endl; // false
}
}

namespace determin_member_exists6
{

// limitation: cannot specify what the type of x is

// https://stackoverflow.com/questions/1005476/how-to-detect-whether-there-is-a-specific-member-variable-in-class/16000226#16000226

template <typename T, typename = int>
struct HasXY : std::false_type { };

template <typename T>
struct HasXY <T, decltype((void)T::x, (void)T::y, 0)> : std::true_type { };

struct A {
	int x;
	int y;
};

struct B {
	char* x;
};

struct C {
	float y;
};

void test()
{
	printf("-----------determin_member_exists6 test-----------\n");
	std::cout << std::boolalpha;
	std::cout << HasXY<A>::value << std::endl; // true
	std::cout << HasXY<B>::value << std::endl; // false
	std::cout << HasXY<C>::value << std::endl; // false
}
}

// Is it possible to write a template to check for a function's existence?
// https://stackoverflow.com/questions/257288/is-it-possible-to-write-a-template-to-check-for-a-functions-existence/264088#264088
// due to https://stackoverflow.com/questions/1966362/sfinae-to-check-for-inherited-member-functions
// won't work for inherited member functions.
namespace determin_member_function_exists
{

struct Hello
{
	int helloworld() { return 0; }
};

struct Generic {};

// SFINAE test
template <typename T>
class has_helloworld
{
	template <typename C> static char test(decltype(&C::helloworld));
	template <typename C> static int test(...);

public:
	static const bool value = sizeof(test<T>(nullptr)) == sizeof(char);
};

void test()
{
	printf("-----------determin_member_function_exists test-----------\n");
	std::cout << std::boolalpha;
	std::cout << has_helloworld<Hello>::value << std::endl; // true
	std::cout << has_helloworld<Generic>::value << std::endl; // false
	std::cout << has_helloworld<bool>::value << std::endl; // false
}

}



int main()
{
	basic_test::test();
	advanced_test::test();
	cpp11_simplification::test();


	determin_member_exists::test();
	determin_member_exists2::test();
	determin_member_exists3::test();
	determin_member_exists4::test();
	determin_member_exists5::test();
	determin_member_exists6::test();

	determin_member_function_exists::test();

}