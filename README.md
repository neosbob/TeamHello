# TeamHello

# Collaborators: 

Bob Wu

Yanzun Huang

Evelyn Chen

# Usage:

git clone https://github.com/UCLA-CS130/TeamHello.git

make clean

make test

make test_coverage

make integration_test

./webserver SimpleConfig

Can test on browser: 

(Can edit SimpleConfig for different file paths, port number...etc)

localhost:8080/static/apple.jpg

localhost:8080/static/sample.html

localhost:8080/testpath/orange.jpg

localhost:8080/echo

# Measure test coverage

-Functions that require usage of asyn_read or async_write are tested minimally.

-Reason: 

    -requires client to provide a message to socket that server can read from.

    -Can't reproduce in unit tests.

-Partial solution: 

    -Refactored such functions to test data that async functions would 
     have worked with 

