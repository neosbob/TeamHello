import subprocess
import os
import time

webserver = subprocess.Popen(['./webserver', 'proxy_302_config'])
time.sleep(0.01)
curl_proc = subprocess.call(["curl", "-s", "localhost:8099", "-o", "output1"])
curl_proc = subprocess.call(["curl", "-s", "www.usc.edu", "-o", "output2"])
diff_proc = subprocess.Popen(["diff", "-u", "output1", "output2"], stdout=subprocess.PIPE)
diff = diff_proc.stdout.read().decode('utf-8')
webserver.kill()
os.remove('output1')
os.remove('output2')
if diff != '':
	print "Proxy 302 test failed!"
	print diff
	exit(1)
else:
	print "Proxy 302 test passed!"
