#ifndef btn_h
#define btn_h
#include <Arduino.h>

class btn
{
  public:
    enum class state {  // Possible return values from loop()
      NONE    = -1,         // Nothing has happened
      DOWN    = 0b00,       // Button is held down
      RELEASE = 0b01,       // Button just release
      PRESS   = 0b10,       // Button just pressed
      UP      = 0b11,       // Button is up
    };

    btn(uint8_t pin, bool pullup = false, uint16_t debounce_count = 2);
    // Contructor

    btn::state loop();
    // Read button, update debouncer and return state.

    uint16_t getCount();    
    // Returns number of tick since last state change

  private:
    uint8_t _pin;           // The GPIO we are using
    uint16_t _dbnc_cnt;     // How many loops() in same state count as valid
    bool _now;              // Current state
    bool _last;             // Previous state
    uint16_t _count;        // Cleared every state change, otherwise inc'd every loop() call,
};

#endif
