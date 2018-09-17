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

/******************************************************************************
*******************************************************************************
**                                                                           **
**           Japanese ".PAR"  Grid  Iterpolation  Data  File                 **
**                                                                           **
*******************************************************************************
******************************************************************************/

/* The original Japanese data file is provided in text formm.  As a performance
   enhancement, we convert the ASCII text file to binary form upon the first
   access.  This conversion process uses the dates on the file to determine
   if a new binary version needs to be built.

   The following object is used to create, manage, and use a binary version
   of the Jgd2k data file.  We need a binary version to insulate ourselves
   from ths possibility of a variable length text file.  Also, it enables us
   to sort the data file.  There is no reason to believe that the text files
   will always be properly sorted for rapid access.

   Note, that unlike all the other grid data files, there is no guarantee that
   the coverage of any such file is rectangular.  Therefore, there is no way
   that the coverage scheme will work.  So, we are therefore limited to
   handling a single file at a time.  The coverage is determined by the
   coverage of that file.

   The binary file that we make consists of a number of the following records,
   with two doubles on the front.  The two doubles represent the
   southwesternmost meshcode in the file.  After the rather trivial header, the
   records appear in ascending order by mesh code.
   
   An attempt was made to make this file format generic, i.e. not specifically
   related to Japanese real estate.  Despite these efforts, there are two
   features of this format that makes the format rather specific to Japan:
   
   1> In generating mesh code, longitude are biased around the 100E meridian.
   2> The grid cell size is defined by the mesh encoding as being 45 seconds
      by 30 seconds.
*/
struct csJgd2kGridRecord_
{
	ulong32_t meshCode;
	long32_t deltaLat;				/* seconds * 100,000 */
	long32_t deltaLng;				/* seconds * 100,000 */
};
struct cs_Japan_
{
	struct csGridCoverage_ coverage;
								/* This is the rectangular min/max and
								   does _NOT_ imply that all real estate
								   inside the rectangle is covered.  The
								   implication is only that real estate
								   outside the rectangular region is
								   definitely not covered. */
	double ewDelta;				/* Grid cell size */
	double nsDelta;				/* Grid cell size */
	double errorValue;
	double cnvrgValue;
	csFILE* strm;				/* file is not opened until required,
								   may be closed if entire contents
								   have been buffered. */
	long32_t bufferSize;		/* Size of the I/O buffer in use. */
	void *dataBuffer;			/* not allocated until required, i.e.
								   file is actually opened. */
	struct csGridCell_ lngCell;	/* Last longitude cell actually used. */
	struct csGridCell_ latCell;	/* Last latitude cwll actaully used. */
	char filePath [MAXPATH];	/* Full path to data source file, used for
								   reporting purposes. */
	char fileName [32];			/* File name only, used for error reporting. */
	short maxIterations;
};

ulong32_t			EXP_LVL9	CSjpnLlToMeshCode (const double ll [2]);
void				EXP_LVL9	CSjpnMeshCodeToLl (double ll [2],ulong32_t meshCode);
int					EXP_LVL9	CScompareJgd2kGridRecord (const struct csJgd2kGridRecord_ *elem1,const struct csJgd2kGridRecord_ *elem2);
struct cs_Japan_*	EXP_LVL7	CSnewJgd2kGridFile (Const char *path,long32_t bufferSize,ulong32_t flags,double density);
void				EXP_LVL7	CSreleaseJgd2kGridFile (struct cs_Japan_ *__This);
void				EXP_LVL7	CSdeleteJgd2kGridFile (struct cs_Japan_ *__This);
int					EXP_LVL9	CSextractJgd2kGridFile (struct cs_Japan_ *__This,Const double* sourceLL);
int					EXP_LVL9	CScalcJgd2kGridFile (struct cs_Japan_* __This,double result [2],Const double* sourceLL);
int					EXP_LVL9	CSmakeBinaryJgd2kFile (struct cs_Japan_* __This);
