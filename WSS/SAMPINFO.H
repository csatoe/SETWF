// This object will be used to store the neccessary data for restoring the
// synthesizer to a saved state.  The IMPLEMENT_SERIAL macro will allow us
// to write the list of objects to disk cleanly and easily.
#ifndef __SAMPINFO_H__
#define __SAMPINFO_H__


// Defines for the default synth patch parameters
#define PATCHES_PER_SAMPLE		(2)

// Class definition

class CSampleInfo : public CObject
{
public:
	CSampleInfo();
	CSampleInfo(BOOL fIsDrum);
	CSampleInfo(BOOL, CString, CString, BYTE, BOOL);
  ~CSampleInfo();
  
	DECLARE_SERIAL(CSampleInfo); 

	virtual void Serialize ( CArchive& ar );
	BOOL operator=( CSampleInfo& NewSI );

	CString m_FileName;
	CString m_SampleName;

	BOOL m_fIsDrum;
	BOOL m_fIsMono;
	BOOL m_fIsLooped;

	CProgram m_Program;
	CPatch m_Patch[PATCHES_PER_SAMPLE];

	void SetSampleNumber( WORD w, BYTE b = 0 );

	WORD GetSampleNumber( BYTE bLayer = 0)
		{ return m_Patch[bLayer].wSampleNumber; }

	void SetRootKey( BYTE bRoot, BYTE bLayer = 0 )
		{ m_Patch[bLayer].nFreqBias = (int)( ( bRoot - MIDDLE_C) * 171 ); }

	BYTE GetRootKey( BYTE bLayer = 0 )
		{ return ( m_Patch[bLayer].nFreqBias / 171 + MIDDLE_C ); }

	void SetPatchVolume( BYTE bVol, BYTE bLayer = 0 );
	BYTE GetPatchVolume( BYTE bLayer = 0 );

	void SetPatchNumber( BYTE bPatchNumber, BYTE bPatchNum = 0 );
	BYTE GetPatchNumber( BYTE bLayer = 0 );

	void SetProgramNumber( BYTE bNumber );
	BYTE GetProgramNumber( void );
	
	void SetLoopEnable( BOOL fEnable )
		{ m_fIsLooped = fEnable; }
	BOOL GetLoopEnable( void )
		{ return m_fIsLooped; }

};



#endif // __SAMPINFO_H__