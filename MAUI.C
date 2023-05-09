// MAUI.C

// Utility for accessing MAUI

#include "maui.h"
#include "names.h"

#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>

//  ***** Variables *****

const BOOL UseXMS=1; ///////// TRUE always. Not yet ready the routins...

BYTE nNoLoad=0; // Info level, download disabled


// Base Address
WORD MPUEmuData = MPUDEF;
// Midi interface usage  0 = HOST, -1 = MIDI
int MPUuseMIDI = 0;
BYTE IrqLine = 2;
BOOL TestIrq = 0;

const char* MPUErrors[] = {"",
			   "Bad Sample number",         //1
			   "Out of Sample memory",
			   "Bad patch number",
			   "Error in number of voices",
			   "",
			   "Sample load already in progress", //6
			   "","","","",
			   "No Sample load request pending",  //11
			   "","",
			   "Bad MIDI channel number",         //14
			   "",
			   "Download Record Error"            //16
			   };


// Sample types
const char* SampleType[4]=
      {"16 bit linear",
       "White noise",
       "8 bit linear",
       "8 bit MULaw"};

// Current version number (1.2), 1.1 is not allowed
const WORD WF_VERSION = 120;

// Comment length
const int  MAX_COMMENT = 64;

// Names of the WaveFront files
const char szBankFile[]    = "Bank";
const char szDrumkitFile[] = "DrumKit";
const char szProgramFile[] = "Program";

// File Identifiers
const UINT BANKFILE    = 0;
const UINT DRUMKITFILE = 1;
const UINT PROGRAMFILE = 2;
const UINT WAVEFILE    = 16;

const UINT WFSFILE     = 32;  // More command
const UINT WSSFILE     = 64;  // More sample

// Store the states
int Samples[512];
int Patches[256];


// Working signals
const char Work[4]={'|','/','-','\\'};
BYTE nWork=0;


// Standard STATIC buffers !!!
BYTE s[300],z[300],y[300];
BYTE sName[33]; // for the names

// Standard structure pointers, not buffers !!!
struct PROGRAM* Prg;
struct DRUM* Drum;
struct PATCH* Patch;

// Pointers to structures !!!
struct SAMPLE* SampleH;
struct ALIAS* Alias;

// Default setting for WAVE loading
const struct LAYER DefLayer={0,127,1,0,0,0,0,4,0};

const struct PROGRAM DefPrg={
      {
      {0,127,0,0,0,0,0,3,0},
      {0,127,0,0,0,0,0,3,0},
      {0,127,0,0,0,0,0,3,0},
      {0,127,0,0,0,0,0,3,0}
      }};

const struct PATCH DefPatch={
    0,127,0,0,0,0,2,0,0,0,0,0,0,0,0,0,1,0,32,0,3,0,0,2,0,0,0,127,0,0,127,0,0,
    //ENV 1
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    //ENV2
    {0,0,0,0,0,0,0,0,0,0,12,0,127,127,127,127,0,0,0,80,0,30,0},
    //LFO 1
    {0,98,0,10,0,0,64,0,0,0,0,0,0},
    //LFO 2
    {0,0,0,2,0,0,0,127,127,0,0,0,0}
  };

const struct DRUM DefDrum={0,127,0,0,0,0,1,3,0};


// from XMS

// The string searching in the XMS blocks
const char* THSsignature="SETWF 1.3x  THS";

const WORD THSXMSsize=(WORD)(((XXMSALL)+1024L)/1024L);

WORD XMM_Initialised=0;
void far *XMM_Control;
struct XMM_Move block;
BYTE btemp;
WORD wtemp;
WORD XMShandle;


//////////////////////////////////////////////////////////////////////////
// -------------------------- FUNCTIONS ----------------------------------
//////////////////////////////////////////////////////////////////////////

// ************************** HOST ************************************

static GoodINT=0;
static void (interrupt far *oldint)(void);


void interrupt host_int(void)
{
  if (inp(HOSTCTRL)&0x40)
  {
    GoodINT=1;
    outp(HOSTCTRL,0xe0);
    if (IrqLine>7) outp(0xa0,0x20);
    outp(0x20,0x20);
  }
  else
  {
    (*oldint)();				/* invoke original handler */
  }
}

