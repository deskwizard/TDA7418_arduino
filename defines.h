#ifndef defines_h
#define defines_h


#define enable 1
#define disable 0

// I2C bus address
#define TDA_ADDR 0x44

// Attenuator definitions
#define Front_Left 0x07
#define Rear_Left 0x08
#define Rear_Right 0x09
#define Front_Right 0x0A
#define Subwoofer 0x0B

// Input channels definitions
#define SE1 0x01
#define SE2 0x02
#define SE3 0x03
#define SE4_PD 0x00
#define MUTE 0x04
#define PD 1
#define SE 0

// Loudness
// LOW is already defined as 0
#define LOWHIGH 1
#define OFF 0

// Middle settings
#define MID_QF_050 0
#define MID_QF_075 1
#define MID_QF_100 2
#define MID_QF_125 3

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
