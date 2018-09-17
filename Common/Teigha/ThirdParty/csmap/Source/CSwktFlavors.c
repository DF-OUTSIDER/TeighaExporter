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

/*		*  *  *  *  R E M A R K S *  *  *  *  *

	Implements all of the flavor based behavior of WKT.  These functions
	hide all of this stuff behind a pretty generic interface.  This
	interface is valid in C or C++.
*/

#include "cs_map.h"
#include "cs_wkt.h"

long32_t CSepsgOpCodeMap (const char *wktName,enum ErcWktFlavor flavor)
{
	extern struct cs_EpsgOpMthMap_ cs_EpsgOpMthMap [];

	long32_t prjCode;
	long epsgOpCode;
	char* cPtr;
	char* dummy;
	const struct cs_EpsgOpMthMap_ *tblPtr;
	
	/* Kludge to handle the use of an EPSG Coordinate Operation Code as a
	   projection specification. */
	prjCode = 0L;
	cPtr = strstr (wktName,"(EPSG OP ");
	if (cPtr != 0)
	{
		epsgOpCode = strtol ((cPtr + 9),&dummy,10);
		if (epsgOpCode > 0L && epsgOpCode < 32768L)
		{
			tblPtr = cs_EpsgOpMthMap;
			while (tblPtr->epsgOpCodeLwr != 0)
			{
				if (epsgOpCode >= tblPtr->epsgOpCodeLwr && epsgOpCode <= tblPtr->epsgOpCodeUpr)
				{
					prjCode = tblPtr->csMapPrjCode;
					break;
				}
				tblPtr++;
			}
		}
	}
	return prjCode;
}

