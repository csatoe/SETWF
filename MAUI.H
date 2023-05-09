// MAUI.H
// Original <FILES.H> 3/31/94 J. Johnson
// Copyright 1994 Turtle Beach Systems, Inc. All Rights Reserved

#ifndef MAUI_H
#define MAUI_H

#define UINT unsigned int
#define DWORD unsigned long
#define BOOL unsigned char
#define WORD unsigned int
#define BYTE unsigned char

// WaveFront Sample Types (used by WFGATE.DLL)

#define ST_SAMPLE 0
#define ST_MULTISAMPLE 1
#define ST_ALIAS 2
#define ST_EMPTY 127

// MAUI Host Commands

#define HCMDSETVOL 0x89         // Set Synthesizer Volume
#define HCMDGETVOL 0x92         // Get Synthesizer Volume !!! MIDI
#define HCMDSETNOV 0x8b         // Set Number of Voices
#define HCMDGETNOV 0x94         // Get Number of Voices   !!! MIDI
#define HCMDSETTUN 0xa6         // Set Synthesizer Tuning
#define HCMDGETTUN 0xa7         // Get Synthesizer Tuning !!! MIDI
#define HCMDDISCHN 0x9a         // Disable Synth Channel
#define HCMDENACHN 0x9b         // Enable Synth Channel
#define HCMDGETSCS 0xab         // Get Synth Channel Status
#define HCMDDISMIS 0x9d         // Disable MIDI-In to Synth
#define HCMDENAMIS 0x9e         // Enable MIDI-In to Synth
#define HCMDENAVMM 0xa8         // Enable Virtual MIDI Mode
#define HCMDDISVMM 0xa9         // Disable Virtual MIDI Mode
#define HCMDREPMST 0xaa         // Report MIDI Status
#define HCMDREPFWV 0x9f         // Report Firmware Version
#define HCMDREPHWV 0xcf         // Report Hardware Version
#define HCMDREPNOS 0xa0         // Report Number of Samples
#define HCMDREPIOL 0xb4         // Report Instantaneous Output Level
#define HCMDREPPOL 0xb5         // Report Peak Output Level

#define HCMDDSAM 0x80           //+ .	Download Sample
#define HCMDDBLK 0x81           // 	Download Block         !!! MIDI
#define HCMDDSAMH 0xac          //+ OK	Download Sample Header
#define HCMDUSAMH 0xad          //- OK	Upload Sample Header   !!! MIDI
#define HCMDDMSAM 0x82          //+ OK	Download Multisample
#define HCMDUMSAM 0xae          //- OK	Upload Multisample     !!! MIDI
#define HCMDDSAMA 0x83          //+ OK	Download Sample Alias
#define HCMDUSAMA 0xaf          //- OK	Upload Sample Alias    !!! MIDI
#define HCMDDELSAM 0x84         //+ OK	Delete Sample
#define HCMDIDESAMT 0xb0        //- OK	Identify Sample Type
//#define HCMDUSAMP 0xd7        // 	Upload Sample Parameters
#define HCMDREPFRE 0x85         // 	Report Free Memory
#define HCMDDPATCH 0x86         //+ OK	Download Patch
#define HCMDUPATCH 0xa3         //- OK	Upload Patch           !!! MIDI
#define HCMDDPRG 0x87           //+ OK	Download Program
#define HCMDUPRG 0xa4           //- OK	Upload Program         !!! MIDI
#define HCMDDEDP 0xb1           //+ OK	Download Enhanced Drum Program
#define HCMDUEDP 0xb2           //- OK	Upload Enhanced Drum Program !!! MIDI
#define HCMDSETEDP 0xb3         // Set Enhanced Drum Program Channel
#define HCMDDISEDP 0xa2         // Disable Drum Program
#define HCMDREPCPN 0xb6         // Report Channel Program Numbers

#define HCMDREPMEM 0xdf         // Report Memory Configuration
#define HCMDSETEMU 0xf0         // Set the emulation mode (1=401/0=6580)
				// (from SETUPSND.EXE)

// MPU

#define MPUWait 1000
// MPU401 commands: (out: port 331h)
#define MPU401_SET_UART	(BYTE)0x3F	// set UART mode
#define MPU401_RESET	(BYTE)0xFF	// reset MIDI device

