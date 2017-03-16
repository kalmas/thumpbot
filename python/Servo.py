import time

def attach(pin):
    print("Servo attached to pin: " + str(pin))

def goTo(degree):
    print("Servo go to: " + str(degree))

def goToAndWait(degree, wait):
    print("Servo go to: " + str(degree))
    time.sleep(wait)

def detach():
    print("Servo detached")