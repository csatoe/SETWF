// SETMAUI.H

#include "maui.h"

#ifndef SETMAUI_H
#define SETMAUI_H

//                      ***** Function prototypes *****

// This prints a DOS help
void Help(void);


// Error procedure
void Error(int num);


// Convert hexadecimal ascii digit to byte
BYTE aschex2byte(char n);


// Get a range from a Command Line
void GetRange(char* source, WORD defmin, WORD defmax, int *min, int *max);


// Switch off pause or debug mode if the standard output is redirected
void IgnorePause(void);


// Pause or Debug Mode
void Pause(BYTE NowMode);


// Get Wave Parameters from the Command Line
void WaveParams(char* p);


// To process the SETWF.CFG file
void ProcessConfig(char* argv[], int mode);
#define pmFirst 0
#define pmLoad 1
#define pmChange 2

//               ***** Macros and variables *****


// Constant for listing
const char* Enabled="Enabled";

      char* Disabled="Disabled";

      char* VolBar="                                                                ";

      char* NoteName[12]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

      char* PanSource[4]={"Keyboard","Velocity","Random","Not used"};

      float TimeTable[128]={0.0,0.008,0.009,0.010,0.012,0.013,0.15,0.016,0.018,
                            0.020,0.022,0.024,0.026,0.029,0.031,0.034,0.037,
                            0.041,0.044,0.048,0.052,0.056,0.061,0.066,0.071,
                            0.076,0.083,0.089,0.096,0.103,0.111,0.120,0.129,
                            0.139,0.150,0.161,0.173,0.186,0.200,0.215,0.231,
                            0.248,0.266,0.286,0.307,0.330,0.354,0.380,0.408,
                            0.438,0.470,0.504,0.541,0.580,0.622,0.668,0.716,
                            0.768,0.824,0.883,0.947,1.016,1.089,1.168,1.253,
                            1.343,1.440,1.544,1.655,1.775,1.903,2.040,2.187,
                            2.345,2.513,2.694,2.888,3.096,3.319,3.558,3.814,
                            4.088,4.382,4.697,5.035,5.397,5.785,6.200,
                            6.646,7.124,7.635,8.184,8.772,9.402,10.078,10.801,
                            11.577,12.409,13.300,14.255,15.279,16.376,17.552,
                            18.812,20.163,21.611,23.162,24.826,26.608,28.518,
                            30.566,32.760,35.112,37.633,40.334,43.230,46.333,
                            49.659,53.244,57.044,61.139,65.528,70.232,75.273,
                            80.676,86.467,92.674,99.326};

      char* ModSource[16]={"LFO 1",
                           "LFO 2",
                           "Envelope 1",
                           "Envelope 2",
                           "Keyboard",
                           "Keyboard (log)",
                           "Velocity",
                           "Velocity (log)",
                           "Random",
                           "Channel Pressure",
                           "Modulation Wheel",
                           "Breath Controller",
                           "Foot Controller",
                           "Volume Controller",
                           "Pan ontroller",
                           "Expression Contr."};

      char* LFOWave[8]={"Triangle",
                        "Sine",
                        "Square",
                        "Saw",
                        "Positive Triangle",
                        "Positive Sine",
			"Positive Square",
			"Positive Saw"};


// SETMAUI Error Messages
const char* ErrorMsg[]={
			"No MPU device found",         //0
			"Error accessing the Extended Memory", //1
			"Illegal Port address",
			"Illegal Volume Settings",
			"Illegal Number of voices",
			"Illegal Synthesizer Tuning",
			"Illegal Channel Number",
			"Illegal Parameter",           //7
			"Could not open file",         //8
			"File read error",             //9
			"Not enough memory",           //10
			"Not allowed file type",       //11
			"Too old version",
			"Not enough WaveFront Sample Memory",
			"Invalid File Type",
			"Couldn't set Internal Sample",
			"Not enough sample ROOM",      //16
			"Not enough patch ROOM",
			"Associated WAVE file error",  //18
			"Not a PCM file",              //19
			"The sample size greater than 1 Megabyte", //20
			"Illegal reference to an EMPTY sample", //21
			"Couldn't open the specified WFS/WSS file", //22
			"Nested WFS file not allowed.",	//23
			"The MPU device is used by another program", //24
			"User break!", //25
			"File error while saving used Extended Memory",//26
			};


// Structured buffers
struct WaveFrontFileHeader WFHead;              // for WaveFront files
struct WaveFrontProgram WFP;
struct WaveFrontPatch WFPatch;
struct WaveFrontDrumkit WFD;
struct WaveFrontExtendedSampleInfo WFESI;
struct ALIAS WFAL;
struct SAMPLE WFSAM;

struct ChunkHeader ChunkH;                      // WAVE chunk
struct fmtChunk ChunkFmt;                       // WAVE format chunk
struct smplChunk ChunkSmpl;                     // WAVE sample chunk



// Pause Mode constants
#define PAUSENEVER 255
#define PAUSENO    0
#define PAUSEDEBUG 1
#define PAUSEPAUSE 2
#define PAUSEALL   3

