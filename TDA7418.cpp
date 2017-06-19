#include "Arduino.h"
#include "Wire.h"
#include "TDA7418.h"
#include "defines.h"

#define DEBUG_MODE

byte TDA7418::_register_data[14] = {PWRON_DEFAULT};

TDA7418::TDA7418() {}


// Initialize TDA7418 with Power on defaults (0xFE)
int TDA7418::begin() {

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


int TDA7418::source(byte _source) {

	if (_source > INPUT_MUTE) {
		return -1;
	}

    _register_data[REG_SOURCE_SEL] &= ~MASK_INPUT;
    _register_data[REG_SOURCE_SEL] |= _source & MASK_INPUT;

    return _write_register(REG_SOURCE_SEL);
}


int TDA7418::inputGain(byte _value) {

	if (_value > 15) {
		return -1;
	}

    _register_data[REG_SOURCE_SEL] &= ~MASK_INPUTGAIN;
    _register_data[REG_SOURCE_SEL] |= _value << 3 & MASK_INPUTGAIN;

    return _write_register(REG_SOURCE_SEL);
}


int TDA7418::diffinMode(byte _mode) {

	if (_mode > 1) {
      return -1;
    }

    if (_mode) {
        _register_data[REG_SOURCE_SEL] |= (1 << 7);
    }
    else {
        _register_data[REG_SOURCE_SEL] &= ~(1 << 7);
    }

    return _write_register(REG_SOURCE_SEL);
}


int TDA7418::loudnessAttenuator(int _value) {
    
	
	if (_value < -15 || _value > 0) {
		return -1;
	}

    _value = abs(_value); // ditch sign bit

    _register_data[REG_LOUDNESS] &= ~MASK_LOUDATT;
    _register_data[REG_LOUDNESS] |= _value & MASK_LOUDATT;

	return _write_register(REG_LOUDNESS);
}


int TDA7418::loudnessCenterFreq(int _freq) {

    byte _value = 0;

    switch (_freq) {
        case FLAT:
            _value = 0;
            loudnessShape(LOW);
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
		
		default:
			return -1;
		break;
    }
	
	_register_data[REG_LOUDNESS] &= ~MASK_LOUDFREQ;
    _register_data[REG_LOUDNESS] |= _value << 4 & MASK_LOUDFREQ;

	return _write_register(REG_LOUDNESS);
}


int TDA7418::loudnessShape(byte _shape) {

	if (_shape > 1) {
      return -1;
    }


    if (_shape) {
        _register_data[REG_LOUDNESS] |= (1 << 6);
    }
    else {
        _register_data[REG_LOUDNESS] &= ~(1 << 6);
    }

    return _write_register(REG_LOUDNESS);
}


int TDA7418::loudnessSoftStep(byte _state) {

	if (_state > 1) {
      return -1;
    }

    if (_state) {
        _register_data[REG_LOUDNESS] &= ~(1 << 7);
    }
    else {
        _register_data[REG_LOUDNESS] |= (1 << 7);
    }

    return _write_register(REG_LOUDNESS);
}


int TDA7418::volume(char _volume) {
	return attenuator(REG_VOLUME, _volume);
}


int TDA7418::volumeSoftStep(byte _state) {

	if (_state > 1) {
      return -1;
    }

    if (_state) {
        _register_data[REG_VOLUME] &= ~(1 << 7);
    }
    else {
        _register_data[REG_VOLUME] |= (1 << 7);
    }

    return _write_register(REG_VOLUME);
}


int TDA7418::trebleAttenuator(int _value) {
    int _result;

	if (_value < -15 || _value > 15) {
		return -1;
	}

    if (_value >= 0) {
        _result = MASK_ATTPOS - _value;
    }
    else {
        _result = _value + MASK_ATTNEG;
    }

    _register_data[REG_TREBLE] &= ~MASK_ATT;
    _register_data[REG_TREBLE] |= _result & MASK_ATT;

    return _write_register(REG_TREBLE);
}


int TDA7418::trebleCenterFreq(int _freq) {
    byte _value = 0;

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
            return -1;
        break;
    }

    _register_data[REG_TREBLE] &= ~MASK_TREBLEFREQ;
    _register_data[REG_TREBLE] |= _value << 5 & MASK_TREBLEFREQ;

    return _write_register(REG_TREBLE);
}


