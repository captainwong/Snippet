#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "calendar.h"

#include <stdio.h>   // snprintf
#include <stdlib.h>  // strtol
#include <string.h>  // memcpy
#include <time.h>    // time, localtime, mktime...

struct FestivalDate {
    const char* name;
    int m, d;
    enum Festival festival;
};

static const struct FestivalDate festivals[] = {
    {"新年", 1, 1, F11_NewYear},
    {"情人节", 2, 14, F214_ValentinesDay},
    {"妇女节", 3, 8, F38_WomensDay},
    {"植树节", 3, 12, F312_ArborDay},
    {"愚人节", 4, 1, F41_AprilFoolsDay},
    {"劳动节", 5, 1, F51_LabourDay},
    {"青年节", 5, 4, F54_YouthDay},
    {"护士节", 5, 12, F512_NurseDay},
    {"儿童节", 6, 1, F61_ChildrensDay},
    {"建党节", 7, 1, F71_PartysDay},
    {"建军节", 8, 1, F81_ArmyDay},
    {"教师节", 9, 10, F910_TeachersDay},
    {"国耻日", 9, 18, F918_NationalShameDay},
    {"国庆节", 10, 1, F101_NationalDay},
    {"平安夜", 12, 24, F1224_ChristmasEve},
    {"圣诞节", 12, 25, F1225_Christmas},
    {NULL, 0, 0, -1}};

static const struct FestivalDate lunarFestivals[] = {
    {"春节", 1, 1, LF11_SprintFestival},
    {"元宵", 1, 15, LF115_ShangYuanFestival},
    {"龙抬头", 2, 2, LF22_DargonRaisesHeadDay},
    {"端午", 5, 5, LF55_DragonBoatFestival},
    {"七夕", 7, 7, LF77_QiXiFestival},
    {"中元", 7, 15, LF715_ZhongYuanFestival},
    {"中秋", 8, 15, LF815_MoonFestival},
    {"重阳", 9, 9, LF99_ChongYangFestival},
    {"寒衣", 10, 1, LF101_WinterClothingFestival},
    {"下元", 10, 15, LF1015_XiaYuanFestival},
    {"腊八", 12, 8, LF128_LaBaFestival},
    {"北方小年", 12, 23, LF1223_XiaoNian},
    {"南方小年", 12, 24, LF1224_XiaoNian},
    {"除夕", 12, 29, LF1229_NewYearsEve},
    {"除夕", 12, 30, LF1230_NewYearsEve},
    {NULL, 0, 0, -1}};

