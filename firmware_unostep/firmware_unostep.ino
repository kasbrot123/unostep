// firmware to control stepper motors with arduino uno + cnc shield V3 
// Authors: Lukas S., Michael T.
// Date: Oct. 2024


/*
get position of motor XYZ
    PX, PY, PZ
    return OK1000
set position of motor XYZ to zero
    ZX, ZY, ZZ
    return OK
move motor XYZ for N steps
    MXS1000, MYS-1000
    return OK
move motor XYZ to position R
    MXP1000, MYP-1000
    return OK
set speed of motor to V
    MXV100, MYV10
    return OK

*/



// pins of stepper motors
const int stepMX = 2;
const int stepMY = 3;
const int stepMZ = 4;

const int dirMX  = 5;
const int dirMY  = 6;
const int dirMZ  = 7;

const int enPin = 8;



long DELAY_MIN = 10; // Speed (max. 7 microseconds)

// initial position of motors
long positionMX = 0;
long positionMY = 0;
long positionMZ = 0;

// delay time controls speed of motors
long delay_X = 50;
long delay_Y = 50;
long delay_Z = 50;


// A and F are error codes
const String OK = "A";
const String FAIL = "F enter ?";


void setup() {
    pinMode(stepMX, OUTPUT);
    pinMode(dirMX, OUTPUT);

    pinMode(stepMY, OUTPUT);
    pinMode(dirMY, OUTPUT);

    pinMode(stepMZ, OUTPUT);
    pinMode(dirMZ, OUTPUT);

    pinMode(enPin, OUTPUT);
    digitalWrite(enPin, HIGH); // Power Off

    Serial.begin(115200);
}




void moveStepper(int step_pin, int dir_pin, long steps, long step_delay) {
    /*
    there is also a library <Accelstepper.h> with alot of useful features but
    it is very slow in comparison to the hard coded method. So we decided to
    use this approach.
    */
    digitalWrite(dir_pin, steps > 0 ? LOW : HIGH);

    digitalWrite(enPin, LOW);
    for (long x = 0; x < abs(steps); x++) {
        digitalWrite(step_pin, HIGH);
        delayMicroseconds(step_delay);
        digitalWrite(step_pin, LOW);
        delayMicroseconds(step_delay);
    }
    digitalWrite(enPin, HIGH);
}

void returnNumber(long number) {
    Serial.print(OK);
    Serial.println(number);
}


