import httplib
import subprocess
from multiprocessing import Process
from time import sleep
import os
import signal
import filecmp
import re
import time

HOSTNAME = "localhost"
PORT = 8080
CONFIGFILENAME = "config"

def serverExec():
	subprocess.call(["./webserver", CONFIGFILENAME])

def testCasesGenerator(url, method, expected_body, case_number):
    serverProcess = Process(target=serverExec)
    serverProcess.start()
    sleep(3)
    connection = httplib.HTTPConnection(HOSTNAME, PORT)
    connection.request(method, url)
    response = connection.getresponse()
    body = response.read()
    if expected_body == body:
        print "Case{0:d} passed!".format(case_number)
    else:
        print "Case{0:d} failed!".format(case_number)
    connection.close()
    subprocess.call(["fuser", "-k", str(PORT) + "/tcp"])
    serverProcess.terminate()

def testCase1():
    testCasesGenerator("/echo", "GET", "GET /echo HTTP/1.1\r\nHost: localhost:8080\r\nAccept-Encoding: identity\r\n\r\n", 1)
    
def testCase2():
    testCasesGenerator("/static/b.txt", "GET", "File Not found or No Handlers to handle uri.", 2)
    
def testCase3():
    # this test case first create a text file in the root path
    # and then request this text file through localhost.
    basePath = ""
    configFile = open(CONFIGFILENAME, 'r')
    for line in configFile:
        tokens = line.lstrip(' ')
        tokens = re.split(' ', tokens)
        if tokens[0] == "base-path" or tokens[0] == "root":
            basePath = tokens[1].replace(";\n", "")
            break;
    configFile.close()
    testFile = open(basePath + "/b.txt", 'w')
    testFile.write("abcde")
    testFile.close()
    testCasesGenerator("/static/b.txt", "GET", "abcde", 3)
    os.remove(basePath + "/b.txt")
    
def multiThreadingTestCase():
    # this test case first send a partial request(only the request line
    # of the http request), then send another complete request. Finally
    # finish the incomplete request
    print "Multi-threading test: "
    serverProcess = Process(target=serverExec)
    serverProcess.start()
    sleep(3)
    connection = httplib.HTTPConnection(HOSTNAME, PORT)
    connection2 = httplib.HTTPConnection(HOSTNAME, PORT)
    connection.putrequest("GET", "/echo") # This will send only the request line to the server. So the request is not complete
    connection2.request("GET", "/static/b.txt")
    response2 = connection2.getresponse()
    body2 = response2.read()
    connection.endheaders()
    connection.send("")
    response = connection.getresponse()
    body = response.read()
    connection.close()
    connection2.close()
    subprocess.call(["fuser", "-k", str(PORT) + "/tcp"])
    serverProcess.terminate()
    if body == "GET /echo HTTP/1.1\r\nHost: localhost:8080\r\nAccept-Encoding: identity\r\n\r\n" and body2 == "File Not found or No Handlers to handle uri.":
        print "Multi-threading test case passed!"
    else:
        print "Error: multi-threading test case is not passed!"

def main():
    subprocess.call(["make"])
    testCase1()
    testCase2()
    testCase3()
    multiThreadingTestCase()
    
if __name__ == "__main__":
    try:
        main()
    except:
        pid = subprocess.check_output(["pidof", "-s", "./webserver"])
        if not pid == "":
            os.kill(int(pid), signal.SIGKILL)
        raise

