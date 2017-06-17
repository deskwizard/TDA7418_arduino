#include "Arduino.h"
#include "Wire.h"
#include "TDA7418.h"
#include "defines.h"

#define DEBUG_MODE

byte TDA7418::_register_data[14] = {PWRON_DEFAULT};

TDA7418::TDA7418() {}


// Initialize TDA7418 with Power on defaults (0xFE)
byte TDA7418::begin() {

    Wire.beginTransmission(TDA_ADDR);

    // Select register 0 with Auto-Increment enabled
    Wire.write(REG_SOURCE_SEL + AUTOINCREMENT);

    // Batch write all registers and reset register values array to default
    for (byte x = REG_SOURCE_SEL; x <= REG_AUDIO_TEST; x++) {
        _register_data[x] = PWRON_DEFAULT;
        Wire.write(_register_data[x]);
    }
  
    byte error = Wire.endTransmission();
    
    return error;
}

void TDA7418::printreg(byte _reg) {
#ifdef DEBUG_MODE
	Serial.print("Reg ");
	Serial.print(_reg);
	Serial.print(" value: ");
	Serial.println(_register_data[_reg], HEX);
#endif
}

void TDA7418::source(byte _source) {

    _register_data[REG_SOURCE_SEL] &= ~MASK_INPUT;
    _register_data[REG_SOURCE_SEL] |= _source & MASK_INPUT;

    _write_register(REG_SOURCE_SEL);
}

void TDA7418::inputgain(byte _value) {

    _register_data[REG_SOURCE_SEL] &= ~MASK_INPUTGAIN;
    _register_data[REG_SOURCE_SEL] |= _value << 3 & MASK_INPUTGAIN;

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

    _register_data[REG_LOUDNESS] &= ~MASK_LOUDATT;
    _register_data[REG_LOUDNESS] |= _value & MASK_LOUDATT;

	printreg(REG_LOUDNESS);

    _write_register(REG_LOUDNESS);
}

