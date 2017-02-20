#include <fstream>
#include <cstdio>
#include "gtest/gtest.h"
#include "config_parser.h"
#include "webserver.h"
#include <stdio.h>
#include <unistd.h>

using namespace boost;
using namespace boost::system;
using namespace boost::asio;

class ConfigTest : public testing::Test {
protected:
    configArguments configArgs;
	std::ofstream configFile;
};

TEST(configParseTest, noConfig)
{
	configArguments configArgs;
	const char* argv[1] = {"./webserver"};
	int errorCode = Server::parseConfig(1, argv, configArgs);
	EXPECT_EQ(errorCode, 1);
}

TEST(configParseTest, correctConfig)
{
	configArguments configArgs;
	const char* argv[2] = {"./webserver", "SimpleConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	EXPECT_EQ(errorCode, 0);
	EXPECT_EQ(configArgs.port, 8080);
}

TEST_F(ConfigTest, formatWrongConfig)
{
	configFile.open("formatWrongConfig");
	configFile << "server {;";
	configFile.close();
	const char* argv[2] = {"./webserver", "formatWrongConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("formatWrongConfig");
	EXPECT_EQ(errorCode, 3);
}

TEST_F(ConfigTest, portNumberWrongConfig)
{
	configFile.open("portNumberWrongConfig");
	configFile << "port 999999;";
	configFile.close();
	const char* argv[2] = {"./webserver", "portNumberWrongConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("portNumberWrongConfig");
	EXPECT_EQ(errorCode, 2);
}

TEST_F(ConfigTest, noPortNumberConfig)
{
	configFile.open("noPortNumberConfig");
	configFile << "port;";
	configFile.close();
	const char* argv[2] = {"./webserver", "noPortNumberConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("noPortNumberConfig");
	EXPECT_EQ(errorCode, 3);
}


TEST_F(ConfigTest, pathWrongConfig)
{
	configFile.open("pathWrongConfig");
	configFile << "port 999999;\npath /echo";
	configFile.close();
	const char* argv[2] = {"./webserver", "pathWrongConfig"};
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("pathWrongConfig");
	EXPECT_EQ(errorCode, 3);
}

TEST_F(ConfigTest, pathCorrectConfig)
{
	const char* argv[2] = {"./webserver", "pathCorrectConfig"};
	configFile.open("pathCorrectConfig");
	configFile << "port 8080;\npath /echo EchoHandler{}\npath /static StaticHandler\n{\n    root ./test_folder;\n}\n";

	configFile.close();
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("pathCorrectConfig");
	EXPECT_EQ(errorCode, 0);
}

TEST_F(ConfigTest, noRootPathConfig)
{
	const char* argv[2] = {"./webserver", "noRootPathConfig"};
	configFile.open("noRootPathConfig");
	configFile << "port 8080;\npath /echo reply_echo;\npath /static reply_static\n{\n    root;\n}\n\npath /testpath reply_static\n{\n    root /home/ubuntu;\n}\n";
	configFile.close();
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("noRootPathConfig");
	EXPECT_EQ(errorCode, 3);
}

TEST_F(ConfigTest, serverUnrecognizedConfig)
{
	const char* argv[2] = {"./webserver", "serverUnrecognizedConfig"};
	configFile.open("serverUnrecognizedConfig");
	configFile << "port 8080;\npath /echo reply_echo;\npath /static reply_static\n{\n    root /home/ubuntu;\n}\n\npath /testpath reply_sta\n{\n    root /home/ubuntu;\n}\n";
	configFile.close();
	int errorCode = Server::parseConfig(2, argv, configArgs);
	remove("serverUnrecognizedConfig");
	EXPECT_EQ(errorCode, 3);
}

/*
TEST(ServerTest, doAcceptTest)
{
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    std::string cwd(buff);
    configArguments configArgs;
    configArgs.port = 8080;
    //configArgs.baseDirectory = cwd;
    configArgs.echo_path = "reply_echo";
    configArgs.static_path = "reply_static";
    Server server(configArgs);
    server.run();
    
    io_service io_service;
    ip::tcp::acceptor acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(),8081));
    std::shared_ptr<session> sesh = std::make_shared<session>(io_service, configArgs.map_path_rootdir, configArgs.echo_path, configArgs.static_path);
    std::string str = "http /echo/a.txt 1.0\r\n";
    asio::async_write(sesh->socket, boost::asio::buffer(str.c_str(), str.length()), [sesh, str](const error_code& e, std::size_t s)
    {
      //return str->c_str();
    });
    
    EXPECT_EQ(server.getTotalRequestCount(), 1);
}
*/
