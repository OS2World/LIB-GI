/*-------------------------------------------------------------------------*/
/*                                                                         */
/*   ERROR.HPP  : Allgemeine Fehler-Basis-Klasse                           */
/*                                                                         */
/*   Version    : V2.00                                                    */
/*                                                                         */
/*   Date       : 15.10.91                                                 */
/*                                                                         */
/*   Copyright  : RF for Animal                                            */
/*                                                                         */
/*-------------------------------------------------------------------------*/
/*
    History:

    16.08.91    RF  Neu-Design
    30.09.91	RF	Falls in " eingekleidet, wird nur dies ausgegeben
	15.10.92	RF	Als Klasse	
	20.10.92	RF	ErrorShow gibt Pointer auf globale Variable
*/
#ifndef ERRORINCL
#define ERRORINCL

/*--------------------------------------------------------------------------*/
class ERROR
	{
   	public:
		unsigned error;
	
	protected:
		ERROR () {error = 0;};
		
	friend char *ErrorShow (unsigned num);
	};
/*--------------------------------------------------------------------------*/
#else

	extern char *ErrorShow (unsigned num);

#endif	/* not ERRORINCL */