/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of COLLADAFramework.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#ifndef __COLLADAFW_PREREQUISITES_H__
#define __COLLADAFW_PREREQUISITES_H__

#include <string>

namespace COLLADAFW
{
    typedef std::string String;
}

#define FW_NEW new 
#define FW_DELETE delete 


#ifndef COLLADAFRMW_STATIC
#  if defined(_MSC_VER) || defined(__BORLANDC__)
#    pragma warning( disable: 4275 4251 )
#    ifdef  COLLADAFRMW_EXPORT_API
#      define _COLLADAFRMW_API    __declspec( dllexport )
#    else
#      define _COLLADAFRMW_API    __declspec( dllimport )
#    endif
#  else // defined(__GNUC__) || defined(__APPLE__) or other unix
#    define _COLLADAFRMW_API __attribute__((visibility("default")))
#  endif
#else
#  define _COLLADAFRMW_API
#endif

#endif //__COLLADAFW_PREREQUISITES_H__
