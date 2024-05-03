// Define GPIO for keyboard
#define PBC1             6      // Keyboard matrix column 1 (out)
#define PBC2            11      // Keyboard matrix column 2 (out)
#define PBC3            14      // Keyboard matrix column 3 (out)
#define PBC4            17      // Keyboard matrix column 4 (out)
#define PBR1            18      // Keyboard matrix row 1 (in)
#define PBR2            19      // Keyboard matrix row 2 (in)
#define PBR3            20      // Keyboard matrix row 3 (in)
#define PBR4            22      // Keyboard matrix row 4 (in)


// --------------------------------------------------------------------------
// Keyboard handler
// --------------------------------------------------------------------------

kbd keys(4 /* cols */, 4 /* rows */, 2 /* debounce count */);             // 4x4 keyboard matrix
uint32_t buf_tmr = 0;
char buf[PARAM_WIDTH + 1];
char* ptr = buf;


void kbd_task() 
{
  static uint32_t kbd_tmr = 0;
  if ((millis() - kbd_tmr) < 5)
    return;
  kbd_tmr = millis();  

  if ((buf_tmr != 0) && ((millis() - buf_tmr) >= 3000))
  {
    // Reset buf & display after 3 secs w/o keypress
    clear_kbd_buf();
    print_current_param_value(true);
  }

  // Scan the keyboard
  kbd::kbdState r = keys.update();
  int8_t key = keys.getKey();
  static uint8_t mod = 10;

  switch(r)
  {
    case kbd::kbdState::PRESS:
      printf("Key PRESS %c\n", keypress(key);
      break;
    case kbd::kbdState::DOWN:
      printf("Key DOWWN %c Count=%i\n", keypress(key), keys.getCount();
      break;
    case kbd::kbdState::UP:
    case kbd::kbdState::RELEASE:
    case kbd::kbdState::NONE:
    default:
      break;
  }
}

void kbd_init()
{
  // Setup keyboard
  // 1 2 3 U          // U = Up
  // 4 5 6 D          // D = Down
  // 7 8 9 P          // P = Param
  // . 0 E S          // E = Enter, S = Start/Stop
  uint8_t co_pins[] = { PBC1, PBC2, PBC3, PBC4 }; // Keyboard column pins (outputs)
  uint8_t ri_pins[] = { PBR1, PBR2, PBR3, PBR4 }; // Keyboard rown pins (inputs)

  keys.begin(co_pins, ri_pins);
  keys.mapKey(0, 0, '1');
  keys.mapKey(1, 0, '2');
  keys.mapKey(2, 0, '3');
  keys.mapKey(3, 0, 'U');   // Up
  keys.mapKey(0, 1, '4');
  keys.mapKey(1, 1, '5');
  keys.mapKey(2, 1, '6');
  keys.mapKey(3, 1, 'D');   // Down
  keys.mapKey(0, 2, '7');
  keys.mapKey(1, 2, '8');
  keys.mapKey(2, 2, '9');
  keys.mapKey(3, 2, 'P');   // Param
  keys.mapKey(0, 3, '.');
  keys.mapKey(1, 3, '0');
  keys.mapKey(2, 3, 'E');   // Enter
  keys.mapKey(3, 3, 'S');   // Start/Stop
}

// --------------------------------------------------------------------------
// Button
// --------------------------------------------------------------------------


btn enc_s(ENC_S_IPIN, true /* pullups enable */, 2 /* debounce count */);

void btn_task()
{
  // Pressing the button cycles through the parameters.
  // Rolling encoder inc/decs current param's value.
    static uint32_t enc_tmr = 0;
  if ((millis() - enc_tmr) < 2)
    return;
  enc_tmr = millis();

  btn::state s = enc_s.loop();
  switch(s)
  {
    case btn::state::PRESS:
      printf("Button PRESS\n");
      break;
    case btn::state::RELEASE:
      printf("Button RELEASE\n");
      break;
    case btn::state::UP:
      printf("Button UP\n");
      break;
    case btn::state::DOWN:
      printf("Button DOWN Count=%i\n", enc_s.getCount());
      break;
    case btn::state::PRESS:
    defaault:
      // Do nothing
      break
  }

}


// --------------------------------------------------------------------------
// Setup / loop
// --------------------------------------------------------------------------

void setup()
{
  kbd_init();
}

void loop()
{
  kbd_task();
  btn_task();
}

