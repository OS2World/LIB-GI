/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   :                                                        */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 10.05.93                                               */
/*                                                                          */
/*   Written       : RF                                                     */
/*                                                                          */
/*	 Revision History :

    10.05.93    RF  PreCompiled-Headers, os2.h wird vom Modul geholt
																			*/
/*--------------------------------------------------------------------------*/

class SLIDER
	{
	private:
		HWND	win;

	public:
		SLIDER (HWND w=0) : win(w) {};
		void Set (HWND w) {win = w;};

        BOOL    SetTickSize     (USHORT ticknum, USHORT size)   { return (WinSendMsg (win, SLM_SETTICKSIZE, MPFROM2SHORT (ticknum, size), 0)); };
        BOOL    SetScaleText    (USHORT ticknum, PSZ text)      { return (WinSendMsg (win, SLM_SETSCALETEXT, MPFROMSHORT (ticknum), MPFROMP (text))); };
        BOOL    SetSliderPos    (USHORT ticknum)                { return (WinSendMsg (win, SLM_SETSLIDERINFO, MPFROM2SHORT (SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE), ticknum)); };

        USHORT  QuerySliderPos  (void)                          { return (SHORT1FROMMP (WinSendMsg (win, SLM_QUERYSLIDERINFO, MPFROM2SHORT (SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE), 0))); };
	};
