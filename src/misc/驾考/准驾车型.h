#pragma once

#include "template.h"

enum 代号 {
	A1, A2, A3,
	B1, B2,
	C1, C2, C3, C4, C5, C6,
	D, E, F, M, N, P,
	代号_COUNT,
};

const char* 代号2string(代号 号) {
	switch (号) {
	case A1: return "A1";
	case A2: return "A2";
	case A3: return "A3";
	case B1: return "B1";
	case B2: return "B2";
	case C1: return "C1";
	case C2: return "C2";
	case C3: return "C3";
	case C4: return "C4";
	case C5: return "C5";
	case C6: return "C6";
	case D: return "D";
	case E: return "E";
	case F: return "F";
	case M: return "M";
	case N: return "N";
	case P: return "P";
	default: return "";
	}
}

enum 车型 {
	大型客车,
	重型牵引挂车,
	城市公交车,
	中型客车,
	大型货车,
	小型汽车,
	小型自动挡汽车,
	低速载货汽车,
	三轮汽车,
	残疾人专用小型自动挡载客汽车,
	轻型牵引挂车,
	普通三轮摩托车,
	普通二轮摩托车,
	轻便摩托车,
	轮式专用机械车,
	无轨电车,
	有轨电车,
	车型_COUNT,
};

const char* 车型2string(车型 型) {
	switch (型) {
	case 大型客车: return "大型客车";
	case 重型牵引挂车: return "重型牵引挂车";
	case 城市公交车: return "城市公交车";
	case 中型客车: return "中型客车";
	case 大型货车: return "大型货车";
	case 小型汽车: return "小型汽车";
	case 小型自动挡汽车: return "小型自动挡汽车";
	case 低速载货汽车: return "低速载货汽车";
	case 三轮汽车: return "三轮汽车";
	case 残疾人专用小型自动挡载客汽车: return "残疾人专用小型自动挡载客汽车";
	case 轻型牵引挂车: return "轻型牵引挂车";
	case 普通三轮摩托车: return "普通三轮摩托车";
	case 普通二轮摩托车: return "普通二轮摩托车";
	case 轻便摩托车: return "轻便摩托车";
	case 轮式专用机械车: return "轮式专用机械车";
	case 无轨电车: return "无轨电车";
	case 有轨电车: return "有轨电车";
	default: return "";
	}
}

struct 准驾车型及代号 {
	车型 准驾车型;
	代号 代号;
	const char* 准驾的车辆;
	std::vector<enum 代号> 准予驾驶的其他准驾车型;
};

static const struct 准驾车型及代号 g_准驾车型及代号[] = {
	{ 大型客车,A1,"大型载客汽车",{A3,B1,B2,C1,C2,C3,C4,M}},
	{ 重型牵引挂车, A2, "总质量大于4500kg的汽车列车", { B1,B2,C1,C2,C3,C4,C6,M }},
	{ 城市公交车,A3,"核载10人以上的城市公共汽车",{C1,C2,C3,C4} },
	{ 中型客车,B1,"中型载客汽车（含核载10人以上,19人以下的城市公共汽车）",{C1,C2,C3,C4,M} },
	{ 大型货车,B2,"重型、中型载货汽车；重型、中型专项作业车",{} },
	{ 小型汽车,C1,"小型、微型载客汽车以及轻型、微型载货汽车；轻型、微型专项作业车",{C2,C3,C4} },
	{ 小型自动挡汽车,C2,"小型、微型自动挡载客汽车以及轻型、微型自动挡载货汽车；轻型、微型自动挡专项作业车；上肢残疾人专用小型自动挡载客汽车",{} },
	{ 低速载货汽车,C3,"低速载货汽车",{C4} },
	{ 三轮汽车,C4,"三轮汽车",{} },
	{ 残疾人专用小型自动挡载客汽车,C5,"残疾人专用小型、微型自动挡载客汽车（允许上肢、右下肢或者双下肢残疾人驾驶）",{} },
	{ 轻型牵引挂车,C6,"总质量小于（不包含等于）4500kg的汽车列车",{} },
	{ 普通三轮摩托车,D,"发动机排量大于50ml或者最大设计车速大于50km/h的三轮摩托车",{E,F} },
	{ 普通二轮摩托车,E,"发动机排量大于50ml或者最大设计车速大于50km/h的二轮摩托车",{F} },
	{ 轻便摩托车,F,"发动机排量小于等于50ml，最大设计车速小于等于50km/h的摩托车",{} },
	{ 轮式专用机械车,M,"轮式专用机械车",{} },
	{ 无轨电车,N,"无轨电车",{} },
	{ 有轨电车,P,"有轨电车",{} },
};

