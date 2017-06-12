#ifndef tda7418_h
#define tda7418_h

#include "Arduino.h"

class TDA7418 {

    public:
        TDA7418();
        void init();
        byte get_sm();
        void set_sm(byte _state);
    private:
        byte _register_data[14] = {0xFE};
};


#endif
