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

#define chrCount(array) (sizeof (array) / sizeof (array[0]))

wchar_t* CSt_strNCpy (wchar_t* trg,const wchar_t* src,unsigned chCnt);
wchar_t* CSt_strNCpy (wchar_t* trg,const char* src,unsigned chCnt);
char* CSt_strNCpy (char* trg,const wchar_t* src,unsigned chCnt);
char* CSt_strNCpy (char* trg,const char* src,unsigned chCnt);

void CSt_errmsg (TCHAR* bufr,unsigned bufrSize);
int CSt_csIsValid (const TCHAR* acKeyName);
long CSt_atof (double *result,const TCHAR* value);
long CSt_ftoa (TCHAR* bufr,int size,double value,long frmt);
int CSt_elSelector  (TCHAR* elKeyName);
struct cs_Eldef_* CSt_eldef (const TCHAR* el_nam);
int CSt_getEllipsoidOf (const TCHAR* csKeyName,TCHAR* ellipsoidName,int size);
int CSt_unEnum (int index,int type,TCHAR* un_name,int un_size);
double CSt_unitlu (short type,const TCHAR* name);
int CSt_isgeo (const TCHAR* csys);
int CSt_getUnitsOf (const TCHAR* csKeyName,TCHAR* unitName,int size);
struct cs_Dtdef_* CSt_dtdef (const TCHAR* dat_nam);
int CSt_getDatumOf (const TCHAR* csKeyName,TCHAR* datumName,int size);
struct cs_Datum_* CSt_dtloc (const TCHAR* dat_nam);
int CSt_dtSelector  (TCHAR* dtKeyName);
struct cs_Csprm_* rxCSbcclu (const TCHAR* csKeyName);
int CSt_csBrowser (TCHAR* csKeyName);

int CSt_dtSelectorA (TCHAR* dtKeyName,TCHAR* description,TCHAR* source,TCHAR* ellipsoid,TCHAR* technique);
int CSt_elSelectorA (TCHAR* elKeyName,TCHAR* description,TCHAR* source);
int CSt_newKeyName (TCHAR* keyName,int type,int preserve);
