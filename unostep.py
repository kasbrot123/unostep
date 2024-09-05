import time
import serial
from serial.tools import list_ports

"""
class


"""


def serial_is_connected(serial):
    if serial is None:
        return False

    is_connected = True
    # problem was:
    # - it interferes with arduino firmware
    # - it writes every time when sending a command
    # - it has to wait every time 
    # -> no good
    # try:
    #     serial.write('?'.encode())
    #     serial.flush()
    # except:
    #     is_connected = False
        # send ping and see if something is comming back
        # or check serial command
        # TO BE IMPLEMENTED
    return is_connected



class Motor:

    def __init__(self, name):
        self.name = name
        self.connection = None

    def setSerial(self, Serial):
        self.connection = Serial


    def isConnected(self):
        return serial_is_connected(self.connection)


    def command(self, Command):
        if not self.isConnected():
            return False

        # answer = self.connection.readlines()
        # print('first answer', answer)

        # self.connection.flush()
        self.connection.write(Command.encode())
        # answer = self.connection.readline()
        answer = self.connection.read_until('\n'.encode()).decode()[:-2]
        print('second answer', answer)

        if answer != 'ok':
            return False

        return True


    def steps(self, Steps):

        Command = 'M'+self.name+str(int(Steps)) + '\n'
        return self.command(Command)

    def position(self, Position):

        Command = 'M' + self.name+ 'P' + str(int(Position)) + '\n'
        return self.command(Command)



class Hydra:

    def __init__(self):
        self.name_com_port = ''
        self.connection = None
        self.X = Motor('1')
        self.Y = Motor('2')
        self.Z = Motor('3')
        # self.A = Motor('A')

    def connect(self, name_port = None):

        if name_port is None:
            avail_com_ports = list_ports.comports()
            n_comports = len(avail_com_ports)
            if n_comports == 1:
                self.name_com_port = avail_com_ports[0].device
            elif n_comports > 1:
                print('There is more than one COM port.')
                for i in avail_com_ports:
                    print(i)
                return False
            else:
                print("No available COM port")
                return False
        else:
            self.name_com_port = name_port


        # irgendwie fehler abfangen 
        self.connection = serial.Serial(
            port='COM3',
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=10
        )

        self.X.setSerial(self.connection)
        self.Y.setSerial(self.connection)
        self.Z.setSerial(self.connection)

        # every one needs a pause from time to time
        time.sleep(2)
        # i dont understand why but the arduino does not catch the first command
        # when there is no pause

        return True
    
    def disconnect(self):
        self.connection.close()

        return True

    def isConnected(self):
        return serial_is_connected(self.connection)




if __name__ == '__main__':
    STEP = 10000

    shield = Hydra()
    shield.connect()
    print('start turning')
    shield.Y.steps(STEP)
    shield.Z.steps(STEP)
    shield.Y.steps(-STEP)
    shield.disconnect()


