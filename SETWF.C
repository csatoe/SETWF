/* Ideas:

1.01
 -o --> keys: +/- volume, countter shows overflows OK
	      C - clear it                                          OK
	      other: exit                                              OK

1.02
 -Do not limit loading only Maui files...                              OK

1.03
  (handle Rio banks and Tropez banks)                OK
 - a remove error message from documentation         OK

1.10beta
 Error-11 - doksiba                                                    OK
 WAV betoltes: WAV,prgoram #, volume, root key                         OK

1.11beta
 16 bit WAVE - it could handle                                   OK
 Wave loading - big files not freezing, signal                   OK

1.15á
 -No erro message loading big files (>1 mega)                              OK
 -WAVE LOOPING new                                                   OK
 -wDBRE options
 -wx-y - LOOP points                                                OK

1.16beta
 -w option changed to WAVEfile,1,2,3,EBDRx-y 

1.20
 SETMAUI.H
 MAUI.H
 SETMAUI.C
 SETMAUI.H
 SETMAUI.PRJ

 SETMAUI.CFG

 Pause Mode modification
 Load modification
 file.wavP127 or file.wav,P127
 and option  -wP127V127N60EBDR0-127

1.21
 Load in one... -- -+

1.22
 setupsnd in the congif

1.23
 -p- bug fix in the p
 good sample deletion (delete and then get free space)

1.24
 WFS = @

==>1.29beta - big changes

!bug corrected:- Change Directory,
	       if SETUPSND start specified...
	      -w parameter did not refresh XMS!

1.29beta2 - This goes to ZAPHOD

1.30 == @ does not required by WFS

1.31 == no recursive WFS, error 23 + WFS processing error!

1.32 == parameter order change

     (95.09.23.)
1.33b == -ee FREE ALL SAMPLE used by internal too!!!
      This goes to Doctor Q

     (95.12.31)
1.34beta working through MIDI interface, but slow, e.g. *** R I O ***

1.35  /xs - save XMS /xl - load XMS

1.39  /r /q WAV(M) + WSS

1.40 bug fixes (/EE /E /W) and some default values (DefWavPrg, etc...)

1.41b buf fixes /W
     added /0 /6 Emu Mode Switch

1.42 add -Z info mode!

-------------
Not yet implemented
?    == AUTO port looking
     == PAT loading
     == WFD new
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dos.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <dir.h>

#include "setwf.h"

// This prints a DOS help
void Help(void)
{
  // available keys:     5 789 a    fg  jk             yz

  printf("\nUsage: SETWF.EXE [options] [filename]\n");
  printf("Parameters:\n");
  //Group 1
  //  /?
  printf("         -bx       - Set Base I/O Port, default: 330h\n");
  printf("         -mpu      - Use the slow MIDI interface\n");  //1.34
  printf("         -1        - Use the first (default) card described in SETWF.CFG\n"); //1.34
  printf("         -2 -3 -4  - Use the 2nd/3rd/4th card described in SETWF.CFG\n"); //1.34
  printf("         -p -pp -pd - Pause & debug; Pause; Debug mode\n");
  printf("         -z        - Don't download WFx files, only print infos\n");
  //Group 2
  printf("         -vx       - Set Volume, 0<=x<=127\n");
  printf("         -nx       - Set Number of Voices, 16<=x<=32\n");
  printf("         -tx       - Set Synthesizer Tuning, -8192<=x<=8191 (2048/12= 1/2 note)\n");
  printf("         -cex-y -cdx-y - Enable / Disable Synth Channels, 1<=x<=y<=16\n");
  printf("         -dex-y -ddx-y - Enable / Disable Drum Channels, 1<=x<=y<=16\n");
  printf("         -ie -id   - Enable / Disable MIDI-In to Synth\n");
  printf("         -me -md   - Enable / Disable Virtual MIDI Mode\n");
  printf("         -o        - Report Peak Output Levels\n");
  printf("         -lu       - Report Patches & Samples Status\n");
  printf("         -lhx-y    - Show structure 0<=x<=y<=255 (>127=DRUMKIT)\n");
  printf("         -lrx-y    - List Program(s), 0<=x<=y<=127\n");
  printf("         -lpx-y    - List Patch(es), 0<=x<=y<=255\n");
  printf("         -lsx-y    - List Sample(s), 0<=x<=y<=511\n");
  printf("         -ldx-y    - List Drum Program, 0<=x<=y<=127");
  Pause(PAUSEPAUSE);
  printf("\n         -u        - Delete All Unused Samples\n");
  printf("         -ex-y     - Delete Non-Internal Sample(s) 0<=x<=y<=511\n");
  printf("         -eex-y    - Delete Sample(s) /Internal too!/ 0<=x<=y<=511\n");
  //Group 3
  printf("\n         filename.WAV[,][Px][Vy][Kz][D|L|E][M][B|I][R|N][a-b] - SampleStore!\n"
	 "                   - Download WAVE file where\n"
	 "                       x = MIDI PRGOGRAM number (0-127, 128-255 DRUM)\n"
	 "                       y = VOLUME (0-127)\n"
	 "                       z = MIDI ROOT KEY (0-127)\n"
	 "                       L,E = Enable Looping\n"
	 "                       D = Disable Looping\n"
	 "                       M = Stereo to mono converting\n"  // 1.36
	 "                       B = Enable Bi-Directional Looping\n"
	 "                       I = Disable Bi-Directional Looping\n"
	 "                       R = Enable Reverse Sample Playing\n"
	 "                       N = Disable Reverse Sample Playing\n"
	 "                       a = Offset to Start of Loop (default: 0)\n"
	 "                       b = Offset to End of Loop (default: SampleSize)\n\n");
  printf("         -w[Px|Sx][Vy][D|L|E][B|I][R|N][a-b]\n"
	 "                   - Modify the parameters of the loaded WAVE file.\n\n\n");
  Pause(PAUSEPAUSE);
  printf("         filename[,]PxAySz\n"
	 "                   - DownLoad a .WFx file where\n"
	 "                       x = MIDI PRGOGRAM number (0-127)\n"
	 "                       y = Set First PATCH number to y\n"
	 "                       z = Set first SAMPLE Number to x\n\n");
  printf("         file.WFS  - process the WFS file (WaveFront File Set)\n\n");

  //Group 4
  printf("         -+        - Force SETWF to allocate & refresh Extended Memory\n");
  printf("         --        - Force SETWF to free up the used Extended memory\n");
  printf("         -xsfile   - Save the currently used Extended Memory to file\n");
  printf("         -xlfile   - Load the Extended Memory from file\n\n");

  printf("         -qxx      - If the card does not use IRQ xx it's reinitializing\n");
  printf("         -r        - Initialize the card calling SETUPSND\n");
  printf("         -!        - Do nothing only test the card (no IRQ test!)\n\n");
  printf("         -0        - Set the MIDI Emulation Mode to MPU-401\n");
  printf("         -6        - Set the MIDI Emulation Mode to 6850\n\n");

  //Group 5
  printf("         -s -sc    - Print card / Channel Status\n");
}


// Error procedure
void Error(int num)
{
  if ((num&1023)!=99) printf("\n ERROR:  %s",ErrorMsg[num&1023]);
  else printf("\n PROGRAM ERROR! Please report it to the author!");
  if (num<1024) CloseXMS();
  exit(16);
}


// Convert hexadecimal ascii digit to byte
BYTE aschex2byte(char n)
{
 if (n>='0' && n<='9') return n-'0';
 else if (toupper(n)>='A' && toupper(n)<='F') return n-'A'+10;
 else return 0;
}


// Get a range from a Command Line
void GetRange(char* source, WORD defmin, WORD defmax, int *min, int *max)
{
  *min=defmin;
  *max=defmax;
  if (source[0]==0) return;
  if (source[0]!='-') sscanf(source,"%d%i",min,max);
  else sscanf(source,"%i",max);
  *max=-*max;
  if (*min>defmax) Error(7);
  if (*max<*min)
  {
    if (strchr(source,'-')) *max=defmax;
    else *max=*min;
  }
}

void CloseXMS(void)
{
  // Return to the old directory
  chdir(dirtemp);

  if (CleanXMS && !XMShandle) isTHSXMS();

  if (XMShandle)
  {
    if (CleanXMS)
    {
      if (!freeTHSXMS()) Error(1024+1);
      printf("\nþ Freeing the used Extended Memory is done.");
    }
    else
    {
      //Store the actual free sample memory + number of sample
     if (nNoLoad==0) {
      lv=GetCRC();

      toXMS(&lv,XXMSMEMOFFS,4);

      // Clear the signed samples flag
      for (i=1; i<512; i++) if (Samples[i]==SIGNED)
      {
	CleanUpSample(i);
      }
      for (i=1; i<256; i++) if (Patches[i]==SIGNED) Patches[i]=EMPTY;

      //Store the 2 array
      toXMS(Samples,XSAMPLENEWOFFS,sizeof(Samples));
      toXMS(Patches,XPATCHNEWOFFS,sizeof(Patches));

      if (XMSSaveFile[0])
      {
	BYTE abuf[256];
	int fh;
	WORD WriteBytes;

	printf("\núúú Saving information to file: %s ",XMSSaveFile);
	if (_dos_creat(XMSSaveFile, 0, &fh) != 0) Error(1024+26);

	for (lv=0;lv<(XXMSALL-256);lv+=256)
	{
	  fromXMS(lv,abuf,256);
	  if (_dos_write(fh, abuf, 256, &WriteBytes) != 0) Error(1024+26);
	}
	fromXMS(lv,abuf,(XXMSALL)-lv);
	if (_dos_write(fh, abuf, (XXMSALL)-lv, &WriteBytes) != 0) Error(1024+26);
	_dos_close(fh);
	printf("OK");
      }
     }
    }
  }

  if (MPUuseMIDI)
  {
    while (!(inp(MPUSTAT) & MPU401_EMPTY))
    {
      inp(MPUEmuData);
      delay(1);
    }
  }
}

// Switch off pause or debug mode if the standard output is redirected
void IgnorePause(void)
{
  if (!(stdout->flags&_F_TERM)) bPause=PAUSENEVER;
}

// Pause or Debug Mode
void Pause(BYTE NowMode)
{
  char ch;

  if (bPause!=PAUSENEVER && bPause&NowMode)
  {
    BYTE x=wherex(),y=wherey();
    struct text_info ti;

    gettextinfo(&ti);
    gotoxy(76,y);
    textattr(ti.attribute | BLINK);
    cprintf("KEY\b\b\b");
    textattr(ti.attribute);
    if ((ch=getch())==27)
    {
      cprintf("   \r\n");
      CloseXMS();
      exit(0);
    }
    else if (toupper(ch)=='P')
    {
      bPause=0;
    }
    cprintf("   ");
    gotoxy(x,y);
  }
}

// To process the SETWF.CFG file
void ProcessConfig(char* argv[], int mode)
{
  static BYTE buf[100];
  FILE* handle;
  char name[60],param[40];

  // default values for forst call only

  if (mode==pmFirst)
  {
    BasePort1=0x330;
    Irq1=2;
    UseMPU1=0;
    BasePort2=0x330;
    Irq2=2;
    UseMPU2=0;
    BasePort2=0x330;
    Irq3=2;
    UseMPU2=0;
    BasePort2=0x330;
    Irq4=2;
    UseMPU2=0;
  }

  // default for all

  // Options for WFx downloading
  FirstPrg=UNDEFINED,
  FirstPatch=UNDEFINED,
  FirstSample=UNDEFINED;

  // Options for WAVE loading
  WavPrg=UNDEFINED,
  WavVol=UNDEFINED,
  WavKey=UNDEFINED;
  WavDrum=NO;

  WavLoopStart=UNDEFINED;
  WavLoopEnd=UNDEFINED;

  Loop=UNDEFINED,
  Bidirectional=UNDEFINED,
  Reverse=UNDEFINED,
  FromFile=YES,
  MonoSample=NO;

  // **** CONFIG file interpreter ****

  p=strrchr(argv[0],'.');
  strcpy(p,".CFG");

  if ((handle = fopen(argv[0], "rt" )) != NULL)
  {
    if (mode==pmFirst) printf("\nþ Using SETWF.CFG");
    while (!feof(handle))
    {
      if ((fgets(buf,128, handle)) == NULL) break;
      if (buf[0]!='\n' && buf[0]!=';')
      {
	sscanf(buf,"%s = %s",&name,&param);
	if (mode==pmFirst)
	{
	  if (!strcmpi(name,"BasePort")) sscanf(param,"%X",&BasePort1);
	  else if (!strcmpi(name,"BasePort1")) sscanf(param,"%X",&BasePort1);
	  else if (!strcmpi(name,"BasePort2")) sscanf(param,"%X",&BasePort2);
	  else if (!strcmpi(name,"BasePort3")) sscanf(param,"%X",&BasePort3);
	  else if (!strcmpi(name,"BasePort4")) sscanf(param,"%X",&BasePort4);
	  else if (!strcmpi(name,"Interrupt")) sscanf(param,"%d",&Irq1);
	  else if (!strcmpi(name,"Interrupt1")) sscanf(param,"%d",&Irq1);
	  else if (!strcmpi(name,"Interrupt2")) sscanf(param,"%d",&Irq2);
	  else if (!strcmpi(name,"Interrupt3")) sscanf(param,"%d",&Irq3);
	  else if (!strcmpi(name,"Interrupt4")) sscanf(param,"%d",&Irq4);
	  else if (!strcmpi(name,"UseMPU")) sscanf(param,"%d",&UseMPU1);
	  else if (!strcmpi(name,"UseMPU1")) sscanf(param,"%d",&UseMPU1);
	  else if (!strcmpi(name,"UseMPU2")) sscanf(param,"%d",&UseMPU2);
	  else if (!strcmpi(name,"UseMPU3")) sscanf(param,"%d",&UseMPU3);
	  else if (!strcmpi(name,"UseMPU4")) sscanf(param,"%d",&UseMPU4);
	  else if (!strcmpi(name,"Pause")) sscanf(param,"%d",&bPause);
	  else if (!strcmpi(name,"Setupsnd"))
	  {
	    strcpy(setupsnd,strchr(buf,'=')+1);
	    while (setupsnd[0]==' ') strcpy(setupsnd,setupsnd+1);
	    strrchr(setupsnd,10)[0]=0;
	    strrchr(setupsnd,13)[0]=0;
	  }
	  else if (!strcmpi(name,"MidiPlayer"))
	  {
	    strcpy(midiplayer,strchr(buf,'=')+1);
	    while (midiplayer[0]==' ') strcpy(midiplayer,midiplayer+1);
	    strrchr(midiplayer,10)[0]=0;
	    strrchr(midiplayer,13)[0]=0;
	  }
	}
	if (mode!=pmChange)
	{
	  if (!strcmpi(name,"DefProgram")) sscanf(param,"%d",&DefFirstPrg);
	  else if (!strcmpi(name,"DefPatch")) sscanf(param,"%d",&DefFirstPatch);
	  else if (!strcmpi(name,"DefSample")) sscanf(param,"%d",&DefFirstSample);
	  else if (!strcmpi(name,"Program")) sscanf(param,"%d",&DefWavPrg);
	  else if (!strcmpi(name,"Volume")) sscanf(param,"%d",&WavVol);
	  else if (!strcmpi(name,"RootKey")) sscanf(param,"%d",&WavKey);
	  else if (!strcmpi(name,"Looping")) sscanf(param,"%d",&Loop);
	  else if (!strcmpi(name,"Reverse")) sscanf(param,"%d",&Reverse);
	  else if (!strcmpi(name,"Bidirectional")) sscanf(param,"%d",&Bidirectional);
        }
      }
    }
    if (WavPrg>127)
    {
      WavPrg&=127;
      WavDrum=YES;
    }
    fclose(handle);
  }
}


//
void WaveParams(char* p)
{
  while (p[0]!=0 && (p[0]<'0' || p[0]>'9'))
  {
    switch (toupper(p[0]))
    {
      case 'P': // Program
		sscanf(++p,"%d",&FirstPrg);
		WavPrg=FirstPrg;
		if (WavPrg>127)
		{
		  WavPrg&=127;
		  WavDrum=YES;
		}
		else WavDrum=NO;
		if (FirstPrg==128) FirstPrg=0;
		while(p[0]>='0'&&p[0]<='9') p++;
		printf("\nþ General MIDI Program number = %d (%s)",WavPrg,ProgramName[WavPrg]);
		break;
      case 'A': // Patch
		sscanf(++p,"%d",&FirstPatch);
		if (FirstPatch<0 || FirstPatch>255) Error(7);
		while(p[0]>='0'&&p[0]<='9') p++;
		printf("\nþ WaveFront PATCH number = %d",FirstPatch);
		break;

      case 'V': // Volume
		sscanf(++p,"%d",&WavVol);
		if (WavVol<0 || WavVol>127) Error(7);
		while(p[0]>='0'&&p[0]<='9') p++;
		printf("\nþ LAYER volume = %d",WavVol);
		break;
      case 'S': // Sample
		sscanf(++p,"%d",&FirstSample);
		if (FirstSample<0 || FirstSample>511) Error(7);
		while(p[0]>='0'&&p[0]<='9') p++;
		printf("\nþ WaveFront SAMPLE number = %d",FirstSample);
		break;
      case 'K': // MIDI Key
		sscanf(++p,"%d",&WavKey);
		if (WavKey<0 || WavKey>127) Error(7);
		while(p[0]>='0'&&p[0]<='9') p++;
		printf("\nþ MIDI Root Key: \t%d (%s%d)",WavKey,NoteName[WavKey%12],(int)(WavKey/12-1));
		break;
      case 'E':
      case 'L':
		Loop=YES;
		FromFile=NO;
		printf("\nþ Looping Enabled.");
		p++;
		break;
      case 'D':
		FromFile=NO;
		Loop=NO;
		printf("\nþ Looping Disabled.");
		p++;
		break;
      case 'M':
		MonoSample=YES;
		printf("\nþ Force Mono Sample.");
                p++;
                break;
      case 'B':
                Bidirectional=YES;
                Loop=YES;
		printf("\nþ Bi-Directional Looping.");
                p++;
                break;
      case 'I':
                Bidirectional=NO;
                printf("\nþ One-Directional Looping.");
                p++;
                break;
      case 'R':
		Reverse=YES;
		printf("\nþ Reverse Sample Playing.");
                p++;
		break;
      case 'N':
		Reverse=NO;
                printf("\nþ Normal Sample Playing.");
		p++;
                break;
      default:  p++;
    }
  }

  if (p[0]!=0)
  {
    sz=strchr(p,'-');
    if (sz==NULL || sz[1]==0) Error(7);
    sscanf(p,"%ld-%ld",&WavLoopStart,&WavLoopEnd);
    if (WavLoopStart<0 || WavLoopStart>WavLoopEnd) Error(7);
    printf("\nþ Loop Points: \t%ld - %ld",WavLoopStart,WavLoopEnd);
  }
}


// Modify Sample Header
void ModifySamHead(WORD sample)
{
  GetSampleType(sample,z);
  switch (z[0])
  {
    case 0: //Sample
	 GetSampleName(sample);
	 printf("\n    ÀÄSAMPLE # %d = %s",sample,sName);
	 GetSampleHeader(sample,z+3);

	 SampleH=(struct SAMPLE*)&z+1;

	 if (WavLoopStart!=UNDEFINED)
	 {
	   Long2MIDI(z+7,16*WavLoopStart,4);
	   Long2MIDI(z+11,16*WavLoopEnd,4);
	 }

	 w=MIDI2Long(z+19,3);
	 if (WavKey!=UNDEFINED)
	 {
	   Long2MIDI(z+19,(long)(w+(WavKey*2048.0/12.0)),3);
	 }
	 if (Loop==YES) z[22]|=8; else if (Loop==NO) z[22]&=(255-8);
	 if (Bidirectional==YES) z[22]|=16; else if (Bidirectional==NO) z[22]&=(255-16);
	 if (Reverse==YES) z[22]|=64; else if (Reverse==NO) z[22]&=(255-64);

	 z[23]=0;
	 GetSampleName(sample);
	 DownSampleHeader(sample,z);
	 break;
    default: break;
  } //switch

}

// Put together a good name from the filename
void GetName(char* FileName, BYTE chn, BYTE j)
{
  if (isWSS)
  {
    FileName=WSS.SampleName;
    if (WSS.fIsMono && chn!=0)
    {
      chn=1;
      j=0;
    }
  }

  if (strrchr(FileName,'\\'))
  {
    strcpy(sName,strrchr(FileName,'\\')+1);
  }
  else if (strrchr(FileName,'/'))
  {
    strcpy(sName,strrchr(FileName,'/')+1);
  }
  else if (strrchr(FileName,':'))
  {
    strcpy(sName,strrchr(FileName,':')+1);
  }
  else strcpy(sName,FileName);
  switch (chn)
  {
    case 2:
	   if (j==0) strcat(sName," (LEFT)");
	   else strcat(sName," (RIGHT)");
	   break;
    case 1:
	   strcat(sName," (MONO)");
	   break;
    default: ;
  }
}

int main (int argc, char* argv[])
{

  DWORD WavSizeInBytes;
  DWORD WavSizeInSamples;

  char FileName[260];           // file names
  char WaveFile[260];

  int fh,fw,WSSfh;              // file handlers

  WORD ReadBytes;               // Read Bytes

  BYTE *aBuf;                   // Allocated buffer

  BYTE FileType;

  //

  printf("\n SETWF -- WaveFront Control DOS Utility. Version 1.42b (C) 1995-96 Endre Csat¢\n"
	 "ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ");

  ProcessConfig(argv,pmFirst);

  if (argc==1)
  {
    printf("\n Usage: SETWF.EXE [options] [filename] or /? to help\n");
    exit(0);
  }
  for (i=1;i<argc;i++)
  {
    if (argv[i][0]=='/' || argv[i][0]=='-')
    {
      switch (toupper(argv[i][1]))
      {
	case '?':
	case 'H':
	     Help();
	     exit(0);

	case '1':
		   MPUEmuData=BasePort1;
                   IrqLine=Irq1;
		   MPUuseMIDI=UseMPU1;
		   printf(" -- configuration 1");
		   break;
	case '2':
		   MPUEmuData=BasePort2;
		   IrqLine=Irq1;
		   MPUuseMIDI=UseMPU2;
		   printf(" -- configuration 2");
		   break;
	case '3':
		   MPUEmuData=BasePort3;
		   IrqLine=Irq1;
		   MPUuseMIDI=UseMPU3;
		   printf(" -- configuration 3");
		   break;
	case '4':
		   MPUEmuData=BasePort4;
		   IrqLine=Irq1;
		   MPUuseMIDI=UseMPU4;
		   printf(" -- configuration 4");
		   break;

	case 'B':
	     MPUEmuData=aschex2byte(argv[i][2])*256+aschex2byte(argv[i][3])*16+aschex2byte(argv[i][5]);
	     switch (MPUEmuData) {
	       case 0x330:
	       case 0x338:
	       case 0x320:
	       case 0x300:
	       case 0x290:
	       case 0x260:
	       case 0x230:
	       case 0x210: break;
	       default: Error(2);
	     }
	     printf("\nþ Base I/O Port: %Xh",MPUEmuData);
	     break;

	case 'M':
	     if (toupper(argv[i][2])=='P' && toupper(argv[i][3])=='U')
		MPUuseMIDI=-1;
	     break;

	case 'P':
	     if (bPause!=PAUSENEVER) {
	       switch (toupper(argv[i][2])) {
		 case 'P':
		      printf("\nþ Pause mode enabled only");
		      bPause=PAUSEPAUSE;
		      break;
		 case 'D':
		      printf("\nþ Debug mode enabled only");
		      bPause=PAUSEDEBUG;
		      break;
		 case '-':
		      printf("\nþ Pause and Debug mode disabled");
		      bPause=PAUSENEVER;
		      break;
		 default:
		      printf("\nþ Pause and Debug mode enabled");
		      bPause=PAUSEPAUSE|PAUSEDEBUG;
		      break;
	       }
	     }
	     break;
	case '-': CleanXMS=1;
	     break;
	case '+': AllocXMS=1;
	     break;
	case '!':
	     break;
	case 'X':  //1.35
	     if (toupper(argv[i][2])=='L') sscanf(argv[i]+3,"%s",&XMSLoadFile);
	     if (toupper(argv[i][2])=='S') sscanf(argv[i]+3,"%s",&XMSSaveFile);
	     break;
	case 'Q':  //1.36 IRQ - test
	     if (argv[i][2]) sscanf(argv[i]+2,"%d",&IrqLine);
	     TestIrq=1;
	     break;
	case 'R':  //1.36 RESET - reset without test
	     Reset=1;
	     break;
	case 'Z':  //1.42 Show infos about WFx - no downloading
	     sscanf(argv[i]+2,"%d",&nNoLoad);
	     printf("\nþ WFx file info level = %d, downloading disabled.",nNoLoad);
	     break;
	default: ;
      }
    }
  } //for

  // Save the current directory
  dirtemp[0]=getdisk()+'A';
  dirtemp[1]=':';
  dirtemp[2]='\\';
  getcurdir(0,dirtemp+3);

  IgnorePause();

  if (nNoLoad==0 && (MPUTest()==0 || Reset))
  {
    if (setupsnd[0])
    {
      printf("\nþ Setting UP the card...");
      strcpy(s,"/c ");
      strcat(s,setupsnd);
      sprintf(s+strlen(s)," /a%x /i%i\0",MPUEmuData,IrqLine);
      spawnl(P_WAIT,getenv("COMSPEC"),getenv("COMSPEC"),s,NULL);
      if (MPUTest()<=0) Error(0);
      printf("\nþ The card available.");
    }
    else Error(0);
  }
  if (AllocXMS || XMSLoadFile[0] || XMSSaveFile[0]) GetOldXMS();

  memcpy(argums,argv[0],128);
  for (i=1;i<argc;i++) argum[i]=argums+(argv[i]-argv[0]);
  argumc=argc;

  do
  {
    for (i=1;i<argumc;i++)
    {
      if (argum[i][0]=='/' || argum[i][0]=='-')
      {
	switch (toupper(argum[i][1]))
	{
	  case '0':
	       printf("\nþ MIDI Emulation Mode = MPU-401");
	       s[0]=HCMDSETEMU;
	       s[1]=1;
	       Command(s,2,1);
	       break;
	  case '6':
	       printf("\nþ MIDI Emulation Mode = 6850");
	       s[0]=HCMDSETEMU;
	       s[1]=0;
	       Command(s,2,1);
	       break;
	  case 'S':
	       if (toupper(argum[i][2])=='C') chstat=1;
	       else status=1;
	       break;
	  case 'V':
	       sscanf(argum[i]+2,"%d",&w);
	       if (w>127) Error(3);
	       printf("\nþ Volume: %d",w);
	       s[0]=HCMDSETVOL;
	       s[1]=w;
	       Command(s,2,1);
	       break;
	  case 'N':
	       sscanf(argum[i]+2,"%d",&w);
	       if (w<16 || w>32) Error(4);
	       printf("\nþ Number of voices: %d",w);
	       s[0]=HCMDSETNOV;
	       s[1]=w;
	       Command(s,2,1);
	       break;
	  case 'T':
	       sscanf(argum[i]+2,"%i",&j);
	       if (j<-8192 || j>8191) Error(5);
	       printf("\nþ Synthesizer Tuning: %i",j);
	       s[0]=HCMDSETTUN;
	       if (j<0) j+=16384;
	       Long2MIDI(s+1,j,2);
	       Command(s,3,1);
	       break;
	  case 'C':
	       if (argum[i][2]==0) {f=1; l=16; }
	       else GetRange(argum[i]+3,1,16,&f,&l);
	       for (w=f;w<=l;w++)
	       {
		 s[0]=((toupper(argum[i][2])=='D')?HCMDDISCHN:HCMDENACHN);
		 s[1]=w-1;
		 printf("\nþ %s Synth Channel # %d",(s[0]==HCMDENACHN?Enabled:Disabled),w);
		 Command(s,2,1);
	       }
	       break;
	  case 'D':
	       if (argum[i][2]==0) {f=1; l=16; }
	       else GetRange(argum[i]+3,1,16,&f,&l);
	       for (w=f;w<=l;w++)
	       {
		 s[0]=((toupper(argum[i][2])=='D')?HCMDDISEDP:HCMDSETEDP);
		 s[1]=w-1;
		 printf("\nþ %s Enhanced Drum Program Channel # %d",(s[0]==HCMDSETEDP?Enabled:Disabled),w);
		 Command(s,2,1);
	       }
	       break;
	  case 'I':
	       s[0]=((toupper(argum[i][2])=='D')?HCMDDISMIS:HCMDENAMIS);
	       printf("\nþ %s MIDI-In to Synth",(s[0]==HCMDENAMIS?Enabled:Disabled));
	       Command(s,1,1);
	       break;
	  case 'M':
	       if (toupper(argum[i][2])!='P')
	       {
		 s[0]=((toupper(argum[i][2])=='D') ? HCMDDISVMM : HCMDENAVMM );
		 printf("\nþ %s Virtual MIDI Mode",(s[0]==HCMDENAVMM?Enabled:Disabled));
		 Command(s,1,1);
	       }
	       break;
	  case 'O':
	       printf("\nþ Report Peak Output Levels: (+/- Volume, C clear flag, else stop displaying!)");
	       n=0;
	       s[0]=HCMDGETVOL;
	       Command(s,1,1);
	       m=s[0];
	       l=0;
	       while (!n) {
		 s[0]=HCMDREPPOL;
		 Command(s,1,7);
		 strset(VolBar,' ');
		 if (MIDI2Long(s,3)>>9>0) strnset(VolBar,'þ',MIDI2Long(s,3)>>9);
		 l=(l+s[6])%1000000;
		 printf("\n%5ld %s %6d",MIDI2Long(s,3),VolBar,l);
		 strset(VolBar,' ');
		 if (MIDI2Long(s+3,3)>>9>0) strnset(VolBar,'þ',MIDI2Long(s+3,3)>>9);
		 printf("\n%5ld %s  (%3d)",MIDI2Long(s+3,3),VolBar,m);
		 delay(40);
		 gotoxy(1,wherey()-2);
		 if (kbhit())
		 {
		   switch (toupper(getch()))
		   {
		     case '+':
			  if (m<127) m++;
			  s[0]=HCMDSETVOL;
			  s[1]=m;
			  Command(s,2,1);
			  l=0;
			  break;
		     case '-':
			  if (m>0) m--;
			  s[0]=HCMDSETVOL;
			  s[1]=m;
			  Command(s,2,1);
			  l=0;
			  break;
		     case 'C':
			  l=0;
			  break;
		     default:
			  n=1;
			  break;
		   }
		 }
	       };
	       gotoxy(1,wherey()+2);
	       break;
	  case 'L':
	       FreeUp(NOPRG);
	       if (toupper(argum[i][2])=='U')
	       {
		 printf("\nþ %3d Used or Internal Sample(s) =",GetUsedSamples());
		 l=1;
		 for (m=0;m<512;m++)
		 {
		   if (Samples[m]>EMPTY)
		   {
		     printf("%4d:%-3d",m,Samples[m]&SMASK);
		     if ((l++%200)==0) Pause(PAUSEPAUSE);
		   }
		 }
		 printf("\nþ %3d Unused Sample(s)=",GetUnusedSamples());
		 for (m=0;m<512;m++)
		 {
		   if (Samples[m]==UNUSED)
		   {
		     printf("%4d:%-3i",m,Samples[m]);
		     if ((l++%200)==0) Pause(PAUSEPAUSE);
		   }
		 }
		 printf("\nþ %3d Empty Sample(s) =",GetFreeSamples());
		 for (m=0;m<512;m++)
		 {
		   if (Samples[m]==EMPTY)
		   {
		     printf("%4d:%-3d",m,Samples[m]&SMASK);
		     if ((l++%200)==0) Pause(PAUSEPAUSE);
		   }
		 }
		 printf("\nþ %3d Used Patch(es)  =",256-GetFreePatches());
		 for (m=0;m<256;m++)
		 {
		   if (Patches[m]!=EMPTY)
		   {
		     printf("%4d:%-3d",m,Patches[m]);
		     if ((l++%200)==0) Pause(PAUSEPAUSE);
		   }
		 }
		 printf("\nþ %3d Empty Patch(es) =",GetFreePatches());
		 for (m=0;m<256;m++)
		 {
		   if (Patches[m]==EMPTY)
		   {
		     printf("%4d:%-3d",m,Patches[m]);
		     if ((l++%200)==0) Pause(PAUSEPAUSE);
		   }
		 }
	       }
	       f=0;
	       l=0;
	       if (argum[i][2]==0 || (toupper(argum[i][2])=='H')) {
		 if (argum[i][2]==0) {f=0; l=255; }
		 else GetRange(argum[i]+3,0,255,&f,&l);
		 printf("\nþ WaveFront Program(s) structure: %d - %d",f,l);
		 nn=(l<128?l:127);
		 if (f<128) for (m=f;m<=nn;m++)
		 {
		   GetProgramName(m);
		   printf("\nPROGRAM # %3d = %s",m,sName);
		   GetProgram(m,s);
		   Prg=(struct PROGRAM*)&s;
		   for (j=0;j<4;j++)
		   {
		     if (Prg->layer[j].fUnmute)
		     {
		       GetPatchName(Prg->layer[j].byPatchNumber);
		       printf("\n  ÀÄPATCH # %5d = %15s",Prg->layer[j].byPatchNumber,sName);
		       GetPatch(Prg->layer[j].byPatchNumber, z);
		       Patch=(struct PATCH*)z;
		       n=Patch->bySampleNumber+256*Patch->fSampleMSB;
		       GetSampleType(n,z);
		       switch (z[0])
		       {
			 case 0: //Sample
			      GetSampleName(n);
			      printf("\n    ÀÄSAMPLE # %d = %s",n,sName);
			      break;
			 case 1: //Multisample
			      GetSampleName(n);
			      printf("\n    ÀÄMULTISAMPLE # %d = %s",n,sName);
			      GetMultiSample(n,z);
			      for (n=1;n<(1<<z[0]);n++) {
				for (w=n+1;w<(1<<z[0]) && MIDI2Long(z+(n*2-1),2)==MIDI2Long(z+(w*2-1),2);w++);
				GetSampleName(MIDI2Long(z+(n*2-1),2));
				{
				  GetSampleType(MIDI2Long(z+(n*2-1),2),y);
				  if (y[0]==0)
				  {
				    printf("\n       ÀÄSAMPLE # %ld = %s",MIDI2Long(z+(n*2-1),2),sName);
				  }
				  else
				  {
				    GetAliasSample(MIDI2Long(z+(n*2-1),2),y);
				    Alias=(struct ALIAS*)y;
				    printf("\n       ÀÄALIAS # %ld = %s",MIDI2Long(z+(n*2-1),2),sName);
				    memcpy(&yy,y,2);
				    GetSampleName(yy);
				    printf("\n          ÀÄSAMPLE # %d = %s",yy,sName);
				  }
				}
				n=w-1;
			      }
			      break;
			 case 2: //Alias
			      GetAliasSample(n,z);
			      Alias=(struct ALIAS*)z;
			      GetSampleName(n);
			      printf("\n    ÀÄALIAS # %d = %s",n,sName);
			      memcpy(&w,z,2);
			      GetSampleName(w);
			      printf("\n       ÀÄSAMPLE # %d = %s",w,sName);
			 default: break;
		       } //switch
		     } //if
		   } //for j
		   Pause(PAUSEPAUSE);
		   printf("\n");
		 } //for m
		 nn=(f>127?f:128);
		 if (l>127) for (m=nn-128;m<=l-128;m++)
		 {
		   GetDrumProgram(m,s);
		   Drum=(struct DRUM*)&s;
		   if (Drum->fUnmute)
		   {
		     printf("\nDRUM # %3d ",m);
		     GetPatchName(Drum->byPatchNumber);
		     printf("\n  ÀÄPATCH # %5d = %15s",Drum->byPatchNumber,sName);
		     GetPatch(Drum->byPatchNumber, z);
		     Patch=(struct PATCH*)z;
		     n=Patch->bySampleNumber+256*Patch->fSampleMSB;
		     GetSampleType(n,z);
		     switch (z[0])
		     {
		       case 0: //Sample
			    GetSampleName(n);
			    printf("\n    ÀÄSAMPLE # %d = %s",n,sName);
			    break;
		       case 1: //Multisample
			    GetSampleName(n);
			    printf("\n    ÀÄMULTISAMPLE # %d = %s",n,sName);
			    GetMultiSample(n,z);
			    for (n=1;n<(1<<z[0]);n++) {
			      for (w=n+1;w<(1<<z[0]) && MIDI2Long(z+(n*2-1),2)==MIDI2Long(z+(w*2-1),2);w++);
			      GetSampleName(MIDI2Long(z+(n*2-1),2));
			      {
				BYTE y[100];
				WORD yy;

				GetSampleType(MIDI2Long(z+(n*2-1),2),y);
				if (y[0]==0)
				{
				  printf("\n       ÀÄSAMPLE # %ld = %s",MIDI2Long(z+(n*2-1),2),sName);
				}
				else
				{
				  GetAliasSample(MIDI2Long(z+(n*2-1),2),y);
				  Alias=(struct ALIAS*)y;
				  printf("\n       ÀÄALIAS # %ld = %s",MIDI2Long(z+(n*2-1),2),sName);
				  memcpy(&yy,y,2);
				  GetSampleName(yy);
				  printf("\n          ÀÄSAMPLE # %d = %s",yy,sName);
				}
			      }
			      n=w-1;
			    }
			    break;
		       case 2: //Alias
			    GetAliasSample(n,z);
			    Alias=(struct ALIAS*)z;
			    GetSampleName(n);
			    printf("\n    ÀÄALIAS # %d = %s",n,sName);
			    memcpy(&w,z,2);
			    GetSampleName(w);
			    printf("\n       ÀÄSAMPLE # %d = %s",w,sName);
		       default: break;
		     } //switch
		     Pause(PAUSEPAUSE);
		     printf("\n");
		   } //if
		 } //for m
	       } //'H'
	       f=0;
	       l=0;
	       if (argum[i][2]==0 || (toupper(argum[i][2])=='R')) {
		 if (argum[i][2]==0) {f=0; l=127; }
		 else GetRange(argum[i]+3,0,127,&f,&l);
		 printf("\nþ WaveFront Program(s): %d - %d",f,l);
		 for (m=f;m<=l;m++) {
		   GetProgramName(m);
		   printf("\n-- PROGRAM # %3d = %s",m,sName);
		   GetProgram(m,s);
		   Prg=(struct PROGRAM*)&s;
		   for (j=0;j<4;j++) {
		     printf("\n-LAYER # %d",j+1);
		     GetPatchName(Prg->layer[j].byPatchNumber);
		     printf("\nPatch Number: %5d = %15s",Prg->layer[j].byPatchNumber,sName);
		     printf("\tMix Level:    %5d %s",Prg->layer[j].fMixLevel,(Prg->layer[j].fUnmute?"":"Mute"));
		     printf("\nSplit Point:    %2s%d %8s %4s",(Prg->layer[j].fSplitType?"":NoteName[(Prg->layer[j].fSplitPoint)%12]),
		       (Prg->layer[j].fSplitType?Prg->layer[j].fSplitPoint:((Prg->layer[j].fSplitPoint)/12-1)),
		       (Prg->layer[j].fSplitType?"Velocity":"Keyboard"),
		       (Prg->layer[j].fSplitDir?"Down":"Up"));
		     printf("\tPan:  ");
		     if (Prg->layer[j].fPanModulated) printf("Moulated %s x %i",PanSource[Prg->layer[j].fPanModSource],(Prg->layer[j].fPan-7));
		     else printf("Fixed setting: %d",Prg->layer[j].fPan);
		   }
		   Pause(PAUSEPAUSE);
		   printf("\n");
		 }
	       }
	       if ((argum[i][2]==0) || (toupper(argum[i][2])=='P')) {
		 if (argum[i][2]==0) {f=0; l=255; }
		 else GetRange(argum[i]+3,0,255,&f,&l);
		 printf("\nþ WaveFront Patch(es): %d - %d",f,l);
		 for (m=f;m<=l;m++) {
		   GetPatchName(m);
		   printf("\n-- PATCH # %3d = %s",m,sName);
		   if (GetPatch(m,s))
		   {
		     Patch=(struct PATCH*)&s;
		     printf("\nFrequency Bias: %6i",Patch->nFreqBias);
		     printf("\t\tAmplitude Bias: %d",Patch->fAmpBias);
		     printf("\nPortamento: %3d",Patch->fPortamento);
		     GetSampleName(Patch->bySampleNumber+256*Patch->fSampleMSB);
		     printf("\t\tSample number: %d = %s",Patch->bySampleNumber+256*Patch->fSampleMSB,sName);
		     printf("\nPitch Bend: %d",Patch->fPitchBend);
		     printf("\nFlags: %s%s%s%s%s%s",(Patch->fMono?"Mono ":""),
		       (Patch->fRetrigger?"Retrigger ":""),
		       (Patch->fNoHold?"No_Hold ":""),
		       (Patch->fRestart?"Restart_Envelope ":""),
		       (Patch->fReuse?"Reuse ":""),
		       (Patch->fResetLfo?"Reset_LFO ":""));
		     printf("\nFM Source 1: %-17s",ModSource[Patch->fFMSource1]);
		     printf("\tFM Multiplier 1: %3d",Patch->cFMAmount1);
		     printf("\nFM Source 2: %-17s",ModSource[Patch->fFMSource2]);
		     printf("\tFM Multiplier 2: %3d",Patch->cFMAmount2);
		     printf("\nAM Source:   %-17s",ModSource[Patch->fAMSource]);
		     printf("\tAM Multiplier: %3d",Patch->cAMAmount);
		     printf("\nFC 1 Mod Source: %-17s",ModSource[Patch->fFC1MSource]);
		     printf("\tFC 1 Mod Multiplier: %3d",Patch->cFC1MAmount);
		     printf("\nFC 1 Mod Scaling: %3d",Patch->cFC1KeyScale);
		     printf("\t\t\tFC 1 Freq Bias: %3d",Patch->cFC1FreqBias);
		     printf("\nFC 2 Mod Source: %-17s",ModSource[Patch->fFC2MSource]);
		     printf("\tFC 2 Mod Multiplier: %3d",Patch->cFC2MAmount);
		     printf("\nFC 2 Mod Scaling: %3d",Patch->cFC2KeyScale);
		     printf("\t\t\tFC 2 Freq Bias: %3d",Patch->cFC2FreqBias);
		     printf("\nRandomizer: %3d",Patch->fRandomizerRate);
		     printf("\n- ENVELOPE 1:");
		     printf("\nAttack Time : %5.3fs, Level: %3d",TimeTable[Patch->envelope1.fAttackTime],Patch->envelope1.cAttackLevel);
		     printf("\tDecay 1 Time: %5.3fs, Level: %3d",TimeTable[Patch->envelope1.fDecay1Time],Patch->envelope1.cDecay1Level);
		     printf("\nDecay 2 Time: %5.3fs, Level: %3d",TimeTable[Patch->envelope1.fDecay2Time],Patch->envelope1.cDecay2Level);
		     printf("\tSustain Time: %5.3fs, Level: %3d",TimeTable[Patch->envelope1.fSustainTime],Patch->envelope1.cSustainLevel);
		     printf("\nRelease Time: %5.3fs, Level: %3d",TimeTable[Patch->envelope1.fReleaseTime],Patch->envelope1.cReleaseLevel);
		     printf("\tRelease 2 Time: %5.3fs",TimeTable[Patch->envelope1.fRelease2Time]);
		     printf("\nAttack Velocity: %d, \tVolume Velocity: %d, \tKeyboard Scaling: %d",Patch->envelope1.fAttackVelocity,Patch->envelope1.fVolumeVelocity,Patch->envelope1.fKeyScale);
		     printf("\n- ENVELOPE 2:");
		     printf("\nAttack Time : %5.3fs, Level: %3d",TimeTable[Patch->envelope2.fAttackTime],Patch->envelope2.cAttackLevel);
		     printf("\tDecay 1 Time: %5.3fs, Level: %3d",TimeTable[Patch->envelope2.fDecay1Time],Patch->envelope2.cDecay1Level);
		     printf("\nDecay 2 Time: %5.3fs, Level: %3d",TimeTable[Patch->envelope2.fDecay2Time],Patch->envelope2.cDecay2Level);
		     printf("\tSustain Time: %5.3fs, Level: %3d",TimeTable[Patch->envelope2.fSustainTime],Patch->envelope2.cSustainLevel);
		     printf("\nRelease Time: %5.3fs, Level: %3d",TimeTable[Patch->envelope2.fReleaseTime],Patch->envelope2.cReleaseLevel);
		     printf("\tRelease 2 Time: %5.3fs",TimeTable[Patch->envelope2.fRelease2Time]);
		     printf("\nAttack Velocity: %3d, \tVolume Velocity: %d, \tKeyboard Scaling: %d",Patch->envelope2.fAttackVelocity,Patch->envelope2.fVolumeVelocity,Patch->envelope2.fKeyScale);
		     Pause(PAUSEPAUSE);
		     printf("\n- LFO 1:");
		     printf("\nSample Type: %-18s",LFOWave[Patch->lfo1.bySampleNumber]);
		     printf("\t\tFrequency: %3d",Patch->lfo1.fFrequency);
		     printf("\nAM Source: %-17s",ModSource[Patch->lfo1.fAMSource]);
		     printf("\tAM Multiplier: %3d",Patch->lfo1.cAMAmount);
		     printf("\nFM Source: %-17s",ModSource[Patch->lfo1.fFMSource]);
		     printf("\tFM Multiplier: %3d",Patch->lfo1.cFMAmount);
		     printf("\nStart Level: %3d\t End Level %3d",Patch->lfo1.cStartLevel,Patch->lfo1.cEndLevel);
		     printf("\nRamp Delay: %5.3fs",TimeTable[Patch->lfo1.fDelayTime]);
		     printf("\tRamp Time: %5.3fs",TimeTable[Patch->lfo1.fRampTime]);
		     printf("\n- LFO 2:");
		     printf("\nSample Type: %-18s",LFOWave[Patch->lfo2.bySampleNumber]);
		     printf("\t\tFrequency: %3d",Patch->lfo2.fFrequency);
		     printf("\nAM Source: %-17s",ModSource[Patch->lfo2.fAMSource]);
		     printf("\tAM Multiplier: %3d",Patch->lfo2.cAMAmount);
		     printf("\nFM Source: %-17s",ModSource[Patch->lfo2.fFMSource]);
		     printf("\tFM Multiplier: %3d",Patch->lfo2.cFMAmount);
		     printf("\nStart Level: %3d\t End Level %3d",Patch->lfo2.cStartLevel,Patch->lfo2.cEndLevel);
		     printf("\nRamp Delay: %5.3fs",TimeTable[Patch->lfo2.fDelayTime]);
		     printf("\tRamp Time: %5.3fs",TimeTable[Patch->lfo2.fRampTime]);
		     if (Patch->lfo2.fWaveRestart) printf("\tWave Restart");
		   }
		   else printf(" Unused Sample");
		   Pause(PAUSEPAUSE);
		   printf("\n");
		 }
	       }
	       if ((argum[i][2]==0) || (toupper(argum[i][2])=='S')) {
		 if (argum[i][2]==0) {f=0; l=511; }
		 else GetRange(argum[i]+3,0,511,&f,&l);
		 printf("\nþ WaveFront Sample(s): %d - %d",f,l);
		 for (m=f;m<=l;m++) {
		   if ((Samples[m]&SMASK)==EMPTY)
		   {
		     printf("\n-- Sample # %3d =  (UNUSED",m);
		     if (Samples[m]==INTERNAL) printf(" INTERNAL");
		     printf(" SAMPLE: %s) \tMemory: %ld",SampleName[m],GetOccMem(m));
		   }
		   else
		   {
		     GetSampleName(m);
		     //UseXMS=0;
		     printf("\n-- Sample # %3d = %s \tMemory: %ld",m,sName,GetOccMem(m));
		     //UseXMS=1;
		     GetSampleType(m,s);
		     //printf(" \t\tMemory: %ld",MIDI2Long(s+1,4));
		     switch (s[0]) {
		       case 0: //Sample
			    GetSampleHeader(m,s);
			    SampleH=(struct SAMPLE*)&s;
			    printf("\nSAMPLE Offset:  %ld - %ld, \tLoop Offset:  %ld - %ld",MIDI2Long(s,4)/16,MIDI2Long(s+12,4)/16,MIDI2Long(s+4,4)/16,MIDI2Long(s+8,4)/16);
			    w=MIDI2Long(s+16,3);
			    printf("\nFrequency Bias: %d = %5.0fHz",w,(44100.0/exp(log(2.0)*((w+0.0)/2048.0-4.0))));
			    printf("\tSample: %s %s%s%s",SampleType[s[19]&3],
				   (s[19]&8?"Loop ":""),
				   (s[19]&16?"Bi-directional Loop ":""),
				   (s[19]&64?"Reverse ":""));
			    break;
		       case 1: //Multisample
			    printf("\nMULTISAMPLE:");
			    GetMultiSample(m,s);
			    for (j=1;j<(1<<s[0]);j++) {
			      for (w=j+1;w<(1<<s[0]) && MIDI2Long(s+(j*2-1),2)==MIDI2Long(s+(w*2-1),2);w++);
			      GetSampleName(MIDI2Long(s+(j*2-1),2));
			      printf("\n%2s%-2d - %2s%-2d : %ld = %s",NoteName[(j-1)%12],(j-1)/12-1,NoteName[(w-2)%12],(w-2)/12-1,MIDI2Long(s+(j*2-1),2),sName);
			      {
				GetSampleType(MIDI2Long(s+(j*2-1),2),y);
				if (y[0]==2)
				{
				  GetAliasSample(MIDI2Long(s+(j*2-1),2),y);
				  memcpy(&yy,y,2);
				  GetSampleName(yy);
				  printf(" ALIASED SAMPLE # %d = %s",yy,sName);
				}
			      }
			      j=w-1;
			    }
			    break;
		       case 2: //Alias
			    GetAliasSample(m,s);
			    Alias=(struct ALIAS*)&s;
			    memcpy(&w,s,2);
			    GetSampleName(w);
			    printf("\nALIASED SAMPLE: %d = %s",w,sName);
			    printf("\nSample Offset:  %ld - %ld, \tLoop Offset:  %ld - %ld",MIDI2Long(s+2,4)/16,MIDI2Long(s+14,4)/16,MIDI2Long(s+6,4)/16,MIDI2Long(s+10,4)/16);
			    w=MIDI2Long(s+18,3);
			    printf("\nFrequency Bias: %d = %5.0fHz",w,(44100.0/exp(log(2.0)*((w+0.0)/2048.0-4.0))));
			    printf("\tSample: %s%s%s",
				   (s[21]&8?"Loop ":""),
				   (s[21]&16?"Bi-directional Loop ":""),
				   (s[21]&64?"Reverse ":""));

			    break;
		       default: break;
		     } //switch
		   }
		   Pause(PAUSEPAUSE);
		 } //for m
	       } //sample

	       if ((argum[i][2]==0) || (toupper(argum[i][2])=='D')) {
		 if (argum[i][2]==0) {f=0; l=127; }
		 else GetRange(argum[i]+3,0,127,&f,&l);
		 printf("\nþ WaveFront Enhanced Drum Program(s): %d - %d",f,l);
		 for (m=f;m<=l;m++) {
		   printf("\n-- Drum # %3d",m);
		   GetDrumProgram(m,s);
		   Drum=(struct DRUM*)&s;
		   GetPatchName(Drum->byPatchNumber);
		   printf("\nPatch Number: %5d = %15s",Drum->byPatchNumber,sName);
		   printf("\tMix Level:    %5d %s",Drum->fMixLevel,(Drum->fUnmute?"":"Mute"));
		   printf("\nGroup:    %d",Drum->fGroup);
		   printf("\tPan:  ");
		   if (Drum->fPanModulated) printf("Moulated %s x %i",PanSource[Drum->fPanModSource],(Drum->fPanAmount-7));
		   else printf("Fixed setting: %d",Drum->fPanAmount);
		   Pause(PAUSEPAUSE);
		   printf("\n");
		 }
	       }
	       break;
	  case 'E':
	       FreeUp(NOPRG);
	       j=0;
	       if (toupper(argum[i][2])=='E')
	       {
		 j=1;
		 memmove(argum[i]+2,argum[i]+3,strlen(argum[i]+3)+1);
	       }
	       if (argum[i][2]==0) {f=0; l=511; }
	       else GetRange(argum[i]+2,0,511,&f,&l);
	       printf("\nþ Deleteing %sSample(s): %d - %d",j==0?"non INTERNAL ":"",f,l);
	       for (m=f;m<=l;m++) {
		 if (!strcmp(SampleName[m],"<Empty>")) // Non-Internal
		 {
		   CleanUpSample(m);
		 }
		 else if (j==1) // Internal!
		 {
		   if (GetOccMem(m)>0)
		   {
		     CleanUpSample(m);
		   }
		 }
	       } //for m
	       break;
	  case 'U':
	       FreeUp(NOPRG);
	       printf("\nþ Deleteing all Unused Samples ...");
	       for (m=0;m<512;m++)
	       {
		 if (Samples[m]==UNUSED)
		 {
		   CleanUpSample(m);
		 }
	       }
	       break;
	  case 'W':
	       FreeUp(NOPRG);
	       ProcessConfig(argv,pmChange);
	       if (argum[i][2]!=0) WaveParams(argum[i]+2);
	       else Error(7);
	       if (WavPrg==UNDEFINED && FirstSample==UNDEFINED) Error(7);

	       if (WavPrg!=UNDEFINED)
	       {
		 if (!WavDrum)
		 {
		   printf("\nþ Modify Program Parameters (%d)",WavPrg);
		   GetProgram(WavPrg,s);
		   Prg=(struct PROGRAM*)&s;

		   for (j=0;j<4;j++)
		   {
		     if (Prg->layer[j].fUnmute)
		     {
		       if (WavVol!=UNDEFINED) Prg->layer[j].fMixLevel=WavVol;
		       GetPatch(Prg->layer[j].byPatchNumber,z);
		       Patch=(struct PATCH*)z;
		       ModifySamHead(Patch->bySampleNumber+256*Patch->fSampleMSB);
		     }
		   }
		   if (WavVol!=UNDEFINED)
		   {
		     GetProgramName(WavPrg);
		     DownProgram(WavPrg, s, z);
		   }
		 }
		 else // DRUM
		 {
		   printf("\nþ Modify Drum Parameters (%d)",WavPrg);
		   GetDrumProgram(WavPrg,s);
		   Drum=(struct DRUM*)&s;
		   if (Drum->fUnmute)
		   {
		     if (WavVol!=UNDEFINED) Drum->fMixLevel=WavVol;
		     GetPatch(Drum->byPatchNumber, z);
		     Patch=(struct PATCH*)z;
		     ModifySamHead(Patch->bySampleNumber+256*Patch->fSampleMSB);
		   }

		   if (WavVol!=UNDEFINED)
		   {
		     DownDrumProgram(WavPrg, s, z);
		   }
		 }
	       }
	       else if (FirstSample!=UNDEFINED) ModifySamHead(FirstSample);
	       break;
	  default: break;
	} //switch
      } //if
/*  } //for
    for (i=1;i<argumc;i++)
*/    else
      {
	// MIDI file
	if (toupper(argum[i][0])=='M' && toupper(argum[i][1])=='I' &&
	    toupper(argum[i][2])=='D' && toupper(argum[i][3])=='I')
	{
	  if (midiplayer[0])
	  {
	    printf("\nþ Playing MIDI file ...");
	    strcpy(y,strchr(argum[i],'=')+1);
	    while (y[0]==' ') strcpy(y,y+1);
	    strrchr(y,10)[0]=0;
	    strrchr(y,13)[0]=0;
	    strcpy(s,"/c ");
	    strcat(s,midiplayer);
	    strcat(s," ");
	    strcat(s,y);
	    spawnl(P_WAIT,getenv("COMSPEC"),getenv("COMSPEC"),s,NULL);
	    if (!MPUTest()) Error(0);
	    printf("\nþ Back from the MIDI player to SETWF.");
	  }
	}
	else if (argum[i][0]!='/' && argum[i][0]!='-')
	{
	  long percent=0,percentm;

	  ProcessConfig(argv,pmLoad);

	  FileType=0;

	  sz=strrchr(argum[i],'.');
	  p=sz+1;

	  if (sz==NULL) Error(11);

	  for (j=1;j<=4 && sz[0]!=0;j++) sz++;

	  if (j!=5) Error(11);

	  if (!strncmpi(p,"WAV",3))
	  {
	    FileType=WAVEFILE;
	  }
	  else if (!strncmpi(p,"WFS",3))
	  {
	    if (WFSfile[0]) Error(23);
	    strcpy(WFSfile,argv[i]);
	    printf("\nþ Processing WaveFront Set file: %s",WFSfile);
	    if (WFSfile[0])
	    {
	      s[0]=0;  //Clear the string
	      if (strrchr(WFSfile,'\\'))
	      {
		chdir(strncat(s,WFSfile,strrchr(WFSfile,'\\')-WFSfile));
	      }
	      else if (strrchr(WFSfile,':'))
	      {
		chdir(strncat(s,WFSfile,strrchr(WFSfile,':')-WFSfile));
	      }
	      stream=fopen(WFSfile, "r");
	      if (stream==0) Error(22);
	    }
	    FileType=WFSFILE;
	  }
	  else if (!strncmpi(p,"WSS",3))
	  {
	    strcpy(WSSFileName,argv[i]);
	    printf("\nþ Processing WaveFront Sample Set: %s",WSSFileName);
	    if (_dos_open(WSSFileName, O_RDONLY, &WSSfh)) Error(22); // WSS Open Error
	    if (_dos_read(WSSfh, y, 19, &ReadBytes) || memcmp(WSSsignature,y+2,17))
	       Error(22);  // Nem WSS file

	    save_argumc=argumc;
	    memcpy(save_argums,argums,128);
            memcpy(save_argum,argum,sizeof(argum));
	    save_i=i;

	    //WSSpos=19; //start position in the file

	    FileType=WSSFILE;
	    memcpy(&isWSS,y,2);
	    if (isWSS<1 || isWSS>512) Error(22); // Error in Sample Number!
	  }
	  else if ((!strncmpi(sz,"WFB",3) && !strncmpi(sz,"WFP",3) &&
		    !strncmpi(sz,"WFD",3))) Error(11);

	  FileName[0]=0;
	  strncat(FileName,argum[i],sz-argum[i]);

	  if (','==sz[0]) sz++;
	  if (sz[0]!=0) WaveParams(sz);

	  if (isWSS)
	  {
	    WavPrg=WSS.bProgramNumber;
	    WavDrum=WSS.fIsDrum;
	    //FirstPatch=WSS.bPatchNumber1;
	    //FirstSample=(WORD)WSS.patch1.bySampleNumber+(WORD)256*(WORD)WSS.patch1.fSampleMSB;
	    Loop=WSS.fIsLooped;
	    MonoSample=WSS.fIsMono;

	  }

	  if (FirstPrg>127 || FirstPatch>255 || FirstSample>511) Error(7);
	  // WAVE file: FirstPrg=Program #, FirstPatch=Volume, FirstSample=Root key

	  if (FileType<WFSFILE) printf("\nþ File Name: %s",FileName);

	  if ((aBuf = (BYTE *) malloc(16384)) == NULL) Error(10);

	  if (FileType==WAVEFILE)
	  {
	    if (_dos_open(FileName, O_RDONLY, &fh) != 0) Error(8);
	    if (_dos_read(fh, &ChunkH, sizeof(struct ChunkHeader), &ReadBytes) != 0) Error(9);
	    if (ReadBytes<sizeof(struct ChunkHeader)) Error(9);
	    if (strncmpi(ChunkH.szName,"RIFF",4)) Error(18);

	    if (_dos_read(fh, &ChunkH.szName, sizeof(ChunkH.szName), &ReadBytes) != 0) Error(9);
	    if (ReadBytes<sizeof(ChunkH.szName)) Error(9);
	    if (strncmpi(ChunkH.szName,"WAVE",4)) Error(18);

	    ChunkH.dwSize=0;
	    do
	    {
	      lseek(fw,ChunkH.dwSize,SEEK_CUR);
	      if (_dos_read(fh, &ChunkH, sizeof(struct ChunkHeader), &ReadBytes) != 0) Error(9);
	      if (ReadBytes<sizeof(struct ChunkHeader)) Error(9);
	    } while (strncmpi(ChunkH.szName,"fmt ",4));

	    if (_dos_read(fh, &ChunkFmt, ChunkH.dwSize, &ReadBytes) != 0) Error(9);
	    if (ReadBytes<ChunkH.dwSize) Error(9);

	    if (ChunkFmt.wFormatTag!=1) Error(19);
	    printf("\nWAVE file: PCM ");
	    printf("%s ",(ChunkFmt.wChannels==2?"STEREO":"MONO"));
	    printf("%ld Hz ",ChunkFmt.dwSamplesPerSec);
	    printf("%d bits  ",ChunkFmt.wResolution);

	    //LOOP & ROOT KEY
	    l=tell(fh);
	    ChunkH.dwSize=0;
	    do
	    {
	      lseek(fh,ChunkH.dwSize,SEEK_CUR);
	      if (tell(fh)>=filelength(fh)) break;
	      if (_dos_read(fh, &ChunkH, sizeof(struct ChunkHeader), &ReadBytes) != 0) Error(9);
	      if (ReadBytes<sizeof(struct ChunkHeader)) Error(9);
	    } while (strncmpi(ChunkH.szName,"smpl",4));

	    if (!strncmpi(ChunkH.szName,"smpl",4))
	    {
	      if (_dos_read(fh, &ChunkSmpl, ChunkH.dwSize, &ReadBytes) != 0) Error(9);
	      if (ReadBytes<ChunkH.dwSize) Error(9);
	      if (ChunkH.dwSize>=44)
	      {
		if (FromFile==YES)
		{
		  WavLoopStart=ChunkSmpl.dwLoopStart;
		  WavLoopEnd=ChunkSmpl.dwLoopEnd;
		  Loop=YES;
		}
	      }
	      if (ChunkH.dwSize>=12 && WavKey==UNDEFINED)
	      {
		WavKey=ChunkSmpl.dwRootKey;
	      }
	    }
	    lseek(fh,l,SEEK_SET);

	    //DATA Chunk!!!
	    ChunkH.dwSize=0;
	    do
	    {
	      lseek(fh,ChunkH.dwSize,SEEK_CUR);
	      if (_dos_read(fh, &ChunkH, sizeof(struct ChunkHeader), &ReadBytes) != 0) Error(9);
	      if (ReadBytes<sizeof(struct ChunkHeader)) Error(9);
	    } while (strncmpi(ChunkH.szName,"DATA",4));

	    printf("%ld bytes",ChunkH.dwSize);
	    percentm=ChunkH.dwSize;
	  }
	  else if (FileType<WAVEFILE) //Not a WAVE file = WFP, WFD, WFP
	  {
	    if (_dos_open(FileName, O_RDONLY, &fh) != 0) Error(8);
	    if (_dos_read(fh, &WFHead, sizeof(struct WaveFrontFileHeader), &ReadBytes) != 0) Error(9);
	    if (ReadBytes<sizeof(struct WaveFrontFileHeader)) Error(9);

	    if (!strcmpi(WFHead.szFileType,szBankFile)) FileType=BANKFILE;
	    else if (!strcmpi(WFHead.szFileType,szDrumkitFile)) FileType=DRUMKITFILE;
	    else if (!strcmpi(WFHead.szFileType,szProgramFile)) FileType=PROGRAMFILE;
	    else Error(14);

	    printf("\n%s ",WFHead.szSynthName);
	    printf("%s ",WFHead.szFileType);
	    printf("%4.2f ",(WFHead.wVersion+0.0)/100);
	    printf("(%d Prgs/%d DrumKits/%d Patches/%d Samples) ",WFHead.wProgramCount,WFHead.wDrumkitCount,WFHead.wPatchCount,WFHead.wSampleCount);
	    printf("%ld byte(s)",WFHead.dwMemoryRequired);
	    if (WFHead.szComment[0]) printf("\nComment: \t%s",WFHead.szComment);

	    NonMaui=(strcmpi("Maui",WFHead.szSynthName) && strcmpi("Tropez",WFHead.szSynthName)
		     && strcmpi("Daytona",WFHead.szSynthName));    //Error(11);

	    if (WFHead.wVersion<120) Error(12);
	  }

	  // Searching used & unused rooms
	  if (FileType==PROGRAMFILE)
	  {
	    lseek(fh,WFHead.dwProgramOffset,SEEK_SET);
	    if (_dos_read(fh, &WFP, sizeof(struct WaveFrontProgram), &ReadBytes) != 0) Error(9);
	    if (ReadBytes<sizeof(struct WaveFrontProgram)) Error(9);
	    GetProgramName(WFP.nNumber);
	    if (nNoLoad>0) printf("\nOriginal program location: %d = %s",WFP.nNumber,sName);
	    if (FirstPrg>=0) WFP.nNumber=FirstPrg;
	    FreeUp(WFP.nNumber);
	    s[0]=HCMDREPFRE;
	    Command(s,1,4);
	    if (nNoLoad==0 && MIDI2Long(s,4)<WFHead.dwMemoryRequired)
	    {
	      printf("\nWaveFront Free: %ld byte(s)",MIDI2Long(s,4));
	      Error(13);
	    }

	  }
	  else if (FileType==DRUMKITFILE)
	  {
	    FreeUp(DRUMKIT);
	    s[0]=HCMDREPFRE;
	    Command(s,1,4);
	    printf("\nWaveFront Free: %ld byte(s)",MIDI2Long(s,4));
	    if (nNoLoad==0 && MIDI2Long(s,4)<WFHead.dwMemoryRequired) Error(13);
	  }
	  else if (FileType==WAVEFILE)
	  {
	    if (WavPrg==UNDEFINED) WavPrg=DefWavPrg;
	    if (WavVol==UNDEFINED) WavVol=127;
	    if (WavKey==UNDEFINED) WavKey=60;
	    //printf("\nMIDI Root Key: \t%d (%s%d)",WavKey,NoteName[WavKey%12],(int)(WavKey/12-1));
	    FreeUp(WavPrg+(WavDrum?128:0));
	    s[0]=HCMDREPFRE;
	    Command(s,1,4);
	    if (nNoLoad==0 && MIDI2Long(s,4)<ChunkH.dwSize)
	    {
	      printf("\nWaveFront Free: %ld byte(s)",MIDI2Long(s,4));
	      Error(13);
	    }
	  };

	  if (FileType<WFSFILE)
	  {
	    if (GetFreeSamples()<WFHead.wSampleCount) Error(16);
	    if (GetFreePatches()<WFHead.wPatchCount) Error(17);
	  }

	  /*Relocation strategy:
	   BANKFILE - never - but NON-MAUI files - no load some INTERNAL SAMPLE
	   PROGRAMFILE,DRUMFILE,WAVE:
	      if original pleace is EMPTY - none
	      if original pleace is INTERNAL &&
		 SampleName[]=NewSampleName &&
		 (
		 ( nSampleType == ALIAS || MULTISAMPLE ) ||
		 ( nSampleType == SAMPLE && INTERNAL SAMPLE)
		 ) - none
	      else - relocate
	  */

	  if (FileType<WFSFILE)
	  {
	    for (j=0;j<512;j++) SampleNew[j]=UNDEFINED; //Set to None
	  }
	  if (FileType==BANKFILE)
	  {
	    if (XMShandle==0)
	    {
	      if (!isTHSXMS())
	      {
		printf("\nþ Allocating Extended Memory.");
		if (!allocTHSXMS()) Error(1);
	      }
	    }
	    // Search unused sample
	    for (m=0;m<512;m++)
	    {
	      if (!strcmp(SampleName[m],"<Empty>"))
		 CleanUpSample(m);
	      else
		 Samples[m]=INTERNAL;
	    }
	    // UNUSED PATCHES
	    for (m=0;m<256;m++) Patches[m]=EMPTY;
	  }
	  else if (FileType<WFSFILE) //BANK, DRUM, PRG, WAVE
	  {
	    //printf("\núúú Calculating relocation(s)... ");

	    if (FileType<WAVEFILE)
	    {
	      l=WFHead.dwSampleOffset;
	      for (j=1;j<=WFHead.wSampleCount;j++) {
		//putch(Work[nWork++&3]); putch(8);
		n=1; //relocate!!

		lseek(fh,WFHead.dwSampleOffset,SEEK_SET);
		if (_dos_read(fh, &WFESI, sizeof(struct WaveFrontExtendedSampleInfo), &ReadBytes) != 0) Error(9);
		if (ReadBytes<sizeof(struct WaveFrontExtendedSampleInfo)) Error(9);

		w=WFESI.nNumber;
		if (FirstSample>=0) w=FirstSample++;

		GetSampleName(w);
		if (Samples[w]==EMPTY)
		{
		  n=0; //Reloc off
		}
		else if (Samples[w]>=INTERNAL && !strcmp(sName,WFESI.szName))
		{
		  if (WFESI.nSampleType!=ST_SAMPLE)
		  {
		    n=0; //Reloc off
		  }
		  else //ST_SAMPLE
		  {
		    if (_dos_read(fh, &WFSAM, sizeof(struct SAMPLE)+1, &ReadBytes) != 0) Error(9);
		    if (ReadBytes<sizeof(struct SAMPLE)+1) Error(9);

		    if (_dos_read(fh, &WaveFile, 260, &ReadBytes) != 0) Error(9);
		    if (ReadBytes<260) Error(9);

		    if (!strcmpi("INTERNAL",WaveFile))
		    {
		      n=0; //Reloc off
		    }
		  }
		}
		if (n)
		{
		  w=SignFreeSample();
		  if (w==512) Error(16);
		}
		SampleNew[WFESI.nNumber]=w;
		WFHead.dwSampleOffset+=WFESI.dwSize;
	      } //for
	      WFHead.dwSampleOffset=l;
	    }
	    else //WAVE FILE
	    {
	      if (FirstSample==UNDEFINED) FirstSample=DefFirstSample;

	      for (j=0;j<ChunkFmt.wChannels;j++)
	      {
		if (!isWSS) w=j+FirstSample;
		else
		{
		  if (j==0) w=(WORD)WSS.patch1.bySampleNumber+(WORD)256*(WORD)WSS.patch1.fSampleMSB;
		  else w=(WORD)WSS.patch2.bySampleNumber+(WORD)256*(WORD)WSS.patch2.fSampleMSB;
		}
		if (Samples[w]!=EMPTY)
		{
		  w=SignFreeSample();
		  if (w==512) Error(16);
		}
		SampleNew[j]=w;
	      }
	    }
	  }

	  if (FileType<WFSFILE)
	  {
	    printf("\nSample(s) loading:  ");
	  }
	  if (FileType<WAVEFILE)
	  {
	    for (j=1;j<=WFHead.wSampleCount;j++) {
	      lseek(fh,WFHead.dwSampleOffset,SEEK_SET);
	      if (_dos_read(fh, &WFESI, sizeof(struct WaveFrontExtendedSampleInfo), &ReadBytes) != 0) Error(9);
	      if (ReadBytes<sizeof(struct WaveFrontExtendedSampleInfo)) Error(9);
	      w=WFESI.nNumber; //Save to Internal
	      if (FileType!=BANKFILE)
	      {
		if (SampleNew[WFESI.nNumber]!=UNDEFINED) WFESI.nNumber=SampleNew[WFESI.nNumber];
	      }
	      if (nNoLoad>0) printf("\n"); else gotoxy(20,wherey());
	      printf("%3d (%3d%1s) = %-32s ",j,WFESI.nNumber,(w==WFESI.nNumber?"":"R"),WFESI.szName);
	      switch (WFESI.nSampleType) {
		case ST_SAMPLE:
		     printf("SAMPLE"); //%ld, %ld, %ld, %d",WFESI.dwSampleRate,WFESI.dwSizeInBytes,WFESI.dwSizeInSamples,WFESI.nChannel);
		     if (_dos_read(fh, &WFSAM, sizeof(struct SAMPLE)+1, &ReadBytes) != 0) Error(9);
		     if (ReadBytes<sizeof(struct SAMPLE)+1) Error(9);

		     if (_dos_read(fh, &WaveFile, 260, &ReadBytes) != 0) Error(9);
		     if (ReadBytes<260) Error(9);

		     if (nNoLoad>0) printf(" %s,%sloop",SampleType[WFSAM.fSampleResolution],WFSAM.fLoop?"":"No ");
		     if (!strcmpi("INTERNAL",WaveFile))
		     {
		       if (FileType!=BANKFILE) GetSampleName(w); else strcpy(sName,SampleName[w]);
		       if (FileType!=BANKFILE && strcmp(WFESI.szName,sName)) //Internal WFP like an ALIAS
		       {
			 Long2MIDI(s+5,WFSAM.sampleStartOffset,4);
			 Long2MIDI(s+9,WFSAM.loopStartOffset,4);
			 Long2MIDI(s+13,WFSAM.loopEndOffset,4);
			 Long2MIDI(s+17,WFSAM.sampleEndOffset,4);
			 Long2MIDI(s+21,WFSAM.nFrequencyBias,3);
			 s[24]=8*WFSAM.fLoop+16*WFSAM.fBidirectional+64*WFSAM.fReverse;
			 s[25]=0;
			 strcpy(sName,WFESI.szName);
			 DownAliasSample(WFESI.nNumber,w,s);
		       }
		       else
		       {
			 printf(" INT.");
			 if (SampleNew[w]!=UNDEFINED)
			 {
			   SampleNew[w]=w;
			   Samples[WFESI.nNumber]&=(SIGNED-1);
			 }
			 if (NonMaui)
			 {
			   UseXMS=0;
			   GetSampleHeader(w,s);
			   UseXMS=1;
			   if ((s[19]&3)!=WFSAM.fSampleResolution ||
			      (WFSAM.loopEndOffset>MIDI2Long(s+12,4)/16) ||
			      (WFSAM.sampleEndOffset>MIDI2Long(s+12,4)/16))
			      w=1024;
			 }
			 if (w!=1024)
			 {
			   //w=WFESI.nNumber
			   Long2MIDI(s+3,WFSAM.sampleStartOffset,4);
			   Long2MIDI(s+7,WFSAM.loopStartOffset,4);
			   Long2MIDI(s+11,WFSAM.loopEndOffset,4);
			   Long2MIDI(s+15,WFSAM.sampleEndOffset,4);
			   Long2MIDI(s+19,WFSAM.nFrequencyBias,3);
			   s[22]=WFSAM.fSampleResolution+8*WFSAM.fLoop+16*WFSAM.fBidirectional+64*WFSAM.fReverse;
			   s[23]=0;
			   //sName already got.
			   DownSampleHeader(w,s);
  //			 Samples[w]+=USED;
			 }
		       }
		     }
		     else
		     {
		       Long2MIDI(s+3,WFESI.dwSizeInSamples,4);
		       Long2MIDI(s+7,WFSAM.sampleStartOffset,4);
		       Long2MIDI(s+11,WFSAM.loopStartOffset,4);
		       Long2MIDI(s+15,WFSAM.loopEndOffset,4);
		       Long2MIDI(s+19,WFSAM.sampleEndOffset,4);
		       Long2MIDI(s+23,WFSAM.nFrequencyBias,3);
		       s[26]=WFSAM.fSampleResolution+8*WFSAM.fLoop+16*WFSAM.fBidirectional+64*WFSAM.fReverse;
		       s[27]=0;
		       if (!strcmpi("EMBEDDED",WaveFile)) { //EMBEDDED
			 strcpy(sName,WFESI.szName); //New Name!
			 DownSample(WFESI.nNumber,s);
			 printf(" EMB.");
			 if (_dos_open(FileName, O_RDONLY, &fw) != 0) Error(8);
			 lseek(fw,tell(fh),SEEK_SET);
		       }
		       else { //WAVE file
			 printf(" WAVE");
			 if (_dos_open(WaveFile, O_RDONLY, &fw) != 0) Error(8);
			 if (_dos_read(fw, &ChunkH, sizeof(struct ChunkHeader), &ReadBytes) != 0) Error(9);
			 if (ReadBytes<sizeof(struct ChunkHeader)) Error(9);
			 if (strncmpi(ChunkH.szName,"RIFF",4)) Error(18);
			 if (_dos_read(fw, &ChunkH.szName, sizeof(ChunkH.szName), &ReadBytes) != 0) Error(9);
			 if (ReadBytes<sizeof(ChunkH.szName)) Error(9);
			 if (strncmpi(ChunkH.szName,"WAVE",4)) Error(18);

			 ChunkH.dwSize=0;
			 do
			 {
			   lseek(fw,ChunkH.dwSize,SEEK_CUR);
			   if (_dos_read(fw, &ChunkH, sizeof(struct ChunkHeader), &ReadBytes) != 0) Error(9);
			   if (ReadBytes<sizeof(struct ChunkHeader)) Error(9);
			 } while (strncmpi(ChunkH.szName,"DATA",4));
			 if (WFESI.dwSizeInBytes==0)
			 {
			   WFESI.dwSizeInBytes=ChunkH.dwSize/(WFESI.nChannel==0?1:2);
			   WFESI.dwSizeInSamples=WFESI.dwSizeInBytes/(WFSAM.fSampleResolution==0?2:1);
			 }
			 Long2MIDI(s+3,WFESI.dwSizeInSamples,4);
			 strcpy(sName,WFESI.szName);
			 DownSample(WFESI.nNumber,s);
		       }
		       while (WFESI.dwSizeInBytes>0)
		       {
			 if (WFESI.nChannel==0 || !strcmpi("EMBEDDED",WaveFile))
			 {
			   m=(WFESI.dwSizeInBytes<4096?WFESI.dwSizeInBytes:4096);
			   if (_dos_read(fw, aBuf, m, &ReadBytes) != 0) Error(9);
			   if (ReadBytes<m) Error(9);
			 }
			 else
			 {
			   m=(WFESI.dwSizeInBytes<4096?WFESI.dwSizeInBytes:4096);
			   if (_dos_read(fw, aBuf, 2*m, &ReadBytes) != 0) Error(9);
			   if (ReadBytes<2*m) Error(9);
			   if (WFSAM.fSampleResolution==2) //8 bit stereo
			   {
			     for(w=0;w<m;w++) aBuf[w]=aBuf[2*w+WFESI.nChannel-1];
			   }
			   else //16 bit stereo
			   {
			     for(w=0;w<m;w+=2)
			     {
			       aBuf[w]=aBuf[2*w+(WFESI.nChannel-1)*2];
			       aBuf[w+1]=aBuf[2*w+1+(WFESI.nChannel-1)*2];
			     }
			   }
			 }
			 if (WFSAM.fSampleResolution==2) { //8 bit linear
			   for (w=0;w<m;w++) aBuf[w]+=0x80;
			 }
			if (nNoLoad==0) {
			 if (MPUuseMIDI) // SampleStore via MIDI
			 {
			    register WORD w0,w1;
			    register WORD e=((m+15)&0x1ff0);

			    printf("\n");
			    memset(s,' ',60);
			    s[60]=0;

			    if (MPU401sendByte(0xf0) || MPU401sendByte(0x00) ||
				MPU401sendByte(0x00) || MPU401sendByte(0x65) ||
				MPU401sendByte(0x10) || MPU401sendByte(0x01) ||
				MPU401sendByte(0x01))
			      Error(24);

			    for (w0=0;w0<e;w0++)
			    {
			      if (MPU401sendByte(aBuf[w0] & 0x7f) ||
				  MPU401sendByte((aBuf[w0] >> 7) & 1))
				  Error(24);

			      if (!(w0 % 100))
				  printf("\r%3d%% %-60s %8ld",(int)(((float)(percent+=100)/(WFHead.dwMemoryRequired))*100),
					   strnset(s,'þ',(int)(((float)(percent)/(WFHead.dwMemoryRequired))*60)),percent);
			      if (kbhit() && getch()==27) Error(25);
			    };
			    if (MPU401sendByte(0xf7)) Error(24);
			    gotoxy(1,wherey()-1);
			 }
			 else // SampleStore via HOST
			 {
			   s[0]=HCMDDBLK;
			   Command(s,1,1);
			   asm {
			     push ds
			     push si
			     mov dx,MPUEmuData
			     add dx,4
			     mov cx,m
			     add cx,15
			     and cx,0x1ff0
			     //lds si,aBuf
			     mov si,aBuf
			     shr cx,1
			     dec cx
			     cld

			     rep outsw

			     add dx,2
			     outsw

			     pop si
			     pop ds
			   }
			 }
			 Command(s,0,1);
			}
			 //printf("%s\b",((WFESI.dwSizeInBytes/4096) % 2?"\\":"/"));
			 WFESI.dwSizeInBytes-=m;
		       }
		       _dos_close(fw);
		     }

		     break;
		case ST_MULTISAMPLE:
		     printf("MULTI      ");
		     if (_dos_read(fh, &m, 2, &ReadBytes) != 0) Error(9);
		     if (ReadBytes<2) Error(9);
		     if (_dos_read(fh, aBuf, 2<<m, &ReadBytes) != 0) Error(9);
		     if (ReadBytes<(2<<m)) Error(9);
		     if (FileType!=BANKFILE)
		       for (w=0;w<1<<m;w++)
		       {
			 f=(aBuf[2*w]+256*aBuf[2*w+1])&511;
			 if (SampleNew[f]!=UNDEFINED)
			 {
			   aBuf[2*w]=SampleNew[f]&255;
			   aBuf[2*w+1]=(SampleNew[f]&256)/256;
			 }
		       }
		     memcpy(s+4,aBuf,2<<m);
		     // Int2Midi(aBuf,s+4,1<<m);
		     strcpy(sName,WFESI.szName);
		     DownMultiSample(WFESI.nNumber,m,s);
		     break;
		case ST_ALIAS:
		     printf("ALIAS      ");
		     if (_dos_read(fh, &WFAL, sizeof(struct ALIAS), &ReadBytes) != 0) Error(9);
		     if (ReadBytes<sizeof(struct ALIAS)) Error(9);
		     if (SampleNew[WFAL.nOriginalSample]!=UNDEFINED) WFAL.nOriginalSample=SampleNew[WFAL.nOriginalSample];

		     GetSampleName(WFAL.nOriginalSample);
		     if (nNoLoad>0) printf("%d, %s",WFAL.nOriginalSample,sName);
		     Long2MIDI(s+5,WFAL.sampleStartOffset,4);
		     Long2MIDI(s+9,WFAL.loopStartOffset,4);
		     Long2MIDI(s+13,WFAL.loopEndOffset,4);
		     Long2MIDI(s+17,WFAL.sampleEndOffset,4);
		     Long2MIDI(s+21,WFAL.nFrequencyBias,3);
		     s[24]=8*WFAL.fLoop+16*WFAL.fBidirectional+64*WFAL.fReverse;
		     s[25]=0;
		     strcpy(sName,WFESI.szName);
		     DownAliasSample(WFESI.nNumber,WFAL.nOriginalSample,s);
		     break;
		case ST_EMPTY:
		     printf("Empty      ");
		     break;
		default: break;
	      }
	      WFHead.dwSampleOffset+=WFESI.dwSize;
	      Pause(PAUSEDEBUG);
	    } //for
	  }
	  else if (FileType==WAVEFILE) //WAVEFILE
	  {
	    l=tell(fh);
	    for (j=0;j<ChunkFmt.wChannels;j++)
	    {
	      lseek(fh,l,SEEK_SET);
	      printf(" WAVE Sample (%d) ",SampleNew[j]);

	      WavSizeInBytes=ChunkH.dwSize/(ChunkFmt.wChannels==1?1:2);
	      if (WavSizeInBytes>=1048576) Error(20);
	      WavSizeInSamples=WavSizeInBytes/(ChunkFmt.wResolution==16?2:1);

	      s[26]=(ChunkFmt.wResolution==16?0:2);

	      if (WavLoopStart==UNDEFINED)
	      {
		WavLoopStart=0;
		WavLoopEnd=WavSizeInSamples;
	      }

	      if (Loop==YES) s[26]|=8; else if (Loop==NO) s[26]&=(255-8);
	      if (Bidirectional==YES) s[26]|=16; else if (Bidirectional==NO) s[26]&=(255-16);
	      if (Reverse==YES) s[26]|=64; else if (Reverse==NO) s[26]&=(255-64);

	      if (WavLoopEnd>WavSizeInSamples) WavLoopEnd=WavSizeInSamples;
	      if (WavLoopStart<0 || WavLoopStart>WavLoopEnd) WavLoopEnd=0;

	      Long2MIDI(s+3,WavSizeInSamples,4);
	      Long2MIDI(s+7,0,4);
	      Long2MIDI(s+11,16*WavLoopStart,4);
	      Long2MIDI(s+15,16*WavLoopEnd,4);
	      Long2MIDI(s+19,16*WavSizeInSamples,4);

	      Long2MIDI(s+23,(long)((log(44100.0/ChunkFmt.dwSamplesPerSec)/log(2.0))*2048.0+(WavKey*2048.0)/12.0),3);

	      //+16*fBidirectional+64*fReverse;
	      s[27]=0;

	      GetName(FileName,ChunkFmt.wChannels,j);

	      DownSample(SampleNew[j],s);
	      while (WavSizeInBytes>0)
	      {
		if (ChunkFmt.wChannels==1) //MONO
		{
		  m=(WavSizeInBytes<4096?WavSizeInBytes:4096);
		  if (_dos_read(fh, aBuf, m, &ReadBytes) != 0) Error(9);
		  if (ReadBytes<m) Error(9);
		}
		else //STEREO
		{
		  m=(WavSizeInBytes<4096?WavSizeInBytes:4096);
		  if (_dos_read(fh, aBuf, 2*m, &ReadBytes) != 0) Error(9);
		  if (ReadBytes<2*m) Error(9);
		  if (ChunkFmt.wResolution==8) //8 bit stereo
		  {
		    if (MonoSample==NO) for(w=0;w<m;w++) aBuf[w]=aBuf[2*w+j];
		    // Convert STEREO to MONO
		    else for(w=0;w<m;w++) aBuf[w]=((WORD)aBuf[2*w]+(WORD)aBuf[2*w+1])/2;
		  }
		  else //16 bit stereo
		  {
		    int v1,v2;
		    long v;

		    if (MonoSample==NO)
		      for(w=0;w<m;w+=2)
		      {
			aBuf[w]=aBuf[2*w+j*2];
			aBuf[w+1]=aBuf[2*w+1+j*2];
		      }
		    else // Convert STEREO to MONO
		      for(w=0;w<m;w+=2)
		      {
			memcpy(&v1,aBuf+(2*w),2);
			memcpy(&v2,aBuf+(2+2*w),2);
			v=(long)v1+(long)v2;
			if (v<-32768) v=-32768;
			else if (v>32767) v=32767;

			aBuf[w]=v & 255;
			aBuf[w+1]=(v >> 8) & 255;
		      }
		  }
		}
		if (ChunkFmt.wResolution==8) { //8 bit linear
		  for (w=0;w<m;w++) aBuf[w]+=0x80;
		}
	       if (nNoLoad==0) {
		if (MPUuseMIDI) // SampleStore via MIDI
		{
		   register WORD w0,w1;
		   register WORD e=((m+15)&0x1ff0);

		   printf("\n");
		   memset(s,' ',60);
		   s[60]=0;

		   if (MPU401sendByte(0xf0) || MPU401sendByte(0x00) ||
		       MPU401sendByte(0x00) || MPU401sendByte(0x65) ||
		       MPU401sendByte(0x10) || MPU401sendByte(0x01) ||
		       MPU401sendByte(0x01))
		     Error(24);

		   for (w0=0;w0<e;w0++)
		   {
		     if (MPU401sendByte(aBuf[w0] & 0x7f) ||
			 MPU401sendByte((aBuf[w0] >> 7) & 1))
			 Error(24);

		     if (!(w0 % 100))
			 printf("\r%3d%% %-60s %8ld",(int)(((float)(percent+=100)/(percentm))*100),
				  strnset(s,'þ',(int)(((float)(percent)/(percentm))*60)),percent);
                     if (kbhit() && getch()==27) Error(25);
		   };
		   if (MPU401sendByte(0xf7)) Error(24);
		   gotoxy(42,wherey()-1);
		}
		else // SampleStore ia HOST
		{
		  s[0]=HCMDDBLK;
		  Command(s,1,1);
		  asm {
		    push ds
		    push si
		    mov dx,MPUEmuData
		    add dx,4
		    mov cx,m
		    add cx,15
		    and cx,0x1ff0
		    //lds si,aBuf
		    mov si,aBuf
		    shr cx,1
		    dec cx
		    cld

		    rep outsw

		    add dx,2
		    outsw

		    pop si
		    pop ds
		  }
		}
		Command(s,0,1);
	       }
		WavSizeInBytes-=m;
	      }
	    if (MonoSample==YES) ChunkFmt.wChannels=1;
	    } //for
	  }
	  if (FileType<WFSFILE)
	  {
	    printf("\n                                                                             "
		   "\rPatch(es) loading: ");
	    for (j=0;j<256;j++) PatchNew[j]=UNDEFINED; //Set to None
	  }
	  if (FileType<WAVEFILE)
	  {
	    lseek(fh,WFHead.dwPatchOffset,SEEK_SET);
	    for (j=1;j<=WFHead.wPatchCount;j++) {
	      if (_dos_read(fh, &WFPatch, sizeof(struct WaveFrontPatch), &ReadBytes) != 0) Error(9);
	      if (ReadBytes<sizeof(struct WaveFrontPatch)) Error(9);

	      w=WFPatch.nNumber;
	      if (FileType!=BANKFILE)
	      {
		if (FirstPatch>=0) WFPatch.nNumber=FirstPatch++; //1.40
		if (FileType!=BANKFILE) GetPatchName(WFPatch.nNumber);
		else strcpy(sName,PatchName[WFPatch.nNumber]);
		if (Patches[WFPatch.nNumber]!=EMPTY && strcmp(WFPatch.szName,sName))
		{
		  if (Patches[WFPatch.nNumber]!=EMPTY) WFPatch.nNumber=SignFreePatch();
		  if (WFPatch.nNumber==256) Error(17);
		  PatchNew[w]=WFPatch.nNumber;
		}

		m=WFPatch.bySampleNumber+256*WFPatch.fSampleMSB;
		if (SampleNew[m]!=UNDEFINED)
		{
		  WFPatch.bySampleNumber=SampleNew[m]&255;
		  WFPatch.fSampleMSB=(SampleNew[m]&256)/256;
		}
	      }

	      if (nNoLoad>0) printf("\n");
	      printf("%3d (%3d%1s) = %-32s",j,WFPatch.nNumber,(w==WFPatch.nNumber?"":"R"),WFPatch.szName);
	      gotoxy(wherex()-45,wherey());
	      strcpy(sName,WFPatch.szName);
	      DownPatch(WFPatch.nNumber, (BYTE*)&WFPatch, s);
	      Pause(PAUSEDEBUG);
	    }

	    if (FileType==BANKFILE)
	    {
	      // UNUSED PATCHES
	      for (m=0;m<256;m++) Patches[m]=EMPTY;
	    }

	    if (WFHead.wDrumkitCount) printf("\nDrumKit(s) loading:  ");
	    lseek(fh,WFHead.dwDrumkitOffset,SEEK_SET);
	    for (j=1;j<=WFHead.wDrumkitCount;j++) {
	      if (_dos_read(fh, &WFD, sizeof(struct WaveFrontDrumkit), &ReadBytes) != 0) Error(9);
	      if (ReadBytes<sizeof(struct WaveFrontDrumkit)) Error(9);
	      printf("%3d",j);
	      gotoxy(wherex()-3,wherey());
	      for (m=0;m<128;m++) {
		if (WFD.drum[m].fUnmute)
		{
		  if (FileType!=BANKFILE && PatchNew[WFD.drum[m].byPatchNumber]!=UNDEFINED) WFD.drum[m].byPatchNumber=PatchNew[WFD.drum[m].byPatchNumber];
  //		if (FileType==BANKFILE) Patches[WFD.drum[m].byPatchNumber]+=USED;
		}
		DownDrumProgram(m, (BYTE*)&WFD.drum[m], s);
	      }
	      Pause(PAUSEDEBUG);
	    }
	    if (WFHead.wDrumkitCount) SetUsed(DRUMKIT);

	    printf("\nProgram(s) loading:  ");
	    lseek(fh,WFHead.dwProgramOffset,SEEK_SET);
	    for (j=1;j<=WFHead.wProgramCount;j++) {
	      if (_dos_read(fh, &WFP, sizeof(struct WaveFrontProgram), &ReadBytes) != 0) Error(9);
	      if (ReadBytes<sizeof(struct WaveFrontProgram)) Error(9);
	      if (FileType!=BANKFILE)
	      {
		if (FirstPrg>=0) WFP.nNumber=FirstPrg++;
		for (m=0;m<4;m++)
		{
		  if (WFP.layer[m].fUnmute && PatchNew[WFP.layer[m].byPatchNumber]!=UNDEFINED)
		    WFP.layer[m].byPatchNumber=PatchNew[WFP.layer[m].byPatchNumber];
		}
	      };
  /*	    else // BANKFILE
	      {
		for (m=0;m<4;m++)
		{
		  if (WFP.layer[m].fUnmute) Patches[WFP.layer[m].byPatchNumber]+=USED;
		}
	      }
  */
	      if (nNoLoad>0) printf("\n");
	      printf("%3d (%3d) = %-32s",j,WFP.nNumber,WFP.szName);
	      gotoxy(wherex()-44,wherey());
	      strcpy(sName,WFP.szName);
	      DownProgram(WFP.nNumber, (BYTE*)&WFP, s);
	      SetUsed(WFP.nNumber);
	      Pause(PAUSEDEBUG);
	    }
	  }
	  else if (FileType==WAVEFILE) //WAVEFILE
	  {
	    if (!isWSS)
	    {
	      memcpy(&WFP,&DefPrg,sizeof(DefPrg));
	      memcpy(&WFD.drum[0],&DefDrum,sizeof(DefDrum));
	    }
	    else
	    {
	      memcpy(&WFP,&WSS.layer,sizeof(WSS.layer));
	      memcpy(&WFD,&WSS.layer,sizeof(WFD.drum));
	    }

	    for (j=0;j<ChunkFmt.wChannels;j++)
	    {
	      if (!isWSS)
	      {
		memcpy(&WFPatch,&DefPatch,sizeof(WFPatch));
		w=j;
	      }
	      else
	      {
		if (j==0)
		{
		  memcpy(&WFPatch,&WSS.patch1.nFreqBias,sizeof(WFPatch)-2-32);
		  w=WSS.bPatchNumber1;
		}
		else
		{
		  memcpy(&WFPatch,&WSS.patch2.nFreqBias,sizeof(WFPatch)-2-32);
		  w=WSS.bPatchNumber2;
		}
	      }
	      if (Patches[w]!=EMPTY)
	      {
		w=SignFreePatch();
		if (w==256) Error(17);
	      }
	      printf("%3d (%3d)",j,w);
	      if (!WavDrum) WFP.layer[j].byPatchNumber=w;
	      else WFD.drum[0].byPatchNumber=w;

	      if (!isWSS)
	      {
		if (!WavDrum)
		{
		  WFP.layer[j].fMixLevel=WavVol;
		  WFP.layer[j].fUnmute=1;
		}
		else
		{
		  WFD.drum[0].fMixLevel=WavVol;
		  WFD.drum[0].fUnmute=1;
		}
	      }

	      WFPatch.bySampleNumber=SampleNew[j]&255;
	      WFPatch.fSampleMSB=(SampleNew[j]&256)/256;
	      gotoxy(wherex()-45,wherey());
	      GetName(FileName,ChunkFmt.wChannels,j);
	      DownPatch(w, (BYTE*)&WFPatch, s);
	      Pause(PAUSEDEBUG);
	    }
	    if (!isWSS)
	    {
	      if (!WavDrum)
	      {
		if (ChunkFmt.wChannels==2)
		{
		  WFP.layer[0].fPan=0;
		  WFP.layer[1].fPan=7;
		}
		else
		{
		  WFP.layer[0].fPan=3;
		}
	      }
	      else
	      {
		WFD.drum[0].fPanAmount=3;
	      }
	    }
	    if (!WavDrum)
	    {
	      printf("\nProgram loading:  %3d",WavPrg);
	      GetName(FileName,0,0);
	      DownProgram(WavPrg, (BYTE*)&WFP, s);
	      SetUsed(WavPrg);
	    }
	    else
	    {
	      printf("\nDrum loading:  %3d",WavPrg);
	      DownDrumProgram(WavPrg, (BYTE*)&WFD.drum[0], s);
	      SetUsed(WavPrg+128);
	    }
	    Pause(PAUSEDEBUG);
	  }

	  _dos_close(fh);
	  free(aBuf);
	} // else
      } // else

      if (isWSS) // Sample Set
      {
	if (FileType!=WSSFILE) isWSS--;
	if (!isWSS) //  0 -> end
	{
	  argumc=save_argumc;
	  memcpy(argums,save_argums,128);
	  memcpy(argum,save_argum,sizeof(argum));
	  i=save_i;
	  _dos_close(fh);
	}
	else //Load a sample
	{
	  //lseek(WSSfh,WSSpos,SEEK_SET);
	  if (_dos_read(WSSfh, y, 1, &ReadBytes)) Error(9);
	  if (_dos_read(WSSfh, argums, y[0], &ReadBytes)) Error(9);
	  argums[y[0]]=0;

	  //Load Sample Name
	  if (_dos_read(WSSfh, y, 1, &ReadBytes)) Error(9);
	  if (_dos_read(WSSfh, WSS.SampleName, y[0], &ReadBytes)) Error(9);
	  WSS.SampleName[y[0]]=0; //inserted OK

	  //Load the whole data
	  if (_dos_read(WSSfh, y, (isWSS>1?0xe2:0xe0), &ReadBytes)) Error(9);

	  WSS.fIsDrum=y[0]; // inserted OK
	  WSS.fIsMono=y[2]; // inserted OK
	  WSS.fIsLooped=y[4];   // inserted OK
	  WSS.bProgramNumber=y[6]; //inserted OK

	  //Program serialize
	  if (!WSS.fIsDrum) //Non drum
	  {
	    for (i=0;i<4;i++) //LAYERS
	    {
	     WSS.layer[i].byPatchNumber=y[8+i*16];  // inserted OK
	     WSS.layer[i].fMixLevel=y[9+i*16];
	     WSS.layer[i].fUnmute=y[10+i*16];             //BOOL=WORD!
	     WSS.layer[i].fSplitPoint=y[12+i*16];   //WORD (because NEXT=WORD)
	     WSS.layer[i].fSplitDir=y[14+i*16];     //BOOL=WORD!
	     WSS.layer[i].fPanModSource=y[16+i*16];       //WORD (because NEXT=WORD)
	     WSS.layer[i].fPanModulated=y[18+i*16];       //BOOL=WORD
	     WSS.layer[i].fPan=y[20+i*16];                //WORD (because NEXT=WORD)
	     WSS.layer[i].fSplitType=y[22+i*16];       //BOOL=WORD
	    }
	  }
	  else
	  {
	    //WSS.bMIDINoteNumber=y[7]; //BYTE - nem kell, mert bProgrumNumber

	    WSS.byPatchNumber=y[8];   //BYTE
	    WSS.fMixLevel=y[9];       //WORD (because NEXT=WORD)
	    WSS.fUnmute=y[11];        //BOOL=WORD
	    WSS.fGroup=y[13];         //BYTE
	    WSS.fUnused1=0;
	    WSS.fPanModSource=y[14];  //BYTE
	    WSS.fPanModulated=y[15];  //BOOL=WORD
	    WSS.fPanAmount=y[17];
	    WSS.fUnused2=0;
	  }

	  // Patch info
	  WSS.bPatchNumber1=y[72];
	  i=73;

	  // Patch 1
	  WSS.patch1.nFreqBias=(int)(y[i]+256*y[i+1]); i+=2;
	  WSS.patch1.fAmpBias=y[i++];
	  WSS.patch1.fUnused1=0;
	  WSS.patch1.fPortamento=y[i++];
	  WSS.patch1.fUnused2=0;
	  WSS.patch1.bySampleNumber=y[i++];
	  WSS.patch1.fSampleMSB=y[i++];
	  WSS.patch1.fUnused3=0;

	  WSS.patch1.fPitchBend=y[i++];

	/*WSS.patch1.fMono:1;
	  WSS.patch1.fRetrigger:1;
	  WSS.patch1.fNoHold:1;
	  WSS.patch1.fRestart:1;
	  WSS.patch1.fFilterConfig:2;
	  WSS.patch1.fReuse:1;
	  WSS.patch1.fResetLfo:1;*/
	  memcpy((BYTE*)(&WSS.patch1.bySampleNumber)+2,&y[i++],1);

	  WSS.patch1.fFMSource1=y[i++];
	  WSS.patch1.fFMSource2=y[i++];
	  WSS.patch1.cFMAmount1=y[i++];
	  WSS.patch1.cFMAmount2=y[i++];
	  WSS.patch1.fAMSource=y[i++];
	  WSS.patch1.fUnused4=0;
	  WSS.patch1.cAMAmount=y[i++];
	  WSS.patch1.fFC1MSource=y[i++];
	  WSS.patch1.fFC2MSource=y[i++];
	  WSS.patch1.cFC1MAmount=y[i++];
	  WSS.patch1.cFC1KeyScale=y[i++];
	  WSS.patch1.cFC1FreqBias=y[i++];
	  WSS.patch1.cFC2MAmount=y[i++];
	  WSS.patch1.cFC2KeyScale=y[i++];
	  WSS.patch1.cFC2FreqBias=y[i++];
	  WSS.patch1.fRandomizerRate=y[i++];
	  WSS.patch1.fUnused5=0;

	  WSS.patch1.envelope1.fAttackTime=y[i++];
	  WSS.patch1.envelope1.fUnused1=0;
	  WSS.patch1.envelope1.fDecay1Time=y[i++];
	  WSS.patch1.envelope1.fUnused2=0;
	  WSS.patch1.envelope1.fDecay2Time=y[i++];
	  WSS.patch1.envelope1.fUnused3=0;
	  WSS.patch1.envelope1.fSustainTime=y[i++];
	  WSS.patch1.envelope1.fUnused4=0;
	  WSS.patch1.envelope1.fReleaseTime=y[i++];
	  WSS.patch1.envelope1.fUnused5=0;
	  WSS.patch1.envelope1.fRelease2Time=y[i++];
	  WSS.patch1.envelope1.fUnused6=0;
	  WSS.patch1.envelope1.cAttackLevel=y[i++];
	  WSS.patch1.envelope1.cDecay1Level=y[i++];
	  WSS.patch1.envelope1.cDecay2Level=y[i++];
	  WSS.patch1.envelope1.cSustainLevel=y[i++];
	  WSS.patch1.envelope1.cReleaseLevel=y[i++];
	  WSS.patch1.envelope1.fAttackVelocity=y[i++];
	  WSS.patch1.envelope1.fUnused7=0;
	  WSS.patch1.envelope1.fVolumeVelocity=y[i++];
	  WSS.patch1.envelope1.fUnused8=0;
	  WSS.patch1.envelope1.fKeyScale=y[i++];
	  WSS.patch1.envelope1.fUnused9=0;

	  WSS.patch1.envelope2.fAttackTime=y[i++];
	  WSS.patch1.envelope2.fUnused1=0;
	  WSS.patch1.envelope2.fDecay1Time=y[i++];
	  WSS.patch1.envelope2.fUnused2=0;
	  WSS.patch1.envelope2.fDecay2Time=y[i++];
	  WSS.patch1.envelope2.fUnused3=0;
	  WSS.patch1.envelope2.fSustainTime=y[i++];
	  WSS.patch1.envelope2.fUnused4=0;
	  WSS.patch1.envelope2.fReleaseTime=y[i++];
	  WSS.patch1.envelope2.fUnused5=0;
	  WSS.patch1.envelope2.fRelease2Time=y[i++];
	  WSS.patch1.envelope2.fUnused6=0;
	  WSS.patch1.envelope2.cAttackLevel=y[i++];
	  WSS.patch1.envelope2.cDecay1Level=y[i++];
	  WSS.patch1.envelope2.cDecay2Level=y[i++];
	  WSS.patch1.envelope2.cSustainLevel=y[i++];
	  WSS.patch1.envelope2.cReleaseLevel=y[i++];
	  WSS.patch1.envelope2.fAttackVelocity=y[i++];
	  WSS.patch1.envelope2.fUnused7=0;
	  WSS.patch1.envelope2.fVolumeVelocity=y[i++];
	  WSS.patch1.envelope2.fUnused8=0;
	  WSS.patch1.envelope2.fKeyScale=y[i++];
	  WSS.patch1.envelope2.fUnused9=0;

	  WSS.patch1.lfo1.bySampleNumber=y[i++];
	  WSS.patch1.lfo1.fFrequency=y[i++];
	  WSS.patch1.lfo1.fUnused1=0;
	  WSS.patch1.lfo1.fAMSource=y[i++];
	  WSS.patch1.lfo1.fFMSource=y[i++];
	  WSS.patch1.lfo1.cFMAmount=y[i++];
	  WSS.patch1.lfo1.cAMAmount=y[i++];
	  WSS.patch1.lfo1.cStartLevel=y[i++];
	  WSS.patch1.lfo1.cEndLevel=y[i++];
	  WSS.patch1.lfo1.fDelayTime=y[i++];
	  WSS.patch1.lfo1.fWaveRestart=y[i]; i+=2;
	  WSS.patch1.lfo1.fRampTime=y[i++];
	  WSS.patch1.lfo1.fUnused2=0;

	  WSS.patch1.lfo2.bySampleNumber=y[i++];
	  WSS.patch1.lfo2.fFrequency=y[i++];
	  WSS.patch1.lfo2.fUnused1=0;
	  WSS.patch1.lfo2.fAMSource=y[i++];
	  WSS.patch1.lfo2.fFMSource=y[i++];
	  WSS.patch1.lfo2.cFMAmount=y[i++];
	  WSS.patch1.lfo2.cAMAmount=y[i++];
	  WSS.patch1.lfo2.cStartLevel=y[i++];
	  WSS.patch1.lfo2.cEndLevel=y[i++];
	  WSS.patch1.lfo2.fDelayTime=y[i++];
	  WSS.patch1.lfo2.fWaveRestart=y[i]; i+=2;
	  WSS.patch1.lfo2.fRampTime=y[i++];
	  WSS.patch1.lfo2.fUnused2=0;

	  // Patch 1
	  WSS.bPatchNumber2=y[i++];

	  WSS.patch2.nFreqBias=(int)(y[i]+256*y[i+1]); i+=2;
	  WSS.patch2.fAmpBias=y[i++];
	  WSS.patch2.fUnused1=0;
	  WSS.patch2.fPortamento=y[i++];
	  WSS.patch2.fUnused2=0;
	  WSS.patch2.bySampleNumber=y[i++];
	  WSS.patch2.fSampleMSB=y[i++] & 1;
	  WSS.patch2.fUnused3=0;

	  WSS.patch2.fPitchBend=y[i++];

	/*WSS.patch2.fMono:1;
	  WSS.patch2.fRetrigger:1;
	  WSS.patch2.fNoHold:1;
	  WSS.patch2.fRestart:1;
	  WSS.patch2.fFilterConfig:2;
	  WSS.patch2.fReuse:1;
	  WSS.patch2.fResetLfo:1;*/
	  memcpy((BYTE*)(&WSS.patch2.bySampleNumber)+2,&y[i++],1);

	  WSS.patch2.fFMSource1=y[i++];
	  WSS.patch2.fFMSource2=y[i++];
	  WSS.patch2.cFMAmount1=y[i++];
	  WSS.patch2.cFMAmount2=y[i++];
	  WSS.patch2.fAMSource=y[i++];
	  WSS.patch2.fUnused4=0;
	  WSS.patch2.cAMAmount=y[i++];
	  WSS.patch2.fFC1MSource=y[i++];
	  WSS.patch2.fFC2MSource=y[i++];
	  WSS.patch2.cFC1MAmount=y[i++];
	  WSS.patch2.cFC1KeyScale=y[i++];
	  WSS.patch2.cFC1FreqBias=y[i++];
	  WSS.patch2.cFC2MAmount=y[i++];
	  WSS.patch2.cFC2KeyScale=y[i++];
	  WSS.patch2.cFC2FreqBias=y[i++];
	  WSS.patch2.fRandomizerRate=y[i++];
	  WSS.patch2.fUnused5=0;

	  WSS.patch2.envelope1.fAttackTime=y[i++];
	  WSS.patch2.envelope1.fUnused1=0;
	  WSS.patch2.envelope1.fDecay1Time=y[i++];
	  WSS.patch2.envelope1.fUnused2=0;
	  WSS.patch2.envelope1.fDecay2Time=y[i++];
	  WSS.patch2.envelope1.fUnused3=0;
	  WSS.patch2.envelope1.fSustainTime=y[i++];
	  WSS.patch2.envelope1.fUnused4=0;
	  WSS.patch2.envelope1.fReleaseTime=y[i++];
	  WSS.patch2.envelope1.fUnused5=0;
	  WSS.patch2.envelope1.fRelease2Time=y[i++];
	  WSS.patch2.envelope1.fUnused6=0;
	  WSS.patch2.envelope1.cAttackLevel=y[i++];
	  WSS.patch2.envelope1.cDecay1Level=y[i++];
	  WSS.patch2.envelope1.cDecay2Level=y[i++];
	  WSS.patch2.envelope1.cSustainLevel=y[i++];
	  WSS.patch2.envelope1.cReleaseLevel=y[i++];
	  WSS.patch2.envelope1.fAttackVelocity=y[i++];
	  WSS.patch2.envelope1.fUnused7=0;
	  WSS.patch2.envelope1.fVolumeVelocity=y[i++];
	  WSS.patch2.envelope1.fUnused8=0;
	  WSS.patch2.envelope1.fKeyScale=y[i++];
	  WSS.patch2.envelope1.fUnused9=0;

	  WSS.patch2.envelope2.fAttackTime=y[i++];
	  WSS.patch2.envelope2.fUnused1=0;
	  WSS.patch2.envelope2.fDecay1Time=y[i++];
	  WSS.patch2.envelope2.fUnused2=0;
	  WSS.patch2.envelope2.fDecay2Time=y[i++];
	  WSS.patch2.envelope2.fUnused3=0;
	  WSS.patch2.envelope2.fSustainTime=y[i++];
	  WSS.patch2.envelope2.fUnused4=0;
	  WSS.patch2.envelope2.fReleaseTime=y[i++];
	  WSS.patch2.envelope2.fUnused5=0;
	  WSS.patch2.envelope2.fRelease2Time=y[i++];
	  WSS.patch2.envelope2.fUnused6=0;
	  WSS.patch2.envelope2.cAttackLevel=y[i++];
	  WSS.patch2.envelope2.cDecay1Level=y[i++];
	  WSS.patch2.envelope2.cDecay2Level=y[i++];
	  WSS.patch2.envelope2.cSustainLevel=y[i++];
	  WSS.patch2.envelope2.cReleaseLevel=y[i++];
	  WSS.patch2.envelope2.fAttackVelocity=y[i++];
	  WSS.patch2.envelope2.fUnused7=0;
	  WSS.patch2.envelope2.fVolumeVelocity=y[i++];
	  WSS.patch2.envelope2.fUnused8=0;
	  WSS.patch2.envelope2.fKeyScale=y[i++];
	  WSS.patch2.envelope2.fUnused9=0;

	  WSS.patch2.lfo1.bySampleNumber=y[i++];
	  WSS.patch2.lfo1.fFrequency=y[i++];
	  WSS.patch2.lfo1.fUnused1=0;
	  WSS.patch2.lfo1.fAMSource=y[i++];
	  WSS.patch2.lfo1.fFMSource=y[i++];
	  WSS.patch2.lfo1.cFMAmount=y[i++];
	  WSS.patch2.lfo1.cAMAmount=y[i++];
	  WSS.patch2.lfo1.cStartLevel=y[i++];
	  WSS.patch2.lfo1.cEndLevel=y[i++];
	  WSS.patch2.lfo1.fDelayTime=y[i++];
	  WSS.patch2.lfo1.fWaveRestart=y[i]; i+=2;
	  WSS.patch2.lfo1.fRampTime=y[i++];
	  WSS.patch2.lfo1.fUnused2=0;

	  WSS.patch2.lfo2.bySampleNumber=y[i++];
	  WSS.patch2.lfo2.fFrequency=y[i++];
	  WSS.patch2.lfo2.fUnused1=0;
	  WSS.patch2.lfo2.fAMSource=y[i++];
	  WSS.patch2.lfo2.fFMSource=y[i++];
	  WSS.patch2.lfo2.cFMAmount=y[i++];
	  WSS.patch2.lfo2.cAMAmount=y[i++];
	  WSS.patch2.lfo2.cStartLevel=y[i++];
	  WSS.patch2.lfo2.cEndLevel=y[i++];
	  WSS.patch2.lfo2.fDelayTime=y[i++];
	  WSS.patch2.lfo2.fWaveRestart=y[i]; i+=2;
	  WSS.patch2.lfo2.fRampTime=y[i++];
	  WSS.patch2.lfo2.fUnused2=0;

	  argumc=2;
	  argum[1]=argums;
	  i=0; // it will be incremented in the FOR
	}
      }
    } //for

    if (WFSfile[0] && !feof(stream))  // WaveFront SET
    {
      argumc=1;
      while (!feof(stream) && argumc==1)
      {
	fgets(argums, 128, stream);
	argum[1]=argums;
	for (i=2;i<64;i++) argum[i]=NULL;
	w=strlen(argums);
	j=1;
	for (i=0; i<w; i++)
	{
	  if (argums[i]==';' || argums[i]==13 || argums[i]==10)
	  {
	    argums[i]=0;
	    if (i>0 && j==1) j=2;
	    i=255;
	  }
	  else if (argums[i]==' ')  // Space
	  {
	    argum[j+1]=argums+i+1;
	    argums[i]=0;
	    if (argums[i+1]!=' ') j++;
	  }
	}
	argumc=j;
      }
      printf("\n");
      procend=feof(stream);
    }
    else procend=1;
  } // DO-LOOP
  while (!procend);
  if (WFSfile[0])
  {
    fclose(stream);
  }


  if (chstat) {
    printf("\nþ Channel Status:");
    s[0]=HCMDGETSCS;
    Command(s,1,3);
    w=s[0]+s[1]*128+s[2]*16384;

    s[0]=HCMDREPCPN;
    Command(s,1,32);
    for (m=1,i=0;i<16;i++,m<<=1) {
      GetProgramName(MIDI2Long(s+2*i,2));
      printf("\nMIDI Channel #%2i: %-8s %s (%d)",i+1,(w&m?Enabled:Disabled),(MIDI2Long(s+2*i,2)<129?sName:"*** DRUM Channel ***"),MIDI2Long(s+2*i,2));
    }
    Pause(PAUSEPAUSE);
  }
  if (status) {
    printf("\nþ WaveFront Status:");
    s[0]=HCMDREPHWV;
    Command(s,1,2);
    printf("\nWaveFront Hardware Version: %u.%u",s[0],s[1]);
    s[0]=HCMDREPFWV;
    Command(s,1,2);
    printf("\nFirmware Version: %u.%u",s[0],s[1]);

    s[0]=HCMDGETVOL;
    Command(s,1,1);
    printf("\nSynthesizer Volume: %u",s[0]);

    s[0]=HCMDGETNOV;
    Command(s,1,1);
    printf("\nNumber of Voices: %i",s[0]);

    s[0]=HCMDGETTUN;
    Command(s,1,2);
    printf("\nSynthesizer Tuning: %d",(s[1]<0x40?s[0]+s[1]*128:(s[0]+s[1]*128)-16384));

    s[0]=HCMDREPMST;
    Command(s,1,1);
    printf("\nVirtual MIDI Mode: ");
    if ((s[0]&1)==1) printf("Enabled"); else printf("Disabled");
    printf("\nVirtual MIDI Switch: ");
    if ((s[0]&2)==2) printf("External"); else printf("Synth");
    printf("\nMIDI-In to synth Mode: ");
    if ((s[0]&4)==0) printf("Enabled"); else printf("Disabled");

    s[0]=HCMDREPNOS;
    Command(s,1,2);
    printf("\nNumber of samples: %u",s[0]+s[1]*128);

    s[0]=HCMDREPMEM;
    Command(s,1,1);
    Command(s+1,0,s[0]);
    lv=0;
    for (i=0;i<s[0];i++)
    {
      switch (s[i+1])
      {
	case 1: lv+=1024; break;
	case 2: lv+=512; break;
	case 4: lv+=256; break;
	case 8: lv+=128; break;
	case 16: lv+=64; break;
	default: break;
      }
    }
    printf("\nWaveFront Sample DRAM size: %ld bytes.",lv*1024);
    Command(s,0,1);
    Command(s+1,0,s[0]);
    lv=0;
    for (i=0;i<s[0];i++)
    {
      switch (s[i+1])
      {
	case 1: lv+=1024; break;
	case 2: lv+=512; break;
	case 4: lv+=256; break;
	case 8: lv+=128; break;
	case 16: lv+=64; break;
	default: break;
      }
    }
    printf("\nWaveFront Sample ROM size: %ld bytes.",lv*1024);

    s[0]=HCMDREPFRE;
    Command(s,1,4);
    printf("\nWaveFront Free Memory: %ld byte(s)",MIDI2Long(s,4));
  }

  CloseXMS();

  printf("\n");
  return 0;
}