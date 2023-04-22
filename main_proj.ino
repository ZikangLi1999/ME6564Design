// Main program
// Course: Complex Ststem
// Author: Zikang Li
// Date: 2023-4-18

#include <Arduino.h>
#include <TaskScheduler.h>
#include <SoftwareSerial.h>
#include "stepper.h"
#include "pushrod.h"
#include "manager.h"

// ##############################################
//                 Global Settings
//
// This part defines global variables and constants
// like pin numbers and parameters of the linear
// actuators.
// ##############################################
// Stepper
const int dir_pin = 2;
const int pul_pin = 3;
const int en_pin = 4;

// Linear Actuator 1 (Sweeping push rod)
const int rod1_pos_pin = 5;                   // Ensure the ros pushed
const int rod1_neg_pin = 6;                   //   when pos_pin is HIGH and neg_pin is LOW
const float rod1_max_pos = 20.;
const float rod1_v = 25.;

// Linear Actuator 2 (Output push rod)
const int rod2_pos_pin = 7;                   // Same as linear actuator 1
const int rod2_neg_pin = 8;
const float rod2_max_pos = 25.;
const float rod2_v = 30.;

// Bluetooth
const int BT_RX = 10;                         // Define the pins of bluetooth HC-05
const int BT_TX = 11;

// Box structure parameters


// ##############################################
//                     Setup
// ##############################################
void setup() {
  // Serial init
  Serial.begin(9600);

  // Stepper init
  Stepper stepper(dir_pin, pul_pin, en_pin);
  // stepper.RunToAngle(30.0);
  // delay(1000);
  // stepper.RunToAngle(60.0);
  // delay(1000);
  // stepper.RunToAngle(0.0);

  // Push rod 1 init
  PushRod sweepRod(rod1_pos_pin, rod1_neg_pin, rod1_max_pos, rod1_v);
  // sweepRod.RunForTime(-1, 1000.);
  // sweepRod.RunForTime(1, 1000.);
  // digitalWrite(rod1_pos_pin, LOW);
  // digitalWrite(rod1_neg_pin, HIGH);
  // delay(1000);
  // sweepRod.RunToPosition(15.);
  // sweepRod.RunToPosition(0.);

  // Push rod 2 init
  PushRod outputRod(rod2_pos_pin, rod2_neg_pin, rod2_max_pos, rod2_v);

  // Bluetooth
  SoftwareSerial bluetooth(BT_RX, BT_TX);

  // Manager init
  Manager mgr(stepper, sweepRod, outputRod, bluetooth);
  mgr.run();
  delay(1000);

  // Test sweep
  // Serial.println("Hello");
  // sweep(90., stepper, sweepRod);
  // delay(5000);
}

// ##############################################
//                 Main Loop
// ##############################################
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello");
  // sweep(90., stepper, sweepRod);
  delay(5000);
}

// ##############################################
//                 Sub-threads
// ##############################################
void sweep(float trgAngle, Stepper stepper, PushRod pushrod)
{
  // float startAngle, endAngle;

  Task moveRodTask(1000000, 1, &pushrodSub);
  Task rotateStepperTask(1000000, 1, &stepperSub);

  Scheduler runner;
  runner.init();
  runner.addTask(moveRodTask);
  runner.addTask(rotateStepperTask);

  moveRodTask.enable();
  rotateStepperTask.enable();

  runner.execute();
}

void stepperSub(Stepper stepper)
{
  // stepper.RunToAngle(180.);
  // stepper.RunToAngle(0.);
  Serial.println("stepperSub");
}

void pushrodSub(PushRod pushrod)
{
  // pushrod.RunToPosition(10.);
  // pushrod.RunToPosition(0.);
  Serial.println("pushrodSub");
}
