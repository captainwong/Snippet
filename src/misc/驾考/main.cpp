#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <time.h>
#include <Windows.h>
#include <jlib/base/noncopyable.h>

#include "准驾车型.h"
#include "扣分.h"


static constexpr int all_type_ids[] = {
	申请机动车驾驶证年龄条件题目::type_id,
	载人超载扣分题目::type_id,
	载货超载扣分题目::type_id,
	超速扣分::超速扣分规则题目::type_id,
	高速相关扣分题目::type_id,
	危险品运输相关扣分题目::type_id,
	其他扣分题目::type_id,
};

static constexpr bool all_type_id_mutual_exclusive(const int* ids, const size_t len) {
	auto begin = ids;
	for (size_t i = 0; i < len; i++) {
		for (size_t j = i + 1; j < len; j++) {
			if (*(begin + i) == *(begin + j)) {
				return false;
			}
		}
	}
	return true;
}

static_assert(
	all_type_id_mutual_exclusive(all_type_ids, sizeof(all_type_ids) / sizeof(all_type_ids[0])), "有typeid雷同，查一下行号"
);


class 驾考 : jlib::noncopyable {
protected:
	static constexpr auto path = "驾考.dat";
	std::unordered_map<int, std::map<int, 题目basePtr>> all题目{};
	double target_ratio = 0.1; // 错题练习时，错误率低于ratio则不再练习
	static constexpr double min_ratio = 0.001;
	static constexpr double max_ratio = 0.5;

	题目basePtr create题目(int type_id) {
		switch (type_id) {
		case 申请机动车驾驶证年龄条件题目::type_id: return std::make_shared<申请机动车驾驶证年龄条件题目>();
		case 载人超载扣分题目::type_id:return std::make_shared<载人超载扣分题目>();
		case 载货超载扣分题目::type_id:return std::make_shared<载货超载扣分题目>();
		case 超速扣分::超速扣分规则题目::type_id:return std::make_shared<超速扣分::超速扣分规则题目>();
		case 高速相关扣分题目::type_id:return std::make_shared<高速相关扣分题目>();
		case 危险品运输相关扣分题目::type_id:return std::make_shared<危险品运输相关扣分题目>();
		case 其他扣分题目::type_id:return std::make_shared<其他扣分题目>();
		default: return nullptr;
		}
	}

	size_t count题目(const std::unordered_map<int, std::map<int, 题目basePtr>>& ps) {
		size_t count = 0;
		for (const auto& i : ps) {
			count += i.second.size();
		}
		return count;
	}

	bool load(std::unordered_map<int, std::map<int, 题目basePtr>>& ps) {
		FILE* f = fopen(path, "rb");
		do {
			ps.clear();
			if (!f) { break; }
			read_element(target_ratio);
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
			return true;
		} while (0);
		if (f) { fclose(f); }
		ps.clear();
		return false;
	}

	bool save(const std::unordered_map<int, std::map<int, 题目basePtr>>& ps) {
		FILE* f = fopen(path, "wb");
		do {
			if (!f) { break; }
			write_element(target_ratio);
			size_t sz = count题目(ps);
			write_element(sz);
			for (const auto& i : ps) {
				for (const auto& j : i.second) {
					write_element(i.first);
					if (!j.second->write(f)) break;
				}
			}

		} while (0);
		if (f) { fclose(f); }
		return false;
	}

