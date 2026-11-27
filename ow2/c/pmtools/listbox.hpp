/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   :                                                        */
/*                                                                          */
/*   Version       : V0.00                                                  */
/*                                                                          */
/*   Date          : 22.04.92                                               */
/*                                                                          */
/*   Written       : MH                                                     */
/*                                                                          */
/*	 Revision History :

     10.05.93    RF  PreCompiled-Headers, os2.h wird vom Modul geholt
                                                                                                                                                        */
/*--------------------------------------------------------------------------*/
#ifndef OW_GI_LISTBOX_HPP
#define OW_GI_LISTBOX_HPP

class LISTBOX
        {
        private:
                HWND	win;

        public:
                LISTBOX (HWND w=0) : win(w) {};
                void Set (HWND w) {win = w;};

                void		DeleteAll () {WinSendMsg (win, LM_DELETEALL, MPVOID, MPVOID);};
                USHORT		Delete (SHORT item) {return ((USHORT)SHORT1FROMMR (WinSendMsg (win, LM_DELETEITEM, MPFROMSHORT (item), MPVOID)));};
                USHORT		Insert (SHORT index, PSZ text) {return ((USHORT)SHORT1FROMMR (WinSendMsg (win, LM_INSERTITEM, MPFROMSHORT (index), MPFROMP (text))));};
                USHORT		InsertAsc (SHORT index, PSZ text) {return ((USHORT)SHORT1FROMMR (WinSendMsg (win, LM_INSERTITEM, MPFROMSHORT (LIT_SORTASCENDING), MPFROMP (text))));};
                USHORT		InsertDesc (SHORT index, PSZ text) {return ((USHORT)SHORT1FROMMR (WinSendMsg (win, LM_INSERTITEM, MPFROMSHORT (LIT_SORTDESCENDING), MPFROMP (text))));};
                USHORT		InsertEnd (SHORT index, PSZ text) {return ((USHORT)SHORT1FROMMR (WinSendMsg (win, LM_INSERTITEM, MPFROMSHORT (LIT_END), MPFROMP (text))));};
                USHORT		Count () {return ((USHORT)SHORT1FROMMR (WinSendMsg (win, LM_QUERYITEMCOUNT, MPVOID, MPVOID)));};
                SHORT		Selection () {return ((SHORT)SHORT1FROMMR (WinSendMsg (win, LM_QUERYSELECTION, MPFROMSHORT (LIT_FIRST), MPVOID)));};
                void		SetSelection (SHORT index) {WinSendMsg (win, LM_SELECTITEM, MPFROMSHORT (index), MPFROMLONG (TRUE));};
                SHORT		GetText (SHORT index, PSZ buffer, SHORT maxlen=255) {return ((SHORT)SHORT1FROMMR (WinSendMsg (win, LM_QUERYITEMTEXT, MPFROM2SHORT (index, maxlen), MPFROMP (buffer))));};
        };

#endif
