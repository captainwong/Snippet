#include <iostream>
#include <string>
#include <cstdlib>

namespace my_solution {
class g {

	std::string pre_ = "go";

public:

	g(std::string s) {
		std::cout << "g" << s.c_str() << std::endl;
	}

	g() {}

	g& operator()(std::string str) {
		pre_ += str;
		std::cout << pre_.c_str() << std::endl;

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
	g("al");
	g()("al");
	g()()("al");
}

};


namespace Russell_Harmon_solution {

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
	std::cout << "Russell Harmon solution:" << std::endl;
	auto g = repeater<>("g");
	std::cout << g("al") << std::endl;
	std::cout << g()("al") << std::endl;
	std::cout << g()()("al") << std::endl;
}


}


int main() 
{
	my_solution::test_goal();

	Russell_Harmon_solution::test_goal();


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
