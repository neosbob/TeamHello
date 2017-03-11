CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
GTEST_DIR=googletest/googletest
SERVERCLASSES=config_parser.cc

default:  config_parser request_handler.o echo_handler.o file_handler.o not_found_handler.o status_handler.o request.o response.o mime_types.o session.o webserver

build: Dockerfile
	docker build -t webserver.build .
	docker run --rm webserver.build > deploy/webserver.tar

#deploy will not work if a .pem for the server is not in the work_dir.
deploy: deploy/Dockerfile.run deploy/webserver.tar
	tar -xf deploy/webserver.tar --directory deploy/       
	chmod 0755 deploy/webserver
	
	#This builds the shruken image.
	cd deploy && docker build -f Dockerfile.run -t webserver.deploy .
	#The following copies the image to the AWS-ec2
	docker save webserver.deploy | bzip2 | pv | ssh -i CS130-TeamHello-Assign8-ec2.pem ec2-user@ec2-54-201-90-157.us-west-2.compute.amazonaws.com 'bunzip2 | docker load'
	#The following runs make using the new image on AWS-ec2 server
	ssh -i CS130-TeamHello-Assign8-ec2.pem ec2-user@ec2-54-201-90-157.us-west-2.compute.amazonaws.com 'make deploy'

file_handler.o: file_handler.cc file_handler.h request_handler.h mime_types.h mime_types.cc response.cc response.h request.cc request.h
	g++ -c -std=c++11 file_handler.cc -lboost_system

echo_handler.o: echo_handler.cc echo_handler.h request_handler.h mime_types.h mime_types.cc response.cc response.h request.cc request.h
	g++ -c -std=c++11 echo_handler.cc -lboost_system

not_found_handler.o: not_found_handler.cc not_found_handler.h request_handler.h mime_types.h mime_types.cc response.cc response.h request.cc request.h
	g++ -c -std=c++11 not_found_handler.cc -lboost_system
	
status_handler.o: status_handler.cc status_handler.h request_handler.h mime_types.h mime_types.cc response.cc response.h request.cc request.h
	g++ -c -std=c++11 status_handler.cc -lboost_system -lboost_thread

request.o: request.cc request.h
	g++ -c -std=c++11 request.cc -lboost_system

response.o: response.cc response.h 
	g++ -c -std=c++11 response.cc -lboost_system

session.o: session.cc session.h request_handler.h echo_handler.h file_handler.h status_handler.h response.cc response.h request.cc request.h webserver.h
	g++ -c -std=c++11 session.cc  -lboost_system -lboost_thread

mime_types.o: mime_types.cc mime_types.h
	g++ -c -std=c++11 mime_types.cc

