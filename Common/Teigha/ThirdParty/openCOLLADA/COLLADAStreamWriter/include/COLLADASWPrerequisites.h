/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

	This file is part of COLLADAStreamWriter.
	
    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/


#ifndef __COLLADASTREAMWRITER_PREREQUISITES_H__
#define __COLLADASTREAMWRITER_PREREQUISITES_H__

#include <string>
#include "COLLADABUURI.h"
#include "COLLADABUNativeString.h"
#include "Math/COLLADABUMathUtils.h"


#ifndef COLLADASW_STATIC
#  if defined(_MSC_VER) || defined(__BORLANDC__)
#    pragma warning( disable: 4275 4251 )
#    ifdef  COLLADASW_EXPORT_API
#      define _COLLADASW_API    __declspec( dllexport )
#    else
#      define _COLLADASW_API    __declspec( dllimport )
#    endif
#  else // defined(__GNUC__) || defined(__APPLE__) or other unix
#    define _COLLADASW_API __attribute__((visibility("default")))
#  endif
#else
#  define _COLLADASW_API
#endif //COLLADASW_STATIC

namespace COLLADASW
{
    typedef COLLADABU::URI URI;
    typedef COLLADABU::Utils Utils;
    typedef COLLADABU::NativeString NativeString;
    typedef COLLADABU::Math::Utils MathUtils;

    typedef std::string String;
    typedef std::wstring WideString;
}

#endif //__COLLADASTREAMWRITER_PREREQUISITES_H__