// 公历每月天数
static const int days_of_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 公历1900-2100 闰大小信息表
static const unsigned int lunarInfo[] = {
    0x04bd8, 0x04ae0, 0x0a570, 0x054d5, 0x0d260, 0x0d950, 0x16554, 0x056a0, 0x09ad0, 0x055d2,  // 1900-1909
    0x04ae0, 0x0a5b6, 0x0a4d0, 0x0d250, 0x1d255, 0x0b540, 0x0d6a0, 0x0ada2, 0x095b0, 0x14977,  // 1910-1919
    0x04970, 0x0a4b0, 0x0b4b5, 0x06a50, 0x06d40, 0x1ab54, 0x02b60, 0x09570, 0x052f2, 0x04970,  // 1920-1929
    0x06566, 0x0d4a0, 0x0ea50, 0x16a95, 0x05ad0, 0x02b60, 0x186e3, 0x092e0, 0x1c8d7, 0x0c950,  // 1930-1939
    0x0d4a0, 0x1d8a6, 0x0b550, 0x056a0, 0x1a5b4, 0x025d0, 0x092d0, 0x0d2b2, 0x0a950, 0x0b557,  // 1940-1949
    0x06ca0, 0x0b550, 0x15355, 0x04da0, 0x0a5b0, 0x14573, 0x052b0, 0x0a9a8, 0x0e950, 0x06aa0,  // 1950-1959
    0x0aea6, 0x0ab50, 0x04b60, 0x0aae4, 0x0a570, 0x05260, 0x0f263, 0x0d950, 0x05b57, 0x056a0,  // 1960-1969
    0x096d0, 0x04dd5, 0x04ad0, 0x0a4d0, 0x0d4d4, 0x0d250, 0x0d558, 0x0b540, 0x0b6a0, 0x195a6,  // 1970-1979
    0x095b0, 0x049b0, 0x0a974, 0x0a4b0, 0x0b27a, 0x06a50, 0x06d40, 0x0af46, 0x0ab60, 0x09570,  // 1980-1989
    0x04af5, 0x04970, 0x064b0, 0x074a3, 0x0ea50, 0x06b58, 0x05ac0, 0x0ab60, 0x096d5, 0x092e0,  // 1990-1999
    0x0c960, 0x0d954, 0x0d4a0, 0x0da50, 0x07552, 0x056a0, 0x0abb7, 0x025d0, 0x092d0, 0x0cab5,  // 2000-2009
    0x0a950, 0x0b4a0, 0x0baa4, 0x0ad50, 0x055d9, 0x04ba0, 0x0a5b0, 0x15176, 0x052b0, 0x0a930,  // 2010-2019
    0x07954, 0x06aa0, 0x0ad50, 0x05b52, 0x04b60, 0x0a6e6, 0x0a4e0, 0x0d260, 0x0ea65, 0x0d530,  // 2020-2029
    0x05aa0, 0x076a3, 0x096d0, 0x04afb, 0x04ad0, 0x0a4d0, 0x1d0b6, 0x0d250, 0x0d520, 0x0dd45,  // 2030-2039
    0x0b5a0, 0x056d0, 0x055b2, 0x049b0, 0x0a577, 0x0a4b0, 0x0aa50, 0x1b255, 0x06d20, 0x0ada0,  // 2040-2049
    /**Add By JJonline@JJonline.Cn**/
    0x14b63, 0x09370, 0x049f8, 0x04970, 0x064b0, 0x168a6, 0x0ea50, 0x06b20, 0x1a6c4, 0x0aae0,  // 2050-2059
    0x092e0, 0x0d2e3, 0x0c960, 0x0d557, 0x0d4a0, 0x0da50, 0x05d55, 0x056a0, 0x0a6d0, 0x055d4,  // 2060-2069
    0x052d0, 0x0a9b8, 0x0a950, 0x0b4a0, 0x0b6a6, 0x0ad50, 0x055a0, 0x0aba4, 0x0a5b0, 0x052b0,  // 2070-2079
    0x0b273, 0x06930, 0x07337, 0x06aa0, 0x0ad50, 0x14b55, 0x04b60, 0x0a570, 0x054e4, 0x0d160,  // 2080-2089
    0x0e968, 0x0d520, 0x0daa0, 0x16aa6, 0x056d0, 0x04ae0, 0x0a9d4, 0x0a2d0, 0x0d150, 0x0f252,  // 2090-2099
    0x0d520                                                                                    // 2100
};

