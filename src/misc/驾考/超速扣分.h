#pragma once

#include "template.h"

namespace 超速扣分 {

enum 超速车型 {
	校车 = 1,
	中型以上载客载货汽车 = 1 << 1,
	危险物品运输车 = 1 << 2,
	其他车辆 = ~(校车 | 中型以上载客载货汽车 | 危险物品运输车),
};

const char* 超速车型2string(超速车型 车型) {
	switch (车型) {
	case 校车: return "校车";
	case 中型以上载客载货汽车: return "中型以上载客载货汽车";
	case 危险物品运输车: return "危险物品运输车";
	case 其他车辆: return "校车、中型以上载客载货汽车、危险物品运输车以外的机动车";
	default: return "";
	}
}

enum 超速道路 {
	高速公路或城市快速路 = 1,
	其他道路 = 2,
};

const char* 超速道路2string(超速道路 道路) {
	switch (道路) {
	case 高速公路或城市快速路: return "高速公路、城市快速路";
	case 其他道路: return "高速公路、城市快速路以外的其他道路";
	default: return "";
	}
}

struct 超速范围 {
	int min = INT_MIN;
	int max = INT_MAX;

	std::string toString() const {
		std::string res;
		if (min != INT_MIN) {
			res = "百分之" + std::to_string(min) + "以上";
		}
		if (max != INT_MAX) {
			res += "未达到百分之" + std::to_string(max);
		}
		return res;
	}
};

struct 超速扣分规则 {
	std::vector<超速车型> 车型;
	超速道路 道路;
	超速范围 范围;
	int 扣分;
};

static const 超速扣分规则 g_超速扣分规则[] = {
	{ {校车, 中型以上载客载货汽车, 危险物品运输车}, 高速公路或城市快速路, {20, INT_MAX}, 12},
	{ {校车, 中型以上载客载货汽车, 危险物品运输车}, 高速公路或城市快速路, {INT_MIN, 20}, 6},
	{ {校车, 中型以上载客载货汽车, 危险物品运输车}, 其他道路, {50, INT_MAX}, 9},
	{ {校车, 中型以上载客载货汽车, 危险物品运输车}, 其他道路, {20, 50}, 6},
	{ {校车, 中型以上载客载货汽车, 危险物品运输车}, 其他道路, {10, 20}, 1},
	{ {其他车辆}, 高速公路或城市快速路, {50, INT_MAX}, 12},
	{ {其他车辆}, 高速公路或城市快速路, {20, 50}, 6},
	{ {其他车辆}, 其他道路, {50, INT_MAX}, 6},
	{ {其他车辆}, 其他道路, {20, 50}, 3},
};


struct 超速扣分规则题目 : 题目base {
	超速扣分规则题目(超速车型 车型, 超速道路 道路, 超速范围 范围, int 扣分)
		: 题目base(), 车型(车型), 道路(道路), 范围(范围), 扣分(扣分)
	{}

	超速车型 车型;
	超速道路 道路;
	超速范围 范围;
	int 扣分;

	virtual std::string question() const override {
		char buf[1024];
		snprintf(buf, sizeof(buf), "驾驶%s在%s上行驶超过规定时速%s的，扣几分？", 超速车型2string(车型), 超速道路2string(道路), 范围.toString().c_str());
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
