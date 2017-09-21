// inspiration https://en.wikipedia.org/wiki/Perfect_number
#include "C:/dev/Global/log2.h"

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <set>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <algorithm>
#include <numeric>

using namespace jlib;

using namespace std;

using data_type = uint32_t;


void test(data_type val) {
	std::set<data_type> factors;
	factors.insert(1);
	
	data_type sqrt_2 = static_cast<data_type>(sqrt(val));
	for (data_type i = 2; i <= sqrt_2; i++) {
		if (val % i == 0) {
			factors.insert(i);
			factors.insert(val / i);
		}
	}

	auto sum = std::accumulate(factors.begin(), factors.end(), (data_type)0);

	if (sum == val) {
		JLOG_INFO("New perfect number found!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! {}", val);
	}
}


int main()
{
	{
		jlib::init_logger();
	}

	mutex mtx;
	condition_variable cv;
	atomic_bool running(true);

	using data_range = std::pair<data_type, data_type>;
	std::queue<data_range> dq;

	//std::thread t([&]() {

		
	//});

	auto begin = chrono::steady_clock::now();
	data_type last_val = 0;

	std::thread provider([&]() {
		for (data_type i = 5; i < static_cast<uint64_t>(numeric_limits<data_type>::max()); ) {
			/*auto now = chrono::steady_clock::now();
			if (chrono::duration_cast<chrono::seconds>(now - begin).count() >= 10) {
				begin = now;
				cout <<"calculating " << i << " speed " << (i - last_val) / 10 << "/s......" << endl;
				last_val = i;
			}*/

			static constexpr data_type RANGE = 10000;

			bool has_new_task = false;

			//test(i);
			{
				lock_guard<mutex> lg(mtx);
				//if (dq.size() < 10) 
				{
					data_range dr;
					dr.first = i;

					uint64_t second = i + RANGE;
					if (second > numeric_limits<data_type>::max()) {
						JLOG_WARN("second {} is bigger than {}, break", second, numeric_limits<data_type>::max());
						break;
					}

					dr.second = i + RANGE;
					dq.push(dr);

					has_new_task = true;
					i += RANGE;
				}
			}

			if (has_new_task) {
				cv.notify_one();
			}

			this_thread::sleep_for(chrono::milliseconds(1));
		}
	});

	vector<thread> consumers;
	for (size_t i = 0; i < std::thread::hardware_concurrency(); i++) {
		consumers.emplace_back(std::thread([&]() {
			while (true) {
				data_range dr(0, 0);

				{
					unique_lock<mutex> ul(mtx);
					cv.wait(ul, [&dq]() {
						return !dq.empty();
					});

					dr = dq.front();
					dq.pop();

					JLOG_INFO("got a task, range is [{}, {})", dr.first, dr.second);
				}

				for (auto val = dr.first; val != dr.second; val++) {
					test(val);
				}

				if (!running.load()) {
					break;
				}
			}

		}));
	}


	provider.join();

	running.store(false);

	for (auto& consumer : consumers) {
		consumer.join();
	}



	std::system("pause");
}

