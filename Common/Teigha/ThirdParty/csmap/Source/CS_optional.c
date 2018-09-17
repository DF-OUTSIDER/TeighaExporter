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
#include <ctype.h>

/**********************************************************************
**	flags = CS_atof (result,ascii);
**
**	char *ascii;				the ASCII string which is to be converted.
**	double *result;				returns the processed result.
**	long32_t flags;					returns information about the format processed,
**								specifically, if an error occurred.
**
**	The returned fmt value provides much information about the
**	value created. Most importantly, if the value is negative,
**	a formatting error of some sort was encountered.  You can
**	examine the rest of the returned value to determine the
**	nature of the error.
**
**	The following bits are used in the returned value:
**
**	Bit 31:	set indiciates an error of some sort, may be serious,
**		maybe not.
**	Bits 0-4: The number of digits ecountered after the decimal
**		point. We don't process more than nine, we don't
**		count past 15.
**	Bit 5:	set if a variation of degrees, minutes, and seconds
**		format was encountered. Zero says a pure decimal
**		number was processed.
**	Bit 6:	set if the number included degrees and minutes only,
**		i.e. there were no seconds.
**	Bit 7:	set if there was an ordinary sign character of some
**		sort, i.e. plus or minus.
**	Bit 8:	set if there was a directional character, i.e.
**		one of NSEWnsew.
**	Bit 9:	set if the information processed indicates a longitude.
**	Bit 10:	set if the information processed indicates a latitude.
**	Bit 16:	set if there were no numeric characters to process.
**	Bit 17: set if there were more than one sign indiciation.
**********************************************************************/

/*
	The following function converts character strings to
	doubles, assuming that the input MAY be in degrees
	minutes and seconds format.
*/

#define EXPONENT 1
#define DEGREES  2
#define MINUTES  3
#define SECONDS  4
#define FRACTION 5
#define BOGUS    6


