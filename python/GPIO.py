HIGH = "ON"
LOW = "OFF"
BOARD = "BOARD"
OUT = "OUT"
IN = "IN"

def output(pin, on):
    print("Pin " + str(pin) + ": " + on)

def input(pin):
    return HIGH

def setmode(mode):
    print("Mode set:" + mode)

def setup(pin, mode):
    print("Pin" + str(pin) + " set to  " + mode)

def cleanup(self):
    print("Cleanup.")

def digitalRead(pin):
    return HIGH

def digitalWrite(pin, state):
    print("Pin" + str(pin) + " set to  " + str(state))
