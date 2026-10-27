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

//Function sigs
void GetCurrentTime(unsigned long &CurrentTime, unsigned long &PrevoiusTime);
void ConvertSeconds(unsigned long CurrentTime, int& hour = CurrentHour,int &minute = CurrentMinute,int &second = CurrentSecond);
void DisplayTime(int hour = CurrentHour, int minute = CurrentMinute, int second = CurrentSecond,bool MilitaryTime = false);
void UpdateTime(unsigned long & Time = CurrentTime,unsigned long & LastTick = PrevoiusTime);



void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  Serial.begin(9600);

  //sync time at program start
  GetCurrentTime(CurrentTime,PrevoiusTime);
}


void loop() {
  // put your main code here, to run repeatedly:
  
  //time managment code
  lcd.clear();
  Serial.print("fail");
  UpdateTime();
  ConvertSeconds(CurrentTime);
  DisplayTime();
  

  delay(50);
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