long32_t EXP_LVL1 CS_atof (double *result,Const char *value)
{
	extern char cs_DecpntC;
	extern char cs_DigsepC;
	extern char cs_RatioC;

	long32_t dblFrmt;

	dblFrmt = CSatof (result,value,cs_DecpntC,cs_DigsepC,cs_RatioC);
	return dblFrmt;
}
long32_t EXP_LVL7 CSatof (double *result,Const char *value,char rdxChr,char sepChr,char ratioChr)
{
	extern char cs_LngDir [4];
	extern char cs_LatDir [4];
	extern double cs_Zero;
	extern double cs_One;
	extern double cs_Ten;
	extern double cs_K90;
	extern double cs_K180;
	extern double cs_K360;
	extern double cs_Sec2Deg;
	extern double cs_Min2Deg;

	char cc;
	char dir_cc;

	int mode;
	int minus;
	int ratio;
	int sgn_flg;
	int err_flg;
	int lng_dir;
	int lat_dir;
	int deg_cnt;
	int min_cnt;
	int sec_cnt;
	int exp_cnt;
	int bog_cnt;
	int tot_cnt;
	int frac_cnt;
	int exp_minus;
	int last_cma;

	long32_t minutes;
	long32_t seconds;
	long32_t exponent;
	long32_t bogus;
	long32_t rtn_val;
	long32_t digit;

	char *cp;
	Const char *ccp;

	double degrees;
	double fraction;
	double divr;

	char my_copy [64];

	/* Deal with the null string input. */

	if (value == NULL || *value == '\0')
	{
		*result = cs_Zero;
		return ((long32_t)cs_ATOF_0BLNK);
	}

	/* Copy the value to my_copy.  In so doing we:
		1) trim leading and trailing white space;
		2) check for exponential notation;
		3) reduce all consecutive non-numeric characters to a
		   single space;
		4) capture a leading minus sign, or a trailing
		   directional character. */

	ccp = value;
	cp = my_copy;
	rtn_val = 0L;
	sgn_flg = FALSE;
	minus = FALSE;
	ratio = FALSE;
	err_flg = 0;
	lng_dir = FALSE;
	lat_dir = FALSE;
	last_cma = 0;

	/* Skip over any leading junk, but capture a sign character
	   while doing so. Also, if we wee the initial ration sequence
	   of "1:", we capture, and skip, that as well. */

	while (*ccp != '\0' && (*ccp < '0' || *ccp > '9') && *ccp != rdxChr)
	{
		if (*ccp == '-' || *ccp == '+')
		{
		        sgn_flg = TRUE;
		        if (*ccp == '-') minus = TRUE;
		}
		if (*ccp == '1' && *(ccp + 1) == ratioChr)
		{
			ratio = TRUE;
			ccp += 1;
		}
		ccp += 1;
	}
	if (*ccp == '1' && *(ccp + 1) == ratioChr)
	{
		ratio = TRUE;
		ccp += 2;
	}

	/* Copy the interesting part of any remaining stuff. */

	dir_cc = '\0';
	tot_cnt = 63;
	while (tot_cnt > 0 && (cc = *ccp++) != '\0')
	{
		/* See if we have encountered an exponential
		   notation sequence. Need to be careful here,
		   E might be a directional character. */

		if ((cc == 'E' || cc == 'e') &&
			(*ccp == '+' || *ccp == '-' || (*ccp >= '0' && *ccp <= '9'))
		   )
		{
			/* Yup, we have an exponential reference. */

			if (*ccp == '-')
			{
				*cp++ = 'e';
				ccp += 1;
			}
			if (*ccp == '+')
			{
				*cp++ = 'E';
				ccp += 1;
			}
			else
			{
				*cp++ = 'E';
			}
			tot_cnt -= 1;
			continue;
		}

		/* Save dir_cc as the last non-space character. */

		if (cc != ' ') dir_cc = cc;

		/* Replace all non numeric stuff with a single space.
		   We let commas and decimal points through. */

		if (cc != rdxChr && cc != sepChr &&
		    (cc < '0' || cc > '9'))
		{
			/* Replace all consecutive non-numeric stuff
			   with a single space. (cp - 1) is OK
			   here as we know the first character to
			   be either a digit or a decimal point. */

			if (*(cp - 1) != ' ')
			{
				tot_cnt -= 1;
				*cp++ = ' ';
			}
		}
		else
		{
			tot_cnt -= 1;
			*cp++ = cc;
		}
	}
	*cp = '\0';
	if (dir_cc == cs_LngDir [2] || dir_cc == cs_LngDir [3])
	{
		lng_dir = TRUE;
		if (sgn_flg) rtn_val |= cs_ATOF_MLTSGN;
		minus = TRUE;
	}
	if (dir_cc == cs_LngDir [0] || dir_cc == cs_LngDir [1])
	{
		lng_dir = TRUE;
		if (sgn_flg) rtn_val |= cs_ATOF_MLTSGN;
	}
	if (dir_cc == cs_LatDir [2] || dir_cc == cs_LatDir [3])
	{
		lat_dir = TRUE;
		if (sgn_flg) rtn_val |= cs_ATOF_MLTSGN;
		minus = TRUE;
	}
	if (dir_cc == cs_LatDir [0] || dir_cc == cs_LatDir [1])
	{
		lat_dir = TRUE;
		if (sgn_flg) rtn_val |= cs_ATOF_MLTSGN;
	}
	if ((rtn_val & cs_ATOF_MLTSGN) != 0)
	{
		CS_erpt (cs_A2F_MLTSGN);
		err_flg = TRUE;
	}
	if (lat_dir || lng_dir)
	{
		rtn_val |= cs_ATOF_DIRCHR;
	}
	if (lng_dir && !lat_dir)
	{
		rtn_val |= cs_ATOF_XEAST;
	}

	/* Process the number. We have four modes:
		DEGREES;
		MINUTES;
		SECONDS:
		FRACTIONS.

	   In any given mode, lptr points to a long which is used to
	   accumulate the value being processed, icnt points to an
	   integer which is used to count the number of characters
	   entered in the specified mode. Non-numeric characters
	   cause a mode shift. After all is parsed, the counts are
	   examined to see what got processed, and how to build the
	   resulting double. */

	degrees  = cs_Zero;
	minutes  = 0L;
	seconds  = 0L;
	fraction = cs_Zero;
	divr = cs_One;
	exponent = 0L;
	deg_cnt  = 0;
	min_cnt  = 0;
	sec_cnt  = 0;
	frac_cnt = 0;
	exp_cnt  = 0;
	exp_minus= FALSE;
	bogus    = 0;
	bog_cnt  = 0;
	last_cma = -1;
	mode = DEGREES;

	cp = my_copy;
	while ((cc = *cp++) != '\0')
	{
		if (cc == 'e')
		{
			mode = EXPONENT;
			exp_minus = TRUE;
			continue;
		}
		if (cc == 'E')
		{
			mode = EXPONENT;
			continue;
		}
		if (cc == sepChr)
		{
			if (mode != DEGREES ||
			    (last_cma >= 0 && last_cma != 3))
			{
				rtn_val |= cs_ATOF_ERRCMA;
				CS_erpt (cs_A2F_ERRCMA);
				err_flg = TRUE;
			}
			rtn_val |= cs_ATOF_COMMA;
			last_cma = 0;
			continue;
		}
		if (cc == rdxChr)
		{
			if (last_cma > 0 && last_cma != 3)
			{
				rtn_val |= cs_ATOF_ERRCMA;
				CS_erpt (cs_A2F_ERRCMA);
				err_flg = TRUE;
			}
			if (mode == FRACTION)
			{
				rtn_val |= cs_ATOF_MLTPNT;
				CS_erpt (cs_A2F_MLTPNT);
				err_flg = TRUE;
			}
			mode = FRACTION;
			continue;
		}
		if (cc < '0' || cc > '9')
		{
			if (mode == EXPONENT) mode = BOGUS;
			else		      mode += 1;
			continue;
		}
		digit = cc - '0';
		if (last_cma >= 0) last_cma += 1;

		switch (mode) {

		case DEGREES:

			if (deg_cnt == 0 && digit == 0)
			{
				rtn_val |= cs_ATOF_DEG0;
			}
			deg_cnt += 1;
			degrees = degrees * cs_Ten + (double)digit;
			break;

		case MINUTES:

			if (min_cnt == 0 && digit == 0)
			{
				rtn_val |= cs_ATOF_MINSEC0;
			}
			min_cnt += 1;
			if (min_cnt <= 9) minutes = minutes * 10L + digit;
			break;

		case SECONDS:

			if (sec_cnt == 0 && digit == 0)
			{
				rtn_val |= cs_ATOF_MINSEC0;
			}
			sec_cnt += 1;
			if (sec_cnt <= 9) seconds = seconds * 10L + digit;
			break;

		case FRACTION:

			frac_cnt += 1;
			divr /= cs_Ten;
			fraction += (double)digit * divr;
			break;

		case EXPONENT:

			exp_cnt += 1;
			if (exp_cnt <= 9) exponent = exponent * 10L + digit;
			break;

		default:

			bog_cnt += 1;
			if (bog_cnt <= 9) bogus = bogus * 10L + digit;
			break;
		}
	}

	/* Figure out the result. */

	*result = degrees;
	if (sec_cnt > 0)
	{
		rtn_val |= cs_ATOF_MINSEC;
		*result += (double)minutes * cs_Min2Deg;
		*result += ((double)seconds + fraction) * cs_Sec2Deg;
	}
	else if (min_cnt > 0)
	{
		rtn_val |= cs_ATOF_MINUTE;
		*result += ((double)minutes + fraction) * cs_Min2Deg;
	}
	else if (frac_cnt > 0)
	{
		rtn_val |= cs_ATOF_FRACTN;
		*result += fraction;
	}
	if ((sec_cnt + min_cnt) > 0 && deg_cnt >= 3)
	{
		rtn_val |= cs_ATOF_FORCE3;
	}

	if (exp_cnt > 0)
	{
		rtn_val |= cs_ATOF_EXPNT;
		*result *= pow (cs_Ten,(double)(exp_minus ? -exponent : exponent));
	}

	if (ratio)
	{
		if (lng_dir || lat_dir || min_cnt != 0 || sec_cnt != 0 || minus)
		{
			rtn_val |= cs_ATOF_RATERR;
			err_flg = TRUE;
			CS_erpt (cs_A2F_RATIO);
		}
		rtn_val |= cs_ATOF_RATIO;
		if (fabs (*result) < 1.0E-12)
		{
			/* Let's not divide by zero. */
			*result = cs_Zero;
			rtn_val |= cs_ATOF_RATERR;
			err_flg = TRUE;
			CS_erpt (cs_A2F_RATIO);
		}
		else
		{
			*result = cs_One - (cs_One / *result);
		}
	}
	else
	{
		if (*result <= cs_K180 && !lat_dir) rtn_val |= cs_ATOF_VALLNG;
		if (*result <= cs_K90  && !lng_dir) rtn_val |= cs_ATOF_VALLAT;
		if (*result >  cs_K180 && *result <= cs_K360) rtn_val |= cs_ATOF_RNG360;
	}
	if (minus) *result = -(*result);

	if (seconds >= 60)
	{
		rtn_val |= cs_ATOF_SECS60;
		CS_erpt (cs_A2F_SECS60);
		err_flg = TRUE;
	}
	if (minutes >= 60)
	{
		rtn_val |= cs_ATOF_MINS60;
		CS_erpt (cs_A2F_MINS60);
		err_flg = TRUE;
	}
	if (frac_cnt > 14) frac_cnt = 14;
	if (bog_cnt > 0 || (deg_cnt + frac_cnt) == 0)
	{
		rtn_val |= cs_ATOF_FMTERR;
		CS_erpt (cs_A2F_FMTERR);
		err_flg = TRUE;
	}
	rtn_val |= (frac_cnt + 1);
	if (err_flg) rtn_val |= cs_ATOF_ERRFLG;		/*lint !e737 !e713 */

	return (rtn_val);
}

