HIGH = "ON"
LOW = "OFF"
BOARD = "BOARD"
OUT = "OUT"
IN = "IN"

def output(pin, on):
    print("Pin " + str(pin) + ": " + on)

def output(pin):
    print("Pin " + str(pin) + ": " + on)

def setmode(mode):
    print("Mode set:" + mode)

def setup(pin, mode):
    print("Pin" + str(pin) + " set to  " + mode)

def cleanup(self):
    print("Cleanup.")

def digitalRead(pin):
    return GPIO.HIGH

def digitalWrite(pin, state):
    print("Pin" + str(pin) + " set to  " + str(state))
