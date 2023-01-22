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
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "../log.h"


using namespace std;
using namespace boost::asio;

io_service service;

class talk_to_client;
typedef std::shared_ptr<talk_to_client> client_ptr;
typedef std::vector<client_ptr> client_array;

client_array clients;

void update_clients_changed();

class talk_to_client : public std::enable_shared_from_this<talk_to_client>, boost::noncopyable
{
	typedef talk_to_client self_type;
	talk_to_client():sock_(service), timer_(service) { AUTO_LOG_FUNCTION; }
	
public:
	typedef boost::system::error_code bec;
	typedef std::shared_ptr<self_type> self_ptr;

	~talk_to_client() { AUTO_LOG_FUNCTION; }

private:
	ip::tcp::socket sock_;
	enum { max_msg = 1024 };
	char read_buffer_[max_msg] = { 0 };
	char write_buffer_[max_msg] = { 0 };
	bool started_ = false;
	string user_name_ = "";
	deadline_timer timer_;
	std::chrono::steady_clock::time_point last_ping_;
	bool clients_changed_ = false;

public:

	void start() {
		AUTO_LOG_FUNCTION;
		printf("start: %s\n", user_name_.c_str());
		started_ = true;
		clients.push_back(shared_from_this());
		last_ping_ = std::chrono::steady_clock::now();
		do_read();
	}

	static self_ptr new_() {
		AUTO_LOG_FUNCTION;
		return self_ptr(new talk_to_client());
	}

	void stop() {
		AUTO_LOG_FUNCTION;
		printf("stop: %s\n", user_name_.c_str());
		if (!started_)return;
		started_ = false;
		sock_.close();

		clients.erase(std::find(clients.begin(), clients.end(), shared_from_this()));
		update_clients_changed();
	}

	bool started() const { AUTO_LOG_FUNCTION; return started_; }

	ip::tcp::socket& sock() { AUTO_LOG_FUNCTION; return sock_; }

	string user_name() const { AUTO_LOG_FUNCTION; return user_name_; }

	void set_clients_changed() { AUTO_LOG_FUNCTION; clients_changed_ = true; }

private:
	// operations

	void do_read() {
		AUTO_LOG_FUNCTION;
		printf("do_read: %s\n", user_name_.c_str());
		async_read(sock_, buffer(read_buffer_),
				   boost::bind(&self_type::read_complete, shared_from_this(), 
							   boost::asio::placeholders::error, 
							   boost::asio::placeholders::bytes_transferred),
				   boost::bind(&self_type::on_read, shared_from_this(), 
							   boost::asio::placeholders::error, 
							   boost::asio::placeholders::bytes_transferred));
		
		post_check_ping();
	}

	void do_write(const string& msg) {
		AUTO_LOG_FUNCTION;
		printf("do_write: %s\n", user_name_.c_str());
		if (!started())return;
		std::copy(msg.begin(), msg.end(), write_buffer_);
		sock_.async_write_some(buffer(write_buffer_, msg.size()),
							   boost::bind(&self_type::on_write, shared_from_this(), 
										   boost::asio::placeholders::error, 
										   boost::asio::placeholders::bytes_transferred));
	}

	void do_ping() {
		AUTO_LOG_FUNCTION;
		printf("do_ping: %s\n", user_name_.c_str());
		do_write("ping\n");
	}

	void do_ask_clients() {
		AUTO_LOG_FUNCTION;
		printf("do_ask_clients: %s\n", user_name_.c_str());
		do_write("ask_clients\n");
	}


private:
	// handlers

	void on_read(const bec& ec, size_t bytes) {
		AUTO_LOG_FUNCTION;
		printf("on_read: %s\n", user_name_.c_str());
		if (ec)stop();
		if (!started())return;
		string msg(read_buffer_, bytes);
		if (msg.find("login ") == 0) on_login(msg);
		else if (msg.find("ping") == 0) on_ping();
		else if (msg.find("ask_clients") == 0) on_clients();
		else printf("invalid msg: %s\n", msg.c_str());
	}

	void on_write(const bec& ec, size_t bytes) {
		AUTO_LOG_FUNCTION;
		printf("on_write: %s\n", user_name_.c_str());
		do_read();
	}

	void on_login(const string& msg) {
		AUTO_LOG_FUNCTION;
		istringstream in(msg);
		in >> user_name_ >> user_name_;
		printf("%s logged in\n", user_name_.c_str());
		do_write("login ok\n");
		update_clients_changed();
	}

	void on_ping() {
		AUTO_LOG_FUNCTION;
		printf("on_ping: %s\n", user_name_.c_str());
		do_write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
		clients_changed_ = false;
	}

	void on_clients() {
		AUTO_LOG_FUNCTION;
		printf("on_clients: %s\n", user_name_.c_str());
		string msg;
		for (auto client : clients) {
			msg += client->user_name() + " ";
		}
		do_write("clients " + msg + "\n");
	}

	void on_check_ping() {
		AUTO_LOG_FUNCTION;
		printf("on_check_ping: %s\n", user_name_.c_str());
		auto now = std::chrono::steady_clock::now();
		auto diff = now - last_ping_;
		auto s = std::chrono::duration_cast<std::chrono::seconds>(diff);
		if (s.count() > 5) {
			printf("stopping %s - no ping in time, %lld s\n", user_name_.c_str(), s.count());
			stop();
		}
		last_ping_ = now;
	}

	void post_check_ping() {
		AUTO_LOG_FUNCTION;
		printf("post_check_ping: %s\n", user_name_.c_str());
		timer_.expires_from_now(boost::posix_time::milliseconds(5000));
		timer_.async_wait(boost::bind(&self_type::on_check_ping, shared_from_this()));
	}

	size_t read_complete(const bec& ec, size_t bytes) {
		if (ec)return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		return found ? 0 : 1;
	}
};


void update_clients_changed() {
	AUTO_LOG_FUNCTION;
	for (auto client : clients) {
		client->set_clients_changed();
	}
}

ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8888));


void handle_accept(const talk_to_client::self_ptr& client, const talk_to_client::bec& ec) {
	AUTO_LOG_FUNCTION;
	client->start();
	auto new_client = talk_to_client::new_();
	acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, boost::asio::placeholders::error));
}

int main()
{
	auto new_client = talk_to_client::new_();
	acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, boost::asio::placeholders::error));

	const auto cores = std::thread::hardware_concurrency();
	std::vector<std::shared_ptr<std::thread>> threads;

	for (unsigned int i = 0; i < cores; i++) {
		threads.push_back(std::shared_ptr<std::thread>(new std::thread([]() { service.run(); })));
	}

	for (unsigned int i = 0; i < cores; i++) {
		threads[i]->join();
	}

	//std::integral_constant
}

