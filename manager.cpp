// Project Manager
// Course: Complex Ststem
// Author: Zikang Li
// Date: 2023-4-21

#include <Arduino.h>
#include <string.h>
#include "manager.h"

Manager::Manager(Stepper &stp, PushRod &swr, PushRod &opr, SoftwareSerial &bt)
{
  this->stepper = &stp;
  this->sweeprod = &swr;
  this->outrod = &opr;
  this->bluetooth = &bt;

  // this->delimiter = ":";

  if ( !( Serial.available()) )
  {
    Serial.begin(9600);
  }
  this->bluetooth->begin(9600);
}


void Manager::run()
{
  char dataBuffer;
  String msg = "";

  while (1)
  {
    bool isEnd = false;

    if (Serial.available()) {
      msg = Serial.readString();
      // msg = msg.substring(0, msg.length()-1);          // Strip the right '\n'
      cmd = split(msg, this->delimiter, cmdlen);
      isEnd = execute(cmd, cmdlen);
    }

    if (bluetooth->available()) {
      msg = "";
      while (1) {
        dataBuffer = bluetooth->read();
        if ( (int)(dataBuffer) == 10 ) break;
        else msg += dataBuffer;
      }
      cmd = split(msg, this->delimiter, cmdlen);
      isEnd = execute(cmd, cmdlen);
    }

    if (isEnd) break;
    delay(2000);
  }
}

bool Manager::execute(String cmd[], int cmdlen)
{
  // for (int i=0; i<cmdlen; i++) {
  //   Serial.println(cmd[i]);
  // }
  bool isEnd = false;

  if ( cmd[0].equals("step") ) {                       // cmd `step:<float trgAngle>`
    float trgAngle = (float) ( cmd[1].toFloat() );
    this->stepper->RunToAngle(trgAngle);
  }
  else if ( cmd[0].equals("push") ) {                  // cmd `push:<int rodID>:<float position>`
    if ( cmd[1].equals("1") ) {
      float pos = (float) ( cmd[2].toFloat() );
      this->sweeprod->RunToPosition(pos);
    }
    else if ( cmd[1].equals("2") ) {
      float pos = (float) ( cmd[2].toFloat() );
      this->outrod->RunToPosition(pos);
    }
    else {
      Serial.println("invalid:"+cmd[0]+this->delimiter+cmd[1]);
    }
  }
  else if ( cmd[0].equals("sweep") ) {                 // cmd `sweep:<float trgAngle>`
    //
  }
  else if ( cmd[0].equals("test") ) {                  // cmd `test`
    Serial.println("Manager works!");                  //   Test if the whole process works
  }
  else if ( cmd[0].equals("end") ) {                   // cmd `end`
    isEnd = true;
  }
  else {                                               // invalid cmds
    Serial.print("invalid:");
    for (int i=0; i<cmdlen; i++) {
      Serial.print(this->delimiter + cmd[i]);
    }
    Serial.println("");
  }

  return isEnd;
}

void Manager::send(String msg)
{

}

// ##############################################
//              Auxiliary Functions
// ##############################################
String *split(String s, char delimiter, int limit) {
  String *res;
  res = new String[limit];

  int resIdx, slen;
  resIdx = 0;
  slen = s.length();
  for (int i=0; i<slen; i++)
  {
    if (s[i] == delimiter) {
      resIdx += 1;
    }
    else {
      res[resIdx] += s[i];
    }
  }

  return res;
}