// MPU401 status codes: (in: port 331h)
#define MPU401_BUSY	(BYTE)0x40	// 1:busy, 0:ready to receive a command
#define MPU401_EMPTY	(BYTE)0x80	// 1:empty, 0:buffer is full
#define MPU401_ACK	(BYTE)0xFE	// command acknowledged


//                     ***** WaveFront Stuctures *****
struct ENVELOPE
{
    BYTE fAttackTime:7;
    BYTE fUnused1:1;
    BYTE fDecay1Time:7;
    BYTE fUnused2:1;
    BYTE fDecay2Time:7;
    BYTE fUnused3:1;
    BYTE fSustainTime:7;
    BYTE fUnused4:1;
    BYTE fReleaseTime:7;
    BYTE fUnused5:1;
    BYTE fRelease2Time:7;
    BYTE fUnused6:1;
    char cAttackLevel;
    char cDecay1Level;
    char cDecay2Level;
    char cSustainLevel;
    char cReleaseLevel;
    BYTE fAttackVelocity:7;
    BYTE fUnused7:1;
    BYTE fVolumeVelocity:7;
    BYTE fUnused8:1;
    BYTE fKeyScale:7;
    BYTE fUnused9:1;
};

struct LFO
{
    BYTE bySampleNumber;
    BYTE fFrequency:7;
    BYTE fUnused1:1;
    BYTE fAMSource:4;
    BYTE fFMSource:4;
    char cFMAmount;
    char cAMAmount;
    char cStartLevel;
    char cEndLevel;
    BYTE fDelayTime:7;
    BYTE fWaveRestart:1;
    BYTE fRampTime:7;
    BYTE fUnused2:1;
};

struct PATCH
{
    int  nFreqBias;         // ** THIS IS IN MOTOROLA FORMAT!! **
    BYTE fAmpBias:7;
    BYTE fUnused1:1;
    BYTE fPortamento:7;
    BYTE fUnused2:1;
    BYTE bySampleNumber;
    BYTE fPitchBend:4;
    BYTE fSampleMSB:1;
    BYTE fUnused3:3;
    BYTE fMono:1;
    BYTE fRetrigger:1;
    BYTE fNoHold:1;
    BYTE fRestart:1;
    BYTE fFilterConfig:2;
    BYTE fReuse:1;
    BYTE fResetLfo:1;
    BYTE fFMSource2:4;
    BYTE fFMSource1:4;
    char cFMAmount1;
    char cFMAmount2;
    BYTE fAMSource:4;
    BYTE fUnused4:4;
    char cAMAmount;
    BYTE fFC1MSource:4;
    BYTE fFC2MSource:4;
    char cFC1MAmount;
    char cFC1KeyScale;
    char cFC1FreqBias;
    char cFC2MAmount;
    char cFC2KeyScale;
    char cFC2FreqBias;
    BYTE fRandomizerRate:7;
    BYTE fUnused5:1;
    struct ENVELOPE envelope1;
    struct ENVELOPE envelope2;
    struct LFO lfo1;
    struct LFO lfo2;
};

struct LAYER
{
    BYTE byPatchNumber;
    BYTE fMixLevel:7;
    BYTE fUnmute:1;
    BYTE fSplitPoint:7;
    BYTE fSplitDir:1;
    BYTE fPanModSource:2;
    BYTE fPanModulated:1;
    BYTE fPan:4;
    BYTE fSplitType:1;
};

struct PROGRAM
{
    struct LAYER layer[4];
};

#define SAMPLE_OFFSET unsigned long

struct SAMPLE
{
    SAMPLE_OFFSET sampleStartOffset;
    SAMPLE_OFFSET loopStartOffset;
    SAMPLE_OFFSET loopEndOffset;
    SAMPLE_OFFSET sampleEndOffset;
    int nFrequencyBias;
    BYTE fSampleResolution:2;  // See preceding enum
    BYTE fUnused1:1;
    BYTE fLoop:1;
    BYTE fBidirectional:1;
    BYTE fUnused2:1;
    BYTE fReverse:1;
    BYTE fUnused3:1;
};

