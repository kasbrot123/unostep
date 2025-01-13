import time
import serial
from serial.tools import list_ports
import keyboard

"""
classes:
    - Motor
        one single motor with all motor commands
    - Hydra
        initializes the serial connection and the motors

"""



class Motor:

    def __init__(self, name):
        self.name = name
        self.connection = None
        self.error_list = []

    def setSerial(self, Serial):
        self.connection = Serial


    def command(self, Command):
        # if not self.isConnected():
        #     return False

        # answer = self.connection.readlines()
        # print('first answer', answer)

        # self.connection.flush()
        self.connection.write(Command.encode())
        # answer = self.connection.readline()
        answer = self.connection.read_until('\n'.encode()).decode()[:-2]
        # print('second answer', answer)

        # flag for everything ok
        if answer[0] == 'A':
            return True, answer[1:]

        print('Error with command:', Command)
        return False, ''


    def steps(self, Steps):
        Command = 'S' + self.name + str(int(Steps)) + '\n'
        return self.command(Command)[0]

    def position(self, Position):
        Command = 'P' + self.name + str(int(Position)) + '\n'
        return self.command(Command)[0]

    def setZero(self, number=0):
        Command = 'Z' + self.name + str(int(number)) + '\n'
        return self.command(Command)[0]

    def setDelay(self, number):
        Command = 'D' + self.name + str(int(number)) + '\n'
        return self.command(Command)[0]

    def getPosition(self):
        Command = 'R' + self.name + '\n'
        return float(self.command(Command)[1])

    def interactive(self,step=1):

        HALT_TIME = 0.1
        STEP = 160
        WAITING = 50
        print('Interactive mode.\nMove with arrow keys, s for step number, quit with q.')
        print('Step size now: {}. 6400 for one full rotation'.format(STEP))
        print('Waiting time t is {} ms'.format(WAITING))
        print('move ',end='')
        while True:
            if keyboard.is_pressed('left arrow'):
                print('move left    ')
                self.steps(STEP)
                time.sleep(HALT_TIME)
            if keyboard.is_pressed('right arrow'):
                print('move right   ')
                self.steps(-STEP)
                time.sleep(HALT_TIME)
            if keyboard.is_pressed('up arrow'):
                print('move up      ')
                self.steps(-STEP)
                time.sleep(HALT_TIME)
            if keyboard.is_pressed('down arrow'):
                print('move down    ')
                self.steps(STEP)
                time.sleep(HALT_TIME)
            if keyboard.is_pressed('s'):
                STEP = int(input('steps:'))
                print('step is now {}, equals {:0.1f}°'.format(STEP, STEP/6400*360))
                time.sleep(HALT_TIME)
            if keyboard.is_pressed('w'):
                WAITING = int(input('waiting time t in ms (t>10ms):'))
                print('waiting time t is now: {} ms'.format(WAITING))
                self.setDelay(WAITING)
                time.sleep(HALT_TIME)

            if keyboard.is_pressed('q'):
                print('quit interactive mode.')
                break



class Hydra:

    def __init__(self):
        self.name_com_port = ''
        self.connection = None
        self.X = Motor('X')
        self.Y = Motor('Y')
        self.Z = Motor('Z')
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
                    print(i.device) # prints 'COM3' etc.
                # return False
                raise Exception('More than one COM port.\nSpecify the port or unplug devices.')
            else:
                print("No available COM port")
                raise Exception('No available COM port.')
                return False
        else:
            self.name_com_port = name_port


        self.connection = serial.Serial(
            port=self.name_com_port,
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS
            # timeout=10
            # no timeout since sometimes it takes more than 10 seconds to move
            # and debugging is bad with more than 10 seconds
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






if __name__ == '__main__':
    STEP = 10000

    shield = Hydra()
    shield.connect()
    print('start turning')
    print('Motor Y')
    shield.Y.steps(STEP)
    print('Motor Z')
    shield.Z.steps(STEP)
    print('Motor X')
    shield.X.steps(-STEP)

    motorX = shield.X
    pos = motorX.getPosition()
    print(pos)

    shield.disconnect()


