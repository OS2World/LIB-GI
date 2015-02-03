/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   : CheckBox                                               */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 24.04.92                                               */
/*                                                                          */
/*   Written       : RF                                                     */
/*                                                                          */
/*	 Revision History :

    10.05.93    RF  PreCompiled-Headers, os2.h wird vom Modul geholt
																			*/
/*--------------------------------------------------------------------------*/

inline unsigned    CheckBoxQuery   (HWND hwnd)
    {
    return (WinSendMsg (hwnd, BM_QUERYCHECK, 0L, 0L));
    }

inline unsigned    CheckBoxSet     (HWND hwnd, ULONG v)
    {
    return (WinSendMsg (hwnd, BM_SETCHECK, v, 0L));
    }


