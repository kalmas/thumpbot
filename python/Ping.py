import GPIO as GPIO
import time

PIN = 0

def attach(pin):
    global PIN

    GPIO.setmode(GPIO.BOARD)
    PIN = pin


def distance():
    print("pinging on " + str(PIN))
    GPIO.setup(PIN, GPIO.OUT)
    GPIO.output(PIN, GPIO.HIGH)

    time.sleep(0.00001)
    GPIO.output(PIN, GPIO.LOW)

    startTime = None
    stopTime = None

    GPIO.setup(PIN, GPIO.IN)
    while GPIO.input(PIN) == GPIO.LOW:
        startTime = time.time()

    while GPIO.input(PIN) == GPIO.HIGH:
        stopTime = time.time()

    timeElapsed = stopTime - startTime

    d = (timeElapsed * 34300) / 2

    return d
