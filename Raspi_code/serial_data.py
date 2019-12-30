import serial
import time
import os

try:
    arduino=serial.Serial('/dev/ttyUSB0',baudrate=9600,timeout=3.0)
except:
    arduino=serial.Serial('/dev/ttyUSB1',baudrate=9600,timeout=3.0)

starttime = time.time()
filename = time.strftime("%d%m%Y-%H%M%S")
file = "/data/" + filename + ".csv"
path = os.getcwd() + file;
f = open(path,"w+")
f.write(" aX ; aY ; aZ ; gX ; gY ; gZ ; angle \n")
f.close()
time.sleep(1)
f = open(path,"ab")

while True:
    if arduino.inWaiting() > 50:
        inputValue = arduino.read(arduino.inWaiting())
        print(inputValue)
        f.write(inputValue)
        if ((time.time() - starttime) > 10):
            f.close()
            time.sleep(0.1)
            f = open(path,"ab")
            starttime = time.time()
