/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   : Joystick-Interface mit Game-Support & Self-Config      */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 10.05.93                                               */
/*                                                                          */
/*   Written       : RF                                                     */
/*                                                                          */
/*	 Revision History :

    15.05.93    RF  Init-Konzept neu
	29.10.93	RF	kbd.h wird nicht mehr importiert
																			*/
/*--------------------------------------------------------------------------*/
#define INCL_DOSDEVICES
#define INCL_PM
#include <os2.h>
#pragma hdrstop

#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include <string.h>
#include <gi.h>
#include <checkbox.hpp>
#include <slider.hpp>

typedef struct
    {
	SHORT   ax;		        // A Joystick X position
	SHORT   ay;     		// A Joystick Y position
	SHORT   bx;		        // B Joystick X position
	SHORT   by;		        // B Joystick Y position
	USHORT  a1c;            // Button A1 Press Count
	USHORT  a2c;            // Button A2 Press Count
	USHORT  b1c;        	// Button B1 Press Count
	USHORT  b2c;            // Button B2 Press Count
	UCHAR ucJs_JoyStickMask;	// Mask of Connected Joystick Pots
	UCHAR   button;	        // Bits of Switches Down
	LONG   lJs_Ticks;		// Total Clock Ticks (60 Hz)
    } JOYSTATUS, *JOYSTATUSP;

typedef struct
    {
    BOOL    init;
    BOOL    abflag;
    BOOL    buttonswap;
    USHORT  threshold;
    USHORT  scale[4];

    // Instance Data
    HFILE   jhand;
    SHORT   x, y, b1, b2, xo, yo;
    char    dir[6];
    } JOYCONFIG, *JOYCONFIGP;

#define MAXSTATUS   50
#define DEFNAME     "joy.cfg"
#define JOYCLASS    "JoyConfig"
#define ABST        3
#define BOXSIZE     4

