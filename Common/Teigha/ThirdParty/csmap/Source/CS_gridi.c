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

/*lint -esym(613,err_list)  possible use of null pointer, but not really */

int EXP_LVL9 CSgridiQ (struct cs_GeodeticTransform_ *gxDef,unsigned short xfrmCode,
														   int err_list [],
														   int list_sz)
{
	extern char cs_Dir [];
	extern char* cs_DirP;
	extern struct cs_GridFormatTab_ cs_GridFormatTab [];

	short idx;
	short idxF;
	short fileCount;

	int err_cnt;
	int fErrCnt;
	int fErrList [4];
	int fListSz;
	int gxIndex;

	Const struct cs_GxIndex_* gxIdxPtr;
	struct cs_GridFormatTab_* frmtTblPtr;
	struct csGeodeticXfromParmsFile_* fileDefPtr;
	struct csGeodeticXformParmsGridFiles_* filesPtr;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;
	fListSz = 4;

	/* Need to perform a check on each file specification. */
	filesPtr = &gxDef->parameters.fileParameters;
	fileCount = filesPtr->fileReferenceCount;
	for (idx = 0;idx < fileCount;idx += 1)
	{
		fileDefPtr = &filesPtr->fileNames [idx];

		/* Locate the file format in the list table and call the file specific
		   quality checker. */
		for (frmtTblPtr = cs_GridFormatTab;frmtTblPtr->formatCode != cs_DTCFRMT_NONE;frmtTblPtr += 1)
		{
			if (frmtTblPtr->formatCode == fileDefPtr->fileFormat)
			{
				break;
			}
		}
		if (frmtTblPtr->formatCode == cs_DTCFRMT_NONE)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_INVDIR;
		}
		else
		{
			/* OK, we have a file specification and a valid format, thus we have
			   a setup function. */
			*cs_DirP = '\0';
			fErrCnt = (*frmtTblPtr->check)(fileDefPtr,cs_Dir,fErrList,fListSz);
			for (idxF = 0;idxF < fErrCnt;idxF += 1)
			{
				if (idxF < fListSz)
				{
					if (++err_cnt < list_sz) err_list [err_cnt] = fErrList [idxF];
				}
			}
		}

		/* Verify that the direction specification is valid. */
		fileDefPtr = &filesPtr->fileNames [idx];
		if (fileDefPtr->direction != 'F' && fileDefPtr->direction != 'f' &&
		    fileDefPtr->direction != 'I' && fileDefPtr->direction != 'i')
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_INVDIR;
		}
	}

	/* Verify the fallback specification. */
	if (filesPtr->fallback [0] != '\0')
	{
		/* Verify that the provided name is indeed an existing transformation
		   and that the type of the transformation is valid. */
		gxIndex = CS_locateGxByName (filesPtr->fallback);
		if (gxIndex < 0)
		{
			if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FBKNAME;
		}
		else
		{
			gxIdxPtr = CS_getGxIndexEntry ((unsigned)gxIndex);
			if (gxIdxPtr == 0)
			{
				/* This is really an internal software error, but this is a
				   "check" function which should not issue any standard
				   error conditions, only report them. */
				if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_ISERFBK;
			}
			else
			{
				if (((gxIdxPtr->methodCode & cs_DTCPRMTYP_MASK) != cs_DTCPRMTYP_STANDALONE) &&
					((gxIdxPtr->methodCode & cs_DTCPRMTYP_MASK) != cs_DTCPRMTYP_GEOCTR) &&
					((gxIdxPtr->methodCode & cs_DTCPRMTYP_MASK) != cs_DTCPRMTYP_PWRSRS))
				{
					if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_FBKMTH;
				}
			}
		}
	}

	/* Verify maxIterations, convergence value, error value, and accuracy. */
	if (gxDef->maxIterations < 0 || gxDef->maxIterations >= 40)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_MAXITR;
	}
	if (gxDef->cnvrgValue <= 1.0E-16 || gxDef->cnvrgValue >= 1.0E-02)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_CNVRGV;
	}
	if (gxDef->errorValue <= 1.0E-14 || gxDef->cnvrgValue >= 1.0 ||
	    gxDef->errorValue <= gxDef->cnvrgValue)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_ERRORV;
	}

	/* That's it for Grid Interpolation transformation. */
	return (err_cnt + 1);
}
/******************************************************************************
The following is the "SetUp" (perhaps constructor would be more appropriate).

Note that this is a generic grid file interpolation object.  Thus the specifics
related to a specific file format are not dealt with here.  What gets handled
here is what is common to all file formats.

Also note that the design is such that a specific transformation of this
type can support grid interpolation data files of different formats.  Thus,
we can support the generic French grid file which covers all of France, and
also include the NTv2 format files used by several municipal governments in
the same transformation.
*/
int EXP_LVL9 CSgridiS (struct cs_GxXform_* gxXfrm)
{
	extern char *cs_DirP;
	extern char cs_Dir [];
	extern char csErrnam [MAXPATH];
	extern struct cs_GridFormatTab_ cs_GridFormatTab [];

	char cc1;
	char cc2;

	short idx;
	short fileCount;

	int status;

	char *cp;
	struct csGridi_* gridi;
	struct cs_GridFile_* gridFilePtr;
	struct cs_GridFormatTab_* frmtTblPtr;
	struct csGeodeticXfromParmsFile_* fileDefPtr;
	struct csGeodeticXformParmsGridFiles_* filesPtr;

	char wrkBufr [MAXPATH + MAXPATH];

	gridFilePtr = NULL;
	gridi = &gxXfrm->xforms.gridi;

	gridi->userDirection = gxXfrm->userDirection;
	gridi->errorValue    = gxXfrm->errorValue;
	gridi->cnvrgValue    = gxXfrm->cnvrgValue;
	gridi->maxIterations = gxXfrm->maxIterations;
	gridi->useBest = FALSE;
	gridi->fallbackDir = cs_DTCDIR_FWD;

	gxXfrm->frwrd2D = (cs_FRWRD2D_CAST)CSgridiF2;
	gxXfrm->frwrd3D = (cs_FRWRD3D_CAST)CSgridiF3;
	gxXfrm->invrs2D = (cs_INVRS2D_CAST)CSgridiI2;
	gxXfrm->invrs3D = (cs_INVRS3D_CAST)CSgridiI3;
	gxXfrm->inRange = (cs_INRANGE_CAST)CSgridiL;
	gxXfrm->release = (cs_RELEASE_CAST)CSgridiR;
	gxXfrm->destroy = (cs_DESTROY_CAST)CSgridiD;

	/* OK, that's the easy stuff.  Now need to loop through each entry in the
	   csGeodeticXformParmsGridFiles_ structure of the cs_GeodeticTransform_
	   definition which we have been provided. */
	filesPtr = &gxXfrm->gxDef.parameters.fileParameters;
	fileCount = filesPtr->fileReferenceCount;
	for (idx = 0;idx < fileCount;idx += 1)
	{
		/* Here once for each file.  Often, there is just one file.  Multiple
		   files is not uncommon.  There will be one or two cases where there
		   are as many as 48 files.

		   For each file, we allocate a cs_GridFile_ structure, populate it
		   with the basic parameters (direction, format, and file path).  We
		   then look the format up in the cs_GridFormatTab_ table, and
		   assuming success, call the initialization function.

		   If all this succeeds, the pointer to the initialized cs_GridFile_
		   object is added to the gridFiles array of the csGridi_ object. */
		fileDefPtr = &filesPtr->fileNames [idx];
		gridFilePtr = (struct cs_GridFile_*)CS_malc (sizeof (struct cs_GridFile_));
		if (gridFilePtr == NULL)
		{
			CS_erpt (cs_NO_MEM);
			goto error;
		}
		gridFilePtr->direction = cs_DTCDIR_NONE;
		if (fileDefPtr->direction == 'F' || fileDefPtr->direction == 'f') gridFilePtr->direction = cs_DTCDIR_FWD;
		if (fileDefPtr->direction == 'I' || fileDefPtr->direction == 'i') gridFilePtr->direction = cs_DTCDIR_INV;
		gridFilePtr->format = CSgridFileFormatCvt (fileDefPtr->fileFormat);
		cp = fileDefPtr->fileName;
		cc1 = *cp;
		cc2 = *(cp + 1);
		if (cc1 == '.' && (cc2 == '\\' || cc2 == '/'))
		{
			/* The file reference is relative.  Relative, now means relative to
			   the main data directory which is maintained in cs_Dir, the same
			   directory to which we look for the various dictionaries. */
			*cs_DirP = '\0';
			CS_stncp (wrkBufr,cs_Dir,MAXPATH);
			CS_stncat (wrkBufr,(cp + 2),MAXPATH);
			CS_stncp (gridFilePtr->filePath,wrkBufr,sizeof (gridFilePtr->filePath));
		}
		else
		{
			/* The file is absolute path name; we can (should be able to) use
			   it as it is. */
			CS_stncp (gridFilePtr->filePath,fileDefPtr->fileName,sizeof (gridFilePtr->filePath));
		}

		/* Force directory separator to the correct character for this run time
		   environment.  Trac Ticket #175. */
		CSrplDirSep (gridFilePtr->filePath);

		// Set up the remainder of the transformation. */
		gridFilePtr->bufferSize = 0L;	/* Zero says use format based default value. */
		gridFilePtr->flags = 0UL;		/* Legacy feature, currently unused. */
		gridFilePtr->density = 0.0;
		gridFilePtr->cnvrgValue = gridi->cnvrgValue;	
		gridFilePtr->errorValue = gridi->errorValue;
		gridFilePtr->maxIterations = gridi->maxIterations;

		for (frmtTblPtr = cs_GridFormatTab;frmtTblPtr->formatCode != cs_DTCFRMT_NONE;frmtTblPtr += 1)
		{
			if (frmtTblPtr->formatCode == gridFilePtr->format)
			{
				break;
			}
		}
		if (frmtTblPtr->formatCode == cs_DTCFRMT_NONE)
		{
			/* Oops!!! The format specification is invalid.  This should never
			   happen as the dictionary compiler verifies this, as does the
			   Q check function. */
			CS_erpt (cs_ISER);
			goto error;
		}

		/* OK, we have a file specification and a valid format, thus we have
		   a setup function. */
		status = (*frmtTblPtr->initialize)(gridFilePtr);
		if (status != 0)
		{
			goto error;
		}

		/* Success!!! Stash a pointer to our initialized object and then
		   continue on to see if there is another file to do. */
		gridi->gridFiles [gridi->fileCount++] = gridFilePtr;
		gridFilePtr = NULL;
	}
	
	/* Deal with the fallback situation. */
	if (filesPtr->fallback [0] != '\0')
	{
		/* Get a transformation from the fallback specification. */
		gridi->fallback = CS_gxloc (filesPtr->fallback,cs_DTCDIR_FWD);
		if (gridi->fallback == 0)
		{
			/* This can happen if a transformation gets deleted. */
			CS_stncp (csErrnam,filesPtr->fallback,MAXPATH);
			CS_erpt (cs_FLBK_NTFND);
			goto error;
		}
		
/* Kludge to fix a defect.  The "RGF93_to_NTF-G-Grid" transformation, or
   anything similar to it, can and will often refer to the "NTF-G_to_WGS84"
   geodetic transformation (a Molodensky transformation) as a fallback.  The
   problem is that the "RGF93_to_NTF-G-Grid" transformation converts from RGF93
   (i.e. WGS84) to NTF, while the "NTF-G_to_WGS84" transformation converts
   in the opposite direction.  This defect is introduced, and a lot of
   additional confusion is experienced, because the French grid interpolation
   data file, "gr3df97a.txt" converts from the new to the old (RGF93 -> NTF),
   while all other grid data interpoolation files convert from the old to
   the new.  The "old to the new" convention of most all grid data files
   (except the French) is consistent with standard datum definitions (i.e.
   convert to WGS84, the newer datum).  There are better ways to deal with
   these issues.  Scheduling pressure and the need to pass a comprehensive
   regression test have lead to this rather kludgy solution.
   
   TODO -- Fix this in a more generic manner.  I suspect, that we need to
   define a new Molodensky transformation that goes in the opposite direction
   as the normal one (i.e. flip the signs on the translation vector) and use
   that as the fall back for transforms which involving NTF and RGF93 which
   are defined to go in the opposite direction. */
/*lint -e525  unexpected indentation change */
/*lint -e539  unexpected indentation change */
if (!CS_stricmp (filesPtr->fallback,"NTF-G_to_WGS84") &&
	!CS_stricmp (gxXfrm->gxDef.srcDatum,"RGF93") &&
	!CS_stricmp (gxXfrm->gxDef.trgDatum,"NTF-G-Grid"))
{
	gridi->fallbackDir = cs_DTCDIR_INV;
}
	}
/*lint +e525 */
/*lint +e539 */
	return 0;

error:
	if (gridFilePtr != NULL)
	{
		CS_free (gridFilePtr);
		gridFilePtr = NULL;
	}
	if (gridi != NULL)
	{
		for (idx = 0;idx < gridi->fileCount;idx += 1)
		{
			gridFilePtr = gridi->gridFiles [idx];
			if (gridFilePtr != NULL)
			{
				(*gridFilePtr->destroy)(gridFilePtr->fileObject.genericPtr);		/*lint !e534  ignoring return value */
			}
		}
	}
	return -1;
}
/*
The 3 dimensional forward conversion function.  Most grid interpolation data
files are strictly horizontal (i.e. 2 dimensional) in nature.  The French
technique, based on the "gr3df97a.txt" data file is the exception.  The
French technique is indeed 3 dimensional.  So our design here supports both
2 and 3 dimensional grid files.
 */
