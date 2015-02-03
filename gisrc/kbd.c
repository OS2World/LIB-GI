/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   : Keyboard-Interface mit Game-Support & Self-Config      */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 23.04.93                                               */
/*                                                                          */
/*   Written       : RF                                                     */
/*                                                                          */
/*	 Revision History :

	28.04.93    RF  ConfigKey fein-finish
	10.05.93    RF  PreCompiled-Headers, os2.h wird vom Modul geholt
	12.05.93    RF  Threshold wird richtig berechnet...
	14.05.93    RF  exportierte Funktionen haben C-Calling conventions
	15.05.93    RF  Init-Konzept Åberarbeitet
	13.07.93 RF+MH  Monitor works OK !!
	11.08.93 RF+MH  Konvertier-Tabelle Mon-Scan->PM-Scan implementiert
	12.08.93    RF  Tasten werden vom KBD-Mon weitergegeben. Appl. muss sie
					abfackeln!
	29.10.93	RF	kbd.h wird nicht mehr importiert
																			*/
/*--------------------------------------------------------------------------*/
#define INCL_WINHOOKS
#define INCL_DOSPROCESS
#define INCL_PM
#include <os2.h>
#pragma hdrstop

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <mem.h>
#include <string.h>
#include <gi.h>
#include <checkbox.hpp>
#include <listbox.hpp>
#include <slider.hpp>
#include <kbdconv.h>

#define MAXSTATUS   50
#define STDNUMDIR   6           // Up, Right, Down, Left, B1, B2
#define DEFNAME     "kbd.cfg"
#define MAXSCANCODE 110

#define ORD_DOSMONOPEN      4
#define ORD_DOSMONCLOSE     3
#define ORD_DOSMONREG       5
#define ORD_DOSMONREAD      2
#define ORD_DOSMONWRITE     1
#define DosMonOpen      Dos16MonOpen
#define DosMonClose     Dos16MonClose
#define DosMonReg       Dos16MonReg
#define DosMonRead      Dos16MonRead
#define DosMonWrite     Dos16MonWrite
#define DosGetInfoSeg   Dos16GetInfoSeg

/*--------------------------------------------------------------------------*/
extern "C" {
APIRET16 APIENTRY16 DosMonOpen  (PCHAR16 name, PUSHORT16 phand);
APIRET16 APIENTRY16 DosMonClose (USHORT hand);
APIRET16 APIENTRY16 DosMonReg   (USHORT hand, PUCHAR16 in, PUCHAR16 out, USHORT pos, USHORT group);
APIRET16 APIENTRY16 DosMonRead  (PUCHAR16 in, USHORT wait, PUCHAR16 data, PUSHORT16 cnt);
APIRET16 APIENTRY16 DosMonWrite (PUCHAR16 out, PUCHAR16 data, USHORT cnt);
APIRET16 APIENTRY16 DosGetInfoSeg (PUSHORT16 iseg, PUSHORT16 lseg);
}
/*--------------------------------------------------------------------------*/
//typedef struct _CHARMSG KEYSTRUCT;

struct KP {
        USHORT          flags;
        char            ascii;
        unsigned char   scan;
        char            foo[8];
        USHORT          ddflags;
        };

typedef struct
        {
        BOOL            init;
        unsigned        numdir;
        unsigned        threshold;
        unsigned        scan[STDNUMDIR], flags[STDNUMDIR];
        } KBDCONFIG, *KBDCONFIGP;
/*--------------------------------------------------------------------------*/
HMODULE     module;
BOOL        inconfig=FALSE;
char        statustext[3][MAXSTATUS];
char        keytext[MAXSTATUS];
char        scankey[15], nonetext[15];
HWND        wid[8], but_ok, but_cancel;
SLIDER      slider;
LISTBOX     lb;
char        *dirname[] = { "", "  Up", "  Right", "  Down", "  Left", "  Button 1",
                                "  Button 2" };