/**********************************************************************
**	actual = CS_ftoa (bufr,size,value,frmt);
**
**	char *bufr;					the buffer in which the result is to be
**								returned.
**	int size;					size of the array pointed to by bufr.
**	double value;				the value to be converted.
**	long32_t frmt;					a bit map of options according to the
**								format rertuned by CS_atof.
**	long32_t actual;				returns the actual format used to
**								output the value.
**********************************************************************/

long32_t EXP_LVL1 CS_ftoa (char *bufr,int size,double value,long32_t frmt)
{
	extern double cs_Zero;
	extern double cs_Half;
	extern double cs_One;
	extern double cs_Ten;
	extern double cs_K60;
	extern double cs_K360;

	extern char cs_DecpntC;
	extern char cs_DigsepC;
	extern char cs_RatioC;
	extern char cs_LngDir [4];
	extern char cs_LatDir [4];

	char lead_ch;

	int ii;
	int jj;
	int left;
	int prec;
	int minus;
	int ratio;
	int dummy;
	int exponent;
	int sgn_idx;
	int dec_pnt;
	int dir_chr;
	int ch_req;
	int min_flg;
	int sec_flg;
	int cma_flg;
	int lng_flg;
	int auto_p;
	int digits;
	int dig_cnt;
	int lead_zero;

	int degrees;
	int minutes;

	char *cp;
	Const char *cpK;

	double log_val;
	double prec_d;
	double dbl_tmp;

	char local [32];

	dir_chr = FALSE;
	sec_flg = FALSE;
	min_flg = FALSE;
	lng_flg = FALSE;
	*bufr = '\0';

	/* Handle the easy cases first. */

	if (size < 1) return (0x80000000L);				/*lint !e569 */
	if ((frmt & cs_ATOF_0BLNK) && value == 0.0)
	{
		return ((long32_t)cs_ATOF_0BLNK);
	}

	/* If range 0 <--> 360 is indicated, and appropriate, do that
	   adjustment now. */

	if (((frmt & cs_ATOF_RNG360) != 0) && value >= -180.0 && value < 0.0)
	{
		value += cs_K360;
	}

	/* Below, we primarily deal with positive numbers. */

 	minus = FALSE;
	if (value < 0.0)
	{
		minus = TRUE;
		value = -value;
	}

	/* Extract the essential information from the requested
	   format. */

	cma_flg = ((frmt & cs_ATOF_COMMA) != 0);
	ratio   = ((frmt & cs_ATOF_RATIO) != 0);
	if (ratio)
	{
		value = fabs (value);
		if (value <= 0.5 || value >= 0.99999999)
		{
			ratio = FALSE;
			frmt = frmt & cs_ATOF_PRCMSK;
			if (frmt < 6L) frmt = 6L;
		}
		else
		{
			value = cs_One / (cs_One - value);
		}
	}
	else
	{
		dir_chr = ((frmt & cs_ATOF_DIRCHR) != 0);
		sec_flg = ((frmt & cs_ATOF_MINSEC) != 0);
		min_flg = sec_flg | ((frmt & cs_ATOF_MINUTE) != 0);		/*lint !e514 */
		lng_flg = (min_flg || sec_flg) &&
		          (((frmt & cs_ATOF_FORCE3) != 0) || (value >= 100.0));
	}

	/* The log (base 10) tells us how many digits we need on the
	   left side of the decimal point, and also the value we
	   will need to place after the E in exponential format.

	   We assume that the smallest number we will deal with is
	   1.0E-19. After all, this is a "mapping" program. */

	if (value > 1.0E-19)
	{
		log_val = log10 (value);
	}
	else
	{
		log_val = -19.0;
	}
	left = exponent = (int)log_val;
	if (log_val < 0.0)
	{
		/* Value is less than one. */

		exponent -= 1;
		lead_zero = 1;
		left = 0;
	}
	else
	{
		/* Value is greater than one. */

		left += 1;
		lead_zero = 0;
	}

	if (exponent >= 3)
	{
		/* Value is greater than 999, we disable minutes
		   and seconds. */

		lng_flg = FALSE;
		sec_flg = FALSE;
		min_flg = FALSE;
		frmt &= ~(cs_ATOF_VALLNG | cs_ATOF_VALLAT | cs_ATOF_MINSEC | cs_ATOF_MINUTE);
	}

	/* Extract the precision and set up any automatic value. */

	prec = (int)(frmt & cs_ATOF_PRCMSK);
	if (prec == 0)
	{
		/* A precision of zero indicates automatic
		   precision. */

		auto_p = TRUE;
		if (sec_flg)
		{
			prec = 2;
		}
		else if (min_flg)
		{
			prec = 4;
		}
		else
		{
			prec = 11 - left;
		}
	}
	else
	{
		/* Otherwise, the extracted precision is one
		   greater than the desired precision. */

		auto_p = FALSE;
		prec -= 1;
	}

	/* Protect against the extremes. */

	if (prec < 0) prec = 0;
	if (prec > 14) prec = 14;

	frmt &= ~cs_ATOF_PRCMSK;
	frmt |= (prec + 1);

	if (ratio)
	{
		ch_req = 2 + left + lead_zero + prec + (prec != 0);		/*lint !e514 */
	}
	else
	{
		ch_req = (minus || dir_chr) + left + lead_zero + prec + (prec != 0);	/*lint !e514 */
	}
	if (min_flg) ch_req += 3;
	if (sec_flg) ch_req += 3;
	if (cma_flg) ch_req += (left / 3);

	/* If we don't have enough room for the requested format,
	   we start choosing alternatives until we find an alternative
	   that will fit.

	   Note, that 99.99% of the time, the value will fit in the
	   space provided.  Therefore, the rather inefficient code
	   given below is rarely executed.  We have favored
	   simplicity of the logic, rather than pure efficiency. */

	if (ch_req >= size && cma_flg)
	{
		/* Turn off the comma flag. */

		frmt &= ~cs_ATOF_COMMA;
		cma_flg = FALSE;
		ch_req -= (left / 3);
	}

	if (ch_req >= size && min_flg && sec_flg)
	{
		/* Switch from degrees minutes and seconds to
		   decimal degrees. */

		frmt &= ~(cs_ATOF_MINSEC | cs_ATOF_MINUTE);
		min_flg = FALSE;
		sec_flg = FALSE;
		lng_flg = FALSE;
		if (prec != 0)
		{
			prec += 5;
			ch_req -= 1;
		}
		else prec = 6;
	}

	if (ch_req >= size && auto_p)
	{
		/* If automatic precision, we try reducing
		   precision to as low as 1. */

		ii = ch_req - size + 1;
		if ((prec - ii) > 0)
		{
			prec -= ii;
			ch_req -= ii;
		}
	}	

	if (ch_req >= size && prec > 6)
	{
		/* Reduce the precision. */

		ch_req -= (prec - 6);
		prec = 6;
	}

	if (ch_req >= size)
	{
		/* Switch to exponential notation. */

		frmt = cs_ATOF_EXPNT;
		ch_req = minus + 6;
	}

	if (ch_req >= size)
	{
		/* All has failed, we'll have to output an overflow
		   indication. */

		frmt =  cs_ATOF_OVRFLW;
	}

	/* We now have frmt, which indicates how the value can be
	   formatted to fit into the space available, and we have
	   digits, which indicates the number of digits involved.
	   Format the number as indicated by the current format
	   specification.
	   
	   First, we write the ratio preamble if appropriate. */

	if (ratio)
	{
		*bufr++ = '1';
		*bufr++ = cs_RatioC;
	}

	/* Now the value. */

	if ((frmt & cs_ATOF_OVRFLW) != 0)
	{
		/* Can't fit anything meaningful, just output
		   an overflow indication. */

		if (size > 3) *bufr++ = '*';
		if (size > 2) *bufr++ = cs_DecpntC;
		if (size > 1) *bufr++ = '*';
	}
	else if ((frmt & cs_ATOF_EXPNT) != 0)
	{
		/* Use exponential notation. */

		digits = size - minus - 6;
		if (abs (exponent) >= 100) digits -= 1;
		prec = digits - 1;

		/* Convert the value, yielding exactly "digits"
		   digits. */

		cpK = CS_ecvt (value,digits,&dec_pnt,&dummy);

		/* Get a local copy of the converted value which
		   we can modify. */

		strncpy (local,cpK,sizeof (local));
		local [sizeof (local) - 1] = '\0';
		cp = local;

		/* Construct the ASCII value. */

		if (minus) *bufr++ = '-';
		*bufr++ = *cp++;
		*bufr++ = cs_DecpntC;
		while (*cp != '\0') *bufr++ = *cp++;
		*bufr++ = 'E';
		if (exponent >= 0) *bufr++ = '+';
		else
		{
			*bufr++ = '-';
			exponent = -exponent;
		}
		if (exponent > 99)
		{
			*bufr++ = '0' + (char)(exponent / 100);
			exponent %= 100;
		}
		*bufr++ = '0' + (char)(exponent / 10);
		*bufr++ = '0' + (char)(exponent % 10);
	}
	else
	{
		/* Here for a normal format. Deal with a preceeding sign
		   character; that's common to all cases. */

		if (minus && !dir_chr) *bufr++ = '-';

		/* If we have degrees, minutes, and seconds, we need to
		   compute the minutes and seconds first, perform round
		   off, and then output the integral portions.

		   In the case of minutes, precision indicates the precision
		   of the minutes conversion. Similarly with seconds.

		   Digits is the number of digits we will use below to
		   convert the remaining real portion of value in
		   order to get the desired precision. */

		digits = left + prec;
		degrees = -10;
		minutes = -10;
		if (sec_flg || min_flg)
		{
			prec_d = pow (cs_Ten,(double)(-prec)) * cs_Half;
			degrees = (int)value;
			value = (value - (double)degrees) * cs_K60;
			if (!sec_flg)
			{
				/* Minutes round off. */

				prec_d = pow (cs_Ten,(double)(-prec)) * cs_Half;
				dbl_tmp = value + prec_d;
				if (dbl_tmp >= cs_K60)
				{
					value = cs_Zero;
					degrees += 1;
				}
			}
			else
			{
				/* Deal with seconds. */

				minutes = (int)value;
				value = (value - (double)minutes) * cs_K60;

				/* Value is now the number of seconds,
				   in double form.  We need to apply
				   a round off, and possibly extend
				   this round off to the minutes and
				   degrees values. */

				dbl_tmp = value + prec_d;
				if (dbl_tmp >= cs_K60)
				{
					value = cs_Zero;
					minutes += 1;
					if (minutes >= 60)
					{
						minutes = 0;
						degrees += 1;
					}
				}
			}

			/* How many digits? */

			digits = 2 + prec;
			if (value < cs_Ten) digits -= 1;
			if (value < cs_One) digits -= 1;
		}

		/* Output the degrees. If the longitude flag is
		   set, we always output three characters.  If not,
		   we output exactly two characters. If degrees
		   is greater than or equal 100, we obviously have
		   to output 3 digits. We never use deg/min/sec
		   if the value is 1000 or greater. */

		if (degrees >= 0)
		{
			lead_ch = ((frmt & cs_ATOF_DEG0) != 0) ? '0' : ' ';
			dig_cnt = (lng_flg) ? 3 : 2;

			/* Insert the appropriate number of fill
			   characters. */

			for (ii = 0;ii < dig_cnt;ii++) *(bufr + ii) = lead_ch;

			/* Insert the real characters, makeing sure we
			   output at least one real digit. */

			ii = dig_cnt - 1;
			do
			{
				*(bufr + ii) = (char)(degrees % 10) + '0';
				degrees /= 10;
				ii -= 1;
			} while (degrees > 0 && ii >= 0);
			bufr += dig_cnt;

			/* Separate degrees from minutes. */

			*bufr++ = ' ';
		}

		/* Output the integral minutes if seconds are being
		   output. */

		if (minutes >= 0)
		{
			/* Similar to the above for degrees, but
			   digit cont (dig_cnt) is always 2 in this
			   case. */

			lead_ch = ((frmt & cs_ATOF_MINSEC0) != 0) ? '0' : ' ';
			*bufr++ = (minutes >= 10) ? (char)(minutes / 10) + '0' : lead_ch;
			*bufr++ = (char)(minutes % 10) + '0';

			/* Separate minutes from seconds. */

			*bufr++ = ' ';

			/* Output a leading character for seconds if approriate. */

			if (value < cs_Ten) *bufr++ = lead_ch;
		}

		/* Now we output value. If we are not doing minutes and
		   seconds, value is the entire value we started with.
		   Otherwise, it is the minutes value, or the seconds
		   value. In any case, it is a double value, and digits
		   indicates the number of digits we are to convert in
		   order to get the requested precision. */

		cpK = CS_ecvt (value,digits,&dec_pnt,&dummy);

		/* Get a copy I can modify. */

		CS_stncp (local,cpK,sizeof (local));
		cp = local;

		/* Compute the number of digits to output, given the
		   requested precision.  We force the resulting
		   string to terminate at that point. */

		ii = dec_pnt + prec;
		if (ii > 0)
		{
			/* Here if there is some significance to
			   the digits in the local array. We terminate
			   the string in local so that the number of
			   digits, including leading zeros, matches
			   the precision we are attempting to produce. */

			local [ii] = '\0';
		}
		else
		{
			/* Here if the value is so small that no
			   significant digits can be output in the
			   requested precision.  In this case, we
			   dummy up local to provide as many zeros
			   as the precision value we are attempting
			   to produce.

			   Note, in so doing, we are in essence moving
			   the decimal point to the '0' position.
			   That is, we are saying that the decimal
			   point shoul preceed the first digit in
			   the local array.

			   In the case of automatic precision, (auto_p
			   == TRUE) we simply outout a single zero.
			   This simply eliminates the need for the
			   code below to strip them back off. */

			dig_cnt = (auto_p) ? 1 : prec;
			for (ii = 0;ii < dig_cnt;ii++) local [ii] = '0';
			local [ii] = '\0';

			/* Setting dec_pnt to zero says the decimal point
			   is immediately to the left of the first digit. */

			dec_pnt = 0;
		}

		/* If the precision is automatic, we trim trailing
		   zeros. We always leave at least one to the right
		   of the decimal point. */

		if (auto_p)
		{
			/* dec_pnt should never be negative here without
			   there being at least one one non-zero digit
			   in local.  However, just in case my brain
			   is soft today: */

			ii = (dec_pnt > 0) ? dec_pnt : 0;

			/* Trim trailing zeros, but never check the
			   digit immediately following the decimal
			   point. */

			jj = (int)strlen (cp) - 1;
			while (jj > ii)
			{
				if (*(cp + jj) != '0') break;
				*(cp + jj) = '\0';
			 	jj -= 1;
			}
		}

		/* If the value is less than 1.0, we output the
		   initial zero (i.e. the one before the decimal
		   point.  If value is less than 0.1, we
		   output the decimal point and all necessary
		   leading zeros.  If dec_pnt is zero, the code
		   code below will output the decimal point, so
		   we don't do it here. */

		if (dec_pnt <= 0)
		{
			*bufr++ = '0';
			if (dec_pnt < 0)
			{
				*bufr++ = cs_DecpntC;
				for (ii = dec_pnt;ii < 0;ii++)
				{
					*bufr++ = '0';
				}
			}
		}

		/* Copy the digits to the output buffer, inserting commas
		   and decimal point in the appropriate locations.

		   Note, dec_pnt is the number of digits to the left
		   of the decimal point. */

		for (ii = 0;*cp != '\0';ii += 1)		/*lint !e441   loop variable (idx) not referenced in conditional expression */
		{
			if (cma_flg && ii != 0)
			{
				/* jj is the number of digits yet to
				   be placed which are left of the
				   decimal point. */

				jj = dec_pnt - ii;
				if (jj > 0 && (jj % 3) == 0)
				{
					*bufr++ = cs_DigsepC;
				}
			}

			/* Output the decimal point if this is the
			   spot for it. */

			if (ii == dec_pnt) *bufr++ = cs_DecpntC;

			/* Now, we can copy the actual digit. */

			*bufr++ = *cp++;
		}

		/* If a directional character was requested, we add
		   it now. */

		if (dir_chr)
		{
			sgn_idx = (minus) ? 2 : 0;
			*bufr++ = (frmt & cs_ATOF_XEAST) ? cs_LngDir [sgn_idx] : cs_LatDir [sgn_idx];
		}
	}
	if (size > 0) *bufr = '\0';
	return (frmt);
}

