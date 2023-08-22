#include <U8g2lib.h>
#include <Bounce2.h>
#include <Servo.h>

Servo servoMotor;  // create servo object to control a servo

const int buzzerPin = 27; 

int servoPositions[] = {180, 90, 45, 135};
int currentServoPosition = 0;
unsigned long servoStartTime = 0;
const unsigned long servoMoveDuration = 1000;  // 1 second


unsigned long previousTime = 0;
int buzzerState = LOW;
unsigned long interval = 500;


bool inMenu0 = false;
bool inMenu1 = false;
bool inMenu2 = false;
bool inMenu3 = false;
bool inMenu4 = false;
bool inMenu5 = false;
bool inMenu6 = false;
bool inMenu7 = false;
bool inMenu8 = false;
bool inMenu9 = false;
bool inMenu10 = false;
bool inMenu11 = false;
bool tone1 = false;
bool isAlarmPlaying = false;
bool servoMoving = false;
bool servoAt180 = false;
bool servoAt90 = false;


unsigned long alarmStartTime = 0;
#define alarm1  "NSAIDs"
#define alarm2  "Acetaminophen"
#define alarm3  "Opioid Analgesics"
#define alarm4  "Muscle Relaxants"
const char *currentAlarmText = "NSAIDs";
        
int a,b,c,d,e,f,g,h;

int Hrs = 12;
int Min = 0;
int tostop = 40;


const int ics7447_count = 4;
const int ics7447_input_pins[ics7447_count][4] = {
  {2, 3, 4, 5}, // IC1: Pins for BCD input A, B, C, D
  {6, 7, 8, 9}, // IC2: Pins for BCD input A, B, C, D
  {10, 11, 12, 13}, // IC3: Pins for BCD input A, B, C, D
  {14, 15, 16, 17} // IC4: Pins for BCD input A, B, C, D
};

byte button_pins[] = {22, 23, 24, 25}; 
#define NUMBUTTONS sizeof(button_pins)
Bounce * buttons = new Bounce[NUMBUTTONS];

U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

#define MENU_SIZE 4
char *menu[MENU_SIZE] = { "Show time", "Time settings", "Tx settings", "Alarms"};
int cursor=0;


void setup() {
  Serial.begin(9600);
  servoMotor.attach(26);  
  servoMotor.write(0);
  pinMode(buzzerPin,OUTPUT);
  for (int i=0; i<NUMBUTTONS; i++) {
    buttons[i].attach(button_pins[i], INPUT_PULLUP); // setup the bounce instance for the current button
    buttons[i].interval(25); // interval in ms
  }
    for (int ic = 0; ic < ics7447_count; ic++) {
    for (int i = 0; i < 4; i++) {
      pinMode(ics7447_input_pins[ic][i], OUTPUT); // Set BCD input pins as output
      digitalWrite(ics7447_input_pins[ic][i], LOW); // Initialize BCD inputs to zero
    }
  }
  display.begin();
  display.setPowerSave(0);
  display.setFont(u8x8_font_pxplusibmcgathin_f);

  showMenu();
  }

void handleAlarmScenario(const char *alarmText) {
  if (inMenu1) {
    currentAlarmText = alarmText; // Update the global variable
    display.setCursor(0, 6);
    display.print("        ");
    display.setCursor(0, 6);
    display.print(currentAlarmText);
  }
  }



