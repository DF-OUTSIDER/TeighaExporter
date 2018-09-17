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

/*lint -esym(534,CSbswap) */

#include "cs_map.h"
#ifndef ANDROID
#include <wchar.h>
#endif
#include <ctype.h>

#if ( _OPR_SYSTEM == _os_UNIX  || _OPR_SYSTEM == _os_SOLARIS) 
# define _cdecl
#endif

/**********************************************************************
**	ptr = CS_malc (blk_size);
**	CS_free (ptr);
**
**	void *ptr;					pointer to allocated memory.
**
**	In the world of DLL's, multiple threads, and who knows what
**	else, memory allocation and releasing has many pit falls.
**	CS-MAP now uses these function exclusively to allocate and
**	free memory. It is also documented in the manual that CS_free
**	be used to release all manner of things. In this manner,
**	hopefully, the same functions will be doing all the malloc'ing
**	and free'ing, in the same environment.
**
**	I think the problem is that a malloc and free get linked into
**	the DLL, and another malloc and free get linked into the main
**	process. If these two functions behave differently for what
**	ever reason (release levels, multi-thread vs single thread,
**	etc.) the heap(s) get corrupted and eventually everything
**	becomes unglued.
**
**	In the future, these functions may be used to maintain a local
**	heap for CS-MAP when running as a DLL.
**********************************************************************/



/**********************************************************************
**	flag = CS_bins (fd,start,eof,rs,rec,comp);
**
**	int fd;						file descriptor of the file to be searched.
**	long32_t start;					position in the file of the first record.
**								Use this to skip over magic numbers, etc.
**	long32_t eof;					position in the file to be considered the
**								end of the file if not the real eof, use 0L
**								to indicate the real eof.
**	int rs;						size of the records in the file.
**	void *rec;					pointer to the comparision record,
**								i.e. a record with the key value being
**								searched for.
**	int (*func)();				pointer to the comparision function.
**	int flag;					returns TRUE (i.e. +1) if search was successful,
**								return FALSE (i.e 0) if the search was unsuccessful,
**								and a -1 if an I/O error or some other
**								obnoxious thing happened.
**
**	In the case of a successful search, the file
**	is left positioned for a read (or write) of the specific
**	record located.  In the event of duplicate records, the
**	file is positioned for the reading of the first of such
**	duplicate records.
**
**	In the event of an unsuccessful search, the file 
**	is left positioned for a read of the record which
**	would immediately follow the searched for record
**	if it did indeed exist.
**
**	This routine uses standard read and seek system
**	calls.  Using the stream buffering for a binary
**	search would actually be slower.
**********************************************************************/


int EXP_LVL7 CS_bins (csFILE *strm,long32_t start,long32_t eofPos,int rs,Const void *rec,int (*comp)(Const void *pp,Const void *qq))
{
	int st;
	int flag = -1;						/* initialization to keep gcc happy */
	size_t rd_cnt;

	long32_t nrecs;
	long32_t hi_rec;
	long32_t lo_rec;
	long32_t test_rec;
	long32_t test_pos;

	char *buff;

	/* Prepare for a possible error. */
	buff = NULL;

	/* Get a buffer which we can use for record I/O. */
	buff = (char *)CS_malc ((unsigned)rs);
	if (buff == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}

	/* Compute the portion of the file which is to be
	   searched.  Quite often, we don't want to
	   search the whole thing. */
	if (start < 0L)
	{
		start = CS_ftell (strm);
		if (start < 0L)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
	}
	if (eofPos <= 0L)
	{
		st = CS_fseek (strm,0L,SEEK_END);
		if (st != 0)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
		eofPos = CS_ftell (strm);
		if (eofPos < 0L)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
	}

	/* Compute the number of records in the file
	   which are to be searched. */
	nrecs = (eofPos - start) / rs;

	/* If there are no records, we don't have a match,
	   any any record would appear at the start. */
	if (nrecs <= 0)
	{
		st = CS_fseek (strm,start,SEEK_SET);
		if (st != 0)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}

		CS_free (buff);
		buff = NULL;

		return (0);
	}

	/* Set the starting and ending record numbers, where
	   the first record is numbered zero. */
	hi_rec = nrecs - 1;
	lo_rec = 0;

	/* Continue to search the records until we have
	   exhausted all possibilities.  The equal part
	   of this test is nescessary so that the last
	   record in the search gets compared for a match
	   or not. */
	test_pos = start;					/* initialization to keep gcc happy */
	while (lo_rec <= hi_rec)
	{
		/* Select the next record to be tested in terms
		   of record number. */
		test_rec = (hi_rec + lo_rec) / 2;

		/* Convert this to a file position. */
		test_pos = (long32_t)(test_rec * rs) + start;

		/* Read in the new test record. */
		st = CS_fseek (strm,test_pos,SEEK_SET);
		if (st != 0)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
		rd_cnt = CS_fread (buff,1,(unsigned)rs,strm);
		if (rd_cnt != (size_t)rs)
		{
			if (ferror (strm)) CS_erpt (cs_IOERR);
			else			   CS_erpt (cs_INV_FILE);
			goto error;
		}

		/* Compare with our search record. */
		flag = (*comp)(buff,rec);
		if (flag > 0)
		{
			/* Here if our test record is past where
			   our search record will be if it does
			   indeed exist.  We can exclude this
			   record (that we just read) and all
			   records past it from the search. */
			hi_rec = test_rec - 1;
		}
		else if (flag < 0)
		{
			/* Here if the test record is before where
			   our search record will be if it does
			   indeed exist.  We can exclude this
			   record and all which precede it from
			   the search. */
			lo_rec = test_rec + 1;
		}
		else
		{
			/* We have found a matching record.  Before
			   we return, we need to make sure that if
			   there are duplicate records in the file,
			   we are returning with the file positioned
			   to read the first of such duplicates. */
			do
			{
				test_pos -= (long32_t)rs;
				if (test_pos < start)
				{
					/* The last record tested is the
					   first in the search area.
					   Seek to search and break,
					   we're all done. */
					st = CS_fseek (strm,start,SEEK_SET);
					if (st != 0)
					{
						CS_erpt (cs_IOERR);
						goto error;
					}
					break;
				}

				/* Seek to the record just prior
				   to the last one tested and
				   read it in. */
				st = CS_fseek (strm,test_pos,SEEK_SET);
				if (st != 0)
				{
					CS_erpt (cs_IOERR);
					goto error;
				}
				rd_cnt = CS_fread (buff,1,(unsigned)rs,strm);
				if (rd_cnt != (size_t)rs)
				{
					if (ferror (strm)) CS_erpt (cs_IOERR);
					else               CS_erpt (cs_INV_FILE);
					goto error;
				}

				/* See if this record also matches
				   our key.  NOTE, that the read
				   positions the file back to the
				   last record tested, which is the
				   desired file position if the
				   match test fails. */
				flag = (*comp)(buff,rec);
			} while (flag == 0);

			/* Reset flag to zero, i.e. we got here because
			   we found a matching record. */
			flag = 0;

			/* Now we can return. */
			break;
		}
	}

	/* If flag is negative here, the file is correctly
	   positioned since we have read the record in and
	   the file is positioned for the next.  If flag
	   is greater than zero, we need to move the file
	   pointer back one record so that the user will
	   be able to re-read the current record. */
	if (flag > 0)
	{
		st = CS_fseek (strm,test_pos,SEEK_SET);
		if (st != 0)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
	}

	/* OK, return and tell the user if we found the
	   desired record or not. */

	CS_free (buff);
	buff = NULL;
	return (flag == 0);

