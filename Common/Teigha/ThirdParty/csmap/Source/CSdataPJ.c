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

#ifdef __NO_SETUP__
#define CSlmbrtS (void (*)(struct cs_Csprm_ *))0
#define CStrmerS (void (*)(struct cs_Csprm_ *))0
#define CSalberS (void (*)(struct cs_Csprm_ *))0
#define CSunityS (void (*)(struct cs_Csprm_ *))0
#define CSoblqmS (void (*)(struct cs_Csprm_ *))0
#define CSmrcatS (void (*)(struct cs_Csprm_ *))0
#define CSazmedS (void (*)(struct cs_Csprm_ *))0
#define CSlmtanS (void (*)(struct cs_Csprm_ *))0
#define CSplycnS (void (*)(struct cs_Csprm_ *))0
#define CSmodpcS (void (*)(struct cs_Csprm_ *))0
#define CSazmeaS (void (*)(struct cs_Csprm_ *))0
#define CSedcncS (void (*)(struct cs_Csprm_ *))0
#define CSmillrS (void (*)(struct cs_Csprm_ *))0
#define CSsstroS (void (*)(struct cs_Csprm_ *))0
#define CSostroS (void (*)(struct cs_Csprm_ *))0
#define CSpstroS (void (*)(struct cs_Csprm_ *))0
#define CSmstroS (void (*)(struct cs_Csprm_ *))0
#define CSnzlndS (void (*)(struct cs_Csprm_ *))0
#define CSsinusS (void (*)(struct cs_Csprm_ *))0
#define CSorthoS (void (*)(struct cs_Csprm_ *))0
#define CSgnomcS (void (*)(struct cs_Csprm_ *))0
#define CSedcylS (void (*)(struct cs_Csprm_ *))0
#define CSvdgrnS (void (*)(struct cs_Csprm_ *))0
#define CScsiniS (void (*)(struct cs_Csprm_ *))0
#define CSrobinS (void (*)(struct cs_Csprm_ *))0
#define CSbonneS (void (*)(struct cs_Csprm_ *))0
#define CSekrt4S (void (*)(struct cs_Csprm_ *))0
#define CSekrt6S (void (*)(struct cs_Csprm_ *))0
#define CSmolwdS (void (*)(struct cs_Csprm_ *))0
#define CShmlsnS (void (*)(struct cs_Csprm_ *))0
#define CSnacylS (void (*)(struct cs_Csprm_ *))0
#define CStacylS (void (*)(struct cs_Csprm_ *))0
#define CSbpcncS (void (*)(struct cs_Csprm_ *))0
#define CSswissS (void (*)(struct cs_Csprm_ *))0
#define CStrmrsS (void (*)(struct cs_Csprm_ *))0
#define CSoccncS (void (*)(struct cs_Csprm_ *))0
#define CSkrovkS (void (*)(struct cs_Csprm_ *))0
#define CSnerthS (void (*)(struct cs_Csprm_ *))0
#define CSsys34S (void (*)(struct cs_Csprm_ *))0
#define CSwinklS (void (*)(struct cs_Csprm_ *))0
#endif

#ifdef __NO_QCHK__
#define CSlmbrtQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CStrmerQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSalberQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSunityQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSoblqmQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSmrcatQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSazmedQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSlmtanQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSplycnQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSmodpcQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSazmeaQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSedcncQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSmillrQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSpstroQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSsstroQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSostroQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSmstroQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSnzlndQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSsinusQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSorthoQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSgnomcQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSedcylQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSvdgrnQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CScsiniQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSrobinQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSbonneQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSekrt4Q (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSekrt6Q (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSmolwdQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CShmlsnQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSnacylQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CStacylQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSbpcncQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSswissQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CStrmrsQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSoccncQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSkrovkQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSnerthQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSsys34Q (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#define CSwinklQ (int (*)(struct cs_Csdef_ *,unsigned short,int [],int))0
#endif