void loop() {
  // Check every minute to update the time
  unsigned long currentMillis = millis();
  static unsigned long prevMinuteMillis = currentMillis;

    // Display hours
  displayDigit(0, Hrs / 10);
  displayDigit(1, Hrs % 10);

  // Display minutes
  displayDigit(2, Min / 10);
  displayDigit(3, Min % 10);
  unsigned long currentTime = millis();
  if(tone1 == true){
  if (currentTime - previousTime >= interval) {
    previousTime = currentTime; // Reset the previous time

    // Toggle the buzzer state
    if (buzzerState == LOW) {
      buzzerState = HIGH;
    } else {
      buzzerState = LOW;
    }

    digitalWrite(buzzerPin, buzzerState);
  }
  }
  else{
  digitalWrite(buzzerPin,LOW);
  }



  if (currentMillis - prevMinuteMillis >= 100) { // One minute has elapsed
    prevMinuteMillis = currentMillis;
      Min++;
  if (Min == 60) {
    Min = 0;
    Hrs = (Hrs + 1) % 24;
  }
  if (inMenu1 == true){
  display.setCursor(6, 2); // Set the position for displaying time on OLED
  display.print("  ");
  display.setCursor(6, 2); // Set the position for displaying time on OLED
  display.print(Hrs);
  display.setCursor(9, 2); // Set the position for displaying time on OLED
  display.print("  ");
  display.setCursor(9, 2); // Set the position for displaying time on OLED
  display.print(Min);
  }
  if (inMenu2 == true){
  display.setCursor(6, 2); // Set the position for displaying time on OLED
  display.print("  ");
  display.setCursor(6, 2); // Set the position for displaying time on OLED
  display.print(Hrs);
  }
  if (inMenu3 == true){
  display.setCursor(6, 2); // Set the position for displaying time on OLED
  display.print("  ");
  display.setCursor(6, 2); // Set the position for displaying time on OLED
  display.print(Hrs);
  display.setCursor(9, 2); // Set the position for displaying time on OLED
  display.print("  ");
  display.setCursor(9, 2); // Set the position for displaying time on OLED
  display.print(Min);
  }

  if (Hrs==a && Min==b){
    changePosition(1);
    
  }
  if (Hrs==c && Min==d){
    changePosition(2);
  }
  if (Hrs==e && Min==f){
    changePosition(3);
  }
  if (Hrs==g && Min==h){
    changePosition(4);
  }

 if (Hrs == a && Min == b+tostop) {
    tone1 = false;
    handleAlarmScenario(alarm2);
  }
  if (Hrs == c && Min == d+tostop) {
    tone1 = false;
    handleAlarmScenario(alarm3);
  }
  if (Hrs == e && Min == f+tostop) {
    tone1 = false;
    handleAlarmScenario(alarm4);
  }
  if (Hrs == g && Min == h+tostop) {
    tone1 = false;
    handleAlarmScenario(alarm1);
  }
  moveServo();
  }





  // process button press:
  for (int i = 0; i<NUMBUTTONS; i++) {
    buttons[i].update(); // Update the Bounce instance
    if ( buttons[i].fell() ) { // If it fell
      if (i==0) { // select
        if (inMenu0 == true){
        inMenu0 = false;
        inMenu1 = false;
        inMenu2 = false;
        inMenu3 = false;
        inMenu4 = false;
        inMenu5 = false;
        inMenu6 = false;
        inMenu7 = false;
        inMenu8 = false;
        inMenu9 = false;
        inMenu10 = false;
        inMenu11 = false;
            if (cursor == 0) {
              showTime();
            } else if (cursor == 1) {
              showTimeSettings();
            } else if (cursor == 2) {
            showTxSettings();
            } else if (cursor == 3) {
              // Add your code for the "Alarms" option here
            }
          }
        else if (inMenu1 == true){
        }
        else if (inMenu2 == true){ 
          showTimeSettings2();       
          inMenu3=true;
          inMenu2 = false;          
        }
        else if (inMenu3 == true){
        }
        else if (inMenu4 == true){        
        }
        else if (inMenu5 == true){
        }
        else if (inMenu6 == true){        
        }
        else{
        }
      }  
      else {
        if (i==1){ // down
        if (inMenu0 == true){
        down();
        }
        else if (inMenu1 == true){
        }
        else if (inMenu2 == true){
        upHrs();
        }
        else if (inMenu3 == true){
        upMin();
        }
        else if (inMenu4 == true){
        upHrs1();
        }
        else{
        display.clearLine(2);
        display.setCursor(0, 2);
        display.print("down"); 
          }
        }
        else if (i==2) { // up
        if (inMenu0 == true){
        up();
        }
        else if (inMenu1 == true){
        }
        else if (inMenu2 == true){
        downHrs();
        }
        else if (inMenu3 == true){
        downMin();
        }
        else if (inMenu4 == true){
        downHrs1();
        }
        else{
        display.clearLine(4);
        display.setCursor(0, 4);
        display.print("up"); 
          }
        }
        else if (i==3){
        inMenu0 = false;
        inMenu1 = false;
        inMenu2 = false;
        inMenu3 = false;
        inMenu4 = false;
        inMenu5 = false;
        inMenu6 = false;
        inMenu7 = false;
        inMenu8 = false;
        inMenu9 = false;
        inMenu10 = false;
        inMenu11 = false;
        showMenu();
        }
        else { 
        }
      }
    } // end if button fell...
  }
  } // end for-loop

