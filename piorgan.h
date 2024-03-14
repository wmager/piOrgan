#ifndef PIORGAN_H
#define PIORGAN_H


#include <fluidsynth.h>
#include <thread>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define PGUP 5
#define PGDOWN 6

#define MAX_OFFSET 128
#define MIN_OFFSET -128

//#define organ_sf "/usr/share/sounds/sf2/Jeux14.sf2"
//#define organ_sf "/usr/share/sounds/sf2/FluidR3_GM.sf2" 
#define regular_sf "/usr/share/sounds/sf2/FluidR3_GM.sf2"
#define organ_sf "/usr/share/sounds/sf2/FluidR3_GM_GS.sf2"
#define DEFAULT_SF regular_sf

struct MKeyIn {
	int instrument;
	int note;
};

struct MPreset {
	int bank;
	int prog;
	char name[100];
};

struct MSetting_v1{
	int valid;
	float gain;	
	int currentInstrument;
	int selectedPresets[3];
	int volumes[3];
	int offsets[3];
	int currentBank[3];	
};

#define REVERB_ON 1
#define CHORUS_ON 2
#define CHORUS_MOD_SINE 4
#define CHORUS_MOD_TRIANGLE 4

struct MSetting{
	int valid;
	float gain;	
	int currentInstrument;
	int selectedPresets[3];
	int volumes[3];
	int offsets[3];
	int currentBank[3];	
    int fxControll; 
    double fxReverbParameter[4];
    double fxChorusParameter[3];
};

class PiOrgan{
    public:
        PiOrgan();
        PiOrgan(const char* soundFont);
        void cleanup();
        int loop();
        int setup();

        std::vector<std::string> getSoundFonts();
        int getPresetsCnt(int instrument);
        MPreset* getPresets(int instrument);
        int changeBank(int instrument);
        int getCurrentSetting();
        void setSelectedPreset(int preset, int instrument);
        void setPresetsChanged(void (*fnk)(int));
        void setSettingChanged(void (*fnk)(int));
        void setOffsetChanged(void (*fnk)(int, int));
        void setSetSaveMode(void (*fnk)(bool)); 

        void setVolume(int volume, int instrument);
        void setOffset(int offset, int instrument);
        void setReverbParameter(double value, int index);
        void setChorusParameter(double value, int index);
        double getReverbParameter(int index);
        double getChorusParameter(int index);
        void increaseOffset( int instrument);
        void decreaseOffset( int instrument);
        int getSelectedPreset(int instrument);
        int getVolume(int instrument);
        int getOffset(int instrument);
        void storeSetting(const int setting);
        void restoreSetting(const int setting);
        int saveSettings();
        int loadSettings();
        void loadSoundFont(const char *soundfont);


    private:
        int openSynth(const char *soundfont);       
        int  openUSB();
        int  getUsbSoundcardId();
        int  geHifiberryCardId();
        void loadPresets();
        
        void doCmd(char cmd);
        void setInstrument(int instrument);
        void readArduino();
        void readMidi();
        void keyPressed(int key);

        void (*settingChanged_)(int) = NULL;
        void (*presetsChanged_)(int) = NULL;
        void (*offsetChanged_)(int, int) = NULL;
        void (*setSaveMode_)(bool) = NULL;

        bool stop_ = false;
        std::thread* threadMidi_;
        std::thread* threadArduino_;

        fluid_synth_t *synth = NULL;
        fluid_sfont_t* sfont = NULL;
        int sfont_id=-1;

        struct MPreset presets[16][128];
        
        struct MKeyIn keymapping[136] = {
	// Pedal    :   1-13
	// Manual I :   n.c.
	// Manual II: 65-108
	{-1,  0},{ 0, 60},{ 0, 61},{ 0, 62},{ 0, 63},{ 0, 64},{ 0, 65},{ 0, 66}, //   0
	{ 0, 67},{ 0, 68},{ 0, 69},{ 0, 70},{ 0, 71},{ 1, 85},{ 1, 86},{ 1, 87}, //   8
	{ 1, 88},{ 1, 89},{ 1, 90},{ 1, 91},{ 1, 92},{ 1, 93},{ 1, 94},{ 1, 95}, //  16
	{ 1, 96},{ 1, 61},{ 1, 62},{ 1, 63},{ 1, 64},{ 1, 65},{ 1, 66},{ 1, 67}, //  24
	{ 1, 68},{ 1, 69},{ 1, 70},{ 1, 71},{ 1, 72},{ 1, 73},{ 1, 74},{ 1, 75}, //  32
	{ 1, 76},{ 1, 77},{ 1, 78},{ 1, 79},{ 1, 80},{ 1, 81},{ 1, 82},{ 1, 83}, //  40
	{ 1, 84},{ 0, 72},{-1,  0},{-1,  0},{-1,  0},{ 1, 53},{ 1, 54},{ 1, 55}, //  48
	{ 1, 56},{ 1, 57},{ 1, 58},{ 1, 59},{ 1, 60},{ 1, 61},{ 1, 62},{ 1, 63}, //  56
	{ 1, 64},{ 2, 53},{ 2, 54},{ 2, 55},{ 2, 56},{ 2, 57},{ 2, 58},{ 2, 59}, //  64
	{ 2, 60},{ 2, 61},{ 2, 62},{ 2, 63},{ 2, 64},{ 2, 65},{ 2, 66},{ 2, 67}, //  72
	{ 2, 68},{ 2, 69},{ 2, 70},{ 2, 71},{ 2, 72},{ 2, 73},{ 2, 74},{ 2, 75}, //  80
	{ 2, 76},{ 2, 77},{ 2, 78},{ 2, 79},{ 2, 80},{ 2, 81},{ 2, 82},{ 2, 83}, //  88
	{ 2, 84},{ 2, 85},{ 2, 86},{ 2, 87},{ 2, 88},{ 2, 89},{ 2, 90},{ 2, 91}, //  96
	{ 2, 92},{ 2, 93},{ 2, 94},{ 2, 95},{ 2, 96},{-1,  0},{-1,  0},{-1,  0}, // 104
	{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0}, // 112
	{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0}, // 120
	{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0},{-1,  0}, // 128
};

    int cntPresets[16];
    int banks[16];
    int cntBanks = 0;

    int channels[3] = {0,1,2};
    int volumes[3] = {127,127,127};
    int offsets[3] = {-24,0,-12};
    int currentBank[3] = {0,0,0};
    int currentInstrument = 0;
    int selectedPresets[3] = {0,0,0};
    float gain;
    //int currentSoundFont = 0;
    //char soundfonts[2][100] = {organ_sf, regular_sf};
    char currentSoundFont[100];
    struct MSetting settings[10];
    int currentSetting = 0;
};

#endif //PIORGAN_H