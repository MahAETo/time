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
bool inMenu12 = false;
bool tone1 = false;
bool isAlarmPlaying = false;
bool servoMoving = false;
bool servoAt180 = false;
bool servoAt90 = false;


unsigned long alarmStartTime = 0;
#define alarm1  "NSAIDs"
#define alarm2  "Acetaminophen"
#define alarm3  "Analgesics"
#define alarm4  "Relaxants"
const char *currentAlarmText = "NSAIDs";   //==(alarm1)
        

int Hrs = 12;
int Min = 0;
int tostop = 5;  //min's
int M1,M2,M3,M4,H1,H2,H3,H4;


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



  if (currentMillis - prevMinuteMillis >= 60000) { // One minute has elapsed
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

  if (Hrs==H1 && Min==M1){
    changePosition(1);
    
  }
  if (Hrs==H2 && Min==M2){
    changePosition(2);
  }
  if (Hrs==H3 && Min==M3){
    changePosition(3);
  }
  if (Hrs==H4 && Min==M4){
    changePosition(4);
  }

 if (Hrs == H1 && Min == M1+tostop) {
    tone1 = false;
    handleAlarmScenario(alarm2);
  }
  if (Hrs == H2 && Min == M2+tostop) {
    tone1 = false;
    handleAlarmScenario(alarm3);
  }
  if (Hrs == H3 && Min == M3+tostop) {
    tone1 = false;
    handleAlarmScenario(alarm4);
  }
  if (Hrs == H4 && Min == M4+tostop) {
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
        inMenu12 = false;
            if (cursor == 0) {
              showTime();
            } else if (cursor == 1) {
              showTimeSettings();
            } else if (cursor == 2) {
            showTxSettings();
            } else if (cursor == 3) {
            showAlarmSettings();
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
          showTxSettings2();       
          inMenu5 =true;
          inMenu4 = false;
        }
        else if (inMenu5 == true){
          showTxSettings3();       
          inMenu6 =true;
          inMenu5 = false;
        }
        else if (inMenu6 == true){ 
          showTxSettings4();       
          inMenu7 =true;
          inMenu6 = false;       
        }
        else if (inMenu7 == true){
          showTxSettings5();       
          inMenu8 =true;
          inMenu7 = false;
        }
        else if (inMenu8 == true){  
          showTxSettings6();       
          inMenu9 =true;
          inMenu8 = false;      
        }
        else if (inMenu9 == true){
          showTxSettings7();       
          inMenu10 =true;
          inMenu9 = false;
        }
        else if (inMenu10 == true){ 
          showTxSettings8();       
          inMenu11 =true;
          inMenu10 = false;       
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
        else if (inMenu5 == true){
        upMin1();
        }
        else if (inMenu6 == true){
        upHrs2();
        }
        else if (inMenu7 == true){
        upMin2();
        }
        else if (inMenu8 == true){
        upHrs3();
        }
        else if (inMenu9 == true){
        upMin3();
        }
        else if (inMenu10 == true){
        upHrs4();
        }
        else if (inMenu11 == true){
        upMin4();
        }
        else if (inMenu12 == true){
      
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
        else if (inMenu5 == true){
        downMin1();
        }
        else if (inMenu6 == true){
        downHrs2();
        }
        else if (inMenu7 == true){
        downMin2();
        }
        else if (inMenu8 == true){
        downHrs3();
        }
        else if (inMenu9 == true){
        downMin3();
        }
        else if (inMenu10 == true){
        downHrs4();
        }
        else if (inMenu11 == true){
        downMin4();
        }
        else if (inMenu12 == true){
        
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
        inMenu12 = false;
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
  display.print("--");
  display.clearLine(6);
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
  display.print("--");
  display.setCursor(0, 6); // Set the position for displaying time on OLED
  display.print("> press Set");
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
  display.print(H1);
  display.setCursor(6, 5); // Set the position for displaying time on OLED
  display.print("--");
  display.setCursor(0, 6); // Set the position for displaying time on OLED
  display.print("> press Set");

  inMenu4 = true;
  }



void showTxSettings2(){
  display.setCursor(8, 4); // Set the position for displaying time on OLED
  display.print(":");
  display.setCursor(6, 4); // Set the position for displaying time on OLED
  display.print(H1);
  display.setCursor(9, 4); // Set the position for displaying time on OLED
  display.print(M1);
  display.setCursor(6, 5); // Set the position for displaying time on OLED
  display.print("  ");
  display.setCursor(9, 5); // Set the position for displaying time on OLED
  display.print("--");
  display.setCursor(0, 6); // Set the position for displaying time on OLED
  display.print("> press Set");

  inMenu5 = true;
 }
void showTxSettings3(){
  display.clearLine(2);
  display.clearLine(4);
  display.clearLine(5);
  display.clearLine(6);
  display.setCursor(0, 2);
  display.print(alarm2);
  display.print(" Setting");
  display.setCursor(0, 4);
  display.print("Time: ");
  display.setCursor(6, 4); // Set the position for displaying time on OLED
  display.print(H2);
  display.setCursor(6, 5); // Set the position for displaying time on OLED
  display.print("--");
  display.setCursor(0, 6); // Set the position for displaying time on OLED
  display.print("> press Set");

  inMenu6 = true;
 }
void showTxSettings4(){
  display.setCursor(8, 4); // Set the position for displaying time on OLED
  display.print(":");
  display.setCursor(6, 4); // Set the position for displaying time on OLED
  display.print(H2);
  display.setCursor(9, 4); // Set the position for displaying time on OLED
  display.print(M2);
  display.setCursor(6, 5); // Set the position for displaying time on OLED
  display.print("  ");
  display.setCursor(9, 5); // Set the position for displaying time on OLED
  display.print("--");
  display.setCursor(0, 6); // Set the position for displaying time on OLED
  display.print("> press Set");

  inMenu7 = true;
 }
void showTxSettings5(){
  display.clearLine(2);
  display.clearLine(4);
  display.clearLine(5);
  display.clearLine(6);
  display.setCursor(0, 2);
  display.print(alarm3);
  display.print(" Setting");
  display.setCursor(0, 4);
  display.print("Time: ");
  display.setCursor(6, 4); // Set the position for displaying time on OLED
  display.print(H3);
  display.setCursor(6, 5); // Set the position for displaying time on OLED
  display.print("--");
  display.setCursor(0, 6); // Set the position for displaying time on OLED
  display.print("> press Set");

  inMenu8 = true;
 }
void showTxSettings6(){
  display.setCursor(8, 4); // Set the position for displaying time on OLED
  display.print(":");
  display.setCursor(6, 4); // Set the position for displaying time on OLED
  display.print(H3);
  display.setCursor(9, 4); // Set the position for displaying time on OLED
  display.print(M3);
  display.setCursor(6, 5); // Set the position for displaying time on OLED
  display.print("  ");
  display.setCursor(9, 5); // Set the position for displaying time on OLED
  display.print("--");
  display.setCursor(0, 6); // Set the position for displaying time on OLED
  display.print("> press Set");

  inMenu9 = true;
 }
void showTxSettings7(){
  display.clearLine(2);
  display.clearLine(4);
  display.clearLine(5);
  display.clearLine(6);
  display.setCursor(0, 2);
  display.print(alarm4);
  display.print(" Setting");
  display.setCursor(0, 4);
  display.print("Time: ");
  display.setCursor(6, 4); // Set the position for displaying time on OLED
  display.print(H4);
  display.setCursor(6, 5); // Set the position for displaying time on OLED
  display.print("--");
  display.setCursor(0, 6); // Set the position for displaying time on OLED
  display.print("> press Set");

  inMenu10 = true;
 }
void showTxSettings8(){
  display.setCursor(8, 4); // Set the position for displaying time on OLED
  display.print(":");
  display.setCursor(6, 4); // Set the position for displaying time on OLED
  display.print(H4);
  display.setCursor(9, 4); // Set the position for displaying time on OLED
  display.print(M4);
  display.setCursor(6, 5); // Set the position for displaying time on OLED
  display.print("  ");
  display.setCursor(9, 5); // Set the position for displaying time on OLED
  display.print("--");
  display.clearLine(6);

  inMenu11 = true;
 }



void showAlarmSettings() {
  display.clearDisplay();
  display.clearLine(0);
  display.setCursor(0, 0);
  display.print(alarm1);
  display.setCursor(8, 1); // Set the position for displaying time on OLED
  display.print(":");
  display.setCursor(6, 1); // Set the position for displaying time on OLED
  display.print(H1);
  display.setCursor(9, 1); // Set the position for displaying time on OLED
  display.print(M1);
  display.setCursor(0, 2);
  display.print(alarm2);
  display.setCursor(8, 3); // Set the position for displaying time on OLED
  display.print(":");
  display.setCursor(6, 3); // Set the position for displaying time on OLED
  display.print(H2);
  display.setCursor(9, 3); // Set the position for displaying time on OLED
  display.print(M2);
  display.setCursor(0, 4);
  display.print(alarm3);
  display.setCursor(8, 5); // Set the position for displaying time on OLED
  display.print(":");
  display.setCursor(6, 5); // Set the position for displaying time on OLED
  display.print(H3);
  display.setCursor(9, 5); // Set the position for displaying time on OLED
  display.print(M3);
  display.setCursor(0, 6);
  display.print(alarm4);
  display.setCursor(8, 7); // Set the position for displaying time on OLED
  display.print(":");
  display.setCursor(6, 7); // Set the position for displaying time on OLED
  display.print(H4);
  display.setCursor(9, 7); // Set the position for displaying time on OLED
  display.print(M4);
  inMenu12 = true;
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
  M1++;
  inMenu5 = false;
  if (M1 == 60) {
    M1 = 0;
  }
  showTxSettings2();
 } 
void downMin1() {
  M1--;
  inMenu5 = false;
  if (M1 == -1) {
    M1 = 59;
  }
  showTxSettings2();
 }

void upHrs1() {
  inMenu4 = false;
  H1++;
    if (H1 == 24) {
    H1 = 0;
    }
  showTxSettings();
 }

void downHrs1() {
  inMenu4 = false;
  H1--;
    if (H1 == -1) {
    H1 = 23;
  }
  showTxSettings();
 }


void upMin2() {
  M2++;
  inMenu7 = false;
  if (M2 == 60) {
    M2 = 0;
  }
  showTxSettings4();
 }
void downMin2() {
  M2--;
  inMenu7 = false;
  if (M2 == -1) {
    M2 = 59;
  }
  showTxSettings4();
 }

void upHrs2() {
  inMenu6 = false;
  H2++;
    if (H2 == 24) {
    H2 = 0;
    }
  showTxSettings3();
 }

void downHrs2() {
  inMenu6 = false;
  H2--;
    if (H2 == -1) {
    H2 = 23;
  }
  showTxSettings3();
 }







void upMin3() {
  M3++;
  inMenu9 = false;
  if (M3 == 60) {
    M3 = 0;
  }
  showTxSettings6();
 }
void downMin3() {
  M3--;
  inMenu9 = false;
  if (M3 == -1) {
    M3 = 59;
  }
  showTxSettings6();
 }

void upHrs3() {
  inMenu8 = false;
  H3++;
    if (H3 == 24) {
    H3 = 0;
    }
  showTxSettings5();
 }

void downHrs3() {
  inMenu8 = false;
  H3--;
    if (H3 == -1) {
    H3 = 23;
  }
  showTxSettings5();
 }




void upMin4() {
  M4++;
  inMenu11 = false;
  if (M4 == 60) {
    M4 = 0;
  }
  showTxSettings8();
 }
void downMin4() {
  M4--;
  inMenu11 = false;
  if (M4 == -1) {
    M4 = 59;
  }
  showTxSettings8();
 }

void upHrs4() {
  inMenu10 = false;
  H4++;
    if (H4 == 24) {
    H4 = 0;
    }
  showTxSettings7();
 }

void downHrs4() {
  inMenu10 = false;
  H4--;
    if (H4 == -1) {
    H4 = 23;
  }
  showTxSettings7();
 }
