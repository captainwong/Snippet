#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX


#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <time.h>
#include <Windows.h>

#include "准驾车型.h"
#include "扣分.h"


constexpr bool all_type_id_mutual_exclusive(std::initializer_list<int> id) {
	auto begin = id.begin();
	for (size_t i = 0; i < id.size(); i++) {
		for (size_t j = i + 1; j < id.size(); j++) {
			if (*(begin + i) == *(begin + j)) {
				return false;
			}
		}
	}
	return true;
}

static_assert(
	all_type_id_mutual_exclusive({ 
		申请机动车驾驶证年龄条件题目::type_id,
		载人超载扣分::超载人扣分规则题目::type_id,
		超速扣分::超速扣分规则题目::type_id,
		其他扣分::其他扣分题目::type_id,
	}), "有typeid雷同，查一下行号"
);

size_t typeSize(int type_id) {
	switch (type_id) {
	case 申请机动车驾驶证年龄条件题目::type_id: return sizeof(申请机动车驾驶证年龄条件题目);
	case 载人超载扣分::超载人扣分规则题目::type_id:return sizeof(载人超载扣分::超载人扣分规则题目);
	case 超速扣分::超速扣分规则题目::type_id:return sizeof(超速扣分::超速扣分规则题目);
	case 其他扣分::其他扣分题目::type_id:return sizeof(其他扣分::其他扣分题目);
	default: return 0;
	}
}

题目basePtr create题目(int type_id) {
	switch (type_id) {
	case 申请机动车驾驶证年龄条件题目::type_id: return std::make_shared<申请机动车驾驶证年龄条件题目>();
	case 载人超载扣分::超载人扣分规则题目::type_id:return std::make_shared<载人超载扣分::超载人扣分规则题目>();
	case 超速扣分::超速扣分规则题目::type_id:return std::make_shared<超速扣分::超速扣分规则题目>();
	case 其他扣分::其他扣分题目::type_id:return std::make_shared<其他扣分::其他扣分题目>();
	default: return nullptr;
	}
}

struct 题目manager {
	static constexpr auto path = "驾考.dat";

	std::unordered_map<int, std::map<int, 题目basePtr>> load() {
		std::unordered_map<int, std::map<int, 题目basePtr>> ps;
		FILE* f = fopen(path, "rb");
		do {
			if (!f) { break; }
			int count = 0;
			if (fread(&count, 1, sizeof(count), f) != sizeof(count)) {
				break;
			}
			bool failed = false;
			for (int i = 0; i < count; i++) {
				int type_id = 0;
				if (fread(&type_id, 1, sizeof(type_id), f) != sizeof(type_id)) {
					failed = true;
					break;
				}
				auto ptr = create题目(type_id);
				if (!ptr) { failed = true; break; }
				if (!ptr->read(f)) { failed = true; break; }
				ps[type_id][ptr->id] = ptr;
			}
			if (failed) break;

			fclose(f);
			return ps;
		} while (0);
		if (f) { fclose(f); }
		return std::unordered_map<int, std::map<int, 题目basePtr>>();
	}

	bool save(const std::unordered_map<int, std::map<int, 题目basePtr>>& ps) {
		FILE* f = fopen(path, "rb");
		do {
			if (!f) { break; }
			size_t sz = ps.size();
			write_elment(sz);
			for (const auto& i : ps) {
				for (const auto& j : i.second) {
					if (!j.second->write(f)) break;
				}
			}

		} while (0);
		if (f) { fclose(f); }
		return false;
	}
};

std::unordered_map<int, std::map<int, 题目basePtr>> g_题目{};

std::unordered_map<int, std::map<int, 题目basePtr>> getAll题目() {
	std::unordered_map<int, std::map<int, 题目basePtr>> map;

	{
		int baseline = 0;
		for (const auto& i : g_申请机动车驾驶证年龄条件) {
			if (i.申请车型.empty()) {
				baseline = i.id;
			} else {
				for (size_t j = 0; j < i.申请车型.size(); j++) {
					map[申请机动车驾驶证年龄条件题目::type_id][i.id - baseline] = 
						(std::make_shared<申请机动车驾驶证年龄条件题目>(申请机动车驾驶证年龄条件题目{ i.id - baseline, i, j }));
				}
			}
		}

	}

	{
		using namespace 载人超载扣分;
		int baseline = 0;
		for (const auto& i : g_超载人扣分规则) {
			if (i.车型.empty()) {
				baseline = i.id;
			} else {
				for (const auto& j : i.车型) {
					map[超载人扣分规则题目::type_id][i.id - baseline] =
						(std::make_shared<超载人扣分规则题目>(超载人扣分规则题目{ i.id - baseline, j, i.范围, i.扣分 }));
				}
			}
		}
	}

	{
		using namespace 超速扣分;
		int baseline = 0;
		for (const auto& i : g_超速扣分规则) {
			if (i.车型.empty()) {
				baseline = i.id;
			} else {
				for (const auto& j : i.车型) {
					map[超速扣分规则题目::type_id][i.id - baseline] =
						(std::make_shared<超速扣分规则题目>(超速扣分规则题目{ i.id - baseline, j, i.道路, i.范围, i.扣分 }));
				}
			}
		}
	}

	{
		using namespace 其他扣分;
		int baseline = 0;
		for (const auto& i : g_其他扣分规则) {
			if (i.描述 == NULL) {
				baseline = i.id;
			} else {
				map[其他扣分题目::type_id][i.id - baseline] =
					(std::make_shared<其他扣分题目>(其他扣分题目{ i.id - baseline, i.描述, i.扣分 }));
			}
		}
	}

	return map;
}

void learn_准驾车型及代号()
{
	printf("%s\n", 准驾车型及代号2string(rand() % 2).c_str());
}

void learn_申请机动车驾驶证年龄条件()
{
	do_test(g_题目[申请机动车驾驶证年龄条件题目::type_id]);
}

void learn_载人超载扣分规则()
{
	do_test(g_题目[载人超载扣分::超载人扣分规则题目::type_id]);
}

void learn_超速扣分规则()
{
	do_test(g_题目[超速扣分::超速扣分规则题目::type_id]);
}

void learn_其他扣分规则() {
	do_test(g_题目[其他扣分::其他扣分题目::type_id]);
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
	append(learn_其他扣分规则);

#undef append

	time_t start = time(NULL);
	srand((unsigned int)start);

	g_题目 = getAll题目();
	题目manager mgr;

	// 读取
	auto fs = mgr.load();
	for (const auto& i : fs) {
		for (const auto& j : i.second) {
			g_题目[i.first][j.first] = j.second;
		}
	}

	while (1) {
		int n = 0;
		do {
			system("cls");
			printf("当前可以选择的项目有：\n");
			for (size_t i = 0; i < 学习项目.size(); i++) {
				const auto& 项目 = 学习项目[i];
				printf("  %d：%s\n", i + 1, 项目.name.c_str());
			}
			printf("请选择要学习的项目：");
			scanf("%d", &n);
		} while (n < 1 || n >(int)学习项目.size());

		学习项目[n - 1].func();

		printf("是否继续学习？按1继续，其他按键退出\n");
		scanf("%d", &n);
		if (n != 1) { break; }
	}

	printf("再见！\n");
}