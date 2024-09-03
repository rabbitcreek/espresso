#include <TFT_eSPI.h>
#include "lv_xiao_round_screen.h"
#include <SPI.h>
#include "gauge5.h"
#include "battGauge.h"
#include "coffeeCup.h"
#include "hotCoffee.h"
#include "newCoffee.h"
#define USE_TFT_ESPI_LIBRARY
#include "font.h"
#include "Adafruit_MCP9601.h"
#define BATTERY_DEFICIT_VOL 1850    // Battery voltage value at loss of charge
#define BATTERY_FULL_VOL 2057  
#define I2C_ADDRESS (0x67)
long timeNow;
float mvolts;
Adafruit_MCP9601 mcp;
//TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite img = TFT_eSprite(&tft);
TFT_eSprite ln = TFT_eSprite(&tft);
int tempLimit = 90;
double rad=0.01745;
int angle;

int sx=120;
int sy=120;//Users/RobertW./Desktop/lv_xiao_round_screen.h
int r=100;

float x[360];
float y[360];
float x2[360];
float y2[360];

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;
lv_coord_t touchX, touchY;

void setup() {
  pinMode(TOUCH_INT, INPUT_PULLUP);
  //Wire.begin();
    ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
    ledcAttachPin(5, pwmLedChannelTFT);
    ledcWrite(pwmLedChannelTFT, 90);

    //pinMode(12,INPUT_PULLUP);
   
    tft.init();
    tft.setRotation(0);
    tft.setSwapBytes(true);
    img.setSwapBytes(true);
    tft.fillScreen(TFT_ORANGE);
    img.createSprite(240, 240);
    

  tft.setPivot(60,60);
  img.setTextDatum(4);
  img.setTextColor(TFT_GREEN,0x0000);
  img.setFreeFont(&Orbitron_Medium_28);

  int i=0;
  int a=136;

  while(a!=44)
  {
   x[i]=r*cos(rad*a)+sx;
   y[i]=r*sin(rad*a)+sy;
   x2[i]=(r-70)*cos(rad*a)+sx;
   y2[i]=(r-70)*sin(rad*a)+sy;
   i++;
   a++;
   if(a==360)
   a=0;
  }
   Serial.begin(115200);
    //while (!Serial) {
      //delay(10);
   // }
    Serial.println("Adafruit MCP9601 test");

    /* Initialise the driver with I2C_ADDRESS and the default I2C bus. */
    if (! mcp.begin(I2C_ADDRESS)) {
        Serial.println("Sensor not found. Check wiring!");
        while (1);
    }

  Serial.println("Found MCP9601!");

  mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
  

  mcp.setThermocoupleType(MCP9600_TYPE_K);
 

  mcp.setFilterCoefficient(3);
  Serial.print("Filter coefficient value set to: ");
  Serial.println(mcp.getFilterCoefficient());

  mcp.setAlertTemperature(1, 30);
  Serial.print("Alert #1 temperature set to ");
  Serial.println(mcp.getAlertTemperature(1));
  mcp.configureAlert(1, true, true);  // alert 1 enabled, rising temp

  mcp.enable(true);

  Serial.println(F("------------------------------"));
  
}

//min angle 136 or 137
//max angle 43

int a1,a2;
int result=0;

void loop() {
  if(chsc6x_is_pressed()){
    chsc6x_get_xy(&touchX, &touchY);
     if(abs( touchX - 120 ) < 20 && (touchY < 30)){
      batt();
     }
 else {touchy();}
   }
float temp = mcp.readThermocouple();
temp = temp * 1.8 + 32;
Serial.print("Farenheit Temp =    ");
Serial.println( temp );

  delay(500);
  
  temp = constrain(temp, 65, 210);
  result=map(temp,65,210,0,267);
  //angle=map(result,minValue[chosenOne],maxValue[chosenOne],0,267);
  angle = result;
if(tempLimit <= temp){
  img.pushImage(0,0,240,240,coffeeReady);
} else  img.pushImage(0,0,240,240,newCoffee);

 
 
 a1=angle-10;
 a2=angle+10;

 if(a1<0)
 a1=angle-4+359;
  if(a2>=359)
 a2=angle+4-359;
int tempNot = temp;
 for (uint16_t a=0; a<7; a++){
img.drawLine(123,119+a,x[angle],y[angle]+a,TFT_RED);}
img.drawString(String(tempNot),120,150);
img.pushSprite(0, 0);
}
void touchy(){
  img.pushImage(0,0,240,240,coffeeCup);
  img.pushSprite(0,0);
  delay(1000);
  tft.fillScreen(TFT_BROWN);
  
  //tft.fillCircle(120, 20, 20, TFT_RED);

   timeNow = millis();
 while(millis()-timeNow < 10000){
  chsc6x_get_xy(&touchX, &touchY);
    tft.fillCircle(touchX, touchY, 10, TFT_YELLOW);
    
    
  
  
  delay(50);
}
}
void batt(){
 mvolts = 0;
    Serial.println("The display is touched.");
    for(int8_t i=0; i<20; i++){
    mvolts += analogReadMilliVolts(D0);
  }
  mvolts /= 20;
  Serial.print("millivolts: ");
  Serial.println(mvolts);
  int32_t level = (mvolts - BATTERY_DEFICIT_VOL) * 100 / (BATTERY_FULL_VOL-BATTERY_DEFICIT_VOL);
  Serial.print("Level:  ");
  Serial.print(level);
  level = map(level,100,0,0,90);
  img.pushImage(0,0,240,240,battGauge);
  img.fillRect(70,100,90,40, TFT_RED);
  img.fillRect(70,100,10,40, TFT_BLACK);
  
  img.pushSprite(0, 0);
  delay(2000);
}
