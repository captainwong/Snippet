/*
* 1900年1月31日 - 2100年12月31日 区间内的公历转农历
* 
* 参考资料：
* https://www.youtube.com/watch?v=AmE3UNt7-ik
* https://www.youtube.com/watch?v=h7_bzyu7R-A
* https://www.youtube.com/watch?v=RuNOoglFIKk
* https://github.com/jjonline/calendar.js
* https://wannianrili.bmcx.com/
*/

#ifndef __CALENDAR_H__
#define __CALENDAR_H__

#pragma once

#define CALENDAR_YEAR_MIN 1900
#define CALENDAR_YEAR_MAX 2100

// 天干
enum HeavenlyStem {
	HS_JIA,		// 甲
	HS_YI,		// 乙
	HS_BING,	// 丙
	HS_DING,	// 丁
	HS_WU,		// 戊
	HS_JI,		// 己
	HS_GENG,	// 庚
	HS_XIN,		// 辛
	HS_REN,		// 壬
	HS_GUI,		// 癸
	HS_COUNT,	// 天干数量
};

// 地支
enum EarthlyBranch {
	EB_ZI,		// 子
	EB_CHOU,	// 丑
	EB_YIN,		// 寅
	EB_MAO,		// 卯
	EB_CHEN,	// 辰
	EB_SI,		// 巳
	EB_WU,		// 午
	EB_WEI,		// 未
	EB_SHEN,	// 申
	EB_YOU,		// 酉
	EB_XU,		// 戌
	EB_HAI,		// 亥
	EB_COUNT,	// 地支数量
};

// 干支
struct GanZhi {
	enum HeavenlyStem gan;
	enum EarthlyBranch zhi;
};

// 十二生肖
enum ChineseZodiac {
	CZ_RAT,		// 鼠
	CZ_OX,		// 牛
	CZ_TIGER,	// 虎 
	CZ_RABBIT,	// 兔
	CZ_DRAGON,	// 龙
	CZ_SNAKE,	// 蛇
	CZ_HORSE,	// 马
	CZ_GOAT,	// 羊
	CZ_MONKEY,	// 猴
	CZ_ROOSTER, // 鸡
	CZ_DOG,		// 狗
	CZ_PIG,		// 猪
	CZ_COUNT,	// 生肖数量
};

// 公历节假日
enum Festival {
	F11_NewYear,		// 新年
	F214_ValentinesDay, // 情人节
	F38_WomensDay,		// 妇女节
	F312_ArborDay,		// 植树节
	F41_AprilFoolsDay,	// 愚人节
	F51_LabourDay,		// 劳动节
	F54_YouthDay,		// 青年节
	F512_NurseDay,		// 护士节
	F61_ChildrensDay,	// 儿童节
	F71_PartysDay,		// 建党节
	F81_ArmyDay,		// 建军节
	F910_TeachersDay,	// 教师节
	F918_NationalShameDay, // 国耻日
	F101_NationalDay,	// 国庆节
	F1224_ChristmasEve, // 平安夜
	F1225_Christmas,	// 圣诞节
};

// 农历节假日
enum LunarFestival {
	LF11_SprintFestival,			// 春节
	LF115_ShangYuanFestival,		// 上元/元宵
#define LF115_LanternFestival LF115_ShangYuanFestival
	LF22_DargonRaisesHeadDay,		// 龙抬头
	LF55_DragonBoatFestival,		// 端午
	LF77_QiXiFestival,				// 七夕
	LF715_ZhongYuanFestival,		// 中元
#define LF715_HungryGhostFestival LF715_ZhongYuanFestival
	LF815_MoonFestival,				// 中秋
	LF99_ChongYangFestival,			// 重阳
#define LF99_DoubleNinthFestival LF99_ChongYangFestival
	LF101_WinterClothingFestival,	// 寒衣
#define LF101_AncestorWorshipFestival LF101_WinterClothingFestival
	LF1015_XiaYuanFestival,			// 下元
	LF128_LaBaFestival,				// 腊八
	LF1223_XiaoNian,				// 北方小年
	LF1224_XiaoNian,				// 南方小年
	LF1229_NewYearsEve,				// 除夕（小年）
	LF1230_NewYearsEve,				// 除夕（大年）
};

