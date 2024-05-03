# btn-keyboard
Small libraries for Arduino for button debouncing and keyboard matrix decoding


btn usage:

btn enc_s(7 /* GPIO */, true /* pullups enable */, 2 /* debounce count */);
void loop()
{
  static uint32_t enc_tmr = 0;
  if ((millis() - enc_tmr) < 2)
    return;
  enc_tmr = millis();

  btn::state s = enc_s.loop();
  switch(s)
  {
    case btn::state::RELEASE:
      printf("Button was just released\n");
      return;

    case  btn::state::UP:
      printf("Button is not pressed\n");
      return;

    case  btn::state::DOWN:
      printf("Button is down. Count=%i\n", enc_s.getCount());
      return;

    case  btn::state::PRESS:
      printf("Button was just pressed\n");
      return;
    
    default:
    case btn::state::NONE:
      // Nothing to do here
      return;
  }
