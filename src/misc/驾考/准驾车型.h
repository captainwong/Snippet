#pragma once

#include "template.h"

enum ���� {
	A1, A2, A3,
	B1, B2,
	C1, C2, C3, C4, C5, C6,
	D, E, F, M, N, P,
	����_COUNT,
};

const char* ����2string(���� ��) {
	switch (��) {
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

enum ���� {
	���Ϳͳ�,
	����ǣ���ҳ�,
	���й�����,
	���Ϳͳ�,
	���ͻ���,
	С������,
	С���Զ�������,
	�����ػ�����,
	��������,
	�м���ר��С���Զ����ؿ�����,
	����ǣ���ҳ�,
	��ͨ����Ħ�г�,
	��ͨ����Ħ�г�,
	���Ħ�г�,
	��ʽר�û�е��,
	�޹�糵,
	�й�糵,
	����_COUNT,
};

const char* ����2string(���� ��) {
	switch (��) {
	case ���Ϳͳ�: return "���Ϳͳ�";
	case ����ǣ���ҳ�: return "����ǣ���ҳ�";
	case ���й�����: return "���й�����";
	case ���Ϳͳ�: return "���Ϳͳ�";
	case ���ͻ���: return "���ͻ���";
	case С������: return "С������";
	case С���Զ�������: return "С���Զ�������";
	case �����ػ�����: return "�����ػ�����";
	case ��������: return "��������";
	case �м���ר��С���Զ����ؿ�����: return "�м���ר��С���Զ����ؿ�����";
	case ����ǣ���ҳ�: return "����ǣ���ҳ�";
	case ��ͨ����Ħ�г�: return "��ͨ����Ħ�г�";
	case ��ͨ����Ħ�г�: return "��ͨ����Ħ�г�";
	case ���Ħ�г�: return "���Ħ�г�";
	case ��ʽר�û�е��: return "��ʽר�û�е��";
	case �޹�糵: return "�޹�糵";
	case �й�糵: return "�й�糵";
	default: return "";
	}
}

struct ׼�ݳ��ͼ����� {
	���� ׼�ݳ���;
	���� ����;
	const char* ׼�ݵĳ���;
	std::vector<enum ����> ׼���ʻ������׼�ݳ���;
};

static const struct ׼�ݳ��ͼ����� g_׼�ݳ��ͼ�����[] = {
	{ ���Ϳͳ�,A1,"�����ؿ�����",{A3,B1,B2,C1,C2,C3,C4,M}},
	{ ����ǣ���ҳ�, A2, "����������4500kg�������г�", { B1,B2,C1,C2,C3,C4,C6,M }},
	{ ���й�����,A3,"����10�����ϵĳ��й�������",{C1,C2,C3,C4} },
	{ ���Ϳͳ�,B1,"�����ؿ�������������10������,19�����µĳ��й���������",{C1,C2,C3,C4,M} },
	{ ���ͻ���,B2,"���͡������ػ����������͡�����ר����ҵ��",{} },
	{ С������,C1,"С�͡�΢���ؿ������Լ����͡�΢���ػ����������͡�΢��ר����ҵ��",{C2,C3,C4} },
	{ С���Զ�������,C2,"С�͡�΢���Զ����ؿ������Լ����͡�΢���Զ����ػ����������͡�΢���Զ���ר����ҵ������֫�м���ר��С���Զ����ؿ�����",{} },
	{ �����ػ�����,C3,"�����ػ�����",{C4} },
	{ ��������,C4,"��������",{} },
	{ �м���ר��С���Զ����ؿ�����,C5,"�м���ר��С�͡�΢���Զ����ؿ�������������֫������֫����˫��֫�м��˼�ʻ��",{} },
	{ ����ǣ���ҳ�,C6,"������С�ڣ����������ڣ�4500kg�������г�",{} },
	{ ��ͨ����Ħ�г�,D,"��������������50ml���������Ƴ��ٴ���50km/h������Ħ�г�",{E,F} },
	{ ��ͨ����Ħ�г�,E,"��������������50ml���������Ƴ��ٴ���50km/h�Ķ���Ħ�г�",{F} },
	{ ���Ħ�г�,F,"����������С�ڵ���50ml�������Ƴ���С�ڵ���50km/h��Ħ�г�",{} },
	{ ��ʽר�û�е��,M,"��ʽר�û�е��",{} },
	{ �޹�糵,N,"�޹�糵",{} },
	{ �й�糵,P,"�й�糵",{} },
};

std::string find_׼�ݳ���_by_����(���� ��) {
	for (const auto& i : g_׼�ݳ��ͼ�����) {
		if (i.���� == ��) {
			return i.׼�ݵĳ���;
		}
	}
	return {};
}

std::string ׼�ݳ��ͼ�����2string(bool expand = false) {
	std::vector<std::string> res = { "׼�ݳ��ͼ�����" };
	for (const auto& i : g_׼�ݳ��ͼ�����) {
		std::vector<std::string> v = { std::string(����2string(i.����)) + ": " + i.׼�ݵĳ��� };
		for (const auto& j : i.׼���ʻ������׼�ݳ���) {
			if (expand) {
				v.push_back(find_׼�ݳ���_by_����(j));
			} else {
				v.push_back(����2string(j));
			}
		}
		res.push_back(jlib::join<std::string>(v, "��"));
	}
	return jlib::join<std::string>(res, "\n\n  ");
}

struct �������� {
	int min = INT_MIN;
	int max = INT_MAX;

	bool operator==(const ��������& rhs) const {
		return min == rhs.min && max == rhs.max;
	}

	bool operator!=(const ��������& rhs) const {
		return !this->operator==(rhs);
	}
};

struct �����������ʻ֤�������� {
	int id;
	�������� ��������{};
	std::vector<����> ����{};
	bool ȫ���� = false;
};

static const �����������ʻ֤�������� g_�����������ʻ֤��������[] = {
	{__LINE__, {}, {}},
	{__LINE__, {18, INT_MAX}, {С������, С���Զ�������, �м���ר��С���Զ����ؿ�����, ���Ħ�г�} },
	{__LINE__, {18, 60}, {�����ػ�����, ��������, ��ͨ����Ħ�г�, ��ͨ����Ħ�г�, ��ʽר�û�е��}},
	{__LINE__, {20, 60}, {���й�����, ���Ϳͳ�, ���ͻ���, ����ǣ���ҳ�, �޹�糵, �й�糵}},
	{__LINE__, {22, 60}, {���Ϳͳ�, ����ǣ���ҳ�}},
	{__LINE__, {19, 60}, {���Ϳͳ�, ����ǣ���ҳ�}, true},
};

std::string ��������2string(�������� ����) {
	std::string res = std::to_string(����.min) + "��������";
	if (����.max != INT_MAX) {
		res += "��" + std::to_string(����.max) + "��������";
	}
	return res;
}

std::vector<��������> get_valid_��������() {
	std::vector<��������> v;
	for (const auto& i : g_�����������ʻ֤��������) {
		if (!i.����.empty()) {
			v.push_back(i.��������);
		}
	}
	return v;
}

struct �����������ʻ֤����������Ŀ : ��Ŀbase {
	DECLARE_TYPE_ID
	�����������ʻ֤����������Ŀ() : ��Ŀbase(0) {}
	�����������ʻ֤����������Ŀ(int id, �����������ʻ֤�������� ����, size_t ����id) : ��Ŀbase(id), ����(����), ����id(����id) {}
	�����������ʻ֤�������� ����;
	size_t ����id;

	virtual std::string question() const override {
		char buf[1024];
		auto ���� = ����2string(����.����[����id]);
		if (����.ȫ����) {
			snprintf(buf, sizeof(buf), "����ȫ���Ƽ�ʻְҵ������ѧ��������%s׼�ݳ��͵ģ����������ڣ�", ����);
		} else {
			snprintf(buf, sizeof(buf), "����%s׼�ݳ��͵ģ����������ڣ�", ����);
		}
		return buf;
	}

	virtual std::string answer() const override {
		return ��������2string(����.��������);
	}

	virtual void print_options() const {
		int i = 1;
		const auto& valid_�������� = get_valid_��������();
		for (const auto& j : valid_��������) {
			printf("  %d.%s\n", i++, ��������2string(j).c_str());
		}
	}

	virtual bool is_ans_correct(int ans) const override {
		const auto& valid_�������� = get_valid_��������();
		return !(ans < 1 || ans >(int)valid_��������.size() || valid_��������[ans - 1] != ����.��������);
	}

	virtual bool read(FILE* f) override {
		if (!__super::read(f)) return false;
		read_element(����id);
		return true;
	}

	virtual bool write(FILE* f) const override {
		if (!__super::write(f)) return false;
		write_elment(����id);
		return true;
	}
};
typedef std::shared_ptr<�����������ʻ֤����������Ŀ> �����������ʻ֤����������ĿPtr;