// 公元1900-2100 24节气日期速查表
static const char* termInfo[] = {
    "9778397bd097c36b0b6fc9274c91aa", "97b6b97bd19801ec9210c965cc920e", "97bcf97c3598082c95f8c965cc920f",
    "97bd0b06bdb0722c965ce1cfcc920f", "b027097bd097c36b0b6fc9274c91aa", "97b6b97bd19801ec9210c965cc920e",
    "97bcf97c359801ec95f8c965cc920f", "97bd0b06bdb0722c965ce1cfcc920f", "b027097bd097c36b0b6fc9274c91aa",
    "97b6b97bd19801ec9210c965cc920e", "97bcf97c359801ec95f8c965cc920f", "97bd0b06bdb0722c965ce1cfcc920f",
    "b027097bd097c36b0b6fc9274c91aa", "9778397bd19801ec9210c965cc920e", "97b6b97bd19801ec95f8c965cc920f",
    "97bd09801d98082c95f8e1cfcc920f", "97bd097bd097c36b0b6fc9210c8dc2", "9778397bd197c36c9210c9274c91aa",
    "97b6b97bd19801ec95f8c965cc920e", "97bd09801d98082c95f8e1cfcc920f", "97bd097bd097c36b0b6fc9210c8dc2",
    "9778397bd097c36c9210c9274c91aa", "97b6b97bd19801ec95f8c965cc920e", "97bcf97c3598082c95f8e1cfcc920f",
    "97bd097bd097c36b0b6fc9210c8dc2", "9778397bd097c36c9210c9274c91aa", "97b6b97bd19801ec9210c965cc920e",
    "97bcf97c3598082c95f8c965cc920f", "97bd097bd097c35b0b6fc920fb0722", "9778397bd097c36b0b6fc9274c91aa",
    "97b6b97bd19801ec9210c965cc920e", "97bcf97c3598082c95f8c965cc920f", "97bd097bd097c35b0b6fc920fb0722",
    "9778397bd097c36b0b6fc9274c91aa", "97b6b97bd19801ec9210c965cc920e", "97bcf97c359801ec95f8c965cc920f",
    "97bd097bd097c35b0b6fc920fb0722", "9778397bd097c36b0b6fc9274c91aa", "97b6b97bd19801ec9210c965cc920e",
    "97bcf97c359801ec95f8c965cc920f", "97bd097bd097c35b0b6fc920fb0722", "9778397bd097c36b0b6fc9274c91aa",
    "97b6b97bd19801ec9210c965cc920e", "97bcf97c359801ec95f8c965cc920f", "97bd097bd07f595b0b6fc920fb0722",
    "9778397bd097c36b0b6fc9210c8dc2", "9778397bd19801ec9210c9274c920e", "97b6b97bd19801ec95f8c965cc920f",
    "97bd07f5307f595b0b0bc920fb0722", "7f0e397bd097c36b0b6fc9210c8dc2", "9778397bd097c36c9210c9274c920e",
    "97b6b97bd19801ec95f8c965cc920f", "97bd07f5307f595b0b0bc920fb0722", "7f0e397bd097c36b0b6fc9210c8dc2",
    "9778397bd097c36c9210c9274c91aa", "97b6b97bd19801ec9210c965cc920e", "97bd07f1487f595b0b0bc920fb0722",
    "7f0e397bd097c36b0b6fc9210c8dc2", "9778397bd097c36b0b6fc9274c91aa", "97b6b97bd19801ec9210c965cc920e",
    "97bcf7f1487f595b0b0bb0b6fb0722", "7f0e397bd097c35b0b6fc920fb0722", "9778397bd097c36b0b6fc9274c91aa",
    "97b6b97bd19801ec9210c965cc920e", "97bcf7f1487f595b0b0bb0b6fb0722", "7f0e397bd097c35b0b6fc920fb0722",
    "9778397bd097c36b0b6fc9274c91aa", "97b6b97bd19801ec9210c965cc920e", "97bcf7f1487f531b0b0bb0b6fb0722",
    "7f0e397bd097c35b0b6fc920fb0722", "9778397bd097c36b0b6fc9274c91aa", "97b6b97bd19801ec9210c965cc920e",
    "97bcf7f1487f531b0b0bb0b6fb0722", "7f0e397bd07f595b0b6fc920fb0722", "9778397bd097c36b0b6fc9274c91aa",
    "97b6b97bd19801ec9210c9274c920e", "97bcf7f0e47f531b0b0bb0b6fb0722", "7f0e397bd07f595b0b0bc920fb0722",
    "9778397bd097c36b0b6fc9210c91aa", "97b6b97bd197c36c9210c9274c920e", "97bcf7f0e47f531b0b0bb0b6fb0722",
    "7f0e397bd07f595b0b0bc920fb0722", "9778397bd097c36b0b6fc9210c8dc2", "9778397bd097c36c9210c9274c920e",
    "97b6b7f0e47f531b0723b0b6fb0722", "7f0e37f5307f595b0b0bc920fb0722", "7f0e397bd097c36b0b6fc9210c8dc2",
    "9778397bd097c36b0b70c9274c91aa", "97b6b7f0e47f531b0723b0b6fb0721", "7f0e37f1487f595b0b0bb0b6fb0722",
    "7f0e397bd097c35b0b6fc9210c8dc2", "9778397bd097c36b0b6fc9274c91aa", "97b6b7f0e47f531b0723b0b6fb0721",
    "7f0e27f1487f595b0b0bb0b6fb0722", "7f0e397bd097c35b0b6fc920fb0722", "9778397bd097c36b0b6fc9274c91aa",
    "97b6b7f0e47f531b0723b0b6fb0721", "7f0e27f1487f531b0b0bb0b6fb0722", "7f0e397bd097c35b0b6fc920fb0722",
    "9778397bd097c36b0b6fc9274c91aa", "97b6b7f0e47f531b0723b0b6fb0721", "7f0e27f1487f531b0b0bb0b6fb0722",
    "7f0e397bd097c35b0b6fc920fb0722", "9778397bd097c36b0b6fc9274c91aa", "97b6b7f0e47f531b0723b0b6fb0721",
    "7f0e27f1487f531b0b0bb0b6fb0722", "7f0e397bd07f595b0b0bc920fb0722", "9778397bd097c36b0b6fc9274c91aa",
    "97b6b7f0e47f531b0723b0787b0721", "7f0e27f0e47f531b0b0bb0b6fb0722", "7f0e397bd07f595b0b0bc920fb0722",
    "9778397bd097c36b0b6fc9210c91aa", "97b6b7f0e47f149b0723b0787b0721", "7f0e27f0e47f531b0723b0b6fb0722",
    "7f0e397bd07f595b0b0bc920fb0722", "9778397bd097c36b0b6fc9210c8dc2", "977837f0e37f149b0723b0787b0721",
    "7f07e7f0e47f531b0723b0b6fb0722", "7f0e37f5307f595b0b0bc920fb0722", "7f0e397bd097c35b0b6fc9210c8dc2",
    "977837f0e37f14998082b0787b0721", "7f07e7f0e47f531b0723b0b6fb0721", "7f0e37f1487f595b0b0bb0b6fb0722",
    "7f0e397bd097c35b0b6fc9210c8dc2", "977837f0e37f14998082b0787b06bd", "7f07e7f0e47f531b0723b0b6fb0721",
    "7f0e27f1487f531b0b0bb0b6fb0722", "7f0e397bd097c35b0b6fc920fb0722", "977837f0e37f14998082b0787b06bd",
    "7f07e7f0e47f531b0723b0b6fb0721", "7f0e27f1487f531b0b0bb0b6fb0722", "7f0e397bd097c35b0b6fc920fb0722",
    "977837f0e37f14998082b0787b06bd", "7f07e7f0e47f531b0723b0b6fb0721", "7f0e27f1487f531b0b0bb0b6fb0722",
    "7f0e397bd07f595b0b0bc920fb0722", "977837f0e37f14998082b0787b06bd", "7f07e7f0e47f531b0723b0b6fb0721",
    "7f0e27f1487f531b0b0bb0b6fb0722", "7f0e397bd07f595b0b0bc920fb0722", "977837f0e37f14998082b0787b06bd",
    "7f07e7f0e47f149b0723b0787b0721", "7f0e27f0e47f531b0b0bb0b6fb0722", "7f0e397bd07f595b0b0bc920fb0722",
    "977837f0e37f14998082b0723b06bd", "7f07e7f0e37f149b0723b0787b0721", "7f0e27f0e47f531b0723b0b6fb0722",
    "7f0e397bd07f595b0b0bc920fb0722", "977837f0e37f14898082b0723b02d5", "7ec967f0e37f14998082b0787b0721",
    "7f07e7f0e47f531b0723b0b6fb0722", "7f0e37f1487f595b0b0bb0b6fb0722", "7f0e37f0e37f14898082b0723b02d5",
    "7ec967f0e37f14998082b0787b0721", "7f07e7f0e47f531b0723b0b6fb0722", "7f0e37f1487f531b0b0bb0b6fb0722",
    "7f0e37f0e37f14898082b0723b02d5", "7ec967f0e37f14998082b0787b06bd", "7f07e7f0e47f531b0723b0b6fb0721",
    "7f0e37f1487f531b0b0bb0b6fb0722", "7f0e37f0e37f14898082b072297c35", "7ec967f0e37f14998082b0787b06bd",
    "7f07e7f0e47f531b0723b0b6fb0721", "7f0e27f1487f531b0b0bb0b6fb0722", "7f0e37f0e37f14898082b072297c35",
    "7ec967f0e37f14998082b0787b06bd", "7f07e7f0e47f531b0723b0b6fb0721", "7f0e27f1487f531b0b0bb0b6fb0722",
    "7f0e37f0e366aa89801eb072297c35", "7ec967f0e37f14998082b0787b06bd", "7f07e7f0e47f149b0723b0787b0721",
    "7f0e27f1487f531b0b0bb0b6fb0722", "7f0e37f0e366aa89801eb072297c35", "7ec967f0e37f14998082b0723b06bd",
    "7f07e7f0e47f149b0723b0787b0721", "7f0e27f0e47f531b0723b0b6fb0722", "7f0e37f0e366aa89801eb072297c35",
    "7ec967f0e37f14998082b0723b06bd", "7f07e7f0e37f14998083b0787b0721", "7f0e27f0e47f531b0723b0b6fb0722",
    "7f0e37f0e366aa89801eb072297c35", "7ec967f0e37f14898082b0723b02d5", "7f07e7f0e37f14998082b0787b0721",
    "7f07e7f0e47f531b0723b0b6fb0722", "7f0e36665b66aa89801e9808297c35", "665f67f0e37f14898082b0723b02d5",
    "7ec967f0e37f14998082b0787b0721", "7f07e7f0e47f531b0723b0b6fb0722", "7f0e36665b66a449801e9808297c35",
    "665f67f0e37f14898082b0723b02d5", "7ec967f0e37f14998082b0787b06bd", "7f07e7f0e47f531b0723b0b6fb0721",
    "7f0e36665b66a449801e9808297c35", "665f67f0e37f14898082b072297c35", "7ec967f0e37f14998082b0787b06bd",
    "7f07e7f0e47f531b0723b0b6fb0721", "7f0e26665b66a449801e9808297c35", "665f67f0e37f1489801eb072297c35",
    "7ec967f0e37f14998082b0787b06bd", "7f07e7f0e47f531b0723b0b6fb0721", "7f0e27f1487f531b0b0bb0b6fb0722"};

