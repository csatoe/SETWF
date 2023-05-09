// This object will be used to store the neccessary data for restoring the
// synthesizer to a saved state.  The IMPLEMENT_SERIAL macro will allow us
// to write the list of objects to disk cleanly and easily.

#ifndef __CPROGRAM_H__
#define __CPROGRAM_H__

// define Pan Modulator Sources
#define PAN_KEYBOARD  (0)
#define PAN_VELOCITY  (1)
#define PAN_RANDOM    (2)
#define PAN_NOTUSED   (3) // fixed

#define NUMBER_OF_LAYERS      (4)
#define MIDDLE_C							(60)


typedef struct layer_tag
  {
    BYTE  bPatchNumber;       // 8bits 0-255
    BYTE  bMixLevel;          // 7bits 0-127, 127=0db, -.5db steps
    BOOL  fNotMuted;          // clr=mute, set=notmuted
    BYTE  bSplitpoint;        // 7bits 0-127, for up note will play when >= split pt
                              //            for down note will play when < split pt
    BOOL  fSplitDown;         // clr=split is up, set split is down
    BYTE  bPanModSource;      // 2bits see defines
    BOOL  fPanModulated;      // set=pan is modulated, clr=pan is fixed
    BYTE  bPanSetorMulti;     // 4bits, if pan is fixed 0=left 7=right,
                              //        else -7 to 7 range for mpyr for pan modulator
    BOOL  fKey_Velocity;      // clr=keyboard, set = velocity
  } LAYER;

typedef struct program_tag
	{
  BYTE    bProgramNumber;      // 7 bits 0-127
  LAYER   lLayer[NUMBER_OF_LAYERS];
  }PROGRAM;


typedef struct edrum_program_tag
  {
  	BYTE	bMIDINoteNumber;		// 7bits 0-127, MIDI note of current enhanced drum program
    BYTE  bPatchNumber;       // 8bits 0-255
    BYTE  bMixLevel;          // 7bits 0-127, 127=0db, -.5db steps
    BOOL  fNotMuted;          //   clr=mute, set=notmuted
    BYTE  bGroup;             // 4bits 0=not grouped, 1-15=exclusive groups

    BYTE  bPanModSource;      // 2bits see defines
    BOOL  fPanModulated;      //   set=pan is modulated, clr=pan is fixed
    BYTE  bPanSetorMulti;     // 4bits, if pan is fixed 0=left 7=right,
                              //        else -7 to 7 range for mpyr for pan modulator
  }EDRUM_PROGRAM;


typedef	union generic_program_tag
	{
	PROGRAM inst;
	EDRUM_PROGRAM drum;
	} GENERIC_PROGRAM;



class CProgram : public CObject
{
public:
	CProgram( BOOL fIsDrum = FALSE );

	DECLARE_SERIAL(CProgram);

	virtual void Serialize ( CArchive& ar );
	BOOL operator=( CProgram& );

	GENERIC_PROGRAM prog;
};

#endif // __CPROGRAM_H__