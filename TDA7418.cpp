#include "Arduino.h"
#include "Wire.h"
#include "TDA7418.h"
#include "TDA7418_reg.h"

#define DEBUG_MODE

byte TDA7418::_register_data[14] = {0xFE};

TDA7418::TDA7418() {}


// Initialize TDA7418 with Power on defaults (0xFE)
void TDA7418::init() {

    Wire.beginTransmission(TDA_ADDR);

    // Select register 0 with Auto-Increment enabled
    Wire.write(REG_SOURCE_SEL + 0x20);

    // Batch write all registers and reset register values array to default
    for (byte x = 0; x <= 0x0D; x++) {
        _register_data[x] = 0xFE;
        Wire.write(_register_data[x]);
    }
  
    Wire.endTransmission();
}

// Get and returns the soft mute state
byte TDA7418::get_sm() {
  byte sm_state;

  Wire.beginTransmission(TDA_ADDR);
  Wire.write(0x00);
  Wire.endTransmission();

  Wire.requestFrom(TDA_ADDR, 1);

#ifdef DEBUG_MODE
  if (Wire.available()) {
    sm_state = Wire.read();

    Serial.print("Soft Mute state: ");
    if (!sm_state) {
      Serial.println("Off");
    }
    else {
      Serial.println("On");
    }
  }
#endif

  return sm_state;
} // get_sm

// Set Soft Mute
void TDA7418::set_sm(byte _state) {
  // Beware, softmute is active low

  if (_state) { // Softmute On request

    _register_data[REG_SOFTMUTE] &= ~(1 << 0);

    Wire.beginTransmission(TDA_ADDR);
    Wire.write(REG_SOFTMUTE);
    Wire.write(_register_data[REG_SOFTMUTE]);
    Wire.endTransmission();
  }
  else {

    _register_data[REG_SOFTMUTE] |= (1 << 0);

    Wire.beginTransmission(TDA_ADDR);
    Wire.write(REG_SOFTMUTE);
    Wire.write(_register_data[REG_SOFTMUTE]);
    Wire.endTransmission();
  }
} //set_sm
