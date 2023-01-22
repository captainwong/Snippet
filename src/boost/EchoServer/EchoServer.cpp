// EchoServer.cpp : Defines the entry point for the console application.
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

#include <cstdlib>

int count = 0;

class talk_to_client : public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable
{
public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<talk_to_client> this_ptr;
	typedef boost::system::error_code error_code;

	static this_ptr new_(boost::asio::io_service& service) {
		auto ptr = boost::shared_ptr<talk_to_client>(new talk_to_client(service));
		return ptr;
	}

	void start() {
		printf("talk_to_client %d start\n", id);
		started_ = true;
		do_read();
	}

	void stop() {
		if (!started_) return;
		started_ = false;
		socket_.close();
		printf("talk_to_client %d stop\n", id);
	}

	socket_type & socket() { return socket_; }

	~talk_to_client() {
		printf("talk_to_client %d dtor\n", id);
	}
private:

	talk_to_client(boost::asio::io_service& service) : socket_(service), started_(false) {
		printf("talk_to_client %d ctor\n", id);
	}

	size_t read_complete(const error_code& ec, size_t bytes) {
		if (ec)return 0;
		bool found = std::find(read_buff_, read_buff_ + bytes, '\n') < read_buff_ + bytes;
		return found ? 0 : 1;
	}

	void do_read() {
		//using namespace std::placeholders;
		/*auto handler1 = boost::bind(&talk_to_client::read_complete,
									shared_from_this(), read_buff_,
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred);
		auto handler2 = boost::bind(&talk_to_client::on_read, shared_from_this(), _1, _2);
		*/
		//auto handler1 = boost::bind(read_complete, boost::)


		/*boost::asio::async_read(socket_, boost::asio::buffer(read_buff_), 
								boost::bind(&talk_to_client::read_complete, 
											shared_from_this(),
											_1, _2), 
								boost::bind(&talk_to_client::on_read,
											shared_from_this(), 
											_1, _2));*/
		
		boost::asio::async_read(socket_, boost::asio::buffer(read_buff_), 
								boost::bind(&talk_to_client::read_complete, 
											shared_from_this(),
											_1, _2), 
								boost::bind(&talk_to_client::on_read,
											shared_from_this(), 
											_1, _2));
	}

	void on_read(const error_code& ec, size_t bytes) {
		if (!ec) {
			std::string msg(read_buff_, bytes);
			printf("%d:%s\n", id, msg.c_str());
			do_write(msg + '\n');
		}
		stop();
	}

	void do_write(const std::string& msg) {
		if (!started_)return;
		std::copy(msg.begin(), msg.end(), write_buff_);
		//using namespace std::placeholders;
		auto handler = boost::bind(&talk_to_client::on_write, shared_from_this(), _1, _2);
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
	int id = count++;
};

using namespace boost::asio;
boost::asio::io_service service;
boost::asio::ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::address_v4(), 8888));

void accept_handler(talk_to_client::this_ptr client, 
					const boost::system::error_code& ec) {
	client->start();
	auto new_client = talk_to_client::new_(service);
	acceptor.async_accept(new_client->socket(), boost::bind(accept_handler, new_client, _1));
}

int main()
{
	{
		auto new_client = talk_to_client::new_(service);
		acceptor.async_accept(new_client->socket(), boost::bind(accept_handler, new_client, _1));
	}
	
	service.run();

	
	
	std::system("pause");
    return 0;
}