void loop() {

    if (Serial.available()) {
        String command = Serial.readString();
        command.trim();

        // help command
        if (command.startsWith("?")) {
            Serial.println("AValid first characters are: Read, Zero, Position, Steps, Delay followed by XYZ and an optional NUMBER.");
            return;
        }

        // get position (read)
        if (command.startsWith("R")) {
            if (command.substring(1,2) == "X") {
                returnNumber(positionMX);
                return;
                // position = stepperX.currentPosition();
            }
            if (command.substring(1,2) == "Y") {
                returnNumber(positionMY);
                return;
                // position = stepperY.currentPosition();
            }
            if (command.substring(1,2) == "Z") {
                returnNumber(positionMZ);
                return;
                // position = stepperZ.currentPosition();
            }
        }


        // set position to zero (or value)
        if (command.startsWith("Z")) {
            long position_set = command.substring(2).toInt(); // from index 3 to end
            if (command.substring(1,2) == "X") {
                positionMX = position_set;
                Serial.println(OK);
                return;
            }
            if (command.substring(1,2) == "Y") {
                positionMY = position_set;
                Serial.println(OK);
                return;
            }
            if (command.substring(1,2) == "Z") {
                positionMZ = position_set;
                Serial.println(OK);
                return;
            }
        }


        // move steps
        if (command.startsWith("S")) {
            long steps = command.substring(2).toInt();
            if (command.substring(1,2) == "X") {
                positionMX = positionMX + steps;
                moveStepper(stepMX, dirMX, steps, delay_X);
                Serial.println(OK);
                return;
            }
            if (command.substring(1,2) == "Y") {
                positionMY = positionMY + steps;
                moveStepper(stepMY, dirMY, steps, delay_Y);
                Serial.println(OK);
                return;
            }
            if (command.substring(1,2) == "Z") {
                positionMZ = positionMZ + steps;
                moveStepper(stepMZ, dirMZ, steps, delay_Z);
                Serial.println(OK);
                return;
            }
        }

        // move to position
        if (command.startsWith("P")) {
            long position_set = command.substring(2).toInt();
            if (command.substring(1,2) == "X") {
                long steps = position_set - positionMX;
                moveStepper(stepMX, dirMX, steps, delay_X);
                positionMX = position_set;
                Serial.println(OK);
                return;
            }
            if (command.substring(1,2) == "Y") {
                long steps = position_set - positionMY;
                moveStepper(stepMY, dirMY, steps, delay_Y);
                positionMY = position_set;
                Serial.println(OK);
                return;
            }
            if (command.substring(1,2) == "Z") {
                long steps = position_set - positionMZ;
                moveStepper(stepMZ, dirMZ, steps, delay_Z);
                positionMZ = position_set;
                Serial.println(OK);
                return;
            }
        }

        // set delay time of stepper
        if (command.startsWith("D")) {
            long number = command.substring(2).toInt();
            if (number > DELAY_MIN){
                if (command.substring(1,2) == "X") {
                    delay_X = number;
                    Serial.println(OK);
                    return;
                }
                if (command.substring(1,2) == "Y") {
                    delay_Y = number;
                    Serial.println(OK);
                    return;
                }
                if (command.substring(1,2) == "Z") {
                    delay_Z = number;
                    Serial.println(OK);
                    return;
                }
            }
        }
        Serial.println(FAIL);
    }
}


        // // Position
        // if (command.startsWith("P1")) {
        //     //Serial.print("Position M1: ");
        //     Serial.println(positionM1);
        //     return;
        // } else if (command.startsWith("P2")) {
        //     //Serial.print("Position M2: ");
        //     Serial.println(positionM2);
        //     return;
        // } else if (command.startsWith("P3")) {
        //     //Serial.print("Position M3: ");
        //     Serial.println(positionM3);
        //     return;
        // }
        //
        // // Set position to zero
        // if (command.startsWith("Z1")) {
        //     Serial.println("ok");
        //     // Serial.print("Position M1 set to zero.");
        //     positionM1 = 0;
        //     return;
        // } else if (command.startsWith("Z2")) {
        //     Serial.println("ok");
        //     // Serial.print("Position M2 set to zero.");
        //     positionM2 = 0;
        //     return;
        // } else if (command.startsWith("Z3")) {
        //     Serial.println("ok");
        //     // Serial.print("Position M3 to zero.");
        //     positionM3 = 0;
        //     return;
        // }

        // int motor_number = 0;
        // bool clockwise = true;
        // long steps = 0;
        //
        // // Choose Motor
        // if (command.startsWith("M1")) {
        //     motor_number = 1;
        // } 
        // else if (command.startsWith("M2")) {
        //     motor_number = 2;
        // }
        // else if (command.startsWith("M3")) {
        //     motor_number = 3;
        // } 
        // else {
        //     Serial.println("Invalid motor number");
        //     // Serial.print(command.substring(0,3));
        //     return;
        // }
        //
        // // Return to position 0 
        // if (command.endsWith("P0")) {
        //     if (motor_number == 1) {
        //         steps = positionM1;
        //         clockwise = (steps < 0);
        //         steps = abs(steps);
        //     } else if (motor_number == 2) {
        //         steps = positionM2;
        //         clockwise = (steps < 0);
        //         steps = abs(steps);
        //     } else if (motor_number == 3) {
        //         steps = positionM3;
        //         clockwise = (steps < 0);
        //         steps = abs(steps);
        //     }
        // } 
        // 
        // else if (command.substring(2,3) == "P") {
        //         if (motor_number == 1) {
        //         positionM1goal = command.substring(3).toInt();
        //         steps = positionM1goal - positionM1;
        //         clockwise = (steps >= 0);
        //         steps = abs(steps);
        //         } else if (motor_number == 2) {
        //         positionM2goal = command.substring(3).toInt();
        //         steps = positionM2goal - positionM2;
        //         clockwise = (steps >= 0);
        //         steps = abs(steps);
        //       } else if (motor_number == 3) {
        //         positionM3goal = command.substring(3).toInt();
        //         steps = positionM3goal - positionM3;
        //         clockwise = (steps >= 0);
        //         steps = abs(steps);
        //       }
        //       }
        //
        // else if (command.substring(2, 3) == "-") {
        //       clockwise = false;
        //       steps = command.substring(3).toInt();
        //       } else {
        //       clockwise = true;
        //       steps = command.substring(2).toInt();
        //       } 
        //       
        //
        //     // Position update
        //     if (motor_number == 1) {
        //         if (clockwise) {
        //             positionM1 += steps;
        //         } else {
        //             positionM1 -= steps;
        //         }
        //     } else if (motor_number == 2) {
        //         if (clockwise) {
        //             positionM2 += steps;
        //         } else {
        //             positionM2 -= steps;
        //         }
        //     } else if (motor_number == 3) {
        //         if (clockwise) {
        //             positionM3 += steps;
        //         } else {
        //             positionM3 -= steps;
        //         }
        //     }
        // 
        //
        // digitalWrite(enPin, LOW); // Power On
        //
        // // Debugging
        // // Serial.print("Motor: ");
        // // Serial.print(motor_number);
        // // Serial.print(", Direction: ");
        // // Serial.print(clockwise ? "Clockwise" : "Counterclockwise");
        // // Serial.print(", Steps: ");
        // // Serial.println(steps);
        //
        //
        // // Control Motor
        // if (motor_number == 1) {
        //     digitalWrite(dirM1, clockwise ? LOW : HIGH);
        //     for (long x = 0; x < steps; x++) {
        //         digitalWrite(stepM1, HIGH);
        //         delayMicroseconds(stepDelay);
        //         digitalWrite(stepM1, LOW);
        //         delayMicroseconds(stepDelay);
        //     }
        // } 
        // else if (motor_number == 2) {
        //     digitalWrite(dirM2, clockwise ? LOW : HIGH);
        //     for (long x = 0; x < steps; x++) {
        //         digitalWrite(stepM2, HIGH);
        //         delayMicroseconds(stepDelay);
        //         digitalWrite(stepM2, LOW);
        //         delayMicroseconds(stepDelay);
        //     }
        // }
        // else if (motor_number == 3) {
        //     digitalWrite(dirM3, clockwise ? LOW : HIGH);
        //     for (long x = 0; x < steps; x++) {
        //         digitalWrite(stepM3, HIGH);
        //         delayMicroseconds(stepDelay);
        //         digitalWrite(stepM3, LOW);
        //         delayMicroseconds(stepDelay);
        //     }
        // }
        //
        // Serial.println("ok");
        //
        // digitalWrite(enPin, HIGH); // Power Off
