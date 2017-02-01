CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
GTEST_DIR=googletest/googletest
SERVERCLASSES=config_parser.cc

default:  config_parser config_parser_test reply_test reply.o request_handle.o webserver webserver_test request_handle_test

reply_test: 
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread reply_test.cc reply.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o reply_test -lboost_system


reply.o: reply.cc reply.h
	g++ -c -std=c++11 reply.cc -lboost_system

request_handle.o: request_handle.cc request_handle.h reply.h
	g++ -c -std=c++11 request_handle.cc  

request_handle_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_handle_test.cc reply.cc reply.h request_handle.cc request_handle.h ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handle_test -lboost_system

config_parser: config_parser.cc config_parser_main.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)

config_parser_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc config_parser.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test

webserver: webserver.h webserver.cc webserver_main.cc config_parser.h config_parser.cc request_handle.h reply.o request_handle.o
	g++ webserver.h webserver.cc webserver_main.cc config_parser.cc reply.o request_handle.o -I /usr/local/Cellar/boost/1.54.0/include -std=c++11 -lboost_system -o webserver

webserver_test:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread webserver_test.cc webserver.cc config_parser.cc reply.cc request_handle.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system

clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM *.gcda *.gcno *.gcov config_parser config_parser_test *.tar.gz webserver reply.o request_handle.o reply_test webserver_test request_handle_test

test: clean default
	./reply_test
	./webserver_test
	./request_handle_test

integration_test: clean default
	./integration_test.sh
	
test_coverage:
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread reply_test.cc reply.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o reply_test -lboost_system -fprofile-arcs -ftest-coverage
	./reply_test; gcov -r reply.cc

	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread request_handle_test.cc reply.cc reply.h request_handle.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o request_handle_test -lboost_system -fprofile-arcs -ftest-coverage
	./request_handle_test; gcov -r request_handle.cc
	
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -lboost_system
	ar -rv libgtest.a gtest-all.o
	g++ -std=c++11 -isystem ${GTEST_DIR}/include -pthread webserver_test.cc webserver.cc config_parser.cc reply.cc request_handle.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o webserver_test -lboost_system -fprofile-arcs -ftest-coverage
	./webserver_test; gcov -r webserver.cc

	

