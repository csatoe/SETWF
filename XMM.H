/*
 *	XMS Driver C Interface Routine Definitions
 */

#include "maui.h"

#ifndef XMM_H
#define XMM_H

#define XXMSMEMOFFS 16L                 // (long)

#define XPRGOFFS 20L
#define XPRGSIZE (16L+32L)
#define XPRGALL 128L*XPRGSIZE

#define XDPRGOFFS XPRGOFFS+XPRGALL
#define XDPRGSIZE (4L)
#define XDPRGALL 128L*XDPRGSIZE

#define XPATCHOFFS XDPRGOFFS+XDPRGALL
#define XPATCHSIZE (66L+32L)
#define XPATCHALL 256L*XPATCHSIZE

#define XSAMPLEOFFS XPATCHOFFS+XPATCHALL
#define XSAMPLESIZE (1L+32L+2L+257L)
#define XSAMPLEALL 512L*XSAMPLESIZE

#define XPATCHNEWOFFS XSAMPLEOFFS+XSAMPLEALL
#define XPATCHNEWSIZE sizeof(Patches)

#define XSAMPLENEWOFFS XPATCHNEWOFFS+XPATCHNEWSIZE
#define XSAMPLENEWSIZE sizeof(Samples)

#define XXMSALL XSAMPLENEWOFFS+XSAMPLENEWSIZE
WORD XMM_Installed();

WORD XMM_AllocateExtended (WORD kbyte, WORD *handle);
WORD XMM_FreeExtended(WORD handle);
WORD XMM_MoveExtended(DWORD bytes, WORD src_handle, long src_addr,
		      WORD dest_handle,  long dest_addr);
WORD XMM_GetHandleInfo(WORD handle, WORD *kbytes);

WORD isTHSXMS(void);    // 1 = true, handle in XMSHandle
WORD allocTHSXMS(void);  // 1 = ok
WORD freeTHSXMS(void);   // 1 = ok

WORD Prg2XMS(BYTE* Data, char* szName, WORD bNumber);
WORD DPrg2XMS(BYTE* Data, WORD bNumber);
WORD Patch2XMS(BYTE* Data, char* szName, WORD bNumber);
WORD Sample2XMS(BYTE* Data, WORD nBytes, char* szName, BYTE nType, WORD bNumber);



struct	XMM_Move {
	unsigned long	Length;
	unsigned short	SourceHandle;
	unsigned long	SourceOffset;
	unsigned short	DestHandle;
	unsigned long	DestOffset;
};

#define fromXMS( xms_offs,dos_offs,bytes ) ( XMM_MoveExtended(bytes,XMShandle,xms_offs,0,(long) dos_offs) )
#define toXMS( dos_offs,xms_offs,bytes ) ( XMM_MoveExtended(bytes,0,(long)dos_offs,XMShandle,xms_offs) )

#endif