#ifndef tda7418_h
#define tda7418_h

#include "Arduino.h"
#include "defines.h"

class TDA7418 {

    public:
        TDA7418();

        int begin();

        int source(byte _source);
        int inputGain(byte _value);
        int diffinMode(byte _mode);

        int loudnessAttenuator(int _value);
        int loudnessCenterFreq(int _freq);
        int loudnessShape(byte _shape);
        int loudnessSoftStep(byte _state);

        int volume(int _volume);
        int volumeSoftStep(byte _state);

        int trebleAttenuator(int _value);
        int trebleCenterFreq(int _freq);

        int middleAttenuator(int _value);
        int middleCenterFreq(int _freq);
        int middleQFactor(byte _factor);
        int middleSoftStep(byte _state);

        int bassAttenuator(int _value);
        int bassCenterFreq(byte _freq);
        int bassQFactor(byte _factor);
        int bassSoftStep(byte _state);
        int bassDCMode(byte _state);

        int smoothingFilter(byte _state);        

        int softMute();
        int softMute(byte _state);
        int softMuteTime(byte _value);
        int softStepTime(byte _value);
        int autoZero(byte _state);

        int testMode(byte _state);
        int testMux(byte _value);
        int schLock(byte _state);
        int mutePinConfig(byte _value);

        int attenuator(char _value);
        int attenuator(byte _channel, char _value);       

		// debug
		void printreg(byte _reg);

    private:
        static byte _register_data[14];
        int _write_register(byte _register);
};


#endif
