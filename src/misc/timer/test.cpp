#include "timer.h"
#include <cstdint>
#include <climits>
#include <thread>
#include <mutex>
#include <atomic>
#include <typeinfo>
#include <vector>

int main()
{
	using namespace jlib;
	using namespace std;

	using data_type = int16_t;
	//constexpr auto max_value = std::numeric_limits<data_type>::max();

	cout << "data type is " << typeid(data_type).name() << endl;
	constexpr auto max_value = std::numeric_limits<data_type>::max() * 1000;
	cout << "max values is:" << max_value << endl;
	cout << "hardware concurrency is:" << std::thread::hardware_concurrency() << endl;
	
	// number version
	//if(0)
	{
		cout << "number:" << endl;
		auto_timer t;
		auto i = max_value;
		while (i--);
	}

	// atomic version with single thread
	{
		cout << "atomic single thread:" << endl;
		auto_timer t;
		atomic<data_type> i(max_value);

		while (i--);


	}

	// atomic version with multi-thread
	//if(0)
	{
		cout << "atomic multi-thread:" << endl;
		
		atomic<data_type> i(max_value);

		std::vector<thread> tv;
		
		{
			auto_timer t;
			for (size_t j = 0; j < std::thread::hardware_concurrency(); j++) {
				tv.push_back(std::thread([&i]() {
					while (i--) {
						//std::this_thread::yield();
						//this_thread::sleep_for(chrono::milliseconds(0));
					}
				}));
			}
			
			for (auto& t : tv) {
				t.join();
			}
		}

		
	}

	system("pause");
}

