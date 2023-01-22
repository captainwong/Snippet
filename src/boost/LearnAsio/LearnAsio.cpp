// LearnAsio.cpp : Defines the entry point for the console application.
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


#include <boost/asio.hpp>
#include <cstdlib>

int main()
{
	using namespace boost;
	asio::io_service io_service;
	asio::ip::tcp::socket socket(io_service);
	asio::ip::tcp::endpoint endpoint(asio::ip::address_v4::from_string("112.16.180.60"), 7892);
	socket.connect(endpoint);



	std::system("pause");

	socket.close();
    return 0;
}

