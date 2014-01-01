#include <Time.h>  
#include <LiquidCrystal.h>
//Initialize LCD library with interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define TIME_HEADER  "T"   // Header tag for serial time sync message

void setup()  {
  Serial.begin(9600); // Setup baud rate at 9600 bps
  lcd.begin(16, 2); // Initialize LCD with number of columns and rows
  Serial.println("Waiting for sync"); // Print message on serial monitor
  lcd.print("Waiting for sync");// Print message on LCD
}

void loop(){    
  if (Serial.available()) { //For any input data on serial port
    processSyncMessage(); //Calls function to process input serial message
  }
  if (timeStatus()!= timeNotSet) {//If time is set/synced, display the current time on lcd and on serial port
    digitalClockDisplay();  
    lcd.clear();  
    DateDisplay();
    TimeDisplay();
  }
  delay(1000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void DateDisplay(){
  lcd.setCursor(0, 0);
  lcd.print("Date:");
  lcd.print(" ");
  lcd.print(month());
  lcd.print("-");
  lcd.print(day());
  lcd.print("-");
  lcd.print(year());
}

void TimeDisplay(){
  lcd.setCursor(0, 1);
  lcd.print("Time:");
  lcd.print(" ");
  lcd.print(hour());
  DigitsCheck(minute());
  DigitsCheck(second());
}

void DigitsCheck(int digits){
  // For Time component this function prints ":" and preceding 0 for digits less than 10 
  lcd.print(":");
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}
