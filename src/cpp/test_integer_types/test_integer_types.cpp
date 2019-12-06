#include <cstdint>
#include <stdio.h>
#include <typeinfo>

#define printType(type) printf("%s\t%s\t%zu\n", #type, typeid(type).name(), sizeof(type));
#define printValue(value) printf("%s\t%s\t%d\n", #value, typeid(decltype(value)).name(), value);
#define printLongValue(value) printf("%s\t%s\t%lld\n", #value, typeid(decltype(value)).name(), value);

int main()
{
	printType(std::int8_t);
	printType(std::int16_t);
	printType(std::int32_t);
	printType(std::int64_t);

	printType(std::uint8_t);
	printType(std::uint16_t);
	printType(std::uint32_t);
	printType(std::uint64_t);

	//printType(std::int_least8_t);
	//printType(std::int_least16_t);
	//printType(std::int_least32_t);
	//printType(std::int_least64_t);

	//printType(std::uint_least8_t);
	//printType(std::uint_least16_t);
	//printType(std::uint_least32_t);
	//printType(std::uint_least64_t);

	//printType(std::int_fast8_t);
	//printType(std::int_fast16_t);
	//printType(std::int_fast32_t);
	//printType(std::int_fast64_t);

	//printType(std::uint_fast8_t);
	//printType(std::uint_fast16_t);
	//printType(std::uint_fast32_t);
	//printType(std::uint_fast64_t);

	printType(std::intmax_t);
	printType(std::intptr_t);
	printType(std::uintmax_t);
	printType(std::uintptr_t);


	printValue(INT8_MIN);
	printValue(INT16_MIN);
	printValue(INT32_MIN);
	printLongValue(INT64_MIN);
	printValue(INT8_MAX);
	printValue(INT16_MAX);
	printValue(INT32_MAX);
	printLongValue(INT64_MAX);
	printValue(UINT8_MAX);
	printValue(UINT16_MAX);
	printValue(UINT32_MAX);
	printLongValue(UINT64_MAX);

	printLongValue(INTMAX_MIN);
	printLongValue(INTMAX_MAX);
	printLongValue(UINTMAX_MAX);

}
