// inspiration https://github.com/eatnumber1/goal/blob/master/README.md
//
// Rules
//
// 1. You are encouraged to break the rules, cleverly.
// 2. When executed, the solution must print "goal" with sufficient o's to demonstrate the program's functionality.
// 3. The code g()('al') must appear in the source.
// 4. g()('al') must not be a string literal.
// 5. 'al' must be a string, or your language's equivalent thereof. You may use your language's standard method of creating a string(e.x.C should use ", ruby may use either " or ').
// 6. g()('al') must be a valid rvalue if applicable in your language.
// 7. g()('al') may not print the string.If returning a string cannot be done in your language, you should submit rationale as to why this is impossible for a solution which prints a string to be accepted.
// 8. You must be able to insert an arbitrary number of() calls without modification to your solution.Therefore solutions like this are incorrect.
// 9. g('al') must return "gal".
//
//

#include <iostream>
#include <string>
#include <cstdlib>
#include "../timer/timer.h"

using namespace jlib;



namespace my_solution {
// this is wrong, class g itself should not print the result.
class g {

	std::string pre_ = "go";

public:

	g(std::string s) {
		std::cout << "g" << s << std::endl;
	}

	g() {}

	g& operator()(std::string str) {
		pre_ += str;
		std::cout << pre_ << std::endl;

		return *this;
	}

	g& operator()() {
		pre_ += "o";
		return *this;
	}
};


void test_goal()
{
	std::cout << "my solution:" << std::endl;
	auto_timer t;
	g("al");
	g()("al");
	g()()("al");
}

};




namespace derrickturk_solution {
// https://github.com/eatnumber1/goal/blob/master/solved/c%2B%2B/derrickturk/goal.cpp

template<unsigned N = 0>
struct repeater
{
	const std::string init_;

	repeater(const std::string& init) : init_(init){}

	std::string operator()(const std::string& terminal) const {
		auto res = init_;
		for (unsigned i = 0; i < N; i++) {
			res += "o";
		}
		return res + terminal;
	}

	repeater<N + 1> operator()() const {
		return repeater<N + 1>(init_);
	}
};

void test_goal()
{
	std::cout << "derrickturk solution:" << std::endl;
	auto_timer t;
	auto g = repeater<>("g");
	std::cout << g("al") << std::endl;
	std::cout << g()("al") << std::endl;
	std::cout << g()()("al") << std::endl;
}


}


namespace my_solution_2 {

class goal {

	std::string pre_ = "g";

public:

	goal() {}

	std::string operator()(std::string str) {
		auto res = pre_ + str;
		pre_ = "g";
		return res;
	}

	goal& operator()() {
		pre_ += "o";
		return *this;
	}
};


void test_goal()
{
	std::cout << "my solution2:" << std::endl;
	auto_timer t;
	goal g;
	std::cout << g("al") << std::endl;
	std::cout << g()("al") << std::endl;
	std::cout << g()()("al") << std::endl;
}


};



int main() 
{
	const size_t TEST_TIMES = 10;

	{
		timer t;
		for (size_t i = 0; i < TEST_TIMES; i++) {
			my_solution::test_goal();
		}
		t.stop();
		t.average(TEST_TIMES);
	}

	{
		timer t;
		for (size_t i = 0; i < TEST_TIMES; i++) {
			derrickturk_solution::test_goal();
		}
		t.stop();
		t.average(TEST_TIMES);
	}

	{
		timer t;
		for (size_t i = 0; i < TEST_TIMES; i++) {
			my_solution_2::test_goal();
		}
		t.stop();
		t.average(TEST_TIMES);
	}


	//f _f = [](const std::string& str) {
	//	std::stringstream ss;
	//	ss << "g" << mid_.c_str() << str;
	//	mid_ += "o";
	//	return ss.str();
	//};

	//std::function<G(void)> g;
	//g = [&_f, &g]() -> f {
	//	std::string str = _f("g");
	//	//std::cout << str.c_str() << std::endl;

	//	return g;
	//};




	

	std::system("pause");
}
