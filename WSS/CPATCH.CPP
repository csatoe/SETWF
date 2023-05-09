// This constructor will default all the values of the sampinfo object.
//
#include "stdafx.h"
#include "cpatch.h"

// CPatch functions
IMPLEMENT_SERIAL(CPatch, CObject, 1)

CPatch::CPatch()
{
	bPatchNumber = 0;
  nFreqBias = 0;
  bAmplitudeBias = 127;
  wSampleNumber = 0;
  bPortamento = 0;
  bPitchBend = 2;
  
  pfFlags.pf.fMono = FALSE;
  pfFlags.pf.fRetrigger = FALSE;
  pfFlags.pf.fNoSustain = TRUE;
  pfFlags.pf.fEnvReset = TRUE;
  pfFlags.pf.fFilterConfig = LPF2_HPF2;
  pfFlags.pf.fNewVoice = TRUE;
  pfFlags.pf.fLFOReset = TRUE;
  
  bFMSrc1 = LFO1;       
  bFMSrc2 = LFO2;
  cFMMultiplier1 = 32;
	cFMMultiplier2 = 0;
	bAMSrc = ENVELOPE2;
  cAMMultiplier = 0;
	bFC1Mod = LFO1;
	bFC2Mod = LFO1;
	cFC1ModMulti = 0;
	cFC1KbdScaling = 0;
  cFC1Bias = 0x00;
	cFC2ModMulti = 0;
	cFC2KbdScaling = 0x00;
	cFC2Bias = 0x00;
	bRandomizerRate = 0;

	eEnvelope1.bAttackTime = 0;
	eEnvelope1.bDecay1Time = 0;
	eEnvelope1.bDecay2Time = 0;
	eEnvelope1.bSustainTime = 0;
	eEnvelope1.bReleaseTime = 0;
	eEnvelope1.bRelease2Time = 0;
	eEnvelope1.bAttackLevel = 0;
	eEnvelope1.bDecay1Level = 0;
	eEnvelope1.bDecay2Level = 0;
	eEnvelope1.bSustainLevel = 0;
	eEnvelope1.bReleaseLevel = 0;
	eEnvelope1.bAttackVelocity = 0;
	eEnvelope1.bVolumeVelocity = 0;
	eEnvelope1.bKeyboardScaling = 0;

	eEnvelope2.bAttackTime = 0;
	eEnvelope2.bDecay1Time = 0;
	eEnvelope2.bDecay2Time = 0;
	eEnvelope2.bSustainTime = 127;
	eEnvelope2.bReleaseTime = 0;
	eEnvelope2.bRelease2Time = 0;
//	eEnvelope2.bRelease2Time = 64;	//test
	eEnvelope2.bAttackLevel = 127;
	eEnvelope2.bDecay1Level = 127;
	eEnvelope2.bDecay2Level = 127;
	eEnvelope2.bSustainLevel = 127;
	eEnvelope2.bReleaseLevel = -127;
	eEnvelope2.bAttackVelocity = 0;
	eEnvelope2.bVolumeVelocity = 127;
	eEnvelope2.bKeyboardScaling = 30;

	lLFO1.bSampleNumber = 0;
  lLFO1.bFrequency = 98;
	lLFO1.bFMSrc = LFO1;
  lLFO1.bAMSrc = MOD_WHEEL;
	lLFO1.cFMMultiplier = 0;
	lLFO1.cAMMultiplier = 64;
	lLFO1.bStartLevel = 0;
  lLFO1.bEndLevel = 0;
  lLFO1.fWaveRestart = 0;
  lLFO1.bRampDelay = 0;
  lLFO1.bRampTime = 0;

	lLFO2.bSampleNumber = 0;
  lLFO2.bFrequency = 0;
	lLFO2.bFMSrc = LFO1;
  lLFO2.bAMSrc = LFO1;
	lLFO2.cFMMultiplier = 0;
	lLFO2.cAMMultiplier = 0;
	lLFO2.bStartLevel = 0;
  lLFO2.bEndLevel = 0;
  lLFO2.fWaveRestart = FALSE;
  lLFO2.bRampDelay = 0;
  lLFO2.bRampTime = 0;
}


