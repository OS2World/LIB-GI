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
							                                                */
/*--------------------------------------------------------------------------*/
#ifndef PMTHREAD_INC
#define PMTHREAD_INC

#define INCL_PM
#include <thread.hpp>
#include <os2.h>
#include <std.h>

//----------------------------------------------------------------------------
class PMTHREAD : public THREAD
	{
	private:
		friend void APIENTRY PMThreadStarter (ULONG);

		void APIENTRY (*pmfunc) (ULONG);
		void *pmdata;
	protected:
		HAB 	hab;

	public:

		virtual ~PMTHREAD	()
			{
			if (hab)
				{
				WinTerminate (hab);
				hab = 0;
				}
			}

		PMTHREAD	() : THREAD() {hab = WinInitialize (0);};
		PMTHREAD	(THREADFUNC func, BOOL start=FALSE);
		PMTHREAD	(void *newdata, THREADFUNC func, BOOL start=FALSE);

		unsigned Start (THREADFUNC func = NULL, void *newdata = NULL);

		HAB Hab () {return hab;};
	};
//----------------------------------------------------------------------------
class PMWINTHREAD : public PMTHREAD
	{
	protected:
		virtual BOOL Init		(HAB, HMQ)  {return TRUE;};
		virtual void ShutDown	(HAB, HMQ)	{};
		virtual BOOL Control	(HAB, HMQ, QMSG *) {return TRUE;};
		virtual BOOL End		(HAB, HMQ) {return TRUE;};

		HMQ			hmq;
		QMSG		qmsg;

    public:

		HMQ Hmq () {return hmq;};
		void Loop (void);
        ~PMWINTHREAD	()
        	{
			if (hmq)
				{
			    WinDestroyMsgQueue(hmq);
				hmq = 0;
				}
        	}

		PMWINTHREAD	(BOOL start = FALSE) : PMTHREAD()
						{
					    hmq = WinCreateMsgQueue(hab, 0);
					    if (start)
							Loop ();
						};
		BOOL HasActiveWin () {return ((BOOL)WinIsThreadActive (hab));};
	};
//----------------------------------------------------------------------------
#endif

