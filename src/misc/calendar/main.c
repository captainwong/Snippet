#include <stdio.h>
#include "calendar.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static const char* weeks[] = { "一", "二", "三", "四", "五", "六", "日" };

static void printDate(const ChineseLunarDate* date) {
	printf("公历 %04d年%d月%d日 星期%s\n"
		   "\t%s\n"
		   "\t距离今天：%d天\n"
		   "\t%s\n"
		   "\t今年的第%d天\n"
		   "\t星座：%s\n"
		   "\t节日：%s\n"
		   "农历 %s年%s%s\n"
		   "\t%s%s年 %s%s月 %s%s日\n"
		   "\t节气：%s\n"
		   "\t节日：%s\n",

		   date->date.y, date->date.m, date->date.d, weeks[date->date.wday - 1],
		   date->istoday ? "今日" : "非今日",
		   date->diff,
		   date->date.isleap ? "闰年" : "平年",
		   date->date.yday,
		   astroToString(date->date.astro),
		   date->date.festival == -1 ? "" : festivalToString(date->date.festival),

		   lunarZodiacToString(date->lunar.zodiac), lunarMonthToString(date->lunar.m), lunarMDayToString(date->lunar.d),
		   heavenlyStemToString(date->lunar.gzy.gan), earthlyBranchesToString(date->lunar.gzy.zhi),
		   heavenlyStemToString(date->lunar.gzm.gan), earthlyBranchesToString(date->lunar.gzm.zhi),
		   heavenlyStemToString(date->lunar.gzd.gan), earthlyBranchesToString(date->lunar.gzd.zhi),
		   date->lunar.term == -1 ? "" : solarTermsToString(date->lunar.term),
		   date->lunar.festival == -1 ? "" : lunarFestivalToString(date->lunar.festival)
	);
}

static void printExtraInfo(int y, int m, int d) {
	ChineseLunarDate date;
	date.date.y = y;
	date.date.m = m;
	date.date.d = d;
	chineseLunarDate(&date);

	if (date.lunar.festival != -1) {
		printf("%-8s", lunarFestivalToString(date.lunar.festival));
	} else if (date.lunar.term != -1) {
		printf("%-8s", solarTermsToString(date.lunar.term));
	} else if (date.date.festival != -1) {
		printf("%-8s", festivalToString(date.date.festival));
	} else {
		if (date.lunar.d == 1) {
			if (date.lunar.isleapmonth) {
				char str[8] = "闰";
				strcat_s(str, sizeof(str) - strlen(str), lunarMonthToString(date.lunar.m));
				printf("%-8s", str);
			} else {
				printf("%-8s", lunarMonthToString(date.lunar.m));
			}
		} else {
			printf("%-8s", lunarMDayToString(date.lunar.d));
		}		
	}
}

enum PrintWhat {
	PrintDay,	// 打印日期
	PrintExtra, // 打印额外信息如节日、节气、农历等
	PrintBlank,	// 打印空行
};

static void printCalendar(int year, int month, int day) {
	ChineseLunarDate day1;
	day1.date.y = year;
	day1.date.m = month;
	day1.date.d = 1;
	chineseLunarDate(&day1);

	printf("\n");
	for (int i = 0; i < 7; i++) {
		printf("%-8s", weeks[i]);
		if (i == 6) { printf("\n"); }
	}

	enum PrintWhat pw = PrintDay;
	for (int d = -day1.date.wday + 2, w = 1; d <= day1.date.mdays; d++) {
		if (d > 0) {
			switch (pw) {
			case PrintDay:
				printf("%-8d", d);
				break;
			case PrintExtra:
				printExtraInfo(year, month, d);
				break;
			case PrintBlank:
				if (d == day) printf("%-8s", "****");
				else printf("%-8s", " ");
				break;
			}
		} else {
			printf("%-8s", " ");
		}
		if (w++ == 7 || d == day1.date.mdays) {
			printf("\n");
			int back = w - 1;
			w = 1;
			switch (pw) {
			case PrintDay: pw = PrintExtra; d -= back; break;
			case PrintExtra: pw = PrintBlank; d -= back; break;
			case PrintBlank: 
				if (d == day1.date.mdays) return; 
				pw = PrintDay; break;
			}
		}
	}
}

int main(int argc, char** argv)
{
	int y = 2022, m = 8, d = 1;
	if (argc == 4) {
		y = atoi(argv[1]);
		m = atoi(argv[2]);
		d = atoi(argv[3]);
	} else if (argc != 1) {
		printf("Usage: %s [y] [m] [d]\n", argv[0]);
		return 1;
	}

	ChineseLunarDate date;
	date.date.y = y;
	date.date.m = m;
	date.date.d = d;
	if (chineseLunarDate(&date)) {
		printDate(&date);
		printCalendar(date.date.y, date.date.m, date.date.d);
	} else {
		fprintf(stderr, "Not a valid date, supported range is 1900-1-31 to 2100-12-31\n");
		return 1;
	}

	return 0;
}
