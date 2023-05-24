
//Motor A
int PWMA = 3;   //Speed control pin 3
int AIN1 = 8;   //Direction - HIGH or LOW
int AIN2 = 9;   //Direction - HIGH or LOW

//Motor B
int PWMB = 5;   //Speed control pin 5
int BIN1 = 11;  //Direction - HIGH or LOW
int BIN2 = 12;  //Direction - HIGH or LOW

//Modes
int motorSpeed = 125;
float angle = 0;
float x = 0;
float y = 0;
int STBY = 7; //standby mode, off when HIGH
int LED = 13;
//Serial comms

char junk;
String inputString="";


void setup() {
  // put your setup code here, to run once:

      pinMode(PWMA, OUTPUT);        //Motor A
      pinMode(AIN1, OUTPUT);
      pinMode(AIN2, OUTPUT);

      pinMode(PWMB, OUTPUT);        //Motor B
      pinMode(BIN1, OUTPUT);
      pinMode(BIN2, OUTPUT);

      pinMode(STBY, OUTPUT);        //Standby

      Serial.begin(9600);           // set the baud rate to 9600, same should be of your Serial Monitor
      pinMode(LED, OUTPUT);          //LED Output
      digitalWrite(STBY, HIGH);

}

void loop() {
  
  // MOTOR CONTROL
 if (digitalRead(STBY)== HIGH){moveRover();}  //If motor are enabled then move to coordinates
 
 
     
     
  //SERIAL COMMS
  if(Serial.available()){
    ReadSerial();
    if(inputString == "en"){ digitalWrite(STBY, HIGH); digitalWrite(LED, HIGH);inputString = "";}//Takes Motor out of standby mode
    else if(inputString == "dis"){ digitalWrite(STBY, LOW); digitalWrite(LED, LOW);inputString = "";angle=0;x=0;y=0;} //Put Motor on standby mode
    else{}
  } 
}

void ReadSerial(){
  char inChar = (char)Serial.read(); //read the input
  inputString += inChar;        //make a string of the characters coming on serial
 // Serial.println(inputString);
  
  if(inChar == '*') {    
    inputString = inputString.substring(0, inputString.length() - 1); // Delete last char *
   // Serial.println(inputString);
    int ind1 = inputString.indexOf(',');
    angle = inputString.substring(0, ind1).toFloat();
    int ind2 = inputString.indexOf(',', ind1+1 );
    x = inputString.substring(ind1+1, ind2).toFloat()-145;
    int ind3 = inputString.indexOf(',', ind2+1 );
    y = inputString.substring(ind2+1).toFloat()-145;
    inputString="";}
/*  while (Serial.available() > 0)  
    { junk = Serial.read() ; }      // clear the serial buffer*/
}


void move(int motor, int speed, int direction) {

/*Move specific motor at speed and direction
*     motor: 1 for Motor A, 2 for motor B
*     speed: 0 is off, and 255 is full speed
*     direction: 0 clockwise, 1 counter-clockwise
*/

      boolean inPin1 = LOW;
      boolean inPin2 = HIGH;

      if(direction == 1){
            inPin1 = HIGH;
            inPin2 = LOW;
      }

      if(motor == 1) {
            digitalWrite(AIN1, inPin1);         //Used to detremine motor direction
            digitalWrite(AIN2, inPin2);         //Used to detremine motor direction
            analogWrite(PWMA, speed);     
      } else {
            digitalWrite(BIN1, inPin1);         //Used to detremine motor direction
            digitalWrite(BIN2, inPin2);         //Used to detremine motor direction
            analogWrite(PWMB, speed);
      }
}

void moveRover() {
  int dir1 = 0;
  int dir2 = 0;
  int spd1 = 0;
  int spd2 = 0;
  if (x ==0 and y==0){ }
  else{
    
    if (angle>0){dir1 = 0; dir2=0;}
    else{dir1 = 1; dir2=1;}

    if(abs(angle)>100 and abs(angle)<170){spd1 = motorSpeed;spd2=(motorSpeed/((abs(angle)-45)/45));}//left side of circle
    else if(abs(angle)<80 and abs(angle)>10){spd2 = motorSpeed;spd1=(motorSpeed/((180-abs(angle)-45)/45));}//right side of circle
    else if(abs(angle)<=10){spd2 = motorSpeed;spd1=motorSpeed/3;dir1=1;} //full right
    else if(abs(angle)>=170){spd1 = motorSpeed;spd2=motorSpeed/3;dir2=1;}//full left
    else if(abs(angle)>80 and abs(angle)<100){spd1 = motorSpeed;spd2 = motorSpeed;} //full fwd and rev band       
    }    
//   Serial.println(spd1);
//   Serial.println(dir1);
//   Serial.println(spd2);
//   Serial.println(dir2);
  move(1,spd1,dir1);
  move(2,spd2,dir2);
}
