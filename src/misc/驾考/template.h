#pragma once

#include <stdio.h>
#include <limits.h>
#include <string>
#include <vector>
#include <list>
#include <jlib/util/str_util.h>
#include <jlib/ansi_color_codes.h>



struct ��Ŀbase {
	virtual ~��Ŀbase() {}
	bool answer_correct = false;
	int incorrect_times = 0;
	virtual std::string question() const = 0;
	virtual std::string answer() const = 0;
	virtual bool is_ans_correct(int ans) const = 0;
	virtual void print_question() {
		printf("%s\n", question().c_str());
	}
	virtual void print_options() const {}
	virtual void print_answer() {
		printf("%s\n", answer().c_str());
	}
	virtual void check_answer(int ans) {
		if (is_ans_correct(ans)) {
			printf(GREEN("�ش���ȷ��\n"));
			answer_correct = true;
		} else {
			printf(RED("�𰸴�����ȷ��Ϊ��%s\n"), answer().c_str());
			incorrect_times++;
		}
	}
};

template <typename T = ��Ŀbase>
int get_��ѡ��Ŀ(const std::vector<T>& ��Ŀ�б�) {
	std::vector<int> ��ѡ��Ŀ;
	for (size_t i = 0; i < ��Ŀ�б�.size(); i++) {
		if (!��Ŀ�б�[i].answer_correct) {
			��ѡ��Ŀ.push_back((int)i);
		}
	}
	if (��ѡ��Ŀ.empty()) {
		return -1;
	} else {
		return ��ѡ��Ŀ[rand() % ��ѡ��Ŀ.size()];
	}
};

template <typename T = ��Ŀbase>
void print_ratio(const std::vector<T>& ��ѡ��Ŀ) {
	std::list<size_t> ids;
	for (size_t i = 0; i < ��ѡ��Ŀ.size(); i++) {
		if (��ѡ��Ŀ[i].incorrect_times > 0) {
			if (ids.empty()) {
				ids.push_back(i);
			} else {
				bool inserted = false;
				for (auto j = ids.begin(); j != ids.end(); j++) {
					if (��ѡ��Ŀ[*j].incorrect_times < ��ѡ��Ŀ[i].incorrect_times) {
						ids.insert(j, i);
						inserted = true;
						break;
					}
				}
				if (!inserted) {
					ids.push_back(i);
				}
			}
		}
	}

	printf("���Ĵ��ⰴ�մ���������Ϊ��\n");
	for (const auto& i : ids) {
		printf("���%d�Σ�%s\n��ȷ��Ϊ��%s\n\n",
			   ��ѡ��Ŀ[i].incorrect_times,
			   ��ѡ��Ŀ[i].question().c_str(),
			   ��ѡ��Ŀ[i].answer().c_str());
	}
};

template<typename T = ��Ŀbase>
void do_test(std::vector<T>& ��Ŀ�б�) {
	size_t answered_times = 0;

	while (1) {
		system("cls");

		int id = get_��ѡ��Ŀ<T>(��Ŀ�б�);
		if (id == -1) {
			printf(GREEN("��ϲ���Ѿ������������Ŀ��\n"));
			if (answered_times > ��Ŀ�б�.size()) {
				print_ratio(��Ŀ�б�);
				printf("�������ͣ��������¿�Ŀһ����һ����\n");
			} else {
				printf(GREEN("�üһ��Ȼû�д�һ���⣬����������\n"));
			}
			break;
		}

		auto& ��Ŀ = ��Ŀ�б�[id];
		��Ŀ.print_question();
		��Ŀ.print_options();

		int i = 0;
		scanf("%d", &i);
		��Ŀ.check_answer(i);

		answered_times++;
		system("pause");
	}
}