struct MULTISAMPLE
{
    int nNumberOfSamples;   // log2 of the number of samples (0->1, ... 7->128, etc)
    int nSampleNumber[128];
};

struct ALIAS
{
    int nOriginalSample;
    SAMPLE_OFFSET sampleStartOffset;
    SAMPLE_OFFSET loopStartOffset;
    SAMPLE_OFFSET loopEndOffset;
    SAMPLE_OFFSET sampleEndOffset;
    int  nFrequencyBias;
    BYTE fSampleResolution:2;
    BYTE fUnused1:1;
    BYTE fLoop:1;
    BYTE fBidirectional:1;
    BYTE fUnused2:1;
    BYTE fReverse:1;
    BYTE fUnused3:1;
};

struct DRUM
{
    BYTE byPatchNumber;
    BYTE fMixLevel:7;
    BYTE fUnmute:1;
    BYTE fGroup:4;
    BYTE fUnused1:4;
    BYTE fPanModSource:2;
    BYTE fPanModulated:1;
    BYTE fPanAmount:4;
    BYTE fUnused2:1;
};

struct DRUMKIT
{
    struct DRUM drum[128];
};


// File header structure. This is used as the file header for all Wavefront data files.
struct WaveFrontFileHeader
{
    char  szSynthName[32];            // model name of the synthesizer
    char  szFileType[32];             // "Bank", "Program", "DrumKit", etc.
    WORD  wVersion;                   // Version times 100; v1.00 = 100, etc.
    WORD  wProgramCount;              // Number of programs in this file
    WORD  wDrumkitCount;              // Number of drumkits in this file
    WORD  wPatchCount;                // Number of patches in this file
    WORD  wSampleCount;               // Number of samples in this file
    WORD  wEffectsCount;              // Number of effects in this file
    DWORD dwProgramOffset;            // Offset to the beginning of program data
    DWORD dwDrumkitOffset;            // Offset to the beginning of drumkit data
    DWORD dwPatchOffset;              // Offset to the beginning of patch data
    DWORD dwSampleOffset;             // Offset to the beginning of sample data
    DWORD dwEffectsOffset;            // Offset to the beginning of effects data
    DWORD dwMemoryRequired;           // Total size in bytes of all samples
    BOOL  bEmbeddedSamples;           // TRUE if samples are stored in this file.
    BOOL  bUnused;                    // Unused
    char  szComment[64];              // Comment (primarily for copyright notices)
    BYTE  byPad[62];                  // for expansion (shrinking rapidly!)
};

// Data structures. Program and patch structures are quite simple; they are just
// the data structs from the synth with names and numbers appended. The drumkit
// struct is even simpler; it is really here only to make the terminology consistent.
// Sample structures are more complex, as they must deal with more information.
//
// In files BELOW VERSION 1.20, the WaveFrontSampleInfo struct is used.
// This is followed by:
//
//   - the appropriate structure (SAMPLE, ALIAS, or MULTISAMPLE) as sent
//     to the synth, AND for samples only
//   - the full path to the sample file (_MAX_PATH characters in length).
//     If the sample is internal, this will be "INTERNAL".
//     If the sample is an embedded file, this will be "EMBEDDED".
//     AND, if the file is embedded (and the sample is not internal) -
//   - an image of the wave or MBS file. This is intended to be saved in a
//     temporary disk file before it is dumped to the synth.
//
// For files of version 1.2 and up, the WavefrontExtendedSampleInfo struct is used.
//     This is followed by the same types of info as in earlier versions, EXCEPT that
//     embedded data is raw sample data; no file headers are used.
//

struct WaveFrontProgram
{
    struct LAYER layer[4];
    int  nNumber;             // The original location of the program
    char szName[32]; // The sample name
};

