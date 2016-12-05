import os
import signal
import sys

os.kill(int(sys.argv[1]), signal.SIGTERM)


while True:
	print("live")
