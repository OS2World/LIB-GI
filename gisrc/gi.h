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

    28.04.93    RF  Enum fÅr Richtungen und Konstante fÅr Anz. Richtungen
                    incl. Buttons (6)
    10.05.93    RF  PreCompiled-Headers, os2.h wird vom Modul geholt
    12.05.93    RF  Konzept etwas Åberdacht...
    21.05.93    RF  dito
    26.05.93    RF  Directions und Buttons getrennt
    06.07.93    RF  Start der Strategy-Routinen
	02.11.93	RF	Threshold wird erst geholt, wenn er gebraucht wird
	05.11.93	RF	Export von fip, numplayer und error fÅr GIConfig
	07.11.93	RF	Mode() wird gar nicht gebraucht !
	08.11.93	RF	div. énderungen am Interface
																			*/
/*--------------------------------------------------------------------------*/
#ifndef GI_INCL
#define GI_INCL

#define INCL_ERRGI
#include <errdef.h>

#define GIF_GETDATASIZE "_GetDataSize"       // Grîsse der Config-Daten
#define GIF_INITIALIZE  "_InitializeDLL"     // Jedes Modul wird so Initialisiert
#define GIF_NAME        "_Name"              // Kopiert den Namen des Moduls
#define GIF_CONFIG      "_ConfigDLL"         // Start des Config-Teils unter PM
#define GIF_THRESHOLD   "_Threshold"         // Gibt Threshold
#define GIF_DIRECTION   "_Direction"         // Gibt aktuelle Direction

#define GI_CONFIGNAME   "GICONFIG.DAT"
#define GI_ENVNAME		"GICONFIG"

#define DIRNONE     -1
#define DIRUP       0
#define DIRRIGHT    1
#define DIRDOWN     2
#define DIRLEFT     3

#define DIRBUT1     1
#define DIRBUT2     2

typedef struct _funcinfo
    {
    char                file[100];      // Name der DLL
    unsigned            datasize;       // Grîsse der Daten fÅr diese Strat.
    void                *data;          // Zeiger auf die Daten
    char                name[100];      // Name der Strategy
	HMODULE             hand;           // Handle auf dieses Modul
    unsigned            (*GetDataSize)(void);
	unsigned            (*func[4])(char __far16 *); // Init, Name, Config, Threshold
	unsigned            (*Direction)(char __far16 *, char __far16 *);
	} FUNCINFO, *FUNCINFOP;

typedef struct _playerinfo
	{
    // Input Data
    unsigned            player;         // Number of actual player
    BOOL                analog;         // Game wants analog-values
    int                 x;              // Actual position x (optional)
    int                 y;              // Actual position y (optional)

    // Output Data
    char                dir[6];         // Values of directions: Up, Right, Down, Left, B1, B2
	char                direction;      // Strongest direction or DIRNONE
	char                diagonal;       // 2nd strongest direction or DIRNONE
	char                buttons;        // 2 bits for buttons

	// Strategy Data
	unsigned            (*Move) (unsigned x, unsigned y);   // Callback to Game!
	void                *stratdata;     // Data for Strategy-Instance
	} PLAYERINFO, *PLAYERINFOP;

#define DIRDIGITAL  FALSE
#define DIRANALOG   TRUE

#define THRESHOLD    100    // Default
#define MAXPLAYER    10

/*--------------------- Intern nur von GI.C --------------------------------*/
#ifdef GI_DEFINE
unsigned _export	error = NOERR;
unsigned _export    numplayer = 0;
FUNCINFOP _export   fip[MAXPLAYER];     // Zeiger auf die fip der Player
char                *funcname[] = { GIF_GETDATASIZE,
									GIF_INITIALIZE, GIF_NAME,
									GIF_CONFIG, GIF_THRESHOLD,
									GIF_DIRECTION };

/*--------------------- Extern fuer alle Kunden von GI ---------------------*/
#else
extern unsigned         error;
extern unsigned         numplayer;
extern FUNCINFOP        fip[];
extern char             *funcname[];

#endif

/*---- Prototypings fÅr alle meine Klienten ----*/
#ifdef __cplusplus
    extern "C" {
#endif

extern unsigned        GIInitialize            ();
extern unsigned        GIGetDirection          (PLAYERINFO *player);

#ifdef __cplusplus
    }
#endif
#endif
