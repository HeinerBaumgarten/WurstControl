#include <PID_v1.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define LEDpin    0
#define WURSTpP0  1 
#define WURSTpP1  2
#define CELSIUS   "\xDF"

//Define Variables we'll be connecting to
double Setpoint, Input, Output;
int lcd_key     = 0;
int adc_key_in  = 0;
int led_st      = 0;
//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);


int read_LCD_buttons()
{
  adc_key_in = analogRead(0);
  if (adc_key_in > 1000) return btnNONE;
  if (adc_key_in < 50)   return btnRIGHT;  
  if (adc_key_in < 250)  return btnUP; 
  if (adc_key_in < 450)  return btnDOWN; 
  if (adc_key_in < 650)  return btnLEFT; 
  if (adc_key_in < 850)  return btnSELECT;  

  return btnNONE;  // when all others fail, return this...
}

void setup(){
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0,0);
  lcd.print("ASG 2000 control"); // print a simple message
  pinMode(LEDpin,OUTPUT);
  pinMode(WURSTpP0,OUTPUT);
  pinMode(WURSTpP1,OUTPUT);

  //initialize the variables we're linked to
  Input = analogRead(1);
  Setpoint = 100;

  //turn the PID on
  myPID.SetMode(AUTOMATIC)
}

void loop(){
  lcd.setCursor(0,1);
  lcd_key = read_LCD_buttons();
  
  switch (lcd_key)               
  {
  case btnRIGHT:
  {
    if(led_st == 0){
      digitalWrite(LEDpin,HIGH);
      lcd.print("Beleuchtung an");
      delay(800);
      led_st = 1;
      break;
    }else if(led_st == 1){
      digitalWrite(LEDpin,LOW);
      lcd.print("Beleuchtung aus");
      delay(800);
      led_st = 0;
      break;
    }else{
      break;
    }
  }
  case btnLEFT:
  {
    lcd.print("LEFT   ");
    break;
  }
  case btnUP:
  {
    lcd.print("Wurst hoch!     ");
    digitalWrite(WURSTpP0,HIGH);
    digitalWrite(WURSTpP1,LOW);
    break;
  }
  case btnDOWN:
  {
    lcd.print("Wurst runter!   ");
    digitalWrite(WURSTpP0,LOW);
    digitalWrite(WURSTpP1,HIGH);
    break;
  }
  case btnSELECT:
  {
    lcd.print("Control Wurst!  ");
    Input = analogRead(1);
    myPID.Compute();
    analogWrite(3,Output);
    break;
  }
  case btnNONE:
  {
    Input = analogRead(1);
    lcd.print(Input);
    lcd.setCursor(6,1);
    lcd.print("\xDF C");     
    digitalWrite(WURSTpP0,LOW);
    digitalWrite(WURSTpP1,LOW);
    break;
  }
  }
}
