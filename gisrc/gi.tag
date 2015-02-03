.*---------------------------------------------------------------------------
.*
.* TAG-File for GI-Documentation
.*
.* Version: 1.0
.*
.* Revision History:
.*
.*      18.11.93    RF  Created
.*      16.12.93    RF  Ready for XMAS-Release
.*---------------------------------------------------------------------------
:userdoc.
:title.GI-Documentation
.*---------------------------------------------------------------------------
.* ABSTRACT                                                         1000
.*---------------------------------------------------------------------------
:h1 x=right width=75% res=1000.Abstract
:p.
:hp2.Abstract:ehp2.
:p.GI is a generic Interface between :link reftype=hd res=6000.Games:elink.
 and :link reftype=hd res=5000.Input Devices:elink..
:p.GI was designed to detach the task of getting and normalizing data from
input devices from the task of programming a game.
:p.The following picture may clarify this&colon.
:cgraphic.

      GAMES                                 INPUT DEVICES

  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                          ÚÄÄÄÄÄÄÄÄÄÄ¿
  ³ Wizard of Wor ³<Ä¿                   ÚÄ<Ä³ Keyboard ³
  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ  ³                   ³   ÀÄÄÄÄÄÄÄÄÄÄÙ
  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿  ³      ÉÍÍÍÍÍÍ»     ³   ÚÄÄÄÄÄÄÄÄÄÄ¿
  ³ Boulder Dash  ³<ÄÅÄÄ<ÄÄÄº  GI  º<ÄÄÄÄÅÄ<Ä³ Joystick ³
  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ  ³      ÈÍÍÍÍÍÍ¼     ³   ÀÄÄÄÄÄÄÄÄÄÄÙ
  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿  ³                   ³   ÚÄÄÄÄÄÄÄÄÄÄ¿
  ³ Tron          ³<Ä´                   ÃÄ<Ä³ Straight ³
  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ  ³                   ³   ÀÄÄÄÄÄÄÄÄÄÄÙ
                     ³                   ³
        ...                                      ...

:ecgraphic.
:p.:hp2.Therefore&colon.:ehp2.
:ul.
:li.An Input Device knows how to handle data from a device
(eg. a Joystick or a keyboard).
:li.A Game does not care what kind of Input Device the user prefers. It just
queries GI for user input.
:eul.
:p.Or with other words&colon.
:p.GI is a black box that hides all information about the user preferences of
input devices from the game.
.*---------------------------------------------------------------------------
.* HIGHLIGHTS                                                       2000
.*---------------------------------------------------------------------------
:h1 x=center y=center width=60% height=60% res=2000 .Highlights
:p.:hp2.Special Highlights of GI version 1.0:ehp2.
:ul compact.
:li.Interface for Games and Input Devices fully written in C
:li.Source code is available
:li.Sample code for both Games and Input Devices available
:li.GI is configured once for all Games!
:li.Possibility for per-Game configuration exists
:li.Analog Input Devices can be used with Games requireing digital user input and vice versa (although this has not been tested yet...)
:li.Up to 10 Players can play simultaneously!
:eul.
.*---------------------------------------------------------------------------
.* INSTALLATION                                                     3000
.*---------------------------------------------------------------------------
:h1 x=right y=bottom width=75% height=100% res=3000 .Installation
:p.:hp2.Installation:ehp2.
:p.
GI consists of two ZIP-Files:
:ul compact.
:li.GIRUN.ZIP (all files needed to :hp1.run:ehp1. GI)
:li.GISRC.ZIP (:hp1.all:ehp1. source files for GI)
:eul.
:p.These two ZIP-Files are bound together in GI.ZIP (as you noticed...).
If you just want to :hp1.use:ehp1. GI, you just unzip GIRUN.ZIP. It contains
all modules required.
:p.The GISRC.ZIP file contains :hp1.all:ehp1. the source code of GI! You
don't need to unzip the TOOLS.ZIP, PMTOOLS.ZIP and STD.ZIP files if you don't
recompile GI.
:p.:hp2.Installation step by step:ehp2.
:ol.
:li.Create a directory named "GI". GI is used in more than one game, so this
step makes sense even if you don't like it...
:li.Copy GIRUN.ZIP to this directory and change (cd) to it.
:li.Unzip GIRUN.ZIP.
:li.Make sure the following files have been unzipped correctly&colon.
    :ul compact.
    :li.GI.DLL
    :li.GICONFIG.EXE
    :eul.
