// EchoClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#ifdef _DEBUG
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_system-vc140-mt-gd-1_59.lib")
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_date_time-vc140-mt-gd-1_59.lib")
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_regex-vc140-mt-gd-1_59.lib")
#else
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_system-vc140-mt-1_59.lib")
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_date_time-vc140-mt-1_59.lib")
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_regex-vc140-mt-1_59.lib")
#endif // _DEBUG

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <thread>
#include <cstdlib>

int count = 0;

class talk_to_server : public boost::enable_shared_from_this<talk_to_server>, boost::noncopyable
{
public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<talk_to_server> this_ptr;
	typedef boost::system::error_code error_code;

	static this_ptr start(boost::asio::io_service& service, boost::asio::ip::tcp::endpoint ep, const std::string& msg) {
		auto ptr = boost::shared_ptr<talk_to_server>(new talk_to_server(service, msg));
		ptr->start(ep);
		return ptr;
	}

	void stop() {
		if (!started_) return;
		started_ = false;
		socket_.close();
		printf("talk_to_server %d stop\n", id_);
	}

	socket_type & socket() { return socket_; }

	~talk_to_server() {
		printf("talk_to_server %d dtor\n", id_);
	}
private:

	talk_to_server(boost::asio::io_service& service, const std::string& msg) : socket_(service), started_(true), message_(msg) {
		printf("talk_to_server %d ctor\n", id_);
	}

	void start(boost::asio::ip::tcp::endpoint ep) {
		printf("talk_to_server %d start\n", id_);
		socket_.async_connect(ep, boost::bind(&talk_to_server::on_connect, shared_from_this(), _1));
	}

	void on_connect(const error_code& ec) {
		if (ec) stop();
		else do_write(message_ + "\n");
	}

	size_t read_complete(const error_code& ec, size_t bytes) {
		if (ec)return 0;
		bool found = std::find(read_buff_, read_buff_ + bytes, '\n') < read_buff_ + bytes;
		return found ? 0 : 1;
	}

	void do_read() {
		boost::asio::async_read(socket_, boost::asio::buffer(read_buff_),
								boost::bind(&talk_to_server::read_complete,
											shared_from_this(),
											_1, _2),
								boost::bind(&talk_to_server::on_read,
											shared_from_this(),
											_1, _2));
	}

	void on_read(const error_code& ec, size_t bytes) {
		if (!ec) {
			std::string msg(read_buff_, bytes - 1);
			printf("%d: server echoed our %s : %s\n", id_, message_.c_str(), message_ == msg ? "OK" : "FAILED");
		}
		stop();
	}

	void do_write(const std::string& msg) {
		if (!started_)return;
		std::copy(msg.begin(), msg.end(), write_buff_);
		auto handler = boost::bind(&talk_to_server::on_write, shared_from_this(), _1, _2);
		socket_.async_write_some(boost::asio::buffer(write_buff_, msg.size()), handler);
	} 

	void on_write(const error_code& ec, size_t bytes) {
		do_read();
	}

	socket_type socket_;
	bool started_;
	enum { max_msg = 1024, };
	char write_buff_[max_msg] = { 0 };
	char read_buff_[max_msg] = { 0 };
	int id_ = count++;
	std::string message_;
};


int main()
{
	using namespace boost::asio;
	boost::asio::io_service service;
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::from_string("127.0.0.1"), 8888);
	char* messages[] = { "John says hi", "so does James", "Lucy just got home"};
	for (auto msg : messages) {
		talk_to_server::start(service, ep, msg);
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
	service.run();

	std::system("pause");
    return 0;
}

