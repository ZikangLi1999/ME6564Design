// Project Manager
// Course: Complex Ststem
// Author: Zikang Li
// Date: 2023-4-21

#ifndef _MANAGER_
#define _MANAGER_

#include <string.h>
#include <SoftwareSerial.h>
#include "stepper.h"
#include "pushrod.h"

// ##############################################
//               Message Protocol
//
// 1. Messages should be a String-like data.
// 2. One command is represented in ONE line.
// 3. (For now) Arduino only handle the action of
//    stepper and linear actuators; other messages
//    like `add_new_tabulets` will be transmitted
//    to the server.
//
// ##############################################
//
//                   Commands
//
// - `test`
//   Test if the manager works.
//
// - `step:<float trgAngle>`
//   The stepper turns to the target angle.
//
// - `push:<int rodID>:<float trgPosition>`
//    The push rod of given ID runs to target position.
//
// - `sweep:<float trgAngle>`
//
// - `end`
//   End the manager thread, and all the destruction
//   functions will be called.
//
// - Other commands are invalid
//   A string `invalid:<String cmd>` will be returned
// ##############################################

class Manager
{
  private:
    String *cmd;
    int cmdlen = 5;
    char delimiter=':';

    Stepper *stepper;
    PushRod *sweeprod, *outrod;
    SoftwareSerial *bluetooth;

  public:
    Manager(Stepper &stepper,      // Init serial
            PushRod &sweeprod,
            PushRod &outrod,
            SoftwareSerial &bluetooth);
    bool execute(String cmd[], int cmdlen);      // Parse and execute the command
    void run();                    // Receive message from serial & call `parse`
    void send(String msg);         // Send message to other machines
};

String *split(String s, char delimiter, int limit);

#endif
