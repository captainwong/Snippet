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

struct 题目base {
	题目base(int id) : id(id) {}
	virtual ~题目base() {}

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
			printf(GREEN("回答正确！\n"));
			answer_correct = true;
			stat.total_correct_times++;
		} else {
			printf(RED("答案错误，正确答案为：%s\n"), answer().c_str());
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

typedef std::shared_ptr<题目base> 题目basePtr;

int get_备选题目(const std::map<int, 题目basePtr>& 题目列表) {
	std::vector<int> 待选题目;
	for (const auto& i : 题目列表) {
		if (!i.second->answer_correct) {
			待选题目.push_back(i.first);
		}
	}
	if (待选题目.empty()) {
		return -1;
	} else {
		return 待选题目[rand() % 待选题目.size()];
	}
};

bool should_insert(const std::map<int, 题目basePtr>& 备选题目, int i, int  j) {
	auto ii = 备选题目.find(i);
	auto jj = 备选题目.find(j);
	if (ii == 备选题目.end() || jj == 备选题目.end()) { printf("kill me"); return false; }
	return jj->second->incorrect_times < ii->second->incorrect_times;
}

void print_ratio(const std::map<int, 题目basePtr>& 备选题目) {
	printf(GREEN("恭喜你已经答完了所有%u道题目！\n"), 备选题目.size());
	
	std::list<int> ids;
	for (const auto& i : 备选题目) {
		if (i.second->incorrect_times > 0) {
			if (ids.empty()) {
				ids.push_back(i.first);
			} else {
				bool inserted = false;
				for (auto j = ids.begin(); j != ids.end(); j++) {
					if(should_insert(备选题目, *j, i.first)) {
					//if (备选题目[*j]->incorrect_times < 备选题目[i.first]->incorrect_times) {
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
		printf(GREEN("好家伙，居然一道题都没错，厉害厉害！\n"));
	} else {
		printf("您一共答错了%u道题，错题按照答错次数排序为：\n", ids.size());
		for (const auto& i : ids) {
			const auto iter = 备选题目.find(i);
			if (iter == 备选题目.end()) continue;
			const auto& ans = iter->second;
			printf("答错%d次：%s\n正确答案为：%s\n\n",
				   ans->incorrect_times,
				   ans->question().c_str(),
				   ans->answer().c_str());
		}
		printf(GREEN("继续加油，距离拿下科目一更进一步！\n"));
	}
	
};

void do_test(std::map<int, 题目basePtr>& 题目列表) {
	size_t answered_times = 0;

	while (1) {
		system("cls");

		int id = get_备选题目(题目列表);
		if (id == -1) {
			print_ratio(题目列表);
			break;
		}

		auto& 题目 = 题目列表[id];
		题目->print_question();
		题目->print_options();

		int i = 0;
		scanf("%d", &i);
		题目->check_answer(i);

		answered_times++;
		system("pause");
	}
}