/**********************************************************************
**	status = CS_spZoneNbr (zoneNbr,is83);
**
**	char *zoneNbr;				pointer to a chracter array of not less
**								than cs_KEYNM_DEF characters which
**								contains the zone number to be converted.
**								Resulting key name is returned in this
**								location.
**	int is83;					non-zero says that the zone number is to be
**								interpreted as a NAD83 state plane zone
**								number.  Otherwise, NAD27 is assumed.
**	int status;					returns a positive value for success,
**								negative for failure.  Zero is returned
**								is a translation actually occurred. See
**								Remarks below.
**
**	cs_SPZ_INVALID				is reported through the error system
**								should an invalid code value be supplied.
**
**	Designed to be used as follows:
**
**		int st;
**		struct cs_Csprm_ *xsprm;
**		char keyName [24];
**		GetKeyName (keyName,sizeof (keyName));
**		csprm = NULL;
**		st = CS_spZoneNbr (keyName,1);
**		if (st >= 0)
**		{
**			csprm = CS_csloc (keyName);
**		}
**		if (csprm == NULL)
**		{
**			alert (CS_errmsg);
**		}
**
**	This implies that CS_spZoneNbr will replace the coordinate
**	system name with the alternative is indeed the keyName
**	array contains a valid state plane zone number.  CS_spZoneNbr
**	does nothing to the supplied array if the initial contents
**	do not look like a state plane zone number.
**
**	A value presented to CS_spZoneNbr is considered a valid
**	zone number field if it contains 3 or 4 decimal degits. A
**	value which does not meet this restriction is not considered
**	a state plane zone number, CS_spZoneNbr does nothing
**	silently, nd returns a zero.
**
**	CS_spZoneNbr returns a positive 1 if a substitution is made.
**	CS_csZoneNbr returns a zero if it decides that the value
**	provided was not that of a state plane zone number as described
**	above.  CS_spZoneNbr returns a -1 if: 1) the value provided
**	consisted of 3 or 4 decimal digits, and 2) the resulting numeric
**	value was not a valid state plane zone number (given the status
**	of the is83 flag.
**********************************************************************/
/*lint -e785   too few initializers for aggrate (cs-ZoneMap) */
/*lint -esym(754,cs_ZoneMap_::type)   not referenced */
/*lint -esym(754,cs_ZoneMap_::fill)   not referenced */
struct cs_ZoneMap_
{
	short zoneNbr;
	char keyName27 [24];
	char keyName83 [24];
	char type;				/* T = Transverse Mercator, L = Lambert Conformal COnic, etc. */
	char fill;
};

