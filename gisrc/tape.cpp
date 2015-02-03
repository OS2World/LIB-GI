/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   : Configurationsmodul zu Tape.c				            */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 31.10.93                                               */
/*                                                                          */
/*   Written       : RF / SC                                                */
/*                                                                          */
/*	 Revision History :

																			*/
/*--------------------------------------------------------------------------*/
#define INCL_PM
#include <OS2.H>
#include <DIR.H>
#include <STDLIB.H>
#include <STRING.H>
#include <PMTHREAD.HPP>
#include <CHECKBOX.HPP>

/*---- Globals ----*/
PMWINTHREAD msgloop;
FILEDLG     filedlg;
HWND		wid[8], title;
char		*text[] = { "Tape-Control                   Status: Stop",
						"Tape-Control                   Status: Record",
						"Tape-Control                   Status: Play" };

/*---- Extern ----*/
extern USHORT	modeflag;
extern long		event;
extern char		filename[];
extern long		count;
extern HWND		globhwnd;
extern BOOL		cfgpresent;
extern unsigned stoponover;

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void Active (HWND hwnd, unsigned mode)
	{
	char	tmp[10];

	WinEnableControl (hwnd, 101, mode);
	WinEnableControl (hwnd, 102, mode);
	WinEnableControl (hwnd, 105, mode);
	WinEnableControl (hwnd, 106, mode);
	WinEnableControl (hwnd, 107, mode);

	WinSetWindowText (title, text[modeflag]);

	itoa (count, tmp, 10);
	WinSetWindowText (wid[3], tmp);
	}
/*--------------------------------------------------------------------------*/
unsigned GetName (HWND hwnd)
	{
	HWND        hwnddlg;
	unsigned    error;
	char		drive[MAXDRIVE], dir[MAXDIR];

	filedlg.cbSize = sizeof (FILEDLG);
	filedlg.fl = FDS_CENTER | FDS_OPEN_DIALOG;
	filedlg.pszTitle = "Choose a file or enter a new name";
	if (filename[0])
		{
		fnsplit (filename, drive, dir, 0, 0);
		fnmerge (filename, drive, dir, "*", ".TAP");
		strcpy (filedlg.szFullFile, filename);
		}
	else
		strcpy (filedlg.szFullFile, "*.TAP");

	hwnddlg = WinFileDlg (HWND_DESKTOP, hwnd, &filedlg);
	if (hwnddlg && filedlg.lReturn == DID_OK)
		{
		strcpy (filename, filedlg.szFullFile);
		WinSetWindowText (wid[6], filename);
		error = NOERR;
		}
	else
		error = ECANCEL;

	return (error);
	}
/*--------------------------------------------------------------------------*/
MRESULT	EXPENTRY dlgproc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
	{
	int			i;
	char		tmp[10];

	switch (msg)
		{
		case WM_INITDLG:
			for (i=0; i<8; i++)
				wid[i] = WinWindowFromID (hwnd, 101+i);
			title = WinWindowFromID (hwnd, FID_TITLEBAR);
			globhwnd = hwnd;
			cfgpresent = TRUE;
			WinSendMsg (hwnd, WM_USER, 0, 0);
			WinCheckButton (hwnd, 115, stoponover);
			Active (hwnd, 1);
			return (FALSE);
		case WM_COMMAND:
		case WM_CONTROL:
			switch (SHORT1FROMMP (mp1))
				{
				case 101: 	// Play
					count = 0;
					modeflag = 2;
					event++;
					Active (hwnd, 0);
					break;
				case 102:	// Record
					//count = 0;
					modeflag = 1;
					event++;
					Active (hwnd, 0);
					break;
				case 103:	// Stop
					modeflag = 0;
					Active (hwnd, 1);
					break;
				case 105:	// Reset Counter
					CheckBoxSet (wid[4], 0);
					WinSetWindowText (wid[3], "0");
					count = 0;
					event++;
					break;
				case 106:	// File...
					GetName (hwnd);
					break;
				case 107:
					WinQueryWindowText (wid[6], 256, filename);
					break;
				case 108:	// End
					cfgpresent = FALSE;
					WinDismissDlg (hwnd, FALSE);
					break;
				case 115:	// Stop on Overplay
					stoponover = WinQueryButtonCheckstate (hwnd, 115);
					break;
				}
			break;
		case WM_USER:
			switch (mp1)
				{
				case 0:		// update counter
					itoa (count, tmp, 10);
					WinSetWindowText (wid[3], tmp);
					break;
				case 1:		// error in TAPE.DLL -> STOP
					modeflag = 0;
					Active (hwnd, 1);
					break;
				}
			break;
		default:
			return (WinDefDlgProc (hwnd, msg, mp1, mp2));
		}
	return (0L);
	}
/*--------------------------------------------------------------------------*/
void main(void)
	{
	WinDlgBox (HWND_DESKTOP, HWND_DESKTOP, dlgproc, 0, 100, NULL);
	}
/*--------------------------------------------------------------------------*/

