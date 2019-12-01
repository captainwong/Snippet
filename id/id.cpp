#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <regex>
#include <time.h>
#include "../id_area_util/area.h"

//const auto& Area = *PArea;

class ID
{
	static constexpr const char Table[] = "0123456789X";
	static constexpr int Length = 18;
	static constexpr int fromChar(char c) { return c != 'X' ? (Table[c] - '0') : 10; }
	static constexpr char toChar(int c) { return Table[c]; }
	static constexpr const char* Regex = R"(^[1-9]\d{5}(18|19|([23]\d))\d{2}((0[1-9])|(10|11|12))(([0-2][1-9])|10|20|30|31)\d{3}[0-9Xx]$)";

	char id[Length + 1] = { 0 };
	bool valid_ = false;

	std::string area_id_ = {};
	std::string birth_year_ = {};
	std::string birth_month_ = {};
	std::string birth_mday_ = {};

	bool checkValid() {
		std::regex e(Regex);
		valid_ = std::regex_match(id, e);
		if (valid_) {
			area_id_.clear();
			std::copy(id, id + 6, std::back_inserter(area_id_));
			birth_year_.clear();
			std::copy(id + 6, id + 10, std::back_inserter(birth_year_));
			birth_month_.clear();
			std::copy(id + 10, id + 12, std::back_inserter(birth_month_));
			birth_mday_.clear();
			std::copy(id + 12, id + 14, std::back_inserter(birth_mday_));

			valid_ = checkChk();
		}

		return valid_;
	}

	void copy(const std::string& id) {
		if (id.size() == Length) {
			memcpy(this->id, id.data(), Length);
			checkValid();
		}
	}

public:
	ID() = default;
	explicit ID(const std::string& id) { copy(id); }
	ID(const ID& rhs) { copy(rhs.str()); }

	bool valid() const { return valid_; }
	std::string str() const { return id; }
	std::string areaId() const { return area_id_; }
	std::string areaName() const { auto i = Area.find(area_id_); if (i != Area.cend()) { return i->second; } return ""; }
	std::string birthYear() const { return birth_year_; }
	std::string birthMonth() const { return birth_month_; }
	std::string birthMday() const { return birth_mday_; }
	int age() const {
		time_t now = time(nullptr);
		auto tm = localtime(&now);
		tm->tm_year += 1900; tm->tm_mon += 1;
		int year = std::stoi(birth_year_);
		if (year == tm->tm_year) { return 0; }
		int month = std::stoi(birth_month_); 
		int mday = std::stoi(birth_mday_);
		if (month > tm->tm_mon || (month == tm->tm_mon && mday > tm->tm_mday)) {
			return tm->tm_year - year - 1;
		}
		return tm->tm_year - year;
	}

	//! 0 for female, 1 for male
	int sex() const {
		return (id[16] - '0') % 2;
	}

	int calcChk() {
		int chk = 0;
		for (int i = 0; i < Length - 1; i++) {
			chk += ((1 << (Length - 1 - i)) % 11) * (id[i] - '0');
		}
		chk = (12 - (chk % 11)) % 11;
		return chk;
	}

	bool checkChk() {
		return toChar(calcChk()) == id[Length - 1];
	}

	void print() {
		if (!valid_) { printf("Invalid ID!\n"); abort(); }
		printf("%s %s %3d %s %s\n", 
			   id,
			   (birth_year_ + "-" + birth_month_ + "-" + birth_mday_).data(), age(), sex() == 0 ? "女" : "男",
			   areaName().data());
	}

	static ID randomID() {
		ID rid;

		// random area id
		{
			int idx = rand() % Area.size();
			auto i = Area.cbegin(); 
			while (idx-- && i != Area.cend()) { 
				i++;
			}
			rid.area_id_ = i->first;
		}

		// random birthday
		{
			time_t now = time(nullptr);
			tm tm = *localtime(&now); tm.tm_isdst = -1;
			tm.tm_mon >= 6 ? (tm.tm_mday -= (rand() % 365)) : (tm.tm_mday += (rand() % 365));
			mktime(&tm);
			tm.tm_year -= rand() % tm.tm_year;
			mktime(&tm);
			auto ptm = &tm;
			rid.birth_year_ = std::to_string(ptm->tm_year + 1900);
			rid.birth_month_ = std::to_string(ptm->tm_mon + 1);
			if (rid.birth_month_.size() == 1) { rid.birth_month_.insert(0, 1, '0'); }
			rid.birth_mday_ = std::to_string(ptm->tm_mday);
			if (rid.birth_mday_.size() == 1) { rid.birth_mday_.insert(0, 1, '0'); }
		}

		// ramdom seq
		std::string seq = std::to_string(10 + rand() % 90);

		// ramdom sex
		std::string sex = std::to_string(rand() % 2);

		std::string withoutChk = rid.area_id_ + rid.birth_year_ + rid.birth_month_ + rid.birth_mday_ + seq + sex;
		memcpy(rid.id, withoutChk.data(), Length - 1);

		rid.id[Length - 1] = toChar(rid.calcChk());
		rid.checkValid();
		return rid;
	}
};

int main()
{
	srand((unsigned int)time(nullptr));
	for (int i = 0; i < 100; i++) {
		ID::randomID().print();
	}
}
