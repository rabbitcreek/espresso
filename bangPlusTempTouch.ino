#include <TFT_eSPI.h> 
#include "lv_xiao_round_screen.h"
#include "gauge5.h"
#define USE_TFT_ESPI_LIBRARY
#include "font.h"
#include "Adafruit_MCP9601.h"

#define I2C_ADDRESS (0x67)

Adafruit_MCP9601 mcp;
//TFT_eSPI tft = TFT_eSPI(); 
TFT_eSprite img = TFT_eSprite(&tft);
TFT_eSprite ln = TFT_eSprite(&tft);

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

int chosenOne=0;
int minValue[6]={0,20,0,0,0,80};
int maxValue[6]={40,100,60,80,70,160};
int dbounce=0;

void setup() {

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
 pinMode(TOUCH_INT, INPUT_PULLUP);
  Wire.begin();
}

//min angle 136 or 137
//max angle 43

int a1,a2;
int result=0;

void loop() {
  if(chsc6x_is_pressed()){
    Serial.println("The display is touched.");
  }
float temp = mcp.readThermocouple();
temp = temp * 1.8 + 32;
Serial.print("Farenheit Temp =    ");
Serial.println( temp );




  delay(500);
  chosenOne = 0;
  result=map(temp,70,200,0,267);
  //angle=map(result,minValue[chosenOne],maxValue[chosenOne],0,267);
  angle = result;


 
 img.pushImage(0,0,240,240,gauge5);
 

 

 //img.drawString(String(analogRead(22)), 30,10,2);

 a1=angle-10;
 a2=angle+10;

 if(a1<0)
 a1=angle-4+359;
  if(a2>=359)
 a2=angle+4-359;
/*
 if(result<=minValue[chosenOne]+4)
 img.fillTriangle(x[angle],y[angle],x2[angle],y2[angle],x2[a2+2],y2[a2+2],TFT_RED);
 else if(result>=maxValue[chosenOne]-4)
 img.fillTriangle(x[angle],y[angle],x2[a1-2],y2[a1-2],x2[angle],y2[angle],TFT_RED);
 else
 img.fillTriangle(x[angle],y[angle],x2[a1],y2[a1],x2[a2],y2[a2],TFT_RED);
 */
 for (uint16_t a=0; a<7; a++){
img.drawLine(123,125+a,x[angle],y[angle]+a,TFT_RED);}
  img.drawString(String(temp),120,150);
 img.pushSprite(0, 0);
}
