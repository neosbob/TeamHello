# TeamHello Webserver API

## Collaborators

* Bob Wu
* Yanzun Huang
* Evelyn Chen

## To Clone the Repository

To clone the repository to own machine:

```
git clone --recursive https://github.com/UCLA-CS130/TeamHello.git
```

## Config File
Edit `config` file to specify the port, the URI prefix, the root directory, and Handlers.

Config file format:

```
port <port_num>;

# specify uri prefix for each handler
path /static StaticHandler{
	root ./test_folder;
}

path /echo EchoHandler{}

path /<uri_prefix> <handler_name> {
	root /<root_dir>;
}

#if no handler matches, call default handler
default <default_handler>{

}

```

## Source Code

### Request

Request class represents an HTTP request. 
Usage: ` auto request = Request::Parse(raw_request)`

```cpp
static std::unique_ptr<Request> Parse(const std::string& raw_request);
std::string read_request_line(std::string line);
bool read_header(std::string headers);

std::string raw_request() const;
std::string method() const;
std::string uri() const;
std::string version() const;
Headers headers() const;
std::string body() const;
```

### Response

The Response class represents an HTTP response. It is constructed by the RequestedHandler, after which the server should call `ToString` to serialize.
 
Usage:
```cpp
Response r;
r.SetStatus(RESPONSE_200);
r.SetBody(...);
return r.ToString();
```


```cpp
enum ResponseCode {
	ok = 200,
	bad_request = 400,
	not_found = 404,
	internal_server_error = 500,
	not_implemented = 501,
};
  
void SetStatus(const ResponseCode response_code);
void AddHeader(const std::string& header_name, const std::string& header_value);
void SetBody(const std::string& body);
std::string ToString();
```


### Request Handler

Represents the parent of all request handlers. 

* `Init` function initializes the handler and returns a response code indicating success of failure condition. `uri_prefix` is the value in the config file that this handler will run for. `config` i sthe contents of the child block for this handler only. 

* `HandleRequest` handles an HTTP request and generates a response. It returns a response code indicating success or failure condition. 

* `CreateByName` generates a pointer to the handler specified in the argument. For example, `auto new_file_handler = RequestHandler::CreateByName("StaticHandler"))`.

* Add this line to the end of `<handler_class>.h` file whenever a new handler class is added. Replace <handler_class> with the name of handler, such as StaticHandler or EchoHandler: 
`REGISTER_REQUEST_HANDLER(<handler_class>)`

```cpp
enum Status {
	OK = 0,
	FAILED = 1
};
  
static RequestHandler* CreateByName(const char* type);
virtual Status Init(const std::string& uri_prefix, const NginxConfig& config) = 0;
virtual Status HandleRequest(const Request& request, Response* response) = 0;
```


#### StaticHandler

StaticHandler returns a specified file to the server. `root_dir` and `prefix` are set at the `Init` function, with both from the Config file. 

`HandleRequest` checks if the file exists, then set the status, headers, and body to the response. 

#### EchoHandler

EchoHandler echos the request back to the server. 

#### NotFoundHandler

NotFoundHandler returns a 404 Not Found response. 

### Session

* `GetRequestHandler` goes through the handlers map and returns the corresponding handler pointer 

* `write_response` writes the response to the socket 

```cpp
std::map<std::string, RequestHandler*> handlers;
RequestHandler* default_handler;
RequestHandler* GetRequestHandler(const std::string& path);

static void read_whole_request(std::shared_ptr<session> pThis);
std::string write_response(Response& response, std::shared_ptr<session> pThis);
```

### Server

* `parseConfig` parses the config file and store the config into `configArguments` struct 

```cpp
static Server* serverBuilder(const NginxConfig& config_out);
void run();
int getTotalRequestCount(){return totalRequestCount;}
	
Server(const configArguments& configArgs);
void doAccept();
static int parseConfig(const NginxConfig& config_out, configArguments& configArgs);
```


## Dispatch Mechanism

In webserver_main.cc, config parser parses the config and pass it to `serverBuilder` in webserver.cc 

In webserver.cc `serverBuilder`, `parseConfig` parses config into configArguments, which is a struct defined in webserver.h, containing port, handler mapping, and a pointer to default handler

In session.cc, request is read from the socket. `GetRequestHandler` is used to determine the appropriate handler, and response is written to the socket. 

## Test

`make test` to run all unit tests

`make integration_test` to run integration test

`make test_coverage` to run for test coverage 

## Build

`make clean` to clean old builds

`make` to compile

`make <target>` to compile a specific file. Check Makefile for all the make options.


## Run

```
$ make clean
$ make
$ ./webserver <config-file>
```

In chrome browser:
`localhost::<port>/<uri_prefix>/<file_name>`

<file_name> is optional. It depends on the type of handler. 
