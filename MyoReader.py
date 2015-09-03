__author__ = 'James Boggs'
__version__ = 'alpha 0.03'

# Just reads Myo input and writes it to a serial port for an Arduino
# to read. A stand-in for reading a Myo directly from the Arduino,
# which will come later.

import serial
import myo as libmyo
libmyo.init('C:\\Users\\James\\Documents\\Coding\\MyoArm\\myo-sdk-win-0.9.0\\bin')
from time import sleep

HUB_INTERVAL_MS = 1000
SERIAL_OUT_PORT = 2 #COM3


if __name__ == '__main__':
    # set up myo listener
    hub = libmyo.Hub()
    feed = libmyo.device_listener.Feed()
    hub.run(HUB_INTERVAL_MS, feed)

    # set up serial communications
    ser = serial.Serial(SERIAL_OUT_PORT)

    # begin Myo loop
    try:
        myo = feed.wait_for_single_device(timeout=10)
        if not myo:
            print("No Myo connected after 10 seconds.")
            raise
        print("Hello, Myo User!")
        print("Myo is facing ", myo.x_direction)
        while hub.running and myo.connected:
            # set orientation data
            orient_x = myo.orientation.x
            orient_y = myo.orientation.y
            orient_z = myo.orientation.z
            orient_w = myo.orientation.w
            # set acceleration data
            accel_x = myo.acceleration.x
            accel_y = myo.acceleration.y
            accel_z = myo.acceleration.z
            # set gyroscopic data
            gyro_x = myo.gyroscope.x
            gyro_y = myo.gyroscope.y
            gyro_z = myo.gyroscope.z

            #write myo data to serial
            ser.write([orient_x, orient_y, orient_z, orient_w,
                       accel_x, accel_y, accel_z,
                       gyro_x, gyro_y, gyro_z])
        print("Goodbye, Myo!")
    finally:
        hub.shutdown()