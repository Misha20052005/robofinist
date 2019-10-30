#include <AccelStepper.h>
#include <Wire.h>

#define StepPerTurn 200

#define XStepPerLoop 100000
#define YStepPerLoop 100000
#define ZStepPerLoop 100000

//#define XStart -50
#define XStart 0
#define XMinPosition 0
#define XMaxPosition 1350

#define YStart 0
#define YMinPosition 0
#define YMaxPosition 5500

//#define ZStart -100
#define ZStart 0
#define ZMinPosition 0
#define ZMaxPosition 7500

#define XScale 10
#define YScale 55
#define ZScale 50

int XCurent;
int YCurent;
int ZCurent;

int XTarget;
int YTarget;
int ZTarget;

AccelStepper StepperZ(1, 7, 6); //использует пин 12 и 13 для dir и step, 1 - режим "external driver" (A4988)
AccelStepper StepperX(1, 3, 2); //использует пин 12 и 13 для dir и step, 1 - режим "external driver" (A4988)
AccelStepper StepperY(1, 5, 4); //использует пин 12 и 13 для dir и step, 1 - режим "external driver" (A4988)

int Received[3];
int ReceivedCount = -1;

void SetupPinMode() {
}

void SetupSerialPort() {
  Serial.begin(9600);           // start serial for output
}

void SetupI2CBus() {
  Wire.begin(4);                // join i2c bus with address #8
  Wire.onReceive(OnI2CReceiveEvent); // register event
}

void SetupStepperMotors() {
  StepperX.setMaxSpeed(1000); //устанавливаем максимальную скорость вращения ротора двигателя (шагов/секунду)
  StepperX.setAcceleration(1000); //устанавливаем ускорение (шагов/секунду^2)
  StepperY.setMaxSpeed(1000); //устанавливаем максимальную скорость вращения ротора двигателя (шагов/секунду)
  StepperY.setAcceleration(1000); //устанавливаем ускорение (шагов/секунду^2)
  StepperZ.setMaxSpeed(1000); //устанавливаем максимальную скорость вращения ротора двигателя (шагов/секунду)
  StepperZ.setAcceleration(1000); //устанавливаем ускорение (шагов/секунду^2)
}

void DoXSteps(int XSteps) {
  if (XSteps) {
    StepperX.move(-XSteps);
    StepperX.runToPosition();
//    StepperX.run();
    XCurent += XSteps;

  }
}

void DoYSteps(int YSteps) {
  if (YSteps) {
    StepperY.move(-YSteps);
    StepperY.runToPosition();
//    StepperY.run();
    YCurent += YSteps;

  }
}

void DoZSteps(int ZSteps) {
  if (ZSteps) {
    StepperZ.move(-ZSteps);
    StepperZ.runToPosition();
//    StepperZ.run();
    ZCurent += ZSteps;
    //    Serial.println(ZTarget);

  }
}

void DoSteps(int XSteps, int YSteps, int ZSteps) {
  DoXSteps(XSteps);
  DoZSteps(ZSteps);
  DoYSteps(YSteps);
}

void DoSteps0(int XSteps, int YSteps, int ZSteps) {
  DoYSteps(YSteps);
  DoZSteps(ZSteps);
  DoXSteps(XSteps);
}

void setup() {
  XCurent = XStart;
  YCurent = YStart;
  ZCurent = ZStart;

  //XTarget = XCurent;
  //YTarget = YCurent;
  //ZTarget = ZCurent;

  XTarget = XMinPosition;
  YTarget = YMinPosition;
  ZTarget = ZMinPosition;

  SetupPinMode();
  SetupSerialPort();
  SetupStepperMotors();
  SetupI2CBus();
}