error:
	if (buff != NULL) CS_free (buff);
	return (-1);
}

/**********************************************************************
**	stat = CS_bswap (rec,frmt);
**
**	void *rec;					pointer to the data record to be swapped.
**	char *frmt;					pointer to a character string which defins the
**								format of the record.  See Remarks below.
**	int stat;					returns 0 if now swapping was done,
**								1 if swapping was performed,
**								-1 if swap order of this machine is
**								not supported.
**
**	If this function is compiled for a little endian machine,
**	this function simply returns after doing nothing.  This
**	is sensed by the value of a long which is initialized as
**	a character array.  Thus, this function, and the whole of
**	CS_MAP Release 6.0 and later, always assumes that it is
**	reading and writing in little endian order.
**
**	The frmt argument is a null terminated character strings
**	which defines the format of the record to be swapped.  In
**	te simple case, there is one character for each data item,
**	lower case is required:
**
**		c = an 8 bit byte
**		s = 16 bit integer
**		l = 32 bit integer
**		f = 32 bit floating point number (IEEE)
**		d = 64 bit floating point number (IEEE)
**
**	Any of the above data types can be preceded by a count.
**	For example, "32c4l32c4d" would indicate that the item
**	to be swapped consists of a 32 byte character array,
**	an array of 4 longs, another 32 byte character array,
**	and an array of 4 doubles.  (CS_bswap does nothing to
**	character arrays.)
**
**	Characters other than the decimal digits and the five
**	characters given above are considered to be 'c'.  Of
**	course, the null character terminates the specification.
**
**	Example for the cs_Eldef_ structure:
**
**		"12c4cdddds64"
**
**	Notice that we ignore the problem of some machines,
**	notably some older DEC machines, which have some strange
**	way of ordering bytes.  We deal only with little endian
**	(least significant byte first) or big endian (most
**	significant byte first).  This greatly simplifies things.
**
**	ALSO NOTICE: there is no way to specify the direction
**	of the swap.  The swap is binary reversible.  That is,
**	calling this function twice on the same data gets you
**	back exactly what you started with.  Since the code has
**	no way of knowing what you are passing it (i.e. little
**	endian or big endian) its up to the caller to know what
**	they're passing and what they will get back.
**
**	In CS_MAP, we pass everything we have just read which
**	has binary data through this function, and we pass
**	everything which we are about to write (which has binary
**	data) through this function.  This assumes that all
**	binary data files are maintained in the little endian
**	(i.e. DOS) form.  This enables users to use DOS programs
**	(like Tralaine) to maintain the coordinate system
**	dictionary and port the file to another platform, like
**	a SUN SparcStation.
**********************************************************************/

/*
	The following data items are used in the detection
	of the type of machine currently executing with
	regard to byte swapping.  This is done dynamically.
*/

/*lint -e708  union initialization; primary purpose of this variable. */
union cs_Bswap_ cs_BswapU =
{
	{'\0','\01','\02','\03'}
};

int EXP_LVL5 CS_isBigEndian ()
{
	extern union cs_Bswap_ cs_BswapU;

	return (cs_BswapU.llll == 0X010203L);
}
int EXP_LVL5 CS_isLittleEndian ()
{
	extern union cs_Bswap_ cs_BswapU;

	return (cs_BswapU.llll == 0X03020100L);
}

int EXP_LVL5 CS_bswap (void *rec,Const char *frmt)
{
	extern union cs_Bswap_ cs_BswapU;

	/* First we see if this is a little endian machine.
	   If it is, we assume that no swapping is required.
	   That is, we assume that the files being read are in
	   little endian (i.e. INTEL) format, regardless of the
	   platform on which we are running. */

	if (cs_BswapU.llll == 0X03020100L)
	{
		/* This is a little endian machine.  We assume that
		   the data files do not need to be swapped. I.e.
		   we assume that all data files are in little
		   endian byte order ala the PC. */

		return (0);
	}
	else if (cs_BswapU.llll != 0X010203L)
	{
		CS_erpt (cs_BSWP_UNKWN);
		return (-1);
	}
	CSbswap (rec,frmt);
	return (1);
}

int EXP_LVL9 CSbswap (void *rec,Const char *frmt)
{
	cs_Register char cc;
	unsigned char byte;

	/* We do compares on these two pointers below.  Therefore,
	   I have made them huge in the Large and Compact memory
	   models.  This may be a bit overly conservative. */

	cs_Register unsigned char Huge *ap;
	cs_Register unsigned char Huge *zp;

	int ii;
	int size;
	int count;

	Const char *fp;
	unsigned char Huge *dp;

	/* This function always performs the indicated byte swapping. */

	fp = frmt;
	dp = (unsigned char Huge *)rec;
	while ((cc = *fp++) != '\0')
	{
		size = count = 1;

		/* If the current character is a digit, we have
		   a count specification to parse.  We extract
		   the count being careful to leave the loop
		   with cc containing the character which terminates
		   the numeric sequence and fp pointing to the
		   character which immediately follows this
		   type character. */

		if (cc >= '0' && cc <= '9')
		{
			count = (cc - '0');
			cc = *fp++;
			while (cc != '\0' && cc >= '0' && cc <= '9')
			{
				count = count * 10 + (cc - '0');
				cc = *fp++;
			}
		}

		switch (cc) {

		case 's': size = sizeof (short);  break;
		case 'l': size = sizeof (long32_t);   break;
		case 'f': size = sizeof (float);  break;
		case 'd': size = sizeof (double); break;

		case 'c':
		default:

			/* Interpret any other character as a
			   c for character. */

			size = sizeof (char);
			break;
		}

		/* At this point, we have two variables: size and count.
		   Size is the size of the item to be swapped, and
		   count is the number of items.  The following should
		   be very fast if indeed the two pointers and the
		   cc variable are in registers.

		   If size is 1, we have nothing to do except
		   bump the dp variable by count. */

		if (size == 1)
		{
			dp += count;
			continue;
		}

		/* Otherwise, we have some swapping to do.  The way
		   this is coded, any decent compiler should produce
		   a very tight little loop. */

		for (ii = 0;ii < count;ii++)
		{
			ap = dp;
			zp = dp = ap + size;
			while (ap < zp)
			{
				byte = *ap;
				*ap++ = *--zp;
				*zp = byte;
			}
		}
	}
	return (1);
}

