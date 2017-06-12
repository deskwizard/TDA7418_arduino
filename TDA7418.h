#ifndef tda7418_h
#define tda7418_h

#include "Arduino.h"
#include "defines.h"

class TDA7418 {

    public:
        TDA7418();

        void init();

        void source(byte _source);
        void diffinmode(byte _mode);

        void volume(byte _volume);
        void volumesoftstep(byte _state);        

        byte softmute();
        void softmute(byte _state);

        void attenuator(int8_t _value);
        void attenuator(uint8_t _channel, int8_t _value);       

    private:
        static byte _register_data[14];

};


#endif