BOOL CPatch::operator==( CPatch& p )
{
  if( nFreqBias != p.nFreqBias )
  	return FALSE;
  if( bAmplitudeBias != p.bAmplitudeBias )
  	return FALSE;
  if( bPortamento != p.bPortamento )
  	return FALSE;              
  if( wSampleNumber != p.wSampleNumber )
  	return FALSE;
  if( bPitchBend != p.bPitchBend )
  	return FALSE;
  
  if( pfFlags.b != p.pfFlags.b )
  	return FALSE;

  if( bFMSrc1 != p.bFMSrc1 )       
  	return FALSE;
  if( bFMSrc2 != p.bFMSrc2 )
  	return FALSE;
  if( cFMMultiplier1 != p.cFMMultiplier1 )
  	return FALSE;
	if( cFMMultiplier2 != p.cFMMultiplier2 )
  	return FALSE;
	if( bAMSrc != p.bAMSrc )
  	return FALSE;
  if( cAMMultiplier != p.cAMMultiplier )
  	return FALSE;
	if( bFC1Mod != p.bFC1Mod )
  	return FALSE;
	if( bFC2Mod != p.bFC2Mod )
  	return FALSE;
	if( cFC1ModMulti != p.cFC1ModMulti )
  	return FALSE;
	if( cFC1KbdScaling != p.cFC1KbdScaling )
  	return FALSE;
  if( cFC1Bias != p.cFC1Bias )
  	return FALSE;
	if( cFC2ModMulti != p.cFC2ModMulti )
  	return FALSE;
	if( cFC2KbdScaling != p.cFC2KbdScaling )
  	return FALSE;
	if( cFC2Bias != p.cFC2Bias )
  	return FALSE;
	if( bRandomizerRate != p.bRandomizerRate )
		return(FALSE);

	if( eEnvelope1.bAttackTime != p.eEnvelope1.bAttackTime )
  	return FALSE;
	if( eEnvelope1.bDecay1Time != p.eEnvelope1.bDecay1Time )
  	return FALSE;
	if( eEnvelope1.bDecay2Time != p.eEnvelope1.bDecay2Time )
  	return FALSE;
	if( eEnvelope1.bSustainTime != p.eEnvelope1.bSustainTime )
  	return FALSE;
	if( eEnvelope1.bReleaseTime != p.eEnvelope1.bReleaseTime )
  	return FALSE;
	if( eEnvelope1.bRelease2Time != p.eEnvelope1.bRelease2Time )
  	return FALSE;
	if( eEnvelope1.bAttackLevel != p.eEnvelope1.bAttackLevel )
  	return FALSE;
	if( eEnvelope1.bDecay1Level != p.eEnvelope1.bDecay1Level )
  	return FALSE;
	if( eEnvelope1.bDecay2Level != p.eEnvelope1.bDecay2Level )
  	return FALSE;
	if( eEnvelope1.bSustainLevel != p.eEnvelope1.bSustainLevel )
  	return FALSE;
	if( eEnvelope1.bReleaseLevel != p.eEnvelope1.bReleaseLevel )
  	return FALSE;
	if( eEnvelope1.bAttackVelocity != p.eEnvelope1.bAttackVelocity )
  	return FALSE;
 	if( eEnvelope1.bVolumeVelocity != p.eEnvelope1.bVolumeVelocity )
  	return FALSE;
	if( eEnvelope1.bKeyboardScaling != p.eEnvelope1.bKeyboardScaling )
  	return FALSE;

	if( eEnvelope2.bAttackTime != p.eEnvelope2.bAttackTime )
  	return FALSE;
	if( eEnvelope2.bDecay1Time != p.eEnvelope2.bDecay1Time )
  	return FALSE;
	if( eEnvelope2.bDecay2Time != p.eEnvelope2.bDecay2Time )
  	return FALSE;
	if( eEnvelope2.bSustainTime != p.eEnvelope2.bSustainTime )
  	return FALSE;
	if( eEnvelope2.bReleaseTime != p.eEnvelope2.bReleaseTime )
  	return FALSE;
	if( eEnvelope2.bRelease2Time != p.eEnvelope2.bRelease2Time )
  	return FALSE;
	if( eEnvelope2.bAttackLevel != p.eEnvelope2.bAttackLevel )
  	return FALSE;
	if( eEnvelope2.bDecay1Level != p.eEnvelope2.bDecay1Level )
  	return FALSE;
	if( eEnvelope2.bDecay2Level != p.eEnvelope2.bDecay2Level )
  	return FALSE;
	if( eEnvelope2.bSustainLevel != p.eEnvelope2.bSustainLevel )
  	return FALSE;
	if( eEnvelope2.bReleaseLevel != p.eEnvelope2.bReleaseLevel )
  	return FALSE;
	if( eEnvelope2.bAttackVelocity != p.eEnvelope2.bAttackVelocity )
  	return FALSE;
	if( eEnvelope2.bVolumeVelocity != p.eEnvelope2.bVolumeVelocity )
  	return FALSE;
	if( eEnvelope2.bKeyboardScaling != p.eEnvelope2.bKeyboardScaling )
  	return FALSE;
	
	if( lLFO1.bSampleNumber != p.lLFO1.bSampleNumber )
  	return FALSE;
  if( lLFO1.bFrequency != p.lLFO1.bFrequency )
  	return FALSE;
	if( lLFO1.bFMSrc != p.lLFO1.bFMSrc )
  	return FALSE;
  if( lLFO1.bAMSrc != p.lLFO1.bAMSrc )
  	return FALSE;
	if( lLFO1.cFMMultiplier != p.lLFO1.cFMMultiplier )
  	return FALSE;
	if( lLFO1.cAMMultiplier != p.lLFO1.cAMMultiplier )
  	return FALSE;
	if( lLFO1.bStartLevel != p.lLFO1.bStartLevel )
  	return FALSE;
  if( lLFO1.bEndLevel != p.lLFO1.bEndLevel )
  	return FALSE;
  if( lLFO1.fWaveRestart != p.lLFO1.fWaveRestart )
  	return FALSE;
  if( lLFO1.bRampDelay != p.lLFO1.bRampDelay )
  	return FALSE;
  if( lLFO1.bRampTime != p.lLFO1.bRampTime )
  	return FALSE;

	if( lLFO2.bSampleNumber != p.lLFO2.bSampleNumber )
  	return FALSE;
  if( lLFO2.bFrequency != p.lLFO2.bFrequency )
  	return FALSE;
	if( lLFO2.bFMSrc != p.lLFO2.bFMSrc )
  	return FALSE;
  if( lLFO2.bAMSrc != p.lLFO2.bAMSrc )
  	return FALSE;
	if( lLFO2.cFMMultiplier != p.lLFO2.cFMMultiplier )
  	return FALSE;
	if( lLFO2.cAMMultiplier != p.lLFO2.cAMMultiplier )
  	return FALSE;
	if( lLFO2.bStartLevel != p.lLFO2.bStartLevel )
  	return FALSE;
  if( lLFO2.bEndLevel != p.lLFO2.bEndLevel )
  	return FALSE;
  if( lLFO2.fWaveRestart != p.lLFO2.fWaveRestart )
  	return FALSE;
  if( lLFO2.bRampDelay != p.lLFO2.bRampDelay )
  	return FALSE;
  if( lLFO2.bRampTime != p.lLFO2.bRampTime )
  	return FALSE;
	
	return TRUE;
}