int TDA7418::middleAttenuator(int _value) {
    int _result;

	if (_value < -15 || _value > 15) {
		return -1;
	}

    if (_value >= 0) {
        _result = MASK_ATTPOS - _value;
    }
    else {
        _result = _value + MASK_ATTNEG;
    }

    _register_data[REG_MIDDLE] &= ~MASK_ATT;
    _register_data[REG_MIDDLE] |= _result & MASK_ATT;

    return _write_register(REG_MIDDLE);
}


int TDA7418::middleCenterFreq(int _freq) {
    byte _value = 0;

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
            return -1;
        break;
    }

    _register_data[REG_MID_BAS_FC] &= ~MASK_MIDDLEFREQ;
    _register_data[REG_MID_BAS_FC] |= _value & MASK_MIDDLEFREQ;

    return _write_register(REG_MID_BAS_FC);
}


int TDA7418::middleQFactor(byte _factor) {

	if (_factor > MID_QF_125) {
		return -1;
	}

    _register_data[REG_MIDDLE] &= ~MASK_QF;
    _register_data[REG_MIDDLE] |= _factor << 5 & MASK_QF;

    return _write_register(REG_MIDDLE);
}


int TDA7418::middleSoftStep(byte _state) {

	if (_state > 1) {
      return -1;
    }

    if (_state) {
        _register_data[REG_MIDDLE] &= ~(1 << 7);
    }
    else {
        _register_data[REG_MIDDLE] |= (1 << 7);
    }

    return _write_register(REG_MIDDLE);
}


int TDA7418::bassAttenuator(int _value) {
    int _result;

	if (_value < -15 || _value > 15) {
		return -1;
	}

    if (_value >= 0) {
        _result = MASK_ATTPOS - _value;
    }
    else {
        _result = _value + MASK_ATTNEG;
    }

    _register_data[REG_BASS] &= ~MASK_ATT;
    _register_data[REG_BASS] |= _result & MASK_ATT;

    return _write_register(REG_BASS);
}


int TDA7418::bassCenterFreq(byte _freq) {
    byte _value = 0;

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
            return -1;
        break;
    }

    _register_data[REG_MID_BAS_FC] &= ~MASK_BASSFREQ;
    _register_data[REG_MID_BAS_FC] |= _value << 2 & MASK_BASSFREQ;

    return _write_register(REG_MID_BAS_FC);
}


int TDA7418::bassQFactor(byte _factor) {

	if (_factor > BASS_QF_200) {
		return -1;
	}

    _register_data[REG_BASS] &= ~MASK_QF;
    _register_data[REG_BASS] |= _factor << 5 & MASK_QF;

    return _write_register(REG_BASS);
}


int TDA7418::bassSoftStep(byte _state) {

	if (_state > 1) {
      return -1;
    }

    if (_state) {
        _register_data[REG_BASS] &= ~(1 << 7);
    }
    else {
        _register_data[REG_BASS] |= (1 << 7);
    }

    return _write_register(REG_BASS);
}


int TDA7418::bassDCMode(byte _state) {

	if (_state > 1) {
      return -1;
    }

    if (_state) {
        _register_data[REG_MID_BAS_FC] |= (1 << 4);        
    }
    else {
        _register_data[REG_MID_BAS_FC] &= ~(1 << 4);
    }

    return _write_register(REG_MID_BAS_FC);
}


int TDA7418::smoothingFilter(byte _state) {

	if (_state > 1) {
      return -1;
    }

    if (_state) {
        _register_data[REG_MID_BAS_FC] |= (1 << 5);        
    }
    else {
        _register_data[REG_MID_BAS_FC] &= ~(1 << 5);
    }

    return _write_register(REG_MID_BAS_FC);
}


int TDA7418::softMute() {
    byte sm_state;

    Wire.beginTransmission(TDA_ADDR);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(TDA_ADDR, 1);


    if (Wire.available()) {
        sm_state = Wire.read();
    }

    return sm_state;
}