void TDA7418::loudnesscenterfreq(int _freq) {

	printreg(REG_LOUDNESS);

    byte _value = 0;
    _register_data[REG_LOUDNESS] &= ~MASK_LOUDFREQ;

    switch (_freq) {
        case FLAT:
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

    _register_data[REG_LOUDNESS] |= _value << 4 & MASK_LOUDFREQ;

	printreg(REG_LOUDNESS);

    _write_register(REG_LOUDNESS);
}


void TDA7418::loudnessshape(byte _shape) {

    if (_shape) {
        _register_data[REG_LOUDNESS] |= (1 << 6);
    }
    else {
        _register_data[REG_LOUDNESS] &= ~(1 << 6);
    }

	printreg(REG_LOUDNESS);

    _write_register(REG_LOUDNESS);
}


void TDA7418::loudnesssoftstep(byte _state) {

    if (_state) {
        _register_data[REG_LOUDNESS] &= ~(1 << 7);
    }
    else {
        _register_data[REG_LOUDNESS] |= (1 << 7);
    }

	printreg(REG_LOUDNESS);

    _write_register(REG_LOUDNESS);
}


void TDA7418::volume(byte _volume) {
    byte _set_volume;

    // bits 0x00 - 0x0F -> 0dBto +15dB
    if (_volume < VOL_OFFSET && _volume >= 0) {
        _set_volume = _volume;
    }
    else {
        _set_volume = VOL_OFFSET + -_volume;
    }

    _register_data[REG_VOLUME] &= ~MASK_VOLUME;
    _register_data[REG_VOLUME] |= _set_volume & MASK_VOLUME;

    _write_register(REG_VOLUME);
}

void TDA7418::volumesoftstep(byte _state) {

    if (_state) {
        _register_data[REG_VOLUME] &= ~(1 << 7);
    }
    else {
        _register_data[REG_VOLUME] |= (1 << 7);
    }

    _write_register(REG_VOLUME);
}


void TDA7418::trebleatt(int _value) {
    int _result;

	printreg(REG_TREBLE);

    if (_value >= 0) {
        _result = MASK_ATTPOS - _value;
    }
    else {
        _result = _value + MASK_ATTNEG;
    }

    _register_data[REG_TREBLE] &= ~MASK_ATT;
    _register_data[REG_TREBLE] |= _result & MASK_ATT;

    _write_register(REG_TREBLE);

	printreg(REG_TREBLE);
}


void TDA7418::treblecenterfreq(int _freq) {
    byte _value = 0;

	printreg(REG_TREBLE);

    _register_data[REG_TREBLE] &= ~MASK_TREBLEFREQ;

    switch (_freq) {
        case 10000:
            _value = 0;
        break;

        case 12500:
            _value = 1;
        break;

        case 15000:
            _value = 2;
        break;

        case 17500:
            _value = 3;
        break;

        default:
            return;
        break;
    }

    _register_data[REG_TREBLE] |= _value << 5 & MASK_TREBLEFREQ;

    _write_register(REG_TREBLE);

	printreg(REG_TREBLE);
}


void TDA7418::middleatt(int _value) {
    int _result;

    if (_value >= 0) {
        _result = MASK_ATTPOS - _value;
    }
    else {
        _result = _value + MASK_ATTNEG;
    }

    _register_data[REG_MIDDLE] &= ~MASK_ATT;
    _register_data[REG_MIDDLE] |= _result & MASK_ATT;

    _write_register(REG_MIDDLE);

	printreg(REG_MIDDLE);
}

void TDA7418::middlecenterfreq(int _freq) {
    byte _value = 0;

    _register_data[REG_MID_BAS_FC] &= ~MASK_MIDDLEFREQ;

    switch (_freq) {
        case 500:
            _value = 0;
        break;

        case 1000:
            _value = 1;
        break;

        case 1500:
            _value = 2;
        break;

        case 2500:
            _value = 3;
        break;

        default:
            return;
        break;
    }

    _register_data[REG_MID_BAS_FC] |= _value & MASK_MIDDLEFREQ;

    _write_register(REG_MID_BAS_FC);

	printreg(REG_MID_BAS_FC);
}


void TDA7418::middleqf(byte _factor) {

    _register_data[REG_MIDDLE] &= ~MASK_QF;
    _register_data[REG_MIDDLE] |= _factor << 5 & MASK_QF;

    _write_register(REG_MIDDLE);

	printreg(REG_MIDDLE);
}


void TDA7418::middlesoftstep(byte _state) {

    if (_state) {
        _register_data[REG_MIDDLE] &= ~(1 << 7);
    }
    else {
        _register_data[REG_MIDDLE] |= (1 << 7);
    }

    _write_register(REG_MIDDLE);

	printreg(REG_MIDDLE);
}


void TDA7418::bassatt(int _value) {
    int _result;

    if (_value >= 0) {
        _result = MASK_ATTPOS - _value;
    }
    else {
        _result = _value + MASK_ATTNEG;
    }

    _register_data[REG_BASS] &= ~MASK_ATT;
    _register_data[REG_BASS] |= _result & MASK_ATT;

    _write_register(REG_BASS);

	printreg(REG_BASS);
}


void TDA7418::basscenterfreq(byte _freq) {
    byte _value = 0;

    _register_data[REG_MID_BAS_FC] &= ~MASK_BASSFREQ;

    switch (_freq) {
        case 60:
            _value = 0;
        break;

        case 80:
            _value = 1;
        break;

        case 100:
            _value = 2;
        break;

        case 200:
            _value = 3;
        break;

        default:
            return;
        break;
    }

    _register_data[REG_MID_BAS_FC] |= _value << 2 & MASK_BASSFREQ;

    _write_register(REG_MID_BAS_FC);

	printreg(REG_MID_BAS_FC);
}


void TDA7418::bassqf(byte _factor) {

    _register_data[REG_BASS] &= ~MASK_QF;
    _register_data[REG_BASS] |= _factor << 5 & MASK_QF;

    _write_register(REG_BASS);

	printreg(REG_BASS);
}


void TDA7418::basssoftstep(byte _state) {

    if (_state) {
        _register_data[REG_BASS] &= ~(1 << 7);
    }
    else {
        _register_data[REG_BASS] |= (1 << 7);
    }

    _write_register(REG_BASS);

	printreg(REG_BASS);
}

void TDA7418::bassdcmode(byte _state) {

    if (_state) {
        _register_data[REG_MID_BAS_FC] |= (1 << 4);        
    }
    else {
        _register_data[REG_MID_BAS_FC] &= ~(1 << 4);
    }

    _write_register(REG_MID_BAS_FC);

	printreg(REG_MID_BAS_FC);
}


void TDA7418::smoothingfilter(byte _state) {

    if (_state) {
        _register_data[REG_MID_BAS_FC] |= (1 << 5);        
    }
    else {
        _register_data[REG_MID_BAS_FC] &= ~(1 << 5);
    }

    _write_register(REG_MID_BAS_FC);

	printreg(REG_MID_BAS_FC);
}


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
}