// 是否公历闰年
static int is_leap_year(int year) {
    return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

// 是否公历有效日期
static int is_valid_date(int y, int m, int d) {
    if (y < CALENDAR_YEAR_MIN || y > CALENDAR_YEAR_MAX) return 0;
    if (y == CALENDAR_YEAR_MIN && m == 1 && d < 31) return 0;
    if (m < 1 || m > 12) return 0;
    if (d < 1) return 0;
    if (m == 2 && is_leap_year(y)) {
        return d <= 29;
    } else {
        return d <= days_of_month[m - 1];
    }
}

// 公历当年有多少天
static int daysInYear(int y) {
    return 365 + is_leap_year(y);
}

// 公历当月有多少天
static int daysInMonth(int y, int m) {
    if (m == 2 && is_leap_year(y)) {
        return 29;
    }
    return days_of_month[m - 1];
}

// 获取今天公历日期
static struct tm tmtoday() {
    time_t t = time(NULL);
    return *localtime(&t);
}

// 公历1900年1月1日到date过去的天数
// 示例 2022年7月16日 返回44756
static int daysSinceEpoch(int y, int m, int d) {
    int days = -1;
    for (int i = CALENDAR_YEAR_MIN; i < y; i++) {
        days += daysInYear(i);
    }
    for (int i = 0; i < m - 1; i++) {
        days += daysInMonth(y, i + 1);
    }
    days += d;
    return days;
}

// 获取公历日期是当年的第几天
static int getYDay(int year, int m, int d) {
    int yday = d;
    for (int i = 0; i < m - 1; i++) {
        yday += days_of_month[i];
        if (i == 1 && is_leap_year(year)) {
            yday++;
        }
    }
    return yday;
}

// 获取公历日期是周几
static int getWDay(int y, int m, int d) {
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    tm->tm_year = y - 1900;
    tm->tm_mon = m - 1;
    tm->tm_mday = d;
    mktime(tm);
    return tm->tm_wday == 0 ? 7 : tm->tm_wday;
}

// 公历日期是什么星座
static enum AstrologicalSign getAstro(int m, int d) {
    static const int mdays[12] = {20, 19, 21, 21, 21, 22, 23, 23, 23, 23, 22, 22};
    return m - ((d < mdays[m - 1]) ? 1 : 0);
}

// 公历日期是什么节日
static enum Festival getFestival(int m, int d) {
    const struct FestivalDate* p = festivals;
    while (p->name) {
        if (p->m == m && p->d == d) {
            return p->festival;
        }
        p++;
    }
    return -1;
}

// 农历y年闰几月（1-12），无闰月返回0
static int lunarLeapMonth(int y) {
    return lunarInfo[y - CALENDAR_YEAR_MIN] & 0x0f;
}

// 农历y年闰月天数
static int lunarLeapDays(int y) {
    if (lunarLeapMonth(y)) {
        return (lunarInfo[y - CALENDAR_YEAR_MIN] & 0x10000) ? 30 : 29;
    }
    return 0;
}

// 农历y年m月天数，m月非闰月，如需计算闰月天数调用lunarLeapDays
static int lunarMonthDays(int y, int m) {
    return (lunarInfo[y - CALENDAR_YEAR_MIN] & (0x10000 >> m)) ? 30 : 29;
}

// 农历y年一整年总天数
static int lunarYearDays(int y) {
    int sum = 348;
    for (int i = 0x8000; i > 0x8; i >>= 1) {
        sum += (lunarInfo[y - CALENDAR_YEAR_MIN] & i) ? 1 : 0;
    }
    return sum + lunarLeapDays(y);
}

// 农历y年生肖
static enum ChineseZodiac lunarZodiac(int y) {
    return (y - 4) % 12;
}

// 农历y年转为干支纪年
static void lunarGanZhiYear(int y, struct GanZhi* gz) {
    // 1925年是甲子年
    gz->gan = abs(y + 1 - 1925) % HS_COUNT;
    gz->zhi = abs(y + 1 - 1925) % EB_COUNT;
}

// 公历y年第n个节气的日期（mday）
// n [1, 24]
static int mdayOfTerm(int y, int n) {
    const char* table = termInfo[y - CALENDAR_YEAR_MIN];
    char buf[7] = {0};
    memcpy(buf, table + ((n - 1) / 4) * 5, 5);
    unsigned long chunk = strtol(buf, NULL, 16);
    snprintf(buf, sizeof(buf), "%lu", chunk);
    int offsets[] = {0, 1, 3, 4};
    int offset = offsets[(n - 1) % 4];
    int bytes = (n % 2) ? 1 : 2;
    memmove(buf, buf + offset, bytes);
    buf[bytes] = '\0';
    int mday = atoi(buf);
    return mday;
}

static void offsetToGanZhi(int offset, struct GanZhi* gz) {
    gz->gan = offset % HS_COUNT;
    gz->zhi = offset % EB_COUNT;
}

// 以公历日期计算月份干支
// 月份干支以中气开始，即立春、惊蛰、清明等奇数节气开始换月
static void lunarGanZhiMonth(int y, int m, int d, struct GanZhi* gz) {
    // 1899年1月5日是小寒，小寒之前是甲子月，之后是乙丑月
    // 这里计算距离1899年1月过去的月数来计算月份干支
    // 如果当月日期在当月的中气之后，是下一个干支月
    int offset = abs(y - 1900) * 12 + m + 11;
    gz->gan = offset % HS_COUNT;
    gz->zhi = offset % EB_COUNT;
    int mdayTerm = mdayOfTerm(y, m * 2 - 1);
    if (d >= mdayTerm) {
        offsetToGanZhi(++offset, gz);
    }
}

// 以公历日期计算干支日
static void lunarGanZhiDay(int y, int m, int d, struct GanZhi* gz) {
    // 距离1900/1/1之前10天的天数。1900/1/1往前10天是甲子日。
    int days = daysSinceEpoch(y, m, d) + 10;
    offsetToGanZhi(days, gz);
}

// 以公历日期计算农历节气
static enum SolarTerms lunarTerm(int y, int m, int d) {
    int term1Mday = mdayOfTerm(y, m * 2 - 1);
    int term2Mday = mdayOfTerm(y, m * 2);
    if (d == term1Mday) {
        return m * 2 - 2;
    }

    if (d == term2Mday) {
        return m * 2 - 1;
    }

    return -1;
}

// 农历日期是什么节日
static enum LunarFestival getLunarFestival(int y, int m, int d) {
    const struct FestivalDate* p = lunarFestivals;
    while (p->name) {
        if (p->m == m && p->d == d) {
            if (m == 12 && d == 29) {  // 特殊处理12/29，如果当年是小年才返回，否则不是节日
                if (lunarMonthDays(y, 12) == 29) {
                    return p->festival;
                }
            } else {
                return p->festival;
            }
        }
        p++;
    }
    return -1;
}

// 公历日期tm转农历日期
static void lunarDate(int y, int m, int d, int* ly, int* lm, int* ld) {
    int days = daysSinceEpoch(y, m, d) - 30;  // 距离1900年1月31日过去的天数
    int ydays = 0, lunarYear;
    for (lunarYear = CALENDAR_YEAR_MIN; lunarYear < CALENDAR_YEAR_MAX + 1 && days > 0; lunarYear++) {
        ydays = lunarYearDays(lunarYear);
        days -= ydays;
    }
    if (days < 0) {
        days += ydays;
        lunarYear--;
    }
    *ly = lunarYear;

    int leapMonth = lunarLeapMonth(lunarYear), isleap = 0, mdays = 0, month;
    for (month = 1; month < 13 && days > 0; month++) {
        if (leapMonth > 0 && month == (leapMonth + 1) && !isleap) {
            --month;
            isleap = 1;
            mdays = lunarLeapDays(*ly);
        } else {
            mdays = lunarMonthDays(*ly, month);
        }
        if (isleap && month == leapMonth + 1) {
            isleap = 0;
        }
        days -= mdays;
    }
    if (days == 0 && leapMonth > 0 && month == leapMonth + 1) {
        if (isleap) {
            isleap = 0;
        } else {
            isleap = 1;
            month--;
        }
    }
    if (days < 0) {
        days += mdays;
        month--;
    }
    *lm = month;
    *ld = days + 1;
}

// 计算公历日期距离今天的天数
static int diffFromToday(int y, int m, int d) {
    struct tm tm = tmtoday();
    return daysSinceEpoch(y, m, d) - daysSinceEpoch(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

const char* heavenlyStemToString(enum HeavenlyStem hs) {
    switch (hs) {
        case HS_JIA:
            return "甲";
        case HS_YI:
            return "乙";
        case HS_BING:
            return "丙";
        case HS_DING:
            return "丁";
        case HS_WU:
            return "戊";
        case HS_JI:
            return "己";
        case HS_GENG:
            return "庚";
        case HS_XIN:
            return "辛";
        case HS_REN:
            return "壬";
        case HS_GUI:
            return "癸";
        default:
            return "Invalid HeavenlyStem";
    }
}

const char* earthlyBranchesToString(enum EarthlyBranch eb) {
    switch (eb) {
        case EB_ZI:
            return "子";
        case EB_CHOU:
            return "丑";
        case EB_YIN:
            return "寅";
        case EB_MAO:
            return "卯";
        case EB_CHEN:
            return "辰";
        case EB_SI:
            return "巳";
        case EB_WU:
            return "午";
        case EB_WEI:
            return "未";
        case EB_SHEN:
            return "申";
        case EB_YOU:
            return "酉";
        case EB_XU:
            return "戌";
        case EB_HAI:
            return "亥";
        default:
            return "Invalid EarthlyBranches";
    }
}

const char* lunarZodiacToString(enum ChineseZodiac zodiac) {
    switch (zodiac) {
        case CZ_RAT:
            return "鼠";
        case CZ_OX:
            return "牛";
        case CZ_TIGER:
            return "虎";
        case CZ_RABBIT:
            return "兔";
        case CZ_DRAGON:
            return "龙";
        case CZ_SNAKE:
            return "蛇";
        case CZ_HORSE:
            return "马";
        case CZ_GOAT:
            return "羊";
        case CZ_MONKEY:
            return "猴";
        case CZ_ROOSTER:
            return "鸡";
        case CZ_DOG:
            return "狗";
        case CZ_PIG:
            return "猪";
        default:
            return "Invalid ChineseZodiac";
    }
}

const char* lunarMonthToString(int m) {
    static const char* str[12] = {
        "正月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "冬月", "腊月"};
    return str[abs(m - 1) % 12];
}

const char* lunarMDayToString(int mday) {
    static const char* str[30] = {
        "初一",
        "初二",
        "初三",
        "初四",
        "初五",
        "初六",
        "初七",
        "初八",
        "初九",
        "初十",
        "十一",
        "十二",
        "十三",
        "十四",
        "十五",
        "十六",
        "十七",
        "十八",
        "十九",
        "二十",
        "廿一",
        "廿二",
        "廿三",
        "廿四",
        "廿五",
        "廿六",
        "廿七",
        "廿八",
        "廿九",
        "三十",
    };
    return str[abs(mday - 1) % 30];
}

const char* solarTermsToString(enum SolarTerms term) {
    static const char* str[24] = {
        "小寒",
        "大寒",
        "立春",
        "雨水",
        "惊蛰",
        "春分",
        "清明",
        "谷雨",
        "立夏",
        "小满",
        "芒种",
        "夏至",
        "小暑",
        "大暑",
        "立秋",
        "处暑",
        "白露",
        "秋分",
        "寒露",
        "霜降",
        "立冬",
        "小雪",
        "大雪",
        "冬至",
    };
    return str[term];
}

const char* astroToString(enum AstrologicalSign astro) {
    switch (astro) {
        case AS_Capricornus:
            return "摩羯";
        case AS_Auarius:
            return "宝瓶";
        case AS_Pisces:
            return "双鱼";
        case AS_Aries:
            return "白羊";
        case AS_Taurus:
            return "金牛";
        case AS_Gemini:
            return "双子";
        case AS_Cancer:
            return "巨蟹";
        case AS_Leo:
            return "狮子";
        case AS_Virgo:
            return "处女";
        case AS_Libra:
            return "天秤";
        case AS_Scorpio:
            return "天蝎";
        case AS_Sagittarius:
            return "射手";
        default:
            return "Invalid AstrologicalSign";
    }
}

const char* lunarFestivalToString(enum LunarFestival f) {
    const struct FestivalDate* p = lunarFestivals;
    while (p->name) {
        if (p->festival == f) {
            return p->name;
        }
        p++;
    }
    return NULL;
}

const char* festivalToString(enum Festival f) {
    const struct FestivalDate* p = festivals;
    while (p->name) {
        if (p->festival == f) {
            return p->name;
        }
        p++;
    }
    return NULL;
}

int chineseLunarDate(ChineseLunarDate* date) {
    struct tm today = tmtoday();

    // 年月日全为0则取今天
    if (date->date.y == 0 && date->date.m == 0 && date->date.d == 0) {
        date->date.y = today.tm_year + 1900;
        date->date.m = today.tm_mon + 1;
        date->date.d = today.tm_mday;
    } else if (!is_valid_date(date->date.y, date->date.m, date->date.d)) {
        return 0;
    }

    date->istoday = (date->date.y - 1900 == today.tm_year && date->date.m - 1 == today.tm_mon && date->date.d == today.tm_mday);
    date->diff = diffFromToday(date->date.y, date->date.m, date->date.d);
    date->date.isleap = is_leap_year(date->date.y);
    date->date.yday = getYDay(date->date.y, date->date.m, date->date.d);
    date->date.wday = getWDay(date->date.y, date->date.m, date->date.d);
    date->date.ydays = daysInYear(date->date.y);
    date->date.mdays = daysInMonth(date->date.y, date->date.m);
    date->date.astro = getAstro(date->date.m, date->date.d);
    date->date.festival = getFestival(date->date.m, date->date.d);

    lunarDate(date->date.y, date->date.m, date->date.d, &date->lunar.y, &date->lunar.m, &date->lunar.d);
    date->lunar.isleapmonth = lunarLeapMonth(date->lunar.y) == date->lunar.m;
    lunarGanZhiYear(date->lunar.y, &date->lunar.gzy);
    lunarGanZhiMonth(date->date.y, date->date.m, date->date.d, &date->lunar.gzm);
    lunarGanZhiDay(date->date.y, date->date.m, date->date.d, &date->lunar.gzd);
    date->lunar.zodiac = lunarZodiac(date->lunar.y);
    date->lunar.term = lunarTerm(date->date.y, date->date.m, date->date.d);
    date->lunar.festival = getLunarFestival(date->lunar.y, date->lunar.m, date->lunar.d);

    return 1;
}

int chineseLunarDateInDiff(const ChineseLunarDate* origin, ChineseLunarDate* dst, int diff) {
    if (!is_valid_date(origin->date.y, origin->date.m, origin->date.d)) {
        return 0;
    }

    int days = daysSinceEpoch(origin->date.y, origin->date.m, origin->date.d) + diff;
    const int maxDaysSincEpoch = daysSinceEpoch(CALENDAR_YEAR_MAX, 12, 31);
    if (days < 31 || days >= maxDaysSincEpoch) {
        return 0;
    }

    int y, m, d, ydays = 0, mdays = 0;
    for (y = CALENDAR_YEAR_MIN; y < CALENDAR_YEAR_MAX + 1 && days > 0; y++) {
        ydays = daysInYear(y);
        days -= ydays;
    }

    if (days < 0) {
        y--;
        days += ydays;
    }

    for (m = 1; m < 13 && days > 0; m++) {
        mdays = daysInMonth(y, m);
        days -= mdays;
    }

    if (days < 0) {
        m--;
        days += mdays;
    }

    d = days + 1;
    dst->date.y = y;
    dst->date.m = m;
    dst->date.d = d;
    return chineseLunarDate(dst);
}
