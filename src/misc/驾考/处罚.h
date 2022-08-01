#pragma once

#include "template.h"




struct 处罚题目 : 题目base {

	处罚题目(int id) : 题目base(id) {}
	

	virtual std::string answer() const override {
		//return std::to_string(扣分);
	}

	virtual bool is_ans_correct(int ans) const override {
		//return 扣分 == ans;
	}
};