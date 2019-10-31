#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
//#include <gfxfont.h>

//#include <MD_MAX72xx.h>
//#include <MD_MAX72xx_lib.h>

#include <DFRobotDFPlayerMini.h>

#include <Wire.h>
//#include "Arduino.h"
#include "SoftwareSerial.h"
//#include "DFRobotDFPlayerMini.h"
//#include <SPI.h>
//#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

#define SLAVE_ADDRESS 0x02

int pinCS = 9;
int numberOfHorizontalDisplays = 4; // теперь у нас по-горизонтали 4 матриц
int numberOfVerticalDisplays = 1; // а по-вертикали, по-прежнему, одна

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int InitCmd = 255;
int ActiveCmd = 254;
int ForwardCmd = 253;
int StopCmd = 252;
int BackCmd = 251;
int StartFillingCmd = 250;
int EndFillingCmd = 249;
int BonVoyageCmd = 248;
int EFBVCmd = 247;
int WellcomeCmd = 246;
int AlertCmd = 245;
int ClearCmd = 244;

int r;

//int InitCmd = 127;
//int ActiveCmd = 126;
//int ForwardCmd = 125;
//int StopCmd = 124;
//int BackCmd = 123;
//int StartFillingCmd = 122;
//int EndFillingCmd = 121;
//int BonVoyageCmd = 120;
//int EFBVCmd = 119;
//int WellcomeCmd = 118;
//int AlertCmd = 116;
//int ClearCmd = 114;


int I2CData;
byte val[] = "0";

int InitSound = 6;
int ActiveSound = 7;
int ForwardSound = 8;
int StopSound = 9;
int BackSound = 10;
int StartFillingSound = 11;
int EndFillingSound = 1;
int BonVoyageSound = 2;
int EFBVSound = 3;
int WellcomeSound = 4;
int AlertSound = 5;

int WaitTimeout = 20; // время между крайними перемещениями букв
int Spacer = 1;
int ChWidth = 5 + Spacer;

int NeedSound = 1;
int Counter = 0;

float pokazaniya = 0;

String InitStr = "Initialization";
String ActiveStr = "Wellcome!";
String ForwardStr = "<<<<<";
String StopStr = "Stop";
String BackStr = ">>>>>";
String StartFillingStr = "     ";
String EndFillingStr = "     ";
String BonVoyageStr = "Bon Voyage!";
String EFBVStr = "Bon Voyage!";
String WellcomeStr = "Wellcome!";
String AlertStr = "Alert!";
String ClearStr = "     ";
String PokazaniyaStr = "";

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void setup()
{
  I2CData = InitCmd;
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Ready!");
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));
  matrix.setIntensity(7); // яркость
  for (int i = 0; i < numberOfHorizontalDisplays; i++) {
    matrix.setRotation( i, 1 );
    myDFPlayer.volume(30);  //Set volume value. From 0 to 30
    //    myDFPlayer.play(1);  //Play the first mp3
  }
}

