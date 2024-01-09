#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calendar.h"

#define CN_FORMAT "%-8s"

static const char* weeks[] = {"一", "二", "三", "四", "五", "六", "日"};

static size_t cn_strlen(const char* str) {
    static const size_t cn_char_width = sizeof("中") - 1;
    size_t len = 0;
    while (*str) {
        if (*str & 0x80) {
            str += cn_char_width;
        } else {
            str++;
        }
        len++;
    }
    return len;
}

// works with fixed width fonts which have the same width for all characters
// one Chinese character is 2 English characters wide
// e.g. use "Noto Sans Mono CJK SC" font
static void printFixedWidthCN(const char* str, size_t width) {
    size_t len = cn_strlen(str) * 2;
    printf("%s", str);
    while (len++ < width) {
        printf(" ");
    }
}

static void printDate(const ChineseLunarDate* date) {
    printf(
        "公历 %04d年%d月%d日 星期%s\n"
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
        date->lunar.festival == -1 ? "" : lunarFestivalToString(date->lunar.festival));
}

static void printExtraInfo(int y, int m, int d) {
    ChineseLunarDate date;
    date.date.y = y;
    date.date.m = m;
    date.date.d = d;
    chineseLunarDate(&date);

    if (date.lunar.festival != -1) {
        printFixedWidthCN(lunarFestivalToString(date.lunar.festival), 8);
    } else if (date.lunar.term != -1) {
        printFixedWidthCN(solarTermsToString(date.lunar.term), 8);
    } else if (date.date.festival != -1) {
        printFixedWidthCN(festivalToString(date.date.festival), 8);
    } else {
        if (date.lunar.d == 1) {
            if (date.lunar.isleapmonth) {
                char str[16] = "闰";
                strcat(str, lunarMonthToString(date.lunar.m));
                printFixedWidthCN(str, 8);
            } else {
                printFixedWidthCN(lunarMonthToString(date.lunar.m), 8);
            }
        } else {
            printFixedWidthCN(lunarMDayToString(date.lunar.d), 8);
        }
    }
}

enum PrintWhat {
    PrintDay,    // 打印日期
    PrintExtra,  // 打印额外信息如节日、节气、农历等
    PrintBlank,  // 打印空行
};

static void printCalendar(int year, int month, int day) {
    ChineseLunarDate day1;
    day1.date.y = year;
    day1.date.m = month;
    day1.date.d = 1;
    chineseLunarDate(&day1);

    printf("\n");
    for (int i = 0; i < 7; i++) {
        printFixedWidthCN(weeks[i], 8);
        if (i == 6) {
            printf("\n");
        }
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
                    if (d == day)
                        printf(CN_FORMAT, "****");
                    else
                        printf(CN_FORMAT, " ");
                    break;
            }
        } else {
            printf(CN_FORMAT, " ");
        }
        if (w++ == 7 || d == day1.date.mdays) {
            printf("\n");
            int back = w - 1;
            w = 1;
            switch (pw) {
                case PrintDay:
                    pw = PrintExtra;
                    d -= back;
                    break;
                case PrintExtra:
                    pw = PrintBlank;
                    d -= back;
                    break;
                case PrintBlank:
                    if (d == day1.date.mdays) return;
                    pw = PrintDay;
                    break;
            }
        }
    }
}

int main(int argc, char** argv) {
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
