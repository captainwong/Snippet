#pragma once

#include <sstream>
#include <string>
#include <chrono>
#include <stdio.h>

class LogFunction {
private:
	const char* _func_name;
	std::chrono::steady_clock::time_point _begin;
	std::thread::id id_;
	std::string thread_id_;
public:
	LogFunction(const char* func_name) : _func_name(func_name) {
		id_ = std::this_thread::get_id();
		std::ostringstream os;
		os << id_;
		thread_id_ = os.str();
		printf("thread id:%s ---- %s in\n", thread_id_.c_str(), _func_name); _begin = std::chrono::steady_clock::now();
	}

	~LogFunction() { 
		auto diff = std::chrono::steady_clock::now() - _begin;
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
		printf("thread id:%s ---- %s out, duration: %lld (ms)\n", thread_id_.c_str(), _func_name, msec.count());
	}
};

#define LOG_FUNCTION(func_name) LogFunction _log_function_object(func_name);
#define AUTO_LOG_FUNCTION LOG_FUNCTION(__FUNCTION__)