/**********************************************************************
**	CS_iicrt (aa,rVal,iVal);	populate (or construct if you like)
**	CS_iiadd (aa,bb,cc);		add
**	CS_iisub (aa,bb,cc);		subtract
**	CS_iikmul (aa,kk,cc);		scalar multiply
**	CS_iimul (aa,bb,cc);		multiply
**	CS_iidiv (aa,bb,cc);		divide
**	CS_iisrs (aa,AB,nn,cc);		series
**	CS_iisrs1 (aa,AB,nn,cc);	series 1st deriv.
**	CS_iisrs0 (aa,AB,nn,cc);	?????.
**	val = CS_iiabs (aa);		absolute value
**	CS_iicnj (aa,cc);		conjugate
**
**	struct cs_Cmplx_ *aa;		first of (usually) two arguments involved in the
**								arithmatic operation.
**	struct cs_Cmplx_ *bb;		second argument involved in the arithmatic
**								operation.
**	double kk;					scalar to be used for operations in which an
**								argument is a scalar.
**	int nn;						the number of terms in the complex series
**								calculation.
**	struct cs_Cmplx_ *AB;		an array of (nn + 1) cs_Cmplx structures which
**								contain the complex series coefficients.  The first
**								element in the array is considered to be element
**								0 and must be set to  complex zero.
**	struct cs_Cmplx_ *cc;		result of binary arithmatic operations is
**								returned here.
**	double val;					value of operation for absolute value.
**	double rVal;				real component value.
**	double iVal;				imaginary component value.
**
**	NOTE: In all functions, all arguments may point to the
**	same structure instance.
**********************************************************************/
void EXP_LVL5 CS_iicrt (struct cs_Cmplx_ *aa,double rVal,double iVal)
{
	aa->real = rVal;
	aa->img  = iVal;
	return;
}
void EXP_LVL5 CS_iicpy (const struct cs_Cmplx_ *aa,struct cs_Cmplx_ *bb)
{
	bb->real = aa->real;
	bb->img  = aa->img;
	return;
}

void EXP_LVL5 CS_iiadd (Const struct cs_Cmplx_ *aa,Const struct cs_Cmplx_ *bb,struct cs_Cmplx_ *cc)
{
	cc->real = aa->real + bb->real;
	cc->img  = aa->img  + bb->img;
	return;
}

void EXP_LVL5 CS_iisub (Const struct cs_Cmplx_ *aa,Const struct cs_Cmplx_ *bb,struct cs_Cmplx_ *cc)
{
	cc->real = aa->real - bb->real;
	cc->img  = aa->img  - bb->img;
	return;
}

void EXP_LVL5 CS_iikmul (	Const struct cs_Cmplx_ *aa,double kk,struct cs_Cmplx_ *cc)
{
	cc->real = aa->real * kk;
	cc->img  = aa->img * kk;
	return;
}

void EXP_LVL5 CS_iimul (Const struct cs_Cmplx_ *aa,Const struct cs_Cmplx_ *bb,struct cs_Cmplx_ *cc)
{
	double tmp;

	tmp      = (bb->real * aa->img) + (aa->real * bb->img);
	cc->real = (aa->real * bb->real) - (aa->img * bb->img);
	cc->img  = tmp;
	return;
}

void EXP_LVL5 CS_iidiv (Const struct cs_Cmplx_ *aa,Const struct cs_Cmplx_ *bb,struct cs_Cmplx_ *cc)
{
	extern double cs_Huge;			/* +1.0E+32 */
	double rnum;
	double inum;
	double denom;

	if (bb->real == 0.0 && bb->img == 0.0)
	{
		cc->real = cc->img = cs_Huge;
		return;
	}

	rnum  = (aa->real * bb->real) + (aa->img * bb->img);
	inum =  (bb->real * aa->img) - (aa->real * bb->img);
	denom = (bb->real * bb->real) + (bb->img * bb->img);
	cc->real = rnum / denom;
	cc->img  = inum / denom;
	return;
}

double EXP_LVL5 CS_iiabs (Const struct cs_Cmplx_ *aa)
{
	return (sqrt (aa->real * aa->real + aa->img * aa->img));
}

void EXP_LVL5 CS_iicnj (Const struct cs_Cmplx_ *aa,struct cs_Cmplx_ *cc)
{
	cc->real =  aa->real;
	cc->img  = -aa->img;
	return;
}

/*
	This function uses horner's rule, applied to complex
	numbers, to compute the result of:

	for jj = 1 to nn  SUM OF  (xy**jj) * (AB sub jj)
*/

void EXP_LVL5 CS_iisrs (Const struct cs_Cmplx_ *xy,Const struct cs_Cmplx_ *AB,int nn,struct cs_Cmplx_ *new_xy)
{
	extern double cs_Two;

	int jj;

	double rr;
	double ss;

	struct cs_Cmplx_ last_aa;
	struct cs_Cmplx_ last_bb;
	struct cs_Cmplx_ new_aa;
	struct cs_Cmplx_ new_bb;

	rr = cs_Two * xy->real;
	ss = xy->real * xy->real + xy->img * xy->img;

	new_aa.real = AB [nn].real;
	new_aa.img  = AB [nn].img;
	new_bb.real = AB [nn - 1].real;
	new_bb.img  = AB [nn - 1].img;

	for (jj = 2;jj <= nn;jj++)
	{
		last_aa.real = new_aa.real;
		last_aa.img  = new_aa.img ;
		last_bb.real = new_bb.real;
		last_bb.img  = new_bb.img ;

		CS_iikmul (&last_aa,rr,&new_aa);
		CS_iiadd  (&last_bb,&new_aa,&new_aa);

		CS_iikmul (&last_aa,ss,&new_bb);
		CS_iisub  (&AB [nn - jj],&new_bb,&new_bb);
	}

	CS_iimul (xy,&new_aa,new_xy);
	CS_iiadd (&new_bb,new_xy,new_xy);
	return;
}

