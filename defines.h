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
#define INPUT_SE1 0x01
#define INPUT_SE2 0x02
#define INPUT_SE3 0x03
#define INPUT_SE4_PD 0x00
#define INPUT_MUTE 0x04
#define PD 1
#define SE 0

// Loudness
// LOW is already defined as 0
#define LOWHIGH 1

// Middle settings
#define MID_QF_050 0
#define MID_QF_075 1
#define MID_QF_100 2
#define MID_QF_125 3

// Bass settings
#define BASS_QF_100 0
#define BASS_QF_125 1
#define BASS_QF_150 2
#define BASS_QF_200 3

// Soft Step/Soft mute times
#define SMT_048 0
#define SMT_096 1
#define SMT_123 2
#define SST_016 0
#define SST_032 1
#define SST_064 2
#define SST_128 3
#define SST_256 4
#define SST_512 5
#define SST_1024 6
#define SST_2048 7


// Test register settings
#define InMuxOutR   0
#define LoudOutR    1
#define VolumeOutR  2
#define VGB126      3
#define REF5V5      4
#define SSCLK       5
#define SMCLK       6
#define Clk200kHz   7
#define NORMAL_MODE 0
#define FAST_MODE   1
#define MUX_OUT     0x00
#define EXT_CLK     0x80
#define MUTE_PIN    0x82

// Registers map
#define AUTOINCREMENT 0x20

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
