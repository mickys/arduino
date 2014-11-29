/*
 * DCMotor library for Arduino.
 * @author Micky Socaci <micky@nowlive.ro>
 */

#ifndef DCMotor_h
#define DCMotor_h

#include "Arduino.h"
#include "EDB.h"


class DCMotor
{
  public:
    DCMotor(int, int, int);
    void forwards();
    void backwards();
    void stop();
    void stop(bool);
    int getId();
    void setRange(int, int, int);
    int getCounter();
    void process();
    
    int getState();
    int getStateO1();
    int getStateO2();
    
    void setMin(int);
    void setMax(int);
    void setCurrent(int);

    int getMin();
    int getMax();
    int getCurrent();
    
    bool getSaveState();
    bool getLightToggle();
    void saveDone();

  private:
    void saveState();
          
    byte id;
    char pin1;
    char pin2;
    int min;
    int max;
    int current;
    bool ready;
    int state;
    bool min_set;
    bool max_set;
    bool current_set;
    bool save_state;
    bool lightToggle;
    
    int stateO1;
    int stateO2;
};

#endif

