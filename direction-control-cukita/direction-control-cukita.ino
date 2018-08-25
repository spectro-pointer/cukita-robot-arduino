#define pinIV  A5 //inversion motor direccion 
#define pinODOMETRO 2 // cuenta inpulsiones de la rueda /una vuelta de rueda 88cm 14 cm por pulso 
#define pinANGULO A4 // sensor de angulo de direccion de ruedas
#define pinIS A2 //comando isquierda joisty 
#define pinDE A3 // comando derecha joisty
#define pinH A1 //homing 
#define pinMA A0 // boton de marcha y arret manual  
#define pinM 8  // reley motor on/off
#define pinA 4  // reley 1
#define pinB 7  // reley 2
#define LedVerde 11 // comando a la derecha
#define LedRojo 10 // comando a la isquierda

// int pinANGULO A4 ;
int option;
int T1 =60 ; // factor de multiplicacion del tiempo 
int pos=99; 
String readString;
int val = 0;       // variable que almacena el valor raw (0 a 1023)


void setup() {
  Serial.begin(9600);

 // Serial.println("servo-test-22-dual-input"); // so I can keep track of what is loaded
  pinMode(LedRojo, OUTPUT);
  pinMode(pinA,OUTPUT); // relay
  pinMode(pinB,OUTPUT); // relay
  pinMode(LedVerde,OUTPUT);
  pinMode(pinM,OUTPUT); // motor
  pinMode(pinMA,INPUT_PULLUP); // boton 
  pinMode(pinH,INPUT_PULLUP); // Homing 
  pinMode(pinDE,INPUT_PULLUP); // comando derecha joisty
  pinMode(pinIS,INPUT_PULLUP); // comando isquierd josty
  pinMode(pinIV,INPUT_PULLUP); // invercion comenado dirrecion 
  
  for (int i= 0; i<100;i ++){
       digitalWrite(pinA,HIGH);
       digitalWrite(pinB,LOW); // aciona actuador  isquierda 
       delay(50);
       digitalWrite(pinA,LOW);
       digitalWrite(pinB,LOW);
       if(!digitalRead(pinH)){ pos=0;
       break;}
    }
  if (pos>0) {
     for (int i= 0; i<100;i ++){
       digitalWrite(pinA,LOW);
       digitalWrite(pinB,HIGH); // aciona actuador  isquierda 
       delay(50);
       digitalWrite(pinA,LOW);
       digitalWrite(pinB,LOW);
       if(!digitalRead(pinH)){ pos=0;
       break;}
    }
    
  }
 // if (pos>0){Serial.println("mierda");}
}

void loop() {
 // val = analogRead(pinANGULO);   // realizar la lectura
 //  Serial.println(val);
  if (!digitalRead(pinMA)){digitalWrite(pinM,HIGH);}else {digitalWrite(pinM,LOW);  
  if (!digitalRead(pinIS)){digitalWrite(pinB,HIGH);}else {digitalWrite(pinB,LOW);}// comando isquierda Joisty 20-08-18
  if (!digitalRead(pinDE)){digitalWrite(pinA,HIGH);}else {digitalWrite(pinA,LOW);}// comando derecha Joisty
  }
  
 while (Serial.available()&&(digitalRead(pinM))) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
    String toPrint = readString + "\n";
    char stringInChar[200];
    toPrint.toCharArray(stringInChar, 200);
    Serial.write(stringInChar);
  }

  if (readString.length() >0) {
   // Serial.println(readString);  //so you can see the captured string 
    int n = readString.toInt();  //convert readString into a number
  if (!digitalRead(pinIV)){n=-n; T1=100;} else {T1=60;} 
     { val = analogRead(pinANGULO);     // read the input pin
      Serial.println(val); }   

   if (n >pos) 
   {
      digitalWrite(pinA,HIGH);
      digitalWrite(pinB,LOW); // aciona actuador  isquierda
      digitalWrite(LedRojo,HIGH);
      //Serial.println(n);
     // Serial.println("isquierda");
      delay(abs(n-pos)*T1);
      digitalWrite(pinA,LOW);
      digitalWrite(pinB,LOW);
      digitalWrite(LedRojo,LOW);
      pos =n;
     // Serial.println(n);
   }
   if (n<pos)
   {
    digitalWrite(pinA,LOW);
    digitalWrite(pinB,HIGH); // aciona actuador  derecha
    digitalWrite(LedVerde,HIGH);
    //Serial.println(n);
  //  Serial.println("derecha");
    delay(abs(pos-n)*T1);
    digitalWrite(pinA,LOW);
    digitalWrite(pinB,LOW);
    digitalWrite(LedVerde,LOW);
    pos=n;
 //   Serial.println(n);
   }
   if (n==0){
    digitalWrite(pinA,LOW);
    digitalWrite(pinB,LOW);
    digitalWrite(LedVerde,LOW);
    digitalWrite(LedRojo,LOW);
 //   Serial.println("stop centro");
   }
 readString=""; //empty for next input
}
}

