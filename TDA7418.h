#ifndef tda7418_h
#define tda7418_h

#include "Arduino.h"
#include "defines.h"

class TDA7418 {

    public:
        TDA7418();

        int begin();

        int source(byte _source);
        int inputgain(byte _value);
        int diffinmode(byte _mode);

        int loudnessattenuator(int _value);
        int loudnesscenterfreq(int _freq);
        int loudnessshape(byte _shape);
        int loudnesssoftstep(byte _state);

        int volume(byte _volume);
        int volumesoftstep(byte _state);

        int trebleatt(int _value);
        int treblecenterfreq(int _freq);

        int middleatt(int _value);
        int middlecenterfreq(int _freq);
        int middleqf(byte _factor);
        int middlesoftstep(byte _state);

        int bassatt(int _value);
        int basscenterfreq(byte _freq);
        int bassqf(byte _factor);
        int basssoftstep(byte _state);
        int bassdcmode(byte _state);

        int smoothingfilter(byte _state);        

        int softmute();
        int softmute(byte _state);
        int softmutetime(byte _value);
        int softsteptime(byte _value);
        int autozero(byte _state);

        int testmode(byte _state);
        int testmux(byte _value);
        int schlock(byte _state);
        int mutepincfg(byte _value);

        int attenuator(char _value);
        int attenuator(byte _channel, char _value);       

		// debug
		void printreg(byte _reg);

    private:
        static byte _register_data[14];
        int _write_register(byte _register);
};


#endif
