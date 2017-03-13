import httplib
import filecmp

HOSTNAME = "54.201.90.157"


def testCase():
    connection1 = httplib.HTTPConnection(HOSTNAME)
    connection1.request("GET", "/cloud/cs130-assign9/Fish.jpg")
    response1 = connection1.getresponse()
    body1 = response1.read()
    
    connection2 = httplib.HTTPConnection("s3-us-west-1.amazonaws.com")
    connection2.request("GET", "/cs130-assign9/Fish.jpg")
    response2 = connection2.getresponse()
    expected_body = response2.read()
    if expected_body == body1:
        print "Case passed!"
    else:
        print "Case failed!".format
    connection1.close()
    connection2.close()
    

def main():
    testCase()
    
if __name__ == "__main__":
    main()