int EXP_LVL9 CSgridiF3 (struct csGridi_ *gridi,double trgLl [3],Const double srcLl [3])
{
	extern char csErrnam [MAXPATH];

	int status;
	int fbStatus;
	int selectedIdx;

	struct cs_GridFile_* gridFilePtr;

	status = 1;		/* Until we know different, we assume no coverage. */
	selectedIdx = CSgridiT (gridi,srcLl,cs_DTCDIR_FWD);
	if (selectedIdx >= 0)
	{
		/* The normal case, we found a grid file which includes the point to be
		   converted within its coverage. */
		gridFilePtr = gridi->gridFiles [selectedIdx];
		if (gridFilePtr != NULL)
		{
			if (gridFilePtr->direction == cs_DTCDIR_FWD)
			{
				/* Call the file format specific forward 3D function. */
				status = (*gridFilePtr->frwrd3D)(gridFilePtr->fileObject.genericPtr,trgLl,srcLl);
			}
			else if (gridFilePtr->direction == cs_DTCDIR_INV)
			{
				/* Call the file format specific inverse 3D function.+ */
				status = (*gridFilePtr->invrs3D)(gridFilePtr->fileObject.genericPtr,trgLl,srcLl);
			}
			else
			{
				CS_stncp (csErrnam,"CS_gridi::1",MAXPATH);
				CS_erpt (cs_ISER);
				status = -1;
			}
		}
		else
		{
			CS_stncp (csErrnam,"CS_gridi::2",MAXPATH);
			CS_erpt (cs_ISER);
			status = -1;
		}
	}
	if (status > 0)
	{
		/* We encountered a coverage issue during the calculation. */
		if (gridi->fallback != 0)
		{
			/* There is a fallback transformation and either there was no
			   primary coverage or the primary transformation failed because
			   of a coverage issue.  (This can happen in the case where an
			   iterative technique was used, e.g. an inverse grid function,
			   and the point provided is very close to the coverage boundary.)
			   
			   These function calls probably should be calls to the 3D versions,
			   but this is what was in the pre-RFC 2 code.  Thus we duplicate that
			   code here in order to get all regression tests to pass.  This is
			   probably a problem in pre-RFC 2 CS-MAP, but we'll fix it only after
			   all regression tests have been passed and acceptance by QA has been
			   made.
			   
			   Note: Most all grid interpolation file methods are 2D, but the
			   French method is different.  That's the likely cause of the bug. */
			if (gridi->fallbackDir == cs_DTCDIR_FWD)
			{
				fbStatus = CS_gxFrwrd2D (gridi->fallback,trgLl,srcLl);
			}
			else if (gridi->fallbackDir == cs_DTCDIR_INV)
			{
				fbStatus = CS_gxInvrs2D (gridi->fallback,trgLl,srcLl);
			}
			else
			{
				fbStatus = 1;
			}
			status = (fbStatus == 0) ? 2 : 1;
		}
		else
		{
			/* There is no fallback transformation and a coverage issue prevented
			   a normal calculation. */
			status = 1;
		}
	}
	return status;
}
int EXP_LVL9 CSgridiF2 (struct csGridi_ *gridi,double* trgLl,Const double* srcLl)
{
	extern char csErrnam [MAXPATH];

	int status;
	int fbStatus;
	int selectedIdx;
	
	struct cs_GridFile_* gridFilePtr;

	status = 1;		/* Until we know different, we assume no coverage. */
	selectedIdx = CSgridiT (gridi,srcLl,cs_DTCDIR_FWD);
	if (selectedIdx >= 0)
	{
		gridFilePtr = gridi->gridFiles [selectedIdx];
		if (gridFilePtr != NULL)
		{
			if (gridFilePtr->direction == cs_DTCDIR_FWD)
			{	
				status = (*gridFilePtr->frwrd2D)(gridFilePtr->fileObject.genericPtr,trgLl,srcLl);
			}
			else if (gridFilePtr->direction == cs_DTCDIR_INV)
			{
				status = (*gridFilePtr->invrs2D)(gridFilePtr->fileObject.genericPtr,trgLl,srcLl);
			}
			else
			{
				CS_stncp (csErrnam,"CS_gridi::3",MAXPATH);
				CS_erpt (cs_ISER);
				status = -1;
			}
		}
		else
		{
			CS_stncp (csErrnam,"CS_gridi::4",MAXPATH);
			CS_erpt (cs_ISER);
			status = -1;
		}
	}
	if (status > 0)
	{
		/* We encountered a coverage issue during the calculation. */
		if (gridi->fallback != 0)
		{
			/* There is a fallback transformation and either there was no
			   primary coverage or the primary transformation failed because
			   of a coverage issue.  (This can happen in the case where an
			   iterative technique was used, e.g. and inverse grid function,
			   and the point provided is very close to the coverage boundary.) */
			if (gridi->fallbackDir == cs_DTCDIR_FWD)
			{
				fbStatus = CS_gxFrwrd2D (gridi->fallback,trgLl,srcLl);
			}
			else if (gridi->fallbackDir == cs_DTCDIR_INV)
			{
				fbStatus = CS_gxInvrs2D (gridi->fallback,trgLl,srcLl);
			}
			else
			{
				fbStatus = 1;
			}
			status = (fbStatus == 0) ? 2 : 1;
		}
		else
		{
			/* There is no fallback transformation and a coverage issue prevented
			   a normal calculation. */
			status = 1;
		}
	}
	return status;
}
int EXP_LVL9 CSgridiI3 (struct csGridi_ *gridi,double* trgLl,Const double* srcLl)
{
	extern char csErrnam [MAXPATH];

	int status;
	int fbStatus;
	int selectedIdx;
	
	struct cs_GridFile_* gridFilePtr;

	status = 1;		/* Until we know different, we assume no coverage. */
	selectedIdx = CSgridiT (gridi,srcLl,cs_DTCDIR_INV);
	if (selectedIdx >= 0)
	{
		gridFilePtr = gridi->gridFiles [selectedIdx];
		if (gridFilePtr != NULL)
		{
			if (gridFilePtr->direction == cs_DTCDIR_FWD)
			{	
				status = (*gridFilePtr->invrs3D)(gridFilePtr->fileObject.genericPtr,trgLl,srcLl);
			}
			else if (gridFilePtr->direction == cs_DTCDIR_INV)
			{	
				status = (*gridFilePtr->frwrd3D)(gridFilePtr->fileObject.genericPtr,trgLl,srcLl);
			}
			else
			{
				CS_stncp (csErrnam,"CS_gridi::5",MAXPATH);
				CS_erpt (cs_ISER);
				status = -1;
			}
		}
		else
		{
			CS_stncp (csErrnam,"CS_gridi::6",MAXPATH);
			CS_erpt (cs_ISER);
			status = -1;
		}
	}
	if (status > 0)
	{
		/* We encountered a coverage issue during the calculation. */
		if (gridi->fallback != 0)
		{
			/* There is a fallback transformation and either there was no
			   primary coverage or the primary transformation failed because
			   of a coverage issue.  (This can happen in the case where an
			   iterative technique was used, e.g. and inverse grid function,
			   and the point provided is very close to the coverage boundary.) */
			if (gridi->fallbackDir == cs_DTCDIR_FWD)
			{
				/* One might think this should be a call to CS_gxInvrs3d.  In
				   releases of CS-MAP, prior to RFC 2, it was the equivalent of
				   CS_gxInvrs2D, so that's what we have here in order to preclude
				   regression test failures.  Thinking about this some more, I
				   believe this is correct, as this is a grid file interpolation
				   method, and most all of these, except the French, is a two
				   dimensional operation.  Thus, when a fall back function is
				   used, the two dimensional version should also be used. */
				/* TODO:  See if this needs to get special treatment for the
				   French variation of grid file interpolation. */
				fbStatus = CS_gxInvrs2D (gridi->fallback,trgLl,srcLl);
			}
			else if (gridi->fallbackDir == cs_DTCDIR_INV)
			{
				/* TODO: See comment above; some special treatment for the French
				   grid file interpolation method may be needed here as the
				   French method is indeed three dimensional. */
				fbStatus = CS_gxFrwrd2D (gridi->fallback,trgLl,srcLl);
			}
			else
			{
				fbStatus = 1;
			}
			status = (fbStatus == 0) ? 2 : 1;
		}
		else
		{
			/* There is no fallback transformation and a coverage issue prevented
			   a normal calculation. */
			status = 1;
		}
	}
	return status;
}
int EXP_LVL9 CSgridiI2 (struct csGridi_ *gridi,double* trgLl,Const double* srcLl)
{
	extern char csErrnam [MAXPATH];

	int status;
	int fbStatus;
	int selectedIdx;
	
	struct cs_GridFile_* gridFilePtr;

	status = 1;		/* Until we know different, we assume no coverage. */
	selectedIdx = CSgridiT (gridi,srcLl,cs_DTCDIR_INV);
	if (selectedIdx >= 0)
	{
		gridFilePtr = gridi->gridFiles [selectedIdx];
		if (gridFilePtr != NULL)
		{
			if (gridFilePtr->direction == cs_DTCDIR_FWD)
			{
				status = (*gridFilePtr->invrs2D)(gridFilePtr->fileObject.genericPtr,trgLl,srcLl);
			}
			else if (gridFilePtr->direction == cs_DTCDIR_INV)
			{
				status = (*gridFilePtr->frwrd2D)(gridFilePtr->fileObject.genericPtr,trgLl,srcLl);
			}
			else
			{
				CS_stncp (csErrnam,"CS_gridi::7",MAXPATH);
				CS_erpt (cs_ISER);
				status = -1;
			}
		}
		else
		{
			CS_stncp (csErrnam,"CS_gridi::8",MAXPATH);
			CS_erpt (cs_ISER);
			status = -1;
		}
	}
	if (status > 0)
	{
		/* We encountered a coverage issue during the calculation. */
		if (gridi->fallback != 0)
		{
			/* There is a fallback transformation and either there was no
			   primary coverage or the primary transformation failed because
			   of a coverage issue.  (This can happen in the case where an
			   iterative technique was used, e.g. and inverse grid function,
			   and the point provided is very close to the coverage boundary.) */
			if (gridi->fallbackDir == cs_DTCDIR_FWD)
			{
				fbStatus = CS_gxInvrs2D (gridi->fallback,trgLl,srcLl);
			}
			else if (gridi->fallbackDir == cs_DTCDIR_INV)
			{
				fbStatus = CS_gxFrwrd2D (gridi->fallback,trgLl,srcLl);
			}
			else
			{
				fbStatus = 1;
			}
			status = (fbStatus == 0) ? 2 : 1;
		}
		else
		{
			/* There is no fallback transformation and a coverage issue prevented
			   a normal calculation. */
			status = 1;
		}
	}
	return status;
}
int EXP_LVL9 CSgridiL (struct csGridi_ *gridi,int cnt,Const double pnts [][3])
{
	int status;
	int index;
	int selectedIdx;

	/* NOTE: a grid file interpolation object with no files is considered a
	   null transformation.  All points are with in the converage of the
	   nuill transformation. */
	status = cs_CNVRT_OK;	/* until we know differently */

	/* We need to check each point independently, as the different points
	   may be covered by different files.  If we have a file which can
	   provide coverage, we assume that to be sufficient evidence to
	   say that the point is covered by this transformation. */
	for (index = 0;index < cnt;index += 1)
	{
		selectedIdx = CSgridiT (gridi,pnts [index],cs_DTCDIR_FWD);
		if (selectedIdx < 0)
		{
			/* No file in the transformation has coverage for this
			   point. */
			status = cs_CNVRT_USFL;
			break;
		}
	}
	return status;
}
int EXP_LVL9 CSgridiN  (struct csGridi_ *gridi)
{
	int isNull;
	
	isNull = (gridi->fileCount == 0);
	return isNull;
}
int EXP_LVL9 CSgridiR (struct csGridi_ *gridi)
{
	short idx;
	int status;
	struct cs_GridFile_* gridFilePtr;

	status = 0;
	for (idx = 0;idx < gridi->fileCount;idx += 1)
	{
		gridFilePtr = gridi->gridFiles [idx];
		if (gridFilePtr != NULL)
		{
			(*gridFilePtr->release)(gridFilePtr->fileObject.genericPtr);		/*lint !e534  ignoring return value, always zero */
		}
	}
	return status;
}
int EXP_LVL9 CSgridiD (struct csGridi_ *gridi)
{
	short idx;
	int status;
	struct cs_GridFile_* gridFilePtr;

	status = 0;
	for (idx = 0;idx < gridi->fileCount;idx += 1)
	{
		gridFilePtr = gridi->gridFiles [idx];
		if (gridFilePtr != NULL)
		{
			(*gridFilePtr->destroy)(gridFilePtr->fileObject.genericPtr);		/*lint !e534  ignoring return value, always 0 */
			CS_free (gridFilePtr);
			gridi->gridFiles [idx] = NULL;
		}
	}

	if (NULL != gridi->fallback)
	{
		(*gridi->fallback).destroy(&gridi->fallback->xforms);

		CS_free(gridi->fallback);
		gridi->fallback = NULL;
	}

	return status;
}
int CSgridiT (struct csGridi_ *gridi,double* ll_src,short direction)
{
	short idx;
	short selectedIdx;

	double density;
	double bestSoFar;
	
	struct cs_GridFile_* gridFilePtr;

	selectedIdx = -1;
	if (!gridi->useBest)
	{
		/* Use the first one in the list with the appropriate coverage. */
		for (idx = 0;idx < gridi->fileCount;idx += 1)
		{
			gridFilePtr = gridi->gridFiles [idx];
			if (gridFilePtr != NULL)
			{
				density = (*gridFilePtr->test)(gridFilePtr->fileObject.genericPtr,ll_src,direction);
				if (density != 0.0)
				{
					selectedIdx = idx;
					break;
				}
			}
		}
	}
	else
	{
		/* Select the specific file which provides the highest grid density
		   (i.e. the smallest grid cells) coverage. */
		bestSoFar = 99.0E+100;
		for (idx = 0;idx < gridi->fileCount;idx += 1)
		{
			gridFilePtr = gridi->gridFiles [idx];
			if (gridFilePtr != NULL)
			{
				density = (*gridFilePtr->test)(gridFilePtr->fileObject.genericPtr,ll_src,direction);
				if (density == 0.0)
				{
					continue;
				}
				/* Densities should always be positive. */
				if (density < bestSoFar)
				{
					bestSoFar = density;
					selectedIdx = idx;
				}
			}
		}
	}
	return selectedIdx;
}
enum csGridFormat CSgridFileFormatCvt (unsigned flagValue)
{
	enum csGridFormat rtnValue;

	switch (flagValue)
	{
		case  cs_DTCFRMT_NONE:    rtnValue = gridFrmtNone;    break;
		case  cs_DTCFRMT_CNTv1:   rtnValue = gridFrmtNTv1;    break;
		case  cs_DTCFRMT_CNTv2:   rtnValue = gridFrmtNTv2;    break;
		case  cs_DTCFRMT_NADCN:   rtnValue = gridFrmtNadcn;   break;
		case  cs_DTCFRMT_FRNCH:   rtnValue = gridFrmtFrnch;   break;
		case  cs_DTCFRMT_JAPAN:   rtnValue = gridFrmtJapan;   break;
		case  cs_DTCFRMT_ATS77:   rtnValue = gridFrmtAts77;   break;
		case  cs_DTCFRMT_OST97:   rtnValue = gridFrmtOst97;   break;
		case  cs_DTCFRMT_OST02:   rtnValue = gridFrmtOst02;   break;
		case  cs_DTCFRMT_GEOCN:   rtnValue = gridFrmtGeocn;   break;
		default:                  rtnValue = gridFrmtUnknown; break;
	}
	return rtnValue;
}