void TDA7418::softmute(byte _state) {

    if (_state) { // Softmute On request
        _register_data[REG_SOFTMUTE] &= ~(1 << 0);
    }
    else {
        _register_data[REG_SOFTMUTE] |= (1 << 0);
    }

    _write_register(REG_SOFTMUTE);
}


void TDA7418::softmutetime(byte _value) {

    _register_data[REG_SOFTMUTE] &= ~MASK_SMT;
    _register_data[REG_SOFTMUTE] |= _value << 1 & MASK_SMT;

    _write_register(REG_SOFTMUTE);
}


void TDA7418::softsteptime(byte _value) {
    _register_data[REG_SOFTMUTE] &= ~MASK_SST;
    _register_data[REG_SOFTMUTE] |= _value << 3 & MASK_SST;

    _write_register(REG_SOFTMUTE);
}


void TDA7418::autozero(byte _state) {

    if (_state) {
        _register_data[REG_SOFTMUTE] &= ~(1 << 6);
    }
    else {
        _register_data[REG_SOFTMUTE] |= (1 << 6);
    }

    _write_register(REG_SOFTMUTE);
}


void TDA7418::testmode(byte _state) {

    if (_state) {
        _register_data[REG_AUDIO_TEST] |= (1 << 0);
    }
    else {
        _register_data[REG_AUDIO_TEST] &= ~(1 << 0);
    }

    _write_register(REG_AUDIO_TEST);
}


void TDA7418::testmux(byte _value) {

    _register_data[REG_AUDIO_TEST] &= ~MASK_TESTMUX;
    _register_data[REG_AUDIO_TEST] |= _value  << 2 & MASK_TESTMUX;

    _write_register(REG_AUDIO_TEST);
}


void TDA7418::schlock(byte _state) {

    if (_state) {
        _register_data[REG_AUDIO_TEST] |= (1 << 6);
    }
    else {
        _register_data[REG_AUDIO_TEST] &= ~(1 << 6);
    }

    _write_register(REG_AUDIO_TEST);
}

void TDA7418::mutepincfg(byte _value) {

    _register_data[REG_AUDIO_TEST] &= ~MASK_MUTEPIN;
    _register_data[REG_AUDIO_TEST] |= _value & MASK_MUTEPIN;

    _write_register(REG_AUDIO_TEST);
}


// Set all atenuators in auto-increment mode (batch writes)
void TDA7418::attenuator(char _value) {
    byte _set_att;

    // bits 0x00 - 0x0F -> 0dBto +15dB
    if (_value < VOL_OFFSET && _value >= 0) {
        _set_att = _value;
    }
    else {
        _set_att = VOL_OFFSET + -_value;
    }

    // Batch write attenuators 7 - 11
    Wire.beginTransmission(TDA_ADDR);
    Wire.write(REG_SPK_ATT_FL + AUTOINCREMENT);

    // Write the 5 attenuators with the same value
    for (byte x = REG_SPK_ATT_FL; x <= REG_SPK_ATT_SUB; x++) {
        _register_data[x] = _set_att;
        Wire.write(_register_data[x]);
    }

    Wire.endTransmission(); // End batch write attenuators 7 - 11

#ifdef DEBUG_MODE
    Serial.print("Sent global data: 0x");
    Serial.println(_set_att, HEX);
#endif
}


// Set single attenuator
void TDA7418::attenuator(byte _channel, char _value) {
    byte _set_att;

    // bits 0x00 - 0x0F -> 0dBto +15dB
    if (_value < VOL_OFFSET && _value >= 0) {
        _set_att = _value;
    }
    else {
        _set_att = VOL_OFFSET + -_value;
    }

    _register_data[_channel] = _set_att;

    _write_register(_channel);

#ifdef DEBUG_MODE
    Serial.print("Sent data: 0x");
    Serial.println(_set_att, HEX);
#endif
}


byte TDA7418::_write_register(byte _register) {

    Wire.beginTransmission(TDA_ADDR);

    Wire.write(_register);
    Wire.write(_register_data[_register]);

    byte error = Wire.endTransmission();

    return error;
}
