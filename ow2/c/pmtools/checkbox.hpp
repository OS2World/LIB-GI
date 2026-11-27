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
#ifndef OW_GI_CHECKBOX_HPP
#define OW_GI_CHECKBOX_HPP

inline unsigned    CheckBoxQuery   (HWND hwnd)
    {
    return ((unsigned)SHORT1FROMMR (WinSendMsg (hwnd, BM_QUERYCHECK, MPVOID, MPVOID)));
    }

inline unsigned    CheckBoxSet     (HWND hwnd, ULONG v)
    {
    return ((unsigned)SHORT1FROMMR (WinSendMsg (hwnd, BM_SETCHECK, MPFROMLONG (v), MPVOID)));
    }

#endif
