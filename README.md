# TeamHello

# Collaborators: 

Bob Wu

Yanzun Huang

Evelyn Chen

# Usage:

git clone https://github.com/UCLA-CS130/TeamHello.git

make 

./webserver SimpleConfig

Can test on browser with:

localhost:8080/static/apple.jpg
localhost:8080/static/sample.html
localhost:8080/testpath/orange.jpg
localhost:8080/echo

- The port number, keyword, and path name can be changed in SimpleConfig
- The default ones are above 

# Run Integration Test:

./integration_test.sh

or 

make

make integration_test

To run integration_test, you have to change the SimpleConfig file to include the path in your computer 

# Run all unit tests

make 

make test 

# Measure test coverage

make

make test_coverage

-Functions that require usage of asyn_read or async_write are tested minimally.

-Reason: 

    -requires client to provide a message to socket that server can read from.

    -Can't reproduce in unit tests.

-Partial solution: 

    -Refactored such functions to test data that async functions would 
     have worked with 

# To test with /static/file_name

whenever /static/file_name is tested, base-path in SimpleConfig needs to edited to be the path on personal computer where the file is served from
