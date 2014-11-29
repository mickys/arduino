/*
 * DCMotor library for Arduino.
 * @author Micky Socaci <micky@nowlive.ro>
 */

#include "Arduino.h"
#include "DCMotor.h"

#define MOTOR_STOP             0x45
#define MOTOR_FORWARDS         0x46
#define MOTOR_BACKWARDS        0x47

/*
 * Constructor.
 */
DCMotor::DCMotor(int _id, int _pin1, int _pin2) {
  state = MOTOR_STOP;
  stateO1 = MOTOR_STOP;
  stateO2 = MOTOR_STOP;
  
  id = _id;
  // set internals
  min = 0;
  max = 360;
  
  min_set = false;
  max_set = false;
  current_set = false;
  
  ready = false;

  // Assign variables.
  pin1 = _pin1;
  pin2 = _pin2;

  // Set pins to output.
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  // Set pins low
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

int DCMotor::getId() {
    return id;
}

void DCMotor::forwards() {
  stop(true);
  state = MOTOR_FORWARDS;
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
}

void DCMotor::backwards() {
  stop(true);
  state = MOTOR_BACKWARDS;
  digitalWrite(pin2, LOW);
  digitalWrite(pin1, HIGH);
}

void DCMotor::stop() {
  state = MOTOR_STOP;
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  lightToggle = false;
  saveState(); 
}

void DCMotor::stop(bool internal) {
  state = MOTOR_STOP;
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  lightToggle = false;
}

void DCMotor::setRange(int _min, int _max, int _current) {
  min = _min;
  max = _max;
  current = _current;
  ready = true;
}

void DCMotor::setMin(int _min)
{
     min_set = true;
     min = _min;
     if(max_set && current_set) { ready = true; }
}

void DCMotor::setMax(int _max)
{
     max_set = true;
     max = _max;
     if(min_set && current_set) { ready = true; }
}

void DCMotor::setCurrent(int _current)
{
     current_set = true;
     current = _current;
     if(min_set && max_set) { ready = true; }
}

int DCMotor::getCounter() {
    return current;
}

int DCMotor::getState() {
    return state;
}

int DCMotor::getStateO1() {
    return stateO1;
}

int DCMotor::getStateO2() {
    return stateO2;
}

void DCMotor::process() {
     // if state is stopped
     if (state == MOTOR_STOP)
     {
         // nothing to do here
         // saveState(); 
     }
     else if (state == MOTOR_FORWARDS)
     {
          lightToggle = true;
          current++;
          if(current >= max)
          {
              // we have reached the end! so stop the motor!
              stop(); // stop and save state
          }
     }
     else if (state == MOTOR_BACKWARDS)
     {
          lightToggle = true;
          current--;
          if(current <= min)
          {
              // we have reached the beginning! so stop the motor!
              stop(); // stop and save state
          }
     }
}


int DCMotor::getMin()
{
    return min;
}

int DCMotor::getMax()
{
    return max;
}

int DCMotor::getCurrent()
{
    return current;
}

void DCMotor::saveState() {
     stateO2 = stateO1;
     stateO1 = state;
     save_state = true;
}

bool DCMotor::getSaveState() {
     return save_state;
}

bool DCMotor::getLightToggle() {
     return lightToggle;
}

void DCMotor::saveDone() {
     save_state = false;
     lightToggle = false;
}

