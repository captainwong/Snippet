#define _WIN32_WINNT 0x0501

#ifdef _DEBUG
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_system-vc140-mt-gd-1_59.lib")
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_date_time-vc140-mt-gd-1_59.lib")
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_regex-vc140-mt-gd-1_59.lib")
#else
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_system-vc140-mt-1_59.lib")
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_date_time-vc140-mt-1_59.lib")
#pragma comment(lib, "D:/dev_libs/boost_1_59_0/stage/lib/libboost_regex-vc140-mt-1_59.lib")
#endif // _DEBUG



//#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <thread>
#include <cstdio>
#include <cstdlib>
#include "../log.h"


using namespace std;
using namespace boost::asio;

io_service service;

class talk_to_svr : public std::enable_shared_from_this<talk_to_svr>, boost::noncopyable
{
	typedef talk_to_svr self_type;
	talk_to_svr(const string& user_name) :sock_(service), started_(true), user_name_(user_name), timer_(service) {
		AUTO_LOG_FUNCTION;
		printf("ctor: %s\n", user_name.c_str());
	}

private:
	ip::tcp::socket sock_;
	enum { max_msg = 1024 };
	char read_buffer_[max_msg] = { 0 };
	char write_buffer_[max_msg] = { 0 };
	bool started_ = false;
	string user_name_ = "";
	deadline_timer timer_;

public:
	typedef boost::system::error_code error_code;
	typedef std::shared_ptr<self_type> self_ptr;

	~talk_to_svr(){ AUTO_LOG_FUNCTION; }

	static self_ptr start(ip::tcp::endpoint ep, const string& user_name) {
		AUTO_LOG_FUNCTION;
		self_ptr client = std::shared_ptr<self_type>(new self_type(user_name));
		client->start(ep);
		return client;
	}

	void stop() {
		AUTO_LOG_FUNCTION;
		if (!started_)return;
		printf("stopping %s\n", user_name_.c_str());
		started_ = false;
		sock_.close();
	}

	bool started() const { AUTO_LOG_FUNCTION;  return started_; }

private:
	// operations

	void start(ip::tcp::endpoint ep) {
		AUTO_LOG_FUNCTION;
		printf("start: %s\n", user_name_.c_str());
		auto self = shared_from_this();
		sock_.async_connect(ep, [self, this](const error_code& ec) {
			if (!ec) {
				do_write("login " + user_name_ + "\n");
			} else {
				printf("async_connect failed: %s\n", user_name_.c_str());
				stop();
			}
		});
	}

	void do_write(const string& msg) {
		AUTO_LOG_FUNCTION;
		printf("do_write: %s\n", user_name_.c_str());
		if (!started())return;
		auto self = shared_from_this();
		std::copy(msg.begin(), msg.end(), write_buffer_);
		sock_.async_write_some(buffer(write_buffer_, msg.size()), [self, this](const error_code& /*ec*/, size_t /*bytes*/) {
			do_read();
		});
	}

	void do_read() {
		AUTO_LOG_FUNCTION;
		printf("do_read: %s\n", user_name_.c_str());
		auto self = shared_from_this();
		async_read(sock_, buffer(read_buffer_), 
				   boost::bind(&self_type::read_complete, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred),
				   boost::bind(&self_type::on_read, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}

	void do_ping() {
		AUTO_LOG_FUNCTION;
		printf("do_ping: %s\n", user_name_.c_str());
		do_write("ping\n");
	}

	void post_one_ping() {
		AUTO_LOG_FUNCTION;
		printf("post_one_ping: %s\n", user_name_.c_str());
		auto self = shared_from_this();
		int ms = rand() % 4000;
		printf("%s post one ping %d ms\n", user_name_.c_str(), ms);
		timer_.expires_from_now(boost::posix_time::millisec(ms));
		timer_.async_wait(std::bind(&self_type::do_ping, self));
	}

	void do_ask_clients() {
		AUTO_LOG_FUNCTION;
		printf("do_ask_clients: %s\n", user_name_.c_str());
		do_write("ask_clients\n");
	}

private:
	// handlers

	void on_read(const error_code& ec, size_t bytes) {
		AUTO_LOG_FUNCTION;
		printf("on_read: %s\n", user_name_.c_str());
		if (ec) stop();
		if (!started()) return;

		string msg(read_buffer_, bytes);
		if (msg.find("login ") == 0) on_login();
		else if (msg.find("ping") == 0) on_ping(msg);
		else if (msg.find("clients ") == 0) on_clients(msg);
		else printf("invalid msg %s\n", msg.c_str());
	}

	void on_login() {
		AUTO_LOG_FUNCTION;
		printf("%s logged in\n", user_name_.c_str());
		do_ask_clients();
	}

	void on_ping(const string& msg) {
		AUTO_LOG_FUNCTION;
		printf("on_ping: %s\n", user_name_.c_str());
		istringstream in(msg);
		string answer;
		in >> answer >> answer;
		if (answer == "client_list_changed") do_ask_clients();
		else post_one_ping();
	}

	void on_clients(const string& msg) {
		AUTO_LOG_FUNCTION;
		auto clients = msg.substr(8);
		printf("%s, new client list:%s", user_name_.c_str(), clients.c_str());
		post_one_ping();
	}

	size_t read_complete(const error_code& ec, size_t bytes) {
		AUTO_LOG_FUNCTION;
		if (ec) return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		return found ? 0 : 1;
	}
	
};


int main()
{
	ip::tcp::endpoint ep(ip::address::from_string("192.168.168.222"), 8888);
	const char* names[] = { "John", "James", "Lucy", "Tracy", "Frank", "Abby" };
	for (auto name : names) {
		talk_to_svr::start(ep, name);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		break;
	}
	service.run();
	std::system("pause");
	return 0;
}
