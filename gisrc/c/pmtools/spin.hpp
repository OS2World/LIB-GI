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

							                                                */
/*--------------------------------------------------------------------------*/

class SPINBUTTON
    {
	private:
		HWND	win;

	public:
		SPINBUTTON (HWND w=0) : win(w) {};
		void Set (HWND w) {win = w;};

        unsigned    SetLimits   (LONG upper, LONG lower) { return WinSendMsg (win, SPBM_SETLIMITS, upper, lower); };
        unsigned    QueryLimits (LONG *upper, LONG *lower) { return WinSendMsg (win, SPBM_QUERYLIMITS, (ULONG)upper, (ULONG)lower); };
        unsigned    SetValue    (LONG value) { return WinSendMsg (win, SPBM_SETCURRENTVALUE, value, 0); };
        unsigned    QueryValue  (LONG *res) { return WinSendMsg (win, SPBM_QUERYVALUE, (LONG)res, 0); };
    };