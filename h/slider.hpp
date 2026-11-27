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
#ifndef OW_GI_SLIDER_HPP
#define OW_GI_SLIDER_HPP

class SLIDER
        {
        private:
                HWND	win;

        public:
                SLIDER (HWND w=0) : win(w) {};
                void Set (HWND w) {win = w;};

        BOOL    SetTickSize     (USHORT ticknum, USHORT size)   { return ((BOOL)SHORT1FROMMR (WinSendMsg (win, SLM_SETTICKSIZE, MPFROM2SHORT (ticknum, size), MPVOID))); };
        BOOL    SetScaleText    (USHORT ticknum, PSZ text)      { return ((BOOL)SHORT1FROMMR (WinSendMsg (win, SLM_SETSCALETEXT, MPFROMSHORT (ticknum), MPFROMP (text)))); };
        BOOL    SetSliderPos    (USHORT ticknum)                { return ((BOOL)SHORT1FROMMR (WinSendMsg (win, SLM_SETSLIDERINFO, MPFROM2SHORT (SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE), MPFROMLONG (ticknum)))); };

        USHORT  QuerySliderPos  (void)                          { return ((USHORT)SHORT1FROMMR (WinSendMsg (win, SLM_QUERYSLIDERINFO, MPFROM2SHORT (SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE), MPVOID))); };
        };

#endif