/* TODO:  This feature should be merged with the "FIPS" feature in CS_fips.c */
struct cs_ZoneMap_ cs_ZoneMap [] =
{
	{  101,   "AL-E",     "AL83-E",'T'  },
	{  102,   "AL-W",     "AL83-W",'T'  },

	{  201,   "AZ-E",     "AZ83-E",'T'  },
	{  202,   "AZ-C",     "AZ83-C",'T'  },
	{  203,   "AZ-W",     "AZ83-W",'T'  },

	{  301,   "AR-N",     "AR83-N",'L'  },
	{  302,   "AR-S",     "AR83-S",'L'  },

	{  401,   "CA-I",     "CA83-I",'L'  },
	{  402,  "CA-II",    "CA83-II",'L'  },
	{  403, "CA-III",    "CA83III",'L'  },
	{  404,  "CA-IV",    "CA83-IV",'L'  },
	{  405,   "CA-V",     "CA83-V",'L'  },
	{  406,  "CA-VI",    "CA83-VI",'L'  },
	{  407, "CA-VII",          "" ,'L'  },

	{  501,   "CO-N",     "CO83-N",'L'  },
	{  502,   "CO-C",     "CO83-C",'L'  },
	{  503,   "CO-S",     "CO83-S",'L'  },

	{  600,     "CT",       "CT83",'L'  },

