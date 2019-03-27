import serial
import sys
from time import sleep


class Uart:
    def __init__(self):
        self._ser = serial.Serial()
        self._ser.port = 'COM4'  # TODO set the correct com port!
        self._ser.baudrate = 9600
        self._ser.timeout = 1
        try:
            self._ser.open()
        except:
            print("Error while opening " + str(e))
            sys.exit()

    def readData(self): # returns received data as string
        return self._ser.readline().decode("utf-8")

    def writeData(self, data): # sends string data
        to_send = bytes(data)
        self._ser.write(to_send)

    def close(self):
        self._ser.close()


def main():
    ser = Uart()
    print("Started")
    while True:
        print("Temp: " + ser.readData())
        text =  raw_input("Please enter something: ")
        ser.writeData(text); 

if __name__ == "__main__":
    main()