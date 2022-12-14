 #pragma once
 #include "main.h"
 

extern okapi::IterativePosPIDController drivePID;

// //void updateOdom();
 void driveForward(double distance, bool backwards);
 void rotate(double angle);



using namespace okapi;

namespace odom
{
  //extern QVector<QLength> pos;
  //extern QVector<Number> heading;
  //extern QAngle curAngle;
  extern double rPrev;
  extern double lPrev;
  //extern void movePIDOdom(float leftTarget, float rightTarget, int ms, float maxV = 1.0f);
  //extern void gyroPID(float degree, bool CW, int ms = 1000);
  //extern void updateOdom();
  //extern void driveToPoint(Point target, bool driveBack);
  //extern void odomRotate(QAngle targetAngle);
  //extern void odomTranslate(QLength targetDistance, bool driveBack);
  //extern void odomDrift();
}
