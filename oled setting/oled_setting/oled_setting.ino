#include <U8g2lib.h>
#include <Bounce2.h>
bool inMenu = true; 
byte button_pins[] = {22, 23, 24, 25}; 
#define NUMBUTTONS sizeof(button_pins)
Bounce * buttons = new Bounce[NUMBUTTONS];

U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

#define MENU_SIZE 4
char *menu[MENU_SIZE] = {"Time settings", "Show time", "Tx settings", "Alarm"};
int cursor=0;

// Variables to store time and alarm settings
#define hours 12
#define minutes 0
int alarmHour = 12;
int alarmMinute = 0;
bool alarmEnabled = false;

void setup() {
  Serial.begin(9600);
  for (int i=0; i<NUMBUTTONS; i++) {
    buttons[i].attach(button_pins[i], INPUT_PULLUP); // setup the bounce instance for the current button
    buttons[i].interval(25); // interval in ms
  }
  
  display.begin();
  display.setPowerSave(0);
  display.setFont(u8x8_font_pxplusibmcgathin_f);

  showMenu();
}

void loop() {

for (int x = 0; x<cursor; x++){
  if (x==0){
    
  }
  if (x==1){

  }
  if (x==2){

  }
  if (x==3){

  }
}


  // process button press:
  for (int i = 0; i<NUMBUTTONS; i++) {
    buttons[i].update(); // Update the Bounce instance
    if ( buttons[i].fell() ) { // If it fell
      if (i==0) { // select
          inMenu = false;
          display.clearDisplay();
          display.clearLine(0);
          display.setCursor(0, 0);
          display.print(menu[cursor*2]);
          executeChoice(cursor); // Execute the selected menu option
      }
      else {
        if (i==1){ // down
        if (inMenu == true){
        down();
        }
        else{
          display.clearLine(2);
          display.setCursor(0, 2);
          display.print("down"); 
        }
        }
        else if (i==2) { // up
        if (inMenu == true){
        up();
        }
        else{
          display.clearLine(4);
          display.setCursor(0, 4);
          display.print("up"); 
        }
        }
        else if (i==3){
          showMenu();
        }
        else { 

        }
      }
    } // end if button fell...
  } // end for-loop of button check
}
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
  inMenu = true;
  cursor=0;
  display.clearDisplay();
  // show menu items:
  for (int i = 0; i<MENU_SIZE; i++) {
    display.drawString(2,i*2,menu[i]);
    
  }
  display.setCursor(0,0);
  display.print('>');
}

/**
 * Execute the task which matches the chosen menu item.
 */
void executeChoice(int choice) {
  switch(choice) {
    case 0:
      // Time settings
      showTimeSettings();
      break;
    case 1:
      // Show time
      showTime();
      break;
    case 2:
      // Tx settings
      showTxSettings();
      break;
    case 3:
      // Alarm
      showAlarmSettings();
      break;
    default:
      break;
  }
}

/**
 * Time settings menu.
 * Allow adjusting the time.
 */
void showTimeSettings() {
  // Implement code to allow adjusting hours and minutes using buttons
  // For example, you can use buttons[1] to increase hours, buttons[2] to decrease hours
  // and buttons[3] to increase minutes, buttons[4] to decrease minutes.
  // Update the 'hours' and 'minutes' variables accordingly.
  // After adjusting, you can show the updated time settings on the display.
  // Don't forget to handle edge cases, such as 12/24-hour format, 0-59 minutes, etc.
}

/**
 * Show the current time on the display.
 */
void showTime() {

}

/**
 * Tx settings menu.
 * Implement code to allow adjusting transmission settings, if needed.
 */
void showTxSettings() {
  // Implement code to adjust transmission settings, if required.
}

/**
 * Alarm menu.
 * Allow setting and displaying alarm settings.
 */
void showAlarmSettings() {
  // Implement code to allow setting the alarm time using buttons
  // For example, you can use buttons[1] to increase hours, buttons[2] to decrease hours
  // and buttons[3] to increase minutes, buttons[4] to decrease minutes.
  // Update the 'alarmHour' and 'alarmMinute' variables accordingly.
  // Implement code to toggle alarm on/off using a button (e.g., buttons[0]).
  // After adjusting, you can show the alarm settings on the display.
  // Don't forget to handle edge cases, such as 12/24-hour format, 0-59 minutes, etc.
  // You may want to use the 'alarmEnabled' variable to keep track of whether the alarm is enabled or not.
}

/**
 * Show the alarm status on the display.
 */
void showAlarmStatus() {
  // Implement code to display the alarm status (enabled or disabled) on the screen.
}