/*
	This function uses horner's rule, applied to complex
	numbers, to compute the result of:

	for jj = 1 to nn  SUM OF  jj * (xy**(jj - 1)) * (AB sub jj)

	Note that this is essentially the first deriviative of the
	the above calculation.
*/

void EXP_LVL5 CS_iisrs1 (	Const struct cs_Cmplx_ *xy,Const struct cs_Cmplx_ *AB,int nn,struct cs_Cmplx_ *new_xy)
{
	extern double cs_Two;

	int jj;

	double rr;
	double ss;

	struct cs_Cmplx_ last_cc;
	struct cs_Cmplx_ last_dd;
	struct cs_Cmplx_ new_cc;
	struct cs_Cmplx_ new_dd;
	struct cs_Cmplx_ tmp_dd;

	rr = cs_Two * xy->real;
	ss = xy->real * xy->real + xy->img * xy->img;

	new_cc.real = (double)nn * AB [nn].real;
	new_cc.img  = (double)nn * AB [nn].img;
	new_dd.real = (double)(nn - 1) * AB [nn - 1].real;
	new_dd.img  = (double)(nn - 1) * AB [nn - 1].img;

	for (jj = 2;jj <= (nn - 1);jj++)
	{
		last_cc.real = new_cc.real;
		last_cc.img  = new_cc.img ;
		last_dd.real = new_dd.real;
		last_dd.img  = new_dd.img ;

		CS_iikmul (&last_cc,rr,&new_cc);
		CS_iiadd  (&last_dd,&new_cc,&new_cc);

		CS_iikmul (&last_cc,ss,&new_dd);
		CS_iikmul (&AB [nn - jj],(double)(nn - jj),&tmp_dd);
		CS_iisub  (&tmp_dd,&new_dd,&new_dd);
	}

	CS_iimul (xy,&new_cc,new_xy);
	CS_iiadd (&new_dd,new_xy,new_xy);
	return;
}

/*
	This function computes:

	for jj = 2 to nn  SUM OF  (jj - 1) * (xy**jj) * (AB sub jj)

	I'm not sure what this is yet, nor have I figured out
	a Horner simplification.  Time constraints require me to
	brute force this at the current time.  We'll do better
	next time.  This shows up in the New Zealand Grid
	calculations.  I think Synder's method may be better,
	(see CSmstroI) but we stick to what the New Zealander's
	defined to stay out of trouble.
*/

void EXP_LVL5 CS_iisrs0 (Const struct cs_Cmplx_ *xy,Const struct cs_Cmplx_ *AB,int nn,struct cs_Cmplx_ *new_xy)
{
	extern double cs_Zero;

	int jj;

	struct cs_Cmplx_ sum;
	struct cs_Cmplx_ tmp;
	struct cs_Cmplx_ xy_pwr;

	sum.real = cs_Zero;
	sum.img  = cs_Zero;
	CS_iimul (xy,xy,&xy_pwr);
	for (jj = 2;jj <= nn;jj++)
	{
		CS_iikmul (&AB [jj],(double)(jj - 1),&tmp);
		CS_iimul  (&tmp,&xy_pwr,&tmp);
		CS_iiadd  (&sum,&tmp,&sum);
		CS_iimul (&xy_pwr,xy,&xy_pwr);
	}
	new_xy->real = sum.real;
	new_xy->img  = sum.img;
	return;
}

/**********************************************************************
**	swp_cnt = CS_ips (fd,rs,eof,comp);
**
**	int fd;						file descriptor of the open file to be
**								sorted.  Obviously, read and write access
**								is required.
**	int rs;						size of the records which are to be sorted.
**	long32_t eof;	            position in the file which is to be considered
**								the end of the file as far as the sort is
**								concerned, zero says use true end of file.
**	int (*comp)();				pointer to the record comparision function.
**	int swp_cnt;				returns the number of swaps which were
**								required to complete the sort, -1 if an
**								error occurred.
**
**	cs_Sortbs external variable which can be set to indicate the
**		maximum size of the sort buffer to be used.  Initial
**		value is defined in CSdata.c.  This variable is an
**		int.   It CANNOT be changed to a long for larger
**		buffers without rewritting this function.
**
**	NOTE:: Only that portion of the file which is between
**	the current position of the file at the time this
**	function is called and the value indicated by the
**	eof argument is sorted.  This enables you to sort
**	data in the middle of a file, especially handy when
**	files have magic numbers and the like.  THEREFORE, be sure
**	to use a seek function immediately before calling
**	this function to establish the location within the
**	the file where the first record to be sorted lives.
**	Before this function returns, the original file position
**	is restored.
**
**********************************************************************/

int EXP_LVL7 CS_ips (csFILE *strm,short rs,long32_t eofPos,int (*comp)(Const void *pp,Const void *qq))
{
	int st;
	size_t rec_cnt;	/* Record count */
	size_t buf_siz;	/* Buffer size */
	size_t rd_cnt;	/* Read check. */
	size_t wr_cnt;	/* Write check. */

	long32_t beg;		/* File position of the beginning of
					   the portion of the file which we
					   are to sort but remains unsorted. */
	long32_t end;		/* File position of the end of the
					   portion of the file which we are
					   to sort but remains unsorted. */
	void *buff;		/* Pointer to malloc'ed sort buffer. */

	/* Prepare for a possible error. */
	buff = NULL;

	/* Establish the extrema of the sort.  We support
	   sorting only a portion of a file. */
	beg = CS_ftell (strm);
	if (beg < 0L)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	if (eofPos <= 0L)
	{
		st = CS_fseek (strm,0L,SEEK_END);
		if (st != 0)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
		end = CS_ftell (strm);
		if (end < 0L)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
		st = CS_fseek (strm,beg,SEEK_SET);
		if (st != 0)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}
	}
	else
	{
		end = eofPos;
	}

	/* If there is nothing to sort, we done. */
	if (end <= beg)
	{
		return 0;
	}

	/* Set up our buffer area. */
	buf_siz = (size_t)(end - beg);
	buff = malloc (buf_siz);
	if (buff == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}

	/* Compute some basic parameters for the sort. */
	rec_cnt = buf_siz / (unsigned short)rs;

	/* Read in the entire file. */
	rd_cnt = CS_fread (buff,1,buf_siz,strm);
	if (rd_cnt != buf_siz)
	{
		if (ferror (strm)) CS_erpt (cs_IOERR);
		else               CS_erpt (cs_INV_FILE);
		goto error;
	}

	/* Sort it. */
	qsort (buff,rec_cnt,rs,(int (_cdecl *)(Const void *pp,Const void *qq))comp);								/*lint !e732 */

	/* Write it back out. */
	st = CS_fseek (strm,beg,SEEK_SET);
	if (st != 0)
	{
		CS_erpt (cs_IOERR);
		goto error;
	}
	wr_cnt = CS_fwrite (buff,1,buf_siz,strm);
	if (wr_cnt != buf_siz)
	{
		if (ferror (strm)) CS_erpt (cs_IOERR);
		else               CS_erpt (cs_DISK_FULL);
		goto error;
	}

	/* Clean up */
	CS_fseek (strm,beg,SEEK_SET);
	free (buff);
	return (1);