//Test MAUI presents at BASE address
BOOL MPUTest(void)
{
  WORD j,m;

  if (MPUuseMIDI)
  {
    printf("\nþ Using MPU-401 MIDI interface");
    return !MPU401initHardware();
  }
  else
  {
    //Read previous bytes
    j=200;
    while (((inp(HOSTSTAT) & HOSTRXREGFUL) != 0) && j>0)
    {
      inp(HOSTDATA);
      delay(2);
      j--;
    }
    j=100;
    while ( 0 == (inp(HOSTSTAT)&HOSTTXREGEMP) && j>0)
    {
      j--;
      delay(1);
    }
    if (0==j) return 0;
    outp(HOSTDATA,0xff);

    j=100;
    while ( 0 == (inp(HOSTSTAT)&HOSTTXREGEMP) && j>0)
    {
      j--;
      delay(1);
    }
    if (0==j) return 0;

    outp(HOSTDATA,HCMDGETNOV);

    j=100;
    while ( 0 == (inp(HOSTSTAT)&HOSTRXREGFUL) && j>0)
    {
      j--;
      delay(1);
    }
    if (0==j || (m=inp(HOSTDATA))<16 || m>32) return 0;

    if (IrqLine==2) IrqLine=9;

    if (TestIrq)
    {
      BYTE save8259_a1,save8259_21;
      BYTE mask=(1<<(IrqLine&7))^0xff;
      WORD delay=60000;

      printf("\nþ Testing the required Interrupt Line %d ",IrqLine);

      if (IrqLine<9)
      {
	if (IrqLine!=2 && !((save8259_21=inp(0x21))&(1<<IrqLine)))
	{
	  printf(" ALREADY USED!");
	  return -1;
	}
	asm {
	  mov ah,0x35
	  mov al,IrqLine
	  add al,8
	  int 0x21
	  mov word ptr oldint,bx
	  mov word ptr oldint+2,es
	  mov ah,0x25
	  mov al,IrqLine
	  add al,8
	  push ds
	  mov dx,offset host_int
	  mov bx,seg host_int
	  mov ds,bx
	  int 0x21
	  pop ds
	  cli

	  mov al,save8259_21
	  and al,mask
	  out 0x21,al

	  mov dx,MPUEmuData
	  add dx,3
	  mov al,0xf0
	  out dx,al

	  sti
	}
      }
      else
      {
	save8259_21=inp(0x21);
	IrqLine-=8;
	if (!((save8259_a1=inp(0xa1))&(1<<IrqLine)))
	{
	  printf("ALREADY USED!");
	  return -1;
	}
	asm {
	  mov ah,0x35
	  mov al,IrqLine
	  add al,0x70
	  int 0x21
	  mov word ptr oldint,bx
	  mov word ptr oldint+2,es
	  mov ah,0x25
	  mov al,IrqLine
	  add al,0x70
	  push ds
	  mov dx,offset host_int
	  mov bx,seg host_int
	  mov ds,bx
	  int 0x21
	  pop ds
	  cli
	  //mov al,0x0a
	  //out 0xa0,al
	  //nop
	  //nop
	  //in al,0xa0

	  mov dx,MPUEmuData
	  add dx,3
	  mov al,0xf0
	  out dx,al

	  //in al,dx

	  mov al,save8259_21
	  and al,0xfb
	  out 0x21,al

	  mov al,save8259_a1
	  and al,mask
	  out 0xa1,al

	  mov al,IrqLine     //IrqLine+=8;
	  add al,8
	  mov IrqLine,al

	  //mov dx,MPUEmuData
	  //add dx,3
	  //in al,dx

	  //mov al,0x0a
	  //out 0xa0,al
	  //nop
	  //nop
	  //in al,0xa0

	  sti
	}
      }

      for (;!GoodINT && delay; delay--);

      outp(HOSTCTRL,0xe0);

      if (IrqLine>7) outp(0xa1,save8259_a1);
      outp(0x21,save8259_21);

      if (IrqLine>7) j=IrqLine+0x68; else j=IrqLine+8;
      asm {
	push ds
	mov ax,j
	mov ah,0x25
	mov dx,word ptr oldint
	mov ds,word ptr oldint+2
	int 0x21
	pop ds
      }

      if (GoodINT)
      {
	printf("OK");
	return 1;
      }
      else
      {
	printf("FAILED");
	return 0;
      }
    }
    return 1;
  }
}

//Sending Host Command to the MAUI & wait for answer
void HostCommand (BYTE* Buffer, WORD OutBytes, WORD InBytes)
{
  WORD i=0,j=0;

  // Read previous bytes, e.g. after an error the host may send bytes
  if (OutBytes>0)
  {
    while ((inp(HOSTSTAT) & HOSTRXREGFUL) != 0 )
    {
      inp(HOSTDATA);
      delay(10);
    }
  }

  //Send Command & receive Answer
  while (i<OutBytes || j<InBytes)
  {
    WAIT (HOSTSTAT, HOSTTXREGEMP|HOSTRXREGFUL );
    if (inp(HOSTSTAT)&HOSTTXREGEMP && i<OutBytes)
       outp(HOSTDATA,Buffer[i++]);

    if (inp(HOSTSTAT)&HOSTRXREGFUL && j<InBytes)
    {
      Buffer[j++]=inp(HOSTDATA);
      if (255==Buffer[0]) { 		//MPU error
	WAIT(HOSTSTAT, HOSTRXREGFUL);
	Buffer[1]=inp(HOSTDATA);
	//if (255==Buffer[1]) return 0;
	if (Buffer[1]!=1 && Buffer[1]!=255)
	{
	  printf("\nMPU ERROR: %d, %s",Buffer[1],MPUErrors[Buffer[1]]);
	  exit(2);
	}
	else break;
      }
    }
  }
}


//Sending Command to the card & wait for answer via MIDI interface
void MIDICommand (BYTE* Buffer, WORD OutBytes, WORD InBytes)
{
  WORD i=0,j=0;
  int offs=-1;
  WORD count=60000;

  // Read previous bytes, e.g. after an error the host may send bytes
  if (OutBytes>0)
  {
    while (!(inp(MPUSTAT) & MPU401_EMPTY))
    {
      inp(MPUEmuData);
      delay(1);
    }

    memmove(Buffer+6,Buffer,OutBytes);
    Buffer[6]&=0x7f;
    Buffer[6+OutBytes]=0xf7;
    Buffer[0]=0xf0;
    Buffer[1]=0;
    Buffer[2]=0;
    Buffer[3]=0x65;
    Buffer[4]=0x10;
    Buffer[5]=1;
    OutBytes+=7;
  }

  //Send Command & receive Answer
  while (i<OutBytes || j<InBytes)
  {
    while ((inp(MPUSTAT) & (MPU401_EMPTY|MPU401_BUSY))==(MPU401_EMPTY|MPU401_BUSY) && count>0)
	  if (j>0 || i==OutBytes) count--;

    if (j>0 || i==OutBytes) count--;

    if (!count) Error(24);

    if (!(inp(MPUSTAT)&MPU401_BUSY) && i<OutBytes)
    {
      outp(MPUEmuData,Buffer[i++]);
      count=60000;
    }

    if (!(inp(MPUSTAT)&MPU401_EMPTY) && j<InBytes)
    {
      count=60000;
      Buffer[j++]=inp(MPUEmuData);
      delay(1);

      if (Buffer[j-1]==0xf0)
      {
	InBytes+=6;
	offs=j-1;
      }

      if (0x7f==Buffer[6]) { 		//MPU error
	while (inp(MPUSTAT) & MPU401_EMPTY) ;
	Buffer[7]=inp(MPUEmuData);
	delay(1);
	while (inp(MPUSTAT) & MPU401_EMPTY) ;
	Buffer[8]=inp(MPUEmuData);
	delay(1);
	Buffer[6]+=0x80;
	//if (255==Buffer[1]) return 0;
	if (Buffer[7]!=1 && Buffer[7]!=0x7f)
	{
	  printf("\nMPU ERROR: %d, %s",Buffer[7],MPUErrors[Buffer[7]]);
	  exit(2);
	}
	else break;
      }
    }
  }
  if (offs!=-1) memmove(Buffer,Buffer+offs+6,j);
}