	std::unordered_map<int, std::map<int, 题目basePtr>> getAll题目() {
		std::unordered_map<int, std::map<int, 题目basePtr>> map;

		/*{
			int baseline = 0;
			for (const auto& i : g_其他扣分规则) {
				if (i.描述 == NULL) {
					baseline = i.id;
				} else {
					map[其他扣分题目::type_id][i.id - baseline] =
						(std::make_shared<其他扣分题目>(其他扣分题目{ i.id - baseline, i.描述, i.扣分 }));
				}
			}
		}*/
#define LOAD_其他扣分(项目) \
{ \
	int baseline = 0; \
	for (const auto& i : g_##项目##规则) { \
		if (i.描述 == NULL) { \
		baseline = i.id; \
		} else { \
			map[项目##题目::type_id][i.id - baseline] = \
				(std::make_shared<项目##题目>(项目##题目{ i.id - baseline, i.描述, i.扣分 })); \
		} \
	} \
}

		{
			int baseline = 0;
			int count = 0;
			for (const auto& i : g_申请机动车驾驶证年龄条件) {
				if ((int)i.车型.size() > count) {
					count = (int)i.车型.size();
				}
			}
			for (const auto& i : g_申请机动车驾驶证年龄条件) {
				if (i.车型.empty()) {
					baseline = i.id;
				} else {
					for (size_t j = 0; j < i.车型.size(); j++) {
						int id = (i.id - baseline) * count + j;
						auto ptr = (std::make_shared<申请机动车驾驶证年龄条件题目>(申请机动车驾驶证年龄条件题目{ id, i, j }));
						int tid = ptr->type_id;
						map[申请机动车驾驶证年龄条件题目::type_id][id] = ptr;
					}
				}
			}

		}

		{
			int baseline = 0;
			int count = 0;
			for (const auto& i : g_载人超载扣分规则) {
				if ((int)i.车型.size() > count) {
					count = (int)i.车型.size();
				}
			}
			for (const auto& i : g_载人超载扣分规则) {
				if (i.车型.empty()) {
					baseline = i.id;
				} else {
					for (const auto& j : i.车型) {
						int id = (i.id - baseline) * count + j;
						map[载人超载扣分题目::type_id][id] =
							(std::make_shared<载人超载扣分题目>(载人超载扣分题目{ id, j, i.范围, i.扣分 }));
					}
				}
			}
		}

		{
			int baseline = 0;
			for (const auto& i : g_载货超载扣分规则) {
				if (i.描述 == NULL) {
					baseline = i.id;
				} else {
					map[载货超载扣分题目::type_id][i.id - baseline] =
						(std::make_shared<载货超载扣分题目>(载货超载扣分题目{ i.id - baseline, i.描述, i.扣分 }));
				}
			}
		}

		{
			using namespace 超速扣分;
			int baseline = 0;
			int count = 0;
			for (const auto& i : g_超速扣分规则) {
				if ((int)i.车型.size() > count) {
					count = (int)i.车型.size();
				}
			}
			for (const auto& i : g_超速扣分规则) {
				if (i.车型.empty()) {
					baseline = i.id;
				} else {
					for (const auto& j : i.车型) {
						int id = (i.id - baseline) * count + j;
						map[超速扣分规则题目::type_id][id] =
							(std::make_shared<超速扣分规则题目>(超速扣分规则题目{ id, j, i.道路, i.范围, i.扣分 }));
					}
				}
			}
		}

		LOAD_其他扣分(高速相关扣分);
		LOAD_其他扣分(危险品运输相关扣分);
		LOAD_其他扣分(其他扣分);
		

		return map;
	}

	void learn_准驾车型及代号() {
		printf("%s\n", 准驾车型及代号2string(rand() % 2).c_str());
	}

	void learn_申请机动车驾驶证年龄条件() {
		do_test(all题目[申请机动车驾驶证年龄条件题目::type_id]);
	}

	void learn_载人超载扣分规则() {
		do_test(all题目[载人超载扣分题目::type_id]);
	}

	void learn_载货超载扣分规则() {
		do_test(all题目[载货超载扣分题目::type_id]);
	}

	void learn_超速扣分规则() {
		do_test(all题目[超速扣分::超速扣分规则题目::type_id]);
	}

	void learn_高速相关扣分规则() {
		do_test(all题目[高速相关扣分题目::type_id]);
	}

	void learn_危险品运输相关扣分规则() {
		do_test(all题目[危险品运输相关扣分题目::type_id]);
	}

	void learn_其他扣分规则() {
		do_test(all题目[其他扣分题目::type_id]);
	}

	void join(std::map<int, 题目basePtr>& origin, const std::map<int, 题目basePtr>& other) {
		for (const auto& i : other) {
			auto iter = origin.find(i.first);
			if (iter == origin.end()) {
				origin[i.first] = i.second;
			} else {
				bool inserted = false;
				for (int j = 0; j < INT_MAX; j++) {
					iter = origin.find(j);
					if (iter == origin.end()) {
						origin[j] = i.second;
						inserted = true;
						break;
					}
				}
				if (!inserted) {
					printf("kill me %s:%d\n", __FILE__, __LINE__);
					exit(1);
				}
			}
		}
	}

	std::map<int, 题目basePtr> filter(const std::map<int, 题目basePtr>& origin) {
		std::map<int, 题目basePtr> dst;
		for (const auto& i : origin) {
			if (i.second->stat.ratio() >= target_ratio) {
				dst[i.first] = i.second;
			}
		}
		return dst;
	}

	void learn_所有扣分规则() {
		std::map<int, 题目basePtr> qs;
		for (auto type_id : all_type_ids) {
			if (type_id != 申请机动车驾驶证年龄条件题目::type_id) {
				join(qs, all题目[type_id]);
			}
		}
		do_test(qs);
	}

	void learn_错题练习() {
		std::map<int, 题目basePtr> qs;
		for (auto type_id : all_type_ids) {
			join(qs, filter(all题目[type_id]));
		}
		if (qs.empty()) {
			printf("错题记录为空！\n");
			return;
		}
		do_test(qs);
	}

	void learn_调整目标错误率() {
		system("cls");
		printf("当前目标错误率为%2.2f%%，错误率低于此值则练习错题时不再出现。\n", target_ratio * 100.0);
		double ratio = 0.0;
		printf("请输入新的目标错误率：");
		scanf("%lf", &ratio);
		if (ratio / 100.0 < min_ratio || ratio / 100.0 > max_ratio) {
			printf(RED("错误率应当处于 %.2lf%% ~ %.2lf%% 之间。\n"), min_ratio * 100.0, max_ratio * 100.0);
			system("pause");
			return;
		}

		target_ratio = ratio;
		printf(GREEN("已将目标错误率修改为 %2.2lf！"), target_ratio);
		system("pause");
	}

	void learn_查看统计信息() {
		system("cls");
		//size_t total_ans_times = 0;
		//size_t total_incorrect_times = 0;

		struct item {
			double ratio;
			int type_id;
			int id;
		};

		uint64_t total_correct_times = 0;
		std::list<item> ids;
		for (const auto& i : all题目) {
			for (const auto& j : i.second) {
				if (j.second->stat.total_incorrect_times > 0) {
					double ratio = j.second->stat.ratio();
					if (ids.empty()) {
						ids.emplace_back(item({ ratio, i.first, j.first }));
					} else {
						bool inserted = false;
						for (auto begin = ids.begin(); begin != ids.end(); begin++) {
							if (begin->ratio < ratio) {
								ids.insert(begin, item({ ratio, i.first, j.first }));
								inserted = true;
								break;
							}
						}
						if (!inserted) {
							ids.emplace_back(item({ ratio, i.first, j.first }));
						}
					}
				} else {
					total_correct_times += j.second->stat.total_correct_times;
				}
			}
		}

		if (total_correct_times == 0) {
			printf("答题记录为空！\n");
		} else if (ids.empty()) {
			printf("恭喜你，一共答过%llu道题，一题不错，全部正确！\n", total_correct_times);
		} else {
			printf("一共答对过%llu道题，答错过%u道题，错题按照错误率排序为：\n", total_correct_times, ids.size());
			for (const auto& i : ids) {
				const auto& q = all题目[i.type_id][i.id];
				printf("错误率%2.2f%%，题目：%s\n正确答案为：%s\n\n", i.ratio * 100.0, q->question().c_str(), q->answer().c_str());
			}
		}
	}

	void learn_清空统计信息() {
		system("cls");
		int yes = 0;
		printf(RED("确定要清空统计信息吗？此操作无法恢复！\n"));
		printf("输入1继续，输入其他任意值取消：");
		scanf("%d", &yes);
		if (yes != 1) return;
		for (auto& i : all题目) {
			for (auto& j : i.second) {
				j.second->stat.clear();
			}
		}
		save(all题目);
		printf("已成功清除统计信息。\n");
	}

public:
	void run() {
		struct 学习项目 {
			std::string name;
			std::function<void(驾考*)> func;
		};

		//学习项目 ddd{ "", std::bind(&题目manager::learn_申请机动车驾驶证年龄条件, std::placeholders::_1) };
		std::vector<学习项目> 学习项目;
#define append(func) 学习项目.push_back(struct 学习项目({#func, std::bind(&驾考::func, std::placeholders::_1)})); 学习项目.back().name = 学习项目.back().name.substr(6);
		append(learn_准驾车型及代号);
		append(learn_申请机动车驾驶证年龄条件);
		append(learn_载人超载扣分规则);
		append(learn_载货超载扣分规则);
		append(learn_超速扣分规则);
		append(learn_高速相关扣分规则);
		append(learn_危险品运输相关扣分规则);
		append(learn_其他扣分规则);
		append(learn_所有扣分规则);
		append(learn_错题练习);
		append(learn_调整目标错误率);
		append(learn_查看统计信息);
		append(learn_清空统计信息);

#undef append

		time_t start = time(NULL);
		srand((unsigned int)start);

		all题目 = getAll题目();

		// 读取统计信息
		std::unordered_map<int, std::map<int, 题目basePtr>> fs;
		load(fs);
		for (const auto& i : fs) {
			for (const auto& j : i.second) {
				all题目[i.first][j.first]->stat = j.second->stat;
			}
		}

		while (1) {
			int n = 0, sr = 0;
			do {
				system("cls");
				printf("当前可以选择的项目有：\n");
				for (size_t i = 0; i < 学习项目.size(); i++) {
					const auto& 项目 = 学习项目[i];
					printf("  %2d：%s\n", i + 1, 项目.name.c_str());
				}
				printf("  %2u: 退出\n", 学习项目.size() + 1);
				printf("请选择要学习的项目：");
				sr = scanf("%d", &n);
				if (sr == 0) { break; }
			} while (n < 1 || n > (int)学习项目.size() + 1);
			if (sr == 0) { break; }
			if (n == (int)学习项目.size() + 1) {
				break;
			}
			学习项目[n - 1].func(this);

			printf("是否继续学习？按1继续，其他按键退出\n");
			scanf("%d", &n);
			if (n != 1) { break; }
		}

		// 保存统计信息
		save(all题目);

		printf("再见！\n");
	}
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

	驾考().run();
}