/*--------------------------------------------------------------------------*/
/*---- Globals for Config-Part ----*/
HMODULE     module;
HAB         hab;
char        statustext[3][MAXSTATUS];
HWND        wid[18], but_ok, but_cancel;
SLIDER      slider;
ULONG       mx, my, ctitle, cborder;
USHORT      tmpscale[4];
RECTL       rcl = {0,0,0,0 };            // Update of Joy-Rectl
unsigned    refresh=0;
int         mode;
BOOL        wait;
JOYCONFIGP  gjc;

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
extern "C"  {
unsigned _export GetDataSize (void)
    {
    return (sizeof (JOYCONFIG));    // Oh, what a hack!
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  InitializeDLL (char __far16 *data)
    {
    ULONG       action;
    JOYCONFIGP  jc;

    jc = (JOYCONFIGP)data;

    if (!jc->init)           // Create New Config
        {
        jc->init     = TRUE;
        jc->abflag   = FALSE;      // A-Joy
        jc->buttonswap= FALSE;
        jc->threshold = THRESHOLD;
        jc->scale[2] = jc->scale[1] = 1300;   // My Joy-Values...
        jc->scale[0] = jc->scale[3] = 0;
        }

    // Open Joy-Driver
    DosOpen ("GAME$", &jc->jhand, &action, 0L, 0L, 0x11L,
                OPEN_FLAGS_NO_CACHE | OPEN_FLAGS_NO_LOCALITY | OPEN_SHARE_DENYNONE,
                NULL);

    return (NOERR);
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  Name (char __far16 *target)
    {
    strcpy (target, "Joystick");
    return (0);
    }
}
/*--------------------------------------------------------------------------*/
void GetJoy (JOYCONFIGP jc)
    {
    ULONG       actsize = sizeof (JOYSTATUS);
    BOOL        h;
    JOYSTATUS   js;

    DosDevIOCtl(jc->jhand, 0x80, 0x20, NULL, 0, NULL, &js, actsize, &actsize);

    jc->xo = (jc->abflag ? js.bx : js.ax) - jc->scale[3];
    jc->yo = (jc->abflag ? js.by : js.ay) - jc->scale[0];
    jc->b1 = jc->abflag ? (js.button & 64)>>6   : (js.button & 16)>>4;
    jc->b2 = jc->abflag ? (js.button & 0x80)>>7 : (js.button & 32)>>5;

    // Skalierung
    jc->x = 200*jc->xo / (jc->scale[1]-jc->scale[3]);    // Das Intervall wird auf 0..200 skaliert
    jc->y = 200*jc->yo / (jc->scale[2]-jc->scale[0]);

    // Richtungen extrahieren
    if (jc->x >= 100)
        {
        jc->dir[1] = jc->x - 100;
        jc->dir[3] = 0;
        }
    else
        {
        jc->dir[1] = 0;
        jc->dir[3] = 100 - jc->x;
        }
    if (jc->y >= 100)      // Y arbeitet invers !!!
        {
        jc->dir[2] = jc->y - 100;
        jc->dir[0] = 0;
        }
    else
        {
        jc->dir[2] = 0;
        jc->dir[0] = 100 - jc->y;
        }

    // Button-Swap
    if (jc->buttonswap)
        {
        h      = jc->b1;
        jc->b1 = jc->b2;
        jc->b2 = h;
        }
    jc->dir[4] = jc->b1;
    jc->dir[5] = jc->b2;
    }
/*--------------------------------------------------------------------------*/
MRESULT	EXPENTRY maxproc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    char        txt[10];

    switch (msg)
        {
        case WM_INITDLG:
            WinSetWindowText (WinWindowFromID (hwnd, 201), statustext[0]);
            mode = 0;
            wait = FALSE;
            WinStartTimer (hab, hwnd, 2, 100);
            break;
        case WM_TIMER:
            GetJoy (gjc);
            WinSetWindowText (WinWindowFromID (hwnd, 203), itoa (gjc->x, txt, 10));
            WinSetWindowText (WinWindowFromID (hwnd, 205), itoa (gjc->y, txt, 10));

            if (gjc->b1|gjc->b2 && !wait)
                wait = TRUE;
            else if (gjc->b1|gjc->b2 && wait)
                break;
            else if (!(gjc->b1|gjc->b2) && wait)
                {
                wait = FALSE;
                if (mode==0)
                    {
                    tmpscale[0] = gjc->yo;
                    tmpscale[3] = gjc->xo;
                    WinSetWindowText (WinWindowFromID (hwnd, 201), statustext[1]);
                    mode++;
                    DosBeep (1000, 100);
                    }
                else if (mode==1)
                    {
                    tmpscale[1] = gjc->xo;
                    tmpscale[2] = gjc->yo;
                    WinSetWindowText (WinWindowFromID (hwnd, 201), statustext[2]);
                    mode++;
                    DosBeep (1200, 100);
                    }
                }
            break;
        case WM_COMMAND:
            WinStopTimer (hab, hwnd, 2);
            switch (SHORT1FROMMP (mp1))
                {
                case DID_OK:
                    gjc->scale[0] = tmpscale[0];
                    gjc->scale[1] = tmpscale[1];
                    gjc->scale[2] = tmpscale[2];
                    gjc->scale[3] = tmpscale[3];
                    WinDismissDlg (hwnd, TRUE);
                    break;
                case DID_CANCEL:
                    WinDismissDlg (hwnd, FALSE);
                    break;
                }
            return (0);
        default:
            return (WinDefDlgProc (hwnd, msg, mp1, mp2));
        }
    return (0);
    }
/*--------------------------------------------------------------------------*/
MRESULT	EXPENTRY joyproc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    HPS     hps;
    POINTL  p;
    RECTL   rr;

    switch (msg)
        {
        case WM_CREATE:
            WinStartTimer (hab, hwnd, 1, 100);
            return (FALSE);
        case WM_TIMER:
            WinInvalidateRect (hwnd, NULL, FALSE);
            return (0);
        case WM_ENABLE:
            if (SHORT1FROMMP (mp1))
                WinStartTimer (hab, hwnd, 1, 100);
            else
                WinStopTimer (hab, hwnd, 1);
            return (0);
        case WM_PAINT:
            hps = WinBeginPaint (hwnd, NULL, &rr);
            if (rcl.xLeft == rcl.xRight || refresh==10)
                {
                WinFillRect (hps, &rr, CLR_WHITE);
                refresh=0;
                }
            else
                {
                WinFillRect (hps, &rcl, CLR_WHITE);
                refresh++;
                }

            // Position des Joysticks zeichnen
            GetJoy (gjc);
            rcl.xLeft = gjc->x*2*mx/200;
            rcl.yBottom = 2*my - (gjc->y*2*my/200);
            rcl.xRight = rcl.xLeft + BOXSIZE;
            rcl.yTop = rcl.yBottom + BOXSIZE;
            WinFillRect (hps, &rcl, CLR_BLACK);
            WinEndPaint (hps);

            // Checkboxen aktivieren
            CheckBoxSet (wid[ 7], gjc->dir[0] > gjc->threshold);  // Up
            CheckBoxSet (wid[ 8], gjc->dir[1] > gjc->threshold);  // Right
            CheckBoxSet (wid[ 9], gjc->dir[2] > gjc->threshold);  // Down
            CheckBoxSet (wid[10], gjc->dir[3] > gjc->threshold);  // Left
            CheckBoxSet (wid[12], gjc->dir[4]);                  // Button 1
            CheckBoxSet (wid[13], gjc->dir[5]);                  // Button 2
            return (0);
        default:
            return (WinDefWindowProc (hwnd, msg, mp1, mp2));
        }
    }
