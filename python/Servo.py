import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)
p = None

def convert(degrees):
    x = degrees / 180
    y = x * 10
    z = y + 2.5
    return z


def attach(pin):
    global p

    GPIO.setup(pin, GPIO.OUT)
    p = GPIO.PWM(pin, 50)
    print("Servo attached to pin: " + str(pin))


def goTo(degree):
    dc = convert(degree)
    p.ChangeDutyCycle(dc)
    print("Servo go to " + str(dc) + " | " + str(degree))


def goToAndWait(degree, wait):
    goTo(degree)
    time.sleep(wait)


def detach():
    p.stop()
    GPIO.cleanup()
    print("Servo detached")