void CPatch::Zero( void )
{
  nFreqBias = 0;
  bAmplitudeBias = 0;
  bPortamento = 0;
  wSampleNumber = 0;
  bPitchBend = 0;
  
  pfFlags.b = 0;

  bFMSrc1 = 0;       
  bFMSrc2 = 0;
  cFMMultiplier1 = 0;
	cFMMultiplier2 = 0;
	bAMSrc = 0;
  cAMMultiplier = 0;
	bFC1Mod = 0;
	bFC2Mod = 0;
	cFC1ModMulti = 0;
	cFC1KbdScaling = 0;
  cFC1Bias = 0;
	cFC2ModMulti = 0;
	cFC2KbdScaling = 0;
	cFC2Bias = 0;
	bRandomizerRate = 0;

	eEnvelope1.bAttackTime = 0;
	eEnvelope1.bDecay1Time = 0;
	eEnvelope1.bDecay2Time = 0;
	eEnvelope1.bSustainTime = 0;
	eEnvelope1.bReleaseTime = 0;
	eEnvelope1.bRelease2Time = 0;
	eEnvelope1.bAttackLevel = 0;
	eEnvelope1.bDecay1Level = 0;
	eEnvelope1.bDecay2Level = 0;
	eEnvelope1.bSustainLevel = 0;
	eEnvelope1.bReleaseLevel = 0;
	eEnvelope1.bAttackVelocity = 0;
	eEnvelope1.bVolumeVelocity = 0;
	eEnvelope1.bKeyboardScaling = 0;

	eEnvelope2.bAttackTime = 0;
	eEnvelope2.bDecay1Time = 0;
	eEnvelope2.bDecay2Time = 0;
	eEnvelope2.bSustainTime = 0;
	eEnvelope2.bReleaseTime = 0;
	eEnvelope2.bRelease2Time = 0;
	eEnvelope2.bAttackLevel = 0;
	eEnvelope2.bDecay1Level = 0;
	eEnvelope2.bDecay2Level = 0;
	eEnvelope2.bSustainLevel = 0;
	eEnvelope2.bReleaseLevel = 0;
	eEnvelope2.bAttackVelocity = 0;
	eEnvelope2.bVolumeVelocity = 0;
	eEnvelope2.bKeyboardScaling = 0;

	lLFO1.bSampleNumber = 0;
  lLFO1.bFrequency = 0;
	lLFO1.bFMSrc = 0;
  lLFO1.bAMSrc = 0;
	lLFO1.cFMMultiplier = 0;
	lLFO1.cAMMultiplier = 0;
	lLFO1.bStartLevel = 0;
  lLFO1.bEndLevel = 0;
  lLFO1.fWaveRestart = 0;
  lLFO1.bRampDelay = 0;
  lLFO1.bRampTime = 0;

	lLFO2.bSampleNumber = 0;
  lLFO2.bFrequency = 0;
	lLFO2.bFMSrc = 0;
  lLFO2.bAMSrc = 0;
	lLFO2.cFMMultiplier = 0;
	lLFO2.cAMMultiplier = 0;
	lLFO2.bStartLevel = 0;
  lLFO2.bEndLevel = 0;
  lLFO2.fWaveRestart = 0;
  lLFO2.bRampDelay = 0;
  lLFO2.bRampTime = 0;
}