int TDA7418::softMute(byte _state) {

	if (_state > 1) {
      return -1;
    }

    if (_state) { // Softmute On request
        _register_data[REG_SOFTMUTE] &= ~(1 << 0);
    }
    else {
        _register_data[REG_SOFTMUTE] |= (1 << 0);
    }

    return _write_register(REG_SOFTMUTE);
}


int TDA7418::softMuteTime(byte _value) {

	if (_value > SMT_123) {
		return -1;
	}

    _register_data[REG_SOFTMUTE] &= ~MASK_SMT;
    _register_data[REG_SOFTMUTE] |= _value << 1 & MASK_SMT;

    return _write_register(REG_SOFTMUTE);
}


int TDA7418::softStepTime(byte _value) {

	if (_value > SST_2048) {
		return -1;
	}

    _register_data[REG_SOFTMUTE] &= ~MASK_SST;
    _register_data[REG_SOFTMUTE] |= _value << 3 & MASK_SST;

    return _write_register(REG_SOFTMUTE);
}


int TDA7418::autoZero(byte _state) {

	if (_state > 1) {
      return -1;
    }

    if (_state) {
        _register_data[REG_SOFTMUTE] |= (1 << 6);
    }
    else {
        _register_data[REG_SOFTMUTE] &= ~(1 << 6);
    }

    return _write_register(REG_SOFTMUTE);
}


int TDA7418::testMode(byte _state) {

	if (_state > 1) {
      return -1;
    }

    if (_state) {
        _register_data[REG_AUDIO_TEST] |= (1 << 0);
    }
    else {
        _register_data[REG_AUDIO_TEST] &= ~(1 << 0);
    }

    return _write_register(REG_AUDIO_TEST);
}


int TDA7418::testMux(byte _value) {

	if (_value > Clk200kHz) {
		return -1;
	}

    _register_data[REG_AUDIO_TEST] &= ~MASK_TESTMUX;
    _register_data[REG_AUDIO_TEST] |= _value  << 2 & MASK_TESTMUX;

    return _write_register(REG_AUDIO_TEST);
}


int TDA7418::schLock(byte _state) {

	if (_state > 1) {
      return -1;
    }

    if (_state) {
        _register_data[REG_AUDIO_TEST] |= (1 << 6);
    }
    else {
        _register_data[REG_AUDIO_TEST] &= ~(1 << 6);
    }

    return _write_register(REG_AUDIO_TEST);
}


int TDA7418::mutePinConfig(byte _value) {

	if (_value != 0 && _value != 0x80 && _value != 0x82) {
		return -1;
	}

    _register_data[REG_AUDIO_TEST] &= ~MASK_MUTEPIN;
    _register_data[REG_AUDIO_TEST] |= _value & MASK_MUTEPIN;

    return _write_register(REG_AUDIO_TEST);
}


// Set all atenuators in auto-increment mode (batch writes)
int TDA7418::attenuator(char _value) {
    byte _set_att;

	if (_value < -80 || _value > 15) {
		return -1;
	}

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

    return Wire.endTransmission(); // End batch write attenuators 7 - 11
}


// Set single attenuator
int TDA7418::attenuator(byte _channel, char _value) {
    byte _set_att;

	if (_value < -80 || _value > 15) {
		return -1;
	}

    // bits 0x00 - 0x0F -> 0dBto +15dB
    if (_value < VOL_OFFSET && _value >= 0) {
        _set_att = _value;
    }
    else {
        _set_att = VOL_OFFSET + -_value;
    }

    _register_data[_channel] = _set_att;

    return _write_register(_channel);
}


int TDA7418::_write_register(byte _register) {

    Wire.beginTransmission(TDA_ADDR);

    Wire.write(_register);
    Wire.write(_register_data[_register]);

    byte error = Wire.endTransmission();

    return error;
}

void TDA7418::printreg(byte _reg) {
	Serial.print("Reg ");
	Serial.print(_reg);
	Serial.print(" value: ");
	Serial.println(_register_data[_reg], HEX);
}
