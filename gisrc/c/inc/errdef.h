/*-------------------------------------------------------------------------*/
/*                                                                         */
/*   ERRDEF.H   : Defines fÅr Fehlermeldungen. GÅltig fÅr alle ANIMAL-     */
/*                Programme.                                               */
/*                                                                         */
/*   Version    : V1.10                                                    */
/*                                                                         */
/*   Date       : 09.07.91                                                 */
/*                                                                         */
/*   Copyright  : RF & MH for Animal                                       */
/*                                                                         */
/*-------------------------------------------------------------------------*/
/*
    History:

	30.09.91    RF  Fehlermeldungen von COM eingebaut, es gibt nur noch
					ein ERRDEF.H !
	23.07.92	RF  neue Defines fÅr COM
	31.07.92	RF  Konzept mit defines pro Abschnitt (analog OS/2)
	16.09.92	RF	ECHANNELUSED neu fÅr COM
	18.04.93    RF  neue Defines fÅr GI
	05.05.93    RF  neue Defines fÅr Hooks und KBDMon fÅr GI
	15.05.93    RF  EBADARG ist neu allgemein fÅr alle (war BTREE)
	08.10.93	RF	EINDEX allgemein, neu
	05.12.93	RF	neue Defines fÅr VIEW.CPP
*/
/*-------------------------------------------------------------------------*/
/* Sections:

	INCL_ERRBTREE		: Fehler fÅr B-Tree
	INCL_ERRMAN			: Fehler fÅr Manager (Compiler, Editor)
	INCL_ERRDATATREE	: Fehler fÅr Globalen Datenbaum
	INCL_ERRCOM			: Fehlermeldungen fÅr Netzwerk
	INCL_ERRTHREAD		: Fehler fÅr CPP-Klasse THREAD
	INCL_ERRSTDWIN		: Fehler fÅr CPP-Klasse STDWIN
	INCL_ERRNAMELIST	: Fehler fÅr CPP-Klasse NAMELIST
	INCL_ERRGI          : Fehler fÅr Game-Interface
	INCL_ERRVIEW
 */



/*----------- Prototypings fÅr Service-Funktionen ---------------------------*/
extern int      error_init      (void);
extern char    *error_txt       (int);

/*-------------------------------------------------------------------------*/

#define NOERR                   0
#define OK                      0
#define ALLOK                   0


/*----------- Allgemeine Warnings -------------------------------------------*/
#define ECANCEL					1 /* "Warnung: Funktion abgebrochen." */

/*----------- Allgemeine Errors ---------------------------------------------*/
#define ENOMEM       81         /* Speicher voll */
#define ENOTFOUND    82         /* File, Variable, ... nicht gefunden */
#define USERBREAK    83         /* Unterbrechung durch den Benutzer */
#define ESEEK        84			/* seek nicht erfolgreich					*/
#define EREAD        85			/* read nicht erfolgreich					*/
#define EWRITE       86			/* write (rec_rewrite) nicht erfolgreich	*/
#define EOPEN        87			/* File konnte nicht geîffnet werden  		*/
#define ECLOSE		 88			/* File konnte nicht geschlossen werden		*/
#define EBADFILETYPE 89       	/* Magic zeigt falschen Typ an.          */
#define EBADARG      90         /* Unerlaubtes Argument Åbergeben.       */
#define EINDEX		 91			/* Index fehler 						 */

/*----------- B-TREE Messages -----------------------------------------------*/
#ifdef INCL_ERRBTREE

#define ENOMELT        1       /* konnte/musste Node nicht melten       */
#define ENODEL         2       /* Eintag konnte nicht gelîscht werden   */

