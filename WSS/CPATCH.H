// This object will be used to store the neccessary data for restoring the
// synthesizer to a saved state.  The IMPLEMENT_SERIAL macro will allow us
// to write the list of objects to disk cleanly and easily.
#ifndef __CPATCH_H__
#define __CPATCH_H__


// define Modulator Sources
#define LFO1					(0)
#define LFO2					(1)
#define ENVELOPE1			(2)
#define ENVELOPE2			(3)
#define KEYBOARD 			(4)
#define KEYBOARD_LOG	(5)
#define VELOCITY    	(6)
#define VELOCITY_LOG	(7)
#define RANDOM        (8)
#define CHN_PRESSURE	(9)
#define MOD_WHEEL   	(10)
#define BREATH_CNTRL	(11)
#define FOOT_CNTRL   	(12)
#define VOLUME      	(13)
#define PAN         	(14)
#define EXPRESSION  	(15)

// define Pan Modulator Sources
#define PAN_KEYBOARD  (0)
#define PAN_VELOCITY  (1)
#define PAN_RANDOM    (2)
#define PAN_NOTUSED   (3) // fixed

// filter config
#define LPF2_HPF2 (0)   // 2 low pass and 2 high pass filters
#define LPF3_HPF1 (1)   // 3 low pass and 1 high pass filters
#define LPF2_LPF2 (2)   // 2 low pass and 2 low pass filters
#define LPF3_LPF1 (3)   // 3 low pass and 1 low pass filters


typedef union patch_flags_tag
	{
    BYTE  b;
    struct pfbits_tag
      {
        BYTE  fMono:1;        // only single notes allowed
        BYTE  fRetrigger:1;   // if not set during mono and second note is released 1st note plays
        BYTE  fNoSustain:1;   // when set the enveloe does not stop at Sustain level(percusion)
        BYTE  fEnvReset:1;    // force envelopes to restart from zero during retrigger
        BYTE  fFilterConfig:2;  // configure filters (see defines)
        BYTE  fNewVoice:1;    // set = always start a new voice, clr=try same channel...
        BYTE  fLFOReset:1;    // set= LFOs start at first sample, clr = random
      } pf;
  } PATCH_FLAGS;
  

class CEnvelope : public CObject
{
public:
	CEnvelope();

	DECLARE_SERIAL(CEnvelope);

	virtual void Serialize ( CArchive& ar );

  BYTE    bAttackTime;      // 7bits
  BYTE    bDecay1Time;      // 7bits
  BYTE    bDecay2Time;      // 7bits
  BYTE    bSustainTime;     // 7bits
  BYTE    bReleaseTime;     // 7bits
  BYTE    bRelease2Time;    // 7bits
  BYTE    bAttackLevel;     // 8bits
  BYTE    bDecay1Level;     // 8bits
  BYTE    bDecay2Level;     // 8bits
  BYTE    bSustainLevel;    // 8bits
  BYTE    bReleaseLevel;    // 8bits
  BYTE    bAttackVelocity;  // 7bits
  BYTE    bVolumeVelocity;  // 7bits
  BYTE    bKeyboardScaling; // 7bits
};


class CLFO : public CObject
{
public:
	CLFO();

	DECLARE_SERIAL(CLFO);

	virtual void Serialize ( CArchive& ar );

  BYTE  bSampleNumber;      // 8bits
  BYTE  bFrequency;         // 7bits 0-127
  BYTE  bFMSrc;             // 4bits
  BYTE  bAMSrc;             // 4bits
  char  cFMMultiplier;      // 8bits -127 to 127, mpyr for oscillator freq
  char  cAMMultiplier;      // 8bits -127 to 127, mpyr for oscillator freq
  BYTE  bStartLevel;        // 8bits 0-127
  BYTE  bEndLevel;          // 8bits 0-127
  BOOL  fWaveRestart;       // LFO 2 only
  BYTE  bRampDelay;         // 7bits 0-127
  BYTE  bRampTime;          // 7bits 0-127
};


class CPatch : public CObject
{
public:
	CPatch();

	void Zero( void );
	
	BOOL operator==( CPatch& );
	BOOL operator=( CPatch& );

	DECLARE_SERIAL(CPatch); 

	virtual void Serialize ( CArchive& ar );

  BYTE  bPatchNumber;       // 8 bits 0-255
  int   nFreqBias;          // 16 bits modifies the frequency relative to the MIDI note, LSB=1/2048 of octave
  BYTE  bAmplitudeBias;     // 7 bit in .5db steps, 127=0db, 0=-63.5db
  BYTE  bPortamento;        // 7 bit 0=none, 127=max
  WORD  wSampleNumber;      // 9 bit indicating which sample to play
  BYTE  bPitchBend;         // 4 bit unsigned 2=pitch bend of +/- 2 semitones

  PATCH_FLAGS pfFlags;      // 8 bits see structure

  BYTE  bFMSrc1;            // 4 bits modulator source for oscillator freq
  BYTE  bFMSrc2;            // 4 bits "
  char  cFMMultiplier1;     // 8 bits -127 to 127, mpyr for oscillator freq
  char  cFMMultiplier2;     // 8 bits "
  BYTE  bAMSrc;             // 4 bits modulator source for amplitude modulator
  char  cAMMultiplier;      // 8 bits -127 to 127, mpyr for oscillator amplitude
  BYTE  bFC1Mod;            // 4 bits modulator source for filter cut freq
  BYTE  bFC2Mod;            // 4 bits "
  char  cFC1ModMulti;       // 8 bits -127 to 127, mpyr for filter cut modulators
  char  cFC1KbdScaling;     // 8 bits -127 to 127, mpyr for fixed keyboard filter cut freq
  char  cFC1Bias;           // 8 bits -127 to 127, bias applied to filter cut freq
  char  cFC2ModMulti;       // 8 bits -127 to 127, mpyr for filter cut modulators
  char  cFC2KbdScaling;     // 8 bits -127 to 127, mpyr for fixed keyboard filter cut freq
  char  cFC2Bias;           // 8 bits -127 to 127, bias applied to filter cut freq
  BYTE  bRandomizerRate;    // 7 bits 0-127, 0=random at note time on only

  CEnvelope eEnvelope1;
  CEnvelope eEnvelope2;

  CLFO   lLFO1;
  CLFO   lLFO2;

};

#endif  // __CPATCH_H__

