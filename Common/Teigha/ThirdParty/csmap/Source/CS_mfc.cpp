/*
 * Copyright (c) 2008, Autodesk, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Autodesk, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Autodesk, Inc. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Autodesk, Inc. OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cs_map.h"

#if defined(__MFC__) && defined(__CPP__)

#include "cs_mfc.h"

wchar_t* CSt_strNCpy (wchar_t* trg,const wchar_t* src,unsigned chCnt)
{
	wcsncpy (trg,src,chCnt);
	*(trg + chCnt - 1) = L'\0';
	return (trg + wcslen (trg));
}
wchar_t* CSt_strNCpy (wchar_t* trg,const char* src,unsigned chCnt)
{
	mbstowcs (trg,src,chCnt);
	*(trg + chCnt - 1) = L'\0';
	return (trg + wcslen (trg));
}
char* CSt_strNCpy (char* trg,const wchar_t* src,unsigned chCnt)
{
	wcstombs (trg,src,chCnt);
	*(trg + chCnt - 1) = '\0';
	return (trg + strlen (trg));
}
char* CSt_strNCpy (char* trg,const char* src,unsigned chCnt)
{
	strncpy (trg,src,chCnt);
	*(trg + chCnt - 1) = '\0';
	return (trg + strlen (trg));
}
int	CSt_stricmp (const TCHAR* cp1,const TCHAR* cp2)
{
	TCHAR cc1, cc2;
	int result;

	result = 0;
	while (result == 0)
	{
		cc1 = *cp1++;
		cc2 = *cp2++;
		if (_istupper (cc1)) cc1 = (TCHAR)_totlower (cc1); 
		if (_istupper (cc2)) cc2 = (TCHAR)_totlower (cc2);
		result = cc1 - cc2;
		if (cc1 == _T('\0') || cc2 == _T('\0')) break;
	}
	return result;
}
int	CSt_strnicmp (const TCHAR* cp1,const TCHAR* cp2,unsigned count)
{
	TCHAR cc1, cc2;
	int result;
	int myCount;

	myCount = (int)count;
	result = 0;
	while ((myCount-- > 0) && (result == 0))
	{
		cc1 = *cp1++;
		cc2 = *cp2++;
		if (_istupper (cc1)) cc1 = (TCHAR)_totlower (cc1); 
		if (_istupper (cc2)) cc2 = (TCHAR)_totlower (cc2);
		result = cc1 - cc2;
		if (cc1 == _T('\0') || cc2 == _T('\0')) break;
	}
	return result;
}
void CSt_cstringAssign (CString& cString,const char* ccPtr)
{
	TCHAR tcTemp [1024];
	CSt_strNCpy (tcTemp,ccPtr,tchrCount (tcTemp));
	cString = tcTemp;	
}

void CSt_errmsg (TCHAR* bufr,unsigned bufrSize)
{
	char errMesg [256];
	CS_errmsg (errMesg,chrCount (errMesg));
	CSt_strNCpy (bufr,errMesg,bufrSize);
}
int CSt_csIsValid (const TCHAR* tcKeyName)
{
	char csKeyName [32];
	CSt_strNCpy (csKeyName,tcKeyName,chrCount (csKeyName));
	int st = CS_csIsValid (csKeyName);
	return st;
}
int CSt_dtIsValid (const TCHAR* tcKeyName)
{
	char dtKeyName [32];
	CSt_strNCpy (dtKeyName,tcKeyName,chrCount (dtKeyName));
	int st = CS_csIsValid (dtKeyName);
	return st;
}
int CSt_elIsValid (const TCHAR* tcKeyName)
{
	char elKeyName [32];
	CSt_strNCpy (elKeyName,tcKeyName,chrCount (elKeyName));
	int st = CS_csIsValid (elKeyName);
	return st;
}
long32_t CSt_atof (double *result,const TCHAR* value)
{
	long32_t rtnValue;
	char chBufr [128];
	
	CSt_strNCpy (chBufr,value,chrCount (chBufr));
	rtnValue = CS_atof (result,chBufr);
	return rtnValue;
}
long32_t CSt_ftoa (TCHAR* bufr,int size,double value,long32_t frmt)
{
	int lclSize;
	long32_t rtnValue;
	char chBufr [128];
	
	chBufr [0] = '\0';
	lclSize = (size >= chrCount (chBufr)) ? chrCount (chBufr) : size;
	rtnValue = CS_ftoa (chBufr,lclSize,value,frmt);
	CSt_strNCpy (bufr,chBufr,size);
	return rtnValue;
}
int CSt_elSelector (TCHAR* elKeyName)
{
	int rtnValue;
	char lclElKeyName [cs_KEYNM_DEF];
	
	CSt_strNCpy (lclElKeyName,elKeyName,chrCount (lclElKeyName));
	rtnValue = CS_elSelector (lclElKeyName);
	CSt_strNCpy (elKeyName,lclElKeyName,cs_KEYNM_DEF);
	return rtnValue;	
}
struct cs_Eldef_* CSt_eldef (const TCHAR* el_nam)
{
	struct cs_Eldef_* rtnValue;
	char lclElKeyName [cs_KEYNM_DEF];
	
	CSt_strNCpy (lclElKeyName,el_nam,chrCount (lclElKeyName));
	rtnValue = CS_eldef (lclElKeyName);
	return rtnValue;
}

int CSt_getEllipsoidOf (const TCHAR* csKeyName,TCHAR* ellipsoidName,int size)
{
	int rtnValue;
	char lclCsKeyName [cs_KEYNM_DEF];
	char lclElKeyName [cs_KEYNM_DEF];

	CSt_strNCpy (lclCsKeyName,csKeyName,chrCount (lclCsKeyName));
	lclElKeyName [0] = '\0';
	rtnValue = CS_getEllipsoidOf (lclCsKeyName,lclElKeyName,chrCount (lclElKeyName));
	CSt_strNCpy (ellipsoidName,lclElKeyName,size);
	return rtnValue;	
}
int CSt_unEnum (int index,int type,TCHAR* un_name,int un_size)
{
	int rtnValue;
	char lclUnKeyName [32];

	lclUnKeyName [0] = '\0';
	rtnValue = CS_unEnum (index,type,lclUnKeyName,chrCount (lclUnKeyName));
	CSt_strNCpy (un_name,lclUnKeyName,un_size);
	return rtnValue;
}
double CSt_unitlu (short type,const TCHAR* name)
{
	double rtnValue;
	char lclUnitName [32];
	
	CSt_strNCpy (lclUnitName,name,chrCount (lclUnitName));
	rtnValue = CS_unitlu (type,lclUnitName);
	return rtnValue;
}
int CSt_isgeo (const TCHAR* csys)
{
	int rtnValue;
	char lclCsKeyName [cs_KEYNM_DEF];
	
	CSt_strNCpy (lclCsKeyName,csys,chrCount (lclCsKeyName));
	rtnValue = CS_isgeo (lclCsKeyName);
	return rtnValue;
}
int CSt_getUnitsOf (const TCHAR* csKeyName,TCHAR* unitName,int size)
{
	int rtnValue;
	char lclCsKeyName [cs_KEYNM_DEF];
	char lclUnitName [32];

	CSt_strNCpy (lclCsKeyName,csKeyName,chrCount (lclCsKeyName));
	lclUnitName [0] = '\0';
	rtnValue = CS_getUnitsOf (lclCsKeyName,lclUnitName,chrCount (lclUnitName));
	CSt_strNCpy (unitName,lclUnitName,size);
	return rtnValue;
}
struct cs_Dtdef_* CSt_dtdef (const TCHAR* dat_nam)
{
	struct cs_Dtdef_* rtnValue;
	char lclDtKeyName [cs_KEYNM_DEF];
	
	CSt_strNCpy (lclDtKeyName,dat_nam,chrCount (lclDtKeyName));
	rtnValue = CS_dtdef (lclDtKeyName);
	return rtnValue;
}
int CSt_getDatumOf (const TCHAR* csKeyName,TCHAR* datumName,int size)
{
	int rtnValue;
	char lclCsKeyName [cs_KEYNM_DEF];
	char lclDtKeyName [32];

	CSt_strNCpy (lclCsKeyName,csKeyName,chrCount (lclCsKeyName));
	lclDtKeyName [0] = '\0';
	rtnValue = CS_getDatumOf (lclCsKeyName,lclDtKeyName,chrCount (lclDtKeyName));
	CSt_strNCpy (datumName,lclDtKeyName,size);
	return rtnValue;
}
struct cs_Datum_* CSt_dtloc (const TCHAR* dat_nam)
{
	struct cs_Datum_* rtnValue;
	char lclDtKeyName [cs_KEYNM_DEF];
	
	CSt_strNCpy (lclDtKeyName,dat_nam,chrCount (lclDtKeyName));
	rtnValue = CS_dtloc (lclDtKeyName);
	return rtnValue;
}
struct cs_Csdef_* CSt_csdef (const TCHAR* keyName)
{
	char cKeyName [cs_KEYNM_DEF];
	CSt_strNCpy (cKeyName,keyName,sizeof (cKeyName));
	struct cs_Csdef_* rtnValue = CS_csdef (cKeyName);
	return rtnValue;
}
struct cs_Csprm_* CSbcclu_T (const TCHAR* csKeyName)
{
	struct cs_Csprm_* rtnValue;
	char lclCsKeyName [cs_KEYNM_DEF];
	
	CSt_strNCpy (lclCsKeyName,csKeyName,sizeof (lclCsKeyName));
	rtnValue = CSbcclu (lclCsKeyName);
	return rtnValue;
}
int CSt_csEnum (int index,TCHAR* keyName,unsigned size)
{
	int st;
	char cKeyName [cs_KEYNM_DEF];
	st = CS_csEnum (index,cKeyName,sizeof (cKeyName));
	if (st >= 0)
	{
		CSt_strNCpy (keyName,cKeyName,size);
	}
	return st;
}
int	CSt_prjEnum (int index,ulong32_t *prj_flags,TCHAR* prj_keynm,
												int keynm_sz,
												TCHAR* prj_descr,
												int descr_sz)
{
	int prjCode;
	char cPrjKeyName [24];
	char cDescription [64];
	
	prjCode = CS_prjEnum (index,prj_flags,cPrjKeyName,sizeof (cPrjKeyName),
										  cDescription,sizeof (cDescription));
	if (prjCode >= 0)
	{
		CSt_strNCpy (prj_keynm,cPrjKeyName,keynm_sz);
		CSt_strNCpy (prj_descr,cDescription,descr_sz);
	}
	return prjCode;
}
int	CSt_quadEnum (int index,TCHAR* descr,int size,int *ident)
{
	int st;
	char cDescription [128];
	st = CS_quadEnum (index,cDescription,sizeof (cDescription),ident);
	if (st > 0)
	{
		CSt_strNCpy (descr,cDescription,size);
	}
	return st;
}
int	CSt_viaEnum (int index,TCHAR* descr,int size,int *ident)
{
	int st;
	char ccDescription [128];

	st = CS_viaEnum (index,ccDescription,sizeof (ccDescription),ident);
	if (st >= 0)
	{
		CSt_strNCpy (descr,ccDescription,size);
	}
	return st;
}
int	CSt_dtEnum (int index,TCHAR* key_name,int size)
{
	int st;
	char ccDtName [cs_KEYNM_DEF];
	st = CS_dtEnum (index,ccDtName,cs_KEYNM_DEF);
	if (st > 0)
	{
		CSt_strNCpy (key_name,ccDtName,size);
	}
	return st;
}
int	CSt_elEnum (int index,TCHAR* key_name,int size)
{
	int st;
	char ccElName [cs_KEYNM_DEF];
	st = CS_elEnum (index,ccElName,cs_KEYNM_DEF);
	if (st > 0)
	{
		CSt_strNCpy (key_name,ccElName,size);
	}
	return st;
}
int	CSt_csGrpEnum (int index,TCHAR* grp_name,int name_sz,TCHAR* grp_dscr,int dscr_sz)
{
	int st;
	char cGrpName [32];
	char cGrpDesc [128];

	cGrpName [0] = cGrpDesc [0] = '\0';
	st = CS_csGrpEnum (index,cGrpName,sizeof (cGrpName),cGrpDesc,sizeof (cGrpDesc));
	if (st >= 0)
	{
		if (grp_name != NULL)
		{
			CSt_strNCpy (grp_name,cGrpName,name_sz);
		}
		if (grp_dscr != NULL)
		{
			CSt_strNCpy (grp_dscr,cGrpDesc,dscr_sz);
		}
	}
	return st;
}
int CSt_csEnumByGroup (int index,const TCHAR* grp_name,struct csT_Csgrplst_ *cs_descr)
{
	int st;
	char cGrpName [32];
	struct cs_Csgrplst_ csGrpList;
	
	CSt_strNCpy (cGrpName,grp_name,sizeof (cGrpName));
	st = CS_csEnumByGroup (index,cGrpName,&csGrpList);
	if (st > 0)
	{
		CSt_strNCpy (cs_descr->key_nm,csGrpList.key_nm,tchrCount (cs_descr->key_nm));
		CSt_strNCpy (cs_descr->descr,csGrpList.descr,tchrCount (cs_descr->descr));
		CSt_strNCpy (cs_descr->source,csGrpList.source,tchrCount (cs_descr->source));
		CSt_strNCpy (cs_descr->ref_typ,csGrpList.ref_typ,tchrCount (cs_descr->ref_typ));
		CSt_strNCpy (cs_descr->ref_to,csGrpList.ref_to,tchrCount (cs_descr->ref_to));
		CSt_strNCpy (cs_descr->unit,csGrpList.unit,tchrCount (cs_descr->unit));
	}
	return st;
}
int CSt_dtSelector (TCHAR* dtKeyName)
{
	int rtnValue;
	char lclDtKeyName [cs_KEYNM_DEF];
	
	CSt_strNCpy (lclDtKeyName,dtKeyName,chrCount (lclDtKeyName));
	rtnValue = CS_dtSelector (lclDtKeyName);
	CSt_strNCpy (dtKeyName,lclDtKeyName,cs_KEYNM_DEF);
	return rtnValue;	
}
int CSt_csBrowser (TCHAR* csKeyName)
{
	int rtnValue;
	char lclCsKeyName [cs_KEYNM_DEF];
	
	CSt_strNCpy (lclCsKeyName,csKeyName,chrCount (lclCsKeyName));
	rtnValue = CS_csBrowser (lclCsKeyName);
	CSt_strNCpy (csKeyName,lclCsKeyName,cs_KEYNM_DEF);
	return rtnValue;
}
int CSt_dtSelectorA (TCHAR* dtKeyName,TCHAR* description,TCHAR* source,TCHAR* ellipsoid,TCHAR* technique)
{
	int st;
	char dtKeyNameA [cs_KEYNM_DEF];
	char descriptionA [64];
	char sourceA [64];
	char ellipsoidA [cs_KEYNM_DEF];
	char techniqueA [32];
	
	CSt_strNCpy (dtKeyNameA,dtKeyName,chrCount (dtKeyNameA));
	*description = _T('\0');
	*source = _T('\0');
	*ellipsoid = _T('\0');
	*technique = _T('\0');
	st = CS_dtSelectorA (dtKeyNameA,descriptionA,sourceA,ellipsoidA,techniqueA);
	if (st == IDOK)
	{
		CSt_strNCpy (dtKeyName,dtKeyNameA,cs_KEYNM_DEF);
		CSt_strNCpy (description,descriptionA,64);
		CSt_strNCpy (source,sourceA,64);
		CSt_strNCpy (ellipsoid,ellipsoidA,cs_KEYNM_DEF);
		CSt_strNCpy (technique,techniqueA,32);
	}
	return st;
}
int CSt_elSelectorA (TCHAR* elKeyName,TCHAR* description,TCHAR* source)
{
	int st;
	char elKeyNameA [cs_KEYNM_DEF];
	char descriptionA [64];
	char sourceA [64];
	
	CSt_strNCpy (elKeyNameA,elKeyName,chrCount (elKeyNameA));
	*description = _T('\0');
	*source = _T('\0');
	st = CS_elSelectorA (elKeyNameA,descriptionA,sourceA);
	if (st == IDOK)
	{
		CSt_strNCpy (elKeyName,elKeyNameA,cs_KEYNM_DEF);
		CSt_strNCpy (description,descriptionA,64);
		CSt_strNCpy (source,sourceA,64);
	}
	return st;
}
int CSt_newKeyName (TCHAR* keyName,int type,int preserve)
{
	char keyNameA [cs_KEYNM_DEF];
	CSt_strNCpy (keyNameA,keyName,chrCount (keyNameA));
	int st = CS_newKeyName (keyNameA,type,preserve);
	if (st == IDOK)
	{
		CSt_strNCpy (keyName,keyNameA,cs_KEYNM_DEF);
	}
	return st;
}

void CSt_csfnm (const TCHAR* new_name)
{
	char cKeyName [cs_KEYNM_DEF];
	CSt_strNCpy (cKeyName,new_name,cs_KEYNM_DEF);
	CS_csfnm (cKeyName);
}
void CSt_dtfnm (const TCHAR* new_name)
{
	char cKeyName [cs_KEYNM_DEF];
	CSt_strNCpy (cKeyName,new_name,cs_KEYNM_DEF);
	CS_dtfnm (cKeyName);
}
void CSt_elfnm (const TCHAR* new_name)
{
	char cKeyName [cs_KEYNM_DEF];
	CSt_strNCpy (cKeyName,new_name,cs_KEYNM_DEF);
	CS_elfnm (cKeyName);
}
int	CSt_altdr (const TCHAR* alt_dir)
{
	int st;
	const char* ccPtr;
	char cDirPath [MAXPATH + 16];

	ccPtr = 0;
	if (alt_dir != 0)
	{
		CSt_strNCpy (cDirPath,alt_dir,sizeof (cDirPath));
		ccPtr = cDirPath;
	}
	st = CS_altdr (ccPtr);
	return st;
}
int	CSt_altdr (TCHAR* alt_dir)
{
	int st;
	char* ccPtr;
	char cDirPath [MAXPATH + 16];

	ccPtr = 0;
	if (alt_dir != 0)
	{
		CSt_strNCpy (cDirPath,alt_dir,sizeof (cDirPath));
		ccPtr = cDirPath;
	}
	st = CS_altdr (ccPtr);
	return st;
}
int	CSt_access (const TCHAR* path,int mode)
{
	char cTemp [MAXPATH + 16];
	CSt_strNCpy (cTemp,path,sizeof (cTemp));
	int st = CS_access (cTemp,mode);
	return st;
}
#endif
