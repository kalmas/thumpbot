import Servo
import time

SERVO_PIN = 18

Servo.attach(SERVO_PIN)
Servo.goToAndWait(0, 2)
Servo.goToAndWait(90, 2)
Servo.goToAndWait(180, 2)

Servo.detach()

