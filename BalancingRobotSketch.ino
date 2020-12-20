// ----------------------- PIN LOCATION ------------------------
int pwm1 = 6; // Enable pin EN2
int pwm2 = 11; // Enable pin EN1
int in_1 = 10;
int in_2 = 9;
int in_3 = A5;
int in_4 = 5;

// ---------------------- INITIALISING VARIABLES ------------------
int currentAngle;
int currentTime;
int balance;
int currentError, proportional, derivative;
int MOTOR;
int up, yaw, pitch, roll;
int timegap;

//  ------------------- DEFINING VARIABLES ----------------------
int targetAngle = 580; // Initially 512, but changed to account for weight distribution
int previousTime = 0;
int previosuError = 0;
int previousAngle = 0;
int integral = 0;

int Kp = 38;
int Ki = 1;
float Kd = 12;

void setup(){
    Serial.begin(9600);
    // -------------- SETTING PIN MODES -----------------
    pinMode(in_1,OUTPUT);
    pinMode(in_2,OUTPUT);
    pinMode(in_3,OUTPUT);
    pinMode(in_4,OUTPUT);
    pinMode(pwm1,OUTPUT);
    pinMode(pwm2,OUTPUT);

    // ---------------- INITIALISING STARTING CONDITIONS ----------------
    digitalWrite(in_1,0);
    digitalWrite(in_2,0);
    digitalWrite(in_3,0);
    digitalWrite(in_4,0);
} // end setup

void loop(){
    // -------------------- TIMER ----------------------------
    currentTime = millis();
    timegap = currentTime - previousTime;
    delay(20);

    // --------------------------- DATA INPUTS -------------------
    balance = digitalRead(A0); //UP PIN
    yaw = digitalRead(A1); // NOT USED
    roll = digitalRead(A2); // NOT USED
    pitch = digitalRead(A3); // NOT USED

    currentAngle = pitch;

    if (balance == false){ // If not within balancing range -> ROBOT IS OFF
        // Set everything to zero
        analogWrite(pwm1,0);
        analogWrite(pwm2,0);
        digitalWrite(in_1,0);
        digitalWrite(in_2,0);
        digitalWrite(in_3,0);
        digitalWrite(in_4,0);
    } // end if

    if (balance == true){ // If within balancing range -> ROBOT IS ON
        // -------------- PID CONTROL -----------------------
        currentError = currentAngle - targetAngle;
        proportional = currentError;
        integral = integral + (currentError * timegap);
        integral = constrain(integral, -100, 100);

        if (MOTOR < 5 && MOTOR > 90){
            MOTOR = map(MOTOR, 5, 90, 60, 120);
        }
        else if (MOTOR >90 && MOTOR < -5){
            MOTOR = map(MOTOR, -90, -5 , -120, -60);
        }

        // ---------- WRITING TO MOTORS -------------------
        if (currentError < -150){
            // FORWARDS (Glass side)
            digitalWrite(in_1, HIGH);
            digitalWrite(in_2, LOW);
            digitalWrite(in_3, LOW);
            digitalWrite(in_4, HIGH);
            MOTOR = abs(MOTOR);
        }
        else if (currentError > 150){
            // BACKWARDS (Wires)
            digitalWrite(in_1, LOW);
            digitalWrite(in_2, HIGH);
            digitalWrite(in_3, HIGH);
            digitalWrite(in_4, LOW);
            MOTOR = abs(MOTOR);
        }
        else if (currentError > -150 && currentError < -2){
            // FORWARDS (Glass side)
            digitalWrite(in_1, HIGH);
            digitalWrite(in_2, LOW);
            digitalWrite(in_3, LOW);
            digitalWrite(in_4, HIGH);
            MOTOR = abs(constrain(MOTOR, -180, 180));
        }
        else if (currentError < -150 && currentError > -2){
            // FORWARDS (Glass side)
            digitalWrite(in_1, LOW);
            digitalWrite(in_2, HIGH);
            digitalWrite(in_3, HIGH);
            digitalWrite(in_4, LOW);
            MOTOR = abs(constrain(MOTOR, -180, 180));
        }
        else {
            MOTOR = 0;
        } // end if

        analogWrite(pwm1, MOTOR);
        analogWrite(pwm1, MOTOR);
    } // end if
} // end loop