#define cs_PRJFLGS_UNITY	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | cs_PRJFLG_CNVRG | \
							 cs_PRJFLG_GEOGR  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGFLS | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_LM2SP	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_LM1SP	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_LMBLG	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_WCCSL	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_MNDOTL	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_TRMER	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_TRMRKRG	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_TRMRS	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_SOTRM	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_UTM		(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_ORGLNG | cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGFLS | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_WCCST	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_MNDOTT	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_ALBER	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | \
							 cs_PRJFLG_EAREA  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_HOM1UV	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_OBLQ | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_HOM1XY	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_OBLQ | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_HOM2UV	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_OBLQ   | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_HOM2XY	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_OBLQ   | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_RSKEW	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_OBLQ   | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_RSKEWC	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_OBLQ   | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_MRCAT	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_MRCATK	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_AZMED	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | \
							 cs_PRJFLG_EDIST  | cs_PRJFLG_AZMTH  | \
							 cs_PRJFLG_FLAT   | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_LMTAN	(cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_PLYCN	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_EDIST  | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_MODPC	(cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_ORGLNG | cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_AZMEA	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | \
							 cs_PRJFLG_EAREA  | cs_PRJFLG_AZMTH  | \
							 cs_PRJFLG_FLAT   | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_EDCNC	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | \
							 cs_PRJFLG_EDIST  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_MILLR	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | cs_PRJFLG_CNVRG | \
							 cs_PRJFLG_CYLND  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_SSTRO	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | cs_PRJFLG_AZMTH  | \
							 cs_PRJFLG_FLAT   | \
							 cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_OSTRO	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | cs_PRJFLG_AZMTH  | \
							 cs_PRJFLG_FLAT   | \
							 cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_PSTRO	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | cs_PRJFLG_AZMTH  | \
							 cs_PRJFLG_FLAT   | \
							 cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_PSTROSL	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | cs_PRJFLG_AZMTH  | \
							 cs_PRJFLG_FLAT)

#define cs_PRJFLGS_MSTRO	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_FLAT   | \
                             cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_NZLND 	(cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_FLAT   | \
                             cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_SINUS	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | cs_PRJFLG_CNVRG | \
							 cs_PRJFLG_EAREA  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_PSEUDO | \
							 cs_PRJFLG_INTR   | \
							 cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_ORTHO	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | \
							 cs_PRJFLG_AZMTH  | \
							 cs_PRJFLG_FLAT   | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_GNOMC	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | \
							 cs_PRJFLG_AZMTH  | \
							 cs_PRJFLG_FLAT   | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_EDCYL	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | cs_PRJFLG_CNVRG | \
							 cs_PRJFLG_EDIST  | \
							 cs_PRJFLG_CYLND  | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_VDGRN	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_OTHER  | \
							 cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_CSINI	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_ROBIN	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_PSEUDO | \
							 cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_BONNE	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_EAREA  | \
							 cs_PRJFLG_CONIC  | cs_PRJFLG_PSEUDO | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_EKRT4	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_EAREA  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_PSEUDO | \
							 cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_EKRT6	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_EAREA  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_PSEUDO | \
							 cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_MOLWD	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_EAREA  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_PSEUDO | \
							 cs_PRJFLG_INTR   | \
							 cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_HMLSN	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_EAREA  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_PSEUDO | \
							 cs_PRJFLG_INTR   | \
							 cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_NACYL	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_EAREA  | \
							 cs_PRJFLG_CYLND  | \
							 cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_TACYL	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_EAREA  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_BPCNC	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_SWISS	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_OBLQ | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_GAUSSK	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_KROVAK	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | cs_PRJFLG_OBLQ | \
							 cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_KRVK95	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | cs_PRJFLG_OBLQ   | \
							 cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_TRMERAF	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_AFFINE | \
							 cs_PRJFLG_TRNSV  | cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_NERTH	(cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | cs_PRJFLG_OTHER  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_OBQCYL	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_CNFRM  | cs_PRJFLG_SCLRED | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_OBLQ | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_SYS34	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_ORGFLS | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_OSTN97	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_ORGFLS)

#define cs_PRJFLGS_AZEDE	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | \
							 cs_PRJFLG_EDIST  | cs_PRJFLG_AZMTH  | \
							 cs_PRJFLG_FLAT   | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_OSTN02	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_ORGFLS)

#define cs_PRJFLGS_SYS34_99	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_ORGFLS | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_RSKEWO	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_OBLQ | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_WINKL	(cs_PRJFLG_SPHERE | \
							 cs_PRJFLG_AZMTH  | \
							 cs_PRJFLG_FLAT   | cs_PRJFLG_PSEUDO | \
							 cs_PRJFLG_ORGLAT | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_NRTHSRT	(cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | cs_PRJFLG_OTHER  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_LMBRTAF	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | cs_PRJFLG_AFFINE | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_SYS34_01	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_TRNSV  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_ORGFLS | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_EDCYLE	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | cs_PRJFLG_CNVRG | \
							 cs_PRJFLG_EDIST  | \
							 cs_PRJFLG_CYLND  | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_PCARREE	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | cs_PRJFLG_CNVRG | \
							 cs_PRJFLG_EDIST  | \
							 cs_PRJFLG_CYLND  | \
							 cs_PRJFLG_RNTRNT)

#define cs_PRJFLGS_MRCATPV	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_SCALH  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CYLND  | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_RNTRNT)
							 
#define cs_PRJFLGS_LMMICH	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | \
							 cs_PRJFLG_RNTRNT)
/*
	Obsolete definitions; preserved for historical reasons only.

#define cs_PRJFLGS_OBLQM	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CYLND  | cs_PRJFLG_OBLQ | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLAT)

#define cs_PRJFLGS_KROVK1	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | cs_PRJFLG_OBLQ   | \
							 cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGFLS)

#define cs_PRJFLGS_KRVK951	(cs_PRJFLG_SPHERE | cs_PRJFLG_ELLIPS | \
							 cs_PRJFLG_SCALK  | cs_PRJFLG_CNVRG  | \
							 cs_PRJFLG_CNFRM  | \
							 cs_PRJFLG_CONIC  | cs_PRJFLG_OBLQ   | \
							 cs_PRJFLG_SCLRED | cs_PRJFLG_ORGLNG | \
							 cs_PRJFLG_ORGLAT | cs_PRJFLG_ORGFLS)
*/

/*
	The following table is searched linearly. Thus, performance is
	best when the most frequently accessed projections are near the
	top of the table. This affect is very, very, small however.
*/
/*					          1         2         3         4         5         6
					 123456789012345678901234567890123456789012345678901234567890123*/

struct cs_Prjtab_ cs_Prjtab [] =
{
	{         "LL",CSunityS,CSunityQ,cs_PRJCOD_UNITY,cs_PRJFLGS_UNITY,0UL,
				"Null Projection, produces/processes Latitude & Longitude"},
	{         "LM",CSlmbrtS,CSlmbrtQ,cs_PRJCOD_LM2SP,cs_PRJFLGS_LM2SP,9802UL,
	    			"Lambert Conformal Conic Projection, Two Standard Parallels"},
	{      "LM1SP",CSlmbrtS,CSlmbrtQ,cs_PRJCOD_LM1SP,cs_PRJFLGS_LM1SP,9801UL,
	    			"Lambert Conformal Conic Projection, One Standard Parallel"},
	{     "LMBLGN",CSlmbrtS,CSlmbrtQ,cs_PRJCOD_LMBLG,cs_PRJFLGS_LMBLG,9803UL,
	    			"Lambert Conformal Conic Projection, Belgian Variation"},
	{    "LM-WCCS",CSlmbrtS,CSlmbrtQ,cs_PRJCOD_WCCSL,cs_PRJFLGS_WCCSL,0UL,
	    			"Lambert Conformal Conic, Wisconsin County Variation"},
	{   "LM-MNDOT",CSlmbrtS,CSlmbrtQ,cs_PRJCOD_MNDOTL,cs_PRJFLGS_MNDOTL,0UL,
	    			"Lambert Conformal Conic, Minnesota DOT Variation"},
	{         "TM",CStrmerS,CStrmerQ,cs_PRJCOD_TRMER,cs_PRJFLGS_TRMER,9807UL,
	    			"Transverse Mercator or Gauss Kruger Projection"},
	{        "UTM",CStrmerS,CStrmerQ,cs_PRJCOD_UTM,cs_PRJFLGS_UTM,0UL,
	    			"Universal Transverse Mercator System"},
	{     "GAUSSK",CStrmerS,CStrmerQ,cs_PRJCOD_GAUSSK,cs_PRJFLGS_GAUSSK,9807UL,
	    			"Gauss Kruger Projection, aka Gauss"},
	{       "SOTM",CStrmerS,CStrmerQ,cs_PRJCOD_SOTRM,cs_PRJFLGS_SOTRM,9808UL,
	    			"South Oriented Transverse Mercator"},
	{    "TM-WCCS",CStrmerS,CStrmerQ,cs_PRJCOD_WCCST,cs_PRJFLGS_WCCST,0UL,
	    			"Transverse Mercator, Wisconsin County Variation"},
	{   "TM-MNDOT",CStrmerS,CStrmerQ,cs_PRJCOD_MNDOTT,cs_PRJFLGS_MNDOTT,0UL,
	    			"Transverse Mercator, Minnesota DOT Variation"},
	{  "TM-SNYDER",CStrmrsS,CStrmrsQ,cs_PRJCOD_TRMRS,cs_PRJFLGS_TRMRS,9807UL,
	    			"Transverse Mercator per J. P. Snyder"},
	{         "AE",CSalberS,CSalberQ,cs_PRJCOD_ALBER,cs_PRJFLGS_ALBER,9822UL,
	    			"Albers Equal Area Conic Projection"},
	{     "HOM1UV",CSoblqmS,CSoblqmQ,cs_PRJCOD_HOM1UV,cs_PRJFLGS_HOM1UV,0UL,
	    			"Unrectified Hotine Oblique Mercator Projection"},
	{     "HOM1XY",CSoblqmS,CSoblqmQ,cs_PRJCOD_HOM1XY,cs_PRJFLGS_HOM1XY,9812UL,
	    			"Alaska Variation of Hotine Oblique Mercator Projection"},
	{     "HOM2UV",CSoblqmS,CSoblqmQ,cs_PRJCOD_HOM2UV,cs_PRJFLGS_HOM2UV,0UL,
	    			"Unrectified Hotine Oblique Mercator Projection, Two Point Form"},
	{     "HOM2XY",CSoblqmS,CSoblqmQ,cs_PRJCOD_HOM2XY,cs_PRJFLGS_HOM2XY,0UL,
	    			"Rectified Skew Orthomorphic, Two Point Form"},
	{      "RSKEW",CSoblqmS,CSoblqmQ,cs_PRJCOD_RSKEW,cs_PRJFLGS_RSKEW,9812UL,
	    			"Rectified Skew Orthomorphic, Azimuth at Projection Center"},
	{     "RSKEWC",CSoblqmS,CSoblqmQ,cs_PRJCOD_RSKEWC,cs_PRJFLGS_RSKEWC,9815UL,
	    			"Rectified Skew Orthomorphic, Origin & Azimuth at Center"},
	{      "MRCAT",CSmrcatS,CSmrcatQ,cs_PRJCOD_MRCAT,cs_PRJFLGS_MRCAT,9805UL,
	    			"Mercator Cylindrical Projection with Standard Parallel"},
	{     "MRCATK",CSmrcatS,CSmrcatQ,cs_PRJCOD_MRCATK,cs_PRJFLGS_MRCATK,9804UL,
	    			"Mercator Cylindrical Projection with Scale Reduction"},
	{      "AZMED",CSazmedS,CSazmedQ,cs_PRJCOD_AZMED,cs_PRJFLGS_AZMED,0UL,
	    			"Lambert Azimuthal Equidistant Projection"},
	{      "AZMEA",CSazmeaS,CSazmeaQ,cs_PRJCOD_AZMEA,cs_PRJFLGS_AZMEA,9820UL,
	    			"Lambert Azimuthal Equal Area Projection"},
	{      "LMTAN",CSlmtanS,CSlmtanQ,cs_PRJCOD_LMTAN,cs_PRJFLGS_LMTAN,9801UL,
	    			"Lambert Tangential Conformal Conic Projection"},
	{      "PLYCN",CSplycnS,CSplycnQ,cs_PRJCOD_PLYCN,cs_PRJFLGS_PLYCN,9818UL,
	    			"Hassler American Polyconic Projection"},
	{      "MODPC",CSmodpcS,CSmodpcQ,cs_PRJCOD_MODPC,cs_PRJFLGS_MODPC,0UL,
	    			"Lallemand IMW Modified Polyconic Projection"},
	{      "EDCNC",CSedcncS,CSedcncQ,cs_PRJCOD_EDCNC,cs_PRJFLGS_EDCNC,0UL,
	    			"Equidistant Conic Projection, aka Simple Conic"},
	{     "MILLER",CSmillrS,CSmillrQ,cs_PRJCOD_MILLR,cs_PRJFLGS_MILLR,0UL,
	    			"Miller Cylindrical Projection"},
	{   "OSTEROUS",CSsstroS,CSsstroQ,cs_PRJCOD_SSTRO,cs_PRJFLGS_SSTRO,0UL,
	    			"Oblique Sterographic Projection, per Snyder (USA)"},
	{     "OSTERO",CSostroS,CSostroQ,cs_PRJCOD_OSTRO,cs_PRJFLGS_OSTRO,9809UL,
	    			"Oblique Sterographic Projection"},
	{     "PSTERO",CSpstroS,CSpstroQ,cs_PRJCOD_PSTRO,cs_PRJFLGS_PSTRO,9810UL,
	    			"Polar Sterographic Projection"},
	{   "PSTEROSL",CSpstroS,CSpstroQ,cs_PRJCOD_PSTROSL,cs_PRJFLGS_PSTROSL,9829UL,
	    			"Polar Sterographic Projection with Standard Circle"},
	{      "SINUS",CSsinusS,CSsinusQ,cs_PRJCOD_SINUS,cs_PRJFLGS_SINUS,0UL,
	    			"Sinusoidal Projection, Optionally Interrupted"},
	{      "ORTHO",CSorthoS,CSorthoQ,cs_PRJCOD_ORTHO,cs_PRJFLGS_ORTHO,0UL,
	    			"Orthographic Projection"},
	{   "GNOMONIC",CSgnomcS,CSgnomcQ,cs_PRJCOD_GNOMC,cs_PRJFLGS_GNOMC,0UL,
	    			"Gnomonic Projection"},
	{      "EDCYL",CSedcylS,CSedcylQ,cs_PRJCOD_EDCYL,cs_PRJFLGS_EDCYL,1029UL,
	    			"Equidistant Cylindrical Projection (Deprecated: Spherical Only)"},
	{    "VDGRNTN",CSvdgrnS,CSvdgrnQ,cs_PRJCOD_VDGRN,cs_PRJFLGS_VDGRN,0UL,
	    			"Van Der Grinten Projection"},
	{    "CASSINI",CScsiniS,CScsiniQ,cs_PRJCOD_CSINI,cs_PRJFLGS_CSINI,9806UL,
	    			"Cassini Projection"},
	{     "MSTERO",CSmstroS,CSmstroQ,cs_PRJCOD_MSTRO,cs_PRJFLGS_MSTRO,0UL,
	    			"Modified Stereographic Projection"},
	{   "NZEALAND",CSnzlndS,CSnzlndQ,cs_PRJCOD_NZLND,cs_PRJFLGS_NZLND,9811UL,
	    			"New Zealand National Grid"},
	{   "ROBINSON",CSrobinS,CSrobinQ,cs_PRJCOD_ROBIN,cs_PRJFLGS_ROBIN,0UL,
	    			"Robinson Cylindrical Projection"},
	{      "BONNE",CSbonneS,CSbonneQ,cs_PRJCOD_BONNE,cs_PRJFLGS_BONNE,9827UL,
	    			"Bonne Pseudoconical Projection"},
	{    "ECKERT4",CSekrt4S,CSekrt4Q,cs_PRJCOD_EKRT4,cs_PRJFLGS_EKRT4,0UL,
	    			"Ekert Pseudocylindrical Projection, Number IV"},
	{    "ECKERT6",CSekrt6S,CSekrt6Q,cs_PRJCOD_EKRT6,cs_PRJFLGS_EKRT6,0UL,
	    			"Ekert Pseudocylindrical Projection, Number VI"},
	{   "MOLLWEID",CSmolwdS,CSmolwdQ,cs_PRJCOD_MOLWD,cs_PRJFLGS_MOLWD,0UL,
	    			"Mollweide Projection"},
	{      "GOODE",CShmlsnS,CShmlsnQ,cs_PRJCOD_HMLSN,cs_PRJFLGS_HMLSN,0UL,
	    			"Goode Homolosine Projection"},
	{     "NEACYL",CSnacylS,CSnacylQ,cs_PRJCOD_NACYL,cs_PRJFLGS_NACYL,0UL,
	    			"Normal Aspect, Equal Area Cylindrical Projection"},
	{     "TEACYL",CStacylS,CStacylQ,cs_PRJCOD_TACYL,cs_PRJFLGS_TACYL,0UL,
	    			"Transverse Aspect, Equal Area Cylindrical Projection"},
	{    "BIPOLAR",CSbpcncS,CSbpcncQ,cs_PRJCOD_BPCNC,cs_PRJFLGS_BPCNC,0UL,
	    			"Bipolar Oblique Conformal Conic Projection"},
	{      "SWISS",CSswissS,CSswissQ,cs_PRJCOD_SWISS,cs_PRJFLGS_SWISS,9814UL,
	    			"Swiss Oblique Cylindrical Projection"},
	{     "KROVAK",CSkrovkS,CSkrovkQ,cs_PRJCOD_KROVAK,cs_PRJFLGS_KROVAK,9819UL,
	    			"Krovak Oblique Conformal Conic, Czechoslovokia"},
	{   "KROVAK95",CSkrovkS,CSkrovkQ,cs_PRJCOD_KRVK95,cs_PRJFLGS_KRVK95,0UL,
	    			"Krovak Oblique Conformal Conic/95 Adjustment"},
	{       "TMAF",CStrmerS,CStrmerQ,cs_PRJCOD_TRMERAF,cs_PRJFLGS_TRMERAF,0UL,
	    			"Transverse Mercator (Gauss/Kruger) with Affine Post Process"},
	{      "NERTH",CSnerthS,CSnerthQ,cs_PRJCOD_NERTH,cs_PRJFLGS_NERTH,0UL,
	    			"Non-georeferenced (aka non-earth) coordinate system"},
	{     "OBQCYL",CSswissS,CSswissQ,cs_PRJCOD_OBQCYL,cs_PRJFLGS_OBQCYL,0UL,
	    			"Oblique Cylindrical Projection (Generalized)"},
	{   "SYSTM34",CSsys34S,CSsys34Q,cs_PRJCOD_SYS34,cs_PRJFLGS_SYS34,0UL,
	    			"Danish System 34, UTM + polynomials (pre-1999 vintage)"},
	{    "OSTN97",CStrmerS,CStrmerQ,cs_PRJCOD_OSTN97,cs_PRJFLGS_OSTN97,0UL,
	    			"Ordnance Survey National Grid Transformation of 1997"},
	{"AZMED-ELEV",CSazmedS,CSazmedQ,cs_PRJCOD_AZEDE,cs_PRJFLGS_AZEDE,0UL,
	    			"Lambert Azimuthal Equidistant, Elevated Ellipsoid"},
	{    "OSTN02",CStrmerS,CStrmerQ,cs_PRJCOD_OSTN02,cs_PRJFLGS_OSTN02,0UL,
	    			"Ordnance Survey National Grid Transformation of 2002"},
	{"SYSTM34-99",CSsys34S,CSsys34Q,cs_PRJCOD_SYS34_99,cs_PRJFLGS_SYS34_99,0UL,
	    			"Danish System 34, UTM + polynomials (1999 vintage)"},
	{     "RSKEWO",CSoblqmS,CSoblqmQ,cs_PRJCOD_RSKEWO,cs_PRJFLGS_RSKEWO,9815UL,
					"Rectified Skew Orthomorphic, Skew Azimuth at Rectified Origin"},
	{    "TRMRKRG",CStrmerS,CStrmerQ,cs_PRJCOD_TRMRKRG,cs_PRJFLGS_TRMRKRG,0UL,
	    			"Transverse Mercator using Kruger Formulation"},
	{     "WINKEL",CSwinklS,CSwinklQ,cs_PRJCOD_WINKL,cs_PRJFLGS_WINKL,0UL,
					"Winkel-Tripel, variable standard latitude" },
	{  "NERTH-SRT",CSnerthS,CSnerthQ,cs_PRJCOD_NRTHSRT,cs_PRJFLGS_NRTHSRT,0UL,
	    			"Non-georeferenced coordinate system with Scale & Rotate"},
	{       "LMAF",CSlmbrtS,CSlmbrtQ,cs_PRJCOD_LMBRTAF,cs_PRJFLGS_LMBRTAF,0UL,
	    			"Lambert Conformal Conic (2SP) with Affine Post Process"},
	{"SYSTM34-01",CSsys34S,CSsys34Q,cs_PRJCOD_SYS34_01,cs_PRJFLGS_SYS34_01,0UL,
	    			"Danish System 34, UTM + polynomials (2001 vintage)"},
	{    "EDCYL-E",CSedcylS,CSedcylQ,cs_PRJCOD_EDCYLE,cs_PRJFLGS_EDCYLE,1028UL,
	    			"Equidistant Cylindrical Projection (Ellipsoidal or Spherical)"},
	{   "PCARREE",CSedcylS,CSedcylQ,cs_PRJCOD_PCARREE,cs_PRJFLGS_PCARREE,0UL,
	    			"Plate Carree / Simple Cylindrical"},
	{  "MRCAT-PV",CSmrcatS,CSmrcatQ,cs_PRJCOD_MRCATPV,cs_PRJFLGS_MRCATPV,1024UL,
	    			"Popular Visualisation Pseudo Mercator"},
	{   "LM-MICH",CSlmbrtS,CSlmbrtQ,cs_PRJCOD_LMMICH,cs_PRJFLGS_LMMICH,1051UL,
	    			"Lambert Conformal Conic Projection, Two Standard Parallels"},
	{           "",NULL,    NULL,    cs_PRJCOD_END,  0L,  0UL,
					""}	/* End of table marker. */
};

/*
	The following table defines the various parameter types.
*/

#define cs_FRMT_LNG		(cs_ATOF_VALLNG | cs_ATOF_MINSEC  | cs_ATOF_DIRCHR | \
						 cs_ATOF_XEAST  | cs_ATOF_MINSEC0 | 5L)