//Sending Command to the card & wait for answer
void Command (BYTE* Buffer, WORD OutBytes, WORD InBytes)
{
  BOOL MIDIdiff=0;

/*
  int i;

  BYTE Buf2[300];

  for(i=0;i<OutBytes;i++) printf("%i,",Buffer[i]);
  printf("\n");
  memcpy(Buf2,Buffer,OutBytes);
  HostCommand(Buf2, OutBytes, InBytes);
*/
  if (nNoLoad>0) return;

  if (!MPUuseMIDI) HostCommand(Buffer, OutBytes, InBytes);
  else
  {
    if (OutBytes>0)
    {
      switch (Buffer[0])
      {
	case HCMDGETVOL:
	case HCMDGETNOV:
	case HCMDGETTUN:
	//case HCMDDBLK:
	       InBytes++;
	       MIDIdiff=1;
	       break;
	case HCMDUPRG:
	case HCMDUEDP:
	       InBytes+=2;
	       MIDIdiff=2;
	       break;
	case HCMDUPATCH:
	case HCMDUSAMH:
	case HCMDUMSAM:
	case HCMDUSAMA:
	       InBytes+=3;
	       MIDIdiff=3;
	       break;
      }
    }
    MIDICommand(Buffer, OutBytes, InBytes); // via MIDI interface
    if (MIDIdiff) memmove(Buffer,Buffer+MIDIdiff,InBytes);
    InBytes-=MIDIdiff;
  }
/*
  for (i=0;i<InBytes && Buf2[i]==Buffer[i];i++);
  if (i<InBytes && Buf2[0]!=255)
  {
    for (i=0;i<InBytes;i++) printf("%i. %i<>%i, ",i,Buf2[i],Buffer[i]);
    if (getch()==0x27) exit(99);
  }
*/
}


// *************************** MPU functions ***************************

// write one byte to MPU-401 data port
int MPU401sendByte(BYTE value)
{
    register WORD timeout = MPUWait;
    register WORD statusport = MPUEmuData + 1;

    /* wait until the device is ready */
    for(;;)
    {
	//WORD delay;
	BYTE status = inp(statusport);	/* read status port */
	if ((status & MPU401_BUSY) == 0)
	    break;
	if (status & MPU401_EMPTY)
	    continue;
	_enable();
	//for (delay = 100; delay; delay--);
	delay(1);
	inp(MPUEmuData);		/* discard incoming data */
	if (--timeout == 0)
	    return -1;			/* port is not responding: timeout */
    }

    outp(MPUEmuData, value);		/* write value to data port */
    return 0;
}


/* get one byte from MPU-401 data port */
int MPU401getByte(void)
{
    register WORD timeout = MPUWait;
    register WORD statusport = MPUEmuData + 1;
    WORD delay;

    /* wait until the device is ready */
    for(;;)
    {
	BYTE status = inp(statusport);	/* read status port */

	if (!(status & MPU401_EMPTY))
	    break;
	if (--timeout == 0)
	    return -1;			/* port is not responding: timeout */
    }

    for (delay = 100; delay; delay--);
    delay=inp(MPUEmuData);

    return delay;
}


/* write block of bytes to MPU-401 port */
static int MPU401sendBlock(BYTE *block, WORD length)
{
    while (length--)
	MPU401sendByte(*block++);
    return 0;
}


/* write one byte to MPU-401 command port */
static int MPU401sendCommand(BYTE value)
{
    register WORD timeout;
    register WORD statusport = MPUEmuData + 1;

    /* wait until the device is ready */
    for(timeout = 0xFFFF;;)
    {
	if ((inp(statusport) & MPU401_BUSY) == 0) /* read status port */
	    break;
	if (--timeout == 0)
	    return -1;			/* port is not responding: timeout */
    }

    outp(statusport, value);		/* write value to command port */

    /* wait for acknowledging the command */
    for(timeout = 0xFFFF;;)
    {
	if ((inp(statusport) & MPU401_EMPTY) == 0) /* read status port */
	    if (inp(MPUEmuData) == MPU401_ACK)
		break;
	if (--timeout == 0)
	    return -1;			/* port is not responding: timeout */
    }

    return 0;
}


/* reset MPU-401 port */
static int MPU401reset(void)
{
    if (!MPU401sendCommand(MPU401_RESET))	/* first trial */
	return 0;
    return MPU401sendCommand(MPU401_RESET);	/* second trial */
}


int MPU401initHardware(void)
{
    if (MPU401reset())
	return -1;
    return MPU401sendCommand(MPU401_SET_UART);	/* set UART mode */
}


int MPU401deinitHardware(void)
{
    return MPU401sendCommand(MPU401_RESET);
}


// *************************** XMS *************************************

// If necessary open the XMS!
void GetOldXMS(void)
{
  DWORD l;
  extern BYTE AllocXMS;

  if (XMShandle==0)
  {
    if (isTHSXMS())
    {
      printf("\nþ Previously used Extended Memory found");
      fromXMS(XXMSMEMOFFS,&l,4); // Get the old free memory
      if (GetCRC()!=l)
      {
	printf(" - but modified!");
	if (!GetAllData()) Error(1);
      }
      else if (AllocXMS) // Load anyway
      {
	printf(" - now updating!");
	if (!GetAllData()) Error(1);
      }
      else // Load the 2 array
      {
	extern char XMSLoadFile[];
	if (XMSLoadFile[0] && !GetAllData()) Error(1);

	l=fromXMS(XSAMPLENEWOFFS,Samples,sizeof(Samples));
	if (!(l&&fromXMS(XPATCHNEWOFFS,Patches,sizeof(Patches)))) Error(1);
      }

    }
    else
    {
      printf("\nþ Allocating Extended Memory. (%ld bytes)",XXMSALL);
      if (!allocTHSXMS()) Error(1);
    }
  }
}