void loop() {
  int XSteps = XTarget - XCurent;
  int YSteps = YTarget - YCurent;
  int ZSteps = ZTarget - ZCurent;

//  StepperX.run();
//  StepperY.run();
//  StepperZ.run();

  if (XSteps > XStepPerLoop)
    XSteps = XStepPerLoop;
  if (-XSteps > XStepPerLoop)
    XSteps = -XStepPerLoop;

  if (YSteps > YStepPerLoop)
    YSteps = YStepPerLoop;
  if (-YSteps > YStepPerLoop)
    YSteps = -YStepPerLoop;

  if (ZSteps > ZStepPerLoop)
    ZSteps = ZStepPerLoop;
  if (-ZSteps > ZStepPerLoop)
    ZSteps = -ZStepPerLoop;

  if (XSteps || YSteps || ZSteps) {
    if (XTarget || YTarget || ZTarget)
      DoSteps(XSteps, YSteps, ZSteps);
    else  
      DoSteps0(XSteps, YSteps, ZSteps);
  }
}

void OnI2CReceiveEvent(int howMany) {
  int i;

  if (ReceivedCount == -1) {
    while (Wire.available()) {
      if (Wire.read() == 129) {
        Serial.println("STS");
        ReceivedCount = 0;
        break;
      }
    }
  }

  if (ReceivedCount >= 0)  {
    while (Wire.available() && (ReceivedCount <= 2)) {
      i = Wire.read();
      //      i += 120;
      if (i > 127) {
        i -= 256;
        //        i-=120;
      }
      Received[ReceivedCount] = i;
      Serial.println(String(ReceivedCount) + ">>" + String(Received[ReceivedCount]));
      ReceivedCount ++;
    }
  }

  if ((ReceivedCount == 3)&&(Received[0] == -126)&&(Received[1] == -126)&&(Received[2] == -126)) {
    int ZToGo = StepperZ.distanceToGo();
    StepperX.move(0);
    StepperX.run();
    StepperY.move(0);
    StepperY.run();
    StepperZ.move(0);
    StepperZ.run();
    Serial.println("Curent => Target (ToGo)");
    Serial.println(String(ZCurent) + " => " + String(ZTarget)+ " ("+ String(ZToGo)+ ")");
    Serial.println("Stop");
    XTarget = XCurent/* - ZToGo*/;
    YTarget = YCurent/* - ZToGo*/;
    ZTarget = ZCurent/* - ZToGo*/;
    ReceivedCount = -1;
  }

  if (ReceivedCount == 3 &&(Received[0] != -126)&&(Received[1] != -126)&&(Received[2] != -126) ) {
    Serial.println("ETS");
    if (Received[0]!=-125) {
      XTarget = (Received[0] + 120)  * XScale;
      if (XTarget < XMinPosition)
        XTarget = XMinPosition;
      if (XTarget > XMaxPosition)
        XTarget = XMaxPosition;
    }  
    if (Received[1]!=-125) {
      YTarget = (Received[1] + 120)  * YScale;
      if (YTarget < YMinPosition)
        YTarget = YMinPosition;
      if (YTarget > YMaxPosition)
        YTarget = YMaxPosition;
    }  
    if (Received[2]!=-125) {
      ZTarget = (Received[2] + 120) * ZScale;
      if (ZTarget < ZMinPosition)
        ZTarget = ZMinPosition;
      if (ZTarget > ZMaxPosition)
        ZTarget = ZMaxPosition;
    }  
    Serial.println("Curent => Target");
    Serial.println(String(XCurent) + " => " + String(XTarget));
    Serial.println(String(YCurent) + " => " + String(YTarget));
    Serial.println(String(ZCurent) + " => " + String(ZTarget));
    
//    i = StepperZ.distanceToGo();
//    if (XTarget == -126) {
//      XTarget = XCurent;
//      StepperX.move(0);
//      StepperX.run();
//    }
//    if (YTarget == -126) {
//      YTarget = YCurent;
//      StepperY.move(0);
//      StepperY.run();
//    }
//
//    if (Received[2] == -126) {
//      Serial.println("Stop");
//      ZTarget = ZCurent;
//      Serial.println(ZTarget);
//      Serial.println(ZCurent);
//      //      Serial.println(ZSteps);
//      StepperZ.move(0);
//      StepperZ.run();
//
//
//    }

    ReceivedCount = -1;
  }
}