/*--------------------------------------------------------------------------*/
MRESULT	EXPENTRY dlgproc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    int         i;
    SWP         swp;

    switch (msg)
        {
        case WM_INITDLG:
            hab=WinInitialize (0);
            for (i=0; i<18; i++)
                wid[i] = WinWindowFromID (hwnd, i+101);
            but_ok      = WinWindowFromID (hwnd, DID_OK);
            but_cancel  = WinWindowFromID (hwnd, DID_CANCEL);
            WinLoadString (hab, module, 200, MAXSTATUS, statustext[0]);
            WinLoadString (hab, module, 201, MAXSTATUS, statustext[1]);
            WinLoadString (hab, module, 202, MAXSTATUS, statustext[2]);

            WinSetWindowText (wid[0], statustext[0]);
            CheckBoxSet (wid[11], gjc->buttonswap);
            slider.Set (wid[4]);
            for (i=0; i<11; i++)
                slider.SetTickSize (i, 6);
            slider.SetScaleText (0, "0%");
            slider.SetScaleText (5, "50%");
            slider.SetScaleText (10, "100%");
            slider.SetSliderPos (gjc->threshold/10);

            ctitle = WinQuerySysValue (HWND_DESKTOP, SV_CYTITLEBAR);
            cborder= WinQuerySysValue (HWND_DESKTOP, SV_CXBORDER);
            WinQueryWindowPos (wid[1], &swp);
            WinRegisterClass (hab, JOYCLASS, joyproc, CS_SAVEBITS | CS_SYNCPAINT, 0);
            mx = (swp.cx-BOXSIZE)/2-ABST;
            my = (swp.cy-BOXSIZE-ctitle)/2-ABST;
            wid[2] = WinCreateWindow (wid[1], JOYCLASS, NULL, WS_VISIBLE, ABST, ABST,
                        mx*2+BOXSIZE, my*2+BOXSIZE, wid[1], HWND_TOP, 103, NULL, 0);
            if (!wid[2])
                return (TRUE);  // Abort Creation

            CheckBoxSet (wid[16], !gjc->abflag);
            CheckBoxSet (wid[17], gjc->abflag);
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
                case 115:   // Def. Max
                    WinEnableWindow (but_ok, FALSE);
                    WinEnableWindow (but_cancel, FALSE);
                    for (i=1; i<14; i++)
                        WinEnableWindow (wid[i], FALSE);
                    WinDlgBox (hwnd, hwnd, maxproc, module, 200, NULL);
                    for (i=1; i<14; i++)
                        WinEnableWindow (wid[i], TRUE);
                    WinEnableWindow (but_cancel, TRUE);
                    WinEnableWindow (but_ok, TRUE);
                    break;
                }
            break;
        case WM_CONTROL:
            if (SHORT1FROMMP (mp1) == 105 && SHORT2FROMMP (mp1) == SLN_CHANGE)
                gjc->threshold = 10*slider.QuerySliderPos();
            else if (SHORT1FROMMP (mp1) == 112)
                gjc->buttonswap = CheckBoxQuery (wid[11]);
            else if (SHORT1FROMMP (mp1) == 117 || SHORT1FROMMP (mp1) == 118)
                gjc->abflag = !CheckBoxQuery (wid[16]);
            break;
        default:
            return (WinDefDlgProc (hwnd, msg, mp1, mp2));
        }
    return (0L);
    }
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  ConfigDLL (char __far16 *dd)
    {
    JOYCONFIG   jcold;

    gjc = (JOYCONFIGP)dd;
    jcold = *gjc;

    if (WinDlgBox (HWND_DESKTOP, HWND_DESKTOP, dlgproc, module, 100, NULL) == FALSE)
        *gjc = jcold;

    return (0);
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  Threshold (char __far16 *dd)
    {
    JOYCONFIGP  jc;

    jc = (JOYCONFIGP)dd;
    return (jc->threshold);
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  Direction (char __far16 *dd, char __far16 *de)
    {
    JOYCONFIGP      jc;
    PLAYERINFOP     pi;

    jc = (JOYCONFIGP)dd;
    pi = (PLAYERINFOP)de;

    GetJoy (jc);
    pi->dir[0] = jc->dir[0];
    pi->dir[1] = jc->dir[1];
    pi->dir[2] = jc->dir[2];
    pi->dir[3] = jc->dir[3];
    pi->dir[4] = jc->dir[4];
    pi->dir[5] = jc->dir[5];
    return (0);
    }
}
/*--------------------------------------------------------------------------*/

