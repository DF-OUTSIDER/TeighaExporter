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

#include "StdAfx.h"
#include "OdaCommon.h"
#include "DwfImportImpl.h"
#include "DwfCallbacks.h"
#include "summinfo.h"
// w2d metadata records are deprecated in 6.00
//

namespace TD_DWF_IMPORT {
#define SUMMINFO_VALUE(value, BigValue, SummValue) \
WT_Result DwfCallbackManager::process_##value(WT_##BigValue &v, WT_File& file)\
{\
	if (importer(file)->_ignoreMetadata) return WT_Result::Success;\
	OdDbDatabaseSummaryInfoPtr summ = oddbGetSummaryInfo(importer(file)->database());\
  summ->set##SummValue(dwfImp::WTS2ODS(v.string()));\
	return WT_Result::Success;\
}

SUMMINFO_VALUE(author, Author, Author);
SUMMINFO_VALUE(creator, Creator, LastSavedBy);
SUMMINFO_VALUE(keywords, Keywords, Keywords);
SUMMINFO_VALUE(subject, Subject, Subject);
SUMMINFO_VALUE(title, Title, Title);
SUMMINFO_VALUE(comments, Comments, Comments);

#define SUMMINFO_CUSTOM_VALUE(value, BigValue, SummValue)\
WT_Result DwfCallbackManager::process_##value(WT_##BigValue &v, WT_File& file)\
{\
	if (importer(file)->_ignoreMetadata) return WT_Result::Success;\
	OdDbDatabaseSummaryInfoPtr summ = oddbGetSummaryInfo(importer(file)->database());\
	summ->addCustomSummaryInfo(SummValue, dwfImp::WTS2ODS(v.string()));\
	return WT_Result::Success;\
}

SUMMINFO_CUSTOM_VALUE(modification_time, Modification_Time, OD_T("Modification time"));
SUMMINFO_CUSTOM_VALUE(description, Description, OD_T("Description"));
SUMMINFO_CUSTOM_VALUE(source_filename, Source_Filename, OD_T("Source file name"));
SUMMINFO_CUSTOM_VALUE(source_creation_time, Source_Creation_Time, OD_T("Source creation time"));
SUMMINFO_CUSTOM_VALUE(source_modification_time, Source_Modification_Time, OD_T("Source modification time"));
SUMMINFO_CUSTOM_VALUE(copyright, Copyright, OD_T("Copyright"));
SUMMINFO_CUSTOM_VALUE(creation_time, Creation_Time, OD_T("Creation time"));
}
