# TeamHello

# Collaborators: 

Bob Wu

Yanzun Huang

Evelyn Chen

# Usage:

git clone https://github.com/UCLA-CS130/TeamHello.git

make 

./webserver SimpleConfig

# Run Integration Test:

./integration_test.sh

or 

make integration_test

# Run all unit tests

make test 

# Measure test coverage

make test_coverage

-Functions that require usage of async_read or async_write are tested minimally.
-Reason: requires client to provide a message to socket that server can read from.
	 Can't reproduce in unit tests.
-Partial solution: Refactored such functions to test data that async functions 
		   would have worked with.

