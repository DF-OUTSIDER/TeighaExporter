/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

// SATConverter.cpp : Defines the entry point for the console application.
//
/************************************************************************/
/* This console application reads an SAT file and converts it to the    */
/* specified type.                                                      */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    SATConverter <infile> <outfile>                                   */
/*                                                                      */
/************************************************************************/
#include "StdAfx.h"

#include "OdaCommon.h"

#include "AcisBuilder/ISatConverter.h"

#include "OdFileBuf.h"

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if (argc < 3) 
	{
    printf("usage: SATConverter <infile> <outfile>\n");
  }
  else
  {
    /********************************************************************/
    /* Create an ISATConverter                                          */
    /********************************************************************/
    ISATConverter *acisFile = ABCreateConverter();
    if (!acisFile)
    {
      printf("Converter creation error.\n");
    }
    else
    {
      try
      {
        OdString in(argv[1]), out(argv[2]);
        OdRdFileBuf inFile(in.c_str());
        OdWrFileBuf outFile(out.c_str());
#if defined(ODA_WINDOWS)
        printf("Proceed file : %ls to %ls\n", argv[1], argv[2]);
#else
        printf("Proceed file : %s to %s\n", argv[1], argv[2]);
#endif
        /****************************************************************/
        /* Read the input file                                          */
        /****************************************************************/
        if (!acisFile->SatIn( &inFile, NULL, true))
        {
          printf("File parsing error.\n");
        }
        else
        {
          printf("File successfully parsed.\n");
          /**************************************************************/
          /* Write the output file                                      */
          /**************************************************************/
          acisFile->SatOut(&outFile, kAfVer400|kAfTypeASCII|kAfTypeIndexed, true);

          if (outFile.length() == 0)
          {
            printf("File converting error.\n");
          }
          else
          {
            printf("File successfully converted.\n");
          }
        }
      }
      catch(const OdError_FileException &err)
      {
        wprintf(OD_T("File error : %ls\n"), err.description().c_str());
      }
      catch(const OdError &)
      {
        printf("Internal error\n");
      }

      ABDeleteConverter(acisFile);
    }
  }

  STD(cout) << "Press RETURN to exit..." << STD(endl);
  STD(cin).get();

	return 0;
}