error:
	if (buff != NULL) free (buff);
	return (-1);
}

/**********************************************************************
**	pntr = CS_lget (str,fld,size,fill);
**
**	char *str;					pointer to character array in which the
**								result is to be returned.
**	char *fld;					pointer to character array containing the
**								left justified field.
**	int size;					length of the left justified field in
**								characters.
**	char fill;					fill character to be stripped from the
**								justified field.
**	char *pntr;					a pointer to the terminating null character
**								of the resultant string is returned.
**
**	A segmentation violation can occur if the value
**	of size is zero or negative.
**
**	This module assumes that the character array pointed
**	to by str is size + 1 characters in size.  No size
**	checking is performed in this routine.
**********************************************************************/

char * EXP_LVL7 CS_lget (cs_Register char *str,Const cs_Register char *fld,int size,char fill)
{
	Const cs_Register char *pp;

	/* Point p at the last character of the field. */

	pp = fld + size - 1;

	/* Find the last non-fill character.  Make
	   sure we don't go past the beginning of the
	   field.  If we don't, we could go right into
	   segmentation violation country. */

	while ((size > 0) && (*pp == fill))
	{
		pp--;
		size--;
	}

	/* Move the string until we encounter the last
	   non-fill character.  If the field is empty
	   (i.e. all fill) we shouldn't even enter this
	   loop. */

	while (size > 0)
	{
		*str++ = *fld++;
		size--;
	}

	/* Terminate the string and return a pointer to
	   the terminating null character. */

	*str = '\0';
	return (str);
}

/**********************************************************************
**	CS_lput (fld,str,size,fill);
**
**	char *fld;					character array in which the left
**								justified field is to be created.
**	char *str;					string to be converted;
**	int size;	size of the field to be created
**								in bytes.
**	char fill;					fill character to be used.
**
**	A zero or negative value for size causes this
**	routine to be an expensive no-op.
**
**	If the string is longer than the size of the
**	field, the data is truncated to the field
**	length, no error condition is given.
**
**	Reccommended for use in situations where security
**	is critical.  Use of this routine can preclude
**	valuable data from being left in files following
**	the null character of an ordinary string.  Note
**	that the null character is a valid fill character.
**********************************************************************/

void EXP_LVL7 CS_lput (char *fld,Const char *str,int size,char fill)
{
	/* We know that we have to store "size"
	   characters, and only "size" characters. */

	while (size > 0)
	{
		/* The value of *str tells us whether to
		   copy or fill. */

		if (*str) *fld++ = *str++;
		else *fld++ = fill;

		size--;
	}

	return;
}

/**********************************************************************
**	pntr = CS_stcpy (dest,source);
**
**	char *dest;					pointer to the character array to which the
**								source string is to be copied.
**	char *source;				pointer to the character array containing
**								the string to be copied.
**	char *pntr;					returns pointer to terminating null
**								character of the destination string.
**
**	Same as "strcpy" except that it returns a pointer to
**	the termination null character of the resulting string
**	rather than a pointer to the beginning of the
**	destination string.  Like "strcpy", no checks as to
**	string length are made, the entire data section of
**	memory can easily be wiped out.
**********************************************************************/

char * EXP_LVL3 CS_stcpy (char *dest,Const char *source)

{
#ifndef _MSC_VER
	do { } while ((*dest++ = *source++) != '\0');
	return (--dest);
#else

	/* Note, strcpy always returns its first argument, which could
	   be null, I suppose. */
	char* result = strcpy(dest, source);
	if (NULL != result)					/*lint !e774  boolean always evaluates to true */
	{
		return dest + strlen(source);
	}
	return NULL;

#endif
}

/**********************************************************************
**	pntr = CS_stncp (dest,from,count);
**
**	char *dest;					location to be copied to.
**	char *source;				location of the string to be copied.
**	int count;					size of the destination array.
**	char *pntr;					returns pointer to the terminating
**								null character of the result.
**
**	A zero or negative count value will cause this routine
**	to return the null pointer as an error signal.  Of
**	course, it is impossible to move zero characters when
**	we are required to properly terminate it with a null
**	character.
**
**	CS_stncp will not copy more than n characters and
**	always guarantees that the result is null terminated.
**	It is analogous to "strncpy" except that it returns a
**	pointer to the end of the result and that it guarantees
**	a result which is null terminated.  In cases where
**	the source string is larger than count, this routine
**	will copy one less character than "strncpy", replacing
**	this extra character with a null character.
**
**	Intended Usage:
**
**	pntr = CS_stncp (to,from,sizeof (to));
**
**********************************************************************/

char * EXP_LVL3 CS_stncp (char *dest,Const char *source,int count)
{
	/* Return a null pointer as an error indication
	   if count is less than or equal to zero. */

	if (count <= 0) return (NULL);

	/* Otherwise, we copy the string.  We will always
	   copy a null character even if it is the only
	   character we copy. */

	count--;
	while ((count > 0) && (*source))
	{
		*dest++ = *source++;
		count--;
	}
	*dest = '\0';
	return (dest);
}