struct WaveFrontPatch
{
    int  nFreqBias;         // ** THIS IS IN MOTOROLA FORMAT!! **
    BYTE fAmpBias:7;
    BYTE fUnused1:1;
    BYTE fPortamento:7;
    BYTE fUnused2:1;
    BYTE bySampleNumber;
    BYTE fPitchBend:4;
    BYTE fSampleMSB:1;
    BYTE fUnused3:3;
    BYTE fMono:1;
    BYTE fRetrigger:1;
    BYTE fNoHold:1;
    BYTE fRestart:1;
    BYTE fFilterConfig:2;
    BYTE fReuse:1;
    BYTE fResetLfo:1;
    BYTE fFMSource2:4;
    BYTE fFMSource1:4;
    char cFMAmount1;
    char cFMAmount2;
    BYTE fAMSource:4;
    BYTE fUnused4:4;
    char cAMAmount;
    BYTE fFC1MSource:4;
    BYTE fFC2MSource:4;
    char cFC1MAmount;
    char cFC1KeyScale;
    char cFC1FreqBias;
    char cFC2MAmount;
    char cFC2KeyScale;
    char cFC2FreqBias;
    BYTE fRandomizerRate:7;
    BYTE fUnused5:1;
    struct ENVELOPE envelope1;
    struct ENVELOPE envelope2;
    struct LFO lfo1;
    struct LFO lfo2;
    int  nNumber;             // The original location of the patch
    char szName[32]; // The patch name
};

struct WaveFrontDrumkit
{
    struct DRUM drum[128];
}
;

// Struct for sample info. This is only used by versions 1.0 and 1.1 of the program
struct WaveFrontSampleInfo
{
    DWORD dwSize;               // Size of this sample, including this struct and any following data
    int   nSampleType;          // ST_SAMPLE, etc. constants from WFSYNTH.H
    int   nNumber;              // The original location of the sample
    char  szName[32];           // Name of the sample
};

// Struct for extended sample info. This is for versions 1.2 and up. Note the similarity
// to the above structure.
struct WaveFrontExtendedSampleInfo
{
    DWORD dwSize;               // Size of this sample, including this struct and any following data
    int   nSampleType;          // ST_SAMPLE, etc. constants from WFSYNTH.H
    int   nNumber;              // The original location of the sample
    char  szName[32];           // Name of the sample
    DWORD dwSampleRate;         // Sample rate in Hz
    DWORD dwSizeInBytes;        // Size of the sample data itself, in bytes
    DWORD dwSizeInSamples;      // Number of samples in the file
    UINT  nChannel;             // CH_ constants from WAVE.H
    BYTE  byUnused[62];         // For expansion
};


//              ***** Structures for WAVE files *****
struct ChunkHeader
{
  char szName[4];
  DWORD dwSize;
};

struct fmtChunk
{
  WORD wFormatTag;
  WORD wChannels;
  DWORD dwSamplesPerSec;
  DWORD dwAvgBytesPerSec;
  WORD wBlockAlign;
  WORD wResolution;
};

struct smplChunk
{
  DWORD dwUnknown1;
  DWORD dwUnknown2;
  DWORD dwUnknown3;
  DWORD dwRootKey;
  DWORD dwUnknown4;
  DWORD dwUnknown5;
  BYTE bSMPTE1;
  BYTE bSMPTE2;
  BYTE bSMPTE3;
  BYTE bSMPTE4;
  DWORD dwUnknown6;
  DWORD dwUnknown7;
  DWORD dwUnknown8;
  DWORD dwUnknown9;
  DWORD dwLoopStart;
  DWORD dwLoopEnd;
  DWORD dwUnknown10;
  DWORD dwUnknown11;
};


// Default MPU Midi Device Base Address
#define MPUDEF 0x330

// Port addresses
#define MPUCTRL MPUEmuData+1            // MPU-401 Emulation Control Register
#define MPUSTAT MPUEmuData+1            // MPU-401 Emulation Status Register
#define HOSTDATA MPUEmuData+2           // Host Data Register
#define HOSTCTRL MPUEmuData+3           // Host Control Register
#define HOSTSTAT MPUEmuData+3           // Host Sattus Register
#define HOSTDOWN MPUEmuData+4           // Host Sample Download Register
#define HOSTEND MPUEmuData+6            // Host Sample Download End Register

// Masks
#define HOSTRXINTEN 0x01                // Host Receive Interrupt Enable
#define HOSTTXINTEN 0x10                // Host Transmit Interrupt Enable
#define MAUIPLAY    0x20                // Maui Playing
#define MASTERINTEN 0x40                // Master Interrupt Enable
#define MASTERRESET 0x80                // Master Reset

