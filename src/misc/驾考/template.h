#pragma once

#include <stdio.h>
#include <limits.h>
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <jlib/util/str_util.h>
#include <jlib/ansi_color_codes.h>

#define DECLARE_TYPE_ID static constexpr int type_id = __LINE__;

struct Stat {
	size_t total_correct_times = 0;
	size_t total_incorrect_times = 0;
	void clear() {
		memset(this, 0, sizeof(*this));
	}
};

struct ��Ŀbase {
	��Ŀbase(int id) : id(id) {}
	virtual ~��Ŀbase() {}

	DECLARE_TYPE_ID
	int id;
	bool answer_correct = false;
	int incorrect_times = 0;
	Stat stat{};

	virtual std::string question() const = 0;
	virtual std::string answer() const = 0;
	virtual bool is_ans_correct(int ans) const = 0;


	virtual void print_question() const { printf("%s\n", question().c_str()); }
	virtual void print_options() const {}
	virtual void print_answer() const { printf("%s\n", answer().c_str()); }
	virtual void check_answer(int ans) {
		if (is_ans_correct(ans)) {
			printf(GREEN("�ش���ȷ��\n"));
			answer_correct = true;
			stat.total_correct_times++;
		} else {
			printf(RED("�𰸴�����ȷ��Ϊ��%s\n"), answer().c_str());
			incorrect_times++;
			stat.total_incorrect_times++;
		}
	}

	virtual bool write(FILE* f) const {
#define write_elment(elem) if (fwrite(&(elem), 1, sizeof((elem)), f) != sizeof((elem))) { fclose(f); f=NULL; return false; }
		write_elment(id);
		write_elment(stat);
		return true;
	}

	virtual bool read(FILE* f) {
#define read_element(elem) if (fread(&(elem), 1, sizeof((elem)), f) != sizeof((elem))) { fclose(f); f=NULL; return false; }
		read_element(id);
		read_element(stat);
		return true;
	}
};

typedef std::shared_ptr<��Ŀbase> ��ĿbasePtr;

int get_��ѡ��Ŀ(const std::map<int, ��ĿbasePtr>& ��Ŀ�б�) {
	std::vector<int> ��ѡ��Ŀ;
	for (const auto& i : ��Ŀ�б�) {
		if (!i.second->answer_correct) {
			��ѡ��Ŀ.push_back(i.first);
		}
	}
	if (��ѡ��Ŀ.empty()) {
		return -1;
	} else {
		return ��ѡ��Ŀ[rand() % ��ѡ��Ŀ.size()];
	}
};

bool should_insert(const std::map<int, ��ĿbasePtr>& ��ѡ��Ŀ, int i, int  j) {
	auto ii = ��ѡ��Ŀ.find(i);
	auto jj = ��ѡ��Ŀ.find(j);
	if (ii == ��ѡ��Ŀ.end() || jj == ��ѡ��Ŀ.end()) { printf("kill me"); return false; }
	return jj->second->incorrect_times < ii->second->incorrect_times;
}

void print_ratio(const std::map<int, ��ĿbasePtr>& ��ѡ��Ŀ) {
	printf(GREEN("��ϲ���Ѿ�����������%u����Ŀ��\n"), ��ѡ��Ŀ.size());
	
	std::list<int> ids;
	for (const auto& i : ��ѡ��Ŀ) {
		if (i.second->incorrect_times > 0) {
			if (ids.empty()) {
				ids.push_back(i.first);
			} else {
				bool inserted = false;
				for (auto j = ids.begin(); j != ids.end(); j++) {
					if(should_insert(��ѡ��Ŀ, *j, i.first)) {
					//if (��ѡ��Ŀ[*j]->incorrect_times < ��ѡ��Ŀ[i.first]->incorrect_times) {
						ids.insert(j, i.first);
						inserted = true;
						break;
					}
				}
				if (!inserted) {
					ids.push_back(i.first);
				}
			}
		}
	}

	if (ids.empty()) {
		printf(GREEN("�üһ��Ȼһ���ⶼû������������\n"));
	} else {
		printf("��һ�������%u���⣬���ⰴ�մ���������Ϊ��\n", ids.size());
		for (const auto& i : ids) {
			const auto iter = ��ѡ��Ŀ.find(i);
			if (iter == ��ѡ��Ŀ.end()) continue;
			const auto& ans = iter->second;
			printf("���%d�Σ�%s\n��ȷ��Ϊ��%s\n\n",
				   ans->incorrect_times,
				   ans->question().c_str(),
				   ans->answer().c_str());
		}
		printf(GREEN("�������ͣ��������¿�Ŀһ����һ����\n"));
	}
	
};

void do_test(std::map<int, ��ĿbasePtr>& ��Ŀ�б�) {
	size_t answered_times = 0;

	while (1) {
		system("cls");

		int id = get_��ѡ��Ŀ(��Ŀ�б�);
		if (id == -1) {
			print_ratio(��Ŀ�б�);
			break;
		}

		auto& ��Ŀ = ��Ŀ�б�[id];
		��Ŀ->print_question();
		��Ŀ->print_options();

		int i = 0;
		scanf("%d", &i);
		��Ŀ->check_answer(i);

		answered_times++;
		system("pause");
	}
}
