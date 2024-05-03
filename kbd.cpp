#include "kbd.h"

kbd::kbd(uint8_t ncols, uint8_t nrows, uint16_t debounce_count)
{
  // Constructor
  _ncols = ncols;
  _nrows = nrows;
  _dbnc_cnt = debounce_count;
  _cpins = NULL;
  _rpins = NULL;
  _keys = NULL;
  _retVal = NULL;
  _cur_col = 0;
  _this_key = -1;
  _last_key = -1;
}

kbd::~kbd()
{
  // Destructor
  if (_cpins)
    free(_cpins);
  if (_rpins)
    free(_rpins);
  if (_keys)
    free(_keys);
  if (_retVal)
    free(_retVal);
}


int kbd::begin(uint8_t *cpins, uint8_t *rpins)
{
  // Initialzer
  _cpins = (uint8_t *) calloc(_ncols, sizeof(uint8_t));
  if (!_cpins)
  {
    return -1;
  }
  memcpy(_cpins, cpins, _ncols);

  _rpins = (uint8_t *) calloc(_nrows, sizeof(uint8_t));
  if (!_rpins)
  {
    free(_cpins);
    return -1;
  }
  memcpy(_rpins, rpins, _nrows);

  _keys = (bool *) calloc(_nrows * _ncols, sizeof(bool));
  if (!_keys)
  {
    free(_cpins);
    free(_rpins);
    return -1;
  }

  _retVal = (int *) calloc(_nrows * _ncols, sizeof(int));
  if (!_retVal)
  {
    free(_cpins);
    free(_rpins);
    free(_keys);
    return -1;
  }

  int i, j;
  for (i = 0; i < _ncols; i++)
    pinMode(_cpins[i], INPUT_PULLUP);
  for (i = 0; i < _nrows; i++)
    pinMode(_rpins[i], INPUT_PULLUP);

  for (i = 0; i < _nrows; i++)
    for (j = 0; j < _ncols; j++)
      _retVal[i * _ncols + j] = (i * _nrows + j + 1);

  return 0;
}

kbd::kbdState kbd::update()
{
  int r, c;

  // Scan the row
  for (r = 0; r < _nrows; r++)
    _keys[_cur_col * _nrows + r] = !digitalRead(_rpins[r]);

  // Stop driving the current col
  digitalWrite(_cpins[_cur_col], 1);
  pinMode(_cpins[_cur_col], INPUT_PULLUP);

  // Next col
  _cur_col++;
  if (_cur_col >= _ncols)
    _cur_col = 0;

  // Drive new col
  digitalWrite(_cpins[_cur_col], 0);
  pinMode(_cpins[_cur_col], OUTPUT);

  if (_cur_col != 0)
    return (kbdState::NONE);

  // Scanned all rows and cols. Now decode
  _last_key = _this_key;
  _this_key = -1;
  for (c = 0; c < _ncols; c++)
  {
    for (r = 0; r < _nrows; r++)
    {
      if (_keys[c * _nrows + r])
      {
        _this_key = c * _nrows + r;
        break;
      }
    }
  }

  if (_last_key == _this_key)
  {
    if (_debouncer < 0xFFFF)
      _debouncer++;
  }
  else
  {
    _debouncer = 0;
  }

  if (_debouncer < _dbnc_cnt)
  {
    return (kbdState::NONE);
  }
  else if (_debouncer == _dbnc_cnt)
  {
    if (_this_key == -1)
      return(kbdState::RELEASE);
    else
      return(kbdState::PRESS);
  }
  else
  {
    if (_this_key == -1)
      return(kbdState::UP);
    else
      return(kbdState::DOWN);
  }
}

int8_t kbd::getKey()
{
  return(_retVal[_this_key]);
}

uint16_t kbd::getCount()
{
  return(_debouncer);
}

void kbd::mapKey(uint8_t col, uint8_t row, int key)
{
  _retVal[col * _nrows + row] = key;
}