BOOL Get1Sample(BYTE* ss,BYTE bCopy, WORD m, int Add)
{
  WORD w;
  DWORD dwMemory;

  if (bCopy==1)
  {
    memcpy(y,ss,5);
  }
  else
  {
    y[0]=HCMDIDESAMT;
    Long2MIDI(y+1,m,2);
    Command(y,3,5);
  }
  memcpy(&dwMemory,y+1,4);
  switch (y[0])
  {
    case 2:
	   y[0]=HCMDUSAMA;
	   Long2MIDI(y+1,m,2);
	   Command(y,3,23);
	   w=MIDI2Long(y,2);
	   memcpy(y,&w,2);
	   Sample2XMS((BYTE*)y,24,(char*)SampleName[m],dwMemory,2,m);
	   if ((Samples[w]&SMASK)==EMPTY) Get1Sample(y,0,w,Add);
	   if ((Add==EMPTY) && (Samples[w]<INTERNAL)) Samples[w]=UNUSED;
	   else Samples[w]+=Add;
	   //w=MIDI2Long(s+16,3);
	   //memcpy(s+18,&w,2);
	   // Store to XMS
	   return 1;
    case 0:
	   if (Add==USED)
	   {
	     z[0]=HCMDUSAMH;
	     Long2MIDI(z+1,m,2);
	     Command(z,3,21);
	   }
	   else
	   {
	     memset(z,0,21);
	   }
	   // Store to XMS
	   return Sample2XMS((BYTE*)z,21,(char*)SampleName[m],dwMemory,0,m);
    default: ;
  }
  return 0;
}

// Get all information from the card (programs, patches, samples...)
BOOL GetAllData(void)
{
  WORD m,j,w,v,percent=0;
  DWORD dwMemory;
  int Add;
  extern char XMSLoadFile[128];

  // From File
  if (XMSLoadFile[0])
  {
    BYTE abuf[256];
    int fh;
    WORD ReadBytes=0;
    DWORD lv;

    if (!_dos_open(XMSLoadFile, O_RDONLY, &fh))
    {

      if (!_dos_read(fh, abuf, 16, &ReadBytes) &&
	 !strncmp(THSsignature,abuf,16))
      {
	lseek(fh,XXMSMEMOFFS,SEEK_SET);
	if (!_dos_read(fh, &lv, 4, &ReadBytes) && GetCRC()==lv)
	{

	  lseek(fh,0,SEEK_SET);
	  ReadBytes=0;

	  for (lv=0;lv<(XXMSALL-256) && !ReadBytes;lv+=256)
	  {
	    if (!(ReadBytes=_dos_read(fh, abuf, 256, &ReadBytes)))
	      toXMS(abuf,lv,256);
	  }

	  if (!ReadBytes && !(ReadBytes=_dos_read(fh, abuf, (XXMSALL)-lv, &ReadBytes)))
	     toXMS(abuf,lv,(XXMSALL)-lv);

	  if (!ReadBytes)
	  {
	    _dos_close(fh);
	    lv=fromXMS(XSAMPLENEWOFFS,Samples,sizeof(Samples));
	    if (!(lv && fromXMS(XPATCHNEWOFFS,Patches,sizeof(Patches)))) Error(1);
	    printf("\núúú Information loaded from file: %s OK",XMSLoadFile);
	    return 1; // OK BANK from file loaded
	  }
	}
	else // file-ban rossz a CRC de jo-e az XMS-ben?
	{
	  fromXMS(XXMSMEMOFFS,&lv,4); // Get the old free memory
	  if (GetCRC()==lv)
	  {
	    _dos_close(fh);
	    lv=fromXMS(XSAMPLENEWOFFS,Samples,sizeof(Samples));
	    if (!(lv && fromXMS(XPATCHNEWOFFS,Patches,sizeof(Patches)))) Error(1);
	    printf("\n! XMS file difference -- not loaded");
	    return 1; // Igen, jo az XMS
	  }
	}
      }
      _dos_close(fh);
    }
    printf("\n! XMS file error -- skipped");
  }

  printf("\núúú Getting information from the card: PROGRAMs ... ");
  //UNUSED PATCHES
  for (m=0;m<256;m++) Patches[m]=EMPTY;

  // Store the programs and search patches
  for (m=0;m<128;m++)
  {
    //putch(Work[nWork++&3]); putch(8);
#define PERC (int)(((float)(percent++)/(128.0+128.0+256.0+512.0))*100)
extern BYTE bPause;

    if (bPause) printf("%3d%%\b\b\b\b",PERC);
    s[0]=HCMDUPRG;
    s[1]=m;
    Command(s,2,32);
    Midi2Byte(s,16);
    Prg=(struct PROGRAM*)&s;
    // Save to the XMS
    if (!Prg2XMS((BYTE*)s,(char*)ProgramName[m],m)) return 0;
    for (j=0;j<4;j++)
    {
      if (Prg->layer[j].fUnmute) Patches[Prg->layer[j].byPatchNumber]+=USED;
    }
  }

  printf("\b\b\b\b\b\b\b\b\b\b\b\b\bDRUM KIT ... ");
  // Store the drumkit and search pathes
  for (m=0;m<128;m++)
  {
    printf("%3d%%\b\b\b\b",PERC);
    s[0]=HCMDUEDP;
    s[1]=m;
    Command(s,2,8);
    Midi2Byte(s,4);
    Drum=(struct DRUM*)&s;
    //Store to the XMS
    if (!DPrg2XMS((BYTE*)s,m)) return 0;
    if (Drum->fUnmute) Patches[Drum->byPatchNumber]+=USED;
  }

  // Search unused sample
  for (m=0;m<512;m++)
  {
    if (!strcmp(SampleName[m],"<Empty>"))
       Samples[m]=EMPTY;
    else
       Samples[m]=INTERNAL;
  }

  printf("\b\b\b\b\b\b\b\b\b\b\b\b\bPATCHes  ... ");
  for (m=0;m<256;m++)
  {
    printf("%3d%%\b\b\b\b",PERC);
    if (Patches[m]!=EMPTY)
    {
      s[0]=HCMDUPATCH;
      Long2MIDI(s+1,m,2);
      Command(s,3,132);
      if (s[0]!=255)
      {
	Midi2Byte(s,66);
	Patch=(struct PATCH*)&s;
	//Store to the XMS
	if (!Patch2XMS((BYTE*)s,(char*)PatchName[m],m)) return 0;

	Samples[Patch->bySampleNumber+256*Patch->fSampleMSB]+=USED;
      }
    }
    else // Unused patch - store the default
    {
      //Store to the XMS
      memset(s,0,66);
      if (!Patch2XMS(s,(char*)PatchName[m],m)) return 0;
    }
  }

  printf("\b\b\b\b\b\b\b\b\b\b\b\b\bSAMPLEs  ... ");
  for (m=0;m<512;m++)
  {
    printf("%3d%%\b\b\b\b",PERC);

    s[0]=HCMDIDESAMT;
    Long2MIDI(s+1,m,2);
    Command(s,3,5);
    memcpy(&dwMemory,s+1,4);

    if ((Samples[m]&SMASK)>EMPTY) Add=USED; else Add=EMPTY;

    switch (s[0])
    {
    case 127: // MIDI interface
    case 255: //Deleted Sample
	 memset(s,0,21);
	 if (!Sample2XMS(s,21,(char*)SampleName[m],0L,s[0],m)) return 0;
	 break;
    case 0: //Sample
	 Get1Sample(s,1,m,Add);
	 break;
    case 1: //Multisample
	 s[0]=HCMDUMSAM;
	 Long2MIDI(s+1,m,2);
	 Command(s,3,1);
	 Command(s+1,0,2<<s[0]);
	 for (j=1;j<(1<<s[0]);j++) {
	   w=MIDI2Long(s+(j*2-1),2);
	   for (v=j+1;v<(1<<s[0]) && MIDI2Long(s+(j*2-1),2)==MIDI2Long(s+(v*2-1),2);v++)
	   {
	     //memcpy(s+(v*2-1),&w,2);
	   }
	   //memcpy(s+(j*2-1),&w,2);
	   if ((Samples[w]&SMASK)==EMPTY) Get1Sample(s,0,w,Add);
	   if ((Add==EMPTY) && (Samples[w]<INTERNAL)) Samples[w]=UNUSED;
	   else Samples[w]+=Add;
	   // Store to XMS
	   j=v-1;
	 }
	 if (!Sample2XMS((BYTE*)s,(2<<s[0])+1,(char*)SampleName[m],dwMemory,1,m)) return 0;
	 break;
    case 2: //Alias
	 Get1Sample(s,1,m,Add);
	 break;
    }
  }

  printf("%3d%%\b\b\b\b",100);
  return 1;
}