/*----------- B-TREE Errors -------------------------------------------------*/
#define EBADRECNO    104       /* Rec_read: Recno zu gross.             */
#define EBADNODENO   107       /* Nodeno zu gross/ nicht gÅltig (-1)    */
#define ENOTYET      108       /* Auf nicht fertiges Feature aufgelaufen*/
#define EROOTGEN     109       /* Root konnte nicht generiert werden.   */
#define EPRGFAILURE  110       /* allg, Prg an unmîglicher Stelle...    */
#define ERECDEL      111       /* Actual-Record ist nicht mehr aktiv.   */
#define EIDXNOTFOUND 112       /* Index-Eintrag wurde nicht gefunden.   */
#define EWRONGTYP    113       /* Ein unbekannter Typ wurde angegeben.  */
#define ELINKNOTFOUND 114      /* zu lîschender Link wurde nicht gefundn*/
#define ENONEXTPREV  115       /* search mit NEXT/PREV ohne FIRST/LAST  */
#define EENDOFFILE   116       /* Versuch mit NEXT Åber Ende zu lesen   */
#define ESTARTOFFILE 117       /* Versuch mit PREV Åber Start zu lesen  */
#define ENODETYP     118       /* Erwartet: Leave Angetroffen: Node     */
#define ENODEEMPTY   120       /* Es wurde ein Node mit 0 EintrÑgen gel.*/
#define EIDXLEN      121       /* IDX-LÑnge nicht akzeptabel.           */
#define EIDXDUPLICATE 122      /* IDX wurde Unique definiert, doppelter SchlÅssel */
#define ENOLRU       123       /* Gesuchter Eintrag in LRU-Tab. fehlt   */
#define ERECHEADCORR 124       /* Header von REC-File korrupt (importcnt) */
#define ELRU         125       /* LRU-Tabelle nicht in Ordnung          */
#define ECURRENCY    126       /* HIGH - Next/Prev nur erlaubt falls Currency gesetzt ist. */
#define ENOFILENAME  127       /* mktemp konnte keinen Namen erzeugen   */
#define EREORG       128       /* Fataler Fehler beim Reorganisieren, Backup benÅtzen */
#define ECNT         129       /* CNT-EintrÑge in REC und IDX sind ungleich */
#define ENOCURRENT   130	   /* REREAD nicht mîglich, kein Current-REC */

#endif /* INCL_ERRBTREE */

/*----------- Manager Errors ------------------------------------------------*/
#ifdef INCL_ERRMAN

/*----------- Interface Tools -----------------------------------------------*/
#define ENOTIMP     150         /* CPY.C: Datentyp noch nicht implementiert */
#define EUNKNOWNINSTRUCTION 200     /* Instruktion unbekannt */
#define EMANGLEDSCRIPT      201     /* Script fehler (report to MH) */
#define ESCENEOVERRUN       202     /* internal error (report to MH) */
#define EEXECFUNCTION       203     /* call by name Fehler */
#define EJUMP               204     /* Fehler bei call, go, go thread */
#define EDATA               205     /* Fehler bei var_init (report to MH) */
#define ENOTSCRIPT          206     /* Kein Scriptfile zum laden */
#define ESCRIPTDATA         207     /* scriptdesc nicht in LDB  gefunden */
#define EFOCUS              208     /* Thread ohne Focus versucht Focus zu geben */

/*----------- Editor Errors und Messages ------------------------------------*/
#define EPIC                300 /* Pic-String nicht in Ordnung              */
#define EBADTYPE            301 /* Pic-Element, Feldtyp nicht erkannt       */
#define ENOTALLOWED         302 /* Msg: LINE-/SCRL-ED kann Zeichen nicht brauchen */
#define EHANDLER            303 /* User-Event-Handler gab Fehlercode zurÅck */
#define EUNKNOWNEVENT       304 /* Event-Typ nicht bekannt                  */
#define ESAMEFIELD          305 /* Msg: LINE-ED Feld wird nicht verlassen   */
#define ENEXTFIELD          306 /* Msg: LINE-ED nÑchstes Feld               */
#define EPREVFIELD          307 /* Msg: LINE-ED vorhergehendes Feld         */
#define EQUITFIELD          308 /* Msg: LINE-ED Feld verlassen (mit ESC)    */
#define ECONTROL            309 /* User-Zeilen-Kontroll-Funktion gab Fehler */
#define EBADDATE            310 /* Datum nicht erlaubt                      */
#define EFIELDFULL          311 /* Vorzeichenwechsel nicht mîglich          */
#define EZERO               312 /* Msg: INT/FLOAT wird Null                 */
#define ESTANDARD           313 /* User-Std-Funkt. gab Fehlercode           */
#define EMASKNOTLOADED      314 /* Daten der Maske nicht im LDB gefunden    */
#define ESHOW               315 /* User-Show-Funkt. gab Fehlercode          */