/**********************************************************************
**	pntr = CS_stncat (dest,from,count);
**
**	char *dest;					character array to be appended to.
**	char *source;				location of the string to be copied.
**	int count;					size of the destination array.
**	char *pntr;					returns pointer to the terminating
**								null character of the result.
**
**	Performs the function of strncat with five(5) major differences:
**
**	1> The count parameter MUST be the size of the target array.
**	2> The destination array is NEVER overlfowed.
**	3> the result in dest is ALWAYS null terminated.
**	4> returns a pointer to the terminating null character in the
**	   destination array.
**	5> returns a null pointer if the arguments are bogus.
**
**********************************************************************/

char * EXP_LVL3 CS_stncat (char *dest,Const char *source,int count)
{
	int length;
	int room;
	char *cPtr;

	cPtr = NULL;
	if (count > 0)
	{
		/* Determine how much room we have in the target character array. */
		length = (int)strlen (dest);
		room = count - length - 1;
		if (room >= 0)
		{
			cPtr = dest + length;
			if (room > 0)
			{
				/* Append the appropriate number of characters on to the end of target
				   array. */
				cPtr = CS_stncp (cPtr,source,room);
			}
		}
	}
	return cPtr;
}

/**********************************************************************
**	CS_tmpfn (tmp_fnm);
**
**	char tmp_fnm [];			location at which the temporary file name
**								is to be created.
**
**	cs_TMPFN_MAXED  Function could not generate a unique name
**					after 626 tries.
**
**	The returned value is always a full path name which will
**	cause the file to be located in the same directory as
**	the Coordinate System Dictionary, et al.
**
**	We should have, perhaps, added a size argument which would
**	help prevent overflow of the provided string array.  However,
**	the function is used very little and it simply needs an
**	argument which points to a character array dimensioned
**	at 80 or some similar number.  The returned path will
**	always be identical to that contained in cs_Dir, and
**	the file name will be of the form DEL_ME.$AA.
**
**	Notice, to keep performance up in an executable which may
**	be using several temporary files simultaneously, the seed
**	for the algorithm is a static variable.  Therefore, if your
**	executable creates hundreds of temporary files, it is very
**	likely that this algorithm will get max'ed out.
**********************************************************************/

static char unique [4] = "ZZ";

int EXP_LVL7 CS_tmpfn (char *tmp_fnm)

{
	extern char cs_Dir [];
	extern char *cs_DirP;

	char cc_save;

	int count;

	char *cp;

	/* Establish the drive and the directory. */

	cc_save = *cs_DirP;
	*cs_DirP = '\0';
	cp = CS_stcpy (tmp_fnm,cs_Dir);
	*cs_DirP = cc_save;

	/* Add a file name. */

	cp = CS_stcpy (cp,"DEL_ME.$");

	/* Add an extension which will make this file unique in the
	   directory.  Hopefully, we will never need to iterate
	   more than once since tempoary files should always be
	   deleted after their use. */

	count = 0;
	strcpy (unique,"ZZ");
	do
	{
		count++;
		if (count >= 676)
		{
			CS_erpt (cs_TMPFN_MAXED);
			return (-1);
		}

		/* Bump the unique portion of the name by one, wrapping
		   arround if necessary. */

		if (unique [1] >= 'Z')
		{
			unique [1] = 'A';
			if (unique [0] >= 'Z')
			{
				unique [0] = 'A';
			}
			else unique [0]++;
		} else unique [1]++;

		/* Add the unique two character name generated above
		   to the result. */
		
		(void) CS_stcpy (cp,unique);

		/* We continue generating names until we find one
		   that doesn't exist.  In this case, access
		   does just what we need it to. */

	} while (CS_access (tmp_fnm,0) == 0);

	/* If we get here, we have made a unique name. */

	return (0);
}

/**********************************************************************
**	tab_ptr = CS_tpars (pntr,table,tab_size);
**
**	char **pntr;				pointer to a the pointer which defines the
**								beginning of the parse.  Pointed to pointer
**								is updated.
**	void *(table []);			pointer to a structure array.  See remarks
**								below for important information about this
**								structure array.
**	int tab_size;				size of the structure in the structure array.
**	void *tab_ptr;				returns a pointer to the structure array
**								element which caused parsed, else NULL.
**
**	The structure used in the structure array passed to this
**	routine can be any type of structure the user wishes
**	to use.  The first element in the structure, however,
**	must be an unsigned char, the second element of the
**	structure must be a character array.  The character
**	array contains an allowable token for the parse.
**	The unsigned char which is the initial element
**	must contain the length of the parse token.  The
**	structure array must be terminated with an entry
**	with a zero string length.
**
**	For example, the following table could be used to
**	parse directions:
**
**	struct {
**		unsigned char str_len;
**		char dir [3];
**		unsigned short dir_code;
**		.
**		.	 Any other stuff you want.
**		.
**	} dir_tab [] =
**	{ 2,"NW",NORTH | WEST},
**	{ 2,"NE",NORTH | EAST},
**	{ 2,"SW",SOUTH | WEST},
**	{ 2,"SE",SOUTH | EAST},
**	{ 1,"N",NORTH},
**	{ 1,"S",SOUTH},
**	{ 1,"E",EAST},
**	{ 1,"W",WEST},
**	{ 0,NULL,0}
**	};
**
**	The CS_tpars routine compares the current parse location
**	in the string and looks for a match.  If a match
**	is found, it returns a pointer to the matching
**	element in the structure array.  The value in *pntr
**	is updated to point at the next character after
**	the match.  If a match is not found, the NULL
**	pointer is returned and the *pntr value remains
**	unchanged.
**
**	Note that the longest strings must appear before
**	the shorter ones.  That is, if the NORTH entry
**	appeared first, this routine would never find a
**	NW or a NE entry.
**
**	The tab_size argument to the function must provide
**	the size of the structure being used.
**********************************************************************/

Const void * EXP_LVL9 CS_tpars (char **pntr,Const void *table,int tab_size)
{
	Const char *tab_ptr;

	tab_ptr = (Const char *)table;
	while (*tab_ptr != '\0')
	{
		if (!CS_strnicmp ((tab_ptr + 1),*pntr,(unsigned char)*tab_ptr))
		{
			break;
		}
		tab_ptr += tab_size;
	}
	if (*tab_ptr)
	{
		*pntr += (unsigned char)*tab_ptr;
	}
	else
	{
		tab_ptr = NULL;
	}
	return (tab_ptr);
}

