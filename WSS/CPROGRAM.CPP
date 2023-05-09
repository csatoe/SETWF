// This constructor will default all the values of the patch object.
//
#include "stdafx.h"
#include "cprogram.h"

IMPLEMENT_SERIAL(CProgram, CObject, 1)

CProgram::CProgram( BOOL fIsDrum )
{
	if ( fIsDrum )
	{
		prog.drum.bMIDINoteNumber = 60;
		prog.drum.bPatchNumber = 0;
		prog.drum.bMixLevel = 0x7F;
		prog.drum.fNotMuted = TRUE;
		prog.drum.bGroup = 0;							// assuming not grouped for now
		prog.drum.bPanModSource = PAN_NOTUSED;	// using fixed pan for now
		prog.drum.fPanModulated = FALSE;
		prog.drum.bPanSetorMulti = 4;	// fixed center pan
	}
	else
	{
		prog.inst.bProgramNumber = 0;
		
		// Initialize all layers of the program
		for (BYTE bCounter = 0; bCounter < NUMBER_OF_LAYERS; bCounter++)
			{
			prog.inst.lLayer[bCounter].bPatchNumber = 0;
			prog.inst.lLayer[bCounter].bMixLevel = 0x7F;
			prog.inst.lLayer[bCounter].fNotMuted = TRUE;
			prog.inst.lLayer[bCounter].bSplitpoint = 0;
			prog.inst.lLayer[bCounter].fSplitDown = FALSE;
			prog.inst.lLayer[bCounter].bPanModSource = PAN_NOTUSED;
			prog.inst.lLayer[bCounter].fPanModulated = FALSE;
			prog.inst.lLayer[bCounter].bPanSetorMulti = 4;
			prog.inst.lLayer[bCounter].fKey_Velocity = 0;
			}
		
		// Turn off layers 1 to 3 of the program	
		for ( int i = 1; i < NUMBER_OF_LAYERS ; i++ )
		{
			prog.inst.lLayer[i].fNotMuted = FALSE;
		}
	}
}

void CProgram::Serialize( CArchive& ar )
{
	if( ar.IsStoring() )     
  {
		ar.Write( &prog, sizeof( GENERIC_PROGRAM ) );
  }
  else
  {
		ar.Read( &prog, sizeof( GENERIC_PROGRAM ) );
  }
}


BOOL CProgram::operator=( CProgram& p )
{
	_fmemcpy( &prog , &(p.prog) ,  sizeof( GENERIC_PROGRAM )	);
	return TRUE;
}