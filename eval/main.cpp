// 表达式求值
//
// 输入：表达式字符串，支持的运算符包括： +  -  *  /  ^   !    (     )
//                                     加 减 乘 除 乘方 阶乘 左括号 右括号
// 输出：



#include <iostream>
#include <string>
#include <cctype>
#include <stack>
#include <unordered_map>

//enum operator_cmp_result {
//	invalid,
//	less_than,
//	equal_to,
//	bigger_than,
//};
//
//class optr_stack
//{
//	class optr 
//	{
//	public:
//		std::string optr_ = {};
//	};
//
//public:
//
//private:
//	std::unordered_map<std::string, 
//};


bool evaluate(std::string expression, double& result) 
{
	std::stack<double> opnd;
	std::stack<char> optr;
	optr.push('\0');
	expression.push_back('\0');

	for (auto c : expression) {
		if (std::isdigit(c)) {

		}
	}

}


int main()
{

}
