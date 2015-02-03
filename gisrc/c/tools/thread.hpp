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
	19.10.92	RF	Funcs sind pascal
	27.02.93	RF	DEF_STACKSIZE = 12k
    15.04.93    MH  Portierung auf 32 Bit
							                                                */
/*--------------------------------------------------------------------------*/
#ifndef THREAD_INC
#define THREAD_INC

#define INCL_PM
#include <os2.h>
#pragma hdrstop
#define INCL_ERRTHREAD
#include <errdef.h>
#include <error.hpp>

#define DEF_STACKSIZE (30*1024)		// default stacksize

typedef void (APIENTRY *THREADFUNC) (ULONG);

class THREAD : public ERROR
	{
    public:

		THREAD  ();
        THREAD  (THREADFUNC func, BOOL start=FALSE);
        THREAD  (void *data, THREADFUNC func, BOOL start=FALSE);

		unsigned Start (THREADFUNC func=NULL, void *data=NULL);

		void Suspend	();
		void Resume		();
		void StackSize	(unsigned size)
			{
			stacksize = size;
			}
		BOOL 	Running () {return running;};
		BOOL 	Started () {return started;};
		TID		Tid		() {return tid;};
		PID		Pid		() {return pid;};

	protected:
		THREADFUNC		threadfunc;

		BOOL			running;
		BOOL			started;
		void			*data;

	private:

		TID				tid;
		PID				pid;
		unsigned		stacksize;
	};
//----------------------------------------------------------------------------
#endif