	{  700,     "DE",       "DE83",'T'  },

	{  901,   "FL-E",     "FL83-E",'T'  },
	{  902,   "FL-W",     "FL83-W",'T'  },
	{  903,   "FL-N",     "FL83-N",'L'  },

	{ 1001,   "GA-E",     "GA83-E",'T'  },
	{ 1002,   "GA-W",     "GA83-W",'T'  },

	{ 1101,   "ID-E",     "ID83-E",'T'  },
	{ 1102,   "ID-C",     "ID83-C",'T'  },
	{ 1103,   "ID-W",     "ID83-W",'T'  },

	{ 1201,   "IL-E",     "IL83-E",'T'  },
	{ 1202,   "IL-W",     "IL83-W",'T'  },

	{ 1301,   "IN-E",     "IN83-E",'T'  },
	{ 1302,   "IN-W",     "IN83-W",'T'  },

	{ 1401,   "IA-N",     "IA83-N",'L'  },
	{ 1402,   "IA-S",     "IA83-S",'L'  },

	{ 1501,   "KS-N",     "KS83-N",'L'  },
	{ 1502,   "KS-S",     "KS83-S",'L'  },

	{ 1601,   "KY-N",     "KY83-N",'L'  },
	{ 1602,   "KY-S",     "KY83-S",'L'  },

	{ 1701,   "LA-N",     "LA83-N",'L'  },
	{ 1702,   "LA-S",     "LA83-S",'L'  },
	{ 1703,   "LA-O",     "LA83-O",'L'  },

	{ 1801,   "ME-E",     "ME83-E",'T'  },
	{ 1802,   "ME-W",     "ME83-W",'T'  },

	{ 1900,     "MD",       "MD83",'L'  },

	{ 2001,     "MA",       "MA83",'L'  },
	{ 2002,  "MA-IS",    "MA83-IS",'L'  },

	{ 2111,   "MI-N",     "MI83-N",'L'  },
	{ 2112,   "MI-C",     "MI83-C",'L'  },
	{ 2113,   "MI-S",     "MI83-S",'L'  },

	{ 2201,   "MN-N",     "MN83-N",'L'  },
	{ 2202,   "MN-C",     "MN83-C",'L'  },
	{ 2203,   "MN-S",     "MN83-S",'L'  },

	{ 2301,   "MS-E",     "MS83-E",'T'  },
	{ 2302,   "MS-W",     "MS83-W",'T'  },

	{ 2401,   "MO-E",     "MO83-E",'T'  },
	{ 2402,   "MO-C",     "MO83-C",'T'  },
	{ 2403,   "MO-W",     "MO83-W",'T'  },

	{ 2500,       "",       "MT83",'L'  },
	{ 2501,   "MT-N",           "",'L'  },
	{ 2502,   "MT-C",           "",'L'  },
	{ 2502,   "MT-S",           "",'L'  },

	{ 2600,       "",       "NE83",'L'  },
	{ 2601,   "NE-N",           "",'L'  },
	{ 2602,   "NE-S",           "",'L'  },

	{ 2701,   "NV-E",     "NV83-E",'T'  },
	{ 2702,   "NV-C",     "NV83-C",'T'  },
	{ 2703,   "NV-W",     "NV83-W",'T'  },

	{ 2800,     "NH",       "NH83",'T'  },

	{ 2900,     "NJ",       "NJ83",'T'  },

	{ 3001,   "NM-E",     "NM83-E",'T'  },
	{ 3002,   "NM-C",     "NM83-C",'T'  },
	{ 3003,   "NM-W",     "NM83-W",'T'  },

	{ 3101,   "NY-E",     "NY83-E",'T'  },
	{ 3102,   "NY-C",     "NY83-C",'T'  },
	{ 3103,   "NY-W",     "NY83-W",'T'  },
	{ 3104,   "NY-L",     "NY83-L",'L'  },

	{ 3200,     "NC",       "NC83",'T'  },

	{ 3301,   "ND-N",     "ND83-N",'L'  },
	{ 3302,   "ND-S",     "ND83-S",'L'  },

	{ 3401,   "OH-N",     "OH83-N",'L'  },
	{ 3402,   "OH-S",     "OH83-S",'L'  },

	{ 3501,   "OK-N",     "OK83-N",'L'  },
	{ 3502,   "OK-S",     "OK83-S",'L'  },

	{ 3601,   "OR-N",     "OR83-N",'L'  },
	{ 3602,   "OR-S",     "OR83-S",'L'  },

