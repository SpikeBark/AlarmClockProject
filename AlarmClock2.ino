/*
Alarm Clock
Authors: Asher Norris
*/


#include "rgb_lcd.h"
#include <list>

rgb_lcd lcd;

//Global Vars
unsigned long CurrentTime;
unsigned long PrevoiusTime;
int CurrentHour,CurrentMinute,CurrentSecond;

const int buzzerpin = 3;
const int ledpin = 7;
const int rotatorysensorpin = A0;
const int switchpin = 6;


// Alarm variables
int alarmHour = 0;
int alarmmin = 0;
bool isSettingAlarm = false;
int editposition = 0; // 0 = edit hours, 1 = editing minute
bool flickerstate = true;

//Function sigs
void GetCurrentTime(unsigned long &CurrentTime, unsigned long &PrevoiusTime);
void ConvertSeconds(unsigned long CurrentTime, int& hour = CurrentHour,int &minute = CurrentMinute,int &second = CurrentSecond);
void DisplayTime(int hour = CurrentHour, int minute = CurrentMinute, int second = CurrentSecond,bool MilitaryTime = false);
void UpdateTime(unsigned long & Time = CurrentTime,unsigned long & LastTick = PrevoiusTime);
void showdisplaywhennotsettingalarm();



void setup() {
  // put your setup code here, to run once:
  pinMode(switchpin, INPUT);
  pinMode(buzzerpin, OUTPUT);
  pinMode(ledpin, OUTPUT);
  pinMode(rotatorysensorpin, INPUT);

  lcd.begin(16, 2);
  Serial.begin(9600);

  //sync time at program start
  GetCurrentTime(CurrentTime,PrevoiusTime);
}


void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(switchpin) == LOW) {
    if (!isSettingAlarm) {
      isSettingAlarm = true;
      editposition = 0; // Start by editing hours
    } else if (editposition == 0) {
      editposition = 1; // Switch from hours to minutes
    } else {
      isSettingAlarm = false;
    }

    delay(300); // Simple delay to wait for the button press to settle
  }

  //time managment code
  UpdateTime();
  ConvertSeconds(CurrentTime);

  if (isSettingAlarm) {
    //checks if 300ms have passed and flips between true and false and flickers

    bool flickerstate = (millis() / 300) % 2 == 0; //uses built in millis() function


    // setting everything up to be displayed
    lcd.setCursor(0, 0);
    lcd.print("set alarm time: ");
    lcd.setCursor(0, 1);


    int sensorValue = analogRead(rotatorysensorpin);

    // Map rotary sensor value based on what position we are editing
    if (editposition == 0) {
      alarmHour = map(sensorValue, 0, 1023, 0, 23);
    } else {
      alarmmin = map(sensorValue, 0, 1023, 0, 59);
    }


    // flickers if editposition is 0 and flickerstate is false
    if (editposition == 0 && !flickerstate) {
      lcd.print("     ");
    } else {
      if (alarmHour < 10) {
        lcd.print("0"); //places a 0 in front of the number if hours are less than 10
      }
      lcd.print(alarmHour);
    }
    lcd.print(":"); //this seperates hours and minutes


    //flickers if editposition is 1 and flickerstate is false
    if (editposition == 1 && !flickerstate) {
      lcd.print("     ");
    } else {
      if (alarmmin < 10) {
        lcd.print("0"); //places a 0 in front of the number if minutes are less than 10
      }
      lcd.print(alarmmin);
    }
    lcd.print("      "); // Clears remaining characters to prevent lefovers from previous
  } else {
    //function call to show default time when not setting up alarm
    showdisplaywhennotsettingalarm();
  }

  delay(50);
}


void showdisplaywhennotsettingalarm() {
  lcd.setCursor(0, 0);  //moves cursor to first line of lcd
  lcd.print("time: ");
  if (CurrentHour < 10) {
    lcd.print("0");
  }
  lcd.print(CurrentHour);
  lcd.print(":");
  if (CurrentMinute < 10) {
    lcd.print("0");
  }
  lcd.print(CurrentMinute);
  lcd.print(":");
  if (CurrentSecond < 10) {
    lcd.print("0");
  }
  lcd.print(CurrentSecond);
  lcd.print(" ");
  lcd.setCursor(0, 1);  //moves cursor to bottom left corner
  lcd.print("alarm: ");
  if (alarmHour < 10) {
    lcd.print("0");
  }
  lcd.print(alarmHour);
  lcd.print(":");

  if (alarmmin < 10) {
    lcd.print("0");
  }
  lcd.print(alarmmin);
  lcd.print("          "); // Clears remaining characters to prevent lefovers from previous
}

/*
Used alot of reference params might wont to switch to just using globals. 
Might make it a little more adjustable and readable
But if it work it works
*/
//function that converts seconds into hours, minutes and seconds
void ConvertSeconds(unsigned long CurrentTime, int &hour,int &minute ,int &second ){
  hour = CurrentTime/3600;
  minute = (CurrentTime%3600)/60;
  second = CurrentTime%60;

}




//pull time from compile time
//WILL BASE TIME FROM LAST COMPILE
//Might Want to fix in the future
void GetCurrentTime(unsigned long &CurrentTime, unsigned long &PrevoiusTime){
  int hour,minute,second;
  //Get Current Time
  sscanf(__TIME__, "%d:%d:%d", &hour, &minute, &second);
  CurrentTime = hour * 3600UL + minute * 60UL + second;
  PrevoiusTime = millis();

}
//Display the time
void DisplayTime(int hour, int minute, int second ,bool MilitaryTime){
  if (MilitaryTime)
    lcd.print(hour);
  else
    lcd.print(hour%12);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);
}


//update time, independent of other loop occurences, doesnt matter time between calls
void UpdateTime(unsigned long & Time,unsigned long & LastTick){
  unsigned long current = millis();
  unsigned long delta = (current - LastTick)/1000UL;

  if (delta > 0){
    LastTick = current;
    Time = Time + delta;
  }
  

}