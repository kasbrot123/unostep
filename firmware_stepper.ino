// firmware to control stepper motors with arduino uno + cnc shield V3 
// Authors: Lukas S., Michael T.
// Date: Sep. 2024

const int stepM1 = 2;
const int dirM1  = 5;

const int stepM2 = 3;
const int dirM2  = 6;

const int stepM3 = 4;
const int dirM3  = 7;

const int enPin = 8;

int stepDelay = 50; // Speed (max. 7 microseconds)

long positionM1 = 0;
long positionM1goal = 0;
long positionM2 = 0;
long positionM2goal = 0;
long positionM3 = 0;
long positionM3goal = 0;

const long stepsPerRevolution = 6400;

void setup() {
    pinMode(stepM1, OUTPUT);
    pinMode(dirM1, OUTPUT);

    pinMode(stepM2, OUTPUT);
    pinMode(dirM2, OUTPUT);

    pinMode(stepM3, OUTPUT);
    pinMode(dirM3, OUTPUT);

    pinMode(enPin, OUTPUT);

    // digitalWrite(enPin, LOW); // Power On
    digitalWrite(enPin, HIGH); // Power Off

    Serial.begin(115200);
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readString();
        command.trim();

        // Position
        if (command.startsWith("P1")) {
            //Serial.print("Position M1: ");
            Serial.println(positionM1);
            return;
        } else if (command.startsWith("P2")) {
            //Serial.print("Position M2: ");
            Serial.println(positionM2);
            return;
        } else if (command.startsWith("P3")) {
            //Serial.print("Position M3: ");
            Serial.println(positionM3);
            return;
        }

        // Set position to zero
        if (command.startsWith("Z1")) {
            Serial.println("ok");
            // Serial.print("Position M1 set to zero.");
            positionM1 = 0;
            return;
        } else if (command.startsWith("Z2")) {
            Serial.println("ok");
            // Serial.print("Position M2 set to zero.");
            positionM2 = 0;
            return;
        } else if (command.startsWith("Z3")) {
            Serial.println("ok");
            // Serial.print("Position M3 to zero.");
            positionM3 = 0;
            return;
        }

        int motor_number = 0;
        bool clockwise = true;
        long steps = 0;

        // Choose Motor
        if (command.startsWith("M1")) {
            motor_number = 1;
        } 
        else if (command.startsWith("M2")) {
            motor_number = 2;
        }
        else if (command.startsWith("M3")) {
            motor_number = 3;
        } 
        else {
            Serial.println("Invalid motor number");
            // Serial.print(command.substring(0,3));
            return;
        }

        // Return to position 0 
        if (command.endsWith("P0")) {
            if (motor_number == 1) {
                steps = positionM1;
                clockwise = (steps < 0);
                steps = abs(steps);
            } else if (motor_number == 2) {
                steps = positionM2;
                clockwise = (steps < 0);
                steps = abs(steps);
            } else if (motor_number == 3) {
                steps = positionM3;
                clockwise = (steps < 0);
                steps = abs(steps);
            }
        } 
        
        else if (command.substring(2,3) == "P") {
                if (motor_number == 1) {
                positionM1goal = command.substring(3).toInt();
                steps = positionM1goal - positionM1;
                clockwise = (steps >= 0);
                steps = abs(steps);
                } else if (motor_number == 2) {
                positionM2goal = command.substring(3).toInt();
                steps = positionM2goal - positionM2;
                clockwise = (steps >= 0);
                steps = abs(steps);
              } else if (motor_number == 3) {
                positionM3goal = command.substring(3).toInt();
                steps = positionM3goal - positionM3;
                clockwise = (steps >= 0);
                steps = abs(steps);
              }
              }

        else if (command.substring(2, 3) == "-") {
              clockwise = false;
              steps = command.substring(3).toInt();
              } else {
              clockwise = true;
              steps = command.substring(2).toInt();
              } 
              

            // Position update
            if (motor_number == 1) {
                if (clockwise) {
                    positionM1 += steps;
                } else {
                    positionM1 -= steps;
                }
            } else if (motor_number == 2) {
                if (clockwise) {
                    positionM2 += steps;
                } else {
                    positionM2 -= steps;
                }
            } else if (motor_number == 3) {
                if (clockwise) {
                    positionM3 += steps;
                } else {
                    positionM3 -= steps;
                }
            }
        

        digitalWrite(enPin, LOW); // Power On

        // Debugging
        // Serial.print("Motor: ");
        // Serial.print(motor_number);
        // Serial.print(", Direction: ");
        // Serial.print(clockwise ? "Clockwise" : "Counterclockwise");
        // Serial.print(", Steps: ");
        // Serial.println(steps);


        // Control Motor
        if (motor_number == 1) {
            digitalWrite(dirM1, clockwise ? LOW : HIGH);
            for (long x = 0; x < steps; x++) {
                digitalWrite(stepM1, HIGH);
                delayMicroseconds(stepDelay);
                digitalWrite(stepM1, LOW);
                delayMicroseconds(stepDelay);
            }
        } 
        else if (motor_number == 2) {
            digitalWrite(dirM2, clockwise ? LOW : HIGH);
            for (long x = 0; x < steps; x++) {
                digitalWrite(stepM2, HIGH);
                delayMicroseconds(stepDelay);
                digitalWrite(stepM2, LOW);
                delayMicroseconds(stepDelay);
            }
        }
        else if (motor_number == 3) {
            digitalWrite(dirM3, clockwise ? LOW : HIGH);
            for (long x = 0; x < steps; x++) {
                digitalWrite(stepM3, HIGH);
                delayMicroseconds(stepDelay);
                digitalWrite(stepM3, LOW);
                delayMicroseconds(stepDelay);
            }
        }

        Serial.println("ok");

        digitalWrite(enPin, HIGH); // Power Off
    }
    }