#define cs_FRMT_LAT		(cs_ATOF_VALLAT | cs_ATOF_MINSEC  | cs_ATOF_DIRCHR | \
						 cs_ATOF_MINSEC0 | 5L)
#define cs_FRMT_AZM		(cs_ATOF_VALLNG | cs_ATOF_MINSEC  | cs_ATOF_DIRCHR | \
						 cs_ATOF_XEAST  | cs_ATOF_MINSEC0 | 5L)
#define cs_FRMT_ANGD	(cs_ATOF_VALLNG | cs_ATOF_MINSEC  | cs_ATOF_MINSEC0 | 5L)
#define cs_FRMT_XXX		(cs_ATOF_COMMA  | 3L)
#define cs_FRMT_YYY		(cs_ATOF_COMMA  | 3L)
#define cs_FRMT_SCL		(7L)
#define cs_FRMT_ROT		(4L)
#define cs_FRMT_COEF	(13L)

struct cs_Prjprm_ csPrjprm [] =
{
	/* Index = 0 Dummy entry, thus allowing use of zero index value to mean no
	   entry. */
	{
									   0.0,		/* Minimum value. */
									   0.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
										0L,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
			"Not Used for this projection",		/* Label */
						   cs_PRMPTYP_NONE,		/* Physical Type */
						   cs_PRMLTYP_NONE,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 1; Central Meridian, cs_PRMCOD_CNTMER */
	{
									-180.0,		/* Minimum value. */
									 180.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LNG,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
						"Central Meridian",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LNG,		/* Logical Type */
									8802UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 2; Northern Standard Parallel, cs_PRMCOD_NSTDPLL */
	{
									 -90.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
			  "Northern Standard Parallel",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									8823UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 3; Southern Standard Parallel, cs_PRMCOD_SSTDPLL */
	{
									 -90.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
			  "Southern Standard Parallel",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									8824UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 4; Standard Parallel, cs_PRMCOD_STDPLL */
	{
									 -90.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
					   "Standard Parallel",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									8832UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 5; First Point Longitude, cs_PRMCOD_GCP1LNG */
	{
									-180.0,		/* Minimum value. */
									 180.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LNG,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				   "First Point Longitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LNG,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 6; First Point Latitude, cs_PRMCOD_GCP1LAT */
	{
									 -90.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
					"First Point Latitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 7; Second Point Longitude, cs_PRMCOD_GCP2LNG */
	{
									-180.0,		/* Minimum value. */
									 180.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LNG,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				  "Second Point Longitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LNG,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 8; Second Point Latitude, cs_PRMCOD_GCP2LAT */
	{
									 -90.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				   "Second Point Latitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 9; Cntral Point Longitude, cs_PRMCOD_GCPLNG */
	{
									-180.0,		/* Minimum value. */
									 180.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LNG,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				 "Central Point Longitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LNG,		/* Logical Type */
									8812UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 10; Central Point Latitude, cs_PRMCOD_GCPLAT */
	{
									 -90.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				  "Central Point Latitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									8811UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 11; Geodesic Azimuth at Prj Center, cs_PRMCOD_GCAZM */
	{
									-180.0,		/* Minimum value. */
									 360.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_AZM,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
   "Geodesic Azimuth at Projection Center",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_AZM,		/* Logical Type */
									8813UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 12; Y Axis Azimuth, cs_PRMCOD_YAXISAZ */
	{
									 -90.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_AZM,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
						  "Y Axis Azimuth",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_AZM,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 13; Eastern Standard Merdian, cs_PRMCOD_ESTDMER */
	{
									-180.0,		/* Minimum value. */
									 180.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LNG,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				"Eastern Standard Meridian",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LNG,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 14; Northern Parallel, cs_PRMCOD_NPARALL */
	{
									 -88.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
					   "Northern Parallel",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 15; Southern Parallel, cs_PRMCOD_SPARALL */
	{
									 -90.0,		/* Minimum value. */
									  88.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
					   "Southern Parallel",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 16; First Pole Longitude, cs_PRMCOD_P1LNG */
	{
									-180.0,		/* Minimum value. */
									 180.0,		/* Maximum value. */
									-110.0,		/* Suitable default. */
							   cs_FRMT_LNG,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
					"First Pole Longitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LNG,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 17; First Pole Latitude, cs_PRMCOD_P1LAT */
	{
									 -89.0,		/* Minimum value. */
									  89.0,		/* Maximum value. */
									 -20.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
					 "First Pole Latitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 18; Second Pole Longitude, cs_PRMCOD_P2LNG */
	{
									-180.0,		/* Minimum value. */
									 180.0,		/* Maximum value. */
									 -45.0,		/* Suitable default. */
							   cs_FRMT_LNG,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				   "Second Pole Longitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LNG,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 19; Second Pole Latitude, cs_PRMCOD_P2LAT */
	{
									 -89.0,		/* Minimum value. */
									  89.0,		/* Maximum value. */
							-19.9933333333,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
					"Second Pole Latitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 20; Distance Between Poles, cs_PRMCOD_ADP1P2 */
	{
									   0.0,		/* Minimum value. */
									 170.0,		/* Maximum value. */
									 104.0,		/* Suitable default. */
							  cs_FRMT_ANGD,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				  "Distance Between Poles",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						   cs_PRMLTYP_ANGD,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 21; Distance to first Standard Parallel, cs_PRMCOD_ADSP1 */
	{
									   0.0,		/* Minimum value. */
									  89.0,		/* Maximum value. */
									  31.0,		/* Suitable default. */
							  cs_FRMT_ANGD,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
	 "Distance to First Standard Parallel",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						   cs_PRMLTYP_ANGD,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 22; Distance to second Standard Parallel, cs_PRMCOD_ADSP2 */
	{
									   0.0,		/* Minimum value. */
									 179.0,		/* Maximum value. */
									  71.0,		/* Suitable default. */
							  cs_FRMT_ANGD,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
	"Distance to Second Standard Parallel",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						   cs_PRMLTYP_ANGD,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 23; Complex Coefficient A, cs_PRMCOD_CMPLXAN */
	{
									  -1.0,		/* Minimum value. */
									   1.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							  cs_FRMT_COEF,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
		  "Complex Series Coefficient A%d",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						 cs_PRMLTYP_CMPLXC,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 1		/* Sprintf Processing */
	},

	/* Index = 24; Complex Coefficient B, cs_PRMCOD_CMPLXBN */
	{
									  -1.0,		/* Minimum value. */
									   1.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							  cs_FRMT_COEF,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
		  "Complex Series Coefficient B%d",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						 cs_PRMLTYP_CMPLXC,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 1		/* Sprintf Processing */
	},

	/* Index = 25; Western End of Longitude Range, cs_PRMCOD_WESTLL */
	{
									-360.0,		/* Minimum value. */
									 180.0,		/* Maximum value. */
									-180.0,		/* Suitable default. */
							   cs_FRMT_LNG,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
		  "Western End of Longitude Range",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LNG,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 26; Eastern End of Longitude Range, cs_PRMCOD_EASTLL */
	{
									   0.0,		/* Minimum value. */
									 720.0,		/* Maximum value. */
									 180.0,		/* Suitable default. */
							   cs_FRMT_LNG,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
		  "Eastern End of Longitude Range",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LNG,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 27; UTM Zone number, cs_PRMCOD_UTMZN */
	{
									   1.0,		/* Minimum value. */
									  60.0,		/* Maximum value. */
									  30.0,		/* Suitable default. */
										 0,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				"UTM Zone Number (1 - 60)",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						   cs_PRMLTYP_ZNBR,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 28; North South Hemisphre, cs_PRMCOD_HSNS */
	{
									  -1.0,		/* Minimum value. */
									   1.0,		/* Maximum value. */
									   1.0,		/* Suitable default. */
										 0,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
			  "Hemisphere, North or South",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						   cs_PRMLTYP_HSNS,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 29; Average Geoid Separation, cs_PRMCOD_GHGT */
	{
									-400.0,		/* Minimum value. */
									 400.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
										4L,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
		   "Average Geoid Height (meters)",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						   cs_PRMLTYP_GHGT,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 30; Average Elevation, cs_PRMCOD_AELEV */
	{
								   -5000.0,		/* Minimum value. */
								  100000.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
										4L,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
		 "Average Elevation (system unit)",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						   cs_PRMLTYP_ELEV,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 31; Oblique Pole Longitude, cs_PRMCOD_POLELNG */
	{
									-180.0,		/* Minimum value. */
									 180.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LNG,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				  "Oblique Pole Longitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LNG,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 32; Oblique Pole Latitude, cs_PRMCOD_POLELAT */
	{
									 -90.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									  40.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				   "Oblique Pole Latitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 33; Oblique Cone Standard Parallel, cs_PRMCOD_OSTDPLL */
	{
									 -89.0,		/* Minimum value. */
									  89.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
		  "Oblique Cone Standard Parallel",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									8818UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 34; Standard Circle Latitude, cs_PRMCOD_STDCIR */
	{
									 -90.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									  60.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				"Standard Circle Latitude",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 35; Affine A Coefficient, cs_PRMCOD_AFA0 */
	{
								-9.999E+10,		/* Minimum value. */
								 9.999E+10,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							  cs_FRMT_COEF,		/* CS_ftoa format */
										0L,		/* Help ID */
										4L,		/* String ID */
	"Affine Transformation A0 Coefficient",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						 cs_PRMLTYP_AFCOEF,		/* Logical Type */
									8623UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 36; Affine B0 Coefficient, cs_PRMCOD_AFB0 */
	{
								-9.999E+10,		/* Minimum value. */
								 9.999E+10,		/* Maximum value. */
									   0.0,		/* Suitable default. */
										4L,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
	"Affine Transformation B0 Coefficient",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						 cs_PRMLTYP_AFCOEF,		/* Logical Type */
									8639UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 37; Affine A1 Coefficient, cs_PRMCOD_AFA1 */
	{
								   -1000.0,		/* Minimum value. */
									1000.0,		/* Maximum value. */
									   1.0,		/* Suitable default. */
							  cs_FRMT_COEF,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
	"Affine Transformation A1 Coefficient",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						 cs_PRMLTYP_AFCOEF,		/* Logical Type */
									8624UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 38; Affine A2 Coefficient, cs_PRMCOD_AFA2 */
	{
								   -1000.0,		/* Minimum value. */
									1000.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							  cs_FRMT_COEF,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
	"Affine Transformation A2 Coefficient",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						 cs_PRMLTYP_AFCOEF,		/* Logical Type */
									8625UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 39; Affine B1 Coefficient, cs_PRMCOD_AFB1 */
	{
								   -1000.0,		/* Minimum value. */
									1000.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							  cs_FRMT_COEF,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
	"Affine Transformation B1 Coefficient",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						 cs_PRMLTYP_AFCOEF,		/* Logical Type */
									8640UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 40; Affine B2 Coefficient, cs_PRMCOD_AFB2 */
	{
								   -1000.0,		/* Minimum value. */
									1000.0,		/* Maximum value. */
									   1.0,		/* Suitable default. */
							  cs_FRMT_COEF,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
	"Affine Transformation B2 Coefficient",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						 cs_PRMLTYP_AFCOEF,		/* Logical Type */
									8641UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 41; Normal Parallel, cs_PRMCOD_NRMLPLL */
	{
									 -90.0,		/* Minimum value. */
									  90.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_LAT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
						 "Normal Parallel",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_LAT,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},

	/* Index = 42; Danish System 34 Region, cs_PRMCOD_DENRGN */
	{
									  1.00,		/* Minimum value. */
									  3.00,		/* Maximum value. */
									  1.00,		/* Suitable default. */
										2L,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
	 "Danish 34/45 Region (1=J, 2=S, 3=B)",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						   cs_PRMLTYP_ZNBR,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},
	/* Index = 43; Skew Azimuth at Projection Origin, cs_PRMCOD_SKWAZM */
	{
									-180.0,		/* Minimum value. */
									 360.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_AZM,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
	    "Skew Azimuth at Rectified Origin",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_AZM,		/* Logical Type */
									8814UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},
	
	/* Index = 44; Scale/Rotate Origin X, cs_PRMCOD_SCLROTORGX */
	{
								-9.999E+10,		/* Minimum value. */
								 9.999E+10,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_XXX,		/* CS_ftoa format */
										0L,		/* Help ID */
										4L,		/* String ID */
	 "X Coordinate of Scale/Rotate Origin",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						  cs_PRMLTYP_XYCRD,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},
	/* Index = 45; Scale/Rotate Origin Y, cs_PRMCOD_SCLROTORGY */
	{
								-9.999E+10,		/* Minimum value. */
								 9.999E+10,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_YYY,		/* CS_ftoa format */
										0L,		/* Help ID */
										4L,		/* String ID */
	 "Y Coordinate of Scale/Rotate Origin",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						  cs_PRMLTYP_XYCRD,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},
	/* Index = 46; Cartesian Scale Factor, cs_PRMCOD_NRTHSCL */
	{
								   1.0E-06,		/* Minimum value. */
								   1.0E+06,		/* Maximum value. */
									   1.0,		/* Suitable default. */
							   cs_FRMT_SCL,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				  "Cartesian Scale Factor",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						  cs_PRMLTYP_SCALE,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},
	/* Index = 47; Cartesian Rotation Angle, cs_PRMCOD_NRTHROT */
	{
									-180.0,		/* Minimum value. */
									 360.0,		/* Maximum value. */
									   0.0,		/* Suitable default. */
							   cs_FRMT_ROT,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				"Cartesian Rotation Angle",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
							cs_PRMLTYP_AZM,		/* Logical Type */
									   0UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	},
	/* Index = 48; Ellipsoid Scale Factor, cs_PRMCOD_ELPSCL */
	{
								   0.99909,		/* Minimum value, quite arbitrary in this case */
								   1.0001,		/* Maximum value, quite arbitrary in this case */
									   1.0,		/* Suitable default. */
							   cs_FRMT_SCL,		/* CS_ftoa format */
										0L,		/* Help ID */
										0L,		/* String ID */
				  "Ellipsoid Scale Factor",		/* Label */
							cs_PRMPTYP_DBL,		/* Physical Type */
						  cs_PRMLTYP_SCALE,		/* Logical Type */
								    1038UL,		/* EPSG parameter code */
										 0,		/* Projection Code */
										 0,		/* Parameter Number */
										 0		/* Sprintf Processing */
	}
};

/*
	The following table maps projection parameter types to
	specific projections.  Note, the primary index is the
	projection code (0 means none, so we have a dummy entry
	on the front), and the second index is the parameter
	number (first parameter = 0).

	Note, we rely on the compiler/linker to provide zeros for
	the uninitialized entries.
*/

/*lint -e785  too few initializers, refering to most all entries with less than 24 params */
struct cs_PrjprmMap_ cs_PrjprmMap [] =
{
	{ cs_PRJCOD_UNITY, { cs_PRMCOD_WESTLL, cs_PRMCOD_EASTLL }},
	{ cs_PRJCOD_LM2SP, { cs_PRMCOD_NSTDPLL,cs_PRMCOD_SSTDPLL}},
	{ cs_PRJCOD_LM1SP, { 0 }},
	{ cs_PRJCOD_LMBLG, { cs_PRMCOD_NSTDPLL,cs_PRMCOD_SSTDPLL}},
	{ cs_PRJCOD_WCCSL, { cs_PRMCOD_NSTDPLL,cs_PRMCOD_SSTDPLL,
						 cs_PRMCOD_GHGT,cs_PRMCOD_AELEV}},
	{cs_PRJCOD_MNDOTL, { cs_PRMCOD_NSTDPLL,cs_PRMCOD_SSTDPLL,
						 cs_PRMCOD_AELEV}},
	{ cs_PRJCOD_TRMER, { cs_PRMCOD_CNTMER }},
	{cs_PRJCOD_GAUSSK, { cs_PRMCOD_CNTMER }},
	{   cs_PRJCOD_UTM, { cs_PRMCOD_UTMZN, cs_PRMCOD_HSNS }},
	{ cs_PRJCOD_SOTRM, { cs_PRMCOD_CNTMER }},
	{ cs_PRJCOD_WCCST, { cs_PRMCOD_CNTMER, cs_PRMCOD_GHGT, cs_PRMCOD_AELEV }},
	{cs_PRJCOD_MNDOTT, { cs_PRMCOD_CNTMER, cs_PRMCOD_AELEV }},
	{ cs_PRJCOD_TRMRS, { cs_PRMCOD_CNTMER }},
	{ cs_PRJCOD_ALBER, { cs_PRMCOD_NSTDPLL,cs_PRMCOD_SSTDPLL }},
	{ cs_PRJCOD_OBLQM, { 0 }},	/* This is a historical artifact, should never occur anymore. */
	{ cs_PRJCOD_MRCAT, { cs_PRMCOD_CNTMER, cs_PRMCOD_STDPLL }},
	{cs_PRJCOD_MRCATK, { cs_PRMCOD_CNTMER}},
	{ cs_PRJCOD_AZMED, { cs_PRMCOD_YAXISAZ }},
	{ cs_PRJCOD_LMTAN, { 0 }},
	{ cs_PRJCOD_PLYCN, { cs_PRMCOD_CNTMER }},
	{ cs_PRJCOD_MODPC, { cs_PRMCOD_CNTMER,  cs_PRMCOD_ESTDMER,
						 cs_PRMCOD_NPARALL, cs_PRMCOD_SPARALL}},
	{ cs_PRJCOD_AZMEA, { cs_PRMCOD_YAXISAZ }},
	{ cs_PRJCOD_EDCNC, { cs_PRMCOD_NSTDPLL, cs_PRMCOD_SSTDPLL }},
	{ cs_PRJCOD_MILLR, { cs_PRMCOD_CNTMER }},
	{ cs_PRJCOD_SSTRO, { cs_PRMCOD_YAXISAZ }},
	{ cs_PRJCOD_OSTRO, { 0 }},
	{ cs_PRJCOD_PSTRO, { 0 }},
	{ cs_PRJCOD_MSTRO, { cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN,
						 cs_PRMCOD_CMPLXAN, cs_PRMCOD_CMPLXBN}},
	{ cs_PRJCOD_NZLND, { 0 }},
	{ cs_PRJCOD_SINUS, { 0 }},
	{ cs_PRJCOD_ORTHO, { 0 }},
	{ cs_PRJCOD_GNOMC, { 0 }},
	{ cs_PRJCOD_EDCYL, { cs_PRMCOD_STDPLL }},
	{ cs_PRJCOD_VDGRN, { 0 }},
	{ cs_PRJCOD_CSINI, { cs_PRMCOD_CNTMER }},
	{ cs_PRJCOD_ROBIN, { 0 }},
	{ cs_PRJCOD_BONNE, { 0 }},
	{ cs_PRJCOD_EKRT4, { 0 }},
	{ cs_PRJCOD_EKRT6, { 0 }},
	{ cs_PRJCOD_MOLWD, { 0 }},
	{ cs_PRJCOD_HMLSN, { 0 }},
	{ cs_PRJCOD_NACYL, { cs_PRMCOD_STDPLL }},
	{ cs_PRJCOD_TACYL, { 0 }},
	{ cs_PRJCOD_BPCNC, { cs_PRMCOD_P1LNG,  cs_PRMCOD_P1LAT,
						 cs_PRMCOD_P2LNG,  cs_PRMCOD_P2LAT,
						 cs_PRMCOD_ADP1P2,
						 cs_PRMCOD_ADSP1,  cs_PRMCOD_ADSP2}},
	{cs_PRJCOD_HOM1UV, { cs_PRMCOD_GCPLNG, cs_PRMCOD_GCPLAT, cs_PRMCOD_GCAZM }},
	{cs_PRJCOD_HOM1XY, { cs_PRMCOD_GCPLNG, cs_PRMCOD_GCPLAT, cs_PRMCOD_GCAZM }},
	{cs_PRJCOD_HOM2UV, { cs_PRMCOD_GCP1LNG, cs_PRMCOD_GCP1LAT,
						 cs_PRMCOD_GCP2LNG, cs_PRMCOD_GCP2LAT}},
	{cs_PRJCOD_HOM2XY, { cs_PRMCOD_GCP1LNG, cs_PRMCOD_GCP1LAT,
						 cs_PRMCOD_GCP2LNG, cs_PRMCOD_GCP2LAT}},
	{ cs_PRJCOD_RSKEW, { cs_PRMCOD_GCPLNG,  cs_PRMCOD_GCPLAT, cs_PRMCOD_GCAZM }},
	{cs_PRJCOD_RSKEWC, { cs_PRMCOD_GCPLNG,  cs_PRMCOD_GCPLAT, cs_PRMCOD_GCAZM }},
	{ cs_PRJCOD_SWISS, { 0 }},
	{cs_PRJCOD_KROVAK, { cs_PRMCOD_POLELNG,  cs_PRMCOD_POLELAT,  cs_PRMCOD_OSTDPLL }},
	{cs_PRJCOD_KRVK95, { cs_PRMCOD_POLELNG,  cs_PRMCOD_POLELAT,  cs_PRMCOD_OSTDPLL }},
   {cs_PRJCOD_PSTROSL, { cs_PRMCOD_STDCIR }},
   {cs_PRJCOD_TRMERAF, { cs_PRMCOD_CNTMER,  cs_PRMCOD_AFA0, cs_PRMCOD_AFB0,
						 cs_PRMCOD_AFA1, cs_PRMCOD_AFA2, cs_PRMCOD_AFB1,
						 cs_PRMCOD_AFB2}},
	{ cs_PRJCOD_NERTH, { 0 }},
	{cs_PRJCOD_OBQCYL, { cs_PRMCOD_NRMLPLL }},
	{ cs_PRJCOD_SYS34, { cs_PRMCOD_DENRGN }},
	{cs_PRJCOD_OSTN97, { 0 }},
	{ cs_PRJCOD_AZEDE, { cs_PRMCOD_YAXISAZ, cs_PRMCOD_AELEV }},
	{cs_PRJCOD_OSTN02, { 0 }},
  {cs_PRJCOD_SYS34_99, { cs_PRMCOD_DENRGN }},
	{cs_PRJCOD_RSKEWO, { cs_PRMCOD_GCPLNG,  cs_PRMCOD_GCPLAT, cs_PRMCOD_SKWAZM }},
   {cs_PRJCOD_TRMRKRG, { cs_PRMCOD_CNTMER }},
   {  cs_PRJCOD_WINKL, {  cs_PRMCOD_STDPLL }},
  { cs_PRJCOD_NRTHSRT, { cs_PRMCOD_NRTHSCL, cs_PRMCOD_NRTHROT, cs_PRMCOD_SCLROTORGX, cs_PRMCOD_SCLROTORGY }},
   {cs_PRJCOD_LMBRTAF, { cs_PRMCOD_NSTDPLL, cs_PRMCOD_SSTDPLL,  cs_PRMCOD_AFA0, cs_PRMCOD_AFB0,
						 cs_PRMCOD_AFA1, cs_PRMCOD_AFA2, cs_PRMCOD_AFB1, cs_PRMCOD_AFB2}},
  {cs_PRJCOD_SYS34_01, { cs_PRMCOD_DENRGN }},
   { cs_PRJCOD_EDCYLE, { cs_PRMCOD_STDPLL }},
  { cs_PRJCOD_PCARREE, { 0 }},
   {cs_PRJCOD_MRCATPV, { cs_PRMCOD_CNTMER}},
   { cs_PRJCOD_LMMICH, { cs_PRMCOD_NSTDPLL,cs_PRMCOD_SSTDPLL,cs_PRMCOD_ELPSCL}},

	{   cs_PRJCOD_END, { 0 }}
};
/*lint +e785 */
