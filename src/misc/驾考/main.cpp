#define _CRT_SECURE_NO_WARNINGS

#include <functional>
#include <time.h>
#include <Windows.h>

#include "准驾车型.h"
#include "载人超载扣分.h"
#include "超速扣分.h"



void learn_准驾车型及代号()
{
	printf("%s\n", 准驾车型及代号2string(rand() % 2).c_str());
}

void learn_申请机动车驾驶证年龄条件()
{
	std::vector<申请机动车驾驶证年龄条件题目> 题目列表;
	for (const auto& i : g_申请机动车驾驶证年龄条件) {
		for(size_t j = 0; j < i.申请车型.size(); j++) {
			题目列表.push_back(申请机动车驾驶证年龄条件题目({ i, j }));
		}
	}

	do_test(题目列表);
}

void learn_载人超载扣分规则()
{
	using namespace 载人超载扣分;
	std::vector<超载人扣分规则题目> 题目列表;
	for (const auto& i : g_超载人扣分规则) {
		for (const auto& j : i.车型) {
			题目列表.push_back(超载人扣分规则题目({ j, i.范围, i.扣分 }));
		}
	}

	do_test(题目列表);
}

void learn_超速扣分规则()
{
	using namespace 超速扣分;
	std::vector<超速扣分规则题目> 题目列表;
	for (const auto& i : g_超速扣分规则) {
		for (const auto& j : i.车型) {
			题目列表.push_back(超速扣分规则题目({ j, i.道路, i.范围, i.扣分 }));
		}
	}

	do_test(题目列表);
}

struct 学习项目 {
	std::string name;
	std::function<void(void)> func;
};

int main()
{
	// https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console
	system("reg add HKEY_CURRENT_USER\\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f");

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 32;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"仿宋");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	std::vector<学习项目> 学习项目;
#define append(func) 学习项目.push_back(struct 学习项目({#func, &func})); 学习项目.back().name = 学习项目.back().name.substr(6);

	append(learn_准驾车型及代号);
	append(learn_申请机动车驾驶证年龄条件);
	append(learn_载人超载扣分规则);
	append(learn_超速扣分规则);

#undef append

	srand((unsigned int)time(NULL));
	int n = 0;
	do {
		printf("当前可以选择的项目有：\n");
		for (size_t i = 0; i < 学习项目.size(); i++) {
			const auto& 项目 = 学习项目[i];
			printf("  %d：%s\n", i + 1, 项目.name.c_str());
		}
		printf("请选择要学习的项目：");
		scanf("%d", &n);
	} while (n < 1 || n > (int)学习项目.size());

	学习项目[n - 1].func();
}