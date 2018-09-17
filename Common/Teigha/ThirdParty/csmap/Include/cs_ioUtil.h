/*
 * Copyright (c) 2012, Autodesk, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the distribution.
 *	* Neither the name of the Autodesk, Inc. nor the names of its
 *	  contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
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

#ifndef __CS_IOUTILH__
#define __CS_IOUTILH__

#ifdef __cplusplus
extern "C" {
#endif

#define CS_CHECK_ARG_RETURN(arg,check,argNo,ret) \
    if ((check)) \
	{ \
		CS_erpt(cs_INV_ARG##argNo); \
		return ret; \
	}

#define CS_CHECK_NULL_ARG_RETURN(arg,argNo,ret) CS_CHECK_ARG_RETURN(arg, NULL == arg, argNo, ret)

#define CS_CHECK_NULL_ARG(arg,argNo) CS_CHECK_NULL_ARG_RETURN(arg, argNo, -1)
#define CS_CHECK_EMPTY_STRNG_ARG(arg,argNo) CS_CHECK_ARG_RETURN(arg, NULL == arg || '\0' == *arg, argNo, -1)

/**************************/
/*    Some default API    */
/**************************/
int CS_setdr(Const char* newTargetDir, char* currentDir);
int CS_getdr(char* currentDir);

/*******************************************/
/*    Coordinate System C++ API wrapper    */
/*******************************************/
csFILE* CS_csFileOpen(Const char* mode);
int CS_csRead (csFILE *strm, struct cs_Csdef_ *cs_def, int* crypt);
int CS_csWrite(csFILE *strm, Const struct cs_Csdef_ *cs_def, int crypt);
int CS_csDelete(struct cs_Csdef_ *cs_def);
int CS_csUpdate(struct cs_Csdef_ *cs_def, int encrypt);
struct cs_Csdef_* CS_csDefinition(Const char *csName, char* pszDirPath, int* isUsrDef);
int CS_csDefinitionAll(struct cs_Csdef_**[]);

/*******************************/
/*    Datum C++ API wrapper    */
/*******************************/
csFILE* CS_dtFileOpen(Const char* mode);
int CS_dtRead (csFILE *strm, struct cs_Dtdef_ *dt_def, int* crypt);
int CS_dtWrite(csFILE *strm, Const struct cs_Dtdef_ *dt_def, int crypt);
int CS_dtDelete(struct cs_Dtdef_ *dt_def);
int CS_dtUpdate(struct cs_Dtdef_ *dt_def, int encrypt);
struct cs_Dtdef_* CS_dtDefinition(Const char *dtName, char* pszDirPath, int* isUsrDef);
int CS_dtDefinitionAll(struct cs_Dtdef_**[]);

/***********************************/
/*    Ellipsoid C++ API wrapper    */
/***********************************/
csFILE* CS_elFileOpen(Const char* mode);
int CS_elRead (csFILE *strm, struct cs_Eldef_ *el_def, int* crypt);
int CS_elWrite(csFILE *strm, Const struct cs_Eldef_ *el_def, int crypt);
int CS_elDelete(struct cs_Eldef_ *el_def);
int CS_elUpdate(struct cs_Eldef_ *el_def, int encrypt);
struct cs_Eldef_* CS_elDefinition(Const char *elName, char* pszDirPath, int* isUsrDef);
int CS_elDefinitionAll(struct cs_Eldef_**[]);

/***************************************/
/*    Geodetic Path C++ API wrapper    */
/***************************************/
csFILE* CS_gpFileOpen(Const char* mode);
int CS_gpRead (csFILE *strm, struct cs_GeodeticPath_ * gp_def);
int CS_gpWrite(csFILE *strm, Const struct cs_GeodeticPath_ * gp_def);
int CS_gpDelete(struct cs_GeodeticPath_ * gp_def);
int CS_gpUpdate(struct cs_GeodeticPath_ *gp_def);
struct cs_GeodeticPath_* CS_gpDefinition(Const char *gpName, char* pszDirPath);
int CS_gpDefinitionAll(struct cs_GeodeticPath_**[]);

/******************************************/
/* Geodetic Transformation C++ API wrapper*/
/******************************************/
csFILE* CS_gxFileOpen(Const char* mode);
int CS_gxRead (csFILE *strm, struct cs_GeodeticTransform_ *gx_def);
int CS_gxWrite(csFILE *strm, Const struct cs_GeodeticTransform_ * gx_def);
int CS_gxDelete(struct cs_GeodeticTransform_ *gx_def);
int CS_gxUpdate(struct cs_GeodeticTransform_ *gx_def);
struct cs_GeodeticTransform_* CS_gxDefinition(Const char *xfrmName, char* pszDirPath);
int CS_gxDefinitionAll(struct cs_GeodeticTransform_** pAllDefs[]);

/****************************/
/* Category C++ API wrapper */
/****************************/
csFILE* CS_ctFileOpen(Const char* mode);
int CS_ctRead (csFILE *strm, struct cs_Ctdef_ *ct_def);
int CS_ctWrite(csFILE *strm, Const struct cs_Ctdef_ *ct_def);
int CS_ctDelete(struct cs_Ctdef_ *ct_def);
int CS_ctUpdate(Const struct cs_Ctdef_ *ct_def);
struct cs_Ctdef_* CS_ctDefinition(Const char *ctName);
int CS_ctDefinitionAll(struct cs_Ctdef_** pAllDefs[]);

int CS_gxGridOvrly(struct cs_GeodeticTransform_ *gx_target, Const struct cs_GeodeticTransform_ *gx_source);
char Const* CS_gxkey(Const struct cs_GeodeticTransform_ *gx_def);
int CS_gxwrtchk(struct cs_GeodeticTransform_ *gx_target, Const struct cs_GeodeticTransform_ *gx_source, int* isProtected);

#ifdef __cplusplus
 }
#endif

#endif