BOOL CPatch::operator=( CPatch& p )
{
	bPatchNumber = p.bPatchNumber;
  nFreqBias = p.nFreqBias;
  bAmplitudeBias = p.bAmplitudeBias;
  bPortamento = p.bPortamento;
  wSampleNumber = p.wSampleNumber;
  bPitchBend = p.bPitchBend;
  pfFlags.b = p.pfFlags.b;
  bFMSrc1 = p.bFMSrc1;       
  bFMSrc2 = p.bFMSrc2;
  cFMMultiplier1 = p.cFMMultiplier1;
	cFMMultiplier2 = p.cFMMultiplier2;
	bAMSrc = p.bAMSrc;
  cAMMultiplier = p.cAMMultiplier;
	bFC1Mod = p.bFC1Mod;
	bFC2Mod = p.bFC2Mod;
	cFC1ModMulti = p.cFC1ModMulti;
	cFC1KbdScaling = p.cFC1KbdScaling;
  cFC1Bias = p.cFC1Bias;
	cFC2ModMulti = p.cFC2ModMulti;
	cFC2KbdScaling = p.cFC2KbdScaling;
	cFC2Bias = p.cFC2Bias;
	bRandomizerRate = p.bRandomizerRate;

	eEnvelope1.bAttackTime = p.eEnvelope1.bAttackTime;
	eEnvelope1.bDecay1Time = p.eEnvelope1.bDecay1Time;
	eEnvelope1.bDecay2Time = p.eEnvelope1.bDecay2Time;
	eEnvelope1.bSustainTime = p.eEnvelope1.bSustainTime;
	eEnvelope1.bReleaseTime = p.eEnvelope1.bReleaseTime;
	eEnvelope1.bRelease2Time = p.eEnvelope1.bRelease2Time;
	eEnvelope1.bAttackLevel = p.eEnvelope1.bAttackLevel;
	eEnvelope1.bDecay1Level = p.eEnvelope1.bDecay1Level;
	eEnvelope1.bDecay2Level = p.eEnvelope1.bDecay2Level;
	eEnvelope1.bSustainLevel = p.eEnvelope1.bSustainLevel;
	eEnvelope1.bReleaseLevel = p.eEnvelope1.bReleaseLevel;
	eEnvelope1.bAttackVelocity = p.eEnvelope1.bAttackVelocity;
 	eEnvelope1.bVolumeVelocity = p.eEnvelope1.bVolumeVelocity;
	eEnvelope1.bKeyboardScaling = p.eEnvelope1.bKeyboardScaling;

	eEnvelope2.bAttackTime = p.eEnvelope2.bAttackTime;
	eEnvelope2.bDecay1Time = p.eEnvelope2.bDecay1Time;
	eEnvelope2.bDecay2Time = p.eEnvelope2.bDecay2Time;
	eEnvelope2.bSustainTime = p.eEnvelope2.bSustainTime;
	eEnvelope2.bReleaseTime = p.eEnvelope2.bReleaseTime;
	eEnvelope2.bRelease2Time = p.eEnvelope2.bRelease2Time;
	eEnvelope2.bAttackLevel = p.eEnvelope2.bAttackLevel;
	eEnvelope2.bDecay1Level = p.eEnvelope2.bDecay1Level;
	eEnvelope2.bDecay2Level = p.eEnvelope2.bDecay2Level;
	eEnvelope2.bSustainLevel = p.eEnvelope2.bSustainLevel;
	eEnvelope2.bReleaseLevel = p.eEnvelope2.bReleaseLevel;
	eEnvelope2.bAttackVelocity = p.eEnvelope2.bAttackVelocity;
	eEnvelope2.bVolumeVelocity = p.eEnvelope2.bVolumeVelocity;
	eEnvelope2.bKeyboardScaling = p.eEnvelope2.bKeyboardScaling;
	
	lLFO1.bSampleNumber = p.lLFO1.bSampleNumber;
  lLFO1.bFrequency = p.lLFO1.bFrequency;
	lLFO1.bFMSrc = p.lLFO1.bFMSrc;
  lLFO1.bAMSrc = p.lLFO1.bAMSrc;
	lLFO1.cFMMultiplier = p.lLFO1.cFMMultiplier;
	lLFO1.cAMMultiplier = p.lLFO1.cAMMultiplier;
	lLFO1.bStartLevel = p.lLFO1.bStartLevel;
  lLFO1.bEndLevel = p.lLFO1.bEndLevel;
  lLFO1.fWaveRestart = p.lLFO1.fWaveRestart;
  lLFO1.bRampDelay = p.lLFO1.bRampDelay;
  lLFO1.bRampTime = p.lLFO1.bRampTime;

	lLFO2.bSampleNumber = p.lLFO2.bSampleNumber;
  lLFO2.bFrequency = p.lLFO2.bFrequency;
	lLFO2.bFMSrc = p.lLFO2.bFMSrc;
  lLFO2.bAMSrc = p.lLFO2.bAMSrc;
	lLFO2.cFMMultiplier = p.lLFO2.cFMMultiplier;
	lLFO2.cAMMultiplier = p.lLFO2.cAMMultiplier;
	lLFO2.bStartLevel = p.lLFO2.bStartLevel;
  lLFO2.bEndLevel = p.lLFO2.bEndLevel;
  lLFO2.fWaveRestart = p.lLFO2.fWaveRestart;
  lLFO2.bRampDelay = p.lLFO2.bRampDelay;
  lLFO2.bRampTime = p.lLFO2.bRampTime;
	
	return TRUE;
}



