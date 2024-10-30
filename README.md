# UnoStep


Arduino programm to control stepper motors on CNC Shield V3



# Arduino Firmware

**TO FLASH THE MODIFIED ARDUINO PRESS THE RESET BUTTON WHEN UPLOADING THE
SKETCH. TRY A FEW TIMES.**

**VERY IMPORTANT NOTE**

We modified the Arduino such that the capacitor near the `RESET ON` label is
not connected. This prevents an auto reset by the serial connection when
disconnect and reconnect. The Arduino Software needs this auto reset to flash
the firmware without any button press. To flash with the modified setup press
the Reset button after hitting the uploading button from the Arduino Software.
It could take a few attepts to hit at the right time.

## Command Set

A serial command for the arduino is based on tree main parts. The first char is
the command char. The second char is the motor specifier and the last part of
the command is an optional number. 

```
SX1000

S ... Step command
X ... specifies the motor X
1000 ... the number of steps for the motor to turn

```
### Commands

- R: Read the position
- S: move number of steps
- Z: set the motor position to zero (or an given number)
- P: move the motor to a given position
- D: sets a delay time in ms to control the motor speed (min. delay is 10 ms)


Examples:

- RX: Read the position of motor X
- SY-1000: Move the motor Y 1000 backwards
- ZZ: sets the position of motor Z to 0
- PY100: move motor Y to position 100
- DX100: sets the delay time for motor X to 100 ms


### Error codes

There are two error codes `A` (everything ok) and `F` (command pattern failed). 


## Issues with the libary `Accelstepper.h`

The libary `Accelstepper.h` has a rich command set for the stepper motors but
it lacks in terms of speed. The Arduino Uno is simply to slow with a clock
speed of 16 MHz and therefore a raw implementation with `digitalWrite` was
chosen.



# Python module

To control the Arduino via python there is a class implemented which sets up a
serial connection and constructs motor classes with basic functionalities. 


## Main functions


```python
# motor class with basic functionalities
class Motor:
    # initializes the motor class
    def __init__(self, name):

    # takes the serial connection from the top class
    def setSerial(self, Serial):

    # sends the raw command via serial conenction
    def command(self, Command):

    # moves 'Steps' steps
    def steps(self, Steps):

    # moves to 'Position'
    def position(self, Position):

    # sets the position of the motor to 'number'
    def setZero(self, number=0):

    # returns the position of the motor
    def getPosition(self):


# main class with all motor classes and the serial connection
class Hydra
    # constructor 
    def __init__(self):

    # connect to serial connection
    def connect(self, name_port = None):

    # disconnect serial connection
    def disconnect(self):
```


## Example 

```python
shield = Hydra()
shield.connect()
print('start turning')
print('Motor Y')
shield.Y.steps(1000)
print('Motor Z')
shield.Z.steps(1000)
print('Motor X')
shield.X.steps(-1000)

motorX = shield.X
pos = motorX.getPosition()
print(pos)

shield.disconnect()
```



