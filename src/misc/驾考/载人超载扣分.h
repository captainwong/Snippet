#pragma once

#include "template.h"

namespace ���˳��ؿ۷� {

enum �����˿۷ֳ��� {
	У�� = 1 << 0,
	��·�������� = 1 << 1,
	���ο������� = 1 << 2,
	���������ؿ����� = 1 << 3,
	�����ؿ�����12 = ~(У�� | ��·�������� | ���ο�������),
	�����ؿ�����6 = (�����ؿ�����12 & ~���������ؿ�����),
};

const char* �����˿۷ֳ���2string(�����˿۷ֳ��� ����) {
	switch (����) {
	case У��: return "У��";
	case ��·��������:return "��·��������";
	case ���ο�������:return "���ο�������";
	case ���������ؿ�����:return "���������ؿ�����";
	case �����ؿ�����12:return "У������·�������������ο�����������������ؿ�����";
	case �����ؿ�����6:return "У������·�������������ο������������������ؿ���������������ؿ�����";
	default:return "";
	}
}

struct ���ط�Χ {
	int min = INT_MIN;
	int max = INT_MAX;

	std::string toString() const {
		std::string res;
		if (min != INT_MIN) {
			res += "�ٷ�֮" + std::to_string(min) + "����";
		}
		if (max != INT_MAX) {
			if (min == INT_MIN) {
				res += "�ٷ�֮" + std::to_string(max) + "����";
			} else {
				res += "δ�ﵽ�ٷ�֮" + std::to_string(max);
			}
		}
		return res;
	}
};

struct �����˿۷ֹ��� {
	std::vector<�����˿۷ֳ���> ����;
	���ط�Χ ��Χ;
	int �۷�;
};

static const �����˿۷ֹ��� g_�����˿۷ֹ���[] = {
	{ {У��, ��·��������, ���ο�������}, { 20, INT_MAX }, 12 },
	{ {�����ؿ�����12}, { 100, INT_MAX }, 12 },
	{ {���������ؿ�����}, { 50, 100 }, 9 },
	{ {У��, ��·��������, ���ο�������}, { INT_MIN, 20 }, 6 },
	{ {���������ؿ�����}, { 20, 50 }, 6 },
	{ {�����ؿ�����6}, { 50, 100 }, 6 },
	{ {�����ؿ�����6}, { 20, 50 }, 3 },
};

struct �����˿۷ֹ�����Ŀ : ��Ŀbase {
	�����˿۷ֹ�����Ŀ(�����˿۷ֳ��� ����, ���ط�Χ ��Χ, int �۷�)
		: ��Ŀbase(), ����(����), ��Χ(��Χ), �۷�(�۷�)
	{}

	�����˿۷ֳ��� ����;
	���ط�Χ ��Χ;
	int �۷�;

	virtual std::string question() const override {
		char buf[1024];
		snprintf(buf, sizeof(buf), "��ʻ%s�����˶�����%s�ģ��ۼ��֣�", �����˿۷ֳ���2string(����), ��Χ.toString().c_str());
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