unsigned    mode;
BOOL        k[MAXSCANCODE];     // Aktuell gedrÅckte Tasten
unsigned    flags=0;    // Aktuell gedrÅckte Sonder-Tasten
unsigned    cscan, cflags;
TID         monid;
KBDCONFIGP  kc;

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
USHORT getcurgroup (void)  // The Mega-Hack of '93 !  (still AM)
    {
    USHORT      iseg, lseg;
    PUCHAR16    gdt;

    DosGetInfoSeg (&iseg, &lseg);
    gdt = (PUCHAR16)((long)iseg<<16);
    return (gdt[24]);
    }
/*--------------------------------------------------------------------------*/
#pragma argsused
void APIENTRY kbd_monitor (ULONG arg)
    {
    UCHAR           ib[128], ob[128];        // buffer
    USHORT          kplen;
    struct KP       kp;
    BOOL            release=FALSE;
    USHORT          monhan;
    USHORT          ff, tscan;

    // init variables & open monitor
    ib[0] = ob[0]    = 128;
    ib[1] = ob[1]    = 0;
    DosSetPriority (PRTYS_THREAD, PRTYC_TIMECRITICAL, 0, monid);

    if (DosMonOpen ("KBD$", &monhan))
        DosBeep (1000,1000);

    if (DosMonReg (monhan, ib, ob, 1, getcurgroup ()))
        DosBeep (2000,1000);

    while (TRUE)
        {
        // read key    and copy to backup
        kplen = sizeof (struct KP);
        DosMonRead  (ib, 0, (PUCHAR16)&kp, &kplen);
        DosMonWrite (ob, (PUCHAR16)&kp, kplen);

        // extract information
        release = (kp.ddflags & 0x0040)>>6;

        if (kp.scan == 0)       // flags
            {
            kp.flags = (kp.flags & 0x7f00) >> 8;
            ff = 0;
            if (kp.flags == 0x2a || kp.flags == 0x36)
                ff = KC_SHIFT;
            else if (kp.flags == 0x1d)
                ff = KC_CTRL;
            else if (kp.flags == 0x38)
                ff = KC_ALT;

            flags ^= ff;
            }
        tscan = montopm[kp.scan];
        k[tscan] = !release;
        }
    }