	{ 3701,   "PA-N",     "PA83-N",'L'  },
	{ 3702,   "PA-S",     "PA83-S",'L'  },

	{ 3800,     "RI",       "RI83",'T'  },

	{ 3900,     "SC",       "SC83",'T'  },

	{ 4001,   "SD-N",     "SD83-N",'L'  },
	{ 4002,   "SD-S",     "SD83-S",'L'  },

	{ 4100,     "TN",       "TN83",'T'  },

	{ 4201,   "TX-N",     "TX83-N",'L'  },
	{ 4202,  "TX-NC",    "TX83-NC",'L'  },
	{ 4203,   "TX-C",     "TX83-C",'L'  },
	{ 4204,  "TX-SC",    "TX83-SC",'L'  },
	{ 4205,   "TX-S",     "TX83-S",'L'  },

	{ 4301,   "UT-N",     "UT83-N",'L'  },
	{ 4302,   "UT-C",     "UT83-C",'L'  },
	{ 4303,   "UT-S",     "UT83-S",'L'  },

	{ 4400,     "VT",       "VT83",'T'  },

	{ 4501,   "VA-N",     "VA83-N",'L'  },
	{ 4502,   "VA-S",     "VA83-S",'L'  },

	{ 4601,   "WA-N",     "WA83-N",'L'  },
	{ 4602,   "WA-S",     "WA83-S",'L'  },

	{ 4701,   "WV-N",     "WV83-N",'L'  },
	{ 4702,   "WV-S",     "WV83-S",'L'  },

	{ 4801,   "WI-N",     "WI83-N",'L'  },
	{ 4802,   "WI-C",     "WI83-C",'L'  },
	{ 4803,   "WI-S",     "WI83-S",'L'  },

	{ 4901,   "WY-E",     "WY83-E",'T'  },
	{ 4902,  "WY-EC",    "WY83-EC",'T'  },
	{ 4903,  "WY-WC",    "WY83-WC",'T'  },
	{ 4904,   "WY-W",     "WY83-W",'T'  },

	{ 5001,   "AK-1",     "AK83-1",'O'  },
	{ 5002,   "AK-2",     "AK83-2",'T'  },
	{ 5003,   "AK-3",     "AK83-3",'T'  },
	{ 5004,   "AK-4",     "AK83-4",'T'  },
	{ 5005,   "AK-5",     "AK83-5",'T'  },
	{ 5006,   "AK-6",     "AK83-6",'T'  },
	{ 5007,   "AK-7",     "AK83-7",'T'  },
	{ 5008,   "AK-8",     "AK83-8",'T'  },
	{ 5009,   "AK-9",     "AK83-9",'T'  },
	{ 5010,  "AK-10",    "AK83-10",'T'  },

	{ 5101,   "HI-1",     "HI83-1",'T'  },
	{ 5102,   "HI-2",     "HI83-2",'T'  },
	{ 5103,   "HI-3",     "HI83-3",'T'  },
	{ 5104,   "HI-4",     "HI83-4",'T'  },
	{ 5105,   "HI-5",     "HI83-5",'T'  },

	{ 5200,     "PR",       "PR83",'T'  },

	{    0,       "",           "",'\0' }
};
/*lint +e785 */

int EXP_LVL1 CS_spZoneNbrMap (char *zoneNbr,int is83)
{
	extern char csErrnam [];

	char cc;
	short zone;
	short length;
	char *cp;
	struct cs_ZoneMap_ *zPtr;

	/* Verify that we have something that looks like a state
	   plane zone code. */

	cp = zoneNbr;
	while ((cc = *cp++) != '\0')
	{
		if (!isdigit (cc)) return (1);
	}
	length = (short)strlen (zoneNbr);
	if (length < 3 || length > 4)
	{
		CS_stncp (csErrnam,zoneNbr,MAXPATH);
		CS_erpt (cs_SPZ_INVALID);
		return (-1);
	}

	/* Look the zone code up in the table. */

	zone = (short)atoi (zoneNbr);
	if (zone < 101 || zone > 5200)
	{
		CS_stncp (csErrnam,zoneNbr,MAXPATH);
		CS_erpt (cs_SPZ_INVALID);
		return (-1);
	}
	for (zPtr = cs_ZoneMap;zPtr->zoneNbr != 0;zPtr += 1)
	{
		if (zPtr->zoneNbr == zone) break;
	}
	if (zPtr->zoneNbr == 0)
	{
		CS_stncp (csErrnam,zoneNbr,MAXPATH);
		CS_erpt (cs_SPZ_INVALID);
		return (-1);
	}

	/* Return the proper value.  If value is '\0', then
	   the zone number is not valid for the indicated
	   datum. */

	if (is83 != 0)
	{
		if (zPtr->keyName83 [0] == '\0')
		{
			/* ZOne number is a valid NAD27 number, but
			   not a valid 83 number. */

			CS_stncp (csErrnam,zoneNbr,MAXPATH);
			CS_erpt (cs_SPZ_INVALID);
			return (-1);	
		}
		CS_stncp (zoneNbr,zPtr->keyName83,cs_KEYNM_DEF);
	}
	else
	{
		if (zPtr->keyName27 [0] == '\0')
		{
			CS_stncp (csErrnam,zoneNbr,MAXPATH);
			CS_erpt (cs_SPZ_INVALID);
			return (-1);	
		}
		CS_stncp (zoneNbr,zPtr->keyName27,cs_KEYNM_DEF);
	}
	return (0);
}

/**********************************************************************
**	st = CS_dynutm (csprm,zone);
**
**	struct m_Csprm_ *csprm;		pointer to the coordinate system definition
**								which is to be modified.
**	int zone;					this argument indicates the number of the
**								desired UTM zone.
**	int st;						returns zero for success. Otherwise, returns
**								a positive CS-MAP error code indicating
**								that csprm did not point to a UTM coordinate
**								system.
**
**	This function is expressly intended for applications where
**	the application dynamically switches from one UTM zone to
**	the next.  Rather than refetch a definition from disk and
**	reinitialize it, or cache numerous definitions in memory,
**	we simply change the central meridian of the coordinate
**	system dynamically.  This, fortunately, is very easy to
**	do for the Transverse Mercator projection.
**
**	CSdynutm performs some basic checking to see that the
**	target coordinate system does indeed appear to be a UTM zone.
**	Checking includes the all the parameters which are common to
**	all UTM zones: projection, scale reduction, central meridian,
**	false easting and false northing.
**********************************************************************/