#define HOSTRXREGFUL 0x02               // Host Receive Register Full
#define HOSTRXINTPEN 0x04               // Host Receive Interrupt Pending
#define HOSTTXREGEMP 0x20               // Host Transmit Register Empty
#define HOSTTXINTPEN 0x40               // Host Transmit Interrupt Pending

//Macro for MPU flags
#define WAIT( port, flag )	while ( (inp(port)&flag) == 0)


// For Patches & Samples arrays
#define EMPTY    0      //Patches too
#define UNUSED   -1
#define USED     1      //Patches too
#define INTERNAL 4096
#define SMASK    4095   //to clear INTERNAL flag
#define SIGNED   8192   // to allocate


// Drum program number for searching empty stuffs
#define DRUMKIT  256
#define NOPRG    1024


/*
 *	XMS Driver C Interface Routine Definitions
 */

#define XXMSMEMOFFS 16L                 // (long)       //0-15 SIGN
							//16-19 FREE
#define XPRGOFFS 20L                                    //20-4131 PRG
#define XPRGSIZE (16L+32L)
#define XPRGALL 128L*XPRGSIZE

#define XDPRGOFFS XPRGOFFS+XPRGALL                      //4132-4643 DRUM
#define XDPRGSIZE (4L)
#define XDPRGALL 128L*XDPRGSIZE

#define XPATCHOFFS XDPRGOFFS+XDPRGALL                   //4644-29731 PATCH
#define XPATCHSIZE (66L+32L)
#define XPATCHALL 256L*XPATCHSIZE

#define XSAMPLEOFFS XPATCHOFFS+XPATCHALL                //29732-181283 SAM
#define XSAMPLESIZE (1L+32L+4L+2L+257L)
#define XSAMPLEALL 512L*XSAMPLESIZE

#define XPATCHNEWOFFS XSAMPLEOFFS+XSAMPLEALL            //181284-
#define XPATCHNEWSIZE sizeof(Patches)

#define XSAMPLENEWOFFS XPATCHNEWOFFS+XPATCHNEWSIZE      //
#define XSAMPLENEWSIZE sizeof(Samples)

#define XXMSALL XSAMPLENEWOFFS+XSAMPLENEWSIZE


struct	XMM_Move {
	unsigned long	Length;
	unsigned short	SourceHandle;
	unsigned long	SourceOffset;
	unsigned short	DestHandle;
	unsigned long	DestOffset;
};

#define fromXMS( xms_offs,dos_offs,bytes ) ( XMM_MoveExtended(bytes,XMShandle,xms_offs,0,(long) dos_offs) )
#define toXMS( dos_offs,xms_offs,bytes ) ( XMM_MoveExtended(bytes,0,(long)dos_offs,XMShandle,xms_offs) )


//                      ***** Functions in MAUI.C *****

// Test MAUI presents at BASE address
BOOL MPUTest(void);
// return BOOLEAN


// Sending Host Command to the MAUI & wait for answer
void Command (BYTE* Buffer, WORD OutBytes, WORD InBytes);
// buffer       = buffer (in & out)
// OutBytes     = Number of Bytes to Send
// InBytes      = Number of Bytes to Receive

//MPU401 initialize;
int MPU401initHardware(void);


int MPU401sendByte(BYTE value);



// XMS from XMM.C
extern WORD XMShandle;
extern void Error(int num);

//-----------------
// Free up patches & samples
void FreeUp(int Current);

// Get the Program information from XMS or MAUI
BOOL GetProgram(WORD n, BYTE* s);
BOOL GetProgramName(WORD n);
// Down the DrumProgram to the Maui and to the XMS
void DownProgram(WORD n, BYTE* from, BYTE* s);


// Get the DrumProgram information from XMS or MAUI
BOOL GetDrumProgram(WORD n, BYTE* s);
// Down the DrumProgram to the Maui and to the XMS
void DownDrumProgram(WORD n, BYTE* from, BYTE* s);


// Get the Patch information from XMS or MAUI
BOOL GetPatch(WORD n, BYTE* s);
BOOL GetPatchName(WORD n);
// Down the Patch to the Maui and to the XMS
void DownPatch(WORD n, BYTE* from, BYTE* s);


