#pragma once

#include "template.h"

namespace ���ٿ۷� {

enum ���ٳ��� {
	У�� = 1,
	���������ؿ��ػ����� = 1 << 1,
	Σ����Ʒ���䳵 = 1 << 2,
	�������� = ~(У�� | ���������ؿ��ػ����� | Σ����Ʒ���䳵),
};

const char* ���ٳ���2string(���ٳ��� ����) {
	switch (����) {
	case У��: return "У��";
	case ���������ؿ��ػ�����: return "���������ؿ��ػ�����";
	case Σ����Ʒ���䳵: return "Σ����Ʒ���䳵";
	case ��������: return "У�������������ؿ��ػ�������Σ����Ʒ���䳵����Ļ�����";
	default: return "";
	}
}

enum ���ٵ�· {
	���ٹ�·����п���· = 1,
	������· = 2,
};

const char* ���ٵ�·2string(���ٵ�· ��·) {
	switch (��·) {
	case ���ٹ�·����п���·: return "���ٹ�·�����п���·";
	case ������·: return "���ٹ�·�����п���·�����������·";
	default: return "";
	}
}

struct ���ٷ�Χ {
	int min = INT_MIN;
	int max = INT_MAX;

	std::string toString() const {
		std::string res;
		if (min != INT_MIN) {
			res = "�ٷ�֮" + std::to_string(min) + "����";
		}
		if (max != INT_MAX) {
			res += "δ�ﵽ�ٷ�֮" + std::to_string(max);
		}
		return res;
	}
};

struct ���ٿ۷ֹ��� {
	std::vector<���ٳ���> ����;
	���ٵ�· ��·;
	���ٷ�Χ ��Χ;
	int �۷�;
};

static const ���ٿ۷ֹ��� g_���ٿ۷ֹ���[] = {
	{ {У��, ���������ؿ��ػ�����, Σ����Ʒ���䳵}, ���ٹ�·����п���·, {20, INT_MAX}, 12},
	{ {У��, ���������ؿ��ػ�����, Σ����Ʒ���䳵}, ���ٹ�·����п���·, {INT_MIN, 20}, 6},
	{ {У��, ���������ؿ��ػ�����, Σ����Ʒ���䳵}, ������·, {50, INT_MAX}, 9},
	{ {У��, ���������ؿ��ػ�����, Σ����Ʒ���䳵}, ������·, {20, 50}, 6},
	{ {У��, ���������ؿ��ػ�����, Σ����Ʒ���䳵}, ������·, {10, 20}, 1},
	{ {��������}, ���ٹ�·����п���·, {50, INT_MAX}, 12},
	{ {��������}, ���ٹ�·����п���·, {20, 50}, 6},
	{ {��������}, ������·, {50, INT_MAX}, 6},
	{ {��������}, ������·, {20, 50}, 3},
};


struct ���ٿ۷ֹ�����Ŀ : ��Ŀbase {
	���ٿ۷ֹ�����Ŀ(���ٳ��� ����, ���ٵ�· ��·, ���ٷ�Χ ��Χ, int �۷�)
		: ��Ŀbase(), ����(����), ��·(��·), ��Χ(��Χ), �۷�(�۷�)
	{}

	���ٳ��� ����;
	���ٵ�· ��·;
	���ٷ�Χ ��Χ;
	int �۷�;

	virtual std::string question() const override {
		char buf[1024];
		snprintf(buf, sizeof(buf), "��ʻ%s��%s����ʻ�����涨ʱ��%s�ģ��ۼ��֣�", ���ٳ���2string(����), ���ٵ�·2string(��·), ��Χ.toString().c_str());
		return buf;
	}

	virtual std::string answer() const override {
		return std::to_string(�۷�);
	}

	virtual bool is_ans_correct(int ans) const override {
		return �۷� == ans;
	}
};

}
