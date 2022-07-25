#pragma once

#include "template.h"


struct 扣分题目 : 题目base {

	扣分题目(int id, int 扣分) : 题目base(id), 扣分(扣分) {}

	int 扣分;

	virtual std::string answer() const override {
		return std::to_string(扣分);
	}

	virtual bool is_ans_correct(int ans) const override {
		return 扣分 == ans;
	}

	virtual bool read(FILE* f)override {
		if (!__super::read(f)) return false;
		read_element(扣分);
		return true;
	}

	virtual bool write(FILE* f) const override {
		if (!__super::write(f)) return false;
		write_elment(扣分);
		return true;
	}
};


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
	int id;
	std::vector<超载人扣分车型> 车型;
	超载范围 范围;
	int 扣分;
};

static const 超载人扣分规则 g_超载人扣分规则[] = {
	{ __LINE__, {}, {}, 0 },
	{ __LINE__, {校车, 公路客运汽车, 旅游客运汽车}, { 20, INT_MAX }, 12 },
	{ __LINE__, {其他载客汽车12}, { 100, INT_MAX }, 12 },
	{ __LINE__, {七座以上载客汽车}, { 50, 100 }, 9 },
	{ __LINE__, {校车, 公路客运汽车, 旅游客运汽车}, { INT_MIN, 20 }, 6 },
	{ __LINE__, {七座以上载客汽车}, { 20, 50 }, 6 },
	{ __LINE__, {其他载客汽车6}, { 50, 100 }, 6 },
	{ __LINE__, {其他载客汽车6}, { 20, 50 }, 3 },
};

struct 超载人扣分规则题目 : 扣分题目 {
	DECLARE_TYPE_ID
	超载人扣分规则题目() : 扣分题目(0, 0) {}
	超载人扣分规则题目(int id, 超载人扣分车型 车型, 超载范围 范围, int 扣分)
		: 扣分题目(id, 扣分), 车型(车型), 范围(范围)
	{}

	超载人扣分车型 车型;
	超载范围 范围;

	virtual std::string question() const override {
		char buf[1024];
		snprintf(buf, sizeof(buf), "驾驶%s超过核定人数%s的，扣几分？", 超载人扣分车型2string(车型), 范围.toString().c_str());
		return buf;
	}

	virtual bool read(FILE* f) override {
		if (!__super::read(f)) return false;
		read_element(车型);
		read_element(范围);
		return true;
	}

	virtual bool write(FILE* f) const override {
		if (!__super::write(f)) return false;
		write_elment(车型);
		write_elment(范围);
		return true;
	}
};
typedef std::shared_ptr<超载人扣分规则题目> 超载人扣分规则题目Ptr;

} // namespace 载人超载扣分 


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
	int id;
	std::vector<超速车型> 车型;
	超速道路 道路;
	超速范围 范围;
	int 扣分;
};

static const 超速扣分规则 g_超速扣分规则[] = {
	{ __LINE__, {}, {}, 0},
	{ __LINE__, {校车, 中型以上载客载货汽车, 危险物品运输车}, 高速公路或城市快速路, {20, INT_MAX}, 12},
	{ __LINE__, {校车, 中型以上载客载货汽车, 危险物品运输车}, 高速公路或城市快速路, {INT_MIN, 20}, 6},
	{ __LINE__, {校车, 中型以上载客载货汽车, 危险物品运输车}, 其他道路, {50, INT_MAX}, 9},
	{ __LINE__, {校车, 中型以上载客载货汽车, 危险物品运输车}, 其他道路, {20, 50}, 6},
	{ __LINE__, {校车, 中型以上载客载货汽车, 危险物品运输车}, 其他道路, {10, 20}, 1},
	{ __LINE__, {其他车辆}, 高速公路或城市快速路, {50, INT_MAX}, 12},
	{ __LINE__, {其他车辆}, 高速公路或城市快速路, {20, 50}, 6},
	{ __LINE__, {其他车辆}, 其他道路, {50, INT_MAX}, 6},
	{ __LINE__, {其他车辆}, 其他道路, {20, 50}, 3},
};

struct 超速扣分规则题目 : 扣分题目 {
	DECLARE_TYPE_ID
	超速扣分规则题目() : 扣分题目(0, 0) {}
	超速扣分规则题目(int id, 超速车型 车型, 超速道路 道路, 超速范围 范围, int 扣分)
		: 扣分题目(id, 扣分), 车型(车型), 道路(道路), 范围(范围)
	{}

	超速车型 车型;
	超速道路 道路;
	超速范围 范围;
	
	virtual std::string question() const override {
		char buf[1024];
		snprintf(buf, sizeof(buf), "驾驶%s在%s上行驶超过规定时速%s的，扣几分？", 超速车型2string(车型), 超速道路2string(道路), 范围.toString().c_str());
		return buf;
	}

	virtual bool read(FILE* f) override {
		if (!__super::read(f)) return false;
		read_element(车型);
		read_element(道路);
		read_element(范围);
		return true;
	}

	virtual bool write(FILE* f) const override {
		if (!__super::write(f)) return false;
		write_elment(车型);
		write_elment(道路);
		write_elment(范围);
		return true;
	}
};
typedef std::shared_ptr<超速扣分规则题目> 超速扣分规则题目Ptr;

} // namespace 超速扣分 

