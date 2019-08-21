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
	std::cout << has_typedef_foobar<int>::value << std::endl;  // Prints false
	std::cout << has_typedef_foobar<foo>::value << std::endl;  // Prints true
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
	std::cout << has_typedef_foobar<int>::value << std::endl;
	std::cout << has_typedef_foobar<foo>::value << std::endl;
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




// Is it possible to write a template to check for a function's existence?
// https://stackoverflow.com/questions/257288/is-it-possible-to-write-a-template-to-check-for-a-functions-existence/264088#264088
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
	typedef char yes[1];
	typedef char no[2];

	template <typename C> static yes& test(decltype(&C::helloworld));
	template <typename C> static no& test(...);

public:
	static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

void test()
{
	printf("-----------determin_member_function_exists test-----------\n");
	std::cout << std::boolalpha;
	std::cout << has_helloworld<Hello>::value << std::endl;
	std::cout << has_helloworld<Generic>::value << std::endl;
}

}



int main()
{
	basic_test::test();
	advanced_test::test();
	cpp11_simplification::test();
	determin_member_function_exists::test();

}