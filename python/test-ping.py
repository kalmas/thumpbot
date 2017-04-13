import Ping
import time

PING_PIN = 16

Ping.attach(PING_PIN)


while True:
	cm = Ping.distance()
	print("distance is " + str(cm) + " cm")

	time.sleep(1)
