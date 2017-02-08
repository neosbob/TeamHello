#include <fstream>
#include <cstdio>
#include "gtest/gtest.h"
#include "config_parser.h"
#include "webserver.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;


TEST(configParseTest, correctConfig)
{
	configArguments configArgs;
	const char* argv[2] = {"./webserver", "SimpleConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	EXPECT_EQ(errorCode, 0);
	EXPECT_EQ(configArgs.port, 8080);
}

TEST(configParseTest, formatWrongConfig)
{
	configArguments configArgs;
	std::ofstream configFile;
	configFile.open("formatWrongConfig");
	configFile << "server {;";
	configFile.close();
	const char* argv[2] = {"./webserver", "formatWrongConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("formatWrongConfig");
	EXPECT_EQ(errorCode, 3);
}

TEST(configParseTest, portNumberWrongConfig)
{
	configArguments configArgs;
	std::ofstream configFile;
	configFile.open("portNumberWrongConfig");
	configFile << "port 999999;";
	configFile.close();
	const char* argv[2] = {"./webserver", "portNumberWrongConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("portNumberWrongConfig");
	EXPECT_EQ(errorCode, 2);
}

TEST(ServerTest, doAcceptTest)
{
    configArguments configArgs;
    configArgs.port = 8080;
    configArgs.baseDirectory = "/home/ubuntu";
    configArgs.echo_path = "reply_echo";
    configArgs.static_path = "reply_static";
    io_service io_service();
    std::shared_ptr<session> sesh = std::make_shared<session>(io_service, configContent.baseDirectory, configContent.echo_path, configContent.static_path);
    
    Server server(configArgs);
    server.run();
    
    std::string str = "http /echo/a.txt 1.0\r\n";
    asio::async_write(sesh->socket, boost::asio::buffer(str.c_str(), str.length()), [sesh, str](const error_code& e, std::size_t s)
    {
      //return str->c_str();
    });
    
    EXPECT_EQ(server.getTotalRequestCount, 1);
}