/*----------- Parser and Scanner Error Messages -----------------------------*/
#define ILLEGAL                 400     // unbekanntes Zeichen  Scanner
#define STRINGNOTTERMINATED     401     // " ohne " am ende     Scanner
#define NUMTOOLARGE             402     // zu grosse nummer     Scanner
#define IDENTTOOLONG            403     // ident zu lang        Scanner
#define FILENOTFOUND            404     // source file not found
#define UNKNOWNBLOCK            405     // wrong word at blockstart
#define NOTEXPECTED             406     // 'Check' failed
#define ILLEGALINSTRUCTION      407     // instruction wrong or not allowed
#define SYNTAX                  408     // syntax-error
#define TOOMANYARGS             409     // zu viele argumente
#define MEMORY                  410     // kein speicher mehr
#define DOUBLESYMBOL            411     // zwei gleiche blîcke mit selben namen
#define ILLEGALNUMBER           412     // reactionlist->illegal number
#define UNKNOWNKEY              413     // Taste in key def. nicht bekannt
#define MISSINGARGS             414     // zu wenig argumente
#define BADARGS                 415     // falsche argumente
#define STARTMISSINGINHEADER    416     // no 'start' in header
#define MISSINGPOS              417     // 'pos' fehlt in windowblock
#define MISSINGDESTTYPE         418     // 'type' or 'dest' fehlt in entry
#define UNKNOWNFIELDTYPE        419     // maskfeldtyp nicht bekannt
#define ONLYFUNC                420     // only func as handler
#define IMPOSSIBLE              421     // should never occure
#define NOENDOFCOMMENT          422     // '/*' without '*/' found
#define ILLEGALFIELDNO			423		// seg 16 und Feld 16 ex. nicht

#endif /* INCL_ERRMAN */

/*----------- Global data tree manager --------------------------------------*/
#ifdef INCL_ERRDATATREE

#define EINIT                   500     /* Liste nicht gefunden oder falsch*/
#define EILLEGAL                501     /* ungÅltiger Name fÅr Variable */
#define EDOUBLE                 502     /* Variable existiert bereits */
#define ETYPE                   504     /* Directory mit Variable verwechselt*/
#define EPROTECTION             505     /* Variable geschÅtzt */
#define ENOTEMPTY               506     /* Verzeichniss nicht leer */
#define EBUSY                   507     /* Variable schon in gebrauch */

#endif /* INCL_ERRDATATREE */

/*------------ Network Errors -----------------------------------------------*/
#ifdef INCL_ERRCOM

#define ETOOBUSY           		601		/* "Remote Computer ist Åberlastet" */
#define ECRC            		602		/* "öbermittlungsfehler" */
#define EUNKNOWNCOM     		603		/* "Commando unbekannt" */
#define EIMPOSSIBLE     		604		/* "Programminkonsistenz" */
#define EQUEUE          		605		/* "Fehler in Warteschlange" */
#define ECHANELTABLE    		606		/* "Fehler in Kanaltabelle" */
#define ESEM            		607		/* "Fehler in Semaphorenhandling" */
#define ETIMEOUT        		608		/* "Timeout" */
#define EPERMISSION     		609		/* "Operation nicht erlaubt" */
#define ENOCONFIGSET    		610		/* "Konfiguration nicht gefunden" */
#define EFILENOTFOUND   		611		/* "Datei nicht gefunden" */
#define EPACKETBACK     		612		/* "Packet kam wieder zurÅck" */
#define ECREATE         		613		/* "Datei kann nicht kreiert werden" */
#define EPACKET         		614		/* "Unbekannter Packet-Status" */
#define EBREAK          		615		/* "Signalpacket fÅr Abbruch der Verbindung" */
#define EREMOTE         		616		/* "Fehler im Remote Computer" */
#define EDATAEXISTS     		617		/* "Konfiguration existiert schon. Com zweimal aktiv ?" */
#define ECFGTABLE      			618		/* "Konfiguration existiert nicht. Com nicht gestartet ?" */
#define EUIBUF          		619		/* "Buffer vom User-Interface nicht gefunden." */
#define ETERMREQ        		620		/* "Termination vom User-Interface gefordert (BREAK)." */
#define ENORESOURCES    		621		/* "DOS: Keine Resourcen verfÅgbar (TOOBUSY)." */
#define EBADHANDLE      		622		/* DOS */
#define ENODRIVER       		623		/* DOS */
#define EBADDRIVER      		624		/* DOS */
#define ENOBLOCK        		625		/* DOS */
#define ENOONE          		626		/* "Timeout bei Nachricht. Kein User am Remote Computer" */
#define ENOBUFMEM       		627		/* DOS */
#define EIOCTL					628		/* "Fehler in IOCTL." */
#define ETHREAD					629		/* "Computer Åberlastet." create_thread */
#define ESTARTUP				630		/* "Start-Daten fÅr Remote nicht Ok." LÑnge nicht ok */
#define EMODEMISMATCH			631		/* "Daten in nicht unterstÅtztem Modus empfangen." */
#define EPOPUP					632		/* "Konnte nicht Auf-Popen." VIOPOPUP-err */
#define EMORE					633     /* "Msg wird fortgesetzt." Kein Fehler ! */
#define ETHREADDEAD				634		/* "Packet an Thread der nicht mehr aktiv ist !" */
#define ECOULDNOTTERM			635		/* "Funktion konnte nicht ordnungsgemÑss beendet werden !" No Term-Msg was sent... */
#define ESELNOTGIVEABLE			636     /* "Speicherproblem: Zurgriffsrecht konnte nicht erteilt werden !" */
#define ECHANNELUSED			637		/* "Es lÑuft bereits eine Applikation mit gleichem Kanal-Name !" Unexpected Name-match in Channel-Table */
#define ENOCOM					638		/* "COM-Port nicht bereit." DosOpen (com) */
#define ETHREADPRIORITY			639		/* "PrioritÑt konnte nicht richtig gesetz werden." CWRITE braucht TimeCritical... */
#define ENOCHANNEL				640		/* "Kein freier Kanal mehr." MAXCHANNEL ist die Limite */
#define ENETINTERRUPTED			641		/* "Warnung: Init findet Netz unterbrochen..." poll wird weiterhin dranbleiben ! */
#define EBADARG					642		/* "Argument illegal." name too long, ... */
#define ENOCOMDLL				643		/* "Bibliothek fÅr Commando nicht vorhanden." DLL-err */
#define ENOCOMFUNC				644		/* "Commando nicht in Bibliothek." func not found */
#define EQUEUEEMPTY				645		/* "Keine Element in der Warteschlange." */

