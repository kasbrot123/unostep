
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


// firmware to control stepper motors with arduino uno + cnc shield V3 
// Authors: Lukas S., Michael T.
// Date: Sep. 2024


#include <AccelStepper.h>

const int stepMX = 2;
const int stepMY = 3;
const int stepMZ = 4;

const int dirMX  = 5;
const int dirMY  = 6;
const int dirMZ  = 7;

//const int enPin = 8;
uint8_t enPin = 8;


// (Type:driver, STEP, DIR)
AccelStepper stepperX(1, stepMX, dirMX);
AccelStepper stepperY(1, stepMY, dirMY);
AccelStepper stepperZ(1, stepMZ, dirMZ);



int stepDelay = 50; // Speed (max. 7 microseconds)
long number;
long position_set;

long positionMX = 0;
long positionMY = 0;
long positionMZ = 0;

long positionM1goal = 0;
long positionM2goal = 0;
long positionM3goal = 0;

const long stepsPerRevolution = 6400;
const String OK = "ok";


int MAX_SPEED = 4000.0; // steps per second
int ACCELERATION = 20000;


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

  // bool invert = true;
  stepperX.setMaxSpeed(MAX_SPEED);
  stepperX.setAcceleration(ACCELERATION);
  stepperX.setSpeed(4000);
  //stepperX.setMinPulseWidth(55);
  // stepperX.setEnablePin(enPin);
  // stepperX.setPinsInverted(false, true, invert);

  stepperY.setMaxSpeed(MAX_SPEED);
  stepperY.setAcceleration(ACCELERATION);
  // stepperY.setEnablePin(enPin);
  // stepperY.setPinsInverted(false, true, invert);

  stepperZ.setMaxSpeed(MAX_SPEED);
  stepperZ.setAcceleration(ACCELERATION);
  // stepperZ.setEnablePin(enPin);
  // stepperZ.setPinsInverted(false, true, invert);

}




void loop() {

  // digitalWrite(enPin, LOW);
  // stepperX.move(640);
  // while (stepperX.run()) {
  //
  // }
  // digitalWrite(enPin, HIGH);
  // int position = stepperX.currentPosition();
  // Serial.println(position);
  //
  // // Serial.println("move forward");
  // delay(2000);

  //digitalWrite(enPin, LOW);
  //stepperX.move(-10000);
  //while (stepperX.run()) {

  //}
  //digitalWrite(enPin, HIGH);

  //Serial.println("move back");
  //delay(2000);


    if (Serial.available()) {
        String command = Serial.readString();
        command.trim();


        // get position
        if (command.startsWith("R")) {
            if (command.substring(1,2) == "X") {
                position = stepperX.currentPosition();
            }
            if (command.substring(1,2) == "Y") {
                position = stepperY.currentPosition();
            }
            if (command.substring(1,2) == "Z") {
                position = stepperZ.currentPosition();
            }
            Serial.println(position);
            return;
        }


        // set position to zero
        if (command.startsWith("Z")) {
            number = command.substring(2).toInt(); // from index 3 to end
            if (command.substring(1,2) == "X") {
                stepperX.setCurrentPosition(number);
                Serial.println(OK);
                return;
            }
            if (command.substring(1,2) == "Y") {
                stepperY.setCurrentPosition(number);
                Serial.println(OK);
                return;
            }
            if (command.substring(1,2) == "Z") {
                stepperZ.setCurrentPosition(number);
                Serial.println(OK);
                return;
            }
        }


        // move steps
        if (command.startsWith("S")) {
            number = command.substring(2).toInt();
            if (command.substring(1,2) == "X") {

                // moveStepper(stepMX, dirMX, number)
                digitalWrite(enPin, LOW);
                int stepDelay = 200;
                for (long x = 0; x < number; x++) {
                  digitalWrite(stepMX, HIGH);
                  delayMicroseconds(stepDelay);
                  digitalWrite(stepMX, LOW);
                  delayMicroseconds(stepDelay);
                }
                // stepperX.move(number);
                // while (stepperX.run()) {
                    
                // }
                digitalWrite(enPin, HIGH);
            }
            if (command.substring(1,2) == "Y") {
                digitalWrite(enPin, LOW);
                stepperY.move(number);
                while (stepperY.run()) {
                    
                }
                digitalWrite(enPin, HIGH);
            }
            if (command.substring(1,2) == "Z") {
                digitalWrite(enPin, LOW);
                stepperZ.move(number);
                while (stepperZ.run()) {
                    
                }
                digitalWrite(enPin, HIGH);
            }
            Serial.println(OK);
        }

        // move to position
        if (command.startsWith("P")) {
            number = command.substring(2).toInt();
            if (command.substring(1,2) == "X") {
                digitalWrite(enPin, LOW);
                stepperX.moveTo(number);
                while (stepperX.run()) {
                    
                }
                digitalWrite(enPin, HIGH);
            }
            if (command.substring(1,2) == "Y") {
                digitalWrite(enPin, LOW);
                stepperY.moveTo(number);
                while (stepperY.run()) {
                    
                }
                digitalWrite(enPin, HIGH);
            }
            if (command.substring(1,2) == "Z") {
                digitalWrite(enPin, LOW);
                stepperZ.moveTo(number);
                while (stepperZ.run()) {
                    
                }
                digitalWrite(enPin, HIGH);
            }
            Serial.println(OK);
        }


        // set velocity of motor
        if (command.startsWith("V")) {
            number = command.substring(2).toInt();
            if (number > 0 && number < MAX_SPEED){
                if (command.substring(1,2) == "X") {
                    stepperX.setSpeed(number);
                    //Serial.println(positionMX);
                }
                if (command.substring(1,2) == "Y") {
                    stepperY.setSpeed(number);
                    //Serial.println(positionMY);
                }
                if (command.substring(1,2) == "Z") {
                    stepperZ.setSpeed(number);
                    //Serial.println(positionMZ);
                }
            }
            Serial.println(OK);
            return;
        }

        
        // Serial.println("NO VALID COMMAND");


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
    }
}