namespace 其他扣分 {

struct 其他扣分规则 {
	int id;
	const char* 描述;
	int 扣分;
};

static const 其他扣分规则 g_其他扣分规则[] = {
	{__LINE__, NULL, 0},
	{__LINE__, "饮酒后驾驶机动车的", 12},
	{__LINE__, "造成致人轻伤以上或者死亡的交通事故后逃逸，尚不构成犯罪的", 12},
	{__LINE__, "使用伪造、变造的机动车号牌、行驶证、驾驶证、校车标牌或者使用其他机动车号牌、行驶证的", 12},
	{__LINE__, "驾驶机动车在高速公路、城市快速路上倒车、逆行、穿越中央分隔带掉头的", 12},
	{__LINE__, "代替实际机动车驾驶人接受交通违法行为处罚和记分牟取经济利益的", 12},

	{__LINE__, "驾驶机动车在高速公路或者城市快速路上违法停车的", 9},
	{__LINE__, "驾驶未悬挂机动车号牌或者故意遮挡、污损机动车号牌的机动车上道路行驶的", 9},
	{__LINE__, "驾驶与准驾车型不符的机动车的", 9},
	{__LINE__, "未取得校车驾驶资格驾驶校车的", 9},
	{__LINE__, "连续驾驶中型以上载客汽车、危险物品运输车辆超过4小时未停车休息或者停车休息时间少于20分钟的", 9},

	{__LINE__, "驾驶载货汽车载物超过最大允许总质量百分之五十以上的", 6},
	{__LINE__, "驾驶机动车载运爆炸物品、易燃易爆化学物品以及剧毒、放射性等危险物品，未按指定的时间、路线、速度行驶或者未悬挂警示标志并采取必要的安全措施的", 6},
	{__LINE__, "驾驶机动车运载超限的不可解体的物品，未按指定的时间、路线、速度行驶或者未悬挂警示标志的", 6},
	{__LINE__, "驾驶机动车运输危险化学品，未经批准进入危险化学品运输车辆限制通行的区域的", 6},
	{__LINE__, "驾驶机动车不按交通信号灯指示通行的", 6},
	{__LINE__, "机动车驾驶证被暂扣或者扣留期间驾驶机动车的", 6},
	{__LINE__, "造成致人轻微伤或者财产损失的交通事故后逃逸，尚不构成犯罪的", 6},
	{__LINE__, "驾驶机动车在高速公路或者城市快速路上违法占用应急车道行驶的", 6},

	{__LINE__, "驾驶机动车在高速公路或者城市快速路上不按规定车道行驶的", 3},
	{__LINE__, "驾驶机动车不按规定超车、让行，或者在高速公路、城市快速路以外的道路上逆行的", 3},
	{__LINE__, "驾驶机动车遇前方机动车停车排队或者缓慢行驶时，借道超车或者占用对面车道、穿插等候车辆的", 3},
	{__LINE__, "驾驶机动车有拨打、接听手持电话等妨碍安全驾驶的行为的", 3},
	{__LINE__, "驾驶机动车行经人行横道不按规定减速、停车、避让行人的", 3},
	{__LINE__, "驾驶机动车不按规定避让校车的", 3},
	{__LINE__, "驾驶载货汽车载物超过最大允许总质量百分之三十以上未达到百分之五十的，或者违反规定载客的", 3},
	{__LINE__, "驾驶不按规定安装机动车号牌的机动车上道路行驶的", 3},
	{__LINE__, "在道路上车辆发生故障、事故停车后，不按规定使用灯光或者设置警告标志的", 3},
	{__LINE__, "驾驶未按规定定期进行安全技术检验的公路客运汽车、旅游客运汽车、危险物品运输车辆上道路行驶的", 3},
	{__LINE__, "驾驶校车上道路行驶前，未对校车车况是否符合安全技术要求进行检查，或者驾驶存在安全隐患的校车上道路行驶的", 3},
	{__LINE__, "连续驾驶载货汽车超过4小时未停车休息或者停车休息时间少于20分钟的", 3},
	{__LINE__, "驾驶机动车在高速公路上行驶低于规定最低时速的", 3},

	{__LINE__, "驾驶机动车不按规定会车，或者在高速公路、城市快速路以外的道路上不按规定倒车、掉头的", 1},
	{__LINE__, "驾驶机动车不按规定使用灯光的", 1},
	{__LINE__, "驾驶机动车违反禁令标志、禁止标线指示的", 1},
	{__LINE__, "驾驶机动车载货长度、宽度、高度超过规定的", 1},
	{__LINE__, "驾驶载货汽车载物超过最大允许总质量未达到百分之三十的", 1},
	{__LINE__, "驾驶未按规定定期进行安全技术检验的公路客运汽车、旅游客运汽车、危险物品运输车辆以外的机动车上道路行驶的", 1},
	{__LINE__, "驾驶擅自改变已登记的结构、构造或者特征的载货汽车上道路行驶的", 1},
	{__LINE__, "驾驶机动车在道路上行驶时，机动车驾驶人未按规定系安全带的", 1},
	{__LINE__, "驾驶摩托车，不戴安全头盔的", 1},
};

struct 其他扣分题目 : 扣分题目 {
	DECLARE_TYPE_ID
	std::string 描述;

	其他扣分题目() : 扣分题目(0, 0) {}
	其他扣分题目(int id, const char* 描述, int 扣分) : 扣分题目(id, 扣分), 描述(描述) {}

	virtual std::string question() const override {
		return 描述;
	}
};
typedef std::shared_ptr<其他扣分题目> 其他扣分题目Ptr;


} // namespace 其他扣分


