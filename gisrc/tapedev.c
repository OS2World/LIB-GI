/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   : Tape-Interface mit Game-Support & Self-Config          */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          :  3.1.93                                                */
/*                                                                          */
/*   Written       : RF                                                     */
/*                                                                          */
/*	 Revision History :

		04.11.93	RF	Record und Play implementiert
		07.11.93	RF	event-time statt letztem Zustand
		18.11.93	RF	StopOnOverPlay eingebaut, BugFix bei decode von diagonal
																			*/
/*--------------------------------------------------------------------------*/
#define INCL_PM
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <string.h>
#include <gi.h>
#include <spin.hpp>

#pragma hdrstop

#include <checkbox.hpp>

typedef struct
	{
	BOOL    init;

	// Instance Data
	long	port;			// Port to monitor
	} TAPECONFIG, *TAPECONFIGP;


/*--------------------------------------------------------------------------*/
/*---- Globals ----*/
long				port;
HFILE   			file;
unsigned _export	modeflag=0;		// 0=default 1=rec 2=play
unsigned			mymode=0;		// Intern um festzustellen, wenn modus „ndert
unsigned _export    stoponover=0;
long _export		event=0;
long   				myevent=0;
char   _export		filename[256] = { 0x00 };
BOOL   _export		cfgpresent=FALSE;
HWND   _export		globhwnd;
long   _export		count=0;		// Tape-Counter

/*---- Globals for Config-Part ----*/
HMODULE     module;
HAB         hab;
SPINBUTTON	sp;

/*---- Prototypings ----*/
unsigned	FileState(unsigned);
unsigned	Record 	(PLAYERINFOP);
unsigned	Play 	(PLAYERINFOP);

/*---- Extern ----*/
extern "C" {
unsigned GIGetThreshold (unsigned fiidx);	// Nur fr interne Zwecke !!
}

/*--------------------------------------------------------------------------*/
extern "C" {
ULONG _dllmain (ULONG termflag, HMODULE modhandle)
	{
	if (!termflag)  // Init...
		module = modhandle;

	return (TRUE);
	}
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export GetDataSize (void)
	{
	return (sizeof (TAPECONFIG));    // Oh, what a hack!
	}
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  InitializeDLL (char __far16 *data)
	{
	ULONG       	action;
	TAPECONFIGP		tc;

	tc = (TAPECONFIGP)data;

	if (!tc->init)           // Create New Config
		{
		tc->init = TRUE;
		tc->port = -1;		// Port muss einmal definiert werden...
		}

	port = tc->port;

	return (NOERR);
	}
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  Name (char __far16 *target)
	{
	strcpy (target, "Tape Recorder");
	return (0);
	}
}
/*--------------------------------------------------------------------------*/
extern "C" {
MRESULT	EXPENTRY dlgproc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
	{
	int         i;
	SWP         swp;

	switch (msg)
		{
		case WM_INITDLG:
			hab=WinInitialize (0);
			sp.Set (WinWindowFromID (hwnd, 101));
			sp.SetLimits (100, 1);
			sp.SetValue (port+1);
			return (FALSE);
		case WM_COMMAND:
			switch (SHORT1FROMMP (mp1))
				{
				case DID_OK:
					sp.QueryValue (&port);
					port--;						// Zero-Based !
					if (GIGetThreshold ((unsigned)port)==0)
						WinMessageBox (HWND_DESKTOP, hwnd, "Illegal Port.",
							"Tape: Fatal Error", 0, MB_OK | MB_ICONEXCLAMATION |
							MB_APPLMODAL);
					else
						WinDismissDlg (hwnd, TRUE);
					break;
				case DID_CANCEL:
					WinDismissDlg (hwnd, FALSE);
					break;
				}
			break;
		default:
			return (WinDefDlgProc (hwnd, msg, mp1, mp2));
		}
    return (0L);
	}
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  ConfigDLL (char __far16 *dd)
	{
	TAPECONFIGP	tc;

	tc = (TAPECONFIGP)dd;

	WinDlgBox (HWND_DESKTOP, HWND_DESKTOP, dlgproc, module, 100, NULL);

	tc->port = port;

	return (0);
	}
}
/*--------------------------------------------------------------------------*/
#pragma argsused
extern "C" {
unsigned _export  Threshold (char __far16 *dd)
	{
	return (GIGetThreshold ((unsigned)port));
	}
}
/*--------------------------------------------------------------------------*/
#pragma argsused
extern "C" {
unsigned _export  Direction (char __far16 *dd, char __far16 *de)
	{
	TAPECONFIGP		tc;
	PLAYERINFOP     pi;
	unsigned		ret;
	ULONG			action;
	unsigned		player;

	pi = (PLAYERINFOP)de;

	// Hat inzwischen der File-Mode ge„ndert ?
	if (myevent != event)
		{
		myevent = event;
		if (mymode)			// das file war offen
			DosClose (file);
		if (modeflag==1)
			{
			if (count)	// write append
				{
				DosOpen (filename, &file, &action, 0, 0,
					OPEN_ACTION_OPEN_IF_EXISTS,
					OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_WRITEONLY,
					0);
				DosSetFilePtr (file, 0, 2, &action);
				}
			else	// write new
				DosOpen (filename, &file, &action, 0, 0,
					OPEN_ACTION_REPLACE_IF_EXISTS | OPEN_ACTION_CREATE_IF_NEW,
					OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_WRITEONLY,
					0);
			}
		else if (modeflag==2)	// read
			DosOpen (filename, &file, &action, 0, 0,
				OPEN_ACTION_OPEN_IF_EXISTS,
				OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_READONLY,
				0);
		if (!file)
			{
			mymode = modeflag = 0;		// Fehler -> Stop
			if (cfgpresent)
				WinPostMsg (globhwnd, WM_USER, 1, 0);
			}
		else
			mymode = modeflag;
		}

	if (modeflag == 2)		// Play
		{
		if (stoponover)
			{
			// Richtung vom Original-Device holen
			player = pi->player;
			pi->player = port;
			GIGetDirection (pi);
			pi->player = player;
			if (pi->direction == DIRNONE)
				ret = Play (pi);
			else
				{
				ret = 0;
				WinPostMsg (globhwnd, WM_USER, 1, 0);
				}
			}
		else
			ret = Play (pi);
		if (ret && cfgpresent)
			WinPostMsg (globhwnd, WM_USER, 1, 0);
		}

	if (modeflag != 2 || ret)
		{
		// Richtung vom Original-Device holen
		player = pi->player;
		pi->player = port;
		GIGetDirection (pi);
		pi->player = player;

		if (modeflag == 1)	// Record
			{
			ret = Record (pi);
			if (ret && cfgpresent)
				WinPostMsg (globhwnd, WM_USER, 1, 0);
			}
		}

	return (0);
	}
}
/*--------------------------------------------------------------------------*/
unsigned Record (PLAYERINFOP pl)
	{
	char	v1, v2, d;
	ULONG	ret;

	// Richtung & Buttons aufbereiten
	d = (pl->direction+1)+5*(pl->diagonal+1)+25*(pl->buttons);

	// Analog-Daten ?
	if (pl->analog)
		{
		if (pl->direction != DIRNONE)
			{
			v1 = - pl->dir[pl->direction];
			if (DosWrite (file, &v1, sizeof (char), &ret) || ret != sizeof (char))
				return (1);
			}
		if (pl->diagonal != DIRNONE)
			{
			v2 = - pl->dir[pl->diagonal];
			if (DosWrite (file, &v2, sizeof (char), &ret) || ret != sizeof (char))
				return (1);
			}
		}

	// Richtung & Buttons
	if (DosWrite (file, &d, sizeof (char), &ret) || ret != sizeof (char))
		return (1);

	count++;
	if (cfgpresent)
		WinPostMsg (globhwnd, WM_USER, 0, 0);

	return (0);
	}
