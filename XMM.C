/*
 * XMM handle routines by THS
 */

#include <dos.h>
#include <string.h>
#include "xmm.h"


//Debug
#include <stdio.h>

// The string searching in the XMS blocks
const char* THSsignature="SETMAUI 1.21THS";

// The XMS block size in kbytes
extern int Patches[256];
extern int Samples[512];

const WORD THSXMSsize=(WORD)(((XXMSALL)+1024L)/1024L);

WORD XMM_Initialised=0;
void far *XMM_Control;
struct XMM_Move block;
BYTE btemp;
WORD wtemp;
WORD XMShandle;


WORD XMM_Installed(void)
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


WORD XMM_AllocateExtended (WORD kbyte, WORD *handle)
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

WORD XMM_FreeExtended(WORD handle)
{
  asm {
    mov ah,0x0a;
    mov dx,handle;
    call [XMM_Control];
    mov wtemp,ax
  }
  return wtemp; // 1 = free ok, 0 = error
}

WORD XMM_MoveExtended(DWORD bytes, WORD src_handle, long src_addr,
		      WORD dest_handle, long dest_addr)
{
  BYTE wb;

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
    printf("%hd",wb);
    Error(99);
  }
  return wtemp; // 1 = Move OK, 0 = error
}

WORD XMM_GetHandleInfo(WORD handle, WORD *kbytes)
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

WORD isTHSXMS(void)
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

WORD allocTHSXMS(void)
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

WORD freeTHSXMS(void)
{
  if (XMShandle!=0)
  {
    if (!XMM_Installed()) return 0;
    return XMM_FreeExtended (XMShandle);
  }
  return 1;
}

///////////////////////////////////////////////////////////

WORD Prg2XMS(BYTE* Data, char* szName, WORD bNumber)
{
  BYTE tmp[XPRGSIZE];

  memcpy(tmp,Data,16);
  memcpy(tmp+16,szName,32);
  return toXMS(tmp,XPRGOFFS+bNumber*XPRGSIZE,XPRGSIZE);
}

WORD DPrg2XMS(BYTE* Data, WORD bNumber)
{
  BYTE tmp[XDPRGSIZE];

  memcpy(tmp,Data,4);
  return toXMS(tmp,XDPRGOFFS+bNumber*XDPRGSIZE,XDPRGSIZE);
}

WORD Patch2XMS(BYTE* Data, char* szName, WORD bNumber)
{
  BYTE tmp[XPATCHSIZE];

  memcpy(tmp,Data,66);
  memcpy(tmp+66,szName,32);
  return toXMS(tmp,XPATCHOFFS+bNumber*XPATCHSIZE,XPATCHSIZE);
}

WORD Sample2XMS(BYTE* Data, WORD nBytes, char* szName, BYTE nType, WORD bNumber)
{
  BYTE tmp[XSAMPLESIZE];

  memcpy(tmp,&nType,1);
  memcpy(tmp+1,szName,32);
  memcpy(tmp+33,&nBytes,2);
  memcpy(tmp+35,Data,nBytes);
  return toXMS(tmp,XSAMPLEOFFS+bNumber*XSAMPLESIZE,XSAMPLESIZE);
}
