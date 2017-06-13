#include "Arduino.h"
#include "Wire.h"
#include "TDA7418.h"
#include "defines.h"

#define DEBUG_MODE

byte TDA7418::_register_data[14] = {0xFE};

TDA7418::TDA7418() {}


// Initialize TDA7418 with Power on defaults (0xFE)
void TDA7418::begin() {

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

void TDA7418::source(byte _source) {

    _register_data[REG_SOURCE_SEL] &= ~0x07;

    _register_data[REG_SOURCE_SEL] |= _source & 0x07;

    _write_register(REG_SOURCE_SEL);
}

void TDA7418::inputgain(byte _value) {

    _register_data[REG_SOURCE_SEL] &= ~0x78;

    _register_data[REG_SOURCE_SEL] |= _value & 0x78;

    _write_register(REG_SOURCE_SEL);
}

void TDA7418::diffinmode(byte _mode) {

    if (_mode) {
        _register_data[REG_SOURCE_SEL] |= (1 << 7);
    }
    else {
        _register_data[REG_SOURCE_SEL] &= ~(1 << 7);
    }
    _write_register(REG_SOURCE_SEL);
}

void TDA7418::loudnessattenuator(int _value) {
    
    _value = abs(_value); // ditch sign bit

    _register_data[REG_LOUDNESS] &= ~0x0F;
    _register_data[REG_LOUDNESS] |= _value & 0x0F;

    _write_register(REG_LOUDNESS);
}

void TDA7418::loudnesscenterfreq(int _freq) {

    byte _value = 0;
    _register_data[REG_LOUDNESS] &= ~0x30;

    switch (_freq) {
        case OFF:
            _value = 0;
            loudnessshape(LOW);
        break;

        case 400:
            _value = 1;
        break;

        case 800:
            _value = 2;
        break;

        case 2400:
            _value = 3;
        break;
    }

    _register_data[REG_LOUDNESS] |= _value & 0x30;
    _write_register(REG_LOUDNESS);
}


void TDA7418::loudnessshape(byte _shape) {

    if (_shape) {
        _register_data[REG_LOUDNESS] |= (1 << 6);
    }
    else {
        _register_data[REG_LOUDNESS] &= ~(1 << 6);
    }

    _write_register(REG_LOUDNESS);
}


void TDA7418::loudnesssoftstep(byte _state) {

    if (_state) {
        _register_data[REG_LOUDNESS] &= ~(1 << 7);
    }
    else {
        _register_data[REG_LOUDNESS] |= (1 << 7);
    }

    _write_register(REG_LOUDNESS);
} // set LSS


void TDA7418::volume(byte _volume) {

    uint8_t _set_volume;

    // bits 0x00 - 0x0F -> 0dBto +15dB
    if (_volume < 0x10 && _volume >= 0) {
        _set_volume = _volume;
    }
    else {
        _set_volume = 0x10 + -_volume;
    }

#ifdef DEBUG_MODE
    Serial.print("Volume sent: ");
    Serial.println(_set_volume, HEX);
#endif

    _register_data[REG_VOLUME] &= ~0x7F;
    _register_data[REG_VOLUME] |= _set_volume & 0x7F;
    _write_register(REG_VOLUME);
} // set volume

void TDA7418::volumesoftstep(byte _state) {

    if (_state) {
        _register_data[REG_VOLUME] &= ~(1 << 7);
    }
    else {
        _register_data[REG_VOLUME] |= (1 << 7);
    }
    _write_register(REG_VOLUME);
} // set VSS


void TDA7418::trebleatt(int _value) {
    int _result;
    
//    Serial.print("Att req: ");
//    Serial.println(_value);

    if (_value >= 0) {
        _result = 0x1F - _value;
    }
    else {
        _result = _value + 0x0F;
    }
    
//    Serial.print("Att sent: ");
//    Serial.println(_result, HEX);

    // bitmask the internal variable
    _register_data[REG_TREBLE] &= ~0x1F;
    _register_data[REG_TREBLE] |= _result & 0x1F;

    // Write to I2c Bus
    _write_register(REG_TREBLE);

//    Serial.print("Att written: ");
//    Serial.println(_register_data[REG_TREBLE], HEX);
}


void TDA7418::treblecenterfreq(int _freq) {

    byte _value = 0;
    _register_data[REG_TREBLE] &= ~0x60;

    switch (_freq) {
        case 10000:
            _value = 0;
        break;

        case 12500:
            _value = 1;
        break;

        case 15000:
            _value = 3;
        break;

        case 17500:
            _value = 4;
        break;

        default:
            return;
        break;
    }

    _register_data[REG_TREBLE] |= _value & 0x60;
    _write_register(REG_TREBLE);
}


void TDA7418::middleatt(int _value) {
    int _result;

    if (_value >= 0) {
        _result = 0x1F - _value;
    }
    else {
        _result = _value + 0x0F;
    }

    // bitmask the internal variable
    _register_data[REG_MIDDLE] &= ~0x1F;
    _register_data[REG_MIDDLE] |= _result & 0x1F;

    // Write to I2c Bus
    _write_register(REG_MIDDLE);
}

void TDA7418::middlecenterfreq(int _freq) {

    byte _value = 0;
    _register_data[REG_MID_BAS_FC] &= ~0x03;

    switch (_freq) {
        case 500:
            _value = 0;
        break;

        case 1000:
            _value = 1;
        break;

        case 1500:
            _value = 3;
        break;

        case 2500:
            _value = 4;
        break;

        default:
            return;
        break;
    }

    _register_data[REG_MID_BAS_FC] |= _value & 0x03;
    _write_register(REG_MID_BAS_FC);
}


void TDA7418::middleqf(byte _factor) {

    _register_data[REG_MIDDLE] &= ~0x60;
    _register_data[REG_MIDDLE] |= _factor & 0x60;
    _write_register(REG_MIDDLE);
}


void TDA7418::middlesoftstep(byte _state) {

    if (_state) {
        _register_data[REG_MIDDLE] &= ~(1 << 7);
    }
    else {
        _register_data[REG_MIDDLE] |= (1 << 7);
    }
    _write_register(REG_MIDDLE);
} // set MSS


void TDA7418::bassatt(int _value) {
    int _result;

    if (_value >= 0) {
        _result = 0x1F - _value;
    }
    else {
        _result = _value + 0x0F;
    }

    // bitmask the internal variable
    _register_data[REG_BASS] &= ~0x1F;
    _register_data[REG_BASS] |= _result & 0x1F;

    // Write to I2c Bus
    _write_register(REG_BASS);
}


void TDA7418::basscenterfreq(byte _freq) {

    byte _value = 0;
    _register_data[REG_MID_BAS_FC] &= ~0x0C;

    switch (_freq) {
        case 60:
            _value = 0;
        break;

        case 80:
            _value = 1;
        break;

        case 100:
            _value = 3;
        break;

        case 200:
            _value = 4;
        break;

        default:
            return;
        break;
    }

    _register_data[REG_MID_BAS_FC] |= _value & 0x0C;
    _write_register(REG_MID_BAS_FC);
}


void TDA7418::bassqf(byte _factor) {

    _register_data[REG_BASS] &= ~0x60;
    _register_data[REG_BASS] |= _factor & 0x60;
    _write_register(REG_BASS);
}


void TDA7418::basssoftstep(byte _state) {

    if (_state) {
        _register_data[REG_BASS] &= ~(1 << 7);
    }
    else {
        _register_data[REG_BASS] |= (1 << 7);
    }
    _write_register(REG_BASS);
} // set MSS

void TDA7418::bassdcmode(byte _state) {

    if (_state) {
        _register_data[REG_MID_BAS_FC] &= ~(1 << 4);
    }
    else {
        _register_data[REG_MID_BAS_FC] |= (1 << 4);
    }
    _write_register(REG_MID_BAS_FC);
} // set Bass DC mode


void TDA7418::smoothingfilter(byte _state) {

    if (_state) {
        _register_data[REG_MID_BAS_FC] &= ~(1 << 5);
    }
    else {
        _register_data[REG_MID_BAS_FC] |= (1 << 5);
    }
    _write_register(REG_MID_BAS_FC);
} // set Bass DC mode


// Get and returns the soft mute state
byte TDA7418::softmute() {
    byte sm_state;

    Wire.beginTransmission(TDA_ADDR);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(TDA_ADDR, 1);


    if (Wire.available()) {
        sm_state = Wire.read();

#ifdef DEBUG_MODE
        Serial.print("Soft Mute state: ");

        if (!sm_state) {
            Serial.println("Off");
        }
        else {
            Serial.println("On");
        }
#endif
    }

    return sm_state;
} // get softmute


// Set Soft Mute
void TDA7418::softmute(byte _state) {

    if (_state) { // Softmute On request
        _register_data[REG_SOFTMUTE] &= ~(1 << 0);
    }
    else {
        _register_data[REG_SOFTMUTE] |= (1 << 0);
    }

    _write_register(REG_SOFTMUTE);
} // set softmute


// Set all atenuators in auto-increment mode (batch writes)
void TDA7418::attenuator(int8_t _value) {
    uint8_t _set_att;

    // bits 0x00 - 0x0F -> 0dBto +15dB
    if (_value < 0x10 && _value >= 0) {
        _set_att = _value;
    }
    else {
        _set_att = 0x10 + -_value;
    }

    // Batch write attenuators 7 - 11
    Wire.beginTransmission(TDA_ADDR);
    Wire.write(REG_SPK_ATT_FL + 0x20);

    // Write the 5 attenuators with the same value
    for (byte x = 0x07; x <= 0x0B; x++) {
        _register_data[x] = _set_att;
        Wire.write(_register_data[x]);
    }

    Wire.endTransmission(); // End batch write attenuators 7 - 11

#ifdef DEBUG_MODE
    Serial.print("Sent global data: 0x");
    Serial.println(_set_att, HEX);
#endif
} // Set all attenuators


// Set single attenuator
void TDA7418::attenuator(uint8_t _channel, int8_t _value) {
    uint8_t _set_att;

    // bits 0x00 - 0x0F -> 0dBto +15dB
    if (_value < 0x10 && _value >= 0) {
        _set_att = _value;
    }
    else {
        _set_att = 0x10 + -_value;
    }

    _register_data[_channel] = _set_att;
    _write_register(_channel);

#ifdef DEBUG_MODE
    Serial.print("Sent data: 0x");
    Serial.println(_set_att, HEX);
#endif
} // Set single attenuator


// Pretty much self-explanatory
byte TDA7418::_write_register(byte _register) {
    Wire.beginTransmission(TDA_ADDR);
    Wire.write(_register);
    Wire.write(_register_data[_register]);
    byte error = Wire.endTransmission();
    return error;
}