//-------------------------------------------- PROGRAM --------------------

// Get the Program Name from the XMS or static
BOOL GetProgramName(WORD n)
{
  if (UseXMS && XMShandle)
  {
    return fromXMS(XPRGOFFS+n*XPRGSIZE+16,sName,32);
  }
  else strncpy(sName,ProgramName[n],32);
  return 1;
}


// Get the Program information from XMS or MAUI
BOOL GetProgram(WORD n, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    return fromXMS(XPRGOFFS+n*XPRGSIZE,s,16);
  }
  else
  {
    s[0]=HCMDUPRG;
    s[1]=n;
    Command(s,2,32);
    if (s[0]==255) return 0;
    Midi2Byte(s,16);
    return 1;
  }
}

// Down the DrumProgram to the Maui and to the XMS
void DownProgram(WORD n, BYTE* from, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    if (!Prg2XMS((BYTE*)from,sName,n)) Error(99);
  }
  s[0]=HCMDDPRG;
  s[1]=n;
  Byte2Midi(from,s+2,16);
  Command(s,34,1);
}

//-------------------------------------------- DRUM PROGRAM ---------------

// Get the DrumProgram information from XMS or MAUI
BOOL GetDrumProgram(WORD n, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    return fromXMS(XDPRGOFFS+n*XDPRGSIZE,s,4);
  }
  else
  {
    s[0]=HCMDUEDP;
    s[1]=n;
    Command(s,2,8);
    if (s[0]==255) return 0;
    Midi2Byte(s,4);
    return 1;
  }
}

// Down the DrumProgram to the Maui and to the XMS
void DownDrumProgram(WORD n, BYTE* from, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    if (!DPrg2XMS((BYTE*)from,n)) Error(99);
  }
  s[0]=HCMDDEDP;
  s[1]=n;
  Byte2Midi((BYTE*)from,s+2,4);
  Command(s,10,1);
}


//-------------------------------------------- PATCH -------------------

// Get the Sample Name from the XMS or static
BOOL GetPatchName(WORD n)
{
  if (UseXMS && XMShandle)
  {
    return fromXMS(XPATCHOFFS+n*XPATCHSIZE+66,sName,32);
  }
  else strncpy(sName,PatchName[n],32);
  return 1;
}


// Get the Patch information from XMS or MAUI
BOOL GetPatch(WORD n, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    return fromXMS(XPATCHOFFS+n*XPATCHSIZE,s,66);
  }
  else
  {
    s[0]=HCMDUPATCH;
    Long2MIDI(s+1,n,2);
    Command(s,3,132);
    if (s[0]==255) return 0;
    Midi2Byte(s,66);
    return 1;
  }
}

// Down the Patch to the Maui and to the XMS
void DownPatch(WORD n, BYTE* from, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    if (!Patch2XMS((BYTE*)from,sName,n)) Error(99);
  }
  s[0]=HCMDDPATCH;
  Long2MIDI(s+1,n,2);
  Byte2Midi(from,s+3,66);
  Command(s,135,1);
//  Patches[n]+=USED;
}

//-------------------------------------------- SAMPLE --------------------

// Get the Sample Name from the XMS or static
BOOL GetSampleName(WORD n)
{
  if (UseXMS && XMShandle)
  {
    return fromXMS(XSAMPLEOFFS+(long)n*XSAMPLESIZE+1L,sName,32);
  }
  else strncpy(sName,SampleName[n],32);
  return 1;
}

// Get the Sample type information from XMS or MAUI
BOOL GetSampleType(WORD n, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    n=fromXMS(XSAMPLEOFFS+(long)n*XSAMPLESIZE,s,2);
    //Lose the amount of memory...
    s[1]=0;
    s[2]=0;
    s[4]=0;
    s[5]=0;
    return n;
  }
  else
  {
    s[0]=HCMDIDESAMT;
    Long2MIDI(s+1,n,2);
    Command(s,3,5);
    return (s[0]!=255);
  }
}

