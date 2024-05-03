#ifndef _KBD_H_
#define _KBD_H_
#include <Arduino.h>

class kbd
{
  public:
    enum class kbdState
    {
      // Possible return values from update().
      NONE    = -1,         // Nothing has happened
      DOWN    = 0b00,       // A key is being held down
      RELEASE = 0b01,       // A key has been released
      PRESS   = 0b10,       // A key has been pressed
      UP      = 0b11,       // No keys are pressed/down
    };

    kbd(uint8_t ncols, uint8_t nrows, uint16_t debounce_count = 2);
    // Call constructor with the size of the keyboard matrix.

    ~kbd();
    // Destructor to free allocaed arrays

    int begin(uint8_t *cpins, uint8_t *rpins);
    // Call begin with arrays of column and row pins.
    // Size must match what was passed to constructor.
    // Returns 0 on success, -1 on (malloc) error.

    kbd::kbdState update();
    // Call periodically to update the keyboard state machine.
    // Return value is one of the kbdStates define above.

    int8_t getKey();
    // Returns the number of the key currently pressed.
    // Only valid right after the last call to update.
    // which returned kbdState::PRESS or kbdState::DOWN

    uint16_t getCount();
    // Returns the number of updates() keyboard has
    // been in current state.
    // Only valid right after the last call to update.

    void mapKey(uint8_t col, uint8_t row, int key);
    // Set the return value for getKey for the key at row:col.
    // If not mapped otherwise, return value will be
    // col * nrows + row

  private:
    uint8_t   _ncols;       // Number of cols in the matrix
    uint8_t   _nrows;       // Number of rows in the matrix
    uint32_t  _poll_ms;     // How often to poll
    uint16_t  _dbnc_cnt;    // How many polls with same result to declare valid
    uint8_t   *_cpins;      // Col pins are open-drain outputs w/pullups
    uint8_t   *_rpins;      // Row pins are inputs w/pullup
    bool      *_keys;       // update interval
    int       *_retVal;     // Return value for key at row:col
    uint8_t   _cur_col;     // Current row in scanning state machine
    int8_t    _this_key;    // The last key pressed or -1 if none
    int8_t    _last_key;    // Previous state of _this_key
    uint16_t  _debouncer;   // Counter for debouncer
};

#endif
