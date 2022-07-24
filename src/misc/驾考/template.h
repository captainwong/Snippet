#pragma once

#include <stdio.h>
#include <limits.h>
#include <string>
#include <vector>
#include <list>
#include <jlib/util/str_util.h>
#include <jlib/ansi_color_codes.h>



struct 题目base {
	virtual ~题目base() {}
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
			printf(GREEN("回答正确！\n"));
			answer_correct = true;
		} else {
			printf(RED("答案错误，正确答案为：%s\n"), answer().c_str());
			incorrect_times++;
		}
	}
};

template <typename T = 题目base>
int get_备选题目(const std::vector<T>& 题目列表) {
	std::vector<int> 待选题目;
	for (size_t i = 0; i < 题目列表.size(); i++) {
		if (!题目列表[i].answer_correct) {
			待选题目.push_back((int)i);
		}
	}
	if (待选题目.empty()) {
		return -1;
	} else {
		return 待选题目[rand() % 待选题目.size()];
	}
};

template <typename T = 题目base>
void print_ratio(const std::vector<T>& 备选题目) {
	std::list<size_t> ids;
	for (size_t i = 0; i < 备选题目.size(); i++) {
		if (备选题目[i].incorrect_times > 0) {
			if (ids.empty()) {
				ids.push_back(i);
			} else {
				bool inserted = false;
				for (auto j = ids.begin(); j != ids.end(); j++) {
					if (备选题目[*j].incorrect_times < 备选题目[i].incorrect_times) {
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

	printf("您的错题按照答错次数排序为：\n");
	for (const auto& i : ids) {
		printf("答错%d次：%s\n正确答案为：%s\n\n",
			   备选题目[i].incorrect_times,
			   备选题目[i].question().c_str(),
			   备选题目[i].answer().c_str());
	}
};

template<typename T = 题目base>
void do_test(std::vector<T>& 题目列表) {
	size_t answered_times = 0;

	while (1) {
		system("cls");

		int id = get_备选题目<T>(题目列表);
		if (id == -1) {
			printf(GREEN("恭喜你已经答对了所有题目！\n"));
			if (answered_times > 题目列表.size()) {
				print_ratio(题目列表);
				printf("继续加油，距离拿下科目一更进一步！\n");
			} else {
				printf(GREEN("好家伙，居然没有错一道题，厉害厉害！\n"));
			}
			break;
		}

		auto& 题目 = 题目列表[id];
		题目.print_question();
		题目.print_options();

		int i = 0;
		scanf("%d", &i);
		题目.check_answer(i);

		answered_times++;
		system("pause");
	}
}