BYTE bPause=PAUSENO;

extern BYTE nNoLoad; // Info level, download disabled

// This arrays for WFP & WAVE loading to different pleace
// So this contains the RELOCATION points
int SampleNew[512];
int PatchNew[256];

#define UNDEFINED -1
#define YES       1
#define NO        0


// Options for WFx downloading
int FirstPrg=UNDEFINED,
    FirstPatch=UNDEFINED,
    FirstSample=UNDEFINED,
    DefFirstPrg=0,
    DefFirstPatch=0,
    DefFirstSample=0;

// Options for WAVE loading
int WavPrg=UNDEFINED,
    WavVol=UNDEFINED,
    WavKey=UNDEFINED,
    WavDrum=UNDEFINED,
    DefWavPrg=0;

long WavLoopStart=UNDEFINED;
long WavLoopEnd=UNDEFINED;

int Loop=UNDEFINED,
    Bidirectional=UNDEFINED,
    Reverse=UNDEFINED,
    FromFile=YES,
    MonoSample=NO;

// If the file MAUI compatible file or not
int NonMaui;


BYTE status=0,  // Need print Status
     chstat=0;  // Need print Channel Status

// To store the path to the SETUPSND
char setupsnd[255]="";

// To store the path to the MIDI player
char midiplayer[255]="";


// Standard pointers and integers
char *p,*sz;
int i,j,f,l;
long lv;
WORD w,n,nn,m,yy;

// XMS using
BOOL AllocXMS=0;  //false
BOOL CleanXMS=0;  //false

// John Caffarella's WaveFront Set file:
char WFSfile[128]="";
char argums[128];
char* argum[64]={argums};
int argumc;


// TB's WSS Sample Set File:
char WSSFileName[128]="";
char save_argums[128];
int save_i;
int save_argumc;
char* save_argum[64]={argums};
//DWORD WSSpos;
WORD isWSS=0; // FALSE, >0: number of sample
// The string searching in the WSS file
const char* WSSsignature="\xff\xff\x1\x0\xb\x00""CSampleInfo";

struct {
    char SampleName[32];
    BOOL fIsDrum;
    BOOL fIsMono;
    BOOL fIsLooped;
    BYTE bProgramNumber;

    // Prgs
    struct LAYER layer[4];

    // Drums
    /*BYTE bMIDINoteNumber;*/

    BYTE byPatchNumber;

    BYTE fMixLevel:7;
    BYTE fUnmute:1;
    BYTE fGroup:4;
    BYTE fUnused1:4;
    BYTE fPanModSource:2;
    BYTE fPanModulated:1;
    BYTE fPanAmount:4;
    BYTE fUnused2:1;

    // Patches
    BYTE bPatchNumber1;
    struct PATCH patch1;
    BYTE bPatchNumber2;
    struct PATCH patch2;

} WSS;


FILE* stream;
BOOL procend=0; // FALSE when start

char dirtemp[MAXDIR];


// *************** EXTERNAL variables MAUI.C **********************
extern struct LAYER DefLayer;
extern struct PROGRAM DefPrg;
extern struct PATCH DefPatch;
extern struct DRUM DefDrum;

// Pointers to structures !!!
extern struct SAMPLE* SampleH;
extern struct ALIAS* Alias;



// Base Address
extern WORD MPUEmuData;
extern int MPUuseMIDI;
extern BYTE IrqLine;
extern BYTE TestIrq;

WORD BasePort1,BasePort2,BasePort3,BasePort4;
int UseMPU1,UseMPU2,UseMPU3,UseMPU4;
BYTE Irq1,Irq2,Irq3,Irq4;

BOOL Reset=0;

// Store the states
extern int Samples[512];
extern int Patches[256];


// Working signals
extern const char Work[4];
extern BYTE nWork;


// Standard STATIC buffers !!!
extern BYTE s[300],z[300],y[300];

// Standard structure pointers, not buffers !!!
extern struct PROGRAM* Prg;
extern struct DRUM* Drum;
extern struct PATCH* Patch;

extern const char* SampleType[4];
extern const char* ProgramName[128];
extern const char* PatchName[256];
extern const char* SampleName[512];

extern const WORD WF_VERSION;

// Comment length
extern const int  MAX_COMMENT;

// Names of the WaveFront files
extern const char szBankFile[];
extern const char szDrumkitFile[];
extern const char szProgramFile[];

// File Identifiers
extern const UINT BANKFILE;
extern const UINT DRUMKITFILE;
extern const UINT PROGRAMFILE;
extern const UINT WAVEFILE;

extern const UINT WSSFILE;
extern const UINT WFSFILE;

// XMS from XMM.C
extern WORD XMShandle;

extern BYTE sName[33]; // for the names

extern BOOL UseXMS;
// Not used ....
//#define BESTPLEACE 1024

char XMSSaveFile[128]="";
char XMSLoadFile[128]="";


//////////////////////// FUNCTIONS //////////////////////////////
void CloseXMS(void);



#endif
