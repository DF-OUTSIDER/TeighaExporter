/*
 * Copyright (c) 1998 by Tech Soft 3D, LLC.
 * The information contained herein is confidential and proprietary to
 * Tech Soft 3D, LLC., and considered a trade secret as defined under
 * civil and criminal statutes.	 Tech Soft 3D shall pursue its civil
 * and criminal remedies in the event of unauthorized use or misappropriation
 * of its trade secrets.  Use of this information by anyone other than
 * authorized employees of Tech Soft 3D, LLC. is granted only under a
 * written non-disclosure agreement, expressly prescribing the scope and
 * manner of such use.
 *
 * $Id: //DWF/Development/Components/Internal/DWF Toolkit/v7.7/develop/global/src/dwf/w3dtk/utility/hqsort.h#1 $
 */


#if 0  
/* the usage from the C file would look something like this (taken from generate_curve_polyline) */
#define QS_TYPE				LineHeap *				/* the type of object or array that contains all values */
#define QS_VAL(thing,a)		(thing->h[a].u_left)	/* returns the value of an item with index a */
#define QS_COMPARE(f1,f2)	(f1>f2)					/* use ">" to sort ascending, "<" to sort descending */
#define QS_SWAP(thing,a,b)	swaph(thing, a, b)		/* swaps two values */
#include "hqsort.h"
/* and the function calls... */
quick_sort(&heap, 0, heap.used - 1);
ASSERT (issorted (&heap, heap.used));
#endif

/* define QS_RESET and include to get rid of all defs */
/* must define QS_NAME for second include */
#ifdef QS_RESET

#undef QS_TYPE			
#undef QS_VAL
#undef QS_COMPARE
#undef QS_SWAP
#undef QS_MEDIANOF3INDEX
#undef QS_COMPARETYPE
#undef QS_COMPLEX_COMPARE
#undef QS_EQUAL
#undef QS_EQUAL_NOT_POSSIBLE
#undef QS_COMPLEX_COMPARE
#undef QS_PIVOT_VAL
#undef QS_USE_SYSTEM_RAND
#undef QS_PRESORTED_CHECK
#undef QS_NEED_ISSORTED
#undef quick_sort
#undef QS_BUBBLE_SORT			
#undef QS_QUICK_SORT_RECURSIVE	
#undef QS_ISSORTED
#undef QS_RESET

#else			
/*
  motivation:
	we want to avoid the function call per comparison, which rules out "void *" based algorithms.
 
  restrictions: 
	1) value to compare must resolve to a single value
	2) items must be identifiable by integer 
	3) whatever type QS_TYPE is defined as must be fully defined at the time of the include
*/

#if !defined(QS_TYPE) || !defined(QS_VAL) || (!defined(QS_COMPARE)&&!defined(QS_COMPLEX_COMPARE)) || !defined(QS_SWAP)
#	 error "source file must define QS_TYPE, QS_VAL, QS_COMPARE and QS_SWAP before including this header"
#endif


#define QS_MEDIANOF3INDEX(a,b,c) (((a)>(b))?(((b)>(c))?(1):(((a)>(c))?(2):(0))):(((a)>(c))?(0):((b)>(c))?(2):(1)))
#ifndef QS_COMPARETYPE
#  define QS_COMPARETYPE float
#endif

/* this optional definition is used for when the comparison has to be multi-tiered */
#ifdef QS_COMPLEX_COMPARE
#  ifndef QS_EQUAL
#	 define QS_EQUAL_NOT_POSSIBLE
#  endif
#else
#  define QS_COMPLEX_COMPARE(thing,i1,i2) QS_COMPARE (QS_VAL (thing,i1), QS_VAL (thing,i2))
#  define QS_PIVOT_VAL(thing,i1) QS_VAL(thing,i1)
#endif


#ifndef QS_BUBBLE_SORT
#define QS_BUBBLE_SORT				bubble_sort 
#endif

#ifndef QS_QUICK_SORT_RECURSIVE
#define QS_QUICK_SORT_RECURSIVE		quick_sort_recursive
#define quick_sort(thing,left,right) quick_sort_recursive(thing,left,right,1024);
#endif

#ifndef QS_ISSORTED
#define QS_ISSORTED					issorted
#endif

static void QS_BUBBLE_SORT(QS_TYPE thing, int left, int right) 
{
	int i, j;

	for (i = left; i <= right; i++) {
		for (j = i+1; j <= right; j++) {
			if (QS_COMPLEX_COMPARE (thing,i,j))
					QS_SWAP (thing, j, i);
		}
	}
}

/* hqsort.h can be used from outside of HOOPS, but you will get an error,
 * "unresolved external symbol HOOPS_WORLD" if you don't define QS_USE_SYSTEM_RAND */
#ifdef QS_USE_SYSTEM_RAND
#  undef URANDOM
#  include <stdlib.h>
#  define URANDOM() (unsigned int)rand()
#endif

