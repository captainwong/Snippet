#include <type_traits>
#include <iostream>
#include <stdint.h>
#include <assert.h>

typedef uint8_t Char;


inline Char sum(Char* arr, Char len) {
	auto& sum_ = arr[len - 1]; sum_ = 0;
	for (Char i = 0; i < len - 1; i++) { sum_ = (sum_ + arr[i]) & 0xFF; }
	return sum_;
}

template <typename T, typename = int>
struct T_has_data_and_len : std::false_type {};

template <typename T>
struct T_has_data_and_len<T, decltype((void)T::data, (void)T::len, 0)> : std::true_type {};

template <class T>
typename std::enable_if<T_has_data_and_len<T>::value, Char>::type 
sum(T& t)
{
	return sum(t.data, t.len);
}

template <class T>
typename std::enable_if<T_has_data_and_len<T>::value, Char>::type
sum(T* t)
{
	return sum<T>(*t);
}


struct Response
{
	static constexpr Char len = 9;
	//                  0     1     2     3     4     5    status type  sum
	Char data[len] = { 0xEB, 0xBA, 0x3F, 0x07, 0xCC, 0xA0, 0x00, 0x00, 0x00 };
	
	void make() {
		data[6] = 1;
		data[7] = 2;
		sum(this);
	}
};

struct A {
	char* data;
};

struct B {
	int len;
};

struct C {

};

static_assert(T_has_data_and_len<Response>::value, "Response must have data and len ");
static_assert(!T_has_data_and_len<A>::value, "A");
static_assert(!T_has_data_and_len<B>::value, "B");
static_assert(!T_has_data_and_len<C>::value, "C");

struct Request
{
	static constexpr Char len = 5;
	Char data[len] = { 0xEB, 0xAB, 0x3F, 0xA0, 0x75 };


};

struct MachineTimerRequest {
	static constexpr Char len = 5;
	Char data[len] = { 0xEB, 0xAB, 0x3F, 0xA5, 0x00 };
};

//! 设置主机定时器回应
struct SetMachineTimerResponse {
	static constexpr Char len = 7;
	Char data[len] = { 0xEB, 0xBA, 0x3F, 0x07, 0xCC, 0xA7, 0x7A };
};

int main()
{
	Request req = {};

	auto c = sum(req);
	assert(c == 0x75);

	Char data[] = { 0xEB, 0xBA, 0x3F, 0x07, 0xCC, 0xA3, 0x00 };
	sum(data, sizeof(data));

	MachineTimerRequest mtr;
	sum(mtr);

	SetMachineTimerResponse smtr;
	sum(smtr);
}