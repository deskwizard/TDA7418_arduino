#ifndef tda7418_reg_h
#define tda7418_reg_h

#define ON 1
#define OFF 0

// I2C bus address
#define TDA_ADDR 0x44

// Output Channel definitions
#define Global 0
#define Subwoofer 1
#define Front_Left 2
#define Rear_Left 3
#define Rear_Right 4
#define Front_Right 5


// Registers map
#define REG_SOURCE_SEL  0x00
#define REG_LOUDNESS    0x01
#define REG_VOLUME      0x02
#define REG_TREBLE      0x03
#define REG_MIDDLE      0x04
#define REG_BASS        0x05
#define REG_MID_BAS_FC  0x06

#define REG_SPK_ATT_FL  0x07
#define REG_SPK_ATT_RL  0x08
#define REG_SPK_ATT_RR  0x09
#define REG_SPK_ATT_FR  0x0A
#define REG_SPK_ATT_SUB 0x0B

#define REG_SOFTMUTE    0x0C
#define REG_AUDIO_TEST  0x0D

#endif