/*--------------------------------------------------------------------------*/
extern "C" {
#pragma argsused
BOOL cdecl hook (HAB hab, PQMSG pqmsg, USHORT msgflag)
    {
    char    s;

    if (pqmsg->msg != WM_CHAR || inconfig)
        return (FALSE);

    s = CHAR4FROMMP (pqmsg->mp1);
    if (s>=MAXSCANCODE)
        DosBeep (2000,1000);
    else
        {
        k[s] = !(SHORT1FROMMP (pqmsg->mp1) & KC_KEYUP);
        flags = SHORT1FROMMP (pqmsg->mp1) & (KC_CTRL | KC_SHIFT | KC_ALT);
        }
    return (FALSE);
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export GetDataSize (void)
    {
    return (sizeof(KBDCONFIG));
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export InitializeDLL (char __far16 *dd)
    {
    PTIB        tib;
    PPIB        pib;
    KBDCONFIGP  kc;

    kc = (KBDCONFIGP)dd;
    if (!kc->init)
        {
        kc->init = TRUE;
        kc->numdir = STDNUMDIR;
        kc->threshold = THRESHOLD;
        }

    // Get run-mode
    DosGetInfoBlocks (&tib, &pib);
    if (pib->pib_ultype == 3)   // Install a PM-Hook
        {
       	if (!WinSetHook (WinInitialize (0), HMQ_CURRENT, HK_INPUT, (PFN)hook, module))
            {
            return (EHOOK);
            }
        }
    else                    // Install KBD-Mon
        {
        DosCreateThread (&monid, kbd_monitor, 0, 0, 4096);
        }

    return (NOERR);
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export Name (char __far16 *target)
    {
    strcpy (target, "Keyboard");
    return (0);
    }
}
/*--------------------------------------------------------------------------*/
void SetState ()
    {
    char    num[10];

    WinEnableWindow (wid[0], mode==0);
    WinEnableWindow (wid[1], mode!=0);
    WinEnableWindow (wid[2], mode==0);

    WinEnableWindow (wid[5], mode==1);
    WinEnableWindow (wid[6], mode==1);
    WinEnableWindow (wid[7], mode==1);

    WinEnableWindow (but_ok, mode==0);
    WinEnableWindow (but_cancel, mode==0);

    WinSetWindowText (wid[3], statustext[mode]);

    if (cscan)
        {
        strcpy (keytext, scankey);
        itoa (cscan, num, 10);
        strcat (keytext, num);
        }
    else
        strcpy (keytext, nonetext);

    WinSetWindowText (wid[4], keytext);

    CheckBoxSet (wid[5], cflags & KC_SHIFT ? TRUE : FALSE);
    CheckBoxSet (wid[6], cflags & KC_ALT   ? TRUE : FALSE);
    CheckBoxSet (wid[7], cflags & KC_CTRL  ? TRUE : FALSE);
    }
/*--------------------------------------------------------------------------*/
MRESULT	EXPENTRY dlgproc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    HAB         hab=WinInitialize (0);
    int         i;
    unsigned    tscan, tflags;

    switch (msg)
        {
        case WM_INITDLG:
            for (i=0; i<8; i++)
                wid[i] = WinWindowFromID (hwnd, i+101);
            but_ok      = WinWindowFromID (hwnd, DID_OK);
            but_cancel  = WinWindowFromID (hwnd, DID_CANCEL);
            WinLoadString (hab, module, 100, MAXSTATUS, statustext[0]);
            WinLoadString (hab, module, 101, MAXSTATUS, statustext[1]);
            WinLoadString (hab, module, 102, MAXSTATUS, statustext[2]);
            WinLoadString (hab, module, 103, 15, scankey);
            WinLoadString (hab, module, 104, 15, nonetext);
            mode = 0;
            SetState ();
            return (FALSE);
        case WM_COMMAND:
            switch (SHORT1FROMMP (mp1))
                {
                case DID_OK:
                    WinDismissDlg (hwnd, TRUE);
                    break;
                case DID_CANCEL:
                    WinDismissDlg (hwnd, FALSE);
                    break;
                case 101:   // Start...
                    mode = 1;
                    SetState ();
                    break;
                case 102:   // Stop...
                    cflags = 0;
                    if (CheckBoxQuery (wid[5]))
                        cflags |= KC_SHIFT;
                    if (CheckBoxQuery (wid[6]))
                        cflags |= KC_ALT;
                    if (CheckBoxQuery (wid[7]))
                        cflags |= KC_CTRL;
                    mode = 0;
                    SetState ();
                    break;
                case 103:   // Test...
                    mode = 2;
                    SetState ();
                    break;
                }
            break;
        case WM_CHAR:
            if (mode == 1)
                {
                cscan  = CHAR4FROMMP (mp1);
                cflags = SHORT1FROMMP (mp1) & (KC_ALT | KC_CTRL | KC_SHIFT);
                SetState ();
                break;
                }
            else if (mode == 2)
                {
                tscan  = CHAR4FROMMP (mp1);
                tflags = SHORT1FROMMP (mp1) & (KC_ALT | KC_CTRL | KC_SHIFT);
                if (tscan == cscan && tflags == cflags)
                    DosBeep (1000,100);
                }
        default:
            return (WinDefDlgProc (hwnd, msg, mp1, mp2));
        }
    return (0L);
    }
/*--------------------------------------------------------------------------*/
unsigned _export ConfigKey (unsigned *scanp, unsigned *flagsp)
    {
    cscan  = *scanp;
    cflags = *flagsp;

    if (WinDlgBox (HWND_DESKTOP, HWND_DESKTOP, dlgproc, module, 100, NULL) == TRUE)
        {
        *scanp = cscan;
        *flagsp= cflags;
        }
    return (0);
    }
/*--------------------------------------------------------------------------*/
void DrawListBox ()
    {
    int     i;

    lb.DeleteAll ();
    for (i=1; i<=kc->numdir; i++)
        {
        if (kc->scan[i-1] || kc->flags[i-1])
            dirname[i][0] = '*';
        else
            dirname[i][0] = ' ';
        lb.Insert (LIT_END, dirname[i]);
        }
    }
/*--------------------------------------------------------------------------*/
MRESULT	EXPENTRY configproc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    USHORT      i;
    int         sel;

    switch (msg)
        {
        case WM_INITDLG:
            lb.Set (WinWindowFromID (hwnd, 201));
            DrawListBox ();
            slider.Set (WinWindowFromID (hwnd, 206));
            for (i=0; i<11; i++)
                slider.SetTickSize (i, 6);
            slider.SetScaleText (0, "0%");
            slider.SetScaleText (5, "50%");
            slider.SetScaleText (10, "100%");
            slider.SetSliderPos (kc->threshold/10);
            break;
        case WM_COMMAND:
            switch (SHORT1FROMMP (mp1))
                {
                case DID_OK:
                    kc->threshold = 10*slider.QuerySliderPos ();
                    WinDismissDlg (hwnd, TRUE);
                    break;
                case DID_CANCEL:
                    WinDismissDlg (hwnd, FALSE);
                    break;
                case 202:   // New...
                    sel = lb.Selection ();
                    if (sel >= 0)    // Item selected
                        {
                        kc->scan[sel] = kc->flags[sel] = 0;
                        ConfigKey (&kc->scan[sel], &kc->flags[sel]);
                        DrawListBox ();
                        }
                    break;
                case 203:   // Define...
                    sel = lb.Selection ();
                     if (sel >= 0)    // Item selected
                        {
                        ConfigKey (&kc->scan[sel], &kc->flags[sel]);
                        DrawListBox ();
                        }
                      break;
                case 204:   // Delete...
                    sel = lb.Selection ();
                    if (sel >= 0)    // Item selected
                        {
                        kc->scan[sel] = kc->flags[sel] = 0;
                        DrawListBox ();
                        }
                    break;
                }
            break;
        case WM_CONTROL:
            if (SHORT1FROMMP (mp1)==201 && SHORT2FROMMP (mp1)==LN_ENTER)
                {
                sel = lb.Selection ();
                ConfigKey (&kc->scan[sel], &kc->flags[sel]);
                DrawListBox ();
                }
            break;
        default:
            return (WinDefDlgProc (hwnd, msg, mp1, mp2));
        }
    return (0);
    }

/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export ConfigDLL (char __far16 *dd)
    {
    KBDCONFIG   back;

    memcpy (&back, dd, sizeof (KBDCONFIG));
    kc = (KBDCONFIGP)dd;
    inconfig = TRUE;

    if (WinDlgBox (HWND_DESKTOP, HWND_DESKTOP, configproc, module, 200, NULL) == FALSE)
        {
        memcpy (dd, &back, sizeof(KBDCONFIG));
        }

    inconfig = FALSE;
    return (NOERR);
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export Threshold (char __far16 *dd)
    {
    return (((KBDCONFIGP)dd)->threshold);
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export Direction (char __far16 *dd, char __far16 *de)
    {
    PLAYERINFOP pi;
    KBDCONFIGP  kc;
    int         i;

    kc = (KBDCONFIGP)dd;
    pi = (PLAYERINFOP)de;
    for (i=0; i<STDNUMDIR; i++)
        {
        if (k[kc->scan[i]] && kc->flags[i] == flags)
            pi->dir[i] = kc->threshold;
        else
            pi->dir[i] = 0;
        }
    return (0);
    }
}
/*--------------------------------------------------------------------------*/