int EXP_LVL3 CS_dynutm (struct cs_Csprm_ *csprm,int zone)
{
	extern char csErrnam [];

	extern double cs_Degree;		/* 0.17 */

	double centLng;

	/* Before making any changes to the coordinate system pointed to
	   by csprm, we make a few, well mayabe a lot of, checks. */

	if (csprm->prj_code != cs_PRJCOD_UTM) goto error;

	/* To change the central meridian, all we need do is change the
	   cent_lng element of the m_Trmer_ structure. Note, that this
	   value is in radians rather than degrees.

	   Of course, we need to determine the longitude of the
	   central meridian having been given the zone number. */

	centLng = (double)(-183 + (6 * zone)) * cs_Degree;		/*lint !e790 */
	csprm->proj_prms.trmer.cent_lng = centLng;
	return (0);

error:
	CS_stncp (csErrnam,csprm->csdef.key_nm,cs_KEYNM_DEF);
	CS_erpt (cs_DYMUTM);
	return (cs_DYMUTM);
}

int EXP_LVL3 CS_utmzon (double lng)
{
	extern double cs_K180;
	extern double cs_K360;

	int zone;
	int ilng;

	ilng = (int)fmod ((lng + cs_K180),cs_K360);
	zone = (ilng / 6) + 1;
	return (zone);
}

/**********************************************************************
**	crc16 = CS_crc16 (crc16,bufr,count);
**
**	unsigned short crc16;		initial value, or value from previous buffer.
**	char *bufr;					pointer to the current buffer to be checked.
**	int count;					number of bytes in buffer to be included in
**								the CRC generation.
**	unsigned short crc16;		returns the accumulated CRC value.
**********************************************************************/

unsigned short csCrc16Tabl [256] =
{
 0X0000, 0X1021, 0X2042, 0X3063, 0X4084, 0X50A5, 0X60C6, 0X70E7,
 0X8108, 0X9129, 0XA14A, 0XB16B, 0XC18C, 0XD1AD, 0XE1CE, 0XF1EF,
 0X1231, 0X0210, 0X3273, 0X2252, 0X52B5, 0X4294, 0X72F7, 0X62D6,
 0X9339, 0X8318, 0XB37B, 0XA35A, 0XD3BD, 0XC39C, 0XF3FF, 0XE3DE,
 0X2462, 0X3443, 0X0420, 0X1401, 0X64E6, 0X74C7, 0X44A4, 0X5485,
 0XA56A, 0XB54B, 0X8528, 0X9509, 0XE5EE, 0XF5CF, 0XC5AC, 0XD58D,
 0X3653, 0X2672, 0X1611, 0X0630, 0X76D7, 0X66F6, 0X5695, 0X46B4,
 0XB75B, 0XA77A, 0X9719, 0X8738, 0XF7DF, 0XE7FE, 0XD79D, 0XC7BC,
 0X48C4, 0X58E5, 0X6886, 0X78A7, 0X0840, 0X1861, 0X2802, 0X3823,
 0XC9CC, 0XD9ED, 0XE98E, 0XF9AF, 0X8948, 0X9969, 0XA90A, 0XB92B,
 0X5AF5, 0X4AD4, 0X7AB7, 0X6A96, 0X1A71, 0X0A50, 0X3A33, 0X2A12,
 0XDBFD, 0XCBDC, 0XFBBF, 0XEB9E, 0X9B79, 0X8B58, 0XBB3B, 0XAB1A,
 0X6CA6, 0X7C87, 0X4CE4, 0X5CC5, 0X2C22, 0X3C03, 0X0C60, 0X1C41,
 0XEDAE, 0XFD8F, 0XCDEC, 0XDDCD, 0XAD2A, 0XBD0B, 0X8D68, 0X9D49,
 0X7E97, 0X6EB6, 0X5ED5, 0X4EF4, 0X3E13, 0X2E32, 0X1E51, 0X0E70,
 0XFF9F, 0XEFBE, 0XDFDD, 0XCFFC, 0XBF1B, 0XAF3A, 0X9F59, 0X8F78,
 0X9188, 0X81A9, 0XB1CA, 0XA1EB, 0XD10C, 0XC12D, 0XF14E, 0XE16F,
 0X1080, 0X00A1, 0X30C2, 0X20E3, 0X5004, 0X4025, 0X7046, 0X6067,
 0X83B9, 0X9398, 0XA3FB, 0XB3DA, 0XC33D, 0XD31C, 0XE37F, 0XF35E,
 0X02B1, 0X1290, 0X22F3, 0X32D2, 0X4235, 0X5214, 0X6277, 0X7256,
 0XB5EA, 0XA5CB, 0X95A8, 0X8589, 0XF56E, 0XE54F, 0XD52C, 0XC50D,
 0X34E2, 0X24C3, 0X14A0, 0X0481, 0X7466, 0X6447, 0X5424, 0X4405,
 0XA7DB, 0XB7FA, 0X8799, 0X97B8, 0XE75F, 0XF77E, 0XC71D, 0XD73C,
 0X26D3, 0X36F2, 0X0691, 0X16B0, 0X6657, 0X7676, 0X4615, 0X5634,
 0XD94C, 0XC96D, 0XF90E, 0XE92F, 0X99C8, 0X89E9, 0XB98A, 0XA9AB,
 0X5844, 0X4865, 0X7806, 0X6827, 0X18C0, 0X08E1, 0X3882, 0X28A3,
 0XCB7D, 0XDB5C, 0XEB3F, 0XFB1E, 0X8BF9, 0X9BD8, 0XABBB, 0XBB9A,
 0X4A75, 0X5A54, 0X6A37, 0X7A16, 0X0AF1, 0X1AD0, 0X2AB3, 0X3A92,
 0XFD2E, 0XED0F, 0XDD6C, 0XCD4D, 0XBDAA, 0XAD8B, 0X9DE8, 0X8DC9,
 0X7C26, 0X6C07, 0X5C64, 0X4C45, 0X3CA2, 0X2C83, 0X1CE0, 0X0CC1,
 0XEF1F, 0XFF3E, 0XCF5D, 0XDF7C, 0XAF9B, 0XBFBA, 0X8FD9, 0X9FF8,
 0X6E17, 0X7E36, 0X4E55, 0X5E74, 0X2E93, 0X3EB2, 0X0ED1, 0X1EF0
};

unsigned short EXP_LVL9 CS_crc16 (unsigned short crc16,unsigned char *cp,int count)
{
	while (--count >= 0)
	{
		crc16 = (unsigned short)(crc16 << 8) ^ csCrc16Tabl [(crc16 >> 8) ^ *cp++];
	}
	return (crc16);
}
