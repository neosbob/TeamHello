#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <map>
#include <vector>
#include <boost/asio.hpp>
#include "config_parser.h"
#include "request_handler.h"
#include <boost/thread/mutex.hpp>
#include <string>

#define IPADDR "127.0.0.1"

struct configArguments
{
    short unsigned int port;
    unsigned int numOfThreads;
    std::string logFileName;
    // Key is uri prefix. Each prefix is mapped to a handler
    std::map<std::string, RequestHandler*> handlerMapping;
    RequestHandler* defaultHandler;
};

class Server
{
public:
    static Server *serverBuilder(const NginxConfig& config_out);
    void run();
    void stop();
    void getStats(std::map<std::string, int>& urlRequestedCount, std::map<std::string, int>& ResponseCodeCount);
    std::map<std::string, std::vector<std::string> > getUriPrefixRequestHandlerMap(){return this->uri_prefix2request_handler_name;}
    static Server *getServerInstance(){return serverInstance;}
    static int parseConfig(const NginxConfig& config_out, configArguments& configArgs, std::map<std::string, std::vector<std::string> >& uri_prefix2request_handler_name);
    static std::string constructLogMsg(std::string url, int response_code);
    static boost::mutex mtx;
	
private:
    Server(configArguments configArgs, std::map<std::string, std::vector<std::string> > uri_prefix2request_handler_name);
    void doAccept();

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::signal_set signals;
    configArguments configContent;
    std::map<std::string, std::vector<std::string> > uri_prefix2request_handler_name;
    
    static Server *serverInstance;
};

#endif //WEBSERVER_H
