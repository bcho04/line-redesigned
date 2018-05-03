#define INF_LEFT_PIN  8 // digital pin
#define INF_MID_PIN   9 // digital pin
#define INF_RIGHT_PIN 10 // digital pin
#define READ_DELAY    10 // in ms

#define STANDBY       3 // digital pin
#define PWMA          6 // digital pin
#define AIN1          4 // digital pin
#define AIN2          5 // digital pin
#define PWMB          0 // digital pin
#define BIN1          2 // digital pin
#define BIN2          1 // digital pin

/*---------------------------------------------------------*/
void move(int motor, int speed, int direction){
  //Move specific motor at speed and direction
  //motor: 0 for B 1 for A
  //speed: 0 is off, and 255 is full speed
  //direction: 0 clockwise, 1 counter-clockwise
  
  digitalWrite(STANDBY, HIGH); //disable standby
  
  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;
  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }
  
  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  } else {
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

void stop(){
  //enable standby
  digitalWrite(STANDBY, LOW); // Stop everything if this happens
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(INF_LEFT_PIN, INPUT); // Define pin modes
  pinMode(INF_MID_PIN, INPUT);
  pinMode(INF_RIGHT_PIN, INPUT);
  pinMode(STANDBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
}

void loop() {
  bool INF_0 = !digitalRead(INF_LEFT_PIN); // Read from the infrared sensor 
  bool INF_1 = !digitalRead(INF_MID_PIN); // ""
  bool INF_2 = !digitalRead(INF_RIGHT_PIN); // ""
  int INF_C = INF_0*4+INF_1*2+INF_2; // Combined state condenses code
  float DIR; // -1 is left, 0 is straight, 1 is right
  int M_LEFT;
  int M_RIGHT;
  if(INF_C == 3) DIR = -1;
  else if(INF_C == 1) DIR = -0.5;
  else if(INF_C == 6) DIR = 1;
  else if(INF_C == 4) DIR = 0.5;
  else DIR = 0;

  if(DIR == 1){
    M_LEFT = 255;
    M_RIGHT = 0; // 128
  }
  if(DIR == 0.5){
    M_LEFT = 255;
    M_RIGHT = 128; // 192
  }
  if(DIR == 0){
    M_LEFT = 255;
    M_RIGHT = 255;
  }
  if(DIR == -0.5){
    M_LEFT = 128; // 192
    M_RIGHT = 255;
  }
  if(DIR == -1){
    M_LEFT = 0; // 128
    M_RIGHT = 255;
  }

  move(1, M_LEFT, 1); // Moves the motor
  move(2, M_RIGHT, 1);
  // print out the value you read:
  Serial.println(String(INF_0) + " " + String(INF_1) + " " + String(INF_2) + " => " + String(INF_C) + " => " + String(DIR) + " => " + String(M_LEFT) + " " + String(M_RIGHT));
  delay(READ_DELAY);        // delay in between reads for stability
}