:li.Edit your CONFIG.SYS file&colon.
  :ol.
  :li.Append the full drive and path name of the GI-directory to your LIBPATH-entry&colon.
    :xmp.:hp4.LIBPATH=...;C&colon.&bsl.GI:ehp4.:exmp.
    :p.Otherwise you'll have to put all the dynamic link libraries (*.DLL) of GI to a location already mentioned in your LIBPATH-entry.
  :li.Add the following line to the end of your CONFIG.SYS&colon.
    :xmp.:hp4.SET GICONFIG=C&colon.&bsl.GI&bsl.GICONFIG.DAT:ehp4.:exmp.
    :p.(Assuming you installed GI in a directory "GI" on the C&colon. drive.)
  :li.If you have a PC-Joystick, add the following line to your CONFIG.SYS&colon.
    :xmp.:hp4.DEVICE=C&colon.&bsl.GI&bsl.GAMEPORT.SYS:ehp4.:exmp.
    :p.(Again assuming you installed GI in C&colon.&bsl.GI.)
  :eol.
:li.Restart your system for the changes in CONFIG.SYS to take effect...
:eol.
:p.That's it. After the restart of your system you can continue by 
:link reftype=hd res=4000.configuring:elink. GI.
.*---------------------------------------------------------------------------
.* CONFIGURATION                                                    4000
.*---------------------------------------------------------------------------
:h1 res=4000 .Configuration
:p.:hp2.Configuration:ehp2.
:p.GI knows two diffrent kinds of configurations&colon.
:ol compact.
:li.Global/Default configuration
:li.Local configuration
:eol.
:p.:hp2.Basics:ehp2.
:p.One of the base ideas behind GI was, that one configuration should be
enough for all kind of games. To statisfy this criterion GI has a global
configuration file (GICONFIG.DAT) that holds all information for the input
devices. This file is located as mentioned in the
:link reftype=hd res=3000.installation:elink. section.
:p.But since the restriction to only one configuration may be too hard (imagine
a game that uses really weird controls) the possibility for a local
configuration was introduced. This local or per-game configuration works just
the same way as the global configuration does. The only difference being that
the configuration file for the local configuration is located in the directory
of the game whereas the global configuration file resides in the GI directory.
:p.:hp2.Configuration of GI:ehp2.
:p.Configuration of GI and its input devices is done with the 
:hp4.GICONFIG:ehp4. program. This program uses a standard OS/2 Presentation Manager
interface. Configuration is performed in two steps&colon.
:ol compact.
:li.Select an input device from the list or define a new by pressing the
:hp4.New Player:ehp4. button and selecting one from the list presented.
(You'll have to select the proper directory first, of course...)
:note.Currently available devices are&colon.STRAIGHT.DLL, KBD.DLL, JOY.DLL and
TAPEDEV.DLL. See :link reftype=hd res=5300.Devices:elink. for further information.
:li.Start the configure part of the selected device by pressing the
:hp4.Define:ehp4. button or double-clicking in the list of devices.
:eol.
:note.Device 1 will be queryed for input to move player 1, device 2 for player 2, etc.!
:p.:hp2.Special Hint when inserting new devices:ehp2.
:p.When you hit the :hp4.New Player:ehp4. button and :hp1.no device in the list is
highlighted:ehp1., the new device will be inserted at the :hp1.end:ehp1. of the list. If
there is already a device highlighted, the new one will be inserted 
:hp1.before:ehp1. the highlighted device!
.*---------------------------------------------------------------------------
.* DEVICES                                                          5000
.*---------------------------------------------------------------------------
:h1 x=center y=center width=60% height=75% res=5000 .Devices
:p.:hp2.Devices:ehp2.
:p.GI cannot work without input devices (see :link reftype=hd res=1000 .Abstract:elink.).
The most important devices have already been realized and are part of this
version of GI.
:p.Since GI is meant to be a generic interface, everybody is encouraged to
implement further devices. Detailed info in :link reftype=hd res=5200 .Writting a new Device:elink..
:p.Prepared, but not implemented yet, is the possiblity to implement
"intelligent" computer players. Such a computer player device would of course
need further information about the rules and strucutres of a game. But for
the game (and the game programer) the computer player would be fully
transparent. The game would not be able to distinguish a human player from
a computer player. This concept hat already been used in realizing the
:link reftype=hd res=5700 .Tape Recorder:elink. device.
.*----------
:h2 width=100% height=100% res=5100 .Interface from GI to Devices
:p.:hp2.Interface from GI to Devices:ehp2.
:note.This section is for programers only.
:p.Every device is placed in a dynamic link library that gets loaded and
called by GI. Therefore all devices have the same interface to GI and
use excatly the same mechanism for excanging data.
:p.A device must suffice to the following requirements:
:ul.
:li.Must contain the functions _GetDataSize, _InitializeDLL, _Name,
_ConfigDLL, _Threshold and _Direction with the parameters as specified
below
:li.Must be callable with std. C-calling conventions
:eul.
:p.The reason for these requirements being that GI comunicates with the
device :hp1.only:ehp1. through these functions. Furtheron, the adresses of
these functions are calculated using a call-by-name mechanism (that's why
the names are very important).
:p.:hp2.Required functions in a device:ehp2.
:ul.
:li.:hp2._GetDataSize (void)&colon.:ehp2.
Returns the number of bytes required to hold all data, that the device
needs to run (see :hp4.instance data:ehp4. below).
:li.:hp2._InitializeDLL (char __far16 *id)&colon.:ehp2.
When GI is initialized, the devices are initialized with this function. They
receive a pointer to a piece of memory (id) as parameter. The piece of memory
has the size returned by the _GetDataSize() function of the device.
I recommend to have a flag :hp4.initialized:ehp4. in the instance data
to detect, if the data area has already been initialized with default
values or not.
:li.:hp2._Name (char __far16 *mm)&colon.:ehp2.
Copies the name of the device in ascii representation to the memory location (mm)
specified by the parameter. This name is shown in the device-list in GICONFIG.
:li.:hp2._ConfigDLL (char __far16 *id)&colon.:ehp2.
Every device should have a configuration part. GI calls this part, when the
user selects a device and presses the :hp4.Define...:ehp4. button. The
configuration should have a Presentation Manager interface (like the included
devices have). In the configuration part, the user may do all that is
needed to use the device for playing afterwards (definitions, calibrations of
the device, ...).
:note.The selections and definitions that should be saved, must be placed in
the instance data. GI will save this area for you!
:li.:hp2._Threshold (char __far16 *id)&colon.:ehp2.
This function is called to convert analog to digital values.
:xmp.
Example 1:
    analog Joystick:     device returns value=0..100

    GI:                  if game wants analog values then
                             return value
                         else
                             if value < threshold then
                                 return 0
                             else
                                 return 1

Example 2:
    (digital) Keyboard:  device returns value=0 or threshold

    GI:                  return value to game
:exmp.
:p.As you notice, the threshold value has a different meaning for analog and
digital devices.
:note.All devices return values in the range 0..100. Digital devices must
return the threshold value instead of 1.
:li.:hp2._Direction (char __far16 *id, char __far16 *pi)&colon.:ehp2.
This function is called whenever the game wants the actual values from the
device. :hp2.Parameters:ehp2. are&colon.
:ul.
:li.:hp4.id&colon.:ehp4. Pointer to the instance data.
:li.:hp4.pi&colon.:ehp4. Pointer to the PLAYERINFO-structure given to GI
by the game. The device fills the :hp4.direction[]:ehp4. field of this
structure. GI calculates the :hp4.direction:ehp4., :hp4.diagonal:ehp4. and
:hp4.buttons:ehp4. fields afterwards from the values found in the :hp4.direction[]:ehp4.
array.
:eul.
:eul.
:note.The leading underscore (_) is appended automatically, if you program
in C or declare the functions :hp4.extern "C":ehp4. in C++.
:p.:hp2.Instance data:ehp2.
:p.The same device may be configured for more than one player (eg. two players
playing on the same keyboard). GI creates a new instance for every configured
use of the device (eg. different keys to move the figures). Therefore all
configured data should be placed in the instance data. GI will keep track
of saving and reloading the instance data for the devices. :hp4.Don't create
any configuration files and don't use the Profile-functions of Presentation
Manager since you cannot be sure that your device will run with PM present!:ehp4.
Furtheron you should avoid using global variables even if you define your
device to have single nonshared data areas. The reason being that your
device may be loaded more than once within the same process and hence sees
the same global data in all instances.
:p.:link reftype=hd res=5200.Writting a new Device:elink.
.*----------
:h2 x=right y=center width=75% height=100% res=5200 .Writting a new Device
:p.:hp2.Writting a new Device:ehp2.
:p.:hp4.It's easy!:ehp4.
:note.This section is for programers only.
:p.I suggest you copy one of the existing devices (as close as possible to
the one you want to implement) and modify it. Some modifications are needed
if you don't use Borland C++. But in principle, it's very easy&colon.
:ul compact.
:li.Make sure you :hp4.#include <gi.h>:ehp4.
:li.Define :hp1.all:ehp1. functions as desrcibed in :link reftype=hd res=5100.
Interface from GI to device:elink.
:li.Implement a minimul functionality and compile your module to a DLL.
:li.Verify the correctness of the names with a library manager (if GI cannot
find :hp1.all:ehp1. functions, it will refuse to load the devie).
:li.Try loading your device in GICONFIG
:li.Implement the full functionality of your device
:li.Send your device to me for distribution in the next release! :hp4.This
step is optional but strongly recomended!:ehp4.
:eul.
:p.
:note.If you need to debug your device, you'll need to recompile all parts
of GI. To do this you need the sources in TOOLS.ZIP also contained in this
distribution. You may use the GIMAKE.CMD command file if you work with
Borland C++.
.*----------
:h2 x=center y=center width=70% height=60% res=5300 .Supported Devices
:p.:hp2.Supported Devices:ehp2.
:p.Distributed with GI are some input devices. The source code for these
is also contained in the GISRC.ZIP file.
:p.Planned are devices for&colon.
:ul compact.
:li.Game-Demos (Intros, etc.)
:li.Computer-Player
:li.Mouse (?)
:eul.
:p.Refinement and enrichment of existing devices is also possible...
.*----------
:h3 x=center y=center width=50% height=50% res=5400 .Straight Auto-Player
:p.:hp2.Straight Auto-Player:ehp2.
:p.This device was designed to make debugging of games easier. It provides
the possibility of an player walking always in the same direction (or
standing still).
:p.This device is also very usefull to fill the list of installed players...
.*----------
:h3 x=right y=bottom width=75% height=100% res=5500 .Joystick
:p.:hp2.Joystick:ehp2.
:p.There are two kinds of joystick available on the market&colon.
:ul compact.
:li.Digital Joysticks and
:li.Analog Joysticks
:eul.
:p.The difference is very simple: Digital joysticks have (mostly) a
mechanical switch for every of the 4 directions (North, East, South and West)
and the fire-buttons. Analog joysticks have (mostly again) 2 variable resistors
which are connected to the stick in a manner, that their resitance changes
as the stick is moved in x- or y-direction. The actual value of these
resistors gets sampled (10 times per second) and digitized by the game-port.
To access the game-port under OS/2 the GAMEPORT.SYS driver must be
:link reftype=hd res=3000.installed:elink..
:note.Support for digital joystick is not yet implemented.
:p.:hp2.Configuring the analog Joystick:ehp2.
:p.The values of the resistors in a joystick vary from product to product.
To calculate the exact values and scale of the joystick, the resitor values
at the extremal points (upper left and lower right corner) must be taken.
This is called calibration.
:p.The calibration part of the joystick input device configuration can be
activated by pressing the :hp4.Define Max.:ehp4.-button in the joystick configuration.
:p.The joystick configuration dialog shows on the left side the analog
behaviour of the joystick. If you move the stick, the tiny black point
(should) change its position...
:note.If the tiny point does not follow the movements of your joystick,
:hp4.JOYSTICK.SYS:ehp4. may not be installed. Check :link reftype=hd res=3000.Installation:elink.
:p.On the right side of the configuration dialog you see the digital
representation of the analog values on the left. Depending on the selected
threshold the checkboxes check with more or less movement of the stick.
We recomend to experiment with the setting for the threshold because this
is crucial for the behaviour in games requireing digital input!
.*----------
:h3 x=right y=bottom width=75% height=100% res=5600 .Keyboard
:p.:hp2.Keyboard:ehp2.
:p.The Keyboard Device works both in OS/2 Presentation Manager and OS/2 Full-
Screen mode (ever tried to do this?). It intercepts all keyboard activities
and checks wheter the actually pressed key (or combination of keys) matches
any defined pattern.
:p.:hp2.Configuration:ehp2.
:ol compact.
:li.Select the direction or button you want to define the key(combination) for.
:li.Press the :hp4.DEFINE:ehp4. button
:li.Press the :hp4.START:ehp4. button in the new dialog window
:li.Hit the key(combination). The scan-values and state of the control-keys
are displayed.
:li.If you hit the correct key(combination) press the :hp4.STOP:ehp4. button
:li.Hit the :hp4.TEST:ehp4. button and afterwards press your key(combination).
If you hear a short beep, your key(combination) was accepted.
:li.Hit the :hp4.OK:ehp4. button, your definition is completed.
:li.Go back to point 1 if you want to define more directions or buttons.
:eol.
:note.An asterisk :hp4.*:ehp4. on the left of a direction or key signals that
it has been defined.
:warning.Define diferent keys for button 1 and button 2 since some games
may distinguish between the buttons!
:p.:hp2.Threshold:ehp2.
:p.The Threshold slider comes to action, when the keyboard is used in a
game requesting analog input. Since it is impossible to tell how strong a
key is typed, the treshold value is used instead of 0 (key not pressed) and
1 (key pressed)...
:p.
:note.The Keyboard device is also used XFEEL and BUILDER.
.*----------
:h3 x=right y=bottom width=75% height=100% res=5700 .Tape Recorder
:p.:hp2.Tape Recorder:ehp2.
:p.The Tape Recorder consists of two parts&colon.
:ul compact.
:li.TAPEDEV.DLL&colon. input device for GI
:li.TAPE.EXE&colon. Program to control the tape
:eul.
:p.The idea of the tape recorder is, that you can monitor and record
input from any other device and play it back later. The concept may be
known from keyboard-monitors that let you define and play back a series
of keystrokes. The Tape Recorder works just this way: You start recording
and play the game. All your movements get recorded. Later on you can
play this session back.
:p.:hp2.Stop on Overplay option:ehp2.
:p.This feature lets you interrupt (better stop) the playing back of a
recorded tape. Just let the tape run until you feel to take over. As soon
as you move your device you will override the tape and stop its running.
:note.This feature can be turned on and off in the Tape Recorder program.
:p.:hp2.Configuring the Tape Recorder:ehp2.
:ol compact.
:li.Install a Tape Recorder Device (TAPEDEV.DLL) for the player you want to record
:li.Configure the Tape Recorder device&colon. The port number must match
the device you want to move your figure with.
:li.Finish configuring GI
:eol.
:xmp.
Say: You want to monitor player 1 in a game. Input comes from keyboard.

Then: You define the tape device a device number 1. The keyboard must also
be defined (eg. number 3). Then you configure the tape devie to monitor
port number 3.  OK?
:exmp.
:p.:hp2.Recording:ehp2.
:ol compact.
:li.Start the Tape Recorder program (TAPE.EXE)
:li.Select a file with the :hp4.File...:ehp4.-button or enter a filename for the tape.
:li.Start your game and make selections and other stuff you don't want to
be recorded.
:li.Switch back to the Tape Recorder and press the :hp4.RECORD:ehp4. button.
:li.Switch to the game again and play. The Tape Recorder is now recording!
:li.As soon as you like, switch back to the Tape Recorder and press the
:hp4.STOP:ehp4. button. Your tape is ready!
:eol.
:p.:hp2.Playing back:ehp2.
:ol compact.
:li.Start the Tape Recorder program (TAPE.EXE), but only if it's not running...
:li.Select the file to play back.
:li.Start your game and move to the point your recording started.
:li.Switch back to the Tape Recorder and press the :hp4.PLAY:ehp4. button. Your
tape is running!
:li.If the "Stop on Overplay" option is checked, you can stop the show
with a single movement from your original device!
:eol.
:note.Play back will stop as soon as the end of the tape is reached.
:p.:hp2.Concept of the Tape Recorder:ehp2.
:p.Let's look at it from the point of view of a game.
:p.The game wants user input. It calls GI with the number of the player.
GI looks at its internal table, which device corresponds to the requested
player. Then, GI requests the actual data from the device, normalizes it
and returns it to the game.
:p.Now the Tape Recorder consists of a Tape Device which can be called 
from GI as any other device. But the Tape Device does not query any hardware.
Instead it calls GI and queries the actual data for the player
configured. In Record-mode the data is stored in a file and in Play-back-mode
data is loaded from disk instead of querying GI for the original
device.
.*---------------------------------------------------------------------------
.* GAMES                                                            6000
.*---------------------------------------------------------------------------
:h1 x=center y=center width=60% height=75% res=6000 .Games
:p.:hp2.Games:ehp2.
:p.All started with :link reftype=hd res=6400.Wizard of Wor:elink.. At a
certain point in the process of developing the game, we had to decide how
to get the user input to move the figures. Since we didn't have a :link reftype=hd res=5500.joystick:elink.
 or any other input device than mouse and :link reftype=hd res=5600.keyboard:elink.
 we "hard coded" the input-part of the game.
:p.But of course we were not content with this kind of restriction. And after
we released the game to public several people asked for joystick support. Now
it would have been easy to simply add this part, but as we were already
planing our next game (:link reftype=hd res=6500.Boulder Dash:elink.), a
generic solution to this problem seemed more convenient...
:p.:hp4.Since GI cannot exist without GAMES, we want to encourage everybody to
adapt existing games or write new games using GI for the user input!:ehp4.
.*----------
:h2 x=right y=bottom width=75% height=100% res=6100 .Interface from Game to GI
:p.:hp2.Interface from Game to GI:ehp2.
:note.This section is for programers only.
:p.:hp2.Requirements:ehp2.
:p.The game must be programmed in a language that allows C-functions to
be called. Therefore the library GI.LIB must be linked to the game that
uses GI.
:p.There are no problems if the game is written in C or C++, allmost any
compiler should be OK...
:note.GI.LIB contains just the definitions for the linker to know, that
GI.DLL must be loaded to actually execute the GI-functions.
:p.:hp2.Interface functions and data structures:ehp2.
:p.There are only two functions and one data structure required for using
GI in a game&colon.
:ul compact.
:li.PLAYERINFO-structure
:li.GIInitialize()
:li.GIGetDirection (PLAYERINFO *)
:eul.
The prototypings and definitions are in GI.H which must be included to
import the functionality of GI.
:p.:hp2.Meanings of data-members in PLAYERINFO-structure:ehp2.
:lines.
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
:elines.
:p.:hp2.player&colon.:ehp2. Zero-based number of player for which you want
the actual direction and button values.
:p.:hp2.analog&colon.:ehp2. TRUE if game wants analog values in the range
of 0..100 for each of the four directions (see below). FALSE if game wants
digital input (eg. player moves up, strenght not required).
:p.:hp2.dir[6]&colon.:ehp2. Actual values of the queried device of the
4 directions and the 2 buttons.
:p.:hp2.direction&colon.:ehp2. Strongest/main direction coded as mentioned
below.
:p.:hp2.diagonal&colon.:ehp2. Second strongest/secondary direction. If you
don't care about diagonal moves, just ignore this field. Otherwise the
actual direction is between :hp1.direction:ehp1. and :hp1.diagonal:ehp1.
(direction=up, diagonal=left --> diagonal left up).
:p.:hp2.buttons&colon.:ehp2. Binary representation of the states of the
two buttons. If a button is pressed the corresponding bit is 1.
:note. All other fields in :hp1.PLAYERINFO:ehp1. are not used yet!
:p.:hp2.Coding of the directions:ehp2.
:lines.
#define DIRNONE     -1
#define DIRUP       0
#define DIRRIGHT    1
#define DIRDOWN     2
#define DIRLEFT     3

#define DIRBUT1     1
#define DIRBUT2     2
:elines.
:p.The definitions of the four directions are as well the indices into the
:hp1.direction[]:ehp1. array as the values of the :hp1.direction:ehp1. and
:hp1.diagonal:ehp1. fields in PLAYERINFO.
:p.:hp2.Consider the following example&colon.:ehp2.
:xmp.
    /* pi.player and pi.analog must be initialized ! */    

    if (GIGetDirection (&amp.pi))
        {
        /* A error occured, see errdef.h for further coments...
        }
    if (pi.buttons)
        {
        /* process buttons now.... */
        }
    if (pi.direction != DIRNONE)  /* or use: pi.dir[] .... */
        {
        /* process direction .... */
        }
:exmp.
:warning.Don't forget to call :hp1.GIInitialize():ehp1. somwhere in the
startup of your game!
:p.:hp2.Comments:ehp2.
:ul.
:li.Typically, you allocate a PLAYERINFO-structure for every player your
game will support.
:li.The fields for :hp1.strategy:ehp1. functionality are not yet used, since
I don't have a good concept for an example of such a strategy player yet...
:li.The interface was designed to be as narrow as possible to make it
easier to use.
:eul.
.*----------
:h2 x=center y=center width=65% height=65% res=6300 .Games supporting GI
:p.:hp2.Games supporting GI:ehp2.
:p.The three games that we wrote up to now have two things in common:
:ol compact.
:li.They use GI for getting user input
:li.Two players can play at the same time
:eol.
:p.We don't want to make the impression of being "Game-Freaks", but we
like playing games and even more than that, we like to write programs.
Not necessarly games, well noted. Games are the ideal trade off between
writting good software and having some fun afterwards...
:p.And by the way&colon. We also wanted to show that OS/2 is a good operating
system!
.*----------
:h3 x=center y=center width=75% height=60% res=6400 .Wizard of Wor
:p.:hp2.Wizard of Wor:ehp2.
:p.Wizard of Wor is a port from an old C64 game. It was always one of my
favourite&colon. simple but fascinating...
:p.The OS/2-port was done in C++, all code rewritten. The pictures and 
dungeons are the same as in the original (took us quite some time to
capture all the figures...). Some differences occur in the speed-up during
the game and the speed of Worluk and the Wizard.
:p.Wizard of Wor is a real CPU-hog. One big problem was, that the maximal
timer resolution of OS/2 (32ms) was far too slow. So we decided to do a
busy-waiting. The mechanism of slowing the game down in a way that fast
maschines are slowed more than slow maschines was invented by Markus and
is unique (well sort of...).
:note.Our highscore on the original C64 Wizard of Wor is at 194'800 points
in two player mode!
.*----------
:h3 x=center y=center width=75% height=60% res=6500 .Boulder Dash
:p.:hp2.Boulder Dash:ehp2.
:p.An all time goodie. It exists on almost every game computer. This can
be seen as a port of the AMIGA version of Boulder Dash. Most of the ideas
for the special objects such as bombs, enemies, swamp, ..., were taken from
there.
:p.:hp2.For those how don't know Boulder Dash:ehp2.
:p.You (and perhaps your friend) are lost in a system of underground caves.
Your task is to collect the diamonds and enter in the house. You can not
proceed to the next level until you completed the level...
.*----------
:h3 x=center y=center width=50% height=50% res=6600 .Tron
:p.:hp2.Tron:ehp2.
:p.Famous motion picture. A real must for all computer freaks. First movie
with computer animated sequences (as far as I know).
:p.The game was introduced in the film. Two players, both try to lock each
other without crashing into the walls. This game is anything but perfect,
many bugs still wait to be fixed...
.*---------------------------------------------------------------------------
.* ABOUT / Credits                                                  7000
.*---------------------------------------------------------------------------
:h1 x=center y=center width=60% height=80% res=7000 .About 'n Credits
:p.:hp2.About 'n Credits:ehp2.
:p.GI is free! Please use, extend, spread, ... GI as you like.
:p.For further enhancments, comments, questions or bugs please send
an email or a short note:
:lines.
            Roman Fischer
            Leonhardstr. 12
            CH-8001 Zuerich
            Switzerland

            rofische@iiic.ethz.ch
:elines.
:p.
:p.:hp2.Credits:ehp2.
:p.Special thanks go to Markus Hof, who always helps me out of conceptual
troubles and ported :hp1.Wizard of Wor:ehp1. to GI!
:p.Credits to: Andre "Infid" Miodonski, ...
:p.Thanks to Frank Ivan, ICA Systems Inc. for the joystick driver, which is included
in the standard version of GI.
.*---------------------------------------------------------------------------
:euserdoc.

