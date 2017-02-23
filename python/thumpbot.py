import RPi.GPIO as GPIO
import Serial as Serial
import Serial as mySerial
import Servo as myServo
import time

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

prevpos = 0
cpos = 0


def initServo():
        prevpos = 0
        cpos = 90

        myServo.attach(SERVO_PIN)
        myServo.write(90)
        time.sleep(.5)
        myServo.detach()


def distance():
        # set Trigger to HIGH
        GPIO.output(PING_PIN, GPIO.HIGH)

        # set Trigger after 0.01ms to LOW
        time.sleep(0.00001)
        GPIO.output(PING_PIN, GPIO.LOW)

        startTime = None
        stopTime = None

        while GPIO.input(PING_PIN) == GPIO.LOW:
                startTime = time.time()

        while GPIO.input(PING_PIN) == GPIO.HIGH:
                stopTime = time.time()

        # time difference between start and arrival
        timeElapsed = stopTime - startTime

        # multiply with the sonic speed (34300 cm/s)
        # and divide by 2, because there and back
        distance = (timeElapsed * 34300) / 2

        return distance


def moveDatServo():
        if cpos == 180:
                # Left most edge.
                prevpos = 181
                cpos = 150
                myServo.write(cpos)
        elif cpos == 0:
                # Right most edge.
                prevpos = -1
                cpos = 30
                myServo.write(cpos)
        elif prevpos < cpos anf cpos < 180:
                # We're moving counter clockwise toward 140.
                prevpos = cpos
                cpos = cpos + 30
                myServo.write(cpos)
        elif prevpos > cpos and cpos > 0:
                # We're moving clockwise toward 20.
                prevpos = cpos
                cpos = cpos - 30
                myServo.write(cpos);

        time.sleep(0.09)



GPIO.setmode(GPIO.BOARD)
GPIO.setup(ON_SWITCH, GPIO.IN)
GPIO.setup(AUTOPILOT_SWITCH, GPIO.IN)
GPIO.setup(LEFT_LED, GPIO.OUT)
GPIO.setup(RIGHT_LED, GPIO.OUT)

Serial.begin(9600)
mySerial.begin(19200)

initServo()


while True:
        if GPIO.digitalRead(AUTOPILOT_SWITCH) == GPIO.HIGH:
                print("Auto pilot engaged!")
                myServo.attach(SERVO_PIN)

                print("Looking for obstacles")
                moveDatServo()
#       avoidCollision();
#       go();
#     } else {
#       // Manual
#       readAndGo();
#     }
#     myservo.detach();
#   } else {
#     initServo();
#     noGOGO();
#     while(digitalRead(onSwitch) != HIGH){}
#   }

