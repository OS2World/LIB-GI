/*-------------------------------------------------------------------------*/
/*                                                                         */
/*   ERROR.CPP  : Allgemeine Fehler-Basis-Klasse                           */
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
    15.04.93    MH  Portierung auf 32 Bit
*/
#define INCL_DOSMISC
#define INCL_DOSPROCESS
#include <os2.h>
#include <error.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE		200
/*-------- Globals ---------------------------------------------------------*/
const char  *set	= "ERRORFILE";		// Name der Environmentvariablen
char		unknown[20]	= "Error: ";
const  char	white[]	= { 32, 9, 10, 11, 12, 13, 0 };
char		line[MAXLINE];

/*--------------------------------------------------------------------------*/
char *ErrorInit ()
    {
    PPIB ppib;
    PTIB ptib;
    char		*env;

    DosGetInfoBlocks (&ptib, &ppib);
    env = ppib->pib_pchenv;
	while (*env)
		{
		strupr (env);
		if (strncmp (env, set, strlen(set)) == 0)
			return (strchr (env, '=')+1);

		env += strlen(env)+1;
		}
    return (NULL);
    }
/*--------------------------------------------------------------------------*/
unsigned GetNum (char *line)
    {
    int     num=0;
    char    *p;
    char    bakline[MAXLINE];

    strcpy (bakline, line);

    /*---- Skip leading spaces ----*/
    strtok (bakline, white);

    /*---- Process word ----*/
    do
        {
        p = strtok (NULL, white);
        }
    while (p && (num = atoi (p)) == 0);

    return (num);
    }
/*--------------------------------------------------------------------------*/
char *GetText (char *line)
	{
	char	*s1, *s2;
	
	s1 = strchr (line, 0x22);
	if (s1)
		{
		s1++;
		s2 = strchr (s1, 0x22);
		if (s2)
			{
			*s2=0x00;
			return (s1);
			}
		}
	return (line);
    }
/*--------------------------------------------------------------------------*/
char *ScanFile (char *filename, unsigned no)
    {
    FILE    *f;

    if ((f = fopen (filename, "r")) == NULL)
        return (NULL);

	

    fgets (line, MAXLINE, f);

    while (!feof (f) && GetNum (line) != no)
        fgets (line, MAXLINE, f);

    if (feof (f))
        return (NULL);

    fclose (f);
	return (GetText (line));
    }
/*--------------------------------------------------------------------------*/
char *ErrorShow (unsigned num)
	{
	char	*nl, *nr, *out;
	
	nl = ErrorInit ();
	if (!nl)
		return (itoa(num, &unknown[7], 10));

	nr = strchr (nl, ';');
	while (nr)
		{
		*nr=0x00;
		out = ScanFile (nl, num);
		if (out)
			return (out);
			
		nl = nr+1;
		nr = strchr (nl, ';');
		}
	out = ScanFile (nl, num);
	if (out)
		return (out);
		
	return (itoa(num, &unknown[7], 10));
	}
/*--------------------------------------------------------------------------*/