// Get the Sample memory information from XMS or MAUI
DWORD GetOccMem(WORD n)
{
  DWORD dwMem;

  if (UseXMS && XMShandle)
  {
    fromXMS(XSAMPLEOFFS+(long)n*XSAMPLESIZE+33L,&dwMem,4);
  }
  else
  {
    s[0]=HCMDIDESAMT;
    Long2MIDI(s+1,n,2);
    Command(s,3,5);
    memcpy(&dwMem,s+1,4);
  }
  return dwMem;
}

// Get the Sample Header from XMS or MAUI
BOOL GetSampleHeader(WORD n, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    n=fromXMS(XSAMPLEOFFS+n*XSAMPLESIZE,s,XSAMPLESIZE);
    if (s[0]!=0) Error(99);
    memmove(s,s+39,s[37]+256*s[38]);
    return n;
  }
  else
  {
    s[0]=HCMDUSAMH;
    Long2MIDI(s+1,n,2);
    Command(s,3,21);
    return (s[0]!=255);
  }
}

// Set the Sample Header in the Maui and in the XMS
void DownSampleHeader(WORD n, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    if (!Sample2XMS((BYTE*)s+3,21,sName,GetOccMem(n),0,n)) Error(99);

  }
  s[0]=HCMDDSAMH;
  Long2MIDI(s+1,n,2);  //WFESI.nNumber
  Command(s,24,1);
}

// Set the Sample in the Maui and in the XMS
void DownSample(WORD n, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    if (!Sample2XMS((BYTE*)s+7,21,sName,GetOccMem(n),0,n)) Error(99);

  }
  s[0]=HCMDDSAM;
  Long2MIDI(s+1,n,2);  //WFESI.nNumber
  Command(s,28,1);
//  Samples[n]+=USED;
}


// Get the MultiSample information from XMS or MAUI
BOOL GetMultiSample(WORD n, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    n=fromXMS(XSAMPLEOFFS+(n)*XSAMPLESIZE,s,XSAMPLESIZE);
    if (s[0]!=1) Error(99);
    memmove(s,s+39,s[37]+256*s[38]);
    return n;
  }
  else
  {
    s[0]=HCMDUMSAM;
    Long2MIDI(s+1,n,2);
    if (s[0]==255) return 0;
    Command(s,3,1);
    Command(s+1,0,2<<s[0]);
    return 1;
  }
}

// Download Multisample to the Maui and to the XMS
void DownMultiSample(WORD n, WORD m, BYTE* s)
{
  Int2Midi(s+4,s+4,1<<m);

  s[3]=m;
  if (UseXMS && XMShandle)
  {
    if (!Sample2XMS((BYTE*)s+3,2<<m,sName,GetOccMem(n),1,n)) Error(99);

  }

  s[0]=HCMDDMSAM;
  Long2MIDI(s+1,n,2);  //WFESI.nNumber
  Command(s,(2<<m)+4,1);
//  Samples[n]+=USED;
}

// Get the Alias information from XMS or MAUI
BOOL GetAliasSample(WORD n, BYTE* s)
{
  if (UseXMS && XMShandle)
  {
    n=fromXMS(XSAMPLEOFFS+n*XSAMPLESIZE,s,XSAMPLESIZE);
    if (s[0]!=2) Error(99);
    memmove(s,s+39,s[37]+256*s[38]);
    return n;
  }
  else
  {
    s[0]=HCMDUSAMA;
    Long2MIDI(s+1,n,2);
    Command(s,3,23);
    return (s[0]!=255);
  }
}

// Set the Alias Sample in the Maui and in the XMS
void DownAliasSample(WORD n, WORD m, BYTE* s)
{
  s[0]=HCMDDSAMA;
  Long2MIDI(s+1,n,2);  //WFESI.nNumber
  memcpy(s+3,&m,2);
  if (UseXMS && XMShandle)
  {
    if (!Sample2XMS((BYTE*)s+3,23,sName,GetOccMem(n),2,n)) Error(99);
  }
  Long2MIDI(s+3,m,2);  //Original Sample
  Command(s,26,1);
//  Samples[n]+=USED;
}

///////////////////---------------------------------------------------

// Convert INT stream to MIDI stream (NUM bytes)
void Int2Midi(BYTE* in, BYTE* out, WORD num)
{
  int i;
  WORD j;

  for (i=0;i<num;i++)
  {
    j=in[i*2]+256*in[i*2+1];
    out[i*2]=j&127;
    out[i*2+1]=j>>7;
  }
}

// Convert NUM bytes stream to MIDI stream (2 x NUM bytes)
void Byte2Midi(BYTE* in, BYTE* out, WORD num)
{
  int i;

  for (i=0;i<num;i++) {
    out[i*2]=in[i]&127;
    out[i*2+1]=in[i]/128;
  }
}


// Clean Up Sample and delete sample memory!
void CleanUpSample(int n)
{
  BYTE s[20];
  //if ((Samples[n])<INTERNAL)
  //{
    Samples[n]=EMPTY;
    s[0]=HCMDDELSAM;
    Long2MIDI(s+1,n,2);
    Command(s,3,1);
  //}
}
///////////////////////////////// F R E E ////////////////////////////

// Free Up Sample and delete sample memory!
void FreeUpSample(int n)
{
  WORD w;

  GetSampleType(n,y);
  if (y[0]==2)
  {
    GetAliasSample(n,y);
    Alias=(struct ALIAS*)y;
    memcpy(&w,y,2);
    // Only free if the alias will free
    if ((Samples[n]&SMASK)==(EMPTY+1)) FreeUpSample(w);
  }

  if ((Samples[n]&SMASK)>EMPTY) Samples[n]-=USED;
  //Clear it
  if ((Samples[n])==EMPTY)
  {
    BYTE s[20];

    s[0]=HCMDDELSAM;
    Long2MIDI(s+1,n,2);
    Command(s,3,1);
  }
}


