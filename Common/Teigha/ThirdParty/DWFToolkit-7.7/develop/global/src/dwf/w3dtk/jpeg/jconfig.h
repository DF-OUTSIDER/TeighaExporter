//
// Copyright (c) 2002 by Tech Soft America, LLC.
// The information contained herein is confidential and proprietary to
// Tech Soft America, LLC., and considered a trade secret as defined under
// civil and criminal statutes.  Tech Soft America shall pursue its civil
// and criminal remedies in the event of unauthorized use or misappropriation
// of its trade secrets.  Use of this information by anyone other than
// authorized employees of Tech Soft America, LLC. is granted only under a
// written non-disclosure agreement, expressly prescribing the scope and
// manner of such use.
//
// $Header: //DWF/Working_Area/Willie.Zhu/w3dtk/jpeg/jconfig.h#1 $
//
// This file contains the required functions and lookup tables to implement the
// some of the compression algorithms.  The algorithm is so named because it
// is based on quantization to a set of vectors that were the output of a point
// repulsion simulation.
//

///
///\file        dwf/w3dtk/jpeg/jconfig.h
///

#ifdef _MSC_VER
#   include "jconfig.vc"
#endif

#   ifdef __VMS
#   include "jconfig.vms"
#endif

#if defined(__APPLE_CC__) || defined(__linux__) || defined(EMCC)
#   include "jconfig.osx"
#endif
/*
 * Note: our normal Unix builds will run "configure" and overwrite
 * this file, so this section shouldn't normally get used...
*/
#       ifdef _AIX
#       include "jconfig.aix"
#       endif
#       ifdef __hpux
#       include "jconfig.hpux"
#       endif
#       ifdef __sgi
#       include "jconfig.irix"
#       endif
#       ifdef __sparc
#       include "jconfig.solaris"
#       endif
/**/
