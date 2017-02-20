CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
GTEST_DIR=googletest/googletest
SERVERCLASSES=config_parser.cc

default:  config_parser config_parser_test request_handler.o request_handler_echo.o request_handler_static.o request_handler_notfound.o mime_types.o session.o webserver webserver_test session_test request_handler_echo_test request_handler_static_test

request_handler.o: request_handler.cc request_handler.h mime_types.h mime_types.cc
	g++ -c -std=c++11 request_handler.cc -lboost_system

request_handler_static.o: request_handler_static.cc request_handler_static.h request_handler.h mime_types.h mime_types.cc
	g++ -c -std=c++11 request_handler_static.cc -lboost_system

request_handler_echo.o: request_handler_echo.cc request_handler_echo.h request_handler.h mime_types.h mime_types.cc
	g++ -c -std=c++11 request_handler_echo.cc -lboost_system

request_handler_notfound.o: request_handler_notfound.cc request_handler_notfound.h request_handler.h mime_types.h mime_types.cc
	g++ -c -std=c++11 request_handler_notfound.cc -lboost_system

session.o: session.cc session.h request_handler.h request_handler_echo.h request_handler_static.h
	g++ -c -std=c++11 session.cc  -lboost_system

mime_types.o: mime_types.cc mime_types.h
	g++ -c -std=c++11 mime_types.cc

webserver: webserver.h webserver.cc webserver_main.cc config_parser.h config_parser.cc session.h request_handler.o session.o mime_types.o request_handler_static.o request_handler_echo.o
	g++ webserver.h webserver.cc webserver_main.cc config_parser.cc request_handler.o session.o mime_types.o request_handler_static.o request_handler_echo.o -I /usr/local/Cellar/boost/1.54.0/include -std=c++11 -lboost_system -o webserver


config_parser: config_parser.cc config_parser_main.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)

request_handler_static_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_handler_static_test.cc request_handler_static.cc mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handler_static_test -lboost_system

request_handler_echo_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_handler_echo_test.cc request_handler_echo.cc mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handler_echo_test -lboost_system

request_handler_test: 
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_handler_test.cc request_handler.cc mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handler_test -lboost_system

session_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread session_test.cc request_handler.cc request_handler.h session.cc session.h mime_types.cc request_handler_static.cc request_handler_echo.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o session_test -lboost_system

webserver_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread webserver_test.cc webserver.cc config_parser.cc request_handler.cc session.cc mime_types.cc request_handler_static.cc request_handler_echo.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system

config_parser_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM *.gcda *.gcno *.gcov config_parser config_parser_test *.tar.gz webserver request_handler.o session.o request_handler_test webserver_test session_test request_handler_echo_test request_handler_static_test

test: clean default
	./webserver_test
	./session_test
	./request_handler_echo_test
	./request_handler_static_test

integration_test: clean default
	python integration_test.py
	
test_coverage:

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread session_test.cc request_handler.cc request_handler.h session.cc mime_types.cc request_handler_static.cc request_handler_echo.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o session_test -lboost_system -fprofile-arcs -ftest-coverage
	./session_test; gcov -r session.cc
	
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread webserver_test.cc webserver.cc config_parser.cc request_handler.cc request_handler.h session.cc mime_types.cc request_handler_static.cc request_handler_echo.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system -fprofile-arcs -ftest-coverage
	./webserver_test; gcov -r webserver.cc
  
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_handler_echo_test.cc request_handler_echo.cc mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handler_echo_test -lboost_system -fprofile-arcs -ftest-coverage
	./request_handler_echo_test; gcov -r request_handler_echo.cc

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_handler_static_test.cc request_handler_static.cc mime_types.cc mime_types.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handler_static_test -lboost_system -fprofile-arcs -ftest-coverage
	./request_handler_static_test; gcov -r request_handler_static.cc

	

	
