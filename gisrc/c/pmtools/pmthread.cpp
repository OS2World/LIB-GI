/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   : none                                                   */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 15.10.92                                               */
/*                                                                          */
/*   Written       : MH                                                     */
/*                                                                          */
/*   Dependency    :                                                        */
/*							                                                */
/*	 Revision History :

	15.10.92	MH	Entfernen von PM-Threads
    15.04.93    MH  Portierung auf 32 Bit
							                                                */
/*--------------------------------------------------------------------------*/
#define INCL_DOSPROCESS
#include <pmthread.hpp>
#include <stdlib.h>

//---------------- Methods for class thread ----------------------------------
extern THREAD	*newthread;

//---------------- Methods for class pmthread --------------------------------
void PMThreadStarter (void);

void APIENTRY PMThreadStarter (ULONG b)
	{
	PMTHREAD *found = (PMTHREAD *)b;

	found->hab = WinInitialize (0);
	found->pmfunc ((ULONG)found->pmdata);
	found->running = found->started = FALSE;
	WinTerminate (found->hab);
	found->hab = 0;
	}

//---------------- Methods for class pmwinthread -----------------------------
void PMWINTHREAD::Loop	(void)
	{
	started = running = TRUE;
	error = OK;

	if (!Init (hab, hmq))   			// Initialization fucked up
		{
		error = EUSERINIT;
		return;
		}

    while(TRUE)
	    {
        while(WinGetMsg(hab, (QMSG *) &qmsg, NULL, 0, 0))
    	    {
    	    if (Control (hab, hmq, (QMSG *) &qmsg))
	            WinDispatchMsg(hab, (QMSG *) &qmsg);
        	}

		if (!End (hab, hmq))
			WinCancelShutdown(hmq, FALSE);
		else
	    	break;
		}

	ShutDown (hab, hmq);
	started = running = FALSE;
	return;
	}

unsigned PMTHREAD::Start (THREADFUNC func, void *newdata)
	{
	if (func)
		pmfunc = func;
	if (newdata)
		pmdata = newdata;
	return (THREAD::Start ());
	}
PMTHREAD::PMTHREAD	(THREADFUNC func, BOOL start) : THREAD(this, PMThreadStarter, FALSE)
	{
	pmfunc = func;
	pmdata = NULL;
	hab = 0;
	if (start)
		Start ();
	};
PMTHREAD::PMTHREAD	(void *newdata, THREADFUNC func, BOOL start) : THREAD(this, PMThreadStarter, FALSE)
	{
	pmfunc = func;
	pmdata = newdata;
	hab = 0;
	if (start)
		Start ();
	};
//============================================================================