// Free Up Patch
void FreeUpPatch(int number)
{
  extern WORD n,w;

  if (Patches[number]>EMPTY) Patches[number]-=USED;
  GetPatch(number,z);
  Patch=(struct PATCH*)z;
  n=Patch->bySampleNumber+256*Patch->fSampleMSB;
  number=n;
  GetSampleType(n,z);
  switch (z[0])
  {
    case 0: //Sample: nothing to do
	 break;
    case 1: //Multisample
	 GetMultiSample(n,z);
	 for (n=1;n<(1<<z[0]);n++) {
	   for (w=n+1;w<(1<<z[0]) && MIDI2Long(z+(n*2-1),2)==MIDI2Long(z+(w*2-1),2);w++);
	   // Only free if the whole multisample will free
	   if ((Samples[number]&SMASK)==(EMPTY+1)) FreeUpSample(MIDI2Long(z+(n*2-1),2));
	   n=w-1;
	 }
	 break;
    case 2: //Alias
/*	 GetAliasSample(n,z);
	 Alias=(struct ALIAS*)z;
	 memcpy(&w,z,2);
	 // Only free if the alias will free
	 if ((Samples[number]&SMASK)==(EMPTY+1)) FreeUpSample(w);
*/
    default: break;
  } //switch
  FreeUpSample(number);
}


// Free up patches & samples
void FreeUp(int Current)
{
  WORD m,j;

  if (!XMShandle) GetOldXMS();

  // Free the program
  if (Current<128)
  {
    GetProgram(Current,s);
    Prg=(struct PROGRAM*)&s;
    for (j=0;j<4;j++)
    {
      if (Prg->layer[j].fUnmute) FreeUpPatch(Prg->layer[j].byPatchNumber);
    }
  }
  // Free up the drumkit
  else if (Current<DRUMKIT)
  {
    GetDrumProgram(Current&127,s);
    Drum=(struct DRUM*)&s;
    if (Drum->fUnmute) FreeUpPatch(Drum->byPatchNumber);
  }
  else if (Current==DRUMKIT)
  {
    for (m=0;m<128;m++)
    {
      GetDrumProgram(m,s);
      Drum=(struct DRUM*)&s;
      if (Drum->fUnmute) FreeUpPatch(Drum->byPatchNumber);
    }
  }
}

/////////////////////////////// U S E ////////////////////////////////

// Use Sample!
void SetUsedSample(int n)
{
  WORD w;

  if (Samples[n]==SIGNED) Samples[n]=EMPTY;

  GetSampleType(n,y);
  if (y[0]==2 && Samples[n]==EMPTY)
  {
    GetAliasSample(n,y);
    Alias=(struct ALIAS*)y;
    memcpy(&w,y,2);
    SetUsedSample(w);
  }

  Samples[n]+=USED;
}


// Use Patch
void SetUsedPatch(int number)
{
 extern WORD n,w;

  if (Patches[number]==SIGNED) Patches[number]=EMPTY;

  Patches[number]+=USED;
  if ((Patches[number]&SMASK)==USED)
  {
    GetPatch(number,z);
    Patch=(struct PATCH*)z;
    n=Patch->bySampleNumber+256*Patch->fSampleMSB;
    number=n;
    if ((Samples[n]&SMASK)==EMPTY)
    {
      GetSampleType(n,z);
      switch (z[0])
      {
	case 0: //Sample: nothing to do
	     break;
	case 1: //Multisample
	     GetMultiSample(n,z);
	     for (n=1;n<(1<<z[0]);n++) {
	       for (w=n+1;w<(1<<z[0]) && MIDI2Long(z+(n*2-1),2)==MIDI2Long(z+(w*2-1),2);w++);
	       SetUsedSample(MIDI2Long(z+(n*2-1),2));
	       n=w-1;
	     }
	     break;
	case 2: //Alias
/*	     GetAliasSample(n,z);
	     Alias=(struct ALIAS*)z;
	     memcpy(&w,z,2);
	     SetUsedSample(w);
*/	default: break;
      } //switch
    }
    SetUsedSample(number);
  }
}


// SetUsed patches & samples
void SetUsed(int Current)
{
  WORD m,j;

  if (!XMShandle) GetOldXMS();

  // Use the program
  if (Current<128)
  {
    GetProgram(Current,s);
    Prg=(struct PROGRAM*)&s;
    for (j=0;j<4;j++)
    {
      if (Prg->layer[j].fUnmute) SetUsedPatch(Prg->layer[j].byPatchNumber);
    }
  }
  else if (Current<DRUMKIT)
  {
    GetDrumProgram(Current,s);
    Drum=(struct DRUM*)&s;
    if (Drum->fUnmute) SetUsedPatch(Drum->byPatchNumber);
  }
  // Use the drumkit
  else if (Current==DRUMKIT)
  {
    for (m=0;m<128;m++)
    {
      GetDrumProgram(m,s);
      Drum=(struct DRUM*)&s;
      if (Drum->fUnmute) SetUsedPatch(Drum->byPatchNumber);
    }
  }
}

///----------------------------

// Get & allocate a free sample
int SignFreeSample(void)
{
  WORD i=0;

  for (;i<512&&Samples[i]!=EMPTY;i++);
  if (i!=512) Samples[i]=SIGNED;
  return i;
}

// Get the number of free samples
int GetFreeSamples(void)
{
  WORD i=0,j=0;

  for (;i<512;i++) if (Samples[i]==EMPTY) j++;
  return j;
}

// Get the number of Used samples
int GetUsedSamples(void)
{
  WORD i=0,j=0;

  for (;i<512;i++) if (Samples[i]>EMPTY) j++;
  return j;
}

// Get the number of not used but not free samples
int GetUnusedSamples(void)
{
  WORD i=0,j=0;

  for (;i<512;i++) if (Samples[i]==UNUSED) j++;
  return j;
}

// Get & allocate a free patch
int SignFreePatch(void)
{
  WORD i=0;

  for (;i<256&&Patches[i]!=EMPTY;i++);
  if (i!=256) Patches[i]=SIGNED;
  return i;
}

// Get the number of free patches
int GetFreePatches(void)
{
  WORD i=0,j=0;

  for (;i<256;i++) if (Patches[i]==EMPTY) j++;
  return j;
}

// Convert WHAT into NUM bytes MIDI number (7 bits numbrs) & store to BUFTO
void Long2MIDI(BYTE* bufto, unsigned long what, BYTE num)
{
  int i;

  for (i=0;i<num;i++)
  {
    bufto[i]=what&127;
    what/=128;
  }
}