// Get the Sample type information from XMS or MAUI
BOOL GetSampleType(WORD n, BYTE* s);
BOOL GetSampleName(WORD n);
DWORD GetOccMem(WORD n);  // The memory occupied by a sample

// Get the Sample Header from XMS or MAUI
BOOL GetSampleHeader(WORD n, BYTE* s);
// Set the Sample Header in the Maui and in the XMS
void DownSampleHeader(WORD n, BYTE* s);
// Set the Sample in the Maui and in the XMS
void DownSample(WORD n, BYTE* s);

// Get the MultiSample information from XMS or MAUI
BOOL GetMultiSample(WORD n, BYTE* s);
// Download Multisample to the Maui and to the XMS
void DownMultiSample(WORD n, WORD m, BYTE* s);

// Get the Alias information from XMS or MAUI
BOOL GetAliasSample(WORD n, BYTE* s);
// Set the Alias Sample in the Maui and in the XMS
void DownAliasSample(WORD n, WORD m, BYTE* s);

// Free Up Sample and delete sample memory!
void FreeUpSample(int n);
// Clean Up Sample and delete sample memory!
void CleanUpSample(int n);

// Use program (set to used!)
void SetUsed(int n);

// Open the XMS if necessary
void GetOldXMS(void);

// Copy all card data to XMS
BOOL GetAllData(void);

// Search unused samples & patches
void GetUsed(int Current);
// Current = The current program (0-127) or the drum program (128)


// Get & allocate a free sample (return the sample number)
int SignFreeSample(void);


// Get the number of free samples
int GetFreeSamples(void);


// Get the number of Used samples
int GetUsedSamples(void);


// Get the number of not used but not free samples
int GetUnusedSamples(void);


// Get & allocate a free patch
int SignFreePatch(void);


// Get the number of free patches
int GetFreePatches(void);

// Set the Sample Header in the Maui and in the XMS
void DownSampleHeader(WORD n, BYTE* s);

// Download Multisample to the Maui and to the XMS
void DownMultiSample(WORD n, WORD m, BYTE* s);

// Convert INT stream to MIDI stream (NUM bytes) 14 bit = 7 + 7
void Int2Midi(BYTE* in, BYTE* out, WORD num);
// in, out = buffers
// num = number of bytes in "IN" buffer


// Convert WHAT into NUM bytes MIDI number (7 bits numbrs) & store to BUFTO
void Long2MIDI(BYTE* bufto, unsigned long what, BYTE num);


// Convert 2 bytes MIDI stream to 1 bytes (7 + 1 bit = 8 bit)
void Midi2Byte(BYTE* buf, BYTE num);
// buf = buffer
// num = destination stream size in bytes


// Convert NUM bytes stream to MIDI stream (2 x NUM bytes, 8 bit = 7 + 1)
void Byte2Midi(BYTE* in, BYTE* out, WORD num);
// in, out = buffers
// num = number of bytes in "IN" buffer


// MIDI2Long converts a NUM bytes MIDI stream to a longint number
unsigned long MIDI2Long(BYTE* buf,BYTE num);
// buf = buffer
// num = the number size in bytes

// GetCRC calculator
DWORD GetCRC(void);

// XMS

BOOL XMM_Installed();

BOOL XMM_AllocateExtended (WORD kbyte, WORD *handle);
BOOL XMM_FreeExtended(WORD handle);
BOOL XMM_MoveExtended(DWORD bytes, WORD src_handle, long src_addr,
		      WORD dest_handle,  long dest_addr);
BOOL XMM_GetHandleInfo(WORD handle, WORD *kbytes);

BOOL isTHSXMS(void);    // 1 = true, handle in XMSHandle
BOOL allocTHSXMS(void);  // 1 = ok
BOOL freeTHSXMS(void);   // 1 = ok

BOOL Prg2XMS(BYTE* Data, char* szName, WORD bNumber);
BOOL DPrg2XMS(BYTE* Data, WORD bNumber);
BOOL Patch2XMS(BYTE* Data, char* szName, WORD bNumber);
BOOL Sample2XMS(BYTE* Data, WORD nBytes, char* szName, DWORD dwMemory, BYTE nType, WORD bNumber);



#endif
