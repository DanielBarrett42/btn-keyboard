#include "btn.h"

btn::btn(uint8_t pin, bool pullup, uint16_t debounce_count)
{
  _pin = pin;

  if (pullup)
    pinMode(_pin, INPUT_PULLUP);
  else
    pinMode(_pin, INPUT);

  _dbnc_cnt = debounce_count;
  _now = _last = false; 
  _count = 0;
}


btn::state btn::loop()
{
  _last = _now;
  _now = digitalRead(_pin);
  if (_now != _last)
  {
    _count = 0;
  }
  else
  {
    if (_count < 0xFFFF)
      _count++;
  }

  if (_count < _dbnc_cnt)
  {
    // Button is still bouncing
    return (state::NONE);
  }
  else if (_count == _dbnc_cnt)
  {
    // Count is at debounce threshold
    if (_now)
      return(state::RELEASE);
    else
      return (state::PRESS);
  }
  else
  {
    // Count is above debounce threshold
    if (_now)
      return(state::UP);
    else
      return (state::DOWN);
  }

}

uint16_t btn::getCount()
{
  return _count;
}
