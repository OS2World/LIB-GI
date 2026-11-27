/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Service-Klasse: Spin-Button                                            */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 23.08.93                                               */
/*                                                                          */
/*   Written       : RF                                                     */
/*                                                                          */
/*	 Revision History :
/*									                                                */
/*--------------------------------------------------------------------------*/
#ifndef OW_GI_SPIN_HPP
#define OW_GI_SPIN_HPP

class SPINBUTTON
    {
        private:
                HWND	win;

        public:
                SPINBUTTON (HWND w=0) : win(w) {};
                void Set (HWND w) {win = w;};

        unsigned    SetLimits   (LONG upper, LONG lower) { return ((unsigned)LONGFROMMR (WinSendMsg (win, SPBM_SETLIMITS, MPFROMLONG (upper), MPFROMLONG (lower)))); };
        unsigned    QueryLimits (LONG *upper, LONG *lower) { return ((unsigned)LONGFROMMR (WinSendMsg (win, SPBM_QUERYLIMITS, MPFROMP (upper), MPFROMP (lower)))); };
        unsigned    SetValue    (LONG value) { return ((unsigned)LONGFROMMR (WinSendMsg (win, SPBM_SETCURRENTVALUE, MPFROMLONG (value), MPVOID))); };
        unsigned    QueryValue  (LONG *res) { return ((unsigned)LONGFROMMR (WinSendMsg (win, SPBM_QUERYVALUE, MPFROMP (res), MPVOID))); };
    };

#endif