/* the following implementation of quick sort is a variant that merges all values
 * that are exactly equal to the pivot as a single block in the middle, so that they can
 * be skipped when we recurse down to lower levels.	 It has a hard-coded limit to the depth
 * of the recursion so that we avoid trashing the stack, but in our tests, we have never
 * been able to make it get anywhere near that limit. */

static void QS_QUICK_SORT_RECURSIVE (QS_TYPE thing, int left, int right, int recursion_limit) 
{
	int i, j, k, lower, upper, count;
	int pivot; /* index of the item to be used as a pivot */

#if defined(HI_PROTO_DEFINED)
	if (recursion_limit == 0) {
		HE_WARNING (2 /* HEC_INTERNAL_ERROR */, 288 /* HES_OUT_OF_RANGE */, "Quick sort recursion depth limit reached");
	}
#endif

	if (recursion_limit == 0 || (right - left) <= 32) {
		QS_BUBBLE_SORT(thing, left, right);
		return;
	}
	

	if (right - left <= 1) {
		if (right <= left) 
			return;
		/* under most circumstances, the following comparison resolves to
			QS_COMPARE (QS_VAL (thing,left), QS_VAL (thing,right)) */
		if (QS_COMPLEX_COMPARE(thing,left,right))
			QS_SWAP (thing, right, left);
		return;
	}

#ifdef QS_PRESORTED_CHECK
	for (i = left; i <right; i++) {
		if (QS_COMPLEX_COMPARE(thing,i,i+1))
			break;
	}
	if (i >= right)
		return;
#endif

	i=left;
	j=right-1;
	lower=left;
	upper=right-1;

	count = right - left + 1;
	if (count > 8) {
		int sample1, sample2, sample3;	
  
		sample1 = left + (URANDOM() % count);
		sample2 = left + (URANDOM() % count);
		sample3 = left + (URANDOM() % count);
		/* these next few statements calculate the median and move it to the rightmost position */
		switch (QS_MEDIANOF3INDEX(QS_VAL (thing,sample1), QS_VAL (thing,sample2), QS_VAL (thing,sample3))) {
			case 0:
				if (sample1 != right)
					QS_SWAP (thing, sample1, right);
				break;
			case 1:
				if (sample2 != right)
					QS_SWAP (thing, sample2, right);
				break;
			case 2:
				if (sample3 != right)
					QS_SWAP (thing, sample3, right);
				break;
		}
	}
	else {
		int sample1 = left + (URANDOM() % count);
		if (sample1 != right)
			QS_SWAP (thing, sample1, right);
	}
	pivot=right;

	while (1) {
		/* under most circumstances, the following comparison resolves to 
			QS_COMPARE (pivot,QS_VAL (thing,i)) */
		while (QS_COMPLEX_COMPARE(thing,pivot,i))  {
			i++;
			if (i == right)
				break;
		}
		while (QS_COMPLEX_COMPARE(thing,j,pivot))  {
			j--;
			if (j == left) 
				break;
		}
		if (i >= j) 
			break;
		QS_SWAP (thing, i, j);

/* if the caller provided a macro to test for equality, use it.	 otherwise, compare QS_VAL's */
#ifndef QS_EQUAL_NOT_POSSIBLE
#  ifdef QS_EQUAL
		if (QS_EQUAL (thing,i,pivot)) { 
			if (lower != i)
				QS_SWAP (thing, lower, i); 
			lower++; 
			i++;
		}
		if (QS_EQUAL (thing,pivot,j)) { 
			if (j != upper)
				QS_SWAP (thing, j, upper); 
			upper--; 
			j--;
		}
#  else
		if (QS_VAL (thing,i) == QS_VAL (thing,pivot)) { 
			if (lower != i)
				QS_SWAP (thing, lower, i); 
			lower++; 
			i++;
		}
		if (QS_VAL (thing,pivot) == QS_VAL (thing,j)) { 
			if (j != upper)
				QS_SWAP (thing, j, upper); 
			upper--; 
			j--;
		}
#  endif
#endif
	}
	if (i != right)
		QS_SWAP (thing, i, right); 
	j = i-1;
	for (k=left; k<lower; k++, j--)
		if (k != j)
			QS_SWAP (thing, k, j);
	i++;
	for (k=right-1; k>upper; k--, i++) 
		if (i != k)
			QS_SWAP (thing, i, k);

	QS_QUICK_SORT_RECURSIVE (thing, left, j, recursion_limit - 1);
	QS_QUICK_SORT_RECURSIVE (thing, i, right, recursion_limit - 1);
}


#ifdef _DEBUG
/* for debugging only */
#ifdef QS_NEED_ISSORTED
local bool QS_ISSORTED (
		QS_TYPE thing,
		int count) 
{
	int i;

	for (i = 1; i < count; i++) {
		if (QS_VAL(thing,i-1) != QS_VAL(thing,i) &&
			QS_COMPARE(QS_VAL(thing,i-1),QS_VAL(thing,i)))
			return false;
	}
	return true;
}
#endif
#endif

/* QS_RESET */
#endif



