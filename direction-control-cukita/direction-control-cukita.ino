//#############
//PIN DEFINITION
#define PIN_JOYSTICK_LEFT 2 //comando isquierda joisty
#define PIN_JOYSTICK_RIGHT 3 // comando derecha joisty
#define PIN_MOTOR_RELAY_1 4  // relay 1
#define PIN_HALL_WHEEL 5 //una vuelta de rueda 88cm 14 cm por pulso
#define PIN_HALL_HOMING 6 //homing hall sensor
#define PIN_MOTOR_RELAY_2 7  // relay 2
#define PIN_MOTOR_SWITCH 8  // reley motor on/off
#define PIN_AUTOMATIC_MODE A0 // button to activate response to line detection
#define PIN_ANGLE A4 // rotative encoder for wheel angle

//#############
//CONSTANTS DEFINITION
#define HOMING_STEPS 200
#define ANGLE_TOLERANCE 2 //in degrees
#define ROTATION_STEP_DELAY 40 //in ms
#define FALSE_ANGLE_TOLERANCE 50
#define ANGLE_CONTINUITY_LIMIT 20
#define WHEEL_ANGLE_ZERO 480 //calibration needed

uint32_t wheelAngle=0, oldAngle=0, newAngle=0;

void getWheelAngle(void){
  wheelAngle = analogRead(PIN_ANGLE);
  wheelAngle = ((wheelAngle-WHEEL_ANGLE_ZERO)/2); //have to define 2 has a constant
  wheelAngle *= -1; //inversing the angle for later
}

void stopTurning(void){
  digitalWrite(PIN_MOTOR_RELAY_1,LOW);
  digitalWrite(PIN_MOTOR_RELAY_2,LOW);
}

void turnRight(void){
  digitalWrite(PIN_MOTOR_RELAY_1,HIGH);
  digitalWrite(PIN_MOTOR_RELAY_2,LOW);
}

void turnLeft(void){
  digitalWrite(PIN_MOTOR_RELAY_1,LOW);
  digitalWrite(PIN_MOTOR_RELAY_2,HIGH);
}

void homingProcedure(){
    for (unsigned char i=0; i<HOMING_STEPS; i++){
      if(!digitalRead(PIN_HALL_HOMING))
        break;
      if (i<(HOMING_STEPS/2))
        turnRight();
      else
        turnLeft();
       delay(ROTATION_STEP_DELAY);
       stopTurning();
    }
    getWheelAngle();
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(PIN_JOYSTICK_LEFT),
                  joystickLeftInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_JOYSTICK_RIGHT),
                  joystickRightInterrupt, CHANGE);
  pinMode(PIN_MOTOR_RELAY_1,OUTPUT);
  pinMode(PIN_MOTOR_RELAY_2,OUTPUT);
  pinMode(PIN_MOTOR_SWITCH,OUTPUT);
  pinMode(PIN_AUTOMATIC_MODE,INPUT_PULLUP);
  pinMode(PIN_HALL_HOMING,INPUT_PULLUP);
  homingProcedure();
  oldAngle = wheelAngle;
}

void joystickLeftInterrupt(void){ //relay 1
  if (!digitalRead(PIN_AUTOMATIC_MODE))
    digitalWrite(PIN_MOTOR_SWITCH,HIGH);
  else{
    digitalWrite(PIN_MOTOR_SWITCH,LOW);
    if (digitalRead(PIN_JOYSTICK_LEFT))
      digitalWrite(PIN_MOTOR_RELAY_1,HIGH);
    else
      digitalWrite(PIN_MOTOR_RELAY_2,LOW);
    digitalWrite(PIN_MOTOR_RELAY_1,LOW);
  }
}

void joystickRightInterrupt(void){ //relay 2
  if (!digitalRead(PIN_AUTOMATIC_MODE)) //joystick
    digitalWrite(PIN_MOTOR_SWITCH,HIGH);
  else{
    digitalWrite(PIN_MOTOR_SWITCH,LOW);
    if (digitalRead(PIN_JOYSTICK_RIGHT))
      digitalWrite(PIN_MOTOR_RELAY_1,HIGH);
    else
      digitalWrite(PIN_MOTOR_RELAY_1,LOW);
    digitalWrite(PIN_MOTOR_RELAY_2,LOW);
  }
}

void goToAngle(int angle){
  if (abs(angle) > FALSE_ANGLE_TOLERANCE)
    return; // skip false angles
  if (abs(angle - oldAngle) >= ANGLE_CONTINUITY_LIMIT)
    return; // don't obey to not continuous information
  else
    oldAngle = angle;

    while(((angle - wheelAngle) <= -ANGLE_TOLERANCE)
        || ((angle - wheelAngle) >= ANGLE_TOLERANCE) ){
    getWheelAngle();
    if (angle - wheelAngle > 0)
      turnRight();
    else if (angle - wheelAngle < 0)
      turnLeft();
    delay(ROTATION_STEP_DELAY);
  }
  stopTurning();
}

void loop() {
//  if (!digitalRead(pinMA)){digitalWrite(pinM,HIGH);}else {digitalWrite(pinM,LOW);
//  if (!digitalRead(pinIS)){digitalWrite(pinB,HIGH);}else {digitalWrite(pinB,LOW);}// comando isquierda Joisty 20-08-18
//  if (!digitalRead(pinDE)){digitalWrite(pinA,HIGH);}else {digitalWrite(pinA,LOW);}// comando derecha Joisty
//  }
  String serial_direction;

  while (Serial.available()&&(!digitalRead(PIN_AUTOMATIC_MODE))) {
    char c = Serial.read();  //gets one byte from serial buffer
    serial_direction += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (serial_direction.length()>0){
    short angle = serial_direction.toInt();  //convert readString into a number
    goToAngle(angle);
  }
}
