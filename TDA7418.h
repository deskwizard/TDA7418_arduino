#ifndef tda7418_h
#define tda7418_h

#include "Arduino.h"
#include "defines.h"

class TDA7418 {

    public:
        TDA7418();

        byte begin();

        void source(byte _source);
        void inputgain(byte _value);
        void diffinmode(byte _mode);

        void loudnessattenuator(int _value);
        void loudnesscenterfreq(int _freq);
        void loudnessshape(byte _shape);
        void loudnesssoftstep(byte _state);

        void volume(byte _volume);
        void volumesoftstep(byte _state);

        void trebleatt(int _value);
        void treblecenterfreq(int _freq);

        void middleatt(int _value);
        void middlecenterfreq(int _freq);
        void middleqf(byte _factor);
        void middlesoftstep(byte _state);

        void bassatt(int _value);
        void basscenterfreq(byte _freq);
        void bassqf(byte _factor);
        void basssoftstep(byte _state);
        void bassdcmode(byte _state);

        void smoothingfilter(byte _state);        

        byte softmute();
        void softmute(byte _state);
        void softmutetime(byte _value);
        void softsteptime(byte _value);
        void autozero(byte _state);

        void testmode(byte _state);
        void testmux(byte _value);
        void schlock(byte _state);
        void mutepincfg(byte _value);

        void attenuator(char _value);
        void attenuator(byte _channel, char _value);       

    private:
        static byte _register_data[14];
        byte _write_register(byte _register);

};


#endif
