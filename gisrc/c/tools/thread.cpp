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
	18.10.92	RF	erbt ERROR
	27.02.93	RF	Statt DosCreateThread wird _dosbeginthread gebraucht
    15.04.93    MH  Portierung auf 32 Bit

							                                                */
/*--------------------------------------------------------------------------*/
#define INCL_DOSPROCESS
#include <thread.hpp>
#include <stdlib.h>
#include <process.h>


//---------------- Methods for class thread ----------------------------------

THREAD::THREAD	()
	{
    PTIB    ptib;
    PPIB    ppib;

    DosGetInfoBlocks (&ptib, &ppib);
    tid = ptib->tib_ordinal;
    pid = ppib->pib_ulpid;

	threadfunc		= NULL;
	started			= TRUE;
	running			= TRUE;
	stacksize		= DEF_STACKSIZE;
	error			= OK;
	}

THREAD::THREAD	(THREADFUNC func, BOOL start)
	{
    PTIB    ptib;
    PPIB    ppib;

    DosGetInfoBlocks (&ptib, &ppib);
    pid = ppib->pib_ulpid;

	threadfunc		= func;
	started			= start;
	running			= FALSE;
	stacksize		= DEF_STACKSIZE;
	error			= OK;
	data			= NULL;
	tid				= 0;

	if (start)
		Start (func);
	}

THREAD::THREAD	(void *newdata, THREADFUNC func, BOOL start)
	{
    PTIB    ptib;
    PPIB    ppib;

    DosGetInfoBlocks (&ptib, &ppib);
    pid = ppib->pib_ulpid;

	threadfunc		= func;
	started			= FALSE;
	running			= FALSE;
	stacksize		= DEF_STACKSIZE;
	error			= OK;
	data			= newdata;
	tid				= 0;

	if (start)
		Start (func, data);
	}

void THREAD::Suspend ()
	{
	if (!started || !running)
		return;

	DosSuspendThread (tid);
	running = FALSE;
	}

void THREAD::Resume ()
	{
	if (running)
		return;

	DosResumeThread (tid);
	running = TRUE;
	}

unsigned THREAD::Start (THREADFUNC func, void *newdata)
	{
	if (started)
		return (error = OK);

	if (func)
		threadfunc = func;
	if (!threadfunc)
		return (error = ENOFUNC);

	if (newdata)
		data = newdata;

    if (DosCreateThread (&tid, threadfunc, (ULONG)data, 0, stacksize))
		return (error = ESTARTTHREAD);

	started = TRUE;
	running = TRUE;

	return (error = OK);
	}
//============================================================================