/*--------------------------------------------------------------------------*/
unsigned Play (PLAYERINFOP pl)
	{
	char	v1=0, v2=0, d;
	ULONG	ret;

	// Daten lesen
	if (DosRead (file, &d, sizeof (char), &ret) || ret != sizeof (char))
		return (1);

	if (d < 0)		// Analog-Daten
		{
		v1 = -d;
		if (DosRead (file, &d, sizeof (char), &ret) || ret != sizeof (char))
			return (1);
		if (d < 0)
			{
			v2 = -d;
			if (DosRead (file, &d, sizeof (char), &ret) || ret != sizeof (char)
					 ||d < 0)
				return (1);
			}
		}

	// Daten aufbereiten
	pl->direction = (d%5)-1;
	pl->diagonal  = ((d/5)%5)-1;
	pl->buttons   = (d/25);

	pl->dir[0] = pl->dir[1] = pl->dir[2] = pl->dir[3] = 0;
	pl->dir[4] = pl->buttons & 0x01;
	pl->dir[5] = pl->buttons & 0x02;

	/*------------------------------------------------------*/
	/* Damit bei der Umrechnung der dir[] in direction und  */
	/* diagonal in GI direction st„rker ist als diagonal,   */
	/* muss threshold+1 fr direction gegeben werden!		*/
	if (pl->direction != DIRNONE)
		pl->dir[pl->direction] = pl->analog ? v1 : GIGetThreshold ((unsigned)port)+1;

	if (pl->diagonal & 0x04)
		pl->diagonal = DIRNONE;
	else
		pl->dir[pl->diagonal]  = pl->analog ? v2 : GIGetThreshold ((unsigned)port);

	count++;
	if (cfgpresent)
		WinPostMsg (globhwnd, WM_USER, 0, 0);

	return (0);
	}
/*--------------------------------------------------------------------------*/