// Convert 2 bytes MIDI stream to 1 bytes (7 + 1 bit = 8 bit)
void Midi2Byte(BYTE* buf, BYTE num)
{
  int i;

  for (i=0;i<num;i++) {
     buf[i]=buf[2*i]+buf[2*i+1]*128;
  }
}

// MIDI2Long converts a NUM bytes MIDI stream to a longint number
unsigned long MIDI2Long(BYTE* buf,BYTE num)
{
  unsigned long l=0;
  int i;

  for (i=num-1;i>=0;i--) l=128*l+buf[i];
  return l;
}


// GetCRC calculator
DWORD GetCRC(void)
{
  BYTE s[20];
  DWORD l;

  s[0]=HCMDREPFRE;  // Free mem +
  Command(s,1,4);
  l=MIDI2Long(s,4);

  s[0]=HCMDREPNOS;  // Number of samples
  Command(s,1,2);
  l+=s[0]+s[1]*128;

  return l;
}

BOOL XMM_Installed(void)
{
  if (XMM_Initialised) return XMM_Initialised;
  asm {
    mov ax,0x4300
    int 0x2f
    mov btemp,al
  }
  if (btemp==0x80)
  {
    asm {
      mov ax,0x4310
      int 0x2f
      mov word ptr XMM_Control,bx
      mov word ptr XMM_Control+2,es
    }
    XMM_Initialised=1;
    return 1;
  }
  else return 0;
}


BOOL XMM_AllocateExtended (WORD kbyte, WORD *handle)
{
  asm {
    mov ah,0x09
    mov dx,kbyte
    call [XMM_Control]

    mov bx,[handle]
    mov word ptr [bx],dx;

    mov wtemp,ax
  }
  return wtemp; // 1 = ok, 0 = error
}

BOOL XMM_FreeExtended(WORD handle)
{
  asm {
    mov ah,0x0a;
    mov dx,handle;
    call [XMM_Control];
    mov wtemp,ax
  }
  return wtemp; // 1 = free ok, 0 = error
}

BOOL XMM_MoveExtended(DWORD bytes, WORD src_handle, long src_addr,
		      WORD dest_handle, long dest_addr)
{
  BYTE wb;

  if (src_handle==dest_handle) Error(99);
  block.Length=bytes;
  block.SourceHandle=src_handle;
  block.SourceOffset=src_addr;
  block.DestHandle=dest_handle;
  block.DestOffset=dest_addr;
  asm {
    push ds
    push bp
    mov ah,0x0b;
    mov si,OFFSET block;
    mov bx,SEG block
    mov ds,bx
    call [XMM_Control];
    pop bp
    pop ds
    mov wtemp,ax
    mov wb,bl
  }
  if (!wtemp)
  {
    printf("\n%hd",wb);
    Error(99);
  }
  return wtemp; // 1 = Move OK, 0 = error
}

BOOL XMM_GetHandleInfo(WORD handle, WORD *kbytes)
{
  asm {
    mov ah,0x0e;
    mov dx,handle;
    call [XMM_Control];

    mov bx,[kbytes]
    mov word ptr [bx],dx;

    mov wtemp,ax
  }
  return wtemp; // 1 = there is a block... , 0 = no
}

BOOL isTHSXMS(void)
{
  WORD i,j;
  WORD size;
  char *temp="1234567890123456";

  if (!XMM_Installed()) return 0;

  if (XMM_AllocateExtended(0, &j) && XMM_FreeExtended(j) )
  {
    for (i=j-1300;i<=j+1300;i++)
    {
      XMShandle=i;
      if (XMM_GetHandleInfo(i,&size))
      {
	if (size==THSXMSsize)
	{
	  if (fromXMS(0,temp,16))
	  {
	    if (!strncmp(THSsignature,temp,16))
	    {
	      return XMShandle;
	    }
	  }
	}
      }
    }
  }
  XMShandle=0;
  return 0;
}

BOOL allocTHSXMS(void)
{
  if (!XMM_Installed()) return 0;

  if (XMM_AllocateExtended (THSXMSsize, &XMShandle))
  {
    toXMS(THSsignature,0,16);
    GetAllData();
    return 1;
  }
  else return 0;
}

BOOL freeTHSXMS(void)
{
  if (XMShandle!=0)
  {
    if (!XMM_Installed()) return 0;
    return XMM_FreeExtended (XMShandle);
  }
  return 1;
}

///////////////////////////////////////////////////////////

BOOL Prg2XMS(BYTE* Data, char* szName, WORD bNumber)
{
  BYTE tmp[XPRGSIZE];

  memset(tmp,0,XPRGSIZE);
  if (!szName[0]) sprintf(szName,"(noname program %d)",bNumber);
  memcpy(tmp,Data,16);
  strncpy(tmp+16,szName,32);
  return toXMS(tmp,XPRGOFFS+bNumber*XPRGSIZE,XPRGSIZE);
}

BOOL DPrg2XMS(BYTE* Data, WORD bNumber)
{
  BYTE tmp[XDPRGSIZE];

  memcpy(tmp,Data,4);
  return toXMS(tmp,XDPRGOFFS+bNumber*XDPRGSIZE,XDPRGSIZE);
}

BOOL Patch2XMS(BYTE* Data, char* szName, WORD bNumber)
{
  BYTE tmp[XPATCHSIZE];

  memset(tmp,0,XPATCHSIZE);
  if (!szName[0]) sprintf(szName,"(noname patch %d)",bNumber);
  memcpy(tmp,Data,66);
  strncpy(tmp+66,szName,32);
  return toXMS(tmp,XPATCHOFFS+bNumber*XPATCHSIZE,XPATCHSIZE);
}

BOOL Sample2XMS(BYTE* Data, WORD nBytes, char* szName, DWORD dwMemory, BYTE nType, WORD bNumber)
{
  BYTE tmp[XSAMPLESIZE];

  memset(tmp,0,XSAMPLESIZE);
  if (!szName[0]) sprintf(szName,"(noname sample %d)",bNumber);
  memcpy(tmp,&nType,1);
  strncpy(tmp+1,szName,32);
  memcpy(tmp+33,&dwMemory,4);
  memcpy(tmp+37,&nBytes,2);
  memcpy(tmp+39,Data,nBytes);
  return toXMS(tmp,XSAMPLEOFFS+bNumber*XSAMPLESIZE,XSAMPLESIZE);
}
