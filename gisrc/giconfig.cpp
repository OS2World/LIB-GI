/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Module        : Game-Interface Configuration                           */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 18.04.93                                               */
/*                                                                          */
/*   Written       : RF                                                     */
/*                                                                          */
/*	 Revision History :

	13.06.93    RF  Wenn InitFip Fehler gibt, wird Msg-Box angezeigt
	20.06.93    RF  Insert vor das Selektierte Item, falls eines selektiert,
					sonst am Ende.
	05.11.93	RF	FileDlg etwas intelligenter...
																			*/
/*--------------------------------------------------------------------------*/
#define INCL_PM
#include <os2.h>
#pragma hdrstop

#include <dir.h>
#include <mem.h>
#include <string.h>
#include <gi.h>
#include <listbox.hpp>
#include <pmthread.hpp>
#include <stdlib.h>

PMWINTHREAD msgloop;
LISTBOX     lb;
FILEDLG     filedlg;
BOOL        ignore=FALSE;
BOOL        inconfig=FALSE;

extern "C"	{
extern unsigned     InitFip     (FUNCINFOP);
extern void         DelFip      (unsigned);
extern unsigned		Save		();
}

/*--------------------------------------------------------------------------*/
int GetName ()
	{
	HWND        hwnddlg;
	FUNCINFOP   fi;
	char		drive[MAXDRIVE], dir[MAXDIR];

	fnsplit (filedlg.szFullFile, drive, dir, 0, 0);
	fnmerge (filedlg.szFullFile, drive, dir, "*", ".DLL");

retry:
	hwnddlg = WinFileDlg (HWND_DESKTOP, HWND_DESKTOP, &filedlg);
	if (hwnddlg && filedlg.lReturn == DID_OK)
		{
		fi = fip[numplayer] = (FUNCINFOP)malloc (sizeof (FUNCINFO));
		if (!fi)
			{
			error = ENOMEM;
			return (-1);
			}
		strcpy (fi->file, filedlg.szFullFile);
		fi->data = NULL;
		if (InitFip (fi))
			{
			WinMessageBox (HWND_DESKTOP, HWND_DESKTOP, ErrorShow (error), NULL, 0,
							MB_OK | MB_ICONHAND | MB_APPLMODAL);
			goto retry;
			}
		return (numplayer++);
		}
	else
		return (-1);
	}
/*--------------------------------------------------------------------------*/
void Reorder (int sel)
    {
	int         i;

    for (i=numplayer; i>sel; i--)
        fip[i] = fip[i-1];
    fip[sel] = fip[numplayer];

    lb.DeleteAll ();
    for (i=0; i<numplayer; i++)
        {
        fip[i]->name[0] = '1'+i;
        fip[i]->name[1] = ' ';
        lb.InsertEnd (0, fip[i]->name);
        }
    }
/*--------------------------------------------------------------------------*/
MRESULT	EXPENTRY dlgproc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    int         i;

    switch (msg)
        {
        case WM_INITDLG:
            inconfig=FALSE;
            lb.Set (WinWindowFromID (hwnd, 102));
            for (i=0; i<numplayer; i++)
                    {
                    fip[i]->name[0] = '1'+i;
					fip[i]->name[1] = ' ';
                    lb.InsertEnd (0, fip[i]->name);
                    }
            return (FALSE);
        case WM_COMMAND:
            if (inconfig)
                break;
            inconfig=TRUE;
            switch (SHORT1FROMMP (mp1))
                {
                case DID_OK:
                    WinDismissDlg (hwnd, TRUE);
                    break;
                case DID_CANCEL:
                    WinDismissDlg (hwnd, FALSE);
                    break;
                case 103:   // New
                    i = GetName ();
                    if (i >= 0)
                        {
                        fip[i]->name[0] = '1'+i;
                        fip[i]->name[1] = ' ';
                        if (lb.Selection () == LIT_NONE)
                            lb.InsertEnd (0, fip[i]->name);
                        else
                            Reorder (lb.Selection ());
                        }
					break;
				case 104:   // Define
					i = lb.Selection ();
					if (i != LIT_NONE)
						fip[i]->func[2] ((char __far16 *)fip[i]->data);    // Config
                    break;
                case 105:   // Delete
                    i = lb.Selection ();
                    if (i == LIT_NONE)
                        break;
                    DelFip (i);
                    lb.DeleteAll ();
                    for (i=0; i<numplayer; i++)
                        {
                        fip[i]->name[0] = '1'+i;
                        fip[i]->name[1] = ' ';
                        lb.InsertEnd (0, fip[i]->name);
                        }
                    break;                }
            inconfig=FALSE;
            break;
		case WM_CONTROL:
            if (SHORT1FROMMP (mp1)==102 && SHORT2FROMMP (mp1)==LN_ENTER)
                {
                if (inconfig)
                    break;
				inconfig=TRUE;
                i = lb.Selection ();
                if (i != LIT_NONE)
                    fip[i]->func[2] ((char __far16 *)fip[i]->data);    // Config
                ignore=TRUE;
                inconfig=FALSE;
                }
            break;
        case WM_CHAR:
            if (ignore)
                {
                ignore=FALSE;
                return (TRUE);
                }
            if (SHORT2FROMMP (mp2) == VK_ESC)
                {
                lb.SetSelection (LIT_NONE);
                return (TRUE);
                }
            break;
        default:
			return (WinDefDlgProc (hwnd, msg, mp1, mp2));
        }
    return (0L);
    }
/*--------------------------------------------------------------------------*/
int main ()
	{
	GIInitialize ();

	// File-Dlg vorbereiten
	filedlg.cbSize = sizeof (FILEDLG);
	filedlg.fl = FDS_CENTER | FDS_OPEN_DIALOG;
	filedlg.pszTitle = "Name of Interface Driver";
	strcpy (filedlg.szFullFile, "*.DLL");

	if (WinDlgBox (HWND_DESKTOP, HWND_DESKTOP, dlgproc, 0, 100, NULL))
		Save ();

	return (NOERR);
	}
