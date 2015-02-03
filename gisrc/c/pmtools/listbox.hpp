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

class LISTBOX
	{
	private:
		HWND	win;

	public:
		LISTBOX (HWND w=0) : win(w) {};
		void Set (HWND w) {win = w;};

		void		DeleteAll () {WinSendMsg (win, LM_DELETEALL, 0, 0);};
		USHORT		Delete (SHORT item) {return ((USHORT)WinSendMsg (win, LM_DELETEITEM, item, 0));};
		USHORT		Insert (SHORT index, PSZ text) {return ((USHORT)WinSendMsg (win, LM_INSERTITEM, index, (ULONG)text));};
		USHORT		InsertAsc (SHORT index, PSZ text) {return ((USHORT)WinSendMsg (win, LM_INSERTITEM, LIT_SORTASCENDING, (ULONG)text));};
		USHORT		InsertDesc (SHORT index, PSZ text) {return ((USHORT)WinSendMsg (win, LM_INSERTITEM, LIT_SORTDESCENDING, (ULONG)text));};
		USHORT		InsertEnd (SHORT index, PSZ text) {return ((USHORT)WinSendMsg (win, LM_INSERTITEM, LIT_END, (ULONG)text));};
		USHORT		Count () {return ((USHORT)WinSendMsg (win, LM_QUERYITEMCOUNT, 0, 0));};
		SHORT		Selection () {return ((SHORT)WinSendMsg (win, LM_QUERYSELECTION, LIT_FIRST, 0));};
		void		SetSelection (SHORT index) {WinSendMsg (win, LM_SELECTITEM, MPFROMSHORT (index), TRUE);};
		SHORT		GetText (SHORT index, PSZ buffer, SHORT maxlen=255) {return ((SHORT)WinSendMsg (win, LM_QUERYITEMTEXT, MPFROM2SHORT (index, maxlen), (ULONG)buffer));};
	};
