/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   : -                                                      */
/*                                                                          */
/*   Version       : V1.00.0                                                */
/*                                                                          */
/*   Date          : 11.02.91                                               */
/*                                                                          */
/*   Written       : MH                                                     */
/*                                                                          */
/*   Dependency    : none                                                   */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/*
	Revision History:
	
	31.07.92 RF		Bool ist unsigned short, damit mit OS/2 vertr„glich...
    14.11.93 RF     BOOL ist nun wieder unsigned long ...	
*/
#ifndef STD_INC
#define STD_INC


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef BOOL
#define BOOL unsigned long
#endif

#ifndef NULL
//#define NULL 0L
#endif

#define forever for (;;)

#endif
