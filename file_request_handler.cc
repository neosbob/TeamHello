//Inspired by Boost example for async_client.cpp and its use of async_read_until
//www.boost.org/doc/libs/1_43_0/doc/html/boost_asio/example/http/client/async_client.cpp

#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <sstream>
#include <ifstream>
#include <cstdlib>
#include <utility>
#include "file_request_handle.h"

using namespace boost;
using namespace boost::system;
using namespace boost::asio;


std::string FileRequestHandler::buildResponseStr(const std::string& body)
{
    return "";
}

std::string FileRequestHandler::getFileContent(const std::string& fileName)
{
    std::ifstream ifs(fileName.c_str(), std::ios::in | std::ios::binary);
    if (ifs)
    {
        stringstream sstr;
        sstr << ifs.rdbuf();
        ifs.close();
        return sstr.str();
    }
    return "";
}
