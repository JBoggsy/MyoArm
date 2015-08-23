__author__ = 'James Boggs'
__version__ = 'alpha 0.01'

import serial
from statistics import median
import myo as libmyo
libmyo.init('C:\\Users\\James\\Documents\\Coding\\MyoArm\\myo-sdk-win-0.9.0\\bin')
from time import sleep, time

LEFT = 0
RIGHT = 1
CALIBRATE_TIME = 5

HUB_INTERVAL_MS = 1000
SERIAL_PORT = 2 #COM3

def mapValue(fromMin, fromMax, toMin, toMax, value):
    if value < fromMin:
        return toMin
    elif value > fromMax:
        return toMax

    fromRange = (fromMax - fromMin)
    toRange = (toMax - toMin)

    normVal = value - fromMin
    valPercent = float(normVal)/fromRange

    newValRaw = valPercent * toRange
    newValue = newValRaw + toMin

    return newValue


def mkrdbl(num):
    return round(num*100, 1)


def getMedianReading(side, timeSecs):
    readings = []
    print("Please move your fist to your {}!".format("right" if side else "left"))
    sleep(4)
    print("Beginning reading!")
    init_time = time()
    while hub.running and myo.connected and (time()-init_time) < timeSecs:
        horiz_pos = mkrdbl(myo.orientation.z)
        print("Horizontal Position:", horiz_pos)
        readings.append(horiz_pos)
        sleep(0.01)
    print("Median {} reading: ".format("right" if side else "left"),  median(readings))
    return median(readings)


if __name__ == '__main__':
    # set up myo listener
    hub = libmyo.Hub()
    feed = libmyo.device_listener.Feed()
    hub.run(HUB_INTERVAL_MS, feed)

    # set up serial communications
    ser = serial.Serial(SERIAL_PORT)

    # begin Myo loop
    try:
        myo = feed.wait_for_single_device(timeout=10)
        if not myo:
            print("No Myo connected after 10 seconds.")
            raise
        print("Hello, Myo User!")
        print("Myo is facing ", myo.x_direction)
        left_reading = getMedianReading(LEFT, CALIBRATE_TIME)
        right_reading = getMedianReading(RIGHT, CALIBRATE_TIME)
        print("Left reading: {} | Right reading: {}".format(left_reading, right_reading))
        old_horiz_pos = 0
        while hub.running and myo.connected:
            raw_horiz_pos = mkrdbl(myo.orientation.z)
            horiz_pos = int(mapValue(right_reading, left_reading, 20, 160, raw_horiz_pos))
            if horiz_pos != old_horiz_pos:
                print("Moving servo to {}".format(horiz_pos))
                ser.write([horiz_pos])
                old_horiz_pos = horiz_pos
            sleep(0.01)
        print("Goodbye, Myo!")
    finally:
        hub.shutdown()