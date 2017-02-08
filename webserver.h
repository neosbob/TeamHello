#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <map>
#include <boost/asio.hpp>
#include "request_handle.h"

struct configArguments
{
    short unsigned int port;
    std::string static_path;
    std::string echo_path;
    std::map<std::string,std::string> map_path_rootdir;
};

class Server
{
public:
	Server(const configArguments& configArgs);
	static int parseConfig(int argc, const char * argv[], configArguments& configArgs);
	void run();
	int getTotalRequestCount(){return totalRequestCount;}
	
private:
	void doAccept();

	boost::asio::io_service io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	configArguments configContent;
	int totalRequestCount;
};

#endif //WEBSERVER_H
