/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Module        : Game-Interface                                         */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 18.04.93                                               */
/*                                                                          */
/*   Written       : RF                                                     */
/*                                                                          */
/*	 Revision History :

	20.05.93    RF  Konzept geÑndert
	26.05.93    RF  Direction und Buttons getrennt
	06.07.93    RF  Diagonal eingebaut
	02.11.93	RF	Threshold wird erst geholt, wenn er gebraucht wird
	07.11.93	RF	Mode() wird gar nicht gebraucht !
	08.11.93	RF	div. énderungen am Interface
																			*/
/*--------------------------------------------------------------------------*/
#define INCL_DOSMODULEMGR
#define INCL_PM
#include <os2.h>
#pragma hdrstop

#define GI_DEFINE
#include <gi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloc.h>

static char     *header = "Game-Interface Config-File V1.00\32";
static char		filename[256];

/*--------------------------------------------------------------------------*/
unsigned _export InitFip (FUNCINFOP fi)		// internal use only
    {
    int     i;

    if (DosLoadModule (NULL, 0L, fi->file, &fi->hand) || !fi->hand)
        return (error = EMODULE);

	if (DosQueryProcAddr (fi->hand, 0L, funcname[0], (PFN *)&fi->GetDataSize))
		return (EINITFUNC);

	for (i=0; i<4; i++)
		{
		if (DosQueryProcAddr (fi->hand, 0L, funcname[i+1], (PFN *)&fi->func[i]))
            return (EINITFUNC);
        }
	if (DosQueryProcAddr (fi->hand, 0L, funcname[5], (PFN *)&fi->Direction))
        return (EINITFUNC);

	if (!fi->data)  // noch keine Daten alloziert
        {
        fi->datasize = fi->GetDataSize ();
		fi->data = malloc (fi->datasize);
		if (!fi->data)
			return (error = ENOMEM);
		memset (fi->data, 0, fi->datasize);
		}
	error = fi->func[0] ((char __far16 *)fi->data);    // Init
	if (error)
		return (error);

	fi->func[1](&fi->name[2]);                              // Get Name of Strategy

	return (error = NOERR);
	}
/*--------------------------------------------------------------------------*/
void _export DelFip (unsigned i)		// internal use only
	{
	for (; i<numplayer; i++)
		fip[i] = fip[i+1];
	numplayer--;
	}
/*--------------------------------------------------------------------------*/
unsigned _export GIInitialize ()
	{
	int         i;
	FILE        *f;
	char        head[50], *fn;

	error = NOERR;
	strcpy (filename, GI_CONFIGNAME);
	f = fopen (filename, "rb");			// Zuerst im Cur-Dir suchen
	if (!f)
		{
		fn = getenv (GI_ENVNAME);	// und dann std-environment
		if (fn)
			strcpy (filename, fn);

		if (!fn || (f=fopen (filename, "rb"))==NULL)
			{
			numplayer=0;
			return (error = ENOTFOUND);
			}
		}

	// Test header
	fread (head, strlen(header), 1, f);
	if (memcmp (head, header, strlen(header)))
		{
		fclose (f);
		return (error = EBADFILETYPE);
		}

	fread (&numplayer, sizeof (unsigned), 1, f);
	for (i=0; i<numplayer; i++)
		{
		fip[i] = (FUNCINFOP)malloc (sizeof (FUNCINFO));
		if (!fip[i])
			return (error = ENOMEM);

		fread (fip[i]->file, sizeof (char), 100, f);
		fread (&fip[i]->datasize, sizeof (unsigned), 1, f);
		fip[i]->data = malloc (fip[i]->datasize);
        if (!fip[i]->data)
            return (error = ENOMEM);

        fread (fip[i]->data, fip[i]->datasize, 1, f);

        if (InitFip (fip[i]))
            return (error);
        }

    fclose (f);

    return (error);
    }
/*--------------------------------------------------------------------------*/
unsigned _export Save ()	// internal use only ! (giconfig.exe)
	{
	FILE    *f;
	int     i;

	f = fopen (filename, "wb");
	if (!f)
        return (error = EOPEN);

	fwrite (header, strlen(header), 1, f);
    fwrite (&numplayer, sizeof (unsigned), 1, f);
    for (i=0; i<numplayer; i++)
        {
        fwrite (fip[i]->file, sizeof (char), 100, f);
        fwrite (&fip[i]->datasize, sizeof (unsigned), 1, f);
		fwrite (fip[i]->data, fip[i]->datasize, 1, f);
        }
    fclose (f);

    return (error = NOERR);
	}
/*--------------------------------------------------------------------------*/
unsigned _export GIGetDirection (PLAYERINFOP pi)
	{
	char    m, mm;
	int     i, mi=-1, mii=-1, th;

	if (!pi || pi->player > numplayer)
		return (error = EBADARG);

	error = fip[pi->player]->Direction ((char __far16 *)fip[pi->player]->data,
										(char __far16 *)pi);
	if (error)
		return (error);

	th = fip[pi->player]->func[3] ((char __far16 *)fip[pi->player]->data);

	if (pi->analog)
		m = mm = 0;
	else
		m = mm = th;
	for (i=0; i<4; i++)
        {
        if (m <= pi->dir[i])
			{
            mm = m;             // max wird zu backup
            m = pi->dir[i];     // neues max setzen

            mii= mi;            // letzer Index wird zum 2. letzten Index
			mi = i;             // neuer Index des max
            }
        else if (mm <= pi->dir[i])  // kein neues max, aber vielleicht neus backup?
            {
			mm = pi->dir[i];    // backup setzen
            mii= i;             // index fÅr backup
			}
		}
	pi->direction = mi;
	pi->diagonal = mii;

	pi->buttons = 0;
	if (pi->dir[4])
		pi->buttons = DIRBUT1;
	if (pi->dir[5])
		pi->buttons |= DIRBUT2;

	return (0);
	}
/*--------------------------------------------------------------------------*/
unsigned _export GIGetThreshold (unsigned fiidx)	// Nur fÅr interne Zwecke !!
	{
	if (fiidx <= numplayer)
		return (fip[fiidx]->func[3] ((char __far16 *)fip[fiidx]->data));
	return (0);
	}
/*--------------------------------------------------------------------------*/