void CPatch::Serialize( CArchive& ar )
{
	CObject::Serialize( ar );

	if( ar.IsStoring() )     
  {
		ar << bPatchNumber;   
		ar << (WORD)nFreqBias;      
		ar << bAmplitudeBias; 
		ar << bPortamento;    
		ar << wSampleNumber;  
		ar << bPitchBend;     
		ar << pfFlags.b;
		ar << bFMSrc1;        
		ar << bFMSrc2;        
		ar << (BYTE)cFMMultiplier1; 
		ar << (BYTE)cFMMultiplier2; 
		ar << bAMSrc;         
		ar << (BYTE)cAMMultiplier;  
		ar << bFC1Mod;        
		ar << bFC2Mod;        
		ar << (BYTE)cFC1ModMulti;   
		ar << (BYTE)cFC1KbdScaling; 
		ar << (BYTE)cFC1Bias;       
		ar << (BYTE)cFC2ModMulti;   
		ar << (BYTE)cFC2KbdScaling; 
		ar << (BYTE)cFC2Bias;       
		ar << bRandomizerRate;
		eEnvelope1.Serialize( ar );
		eEnvelope2.Serialize( ar );
		lLFO1.Serialize( ar );
		lLFO2.Serialize( ar );
  }
  else
  {
		WORD w;
		BYTE b;		
		ar >> bPatchNumber;   
		ar >> w;
		nFreqBias = w;
		ar >> bAmplitudeBias; 
		ar >> bPortamento;    
		ar >> wSampleNumber;  
		ar >> bPitchBend;     
		ar >> pfFlags.b;
		ar >> bFMSrc1;        
		ar >> bFMSrc2;        
		ar >> b;
		cFMMultiplier1 = b;
		ar >> b;
		cFMMultiplier2 = b;
		ar >> bAMSrc;         
		ar >> b;
		cAMMultiplier = b;
		ar >> bFC1Mod;        
		ar >> bFC2Mod;        
		ar >> b;
		cFC1ModMulti = b;
		ar >> b;
		cFC1KbdScaling = b;
		ar >> b;
		cFC1Bias = b;
		ar >> b;
		cFC2ModMulti = b;
		ar >> b;
		cFC2KbdScaling = b;
		ar >> b;
		cFC2Bias = b;
		ar >> bRandomizerRate;
		eEnvelope1.Serialize( ar );
		eEnvelope2.Serialize( ar );
		lLFO1.Serialize( ar );
		lLFO2.Serialize( ar );
  }
}


