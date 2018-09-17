#if (defined(_WIN32) && (defined(_MSC_VER) || defined(__BORLANDC__)))
#include "jconfig.vc"       /* 	Windows NT/95, MS Visual C++, MS Developer Studio */

#elif(0)
#include "jconfig.manx"	    /* Amiga, Manx Aztec C */
#elif(0)
#include "jconfig.sas"	    /* Amiga, SAS C */
#elif(0)
#include "jconfig.mac"	    /* Apple Macintosh, Metrowerks CodeWarrior */
#elif(0)
#include "jconfig.st"	      /* Atari ST/STE/TT, Pure C or Turbo C */
#elif(0)
#include "jconfig.bcc"	    /* MS-DOS or OS/2, Borland C */
#elif(0)
#include "jconfig.dj"	      /* MS-DOS, DJGPP (Delorie's port of GNU C) */
#elif(0)
#include "jconfig.mc6"	    /* MS-DOS, Microsoft C (16-bit only) */
#elif(0)
#include "jconfig.wat"	    /* MS-DOS, OS/2, or Windows NT, Watcom C */
#elif(0)
#include "jconfig.vms"	    /* Digital VMS */
#endif

#if defined(_AIX) || defined(__sun) || defined(__GNUC__) || defined(sgi) || defined(__hpux)
#define HAVE_PROTOTYPES
#endif