void loop()
{


  if (I2CData == InitCmd)
  {
    Serial.println(I2CData);
    if (NeedSound)
      myDFPlayer.play(InitSound);
    NeedSound = 0;

    if (Counter >= (ChWidth * InitStr.length() + matrix.width() - 1 - Spacer))
      Counter = 0;

    matrix.fillScreen(LOW);

    int letter = Counter / ChWidth;
    int x = (matrix.width() - 1) - Counter % ChWidth;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < InitStr.length())
        matrix.drawChar(x, y, InitStr[letter], HIGH, LOW, 1);
      letter--;
      x -= ChWidth;
    }

    matrix.write();
    delay(WaitTimeout);
    Counter++;
  }

  if (I2CData == ActiveCmd)
  {
    if (NeedSound)
      myDFPlayer.play(ActiveSound);
    NeedSound = 0;

    if (Counter >= (ChWidth * ActiveStr.length() + matrix.width() - 1 - Spacer))
      Counter = 0;

    matrix.fillScreen(LOW);

    int letter = Counter / ChWidth;
    int x = (matrix.width() - 1) - Counter % ChWidth;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < ActiveStr.length())
        matrix.drawChar(x, y, ActiveStr[letter], HIGH, LOW, 1);
      letter--;
      x -= ChWidth;
    }

    matrix.write();
    delay(WaitTimeout);
    Counter++;
  }

  if (I2CData == ForwardCmd)
  {
    if (NeedSound)
      myDFPlayer.play(ForwardSound);
    NeedSound = 0;

    if (Counter >= (ChWidth * ForwardStr.length() + matrix.width() - 1 - Spacer))
      Counter = 0;

    matrix.fillScreen(LOW);

    int letter = Counter / ChWidth;
    int x = (matrix.width() - 1) - Counter % ChWidth;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < ForwardStr.length())
        matrix.drawChar(x, y, ForwardStr[letter], HIGH, LOW, 1);
      letter--;
      x -= ChWidth;
    }

    matrix.write();
    delay(WaitTimeout);
    Counter++;
  }

  if (I2CData == StopCmd)
  {
    if (NeedSound)
      myDFPlayer.play(StopSound);
    NeedSound = 0;

    matrix.fillScreen(LOW);

    int letter = 0;
    int x = 4;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < StopStr.length())
        matrix.drawChar(x, y, StopStr[letter++], HIGH, LOW, 1);
      x += ChWidth;
    }
    matrix.write();
  }

  if (I2CData == BackCmd)
  {
    if (NeedSound)
      myDFPlayer.play(BackSound);
    NeedSound = 0;

    if (Counter >= (ChWidth * BackStr.length() + matrix.width() - 1 - Spacer))
      Counter = 0;

    matrix.fillScreen(LOW);

    int letter = Counter / ChWidth;
    int x = (0 + 1) + Counter % ChWidth;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < BackStr.length())
        matrix.drawChar(x, y, BackStr[letter], HIGH, LOW, 1);
      letter--;
      x += ChWidth;
    }

    matrix.write();
    delay(WaitTimeout);
    Counter++;
  }

  if (I2CData == StartFillingCmd)
  {
    if (NeedSound)
      myDFPlayer.play(StartFillingSound);
    NeedSound = 0;

    matrix.fillScreen(LOW);

    int letter = 0;
    int x = 4;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < StartFillingStr.length())
        matrix.drawChar(x, y, StartFillingStr[letter++], HIGH, LOW, 1);
      x += ChWidth;
    }
    matrix.write();
  }

  if (I2CData == EndFillingCmd)
  {
    if (NeedSound)
      myDFPlayer.play(EndFillingSound);
    NeedSound = 0;

    matrix.fillScreen(LOW);

    int letter = 0;
    int x = 4;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < EndFillingStr.length())
        matrix.drawChar(x, y, EndFillingStr[letter++], HIGH, LOW, 1);
      x += ChWidth;
    }
    matrix.write();
  }

  if (I2CData == BonVoyageCmd)
  {
    if (NeedSound)
      myDFPlayer.play(BonVoyageSound);
    NeedSound = 0;

    if (Counter >= (ChWidth * BonVoyageStr.length() + matrix.width() - 1 - Spacer))
      Counter = 0;

    matrix.fillScreen(LOW);

    int letter = Counter / ChWidth;
    int x = (matrix.width() - 1) - Counter % ChWidth;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < BonVoyageStr.length())
        matrix.drawChar(x, y, BonVoyageStr[letter], HIGH, LOW, 1);
      letter--;
      x -= ChWidth;
    }

    matrix.write();
    delay(WaitTimeout);
    Counter++;
  }

  if (I2CData == WellcomeCmd)
  {
    if (NeedSound)
      myDFPlayer.play(WellcomeSound);
    NeedSound = 0;

    if (Counter >= (ChWidth * WellcomeStr.length() + matrix.width() - 1 - Spacer))
      Counter = 0;

    matrix.fillScreen(LOW);

    int letter = Counter / ChWidth;
    int x = (matrix.width() - 1) - Counter % ChWidth;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < WellcomeStr.length())
        matrix.drawChar(x, y, WellcomeStr[letter], HIGH, LOW, 1);
      letter--;
      x -= ChWidth;
    }

    matrix.write();
    delay(WaitTimeout);
    Counter++;
  }

  if (I2CData == AlertCmd)
  {
    if (NeedSound)
      myDFPlayer.play(AlertSound);
    NeedSound = 0;

    matrix.fillScreen(LOW);

    int letter = 0;
    int x = 2;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < AlertStr.length())
        matrix.drawChar(x, y, AlertStr[letter++], HIGH, LOW, 1);
      x += ChWidth;
    }
    matrix.write();
  }

  if (I2CData == ClearCmd)
  {
    //        if(NeedSound)
    //          myDFPlayer.play(ClearSound);
    //        NeedSound = 0;

    matrix.fillScreen(LOW);

    int letter = 0;
    int x = 4;
    int y = (matrix.height() - 8) / 2;

    while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
    {
      if ( letter < ClearStr.length())
        matrix.drawChar(x, y, ClearStr[letter++], HIGH, LOW, 1);
      x += ChWidth;
    }
    matrix.write();
  }

  if (I2CData < 200 & I2CData > 0) {
    if (NeedSound)
      myDFPlayer.play(StartFillingSound);
    NeedSound = 0;

    pokazaniya = I2CData;
    Serial.println(pokazaniya);
    float pokazaniya1 = pokazaniya / 5;
    for (float m = pokazaniya1; m >= 0; m -= 0.1) {

      String v = " 0";
      PokazaniyaStr = (String)m;
      //      PokazaniyaStr += v;
      Serial.println(PokazaniyaStr);
      matrix.fillScreen(LOW);

      int letter = 0;
      int x = 1;
      int y = (matrix.height() - 8) / 2;

      while (((x + ChWidth - Spacer) >= 0) && (letter >= 0))
      {
        if ( letter < PokazaniyaStr.length())
          matrix.drawChar(x, y, PokazaniyaStr[letter++], HIGH, LOW, 1);
        x += ChWidth;
      }
      matrix.write();
      delay(150);
    }

    NeedSound = 1;
    I2CData = EndFillingCmd;
  }


}

void receiveData(int byteCount)
{
  NeedSound = 1;
  Counter = 0;
  if (Wire.available()) {
    if (Wire.read() >= 100) {
      I2CData = Wire.read();
    }
  }
  Serial.println(I2CData);
  while (Wire.available())
    Wire.read();
}

// Функция для отправки данных.
void sendData()
{

  Wire.write(val, sizeof val);
  //Serial.println(val[0]);
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;

    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

