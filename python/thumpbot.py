import GPIO as GPIO
import Serial as Serial
import Serial as mySerial
import Servo as myServo
import time
import Ping

# # blinking function
# def blink(pin):
#         GPIO.output(pin,GPIO.HIGH)
#         time.sleep(1)
#         GPIO.output(pin,GPIO.LOW)
#         time.sleep(1)
#         return
#
# # to use Raspberry Pi board pin numbers
# GPIO.setmode(GPIO.BOARD)
# # set up GPIO output channel
# GPIO.setup(11, GPIO.OUT)
# # blink GPIO17 50 times
# for i in range(0, 10):
#         blink(11)
# GPIO.cleanup()

ON_SWITCH = 3
AUTOPILOT_SWITCH = 4
LEFT_LED = 12
RIGHT_LED = 13
SERVO_PIN = 9
PING_PIN = 6

PPOS = 0
CPOS = 0

def init():
        PPOS = 0
        CPOS = 90

        GPIO.output(RIGHT_LED, GPIO.HIGH)
        GPIO.output(LEFT_LED, GPIO.HIGH)
        time.sleep(.5)
        GPIO.output(RIGHT_LED, GPIO.LOW)
        GPIO.output(LEFT_LED, GPIO.LOW)

        myServo.attach(SERVO_PIN)
        myServo.goTo(90)
        time.sleep(.5)
        myServo.detach()

        Ping.attach(24)


def ping():
        return Ping.distance()
        # return 42


def noGOGO():
        print("NO GO GO!")


def GOGO():
        print("GO GO Forward")


def goBackLeft():
        print("GO back left")


def goBackRight():
        print("GO back right")


def delay(ms):
        time.sleep(ms/1000)


def lightsOut():
        GPIO.output(LEFT_LED, GPIO.LOW)
        GPIO.output(RIGHT_LED, GPIO.LOW)


def turnRight(wait):
        print("TURN right")
        delay(wait)


def turnLeft(wait):
        print("TURN left")
        delay(wait)


def moveDatServo():
        global CPOS
        global PPOS

        myServo.attach(SERVO_PIN)

        if CPOS == 180:
                # Left most edge
                PPOS = 181
                CPOS = 150
                myServo.goTo(CPOS)
        elif CPOS == 0:
                # Right most edge
                PPOS = -1
                CPOS = 30
                myServo.goTo(CPOS)
        elif PPOS < CPOS and CPOS < 180:
                # We're moving counter clockwise toward 140
                PPOS = CPOS
                CPOS = CPOS + 30
                myServo.goTo(CPOS)
        elif PPOS > CPOS and CPOS > 0:
                # We're moving clockwise toward 20
                PPOS = CPOS
                CPOS = CPOS - 30
                myServo.goTo(CPOS)

        time.sleep(0.09)
        myServo.detach()


def goBackCheck():
        noGOGO()

        myServo.goToAndWait(160, 0.25)
        cmR = ping()

        myServo.goToAndWait(20, 0.25)
        cmL = ping()

        if cmR > cmL:
                goBackLeft()
        else:
                goBackRight()

        delay(600)



def avoidCollision():
        cm = ping()
        if CPOS == 90:
                if cm < 60:
                        # Hazard ahead!
                        GPIO.output(LEFT_LED, GPIO.HIGH)
                        GPIO.output(RIGHT_LED, GPIO.HIGH)
                        goBackCheck()
        elif CPOS == 120:
                if cm < 70:
                        # Hazard to left!
                        GPIO.output(LEFT_LED, GPIO.HIGH)
                        turnRight(140)
        elif CPOS == 60:
                if cm < 70:
                        # Hazard to right!
                        GPIO.output(RIGHT_LED, GPIO.HIGH)
                        turnLeft(140)
        elif CPOS == 180:
                if cm < 55:
                        # Hazard to far left!
                        GPIO.output(LEFT_LED, GPIO.HIGH)
                        turnRight(200)
        elif CPOS == 0:
                if cm < 55:
                        # Hazard to far right!
                        GPIO.output(RIGHT_LED, GPIO.HIGH)
                        turnLeft(200)

        lightsOut()


GPIO.setmode(GPIO.BOARD)
GPIO.setup(ON_SWITCH, GPIO.IN)
GPIO.setup(AUTOPILOT_SWITCH, GPIO.IN)
GPIO.setup(LEFT_LED, GPIO.OUT)
GPIO.setup(RIGHT_LED, GPIO.OUT)

Serial.begin(9600)
mySerial.begin(19200)

init()

while True:
        if GPIO.digitalRead(AUTOPILOT_SWITCH) == GPIO.HIGH:
                print("Auto pilot engaged!")

                print("Looking for obstacles")
                moveDatServo()
                avoidCollision()
                GOGO()
        else:
                print("Manual Mode")
                # readAndGo();
