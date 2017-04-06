HIGH = "ON"
LOW = "OFF"
BOARD = "BOARD"
OUT = "OUT"
IN = "IN"

class PulseWidthModulized:
    def ChangeDutyCycle(self, dc):
        print("Duty cycle is:" + str(dc))

    def stop(self):
        print("pwm stopped")


def setmode(mode):
    print(str(mode))


def setup(pin, mode):
    print("Set up: " + str(pin) + " " + str(mode))


def output(pin, mode):
    print("Output: " + str(pin) + " " + str(mode))


def digitalRead(pin):
    return HIGH


def digitalWrite(pin, state):
    print("Pin" + str(pin) + " set to  " + str(state))


def PWM(pin, pwm):
    print("Pin" + str(pin) + " set to  " + str(pwm))
    return PulseWidthModulized()


def cleanup():
    print("Cleanup.")


def input(pin):
    return HIGH
