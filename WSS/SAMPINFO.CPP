// This constructor will default all the values of the sampinfo object.
//
#include "stdafx.h"
#include "cpatch.h"
#include "cprogram.h"
#include "sampinfo.h"


IMPLEMENT_SERIAL(CSampleInfo, CObject, 1)


CSampleInfo::CSampleInfo()
{
	m_FileName = "default";
	m_SampleName = "default";

	m_fIsDrum = FALSE;
	m_fIsMono = TRUE; 
	m_fIsLooped = TRUE;
	TRACE("Creating CSampleInfo !!!\n");
//	TRACE("Name: %s Num: %c Dr: %u Mo: %u\n", m_FileName, m_Program.prog.inst.bProgramNumber, m_fIsDrum, m_fIsMono);
}

CSampleInfo::CSampleInfo(BOOL fIsDrum)
	:m_Program(fIsDrum)
{
	m_FileName = "default";
	m_SampleName = "default";

	m_fIsDrum = fIsDrum;
	m_fIsMono = TRUE;
	m_fIsLooped = TRUE;
	TRACE("Creating CSampleInfo !!!\n");
}

CSampleInfo::CSampleInfo(BOOL fIsDrum, CString File, CString Path, 
													BYTE ProgNum, BOOL fIsMono)
					:m_Program(fIsDrum)
{
	m_SampleName = File;
	m_FileName = Path;
	m_fIsDrum = fIsDrum;
	SetProgramNumber(ProgNum);
	m_fIsMono = fIsMono;
	m_fIsLooped = TRUE;
	TRACE("Creating CSampleInfo !!!\n");
}
													
CSampleInfo::~CSampleInfo()
{
	TRACE("DESTROYING CSampleInfo !!! Name: %s\n", m_FileName);
}

void CSampleInfo::Serialize( CArchive& ar )
{
	CObject::Serialize( ar );
	
	if( ar.IsStoring() )     
  {
		ar << m_FileName;
		ar << m_SampleName;
		ar <<	(WORD)m_fIsDrum;
		ar <<	(WORD)m_fIsMono;      
		ar << (WORD)m_fIsLooped;
/*		ar <<	m_bProgramNumber;
		ar << m_wSampleNumber;
		ar <<	m_wSampleNumber2;
		ar <<	m_wReserved2;
		ar <<	m_wReserved3;
		ar <<	m_wReserved4;*/
		m_Program.Serialize( ar );
		for (BYTE bCounter = 0; bCounter < PATCHES_PER_SAMPLE; bCounter++)
			{
			m_Patch[bCounter].Serialize( ar );
			}
  }
  else
  {
		WORD w;
		ar >> m_FileName;
		ar >> m_SampleName;
		ar >> w;
		m_fIsDrum = w;
		ar >> w;
		m_fIsMono = w;
		ar >> w;
		m_fIsLooped = w;
/*		ar >> m_bProgramNumber;
		ar >> m_wSampleNumber;
		ar >> m_wSampleNumber2;
		ar >> m_wReserved2;
		ar >> m_wReserved3;
		ar >> m_wReserved4;*/
		m_Program.Serialize( ar );
		for (BYTE bCounter = 0; bCounter < PATCHES_PER_SAMPLE; bCounter++)
			{
			m_Patch[bCounter].Serialize( ar );
			}
  }
}


void CSampleInfo::SetSampleNumber( WORD w, BYTE b )
{
	m_Patch[b].wSampleNumber = w;
}



void CSampleInfo::SetPatchVolume( BYTE bVol, BYTE bLayer )
{
	if ( m_fIsDrum )
	{
		m_Program.prog.drum.bMixLevel = bVol;
	}
	else
	{
		m_Program.prog.inst.lLayer[bLayer].bMixLevel = bVol;
	}
}


BYTE CSampleInfo::GetPatchVolume( BYTE bLayer )
{
	if ( m_fIsDrum )
	{
		return m_Program.prog.drum.bMixLevel;
	}
	else
	{
		return m_Program.prog.inst.lLayer[bLayer].bMixLevel;
	}
}

void CSampleInfo::SetPatchNumber( BYTE bNumber, BYTE bLayerNum )
{
	if ( m_fIsDrum )
	{
		m_Program.prog.drum.bPatchNumber = bNumber;
	}
	else
	{
		m_Program.prog.inst.lLayer[bLayerNum].bPatchNumber = bNumber;
	}
	m_Patch[bLayerNum].bPatchNumber = bNumber;
}


BYTE CSampleInfo::GetPatchNumber( BYTE bLayer )
{
	if ( m_fIsDrum )
	{
		return m_Program.prog.drum.bPatchNumber;
	}
	else
	{
		return m_Program.prog.inst.lLayer[bLayer].bPatchNumber;
	}
}


void CSampleInfo::SetProgramNumber( BYTE bNumber )
{
	if ( !m_fIsDrum )
	{
		m_Program.prog.inst.bProgramNumber = bNumber;
	}
	else
	{
		m_Program.prog.drum.bMIDINoteNumber = bNumber;
	}
}

BYTE CSampleInfo::GetProgramNumber( void )
{
	if ( !m_fIsDrum )
	{
		return(m_Program.prog.inst.bProgramNumber);
	}
	else
	{
		return(m_Program.prog.drum.bMIDINoteNumber);
	}
}



BOOL CSampleInfo::operator=( CSampleInfo& si )
{
	m_FileName = si.m_FileName;
	m_SampleName = si.m_SampleName;

	m_fIsDrum = si.m_fIsDrum;
	m_fIsMono = si.m_fIsMono;
	m_fIsLooped = si.m_fIsLooped;

	m_Program = si.m_Program;
	
	for ( int i = 0 ; i < PATCHES_PER_SAMPLE ; i++ )
	{
		m_Patch[i] = si.m_Patch[i];
	}

	return TRUE;
}