void down(){
        display.setCursor(0,cursor * 2);
        display.print(' ');
          cursor--;
          if (cursor<0) 
          cursor=(MENU_SIZE-1);
        // show cursor at new line:
        display.setCursor(0,cursor * 2);
        display.print('>');
  }

void up(){
        display.setCursor(0,cursor * 2);
        display.print(' ');
          cursor++;
          if (cursor>(MENU_SIZE-1)) 
          cursor=0;
        // show cursor at new line:
        display.setCursor(0,cursor * 2);
        display.print('>');
  }

void showMenu() {
  inMenu0 = true;
  cursor=0;
  display.clearDisplay();
  // show menu items:
  for (int i = 0; i<MENU_SIZE; i++) {
    display.drawString(2,i*2,menu[i]);
    
  }
  display.setCursor(0,0);
  display.print('>');
  }


void displayDigit(int ic, int digit) {
  // Map the digit to BCD
  const byte bcdMap[] = {
    B0000, // 0
    B0001, // 1
    B0010, // 2
    B0011, // 3
    B0100, // 4
    B0101, // 5
    B0110, // 6
    B0111, // 7
    B1000, // 8
    B1001  // 9
  };

  // Set the BCD input pins based on the digit
  for (int i = 0; i < 4; i++) {
    digitalWrite(ics7447_input_pins[ic][i], bitRead(bcdMap[digit], i));
  }
  }

void showTime() {
  display.clearDisplay();
  display.clearLine(0);
  display.setCursor(0, 0);
  display.print(menu[cursor]);
  display.setCursor(0, 2); // Set the position for displaying time on OLED
  display.print("Time: ");
  display.setCursor(6, 2); // Set the position for displaying time on OLED
  display.print(Hrs);
  display.print(":");
  display.setCursor(9, 2); // Set the position for displaying time on OLED
  display.print(Min);
  display.setCursor(0, 4); // Set the position for displaying time on OLED
  display.print("Next Player:");
  display.setCursor(0, 6);
  display.print(currentAlarmText); // Display the current alarm text

  inMenu1 = true;
  }


void showTimeSettings2() {
  display.setCursor(8, 2); // Set the position for displaying time on OLED
  display.print(":");
  display.setCursor(6, 2); // Set the position for displaying time on OLED
  display.print(Hrs);
  display.setCursor(9, 2); // Set the position for displaying time on OLED
  display.print(Min);
  display.setCursor(6, 3); // Set the position for displaying time on OLED
  display.print("  ");
  display.setCursor(9, 3); // Set the position for displaying time on OLED
  display.print("__");
  inMenu3 = true;

  }

void showTimeSettings() {
  display.clearDisplay();
  display.clearLine(0);
  display.setCursor(0, 0);
  display.print(menu[cursor]);
  display.setCursor(0, 2); // Set the position for displaying time on OLED
  display.print("Time: ");
  display.setCursor(6, 2); // Set the position for displaying time on OLED
  display.print(Hrs);
  display.setCursor(6, 3); // Set the position for displaying time on OLED
  display.print("__");
  inMenu2 = true;
  }

void showTxSettings() {
  display.clearDisplay();
  display.clearLine(0);
  display.setCursor(0, 0);
  display.print(menu[cursor]);
  display.setCursor(0, 2);
  display.print(alarm1);
  display.print(" Setting");
  display.setCursor(0, 4);
  display.print("Time: ");
  display.setCursor(6, 4); // Set the position for displaying time on OLED
  display.print(a);
  display.setCursor(6, 5); // Set the position for displaying time on OLED
  display.print("__");


  inMenu4 = true;
  }



