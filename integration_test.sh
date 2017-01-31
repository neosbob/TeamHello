#!/bin/bash

set -e #if any command fails, abort
make

echo "Running web server now..."
./webserver SimpleConfig &>/dev/null & #runs silently in the background

echo "Sending a request to the web server..."
curl -s -I http://localhost:8080 > actual.txt

echo "Shutting down web server..."
pkill webserver # shuts down the server 

diff expected.txt actual.txt

if [ $? -eq 0 ]
then
	echo "The exit code is" $?
	echo "Test succeeded!"
else
	echo "The exit code is" $?
	echo "Test failed!"
fi 
