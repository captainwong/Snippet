#pragma once

#include "template.h"

namespace 载人超载扣分 {

enum 超载人扣分车型 {
	校车 = 1 << 0,
	公路客运汽车 = 1 << 1,
	旅游客运汽车 = 1 << 2,
	七座以上载客汽车 = 1 << 3,
	其他载客汽车12 = ~(校车 | 公路客运汽车 | 旅游客运汽车),
	其他载客汽车6 = (其他载客汽车12 & ~七座以上载客汽车),
};

const char* 超载人扣分车型2string(超载人扣分车型 车型) {
	switch (车型) {
	case 校车: return "校车";
	case 公路客运汽车:return "公路客运汽车";
	case 旅游客运汽车:return "旅游客运汽车";
	case 七座以上载客汽车:return "七座以上载客汽车";
	case 其他载客汽车12:return "校车、公路客运汽车、旅游客运汽车以外的其他载客汽车";
	case 其他载客汽车6:return "校车、公路客运汽车、旅游客运汽车、七座以上载客汽车以外的其他载客汽车";
	default:return "";
	}
}

struct 超载范围 {
	int min = INT_MIN;
	int max = INT_MAX;

	std::string toString() const {
		std::string res;
		if (min != INT_MIN) {
			res += "百分之" + std::to_string(min) + "以上";
		}
		if (max != INT_MAX) {
			if (min == INT_MIN) {
				res += "百分之" + std::to_string(max) + "以下";
			} else {
				res += "未达到百分之" + std::to_string(max);
			}
		}
		return res;
	}
};

struct 超载人扣分规则 {
	std::vector<超载人扣分车型> 车型;
	超载范围 范围;
	int 扣分;
};

static const 超载人扣分规则 g_超载人扣分规则[] = {
	{ {校车, 公路客运汽车, 旅游客运汽车}, { 20, INT_MAX }, 12 },
	{ {其他载客汽车12}, { 100, INT_MAX }, 12 },
	{ {七座以上载客汽车}, { 50, 100 }, 9 },
	{ {校车, 公路客运汽车, 旅游客运汽车}, { INT_MIN, 20 }, 6 },
	{ {七座以上载客汽车}, { 20, 50 }, 6 },
	{ {其他载客汽车6}, { 50, 100 }, 6 },
	{ {其他载客汽车6}, { 20, 50 }, 3 },
};

struct 超载人扣分规则题目 : 题目base {
	超载人扣分规则题目(超载人扣分车型 车型, 超载范围 范围, int 扣分)
		: 题目base(), 车型(车型), 范围(范围), 扣分(扣分)
	{}

	超载人扣分车型 车型;
	超载范围 范围;
	int 扣分;

	virtual std::string question() const override {
		char buf[1024];
		snprintf(buf, sizeof(buf), "驾驶%s超过核定人数%s的，扣几分？", 超载人扣分车型2string(车型), 范围.toString().c_str());
		return buf;
	}

	virtual std::string answer() const override {
		return std::to_string(扣分);
	}

	virtual bool is_ans_correct(int ans) const override {
		return 扣分 == ans;
	}
};

}