/**********************************************************************
**	len = CS_trim (string);
**
**	char *string;				string which is to be trimmed.
**								Result returned in same location.
**	int len;					returns the length of the result.
**********************************************************************/
int EXP_LVL7 CS_trim (char *string)
{
	cs_Register char *pp;
	cs_Register char *qq;

	pp = qq = string;

	/* Skip over any leading white space. */

	while (*pp != '\0' && (*pp == ' '  ||
						   *pp == '\t' ||
						   *pp == '\n' ||
						   *pp == '\r')) pp++;

	/* pp now points to the first non-white space
	   character, which could very well be the null
	   character which terminates the string.  Copy
	   the remainder to front of the array. */

	do {} while ((*qq++ = *pp++) != '\0');
	qq -= 2;

	/* qq now points to the last character of the string.
	   We now need to skip over any trailing white space
	   and insert a null character to terminate the string
	   before the trailing white space. */

	while (qq >= string && (*qq == ' ' || *qq == '\t' || *qq == '\n' || *qq == '\r')) qq--;

	/* qq now points to the last non-white space character
	   in the string. */

	*++qq = '\0';

	return (int)(qq - string);
}
int EXP_LVL7 CS_trimWc (wchar_t *string)
{
	cs_Register wchar_t *pp;
	cs_Register wchar_t *qq;

	pp = qq = string;

	/* Skip over any leading white space. */

	while (*pp != L'\0' && (*pp == L' '  ||
							*pp == L'\t' ||
							*pp == L'\n' ||
							*pp == L'\r')) pp++;

	/* pp now points to the first non-white space
	   character, which could very well be the null
	   character which terminates the string.  Copy
	   the remainder to front of the array. */
	do {} while ((*qq++ = *pp++) != L'\0');
	qq -= 2;

	/* qq now points to the last character of the string.
	   We now need to skip over any trailing white space
	   and insert a null character to terminate the string
	   before the trailing white space. */

	while (qq >= string && (*qq == L' ' || *qq == L'\t' ||
										   *qq == L'\n' ||
										   *qq == L'\r')) qq--;

	/* qq now points to the last non-white space character
	   in the string. */

	*++qq = L'\0';

	return (int)(qq - string);
}

Const char* EXP_LVL3 CS_stristr (Const char *str1,Const char *str2)
{
	int status;
	int cmpCnt;
	size_t strLen;

	strLen = strlen (str2);
	if (strLen == 0) return NULL;
	status = -1;
	cmpCnt = (int)strlen (str1) - (int)strLen + 1;
	while (cmpCnt-- > 0)
	{
		if ((status = CS_strnicmp (str1,str2,strLen)) == 0) break;
		str1 += 1;
	}
	return status ? NULL : str1;
}
long32_t EXP_LVL5 CS_degToSec (double degrees)
{
	extern double cs_Deg2Sec;

	long32_t seconds;

	if (degrees >= 0.0)
	{
		seconds = (long32_t)((degrees + 1.0E-12) * cs_Deg2Sec);
	}
	else
	{
		seconds = (long32_t)((degrees - 1.0E-12) * cs_Deg2Sec);
	}
	return seconds;
}
void EXP_LVL5 CSasciiToXml (char *xml,const char *ascii)
{
	const char *srcPtr;
	char *trgPtr;

	char cc;

	srcPtr = ascii;
	trgPtr = xml;
	while ((cc = *srcPtr++) != '\0')
	{
		if      (cc == '&' ) trgPtr = CS_stcpy (trgPtr,"&amp;");
		else if (cc == '<' ) trgPtr = CS_stcpy (trgPtr,"&lt;");
		else if (cc == '>' ) trgPtr = CS_stcpy (trgPtr,"&gt;");
		else if (cc == '"' ) trgPtr = CS_stcpy (trgPtr,"&quot;");
		else if (cc == '\'') trgPtr = CS_stcpy (trgPtr,"&apos;");
		else                 *trgPtr++ = cc;
	}
	*trgPtr = '\0';
	return;
}

/* Double comparison.  Compares doubles regardless of their magnitude.
   Ignores noise in the two or three least significant bits of the mantissa.
   Returns TRUE if the two values are essentially equal.  Note that the
   sign of the values comes into play when comparing the mantissas.  Thus
   a very very small negative value will successfully compare with a
   very very small positive value. */
int EXP_LVL1 CS_cmpDbls (double first,double second)
{
	int exp1, exp2;
	double mant1,mant2;

	mant1 = frexp (first ,&exp1);
	mant2 = frexp (second,&exp2);

	/* Need to deal with the case where the difference between the exponents
	   is +/-1.  This is necessary as the two values may be infinitesimally
	   greater and less than an even true binary value like 0.5.  In this
	   case, they would be equal, but the exponents would not be the same. */
	if ((exp1 - exp2) == 1)
	{
		/* exp of first is bigger than exp of second.   Adjust exp2 and mant2
		   so that they still represent the same value, but exp2 is now the
		   same as exp1.  Note, this de-normalizes the numbers,  but that
		   does not affect the result in any way. */
		exp2 += 1;
		mant2 *= 0.5;
	}
	else if ((exp1 - exp2) == -1)
	{
		exp1 += 1;
		mant1 *= 0.5;
	}

	/* Now, if the exponents are not equal, obviously the original values
	   are not equal; not even close. */
	if (exp1 != exp2) return FALSE;

	/* If the exponents are equal, then we can simply compare the mantissas.
	   We ignore any difference in the last few bits.  We use 5.0E-12 as
	   we assume there are 12 digits of precision in a 'double'.  We would
	   use a much larger value if we were comparing 'float's. */
	return (int)(fabs (mant1 - mant2) < 5.0E-12);
}
/* Environment variable substitution.  This function will replace references
   to environmental variables in strings in place. The return value is zero
   if no subsitutions took place, 1 if one or more substitutions took place,
   and -1 if some sort of error (string buffer overflow most common) took
   place. Note, for performance and simplicity reasons, it is considered
   an error to provide a string buffer whose size exceeds MAXPATH bytes.

   THIS FUNCTION DOES NOT DO RECURSIVE SUBSTITUTIONS.  If you need/want
   that, you need to call this function multiple times until you get a
   zero return value. */
   