webserver: webserver.h webserver.cc webserver_main.cc config_parser.h config_parser.cc session.h request_handler.o session.o mime_types.o file_handler.o echo_handler.o not_found_handler.o status_handler.o request.o response.o http_client.o proxy_handler.o
	g++ webserver.h webserver.cc webserver_main.cc config_parser.cc request_handler.o session.o mime_types.o file_handler.o echo_handler.o not_found_handler.o status_handler.o request.o response.o http_client.o proxy_handler.o -I cpp-markdown/*.cpp -I /usr/local/Cellar/boost/1.54.0/include -std=c++11 -static-libgcc -static-libstdc++ -pthread -Wl,-Bstatic -lboost_thread -lboost_system -lboost_regex -o webserver


config_parser: config_parser.cc config_parser_main.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)

file_handler_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread file_handler_test.cc file_handler.cc file_handler.h request_handler.cc request_handler.h mime_types.cc mime_types.h request.cc request.h response.cc response.h not_found_handler.cc not_found_handler.h config_parser.cc config_parser.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o file_handler_test -lboost_system

echo_handler_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread echo_handler_test.cc echo_handler.cc request_handler.cc request_handler.h request.cc request.h response.cc response.h mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o echo_handler_test -lboost_system

request_handler_test: 
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_handler_test.cc request_handler.cc mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handler_test -lboost_system

not_found_handler_test: 
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread not_found_handler_test.cc not_found_handler.cc request_handler.cc request_handler.h mime_types.cc mime_types.h request.cc request.h response.cc response.h file_handler.cc file_handler.h config_parser.cc config_parser.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o not_found_handler_test -lboost_system


request_test: 
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_test.cc request.cc response.cc response.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_test -lboost_system

response_test: 
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread response_test.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o response_test -lboost_system


session_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread session_test.cc request_handler.cc request_handler.h session.cc session.h mime_types.cc file_handler.cc echo_handler.cc not_found_handler.cc request.cc response.cc webserver.h webserver.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o session_test -lboost_system -lboost_thread

webserver_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread webserver_test.cc webserver.cc config_parser.cc request_handler.cc session.cc mime_types.cc file_handler.cc echo_handler.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system

config_parser_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test

http_client_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread http_client_test.cc http_client.cc proxy_handler.cc session.cc config_parser.cc request_handler.cc file_handler.cc not_found_handler.cc request.cc webserver.cc mime_types.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o http_client_test -lboost_system -pthread -lboost_thread -fprofile-arcs -ftest-coverage

proxy_handler_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system -lboost_thread
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread proxy_handler_test.cc proxy_handler.cc session.cc http_client.cc not_found_handler.cc mime_types.cc config_parser.cc request_handler.cc file_handler.cc request.cc response.cc webserver.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o proxy_handler_test -pthread -lboost_system -lboost_thread -fprofile-arcs -ftest-coverage

clean:
	rm -rf *.o *.a *~ *.gch *.swp *.dSYM *.gcda *.gcno *.gcov config_parser config_parser_test *.tar.gz webserver request_handler.o session.o request_handler_test webserver_test session_test echo_handler_test file_handler_test response_test request_test not_found_handler_test session_test http_client_test proxy_handler_test

test: clean config_parser_test echo_handler_test file_handler_test not_found_handler_test response_test request_test session_test http_client_test proxy_handler_test
	./echo_handler_test
	./file_handler_test
	./response_test
	./request_test
	./not_found_handler_test
	./session_test
	./http_client_test
	./proxy_handler_test
	./config_parser_test

integration_test: clean default
	python integration_test.py

reverse_proxy_integration_test: webserver
	python integration_test_proxy.py

reverse_proxy_302_test: webserver
	python reverse_proxy_302.py
	
test_coverage:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread echo_handler_test.cc echo_handler.cc request_handler.cc request_handler.h request.cc request.h response.cc response.h mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o echo_handler_test -lboost_system -fprofile-arcs -ftest-coverage
	./echo_handler_test; gcov -r echo_handler.cc

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread file_handler_test.cc file_handler.cc request_handler.cc request_handler.h mime_types.cc mime_types.h request.cc request.h response.cc response.h not_found_handler.cc not_found_handler.h config_parser.cc config_parser.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o file_handler_test -lboost_system -fprofile-arcs -ftest-coverage
	./file_handler_test; gcov -r file_handler.cc

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread not_found_handler_test.cc not_found_handler.cc request_handler.cc request_handler.h mime_types.cc mime_types.h request.cc request.h response.cc response.h file_handler.cc file_handler.h config_parser.cc config_parser.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o not_found_handler_test -lboost_system -fprofile-arcs -ftest-coverage
	./not_found_handler_test; gcov -r not_found_handler.cc

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread response_test.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o response_test -lboost_system -fprofile-arcs -ftest-coverage
	./response_test; gcov -r response.cc

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_test.cc request.cc response.cc response.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_test -lboost_system -fprofile-arcs -ftest-coverage
	./request_test; gcov -r request.cc

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread session_test.cc request_handler.cc session.cc mime_types.cc file_handler.cc echo_handler.cc not_found_handler.cc request.cc response.cc webserver.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o session_test -lboost_system -fprofile-arcs -ftest-coverage
	./session_test; gcov -r session.cc

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread http_client_test.cc http_client.cc proxy_handler.cc session.cc config_parser.cc request_handler.cc file_handler.cc not_found_handler.cc request.cc webserver.cc mime_types.cc response.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o http_client_test -lboost_system -pthread -lboost_thread -fprofile-arcs -ftest-coverage
	./http_client_test; gcov -r http_client.cc

proxy_handler_unit_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread proxy_handler_test.cc proxy_handler.cc session.cc http_client.cc not_found_handler.cc mime_types.cc config_parser.cc request_handler.cc file_handler.cc request.cc response.cc webserver.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o proxy_handler_test -lboost_system -pthread -lboost_thread -fprofile-arcs -ftest-coverage
	./proxy_handler_test; gcov -r proxy_handler.cc