std::string find_准驾车辆_by_代号(代号 号) {
	for (const auto& i : g_准驾车型及代号) {
		if (i.代号 == 号) {
			return i.准驾的车辆;
		}
	}
	return {};
}

std::string 准驾车型及代号2string(bool expand = false) {
	std::vector<std::string> res = { "准驾车型及代号" };
	for (const auto& i : g_准驾车型及代号) {
		std::vector<std::string> v = { std::string(代号2string(i.代号)) + ": " + i.准驾的车辆 };
		for (const auto& j : i.准予驾驶的其他准驾车型) {
			if (expand) {
				v.push_back(find_准驾车辆_by_代号(j));
			} else {
				v.push_back(代号2string(j));
			}
		}
		res.push_back(jlib::join<std::string>(v, "，"));
	}
	return jlib::join<std::string>(res, "\n\n  ");
}

struct 年龄限制 {
	int min = INT_MIN;
	int max = INT_MAX;

	bool operator==(const 年龄限制& rhs) const {
		return min == rhs.min && max == rhs.max;
	}

	bool operator!=(const 年龄限制& rhs) const {
		return !this->operator==(rhs);
	}
};

struct 申请机动车驾驶证年龄条件 {
	int id;
	年龄限制 年龄限制{};
	std::vector<车型> 车型{};
	bool 全日制 = false;
};

static const 申请机动车驾驶证年龄条件 g_申请机动车驾驶证年龄条件[] = {
	{__LINE__, {}, {}},
	{__LINE__, {18, INT_MAX}, {小型汽车, 小型自动挡汽车, 残疾人专用小型自动挡载客汽车, 轻便摩托车} },
	{__LINE__, {18, 60}, {低速载货汽车, 三轮汽车, 普通三轮摩托车, 普通二轮摩托车, 轮式专用机械车}},
	{__LINE__, {20, 60}, {城市公交车, 中型客车, 大型货车, 轻型牵引挂车, 无轨电车, 有轨电车}},
	{__LINE__, {22, 60}, {大型客车, 重型牵引挂车}},
	{__LINE__, {19, 60}, {大型客车, 重型牵引挂车}, true},
};

std::string 年龄限制2string(年龄限制 限制) {
	std::string res = std::to_string(限制.min) + "周岁以上";
	if (限制.max != INT_MAX) {
		res += "，" + std::to_string(限制.max) + "周岁以下";
	}
	return res;
}

std::vector<年龄限制> get_valid_年龄限制() {
	std::vector<年龄限制> v;
	for (const auto& i : g_申请机动车驾驶证年龄条件) {
		if (!i.车型.empty()) {
			v.push_back(i.年龄限制);
		}
	}
	return v;
}

struct 申请机动车驾驶证年龄条件题目 : 题目base {
	DECLARE_TYPE_ID
	申请机动车驾驶证年龄条件题目() : 题目base(0) {}
	申请机动车驾驶证年龄条件题目(int id, 申请机动车驾驶证年龄条件 条件, size_t 车型id) : 题目base(id), 条件(条件), 车型id(车型id) {}
	申请机动车驾驶证年龄条件 条件;
	size_t 车型id;

	virtual std::string question() const override {
		char buf[1024];
		auto 车型 = 车型2string(条件.车型[车型id]);
		if (条件.全日制) {
			snprintf(buf, sizeof(buf), "接受全日制驾驶职业教育的学生，申请%s准驾车型的，年龄限制在？", 车型);
		} else {
			snprintf(buf, sizeof(buf), "申请%s准驾车型的，年龄限制在？", 车型);
		}
		return buf;
	}

	virtual std::string answer() const override {
		return 年龄限制2string(条件.年龄限制);
	}

	virtual void print_options() const {
		int i = 1;
		const auto& valid_年龄限制 = get_valid_年龄限制();
		for (const auto& j : valid_年龄限制) {
			printf("  %d.%s\n", i++, 年龄限制2string(j).c_str());
		}
	}

	virtual bool is_ans_correct(int ans) const override {
		const auto& valid_年龄限制 = get_valid_年龄限制();
		return !(ans < 1 || ans >(int)valid_年龄限制.size() || valid_年龄限制[ans - 1] != 条件.年龄限制);
	}

	virtual bool read(FILE* f) override {
		if (!__super::read(f)) return false;
		read_element(车型id);
		return true;
	}

	virtual bool write(FILE* f) const override {
		if (!__super::write(f)) return false;
		write_elment(车型id);
		return true;
	}
};
typedef std::shared_ptr<申请机动车驾驶证年龄条件题目> 申请机动车驾驶证年龄条件题目Ptr;
