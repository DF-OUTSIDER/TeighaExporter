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

/*lint -esym(613,err_list) */

int EXP_LVL9 CSbdkasQ (struct cs_GeodeticTransform_ *gxDef, unsigned short xfrmCode, int err_list [], int list_sz)
{
	extern double cs_DelMax;
	extern double cs_RotMax;
	extern double cs_SclMax;

	int err_cnt;

	/* We will return (err_cnt + 1) below. */
	err_cnt = -1;
	if (err_list == NULL) list_sz = 0;

	/* Check the definition stuff specific to csBdkas_ */
	if (fabs (gxDef->parameters.geocentricParameters.deltaX) > cs_DelMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_DELTAX;
	}
	if (fabs (gxDef->parameters.geocentricParameters.deltaY) > cs_DelMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_DELTAY;
	}
	if (fabs (gxDef->parameters.geocentricParameters.deltaZ) > cs_DelMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_DELTAZ;
	}
	if (fabs (gxDef->parameters.geocentricParameters.rotateX) > cs_RotMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_ROTATEX;
	}
	if (fabs (gxDef->parameters.geocentricParameters.rotateY) > cs_RotMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_ROTATEY;
	}
	if (fabs (gxDef->parameters.geocentricParameters.rotateZ) > cs_RotMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_ROTATEZ;
	}
	if (fabs (gxDef->parameters.geocentricParameters.scale) > cs_SclMax)
	{
		if (++err_cnt < list_sz) err_list [err_cnt] = cs_DTQ_BWSCALE;
	}
	return (err_cnt + 1);
}
/******************************************************************************
	What we call the Bursa/Wolfe transformation is an approximation of the
	Seven Parameter transformation.  The following approximations apply:

	1> The cosine of all rotation angles is one.
	2> The sine of all rotation angles is equal to the angle itself.
	3> The product of two sine terms is zero.
*/
int EXP_LVL9 CSbdkasS (struct cs_GxXform_* gxXfrm)
{
	extern double cs_One;				/* 1.0 */
	extern double cs_Sec2Rad;			/* 4.848...E-06 */

	double trgScale;
	struct csBdkas_ *badekas;

	badekas = NULL;
	badekas = &gxXfrm->xforms.bdkas;
	
	badekas->srcERad = gxXfrm->srcDatum.e_rad;
	badekas->srcESqr = gxXfrm->srcDatum.ecent * gxXfrm->srcDatum.ecent;

	badekas->trgERad = gxXfrm->trgDatum.e_rad;
	badekas->trgESqr = gxXfrm->trgDatum.ecent * gxXfrm->trgDatum.ecent;

	badekas->deltaX = gxXfrm->gxDef.parameters.geocentricParameters.deltaX;
	badekas->deltaY = gxXfrm->gxDef.parameters.geocentricParameters.deltaY;
	badekas->deltaZ = gxXfrm->gxDef.parameters.geocentricParameters.deltaZ;
	badekas->rotX   = gxXfrm->gxDef.parameters.geocentricParameters.rotateX * cs_Sec2Rad;
	badekas->rotY   = gxXfrm->gxDef.parameters.geocentricParameters.rotateY * cs_Sec2Rad;
	badekas->rotZ   = gxXfrm->gxDef.parameters.geocentricParameters.rotateZ * cs_Sec2Rad;
    
    badekas->xlateX = gxXfrm->gxDef.parameters.geocentricParameters.translateX;
    badekas->xlateY = gxXfrm->gxDef.parameters.geocentricParameters.translateY;
    badekas->xlateZ = gxXfrm->gxDef.parameters.geocentricParameters.translateZ;

	trgScale = cs_One + (gxXfrm->gxDef.parameters.geocentricParameters.scale * 1.0E-06);
	if (fabs (trgScale) < 1.0E-06)
	{
		// An actual scale factor of zero would produce a floating point
		// exception.  Can't allow that.
		CS_erpt (cs_ISER);
	}
	else
	{
		badekas->scale = trgScale;
	}

	badekas->errorValue    = gxXfrm->errorValue;
	badekas->cnvrgValue    = gxXfrm->cnvrgValue;
	badekas->maxIterations = gxXfrm->maxIterations;

	gxXfrm->frwrd2D = (cs_FRWRD2D_CAST)CSbdkasF2;
	gxXfrm->frwrd3D = (cs_FRWRD3D_CAST)CSbdkasF3;
	gxXfrm->invrs2D = (cs_INVRS2D_CAST)CSbdkasI2;
	gxXfrm->invrs3D = (cs_INVRS3D_CAST)CSbdkasI3;
	gxXfrm->inRange = (cs_INRANGE_CAST)CSbdkasL;
	gxXfrm->release = (cs_RELEASE_CAST)CSbdkasR;
	gxXfrm->destroy = (cs_DESTROY_CAST)CSbdkasD;

	return 0;
}
int EXP_LVL9 CSbdkasF3 (struct csBdkas_ *badekas, double trgLl [3], Const double srcLl [3])
{
	double xx, yy, zz;
	double xyz [3];
    double xTranslated, yTranslated, zTranslated;

	/* Convert the geographic coordinates to geocentric XYZ coordinates. */
	CS_llhToXyz (xyz, srcLl, badekas->srcERad, badekas->srcESqr);

    /* Copied from http://info.ogp.org.uk/geodesy/guides/docs/G7-2.pdf, page 111

     | Xt |       |  1   +Rz  -Ry |   | Xs - Xp |   | Xp |   | dX |
     | Yt | = M * | -Rz   1   +Rx | * | Ys - Yp | + | Yp | + | dY |
     | Zt |       | +Ry  -Rx   1  |   | Zs - Zp |   | Zp |   | dZ |
    
    (dX, dY, dZ) : Translation vector, to be added to the point's position vector in the source coordinate system
                    in order to transform from source coordinate reference system to target coordinate reference system; also: the
                    coordinates of the origin of source coordinate reference system in the target frame.

    (Rx, Ry, Rz) : Rotations to be applied to the coordinate reference frame. The sign convention is such that a
                    positive rotation of the frame about an axis is defined as a clockwise rotation of the coordinate reference
                    frame when viewed from the origin of the Cartesian coordinate system in the positive direction of that axis,
                    that is a positive rotation about the Z-axis only from source coordinate reference system to target coordinate
                    reference system will result in a smaller longitude value for the point in the target coordinate reference
                    system. Although rotation angles may be quoted in any angular unit of measure, the formula as given here
                    requires the angles to be provided in radians.

    (Xp, Yp, Zp) : Coordinates of the point about which the coordinate reference frame is rotated, given in the
                    source Cartesian coordinate reference system.

    M            : The scale factor to be applied to the position vector in the source coordinate reference system
                    in order to obtain the correct scale of the target coordinate reference system. M = (1+dS*10-6), where dS is
                    the scale correction expressed in parts per million.
    
    */

    /*
    Adjust these cartesian coordinates via the Bursa/Wolf transformation.
	   First, we apply the rotation matrix in a computationally effieicent
	   form.  That is, several elements of the matrix are assumed to have
	   fixed values. */

    /* Apply the rotation by making sure, we've considered the XYZ(p)
        This is (Xs - Xp)
    */

    xTranslated = xyz[XX] - badekas->xlateX;
    yTranslated = xyz[YY] - badekas->xlateY;
    zTranslated = xyz[ZZ] - badekas->xlateZ;

    xx = xTranslated + (badekas->rotZ * yTranslated) + (-badekas->rotY * zTranslated);
	yy = (-badekas->rotZ * xTranslated) + yTranslated + (badekas->rotX * zTranslated);
	zz = (badekas->rotY * xTranslated) + (-badekas->rotX * yTranslated) + zTranslated;

    /* Apply the scale */
	xx *= badekas->scale;
	yy *= badekas->scale;
	zz *= badekas->scale;

	/* Apply the rotation point values + delta*/
	xyz [XX] = xx + badekas->xlateX + badekas->deltaX;
	xyz [YY] = yy + badekas->xlateY + badekas->deltaY;
	xyz [ZZ] = zz + badekas->xlateZ + badekas->deltaZ;

	/* Convert the new X, Y, and Z back to latitude and longitude.
	   CS_xyzToLlh returns degrees. */

	return CS_xyzToLlh (trgLl, xyz, badekas->trgERad, badekas->trgESqr);
}
int EXP_LVL9 CSbdkasF2 (struct csBdkas_ *badekas, double* trgLl, Const double* srcLl)
{
	extern double cs_Zero;				/* 0.0 */

	int status;
	double lcl_ll [3];

	trgLl [LNG] = lcl_ll [LNG] = srcLl [LNG];
	trgLl [LAT] = lcl_ll [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	//set height to zero - not to be considered here
    lcl_ll [HGT] = cs_Zero;
	status = CSbdkasF3 (badekas, lcl_ll, lcl_ll);
	if (status >= 0)
	{
		trgLl [LNG] = lcl_ll [LNG];
		trgLl [LAT] = lcl_ll [LAT];
	}
	return status;
}
int EXP_LVL9 CSbdkasI3 (struct csBdkas_ *badekas, double* trgLl, Const double* srcLl)
{
    int status;

	double xx, yy, zz;
	double xyz [3];

	/* Convert the geographic coordinates to geocentric XYZ coordinates. */
	CS_llhToXyz (xyz, srcLl, badekas->trgERad, badekas->trgESqr);
	
	// Invert the scaling and translation.
	xx = (xyz [XX] - badekas->deltaX - badekas->xlateX) / badekas->scale;
	yy = (xyz [YY] - badekas->deltaY - badekas->xlateY) / badekas->scale;
	zz = (xyz [ZZ] - badekas->deltaZ - badekas->xlateZ) / badekas->scale;

	// Apply the inverse of the rotation matrix.  Essentially, this code
	// is equivalent to the multiplication by the transpose of the original
	// rotation matrix.  This works as the matrix is close to being orthogonal.

	//???// In this case, not only is the underlying Coordinate Frame rotation
	// matrix not perfectly orthogonal, but also the evaluation point for a
	// perfect inverse would need to be determined, if we are to get creep down
	// to normal standards.
	xyz [XX] =                      xx - badekas->rotZ *    yy + badekas->rotY *    zz;
	xyz [YY] =  badekas->rotZ *     xx +                    yy - badekas->rotX *    zz;
	xyz [ZZ] = -badekas->rotY *     xx + badekas->rotX *    yy +                    zz;
	
	xyz [XX] += badekas->xlateX;
	xyz [YY] += badekas->xlateY;
	xyz [ZZ] += badekas->xlateZ;
	
	// Back to geographic, using the source ellipsoiod since this is an
	// inverse calculation.
	status = CS_xyzToLlh (trgLl, xyz, badekas->srcERad, badekas->srcESqr);

	return status;
}

/******************************************************************************
	This is an iterative exercise of CSbdkasF2.  Thus, this calculation
	will produce a rather precise inverse of geographic coordinates.  Used in
	the two dimensional case as we don't know what the ellipsoid elevation is.
	Thus, we can't precisely compute the inverse of a coordinate we might
	have computed the forward for, and the using application threw away the
	Z.  The end result of all this can be positional creep unless we do
	as coded below.
*/
int EXP_LVL9 CSbdkasI2 (struct csBdkas_ *badekas, double* trgLl, Const double* srcLl)
{
	int ii;
	int lngOk;
	int latOk;
	int rtnVal;

	double guess [3];
	double newLl [3];
	double epsilon [3];

	/* Assume everything goes OK until we know different. */
	rtnVal = 0;
	epsilon [0] = epsilon [1] = badekas->cnvrgValue;		/* keep gcc compiler happy */

	trgLl [LNG] = guess [LNG] = srcLl [LNG];
	trgLl [LAT] = guess [LAT] = srcLl [LAT];
	trgLl [HGT] = srcLl [HGT];

	/* Start a loop which will iterate as many as maxIteration times. */
	for (ii = 0;ii < badekas->maxIterations;ii++)
	{
		/* Assume we are done until we know different. */
		lngOk = latOk = TRUE;

		/* Compute the WGS-84 lat/long for our current guess. */
		rtnVal = CSbdkasF2 (badekas, newLl, guess);
		if (rtnVal != 0)
		{
			/* Oopps!! We must have been given some pretty strange
			   coordinate values. */
			break;
		}

		/* See how far we are off. */
		epsilon [LNG] = srcLl [LNG] - newLl [LNG];
		epsilon [LAT] = srcLl [LAT] - newLl [LAT];

		/* If our guess at the longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LNG]) > badekas->cnvrgValue)
		{
			lngOk = FALSE;
			guess [LNG] += epsilon [LNG];
		}
		/* If our guess longitude is off by more than
		   small, we adjust our guess by the amount we are off. */
		if (fabs (epsilon [LAT]) > badekas->cnvrgValue)
		{
			latOk = FALSE;
			guess [LAT] += epsilon [LAT];
		}

		/* If our current guess produces a newLl that is within
		   samllValue of srcLl, we are done. */
		if (lngOk && latOk) break;
	}

	/* If we didn't resolve in maxIteration tries, we issue a warning
	   message.  Usually, three or four iterations does the trick. */
	if (ii >= badekas->maxIterations )
	{
		CS_erpt (cs_BW_CNVRG);

		/* Issue a warning if we're close, a fatal if we are still way off.
		   In any case, we return the last computed value.  We could have
		   gotten very fancy with this stuff, but it would have had serious
		   affects on the performance.  So we just check epsilon here as
		   we know we have an error and this doesn't happen very often. */
		rtnVal = 1;
		if (fabs (epsilon [LNG]) > badekas->errorValue ||
		    fabs (epsilon [LAT]) > badekas->errorValue)
		{
			rtnVal = -1;
		}
	}

	/* If we are not returning fatal error status, we return the last
	   computed value even if we are returning a warning status.  It may be
	   of value, often is. */
	if (rtnVal >= 0)
	{
		trgLl [LNG] = guess [LNG];
		trgLl [LAT] = guess [LAT];
	}
	return rtnVal;
}
int EXP_LVL9 CSbdkasN (struct csBdkas_ *bdkas)
{
	int isNull;

	isNull = (fabs (bdkas->deltaX) < 1.0E-03) &&
			 (fabs (bdkas->deltaY) < 1.0E-03) &&
			 (fabs (bdkas->deltaZ) < 1.0E-03) &&
			 (fabs (bdkas->rotX)   < 1.0E-06) &&
			 (fabs (bdkas->rotY)   < 1.0E-06) &&
			 (fabs (bdkas->rotZ)   < 1.0E-06) &&
			 (fabs (bdkas->scale)  < 1.0E-08);
	return isNull;
}
int EXP_LVL9 CSbdkasL (struct csBdkas_ *badekas, int cnt,Const double pnts [][3])
{
	return cs_CNVRT_OK;
}
int EXP_LVL9 CSbdkasR (struct csBdkas_ *badekas)
{
	return 0;
}
int EXP_LVL9 CSbdkasD (struct csBdkas_ *badekas)
{
	return 0;
}
