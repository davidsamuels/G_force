#include <Arduino_LSM6DS3.h>   // lib for inertial measuerment unit built into arduino iot 33 board
#include <Adafruit_NeoPixel.h> //lib for led strip

#define PIN  2     //pin the rgb strip is connected to
#define NumPixel 32 //  4 pairs of 8led strips
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NumPixel, PIN, NEO_GRB + NEO_KHZ800);
void setup()
{
  // put your setup code here, to run once:
  strip.begin();
  strip.setBrightness(20);
  strip.show();

  pinMode(12, INPUT_PULLUP); //BUTTON
  pinMode(10, OUTPUT);       // EFFECTIVLY SWITCHABLE GROUND
  digitalWrite(10, LOW);     //GROUND THE BUTON

  Serial.begin(9600);
  //while (!Serial);

  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1);
      
  }
}

float fxcur, fycur, fzcur, fxzero, fyzero, fzzero, fxcal, fycal, fzcal; // float varibles to hold accelorometer data

void loop()
{
  smoothData();
  if (digitalRead(12) == 0) //cal button
    calibration();

  offsetCorrection();
  lit();
  
}

float smoothData()
{
  float x, y, z; // temporary varibles to hold data for averaging/smoothing data
  float sumx = 0, sumy = 0 ; 
  float avex = 0, avey = 0;
  int smoothSize = 8; //bigger the num the stronger the effect and slower the calculation

  for (int i = 0; i < smoothSize; i++)
  {
    while (IMU.accelerationAvailable() == 0)
    {
      //Serial.println("waiting for data ");
    }
    IMU.readAcceleration(x, y, z);
    sumx = sumx + x;
    sumy = sumy + y;
  }

  fxcur = sumx / (float)smoothSize;
  fycur = sumy / (float)smoothSize;
  Serial.println("fxcur ");
  Serial.println(fxcur);
  Serial.println("fycur ");
  Serial.println(fycur);
}

void calibration() //after this function is called we have calibrated values to work with
{
  //Serial.println("button pressed zeroing sensor");
  IMU.readAcceleration(fxzero, fyzero, fzzero);
}

void offsetCorrection()
{
  fxcal = fxcur - fxzero;
  fycal = fycur - fyzero;
  //if fxcal > 1 , fxcal = 1
  Serial.println("fxcal ");
  Serial.println(fxcal);
  Serial.println("fycal ");
  Serial.println(fycal);
}

void lit()
{
  strip.fill(strip.Color(0,0,0),0,0); //erase strip
  if (fycal > 0) //leds [0-7]
  {
    //need to think how to select within range
    for(int yptmp = 0 ; yptmp < (int)(fycal*8); yptmp++)
    {
      strip.setPixelColor(yptmp,strip.ColorHSV(10000*yptmp+1000,255,255));
    }
    strip.setPixelColor((int)(fycal*8),255,0,0);
    Serial.println("posistion y+ ");
    Serial.println((fycal*8));
  }
  else //leds [16-23]
  {
    for(int yptmp = 16 ; yptmp < (int)(-fycal*8)+16; yptmp++)
    {
      strip.setPixelColor(yptmp,strip.ColorHSV(10000*yptmp+1000,255,255));
    }
    strip.setPixelColor((int)(-fycal*8)+16,255,0,0);
    Serial.println("posistion y- ");
    Serial.println((-fycal*8)+16);
  }

  if (fxcal > 0) //leds[24-31]
  {
     for(int xptmp = 24 ; xptmp < (int)(fxcal*8)+24; xptmp++)
    {
      strip.setPixelColor(xptmp,strip.ColorHSV(10000*xptmp+1000,255,255));
    }
    strip.setPixelColor((int)(fxcal*8)+24,255,0,0);
    Serial.println("posistion x+ ");
    Serial.println((fxcal*8)+8);

  }
  else //leds[8-15]
  {
    for(int xptmp = 8 ; xptmp < (int)(-fxcal*8)+8 ; xptmp++)
    {
      strip.setPixelColor(xptmp,strip.ColorHSV(10000*xptmp+1000,255,255));
    }
    strip.setPixelColor((int)(-fxcal*8)+8,255,0,0);
    Serial.println("posistion x- ");
    Serial.println((-fxcal*8)+25);
  }
  Serial.println("\n \n sending values to display ");
  strip.show();
}