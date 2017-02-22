#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <map>
#include <boost/asio.hpp>
#include "session.h"

struct configArguments
{
    short unsigned int port;
    // Key is uri prefix. Each prefix is mapped to a handler
    std::map<std::string, RequestHandler*> handlerMapping;
    RequestHandler* defaultHandler;
};

class Server
{
public:
    static Server* serverBuilder(const NginxConfig& config_out);
	void run();
	int getTotalRequestCount(){return totalRequestCount;}
	
private:
    Server(const configArguments& configArgs);
	void doAccept();
	int parseConfig(const NginxConfig& config_out, configArguments& configArgs);

	boost::asio::io_service io_service;
	boost::asio::ip::tcp::acceptor acceptor;
	configArguments configContent;
	int totalRequestCount;
};

#endif //WEBSERVER_H