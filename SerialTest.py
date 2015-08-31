from time import sleep
import serial

while True:
    ser = serial.Serial(2)
    ser.write([1])
    ser.close()
    sleep(1)