// 二十四节气
enum SolarTerms {
	ST_LesserCold,					// 小寒
	ST_GreaterCold,					// 大寒
	ST_BeginningOfSpring,			// 立春
	ST_RainWater,					// 雨水
	ST_AwaikeningFromHibernation,	// 惊蛰 
	ST_VernalEquinox,				// 春分
	ST_FreshGreen,					// 清明
	ST_GrainRain,					// 谷雨
	ST_BeginningOfSummer,			// 立夏
	ST_LesserFullness,				// 小满
	ST_GrainInEar,					// 芒种
	ST_SummerSolstice,				// 夏至
	ST_LesserHeat,					// 小暑
	ST_GreaterHeat,					// 大暑
	ST_BeginningOfAutumn,			// 立秋
	ST_EndOfHeat,					// 处暑
	ST_WhiteDew,					// 白露
	ST_AutumalEquinox,				// 秋分
	ST_ColdDew,						// 寒露
	ST_FirstFrost,					// 霜降
	ST_BeginningOfWinter,			// 立冬
	ST_LightSnow,					// 小雪
	ST_HeaveySnow,					// 大雪
	ST_WinterSolstice,				// 冬至
};

// 十二星座
enum AstrologicalSign {
	AS_Capricornus,	// 摩羯
	AS_Auarius,		// 宝瓶
	AS_Pisces,		// 双鱼
	AS_Aries,		// 白羊
	AS_Taurus,		// 金牛
	AS_Gemini,		// 双子
	AS_Cancer,		// 巨蟹
	AS_Leo,			// 狮子
	AS_Virgo,		// 处女
	AS_Libra,		// 天秤
	AS_Scorpio,		// 天蝎
	AS_Sagittarius,	// 射手
};

typedef struct ChineseLunarDate {
	struct Date {
		int y, m, d;					// 年月日
		int yday;						// 一年内的第几天
		int wday;						// 周几，范围 [1, 7] 表示 周一到周日
		int ydays;						// 当年有多少天
		int mdays;						// 当月有多少天
		int isleap;						// 是否闰年
		enum Festival festival;			// 节假日
		enum AstrologicalSign astro;	// 星座
	} date;			// 公历日期

	struct LunarDate {
		int y, m, d;					// 年月日
		int isleapmonth;				// 当月是否是闰月
		struct GanZhi gzy, gzm, gzd;	// 干支年月日
		enum ChineseZodiac zodiac;		// 生肖
		enum LunarFestival festival;	// 节假日
		enum SolarTerms term;			// 节气
	} lunar;		// 农历日期

	int istoday;	// 是否今天
	int diff;		// 距离今天的天数

} ChineseLunarDate;


/* 公历日期转为农历日期
 * 公历日期范围：1900年1月31日~2100年12月31日
 * 年月日全0则取今天
 * 年月日非法或超过范围则返回0
 * 成功返回1
 */
int chineseLunarDate(ChineseLunarDate* date);
// 根据origin日期，获取距离它diff天的日期dst
int chineseLunarDateInDiff(const ChineseLunarDate* origin, ChineseLunarDate* dst, int diff);

/* 字符串获取函数 */
// 天干字符串
const char* heavenlyStemToString(enum HeavenlyStem hs);
// 地支字符串
const char* earthlyBranchesToString(enum EarthlyBranch eb);
// 生肖字符串
const char* lunarZodiacToString(enum ChineseZodiac zodiac);
// 农历月份，正月、一月、二月、。。。、冬月、腊月
const char* lunarMonthToString(int m);
// 农历日子，初一、。。。初十、十一、。。。二十、廿一、。。。、三十
const char* lunarMDayToString(int mday);
// 节气字符串
const char* solarTermsToString(enum SolarTerms term);
// 星座字符串
const char* astroToString(enum AstrologicalSign astro);
// 公历节日字符串
const char* festivalToString(enum Festival f);
// 农历节日字符串
const char* lunarFestivalToString(enum LunarFestival f);


#endif // __CALENDAR_H__