/*lint -esym(613,envValPtr)  possible use of null pointer; I don't think so. */
int CS_envsub (char* stringBufr,size_t bufrSize /*in chars*/)
{
	extern char cs_EnvchrC;
	extern char cs_EnvStartC;
	extern char cs_EnvEndC;
	extern char csErrnam [MAXPATH];

	enum envSubState {	envSubBegin = 0,
						envSubCopy,
						envSubExtractL1,
						envSubExtractL2,
						envSubExtractW,
						envSubGetValue,
						envSubValueCopy,
						envSubComplete,
						envSubDone,
						envSubError
					 } state;

	int subCount = 0;
	int envNameCount = 0;
	int trgBufrCount = 0;

	char* srcPtr;
	char* trgPtr;
	char* envNamePtr;
	char* envValPtr;

	char envNameBufr [MAXPATH + 16];
	char wrkBufr [MAXPATH + 16];

	if ((stringBufr == 0) || (strlen (stringBufr) >= MAXPATH))
	{
		CS_erpt (cs_INV_ARG1);
		goto error;
	}
	if (bufrSize > MAXPATH)
	{
		CS_erpt (cs_INV_ARG2);
		goto error;
	}

	trgPtr = wrkBufr;				// Redundant, to preclude uninitialized warning.
	envNamePtr = envNameBufr;		// Redundant, to preclude uninitialized warning.
	envValPtr = 0;					// Redundant, to preclude uninitialized warning.

	state = envSubBegin;
	srcPtr = stringBufr;			// To kep lint happy.
	while (state != envSubDone && state != envSubError)
	{
		switch (state)	{
		case envSubBegin:
			subCount = 0;
			envNameCount = 0;
			trgBufrCount = 0;
			srcPtr = stringBufr;
			trgPtr = wrkBufr;
			state = envSubCopy;
			break;
		case envSubCopy:
			if (*srcPtr != cs_EnvchrC)
			{
				trgBufrCount += 1;
				if ((*trgPtr++ = *srcPtr++) == '\0')
				{
					state = envSubComplete;
				}
			}
			else
			{
				/* Prepare to capture the environmental variable name. */
				envNamePtr = envNameBufr;
				*envNamePtr = '\0';

				/* Skip the intro character, whatever it is. */
				srcPtr += 1;
				if (cs_EnvchrC == '$')
				{
					/* Here in the case of Lunix/Unix/et al.  The environmental
					   variable name may be enclosed within brackets, and
					   maybe not. */
					if (*srcPtr == cs_EnvStartC)
					{
						srcPtr += 1;
						state = envSubExtractL2;
					}
					else
					{
						state = envSubExtractL1;
					}
				}
				else
				{
					state = envSubExtractW;
				}
			}
			break;

		case envSubExtractL1:
			if (CS_isalnum (*srcPtr) || *srcPtr == '_' || *srcPtr == '\0')
			{
				/* A name character, copy it to the variable name buffer. */
				*envNamePtr++ = *srcPtr++;
				envNameCount += 1;
			}
			else
			{
				/* A character which terminates the environmental variable
				   name. */
				*envNamePtr = '\0';
				envNameCount += 1;
				state = envSubGetValue;
			}
			break;
		case envSubExtractL2:
			/* Here to extract the environmental variable name, terminated
			   by the cs_EnvEndC character. */
			if (*srcPtr == '\0')
			{
				CS_stncp (csErrnam,stringBufr,32);
				CS_erpt (cs_ENV_FORMAT);
				state = envSubError;
			}
			else if (*srcPtr != cs_EnvEndC)
			{
				*envNamePtr++ = *srcPtr++;
			}
			else
			{
				*envNamePtr = '\0';
				envNameCount += 1;
				srcPtr += 1;
				state = envSubGetValue;
			}
			break;
		case envSubExtractW:
			/* Here to extract a variable name surrounded by the cs_EnvchrC
			   character, as in windows using cs_EnvchrC = '%'. */
			if (*srcPtr == '\0')
			{
				CS_stncp (csErrnam,stringBufr,32);
				CS_erpt (cs_ENV_FORMAT);
				state = envSubError;
			}
			else if (*srcPtr != cs_EnvchrC)
			{
				*envNamePtr++ = *srcPtr++;
				envNameCount += 1;
			}
			else
			{
				*envNamePtr = '\0';
				envNameCount += 1;
				srcPtr += 1;
				state = envSubGetValue;
			}
			break;
		case envSubGetValue:
			envValPtr = CS_getenv (envNameBufr);
			if (envValPtr == 0)
			{
				CS_stncp (csErrnam,envNameBufr,32);
				CS_erpt (cs_ENV_NOVAR);
				state = envSubError;
			}
			else
			{
				state = envSubValueCopy;
			}
			break;
		case envSubValueCopy:
			if (*envValPtr != '\0')
			{
				*trgPtr++ = *envValPtr++;		/*lint !e613  null pointer? I don't think so. */
				trgBufrCount += 1;
			}
			else
			{
				subCount += 1;
				state = envSubCopy;
			}
			break;
		case envSubComplete:
			CS_stncp (stringBufr,wrkBufr,(int)bufrSize);
			state = envSubDone;
			break;
		case envSubDone:
			/* Should never get here.  Primarily here to keep lint
			   happy. */
			CS_stncp (csErrnam,"CS_supprt:8",MAXPATH);
			CS_erpt (cs_ISER);
		case envSubError:			/*lint !e616  deliberate flow through */
		default:
			goto error;
			break;					/*lint !e527  unreachable code. */
		}
		if (trgBufrCount >= (MAXPATH - 1))
		{
			CS_erpt (cs_ENV_TOOLONG);
			state = envSubError;
		}
		if (envNameCount >= (MAXPATH - 1))
		{
			CS_stncp (csErrnam,stringBufr,32);
			CS_erpt (cs_ENV_FORMAT);
			state = envSubError;
		}
	}
	return subCount;
error:
	return -1;

}

/*lint +esym(613,envValPtr) */
int CS_envsubWc (wchar_t* stringBufr,size_t bufrSize /*in chars*/)
{
	int subCount;
	char workBufr [MAXPATH];

	if ((stringBufr == 0) || (wcslen (stringBufr) >= MAXPATH))
	{
		CS_erpt (cs_INV_ARG1);
		goto error;
	}
	if (bufrSize > MAXPATH)
	{
		CS_erpt (cs_INV_ARG2);
		goto error;
	}

	wcstombs (workBufr,stringBufr,MAXPATH);				/*lint !e534  ignoring return value */
	workBufr [MAXPATH - 1] = '\0';
	subCount = CS_envsub (workBufr,MAXPATH);
	if (subCount > 0)
	{
		mbstowcs (stringBufr,workBufr,bufrSize);		/*lint !e534  ignoring return value */
	}
	return subCount;
error:
	return -1;
}
