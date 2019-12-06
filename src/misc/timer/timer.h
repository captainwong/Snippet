#pragma once

#include <chrono>
#include <iostream>

namespace jlib {
class timer {
	std::chrono::steady_clock::time_point begin_ = {};
	std::chrono::steady_clock::time_point end_ = {};
public:

	timer() {
		start();
	}

	void start() {
		begin_ = std::chrono::steady_clock::now();
	}

	void stop() {
		end_ = std::chrono::steady_clock::now();
		auto diff = end_ - begin_;
		auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
		auto micro = std::chrono::duration_cast<std::chrono::microseconds>(diff).count() % 10000000;
		std::cout << "Time: " << milli << "." << micro << "ms" << std::endl;
	}

	void average(size_t count) {
		auto diff = end_ - begin_;
		auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
		auto micro = std::chrono::duration_cast<std::chrono::microseconds>(diff).count() % 10000000;
		std::cout << "Average Time: " << milli / count << "." << micro / count << "ms" << std::endl;
	}

	//void output(const std::chro)

};

class auto_timer {
	timer t_ = {};

public:

	auto_timer() {}

	~auto_timer() { t_.stop(); }
};

}