// CLFO functions
IMPLEMENT_SERIAL(CLFO, CObject, 1)

CLFO::CLFO()
{
	bSampleNumber = 0;
  bFrequency = 0;
	bFMSrc = 0;
  bAMSrc = 0;
	cFMMultiplier = 0;
	cAMMultiplier = 0;
	bStartLevel = 0;
  bEndLevel = 0;
  fWaveRestart = 0;
  bRampDelay = 0;
  bRampTime = 0;
}


void CLFO::Serialize( CArchive& ar )
{
	CObject::Serialize( ar );
	
	if( ar.IsStoring() )     
  {
		ar << bSampleNumber;
		ar << bFrequency;    
		ar << bFMSrc;        
		ar << bAMSrc;        
		ar << (BYTE)cFMMultiplier; 
		ar << (BYTE)cAMMultiplier; 
		ar << bStartLevel;   
		ar << bEndLevel;     
		ar << (WORD)fWaveRestart;  
		ar << bRampDelay;    
		ar << bRampTime;     
  }
  else
  {
  	BYTE b;
  	WORD w;
		ar >> bSampleNumber; 
		ar >> bFrequency;    
		ar >> bFMSrc;        
		ar >> bAMSrc;        
		ar >> b;
		cFMMultiplier = b;
		ar >> b;
		cAMMultiplier = b;
		ar >> bStartLevel;   
		ar >> bEndLevel;     
		ar >> w;
		fWaveRestart = w;
		ar >> bRampDelay;    
		ar >> bRampTime;     
  }
}



// CEnvelope functions
IMPLEMENT_SERIAL(CEnvelope, CObject, 1)

CEnvelope::CEnvelope()
{
	bAttackTime = 0;
	bDecay1Time = 0;
	bDecay2Time = 0;
	bSustainTime = 0;
	bReleaseTime = 0;
	bRelease2Time = 0;
	bAttackLevel = 0;
	bDecay1Level = 0;
	bDecay2Level = 0;
	bSustainLevel = 0;
	bReleaseLevel = 0;
	bAttackVelocity = 0;
	bVolumeVelocity = 0;
	bKeyboardScaling = 0;
}


void CEnvelope::Serialize( CArchive& ar )
{
	CObject::Serialize( ar );

	if( ar.IsStoring() )     
  {
		ar << bAttackTime;     
		ar << bDecay1Time;     
		ar << bDecay2Time;     
		ar << bSustainTime;    
		ar << bReleaseTime;    
		ar << bRelease2Time;   
		ar << bAttackLevel;    
		ar << bDecay1Level;    
		ar << bDecay2Level;    
		ar << bSustainLevel;   
		ar << bReleaseLevel;   
		ar << bAttackVelocity; 
		ar << bVolumeVelocity; 
		ar << bKeyboardScaling;
  }
  else
  {
		ar >> bAttackTime;      
		ar >> bDecay1Time;       
		ar >> bDecay2Time;       
		ar >> bSustainTime;      
		ar >> bReleaseTime;     
		ar >> bRelease2Time;    
		ar >> bAttackLevel;      
		ar >> bDecay1Level;      
		ar >> bDecay2Level;      
		ar >> bSustainLevel;     
		ar >> bReleaseLevel;   
		ar >>	bAttackVelocity; 
		ar >> bVolumeVelocity; 
		ar >> bKeyboardScaling;
  }
}