void showTxSettings2(){
  
}
void showTxSettings3(){
  
}
void showTxSettings4(){

}
void showTxSettings5(){

}
void showTxSettings6(){

}
void showTxSettings7(){

}
void showTxSettings8(){

}



void showAlarmSettings() {
  display.clearDisplay();
  display.clearLine(0);
  display.setCursor(0, 0);
  display.print(menu[cursor]);

  inMenu5 = true;
  }





void upMin() {
  Min++;
  inMenu2 = false;
  if (Min == 60) {
    Min = 0;
  }
  showTimeSettings2();
  }
void downMin() {
  Min--;
  inMenu2 = false;
  if (Min == -1) {
    Min = 59;
  }
  showTimeSettings2();
  }

void upHrs() {
  inMenu3 = false;
  Hrs++;
    if (Hrs == 24) {
    Hrs = 0;
    }
  showTimeSettings();
  }

void downHrs() {
  inMenu3 = false;
  Hrs--;
    if (Hrs == -1) {
    Hrs = 23;
  }
  showTimeSettings();
  }


void changePosition(int alarmIndex) {
  currentServoPosition = servoPositions[alarmIndex];
  servoStartTime = millis();
  tone1=true;
  }


void moveServo() {
  if (currentServoPosition != servoMotor.read()) {
    unsigned long currentTime = millis();
    if (currentTime - servoStartTime < servoMoveDuration) {
      int newPosition = map(currentTime, servoStartTime, servoStartTime + servoMoveDuration, servoMotor.read(), currentServoPosition);
      servoMotor.write(newPosition);
    } else {
      servoMotor.write(currentServoPosition);
    }
  }
  }






void upMin1() {
  b++;
  inMenu5 = false;
  if (b == 60) {
    b = 0;
  }
  showTxSettings2();
}
void downMin1() {
  b--;
  inMenu5 = false;
  if (b == -1) {
    b = 59;
  }
  showTxSettings2();
}

void upHrs1() {
  inMenu4 = false;
  a++;
    if (a == 24) {
    a = 0;
    }
  showTxSettings();
}

void downHrs1() {
  inMenu4 = false;
  a--;
    if (a == -1) {
    a = 23;
  }
  showTxSettings();
}


void upMin2() {
  d++;
  inMenu7 = false;
  if (d == 60) {
    d = 0;
  }
  showTxSettings4();
}
void downMin2() {
  d--;
  inMenu7 = false;
  if (d == -1) {
    d = 59;
  }
  showTxSettings4();
}

void upHrs2() {
  inMenu6 = false;
  c++;
    if (c == 24) {
    c = 0;
    }
  showTxSettings3();
}

void downHrs2() {
  inMenu6 = false;
  c--;
    if (c == -1) {
    c = 23;
  }
  showTxSettings3();
}







void upMin3() {
  f++;
  inMenu9 = false;
  if (f == 60) {
    f = 0;
  }
  showTxSettings6();
}
void downMin3() {
  f--;
  inMenu9 = false;
  if (f == -1) {
    f = 59;
  }
  showTxSettings6();
}

void upHrs3() {
  inMenu8 = false;
  e++;
    if (e == 24) {
    e = 0;
    }
  showTxSettings5();
}

void downHrs3() {
  inMenu8 = false;
  e--;
    if (e == -1) {
    e = 23;
  }
  showTxSettings5();
}




void upMin4() {
  g++;
  inMenu11 = false;
  if (g == 60) {
    g = 0;
  }
  showTxSettings8();
}
void downMin4() {
  g--;
  inMenu11 = false;
  if (g == -1) {
    g = 59;
  }
  showTxSettings8();
}

void upHrs4() {
  inMenu10 = false;
  h++;
    if (h == 24) {
    h = 0;
    }
  showTxSettings7();
}

void downHrs4() {
  inMenu10 = false;
  h--;
    if (h == -1) {
    h = 23;
  }
  showTxSettings7();
}