#endif /* INCL_ERRCOM */

/*------------ Thread Errors ------------------------------------------------*/
#ifdef INCL_ERRTHREAD
#define ENOFUNC                         700
#define ESTARTTHREAD                    701
#define EUSERINIT                       702
#endif

/*------------ StdWin Errors ------------------------------------------------*/
#ifdef INCL_ERRSTDWIN
#define EREGISTER                       800
#define ECLOSEWIN                       801
#define EOPENWIN                        802
#define EPROFILE                        803
#endif

/*------------ MSGBOX Errors ------------------------------------------------*/
#ifdef INCL_ERRMSGBOX
#define ESETTITLE						900 /* "Titel der Message-Box konnte nicht gesetzt werden." */
#define EINVALIDATE						901 /* "Message-Box konnte nicht invalidiert werden." */
#define EBITMAPNOTFOUND					902 /* "Bitmap nicht gefunden." */
#define EPOSTMSG						903 /* "Message-Box konnte nicht benachrichtigt werden." Error in WinPostQueueMsg */
#endif

/*----------- NameList Errors -----------------------------------------------*/
#ifdef INCL_ERRNAMELIST
#define EBADARGS						1000
#endif

/*----------- Game-Interface GI Errors --------------------------------------*/
#ifdef INCL_ERRGI
#define ETOOMANYPLAYERS	                1100 /* "Zu viele Spieler." */
#define EMODULE                         1101 /* "Funktion konnte nicht geladen werden." Modul not found */
#define EINITFUNC                       1102 /* "Funktion konnte nicht initalisiert werden." InitFunc not found */
#define EFUNC                           1103 /* "Funktion konnte nicht geladen werden." Func not found */
#define EHOOK                           1104 /* "Hook-Fehler." Install or Remove */
#define EKBDMON                         1105 /* "Fehler im Tastaturmonitor." Install or Remove */
#endif

/*----------- VIEW.CPP of Boulder-Dash Errors -------------------------------*/
#ifdef INCL_ERRVIEW
#define EVIOSAVREDRAWWAIT			    1200 /* "VioSavRedrawWait() nicht erfolgreich." */
#define EVIOSCRLOCK						1201 /* "VioScrLock() nicht erfolgreich." */
#define EVIOSCRUNLOCK					1202 /* "VioScrUnLock() nicht erfolgreich." */
#define EVIOGETPHYSBUF					1203 /* "VioGetPhysBuf() nicht erfolgreich." */
#endif
