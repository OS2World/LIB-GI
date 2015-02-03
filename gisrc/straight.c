/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   : Test-Player der ohne EinflÅsse selber Richtung wÑhlt   */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 11.08.93                                               */
/*                                                                          */
/*   Written       : RF                                                     */
/*                                                                          */
/*	 Revision History :

    11.08.93    RF  Neu
   							                                                */
/*--------------------------------------------------------------------------*/
#define INCL_PM
#include <os2.h>
#include <string.h>
#include <gi.h>
#include <checkbox.hpp>
#include <slider.hpp>

typedef struct
    {
    BOOL    init;
    USHORT  threshold;

    // Instance Data
    char    dir[6];
    } STRAIGHTCONFIG, *STRAIGHTCONFIGP;

/*--------------------------------------------------------------------------*/
/*---- Globals for Config-Part ----*/
HMODULE     module;
HAB         hab;
HWND        wid[7], but_ok, but_cancel;
SLIDER      slider;
STRAIGHTCONFIGP  gjc;

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
    return (sizeof (STRAIGHTCONFIG));
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  InitializeDLL (char __far16 *data)
    {
    STRAIGHTCONFIGP  sc;

    sc = (STRAIGHTCONFIGP)data;

    if (!sc->init)           // Create New Config
        {
        sc->init     = TRUE;
        sc->threshold = THRESHOLD;
        sc->dir[0] = sc->dir[1] = sc->dir[2] =
        sc->dir[3] = sc->dir[4] = sc->dir[5] = 0;
        }
    return (NOERR);
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  Name (char __far16 *target)
    {
    strcpy (target, "Straight Autoplayer");
    return (0);
    }
}
/*--------------------------------------------------------------------------*/
MRESULT	EXPENTRY dlgproc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    int         i;

    switch (msg)
        {
        case WM_INITDLG:
            hab=WinInitialize (0);
            for (i=0; i<7; i++)
                wid[i] = WinWindowFromID (hwnd, i+100);
            but_ok      = WinWindowFromID (hwnd, DID_OK);
            but_cancel  = WinWindowFromID (hwnd, DID_CANCEL);
            slider.Set (wid[6]);
            for (i=0; i<11; i++)
				slider.SetTickSize (i, 6);
            slider.SetScaleText (0, "0%");
            slider.SetScaleText (5, "50%");
            slider.SetScaleText (10, "100%");
			slider.SetSliderPos (gjc->threshold/10);

            for (i=0; i<6; i++)
                CheckBoxSet (wid[i], gjc->dir[i]);
            return (FALSE);
        case WM_COMMAND:
            WinDismissDlg (hwnd, SHORT1FROMMP (mp1) == DID_OK ? TRUE : FALSE);
            break;
        case WM_CONTROL:
            i = SHORT1FROMMP (mp1);
            if (i == 106 && SHORT2FROMMP (mp1) == SLN_CHANGE)
                gjc->threshold = 10*slider.QuerySliderPos();
            else if (i >= 100 && i <= 105)
                gjc->dir[i-100] = CheckBoxQuery (wid[i-100]);
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
    STRAIGHTCONFIG   jcold;

    gjc = (STRAIGHTCONFIGP)dd;
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
    STRAIGHTCONFIGP  jc;

    jc = (STRAIGHTCONFIGP)dd;
    return (jc->threshold);
    }
}
/*--------------------------------------------------------------------------*/
extern "C" {
unsigned _export  Direction (char __far16 *dd, char __far16 *de)
    {
    STRAIGHTCONFIGP      jc;
    PLAYERINFOP     pi;

    jc = (STRAIGHTCONFIGP)dd;
    pi = (PLAYERINFOP)de;

    pi->dir[0] = jc->dir[0] ? jc->threshold : 0;
    pi->dir[1] = jc->dir[1] ? jc->threshold : 0;
    pi->dir[2] = jc->dir[2] ? jc->threshold : 0;
    pi->dir[3] = jc->dir[3] ? jc->threshold : 0;
    pi->dir[4] = jc->dir[4];
    pi->dir[5] = jc->dir[5];
    return (0);
    }
}
/*--------------------------------------------------------------------------*/

