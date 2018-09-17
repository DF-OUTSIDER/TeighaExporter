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

/*lint -esym(550,ctItmName,dummy)  not accessed, used to support sizeof operator */
/*lint -esym(550,crypt)            not accessed, required function parameter */

#include "cs_map.h"
#include "cs_ioUtil.h"


short CSctCompCsChk (csFILE *csStrm,Const char* csName);

//wraps the category information read from the dictionary file
//if no user-defined directory has been set, categories can be edited
//directly in the file. If a separate directory has been set, however,
//content cannot be removed from those categories
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Internal, not exported, API ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**** Prototypes of internal functions ---> ****/

struct cs_Ctdef_* CSgetCtDefHead();
struct cs_Ctdef_* CSgetCtDefTail();
struct cs_Ctdef_* CSgetCtDefTailEx(int* catCount);
int LinkInCategory(struct cs_Ctdef_* pNewCategory, struct cs_Ctdef_* pToBeReplaced, int releaseCategory);
int UnlinkCategory(struct cs_Ctdef_* pToBeRemoved, int releaseCategory);
int AppendCategory(struct cs_Ctdef_* pToAppend);
int GetCategoryPtr(const char* catName, struct cs_Ctdef_** ppCategory);
int GetCategoryPtrEx(struct cs_Ctdef_* pHead, const char* catName, struct cs_Ctdef_** ppCategory);
int GetCategoryPtrIdx(unsigned index, struct cs_Ctdef_** ppCategory);
int GetIndexOfName(Const struct cs_Ctdef_* pCategory, Const char* name);
int AddCsName(struct cs_Ctdef_* ctDefPtr, Const char* csName);
int CanModifyCsName(Const char* catName, unsigned idx, struct cs_Ctdef_** ctDefPtr);
int ExtendCsNameBlock(struct cs_Ctdef_* pCategoryIn, size_t byCount);

/**** <--- Prototypes of internal functions ****/

/*************************************************************************
* Internal (unexported) function
* 
* Returns the current head of all in-memory cs_Ctdef_ instances.
* If no such category information has been read yet, this function will try reading all category information from the current Category.CSD file(s)
* NULL:     if the function failed somewhere; IO error, OOM
* POINTER:  the pointer to the current head cs_Ctdef_ instance.
**************************************************************************/
/* live */ struct cs_Ctdef_* CSgetCtDefHead ()
{
	extern struct cs_Ctdef_* cs_CtDefHead;
	if (NULL != cs_CtDefHead)
		return cs_CtDefHead;

	cs_CtDefHead = CSrdCatFile();
	return cs_CtDefHead;
}

/*************************************************************************
* Internal (unexported) function
* 
* Returns the current tail of all in-memory cs_Ctdef_ instances.
* If no such category information has been read yet, this function will try reading all category information from the current Category.CSD file(s)
* NULL:     if the function failed somewhere; IO error, OOM
* POINTER:  the pointer to the current tail cs_Ctdef_ instance.
**************************************************************************/
/* live */ struct cs_Ctdef_* CSgetCtDefTail ()
{
	return CSgetCtDefTailEx(NULL);
}

/*************************************************************************
* Internal (unexported) function
* 
* Returns the current tail, and optionally, the number of all in-memory cs_Ctdef_ instances.
* If no such category information has been read yet, this function will try reading all category information from the current Category.CSD file(s)
* NULL:     if the function failed somewhere; IO error, OOM
* POINTER:  the pointer to the current tail cs_Ctdef_ instance.
**************************************************************************/
/* live */ struct cs_Ctdef_* CSgetCtDefTailEx (int* pCatCount)
{
	extern struct cs_Ctdef_* cs_CtDefHead;
	struct cs_Ctdef_* ctTail = NULL;
	int catCount = 0;

	cs_CtDefHead = CSgetCtDefHead();
	if (NULL == cs_CtDefHead)
	{
		if (NULL != pCatCount)
			*pCatCount = 0;

		return NULL;
	}

	++catCount; //i.e. 1

	ctTail = cs_CtDefHead;
	while(NULL != ctTail->next)
	{
		++catCount;
		ctTail = ctTail->next;
	}

	if (NULL != pCatCount)
		*pCatCount = catCount;

	return ctTail;
}

/*************************************************************************
* Internal (unexported) function
* 
* Returns a live pointer in [ppCategory] to a category object from the internal list
* 0:    function succeeded; [ppCategory] may or may not be set to an instance of a [cs_Ctdef_] definition
* -1:   in case of any error; invalid category name, IO error, OOM etc.
**************************************************************************/
/* live */ int GetCategoryPtr(const char* catName, struct cs_Ctdef_** ppCategory)
{
	return GetCategoryPtrEx(NULL, catName, ppCategory);
}

/* live */ int GetCategoryPtrEx(struct cs_Ctdef_* pHead, const char* catName, struct cs_Ctdef_** ppCategory)
{
	if (NULL == catName || '\0' == catName[0] || NULL == ppCategory)
	{
		CS_erpt(cs_INV_ARG2);
		return -1;
	}

	if (NULL == pHead)
		pHead = CSgetCtDefHead();

	if (NULL == pHead)
		return -1;

	while (pHead != NULL)
	{
		if (CS_stricmp (pHead->ctName,catName) == 0)
		{
			*ppCategory = pHead;
			return 0;
		}

		pHead = pHead->next;
	}

	//no category with that name has been found
	*ppCategory = NULL;
	return 0;
}

/* live */ int GetCategoryPtrIdx(unsigned index, struct cs_Ctdef_** ppCategory)
{
	extern int cs_Error;

	unsigned currentIndex = 0;
	struct cs_Ctdef_* pHead = NULL;

	cs_Error = 0;

	CS_CHECK_NULL_ARG(ppCategory, 2);

	*ppCategory = NULL;

	pHead = CSgetCtDefHead();
	if (NULL == pHead)
		return -1;

	for( ; index != currentIndex && NULL != pHead; ++currentIndex)
	{
		pHead = pHead->next;
	}

	if (NULL != pHead)
		*ppCategory = pHead;

	return 0;
}

/******************************************************************************
* Internal (unexported) function
*
* Replaces the category [pToBeReplaced] with the category [pNewCategory] in the live
* list of categories.
*******************************************************************************/
int LinkInCategory(struct cs_Ctdef_* pNewCategory, struct cs_Ctdef_* pToBeReplaced, int releaseCategory)
{
	extern int cs_Error;
	extern struct cs_Ctdef_* cs_CtDefHead;

	cs_Error = 0;

	if (NULL == pNewCategory || NULL == pToBeReplaced)
	{
		CS_erpt(cs_ISER);
		return -1;
	}

	//link in our category...
	pNewCategory->next = pToBeReplaced->next;
	pNewCategory->previous = pToBeReplaced->previous;

	//...and unlink the old one
	pToBeReplaced->next = pToBeReplaced->previous = NULL;

	//make sure, the (former) next category is now linked back to newly linked in category
	if (NULL != pNewCategory->next)
		pNewCategory->next->previous = pNewCategory;

	if (NULL != pNewCategory->previous)
		pNewCategory->previous->next = pNewCategory;
	else
	{
		//the category we're updating was the head; make sure,
		//we now also reset the current list's head
		cs_CtDefHead = pNewCategory;
	}

	if (TRUE == releaseCategory)
		CSrlsCategory(pToBeReplaced);

	return 0;
}

/******************************************************************************
* Internal (unexported) function
*
* Replaces the category [pToBeReplaced] with the category [pNewCategory] in the live
* list of categories.
*******************************************************************************/
int UnlinkCategory(struct cs_Ctdef_* pToBeRemoved, int releaseCategory)
{
	extern int cs_Error;
	extern struct cs_Ctdef_* cs_CtDefHead;
	cs_Error = 0;

	if (NULL == pToBeRemoved)
	{
		CS_erpt(cs_ISER);
		return -1;
	}

	if (NULL != pToBeRemoved->next)
		pToBeRemoved->next->previous = pToBeRemoved->previous;

	if (NULL != pToBeRemoved->previous)
		pToBeRemoved->previous->next = pToBeRemoved->next;
	else
	{
		//pToBeRemoved is the list's head
		cs_CtDefHead = pToBeRemoved->next; //which might be NULL; what's no problem
	}

	if (TRUE == releaseCategory)
	{
		CSrlsCategory(pToBeRemoved);
		pToBeRemoved = NULL;
	}

	return 0;
}

/******************************************************************************
* Internal (unexported) function
*
* Replaces the category [pToBeReplaced] with the category [pNewCategory] in the live
* list of categories.
*******************************************************************************/
int AppendCategory(struct cs_Ctdef_* pToAppend)
{
	extern int cs_Error;
	extern struct cs_Ctdef_* cs_CtDefHead;
	struct cs_Ctdef_* pTailCategory;

	cs_Error = 0;

	if (NULL == pToAppend)
	{
		CS_erpt(cs_ISER);
		return -1;
	}

	pTailCategory = CSgetCtDefTail();
	if (NULL == pTailCategory)
	{
		if (0 != cs_Error)
			return -1;

		//that's now our new head
		cs_CtDefHead = pToAppend;
	}
	else
	{
		pTailCategory->next = pToAppend;
		pToAppend->previous = pTailCategory;
	}

	return 0;
}

/******************************************************************************
* Internal (unexported) function
*
* Returns the first(!) index of the CS name in the category passed in via [pCategory]
* -1 if not found or in case of an error (invalid argument)
*******************************************************************************/
int GetIndexOfName(Const struct cs_Ctdef_* pCategory, Const char* name)
{
	unsigned nameIdx;
	size_t csNameSize = sizeof(((struct cs_CtItmName_*)0)->csName) / sizeof(char);

	if (NULL == pCategory)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	if (NULL == name || '\0' == *name || CS_nampp((char*)name))
	{
		CS_erpt(cs_INV_ARG2);
		return -1;
	}

	for(nameIdx = 0; nameIdx < pCategory->nameCnt; ++nameIdx)
	{
		if (0 == CS_strnicmp((pCategory->csNames + nameIdx)->csName, name, csNameSize))
			return nameIdx;
	}

	return -1;
}

/******************************************************************************
* Internal (unexported) function
*
* Adds the CS name to the category passed in via [pCategory].
* Returns 0 if successful, a negative value otherwise.
* In particular, this value returns -2, if memory allocation was required but failed.
*******************************************************************************/
int AddCsName(struct cs_Ctdef_* ctDefPtr, Const char* csName)
{
	char csNameTemp[cs_KEYNM_DEF] = { '\0' };
	char *destPtr = NULL;
	int allocStatus = -1;

	if (NULL == ctDefPtr)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	if (NULL == csName || '\0' == *csName)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	CS_stncp(csNameTemp, csName, cs_KEYNM_DEF);
	if (CS_nampp(csNameTemp))
		return -1;

	allocStatus = ExtendCsNameBlock(ctDefPtr, 1);
	if (allocStatus)
		return allocStatus;

	//copy the new CS name into the position after the last valid index
	destPtr = (ctDefPtr->csNames[ctDefPtr->nameCnt]).csName;
	CS_stncp (destPtr, csNameTemp, sizeof(((struct cs_CtItmName_ *)0)->csName) / sizeof(char));
	++ctDefPtr->nameCnt;

	return 0;
}

/*************************************************************************
* Internal (unexported) function
* 
* Can the CS name at the given index be edited; i.e. modified & deleted?
* Return values:
*   -1: error; details can be found in [cs_Error]
*    0: FALSE - the CS name at the given index cannot be edited
*    1: TRUE - the CS name at the given index can be edited
**************************************************************************/
int CanModifyCsName(Const char* catName, unsigned idx, struct cs_Ctdef_** ctDefPtr)
{
	extern int cs_Error;
	extern char csErrnam [];

	cs_Error = 0;

	//have we been called "correctly"?
	if (NULL == ctDefPtr)
	{
		CS_erpt(cs_ISER);
		return -1;
	}

	//does there a category exists and/or had the [catName] argument at least been valid?
	if (GetCategoryPtr(catName, ctDefPtr))
		return -1;

	if (NULL == *ctDefPtr)
	{
		CS_stncp(csErrnam, catName, MAXPATH);
		CS_erpt(cs_CT_NOT_FND);

		return -1;
	}

	//special case; empty categories can always be edited
	if (0 == idx && 0 == (*ctDefPtr)->nameCnt)
		return TRUE;

	if (idx >= (*ctDefPtr)->nameCnt)
	{
		CS_erpt(cs_INV_ARG2);
		return -1;
	}

	
	/*
	Ok, we have found the desired category. Verify, that
	it is not a system category, i.e. the [protected]
	field is not TRUE
	*/
	if (FALSE == (*ctDefPtr)->protect)
		return TRUE;

	//the "first user CS name" index must be >= to [userNameIndex];
	//we can only modify entries which are declared as being modifieable
	return (idx >= (*ctDefPtr)->userNameIndex) ? TRUE : FALSE;
}

/*************************************************************************
* Internal (unexported) function
* Increases the [csNames] memory, if necessary, so an additional number of 
* at least [byCount] CS names can be stored in the category. If [byCount] is 0, 
* the default number of name blocks is allocated.
*
* Return 0 if successful, a negative value otherwise, in which case [ctDefPtr->csNames]
* is left unchanged.
**************************************************************************/
int ExtendCsNameBlock(struct cs_Ctdef_* pCategoryIn, size_t byCount)
{
	size_t newSize = 0;
	size_t blockSize = 0;
	size_t totalRequiredCount = 0;
	struct cs_CtItmName_* allocatedBlock = NULL;

	__ALIGNMENT__1
	blockSize = sizeof (struct cs_CtItmName_);

	if (NULL == pCategoryIn)
	{
		CS_erpt(cs_ISER); //what have we been given here???
		return -1;
	}

	if (0 == byCount)
		byCount = cs_CATDEF_ALLOC;

	//check, whether we've to allocate new memory in our [csNames] array
	totalRequiredCount = pCategoryIn->nameCnt + byCount;
	if (totalRequiredCount <= pCategoryIn->allocCnt) //Nope, enough space is available already
		return 0;

	//make sure, we're allocating at least a number of [cs_CATDEF_ALLOC] blocks
	if (byCount < cs_CATDEF_ALLOC)
		totalRequiredCount += (cs_CATDEF_ALLOC - byCount);

	newSize = totalRequiredCount * blockSize;
	allocatedBlock = (struct cs_CtItmName_*) CS_ralc (pCategoryIn->csNames, newSize);
	if (NULL == allocatedBlock)
	{
		//[CS_ralc], i.e. [realloc()] leaves the memory block unchanged; setting
		//[pCategoryIn->csNames] to NULL would actually create a mem leak; so
		//we're reporting the OOM condition and return
		CS_erpt(cs_NO_MEM);
		return -2;
	}

	//now update the struct...
	pCategoryIn->csNames = allocatedBlock;
	pCategoryIn->allocCnt = (ulong32_t)totalRequiredCount;
	//...and zero out everything, that's beyond the last CS name
	memset(&pCategoryIn->csNames[pCategoryIn->nameCnt], '\0', (totalRequiredCount - pCategoryIn->nameCnt) * blockSize);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Public, exported, API ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Returns +1 if the provided name is that of a defined category, else returns
 * zero.  Returns -1 if no category data has not been initialized.
 *****************************************************************************/
int EXP_LVL1 CS_vldCtName (const char* catName)
{
	return CS_vldCtNameEx(catName, NULL);
}

/******************************************************************************
 * Returns +1 (TRUE) if the provided name is that of a defined category, else returns
 * zero.  Returns -1 if no category data has not been initialized.
 *****************************************************************************/
int EXP_LVL1 CS_vldCtNameEx (const char* catName, struct cs_Ctdef_* pCtDef)
{
	struct cs_Ctdef_* ctDefPtr = NULL;

	if (GetCategoryPtr(catName, &ctDefPtr))
		return -1;

	if (NULL == ctDefPtr)
		return 0; //no category has been found

	//if we haven't been given a category that we'd have to
	//fill, we can return +1 directly to the caller
	if (NULL == pCtDef)
		return 1;

	//if, however, we've been given a cs_Ctdef_ via [pCtDef] instance, 
	//we'll be filling that with the information from [ctDefPtr]
	if (NULL == CScpyCategoryEx(pCtDef, ctDefPtr, TRUE /* copy the protect flag */))
		goto error;

	return 1;

error:
	if (NULL != pCtDef)				/*lint !e774  boolean always evaluates to true */
		CSclnCategory(pCtDef);		/*lint !e534  ignoring return value */

	return -1;
}

/******************************************************************************
 * Returns the category name associated with the idx'th category in the
 * category list.  Returns null if no category data available, or if the idx
 * argument is too large.  idx of zero means the first category.
 *
 * Use this function to enumerate all category names by incrementing the idx
 * value by one after each call, until such time as a null pointer is returned.
 *****************************************************************************/
Const char* EXP_LVL1 CS_getCatName (unsigned idx)
{
	unsigned myIdx = 0;
	Const char* rtnValue = 0;
	struct cs_Ctdef_* ctDefPtr;

	ctDefPtr = CSgetCtDefHead();
	while (ctDefPtr != NULL)
	{
		if (myIdx == idx)
		{
			rtnValue = ctDefPtr->ctName;
			break;
		}

		myIdx++;
		ctDefPtr = ctDefPtr->next;
	}	
	return rtnValue;
}

/******************************************************************************
 * Returns the number of CS names currently contained in the category
 * indicated by the catName argument.
 * Returns -1 in case of an error or if the no category with such a name exists
 * Returns the number of CS names otherwise (including 0).
 *****************************************************************************/
int EXP_LVL1 CS_getItmNameCount (const char* catName)
{
	struct cs_Ctdef_* ctDefPtr;
	if (GetCategoryPtr(catName, &ctDefPtr))
		return -1;

	if (NULL == ctDefPtr)
	{
		CS_erpt(cs_CT_NOT_FND);
		return -1;
	}

	return (int) ctDefPtr->nameCnt;
}

/******************************************************************************
 * Returns the item name associated with the idx'th item in the category
 * indicated by the catName argument. Returns null if no category data
 * available, or if the catName argument is not that of a valid category, or
 * the idx argument is too large.  idx of zero means the first item name.
 *
 * Use this function to enumerate all item names within a ctaegory by
 * incrementing the idx value by one after each call, until such time as a
 * null pointer is returned.
 *****************************************************************************/
Const char* EXP_LVL1 CS_getItmName (const char* catName,unsigned idx)
{	
	struct cs_Ctdef_* ctDefPtr;
	
	if (GetCategoryPtr(catName, &ctDefPtr))
		return NULL;

	if (NULL == ctDefPtr)
	{
		CS_erpt(cs_CT_NOT_FND);
		return NULL;
	}

	if (idx >= ctDefPtr->nameCnt)
	{
		CS_erpt(cs_INV_ARG2);
		return NULL;
	}

	return (ctDefPtr->csNames + idx)->csName;
}

/******************************************************************************
 * Sets the file name to read the categories from.
 * This will automatically unset the in-memory category list.
 *****************************************************************************/
void EXP_LVL3 CS_ctfnm (const char* new_name)
{
	extern char cs_Ctname [];
	if (NULL == new_name || '\0' == new_name[0])
	{
		CS_erpt(cs_INV_ARG1);
		return;
	}

	(void)CS_stncp (cs_Ctname,new_name,cs_FNM_MAXLEN);

	//and release the categories now that we're supposed to read/write the content from/to
	//a new category file
	CSrlsCategories();
}

/******************************************************************************
 * Returns the [cs_Ctdef_*] struct for the category with the given name
 *
 * The caller is responsible for releasing the category through CSrlsCategory()
 *****************************************************************************/
struct cs_Ctdef_* EXP_LVL3 CSgetCtDef(const char* catName)
{
	extern int cs_Error;
	extern char csErrnam [];

	int searchResult = 0;
	struct cs_Ctdef_* pFoundCategory = NULL;

	cs_Error = 0;

	pFoundCategory = CSnewCategoryEx(NULL, FALSE);
	if (NULL == pFoundCategory)
		return NULL; //OOM, for example

	searchResult = CS_vldCtNameEx(catName, pFoundCategory);
	if (TRUE == searchResult)
	{
		return pFoundCategory;
	}
	else if (FALSE == searchResult)
	{
		CS_stncp(csErrnam, catName, MAXPATH);
		CS_erpt(cs_CT_NOT_FND);
	}
	//else: searchResult == -1 --> error must have been reported already

	CSrlsCategory(pFoundCategory);
	return NULL;
}

/*******************************************
 * Copies all category definitions in a newly
 * allocated array pointed to by [pDefArray]
 * 
 * Returns the number of pointer to elements contained
 * in [pDefArray]. Returns >= 0 if successful.
 * -1 otherwise in which case [*pDefArray] will
 * have been set to NULL.
********************************************/
int EXP_LVL3 CSgetCtDefAll(struct cs_Ctdef_ **pDefArray[])
{
	extern int cs_Error;

	int catCount = 0;
	int catCountIndex = 0;
	int i;
	struct cs_Ctdef_* pCatTail = NULL;
	struct cs_Ctdef_* pCatCurrent = NULL;
	struct cs_Ctdef_* pCopiedCurrent = NULL;
	size_t arraySize;

	cs_Error = 0;

	if (NULL == pDefArray)
	{
		CS_erpt(cs_INV_ARG1);
		return 0;
	}

	*pDefArray = NULL;

	pCatTail = CSgetCtDefTailEx(&catCount);
	if (NULL == pCatTail)
	{
		if (cs_Error)
			goto error;

		return 0;
	}

	//malloc the array that's going to hold all the
	//pointers
	arraySize = catCount * sizeof(struct cs_Ctdef_ *);
	(*pDefArray) = (struct cs_Ctdef_ **) CS_malc (arraySize);
	if (NULL == *pDefArray)
	{
		CS_erpt(cs_NO_MEM);
		goto error;
	}

	memset(*pDefArray, 0x0, arraySize);

	//now go backward - as we've the tail pointer anyway and
	pCatCurrent = pCatTail;
	catCountIndex = catCount - 1;
	while(NULL != pCatCurrent && catCountIndex >= 0)
	{
		pCopiedCurrent = CScpyCategory(pCatCurrent);
		if (NULL == pCopiedCurrent)
			goto error;

		(*pDefArray)[catCountIndex] = pCopiedCurrent;
		--catCountIndex;

		pCatCurrent = pCatCurrent->previous;
	}

	if (-1 != catCountIndex || NULL != pCatCurrent)
	{
		//clean up what ever we've here...
		CSrlsCategories();
		CS_erpt(cs_ISER);

		goto error;
	}

	return catCount;

error:

	if (NULL != *pDefArray)
	{
		for(i = 0; i < catCount; ++i)
			CSrlsCategory((*pDefArray)[i]);

		CS_free(*pDefArray);
		*pDefArray = NULL;
	}

	return -1;
}

/*******************************************
 * Cleans any content, except for the name,
 * from the category passed in.
********************************************/
int EXP_LVL3 CSclnCategory(struct cs_Ctdef_ * pCategoryIn)
{
	char catName[sizeof((struct cs_Ctdef_*)0)->ctName / sizeof(char)] = { '\0' };
	if (NULL == pCategoryIn)
		goto invalid_arg;

	//save the category name...
	CS_stncp(catName, pCategoryIn->ctName, sizeof(catName) / sizeof(char));
	//...and free the [csNames] pointer
	CS_free(pCategoryIn->csNames);

	//zero everything so we don't forget a field...
	memset(pCategoryIn, 0x0, sizeof(struct cs_Ctdef_));

	//...and copy the category name back in
	CS_stncp(pCategoryIn->ctName, catName, sizeof(catName) / sizeof(char));

	return 0;

invalid_arg:
	CS_erpt(cs_INV_ARG1);
	return -1;
}

struct cs_Ctdef_* EXP_LVL3 CScpyCategory(Const struct cs_Ctdef_ * pSrcCategory)
{
	return CScpyCategoryEx(NULL, pSrcCategory, TRUE);
}

struct	cs_Ctdef_*	EXP_LVL3 CScpyCategoryEx(struct cs_Ctdef_* pDstCategory, Const struct cs_Ctdef_ * pSrcCategory, int setProtectFlag)
{
	extern int cs_Error;

	size_t allocBlockCount = 0;
	ulong32_t index = 0;
	int newCategoryAlloc = FALSE;

	cs_Error = 0;

	if (NULL == pSrcCategory || '\0' == pSrcCategory->ctName[0])
		goto error_invalid_arg;

	if (NULL == pDstCategory)
	{
		pDstCategory = CSnewCategoryEx(pSrcCategory->ctName, FALSE);
		if (NULL == pDstCategory) //error has already been reported
			goto error;

		newCategoryAlloc = TRUE;
	}
	else
	{
		//remove everything, except for the name, from the category
		CSclnCategory(pDstCategory);			/*lint !e534   ignoring return value */
	}

	CS_stncp(pDstCategory->ctName, pSrcCategory->ctName, sizeof(((struct cs_Ctdef_*)0)->ctName) / sizeof(char));

	//this has to be allocated in one block - so it can be free'd correctly by CSrlsCategory
	allocBlockCount = pSrcCategory->allocCnt * sizeof(struct cs_CtItmName_);
	pDstCategory->csNames = (struct cs_CtItmName_*) CS_malc (allocBlockCount);
	if (NULL == pDstCategory->csNames)
	{
		CS_erpt(cs_NO_MEM);
		goto error;
	}

	memset(pDstCategory->csNames, 0, allocBlockCount);

	//now we've to copy all names (i.e. cs_CtItmName_ structs) from the source into the target
	for(index = 0; index < pSrcCategory->nameCnt && index < pSrcCategory->allocCnt; ++index)
	{
		CS_stncp(pDstCategory->csNames[index].csName, pSrcCategory->csNames[index].csName, cs_KEYNM_DEF);
		if (CS_nampp(pDstCategory->csNames[index].csName))
		{
			//this CS string is invalid...
			CS_erpt(cs_INV_ARG2); //...i.e. [pSrcCategory]
			goto error;
		}
	}

	pDstCategory->nameCnt = index;
	pDstCategory->allocCnt = pSrcCategory->allocCnt;
	pDstCategory->userNameIndex = pSrcCategory->userNameIndex;

	//note, that the returned category has its [next] & [previous] fields set to NULL
	if (TRUE == setProtectFlag)
		pDstCategory->protect = pSrcCategory->protect;

	return pDstCategory;

error_invalid_arg:
	CS_erpt(cs_INV_ARG2);

error:
	if (newCategoryAlloc && NULL != pDstCategory)
	{
		CSrlsCategory(pDstCategory);
		pDstCategory = NULL;
	}

	return NULL;
}

/******************************************************************************
 * Replaces the idx'th item name in the category indicated by the catName
 * argument.  It is assumed that the idx argument is that which was used to
 * locate the desired item.
 *
 * Function returns zero on success, -1 for failure.  Failure can be caused by:
 * 1> Category data has not been initialized.
 * 2> ctName argument is not that of a valid category
 * 3> idx argument does not select an item name in the indicated category.
 *****************************************************************************/
int EXP_LVL3 CSrplItmName (Const char* catName, unsigned idx, Const char* newName)
{
	char newCrsName[cs_KEYNM_DEF] = { '\0' };
	int categoryUpdate;

	struct cs_Ctdef_* ctDefPtr;

	if (    NULL == CS_stncp(newCrsName, newName, sizeof(newCrsName) / sizeof(char))
		||  CS_nampp(newCrsName))
	{
		CS_erpt(cs_INV_ARG3); //note, that this overwrites the error which might have been reported by CS_nampp
		return -1;
	}

	categoryUpdate = CanModifyCsName(catName, idx, &ctDefPtr);
	if (categoryUpdate < 1)
		return -1;

	//[1 == CanModifyCrsName()] tells us, that we can modify the entry at the given index

	//now replace the CRS name
	CS_stncp ((ctDefPtr->csNames + idx)->csName, newCrsName, sizeof(((struct cs_CtItmName_ *)0)->csName) / sizeof(char));
	return 0;
}

/******************************************************************************
 * Removes the idx'th item name in the category indicated by the catName
 * argument.  It is assumed that the idx argument is that which was used to
 * locate the desired item.
 *
 * Function returns zero on success, -1 for failure.  Failure can be caused by:
 * 1> Category data has not been initialized.
 * 2> ctName argument is not that of a valid category
 * 3> idx argument does not select an item name in the indicated category.
 *****************************************************************************/
int EXP_LVL3 CSrmvItmName (Const char* catName, unsigned idx)
{
	struct cs_Ctdef_* ctDefPtr;
	int categoryUpdate;

	categoryUpdate = CanModifyCsName(catName, idx, &ctDefPtr);
	if (categoryUpdate < 1)
		return -1;

	//a CS name exists at the given index; remove it from the 'live'
	//category instance
	return CSrmvItmNameEx(ctDefPtr, ctDefPtr->csNames[idx].csName);
}

int EXP_LVL3 CSrmvItmNameEx (struct cs_Ctdef_ *pCategoryIn, Const char* name)
{
	extern int cs_Error;
	extern char csErrnam [];

	int csNameIndex = -1;

	cs_Error = 0;

	if (NULL == pCategoryIn)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	if (NULL == name || '\0' == *name)
	{
		CS_erpt(cs_INV_ARG2);
		return -1;
	}

	csNameIndex = GetIndexOfName(pCategoryIn, name);
	if (csNameIndex < 0)
	{
		Const char * pszCategoryName = pCategoryIn->ctName;
		if ('\0' == *pszCategoryName)
		{
			pszCategoryName = "Unknown";
		}

		//cs is not contained
		CS_stncp(csErrnam, pszCategoryName, MAXPATH);
		CS_erpt(cs_CT_CS_NOT_IN);

		return -1;
	}

	if (pCategoryIn->nameCnt != (ulong32_t)(csNameIndex - 1))
	{
		//it's not the last item we've to remove; that is, we don't just have to
		//reduce the count but really have to do some mem moving here
		//
		//use memmove because the regions do overlap
		size_t numberOfBlocks = (pCategoryIn->nameCnt - csNameIndex - 1) * sizeof(struct cs_CtItmName_);
		void* movedMem = memmove(&pCategoryIn->csNames[csNameIndex], &pCategoryIn->csNames[csNameIndex + 1], numberOfBlocks);
		if (NULL == movedMem)
		{
			//(it's "likely", that)...
			//...we ran OOM when memmove tried to allocate the target memory before doing the move
			CS_erpt(cs_NO_MEM);
			return -1;
		}
	}
	//else:
	//  we can just cut off the last item

	//reduce the overall name count once we're done... (note, that we don't have to decrement the [userNameIndex] field)
	--pCategoryIn->nameCnt;
	//...and zero out the last block; just to finish it
	memset(&pCategoryIn->csNames[pCategoryIn->nameCnt], 0x0, sizeof(struct cs_CtItmName_));

	return 0;
}

int EXP_LVL3 CSrmvItmNames (Const char* catName)
{
	extern int cs_Error;
	extern char csErrnam [];

	int categoryUpdate;
	struct cs_Ctdef_ *ctDefPtr = NULL; //our live pointer

	cs_Error = 0;

	if (GetCategoryPtr(catName, &ctDefPtr))
		return -1;

	if (NULL == ctDefPtr)
	{
		CS_erpt(cs_CT_NOT_FND);
		CS_stncp(csErrnam, catName, MAXPATH);

		return -1;
	}

	categoryUpdate = CanModifyCsName(catName, 0, &ctDefPtr);
	if (categoryUpdate < 1)
		return -1;

	return CSrmvItmNamesEx(ctDefPtr);
}

int EXP_LVL3 CSrmvItmNamesEx (struct cs_Ctdef_ *pCategoryIn)
{
	extern int cs_Error;

	cs_Error = 0;

	if (NULL == pCategoryIn)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	CS_free(pCategoryIn->csNames);
	pCategoryIn->csNames = NULL;
	pCategoryIn->allocCnt = pCategoryIn->nameCnt = pCategoryIn->userNameIndex = 0;

	return 0;
}

/******************************************************************************
 * Adds an item name to the category indicated by the ctName argument.
 *
 * Function returns zero on success, -1 for failure.  Failure can be caused by:
 * 1> Category data has not been initialized.
 * 2> ctName argument is not that of a valid category
 *
 * If reallocation is necessary, and the reallocation fails, a -2 value is
 * returned.
 *
 * THIS FUNCTION DOES NOT CHECK TO SEE IF THE PROVIDED ITEM NAME ALREADY
 * EXISTS.
 *****************************************************************************/
int EXP_LVL3 CSaddItmName(Const char* catName, Const char* newName)
{
	extern int cs_Error;
	extern char csErrnam [];

	struct cs_Ctdef_* ctDefPtr = NULL;

	cs_Error = 0;

	if (GetCategoryPtr(catName, &ctDefPtr))
		return -1;

	if (NULL == ctDefPtr)
	{
		CS_erpt(cs_CT_NOT_FND);
		CS_stncp(csErrnam, catName, MAXPATH);

		return -1;
	}

	return CSaddItmNameEx(ctDefPtr, newName);
}

/******************************************************************************
 * Adds an item name to the category indicated by the pCategoryIn argument.
 *
 * Function returns zero on success, -1 for failure.  Failure can be caused by:
 * 1> Category data has not been initialized.
 * 2> ctName argument is not that of a valid category
 *
 * If reallocation is necessary, and the reallocation fails, a -2 value is
 * returned.
 *
 * THIS FUNCTION DOES NOT CHECK TO SEE IF THE PROVIDED ITEM NAME ALREADY
 * EXISTS.
 *****************************************************************************/
int EXP_LVL3 CSaddItmNameEx(struct cs_Ctdef_ *pCategoryIn, Const char* newName)
{
	extern int cs_Error;
	extern char csErrnam [];

	cs_Error = 0;

	if (NULL == pCategoryIn)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	//make sure all arguments we got are valid
	if (NULL == newName || 0 != CS_nampp((char*)newName))
	{
		CS_erpt(cs_INV_ARG2);
		return -1;
	}

	if (GetIndexOfName(pCategoryIn, newName) >= 0)
	{
		CS_erpt(cs_CT_CS_ADD_DUP);
		CS_stncp(csErrnam, newName, MAXPATH);

		return -1;
	}

	return AddCsName(pCategoryIn, newName);
}

/**********************************************************************
**	strm = CS_ctopn (mode);
**
**	char *mode;					mode of the open, cs_map.h ala _STRM_???
**	csFILE *strm;				returns a stream pointer to the open file.
**
**	Will return NULL if a problem was encountered.
**
**	File is positioned past the magic number on the front.
**********************************************************************/
csFILE* EXP_LVL3 CS_ctopn (Const char *mode)
{
	extern char cs_Dir [];
	extern char *cs_DirP;
	extern char cs_Ctname [];
	extern char csErrnam [];

	size_t rd_cnt;

	csFILE *strm = NULL;

	cs_magic_t magic;

	strcpy (cs_DirP, cs_Ctname);
	strm = CS_fopen (cs_Dir,mode);
	if (strm != NULL)
	{
		magic = 0L;
		rd_cnt = CS_fread ((char *)&magic,1,sizeof (magic),strm);

		if (rd_cnt == sizeof (magic))
		{
			CS_bswap (&magic,"l");
			if (magic != cs_CTDEF_MAGIC)
			{
				CS_fclose (strm);
				strm = NULL;
				strcpy (csErrnam,cs_Dir);
				CS_erpt (cs_CS_BAD_MAGIC);
			}
		}
		else
		{
			if (CS_ferror (strm)) CS_erpt (cs_IOERR);
			else				  CS_erpt (cs_INV_FILE);
			CS_fclose (strm);
			strm = NULL;
		}
	}
	else
	{
		strcpy (csErrnam,cs_Dir);
		CS_erpt (cs_CT_DICT);
	}

	return (strm);
}

/**********************************************************************
**	int CSdelCategory(catName)
**
**	char *catName;	the category to be deleted
**
**	Returns 0 if the category has been deleted, a negative value otherwise.
**	cs_Error contains additional information then.
**********************************************************************/
int EXP_LVL3 CSdelCategory(Const char* catName)
{
	extern int cs_Error;
	extern char csErrnam [];
	extern short cs_Protect;

	int unlinkStatus = 0;
	struct cs_Ctdef_* pCategory = NULL;

	cs_Error = 0;

	if (GetCategoryPtr(catName, &pCategory))
	{
		//there's was an error trying to retrieve the category
		//the error has been already reported
		return -1;
	}

	//no category with that name exists
	if (NULL == pCategory)
	{
		CS_stncp(csErrnam, catName, MAXPATH);
		CS_erpt(cs_CT_NOT_FND);
		return -1;
	}

	//we've at the moment no way to store the [protect] information
	//in the category definitions;
	if (cs_Protect >= 0)
	{
		//protection of system categories is enabled
		if (TRUE == pCategory->protect)
		{
			CS_stncp(csErrnam, catName, MAXPATH);
			CS_erpt(cs_CT_PROT);
			return -1;
		}
	}

	unlinkStatus = UnlinkCategory(pCategory, TRUE);
	if (unlinkStatus)
	{
		return unlinkStatus;
	}

	return CSupdCategories(NULL);
}

/**********************************************************************
**	int = CSupdCategory (ctDefPtr);
**
**	Const struct cs_Ctdef_* ctDefPtr; The category instance to update
**
**	Adds, if not yet existing, a new category to the in-memory list.
**	If a category with the same name already exists, it will be overwritten
**	with the passed in content. The caller is responsible for free'ing
**	the instance passed in.
**
**	Function returns >= 0 on success (-1 for failure)
**	where +1 indicates, that the category had been updated and 0 that the category had been added.
**	Failure can be caused by:
**	1>	NULL input
**	2>	file I/O errors: this might happen, if the categories have not yet been read from
**		the dictionary
**	3>	other problems like out of memory: As the category instance's content will be copied,
		out of memory or any other system-related error conditions can occur.
**********************************************************************/
int EXP_LVL3 CSupdCategory(Const struct cs_Ctdef_* categoryIn)
{
	extern int cs_Error;
	extern char csErrnam [];
	extern short cs_Protect;

	char testCsName[cs_KEYNM_DEF] = { '\0' };

	struct cs_Ctdef_* toUpdate = NULL;
	struct cs_Ctdef_* pLocalCategory = NULL;

	int categoryUpdated = FALSE;
	int protectionEnabled = TRUE;
	int doCategoryUpdateOverlay = FALSE;
	
	ulong32_t index;

	if (cs_Protect < 0)
		protectionEnabled = FALSE;

	cs_Error = 0;

	if (NULL == categoryIn)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	//we accept all category names - as long as they are not empty...
	if ('\0' == *categoryIn->ctName)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	if (GetCategoryPtr(categoryIn->ctName, &toUpdate))
		return -1; //IO error, OOM or whatever else

	//now we've to overlay our internal category with what's in
	//our memory list by making sure, we're not overwriting something we're
	//actually not supposed to overwrite...
	if (NULL != toUpdate) //the category already exists
	{
		if (protectionEnabled && TRUE == toUpdate->protect)
			doCategoryUpdateOverlay = TRUE;

		//if the category is somehow prtected, we start with a local copy and overlay that...
		if (TRUE == doCategoryUpdateOverlay)
		{
			pLocalCategory = CScpyCategory(toUpdate);
		}
		else
		{
			//...otherwise,  we're simply copying in what the user has given us
			pLocalCategory = CScpyCategoryEx(NULL, categoryIn, FALSE /* don't set any protect flag */);
			pLocalCategory->userNameIndex = 0; //unset the [userNameIndex] information; the new category is fully writable here
		}

		if (NULL == pLocalCategory)
			return -1;

		if (TRUE == doCategoryUpdateOverlay)
		{
			//Now check, whether we'd be overwriting something;
			//we can only modify CS names which are not defined as being "system defined"
			if (categoryIn->userNameIndex != toUpdate->userNameIndex)
			{
				CS_erpt(cs_CT_PROT);
				CS_stncp(csErrnam, categoryIn->ctName, MAXPATH);

				goto error;
			}

			//"cut off" all user defined CS names...
			pLocalCategory->nameCnt = pLocalCategory->userNameIndex;
			//...and copy over all user defined names from the category we've been given
			for(index = categoryIn->userNameIndex; index < categoryIn->nameCnt; ++index)
			{
				CS_stncp(testCsName, categoryIn->csNames[index].csName, cs_KEYNM_DEF);
				if (CS_nampp(testCsName))
				{
					goto error;
				}

				if (AddCsName(pLocalCategory, testCsName))
					goto error;
			}
		}

		/* release [toUpdate] */
		LinkInCategory(pLocalCategory, toUpdate, TRUE);  /*lint !e534  ignoring return value */

		//make sure, we're not releasing anything anymore...
		pLocalCategory = NULL;
		toUpdate = NULL;

		categoryUpdated = TRUE;
	}
	else
	{
		pLocalCategory = CScpyCategory(categoryIn);
		if (NULL == pLocalCategory)
			return -1;

		//newly added categories cannot be protected
		pLocalCategory->userNameIndex = 0;
		pLocalCategory->protect = 0;

		if (AppendCategory(pLocalCategory))
			goto error;

		pLocalCategory = NULL;
	}

	//this will write the updated category file(s) completely
	if (0 != CSupdCategories(NULL))
		goto error;

	return (TRUE == categoryUpdated) ?
		1 //existing category was updated
		: 0; //new category has been added

error:
	if (NULL != pLocalCategory)
	{
		CSrlsCategory(pLocalCategory);
		pLocalCategory = NULL;
	}

	return -1;
}

/******************************************************************************
 * Serializes the given cs_Ctdef_ pointer to a file. If cs_UserDir[] has been set
 * all category information will either be written into that directory; otherwise
 * into cs_Dir[]. The file name will always be the name set in cs_ctname[].
 * Note, that calling this function will ALWAYS truncate the existing file.
 *
 * Function returns zero on success, -1 for failure.  Failure can be caused by:
 * 1> stream write error
 * 2> badly formatted cotegory structure.
 *****************************************************************************/
int EXP_LVL3 CSupdCategories (Const struct cs_Ctdef_* ctDefPtr)
{
	extern struct cs_Ctdef_* cs_CtDefHead;

	extern char cs_Dir[260];
	extern char *cs_DirP;
	extern char cs_UserDir [];
	extern char cs_Ctname [];

	int writeStatus = -1;
	cs_magic_t magic = cs_CTDEF_MAGIC;
	ulong32_t i;

	csFILE * targetFileStream = NULL;

	struct cs_Ctdef_* ctToWrite = NULL;

	char currentDir[MAXPATH] = { '\0' };
	char cs_dirTemp[MAXPATH] = { '\0' };

	//do updates have to go into our separate dictionary?
	int hasUserDictionary = ('\0' != *cs_UserDir);

	//if we haven't been given a pointer, we serialize all
	//category information we currently have in memory
	if (NULL == ctDefPtr)
		ctDefPtr = cs_CtDefHead;

	CS_getdr(currentDir);			/*lint !e534  jgnoring return value */

	//if we've a user dictionary path, we must only
	//write out the "user definitions"; the system file we must *not* touch
	if (hasUserDictionary)
	{
		if (CS_setdr(cs_UserDir, cs_dirTemp))
			goto error;
	}
	
	CS_stncp(cs_DirP, cs_Ctname, cs_FNM_MAXLEN);
	targetFileStream = CS_fopen(cs_Dir, _STRM_BINWR);
	if (NULL == targetFileStream)
		goto error;

	//put in the required magic header now that we've re-created the file
	if (1 != CS_fwrite(&magic, sizeof(cs_magic_t), 1, targetFileStream))
	{
		CS_erpt (cs_IOERR); //files are closed before exiting
		goto error;
	}

	writeStatus = 0;
	while (writeStatus >= 0 && ctDefPtr != NULL)
	{
		if (!hasUserDictionary //if we don't have 2 separate files...
			|| !ctDefPtr->protect) //... or this current category isn't protected (because it has been newly added by the user) ...
		{
			//... then, everything goes into our default file; write all out & done
			writeStatus = CSwrtCategory (targetFileStream, ctDefPtr);
		}
		else
		{
			//have there been names added to this "system" category?
			if (ctDefPtr->userNameIndex < ctDefPtr->nameCnt)
			{
				//Yes; so we've to split this category up and write all user defined entries to
				//the separate file
				ctToWrite = CSnewCategoryEx(ctDefPtr->ctName, TRUE);
				if (NULL == ctToWrite) //most likely OOM
					goto error;

				//now, take all user-added names as write them into the separate file
				for(i = ctDefPtr->userNameIndex; i < ctDefPtr->nameCnt; ++i)
				{
					if (AddCsName(ctToWrite, ctDefPtr->csNames[i].csName))
						goto error;
				}

				writeStatus = CSwrtCategory(targetFileStream, ctToWrite);
				if (writeStatus < 0)
					goto error;

				CSrlsCategory(ctToWrite);
				ctToWrite = NULL;
			}
		}
		
		ctDefPtr = ctDefPtr->next;
	}

	if (0 != writeStatus)
		goto error;

	if (NULL != targetFileStream)		/*lint !e774  boolean always evaluates to true */
	{
		CS_fclose(targetFileStream);
		targetFileStream = NULL;
	}

	//set the CSD file directory back to what it was before
	CS_setdr(currentDir, NULL);			/*lint !e534  ignoring return value */
	return 0;

error:
	if (NULL != targetFileStream)
	{
		CS_fclose(targetFileStream);
		targetFileStream = NULL;
	}

	CSrlsCategory(ctToWrite);
	ctToWrite = NULL;

	//set the CSD file directory back to what it was before
	CS_setdr(currentDir, NULL);			/*lint !e534  ignoring return value */

	return -1;
}

/******************************************************************************
 * Writes the provided category to the provided stream.  Stream must be opened
 * in binary mode as binary data will be written.  (Of course, it must also be
 * opened for writing.)  The new category data will be written at the current
 * position of the stream.
 *
 * Function returns zero on success, -1 for failure.  Failure can be caused by:
 * 1> stream write error
 * 2> badly formatted cotegory structure.
 *****************************************************************************/
int EXP_LVL3 CSwrtCategory (csFILE* stream,Const struct cs_Ctdef_*ctDefPtr)
{
	size_t wrCnt;
	unsigned idx;
	struct cs_CtItmName_* itmPtr;
	struct cs_CtItmName_ dummy;

	if (NULL == stream)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	if (NULL == ctDefPtr)
	{
		CS_erpt(cs_INV_ARG2);
		return -1;
	}

	CS_stncp (dummy.csName,cs_CATDEF_UNUSED,sizeof (dummy.csName));
	wrCnt = CS_fwrite (ctDefPtr->ctName,sizeof (ctDefPtr->ctName),1,stream);
	if (wrCnt == 1)
	{
		wrCnt = CS_fwrite(&ctDefPtr->nameCnt,sizeof(ctDefPtr->nameCnt),1,stream);
	}
	if (wrCnt == 1)
	{
		wrCnt = CS_fwrite (&ctDefPtr->allocCnt,sizeof(ctDefPtr->allocCnt),1,stream);
	}

	/* Item names. */
	for (idx = 0;wrCnt == 1 && idx < ctDefPtr->allocCnt;idx += 1)
	{
		if (idx < ctDefPtr->nameCnt)
		{
			itmPtr = ctDefPtr->csNames + idx;
			wrCnt = CS_fwrite (itmPtr->csName,sizeof (itmPtr->csName),1,stream);
		}
		else
		{
			wrCnt = CS_fwrite (dummy.csName,sizeof (dummy.csName),1,stream);
		}
	}
	return (wrCnt == 1) ? 0 : -1;
}

int EXP_LVL3 CSrplCatNameEx (Const char* oldCtName, Const char* newCtName)
{
	extern char csErrnam [];
	extern int cs_Error;
	struct cs_Ctdef_* liveCatPtr = NULL;

	cs_Error = 0;

	CS_CHECK_EMPTY_STRNG_ARG(oldCtName, 1);
	CS_CHECK_EMPTY_STRNG_ARG(newCtName, 2);

	if (GetCategoryPtr(oldCtName, &liveCatPtr))
		return -1;

	if (NULL == liveCatPtr)
	{
		CS_stncp(csErrnam, oldCtName, MAXPATH);
		CS_erpt(cs_CT_NOT_FND);

		goto error;
	}

	if (TRUE == liveCatPtr->protect)
	{
		CS_stncp(csErrnam, oldCtName, MAXPATH);
		CS_erpt(cs_CT_PROT);

		goto error;
	}

	CS_stncp (liveCatPtr->ctName, newCtName, cs_CATDEF_CATNMSZ);
	return 0;

error:
	return -1;
}

int EXP_LVL3 CSrplCatName (Const char* newCtName, unsigned idx)
{
	extern int cs_Error;

	struct cs_Ctdef_* liveCatPtr = NULL;

	cs_Error = 0;

	CS_CHECK_EMPTY_STRNG_ARG(newCtName, 1);
	if (GetCategoryPtrIdx(idx, &liveCatPtr))
		return -1;

	if (NULL == liveCatPtr)
	{
		CS_erpt(cs_INV_ARG2);
		return -1;
	}

	return CSrplCatNameEx(liveCatPtr->ctName, newCtName);
}

int EXP_LVL3 CSaddCategory (Const char* catName)
{
	extern int cs_Error;
	struct cs_Ctdef_* newDefPtr = NULL;

	cs_Error = 0;

	if (NULL == catName || '\0' == *catName)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	/* Create a new empty category. */
	newDefPtr = CSnewCategory (catName);
	if (newDefPtr == NULL)
		goto error;

	//Add the new category struct to the current list.
	if (AppendCategory(newDefPtr))
		goto error;

	return 0;

error:
	if (NULL != newDefPtr)
	{
		CSrlsCategory(newDefPtr);
		newDefPtr = NULL;
	}

	return -1;
}

struct cs_Ctdef_* EXP_LVL3 CSnewCategory (Const char* ctName)
{
	return CSnewCategoryEx(ctName, TRUE);
}

struct cs_Ctdef_* EXP_LVL3 CSnewCategoryEx (Const char* ctName, int preAllocate)
{
	extern int cs_Error;
	struct cs_Ctdef_* newDefPtr;

	cs_Error = 0;

	newDefPtr = (struct cs_Ctdef_*) CS_malc (sizeof (struct cs_Ctdef_));
	if (newDefPtr == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	
	memset(newDefPtr, 0x0, sizeof(struct cs_Ctdef_));

	if (NULL != ctName && '\0' != ctName[0])
		CS_stncp (newDefPtr->ctName,ctName,sizeof (newDefPtr->ctName));

	if (TRUE == preAllocate)
	{
		if (ExtendCsNameBlock(newDefPtr, 0))
		{
			CSrlsCategory(newDefPtr);
			goto error;
		}
	}

	return newDefPtr;

error:
	return NULL;
}

struct cs_Ctdef_* EXP_LVL3 CSrdCategory (csFILE* stream)
{
	extern int cs_Error;
	struct cs_Ctdef_* ctDefPtr = NULL;
	int readStatus;

	cs_Error = 0;

	ctDefPtr = CSnewCategoryEx(NULL, FALSE);
	if (ctDefPtr == NULL)
		goto error;
	
	readStatus = CSrdCategoryEx(stream, ctDefPtr);
	if (readStatus < 0)
		goto error;

	if (0 == readStatus)
	{
		//free the instance if we've reached EOF
		CSrlsCategory(ctDefPtr);
		ctDefPtr = NULL;
	}

	//note, that [cs_Ctdef_::previous] and [cs_Ctdef_::next] are set to NULL for
	//the instance being returned here
	return ctDefPtr; //might be NULL in case of EOF

error:
	CSrlsCategory(ctDefPtr);
	ctDefPtr = NULL;

	return NULL;
}

int EXP_LVL3 CSrdCategoryEx (csFILE* stream, struct cs_Ctdef_ *ctDefPtr)
{
	extern int cs_Error;
	size_t rdCnt;
	unsigned idx;
	unsigned allocSize;
	struct cs_CtItmName_ *destPtr;
	size_t csNameSize = sizeof(((struct cs_CtItmName_ *)0)->csName) / sizeof(char);

	cs_Error = 0;

	if (NULL == stream)
	{
		CS_erpt(cs_INV_ARG1);
		return -1;
	}

	if (NULL == ctDefPtr)
	{
		CS_erpt(cs_INV_ARG2);
		return -1;
	}

	//clean the category instance because we'll be filling it with
	//the information from the file
	CSclnCategory(ctDefPtr);				/*lint !e534  ignoring return value */

	rdCnt = CS_fread (ctDefPtr->ctName,sizeof (ctDefPtr->ctName),1,stream);
	if (rdCnt == 1)
	{
		rdCnt = CS_fread (&ctDefPtr->nameCnt,sizeof (ctDefPtr->nameCnt),1,stream);
	}
	if (rdCnt == 1)
	{
		ctDefPtr->userNameIndex = ctDefPtr->nameCnt;
		rdCnt = CS_fread (&ctDefPtr->allocCnt,sizeof (ctDefPtr->allocCnt),1,stream);
	}
	if (rdCnt != 1)
	{
		if (CS_ferror (stream))
		{ 
			CS_erpt (cs_IOERR);
		}

		//could be EOF
		goto error;
	}

	allocSize = ctDefPtr->allocCnt * sizeof (struct cs_CtItmName_);
	ctDefPtr->csNames = (struct cs_CtItmName_*) CS_malc (allocSize);
	if (ctDefPtr->csNames == NULL)
	{
		CS_erpt (cs_NO_MEM);
		goto error;
	}
	for (idx = 0;idx < ctDefPtr->allocCnt;idx += 1)
	{
		destPtr = (ctDefPtr->csNames + idx);
		rdCnt = CS_fread (destPtr,sizeof (struct cs_CtItmName_),1,stream);
		if (rdCnt != 1)
		{
			CS_erpt (cs_IOERR);
			goto error;
		}

		//and make sure, the name is NULL-terminated
		(ctDefPtr->csNames + idx)->csName[(csNameSize) - 1] = '\0';
	}

	return 1;

error:
	CSclnCategory(ctDefPtr);			/*lint !e534  ignoring return value */
	return (0 != cs_Error) ? -1 : 0;
}

struct cs_Ctdef_* EXP_LVL3 CSrdCatFile ()
{
	extern char csErrnam [];
	extern int cs_Error;
	extern char cs_Dir [];
	extern char cs_UserDir [];
	
	struct cs_Ctdef_* ctDefHead = NULL;

	csFILE* stream = NULL;
	struct cs_Ctdef_ *ctDefPtr = NULL;
	struct cs_Ctdef_ *ctLastDefPtr = NULL;
	struct cs_Ctdef_ *systemCategory = NULL;

	ulong32_t csNameIndex = 0;

	unsigned i = 0;
	int dictionaryFound = FALSE;
	int currentIsUserDictionary = FALSE;
	int hasUserDictionaryPath = FALSE;
	
	char targetPaths[2][MAXPATH] = { {'\0'},
									 {'\0'} };		/* keeping gcc happy */
	char const* pTargetPath;

	char currentDir[MAXPATH] = { '\0' }; //the variable we're temporarily storing the current path in

	char tempDir[MAXPATH] = { '\0' };

	cs_Error = 0;
	hasUserDictionaryPath = ('\0' != cs_UserDir[0]);

	CS_getdr(currentDir);				/*lint !e534  ignoring return value */
	
	//first look into the directory first where we're storing the system CSD files in.
	//note, that this is different to all other types of dictionaries.
	CS_stncp(targetPaths[0], currentDir, sizeof(targetPaths[0]));
	CS_stncp(targetPaths[1], cs_UserDir, sizeof(targetPaths[1]));

	for (i = 0; i < (sizeof(targetPaths) / sizeof(targetPaths[0])); ++i)
	{
		if (NULL != stream)
		{
			CS_fclose (stream);
			stream = NULL;
		}

		/* Search the file for the requested coordinate system definition. */
		pTargetPath = targetPaths[i];
		if ('\0' == *pTargetPath)
			continue;

		//switch [cs_dir] and [cs_DirP] to whatever the current target directory is
		if (CS_setdr(pTargetPath, tempDir))
			goto error;

		stream = CS_ctopn(_STRM_BINRD);

		//then switch it back to our [cs_dir] folder
		if (CS_setdr(tempDir, NULL))
			goto error;

		if (stream == NULL)
		{
			cs_Error = 0; //no problem; we've more paths to go...
			continue;
		}
		
		dictionaryFound = TRUE;

		ctDefPtr = NULL;
		ctLastDefPtr = NULL;

		currentIsUserDictionary = (i == 1);
		for (;;)
		{
			//at this point we know, that the file exists and we could open it.
			//So, no matter whether this was the user-defined file or
			//the "system" file, we don't continue if there was an error reading
			//from the file
			ctDefPtr = CSrdCategory (stream);
			if (ctDefPtr == NULL)
			{
				if (cs_Error != 0)
				{
					goto error;
				}

				break; //EOF reached
			}

			//what's the file we're reading from?
			if (!currentIsUserDictionary)
			{
				ctDefPtr->protect = hasUserDictionaryPath ? TRUE : FALSE;
				ctDefPtr->previous = ctLastDefPtr; //[ctLastDefPtr] can be NULL here
			}
			else
			{
				systemCategory = NULL;
				//categories read from "cs_UserDir" are write enabled by default
				ctDefPtr->protect = FALSE;
				if (NULL != ctDefHead)
				{
					if (GetCategoryPtrEx(ctDefHead, ctDefPtr->ctName, &systemCategory))
						goto error; //if this has failed, something is going *very* bad here...
				}
				//else: systemCategory is still NULL... meaning that [ctDefPtr] will be made our new head

				if (NULL != systemCategory)
				{
					//a system category already exists - we now have to overlay the in-memory structures
					//with what's in the separate file
					systemCategory->userNameIndex = systemCategory->nameCnt;
					for (csNameIndex = 0; csNameIndex < ctDefPtr->nameCnt; ++csNameIndex)
					{
						if (AddCsName(systemCategory, ctDefPtr->csNames[csNameIndex].csName))
							goto error;
					}

					//we've copied all names over - now, make sure, we don't use this unlinked category instance...
					CSrlsCategory(ctDefPtr);
					ctDefPtr = NULL;

					//...and skip the part below; continue with the next entry; a head we already have
					continue;
				}
				//else:
				//let the newly read category be added to the existing list
				//see code block below
			}

			/* Add it to the existing category tree. */
			if (ctDefHead == NULL)
			{
				ctDefHead = ctDefPtr;
			}
			else
			{
				//get the last category in the list
				if (NULL == ctLastDefPtr)
				{
					ctLastDefPtr = ctDefHead;
					while (ctLastDefPtr->next != NULL)
					{
						ctLastDefPtr = ctLastDefPtr->next;
					}
				}

				//and interconnect the 2 categories
				ctLastDefPtr->next = ctDefPtr;
				ctDefPtr->previous = ctLastDefPtr;
			}

			ctLastDefPtr = ctDefPtr; //keep track of the last cs_Ctdef_ instance we linked in

		} //for (;;)
	}

	if (stream != NULL)
	{
		CS_fclose (stream);
		stream = NULL;
	}

	if (FALSE == dictionaryFound)
	{
		strcpy (csErrnam, cs_Dir);
		CS_erpt (cs_CSDICT);

		goto error; //make our intention clear...
	}

	return ctDefHead;

error:

	if (ctDefHead != NULL)
	{
		CSrlsCategoryList (ctDefHead);
		ctDefHead = NULL;
	}

	if (stream != NULL)
	{
		CS_fclose (stream);
		stream = NULL;
	}
	return NULL;
}

int EXP_LVL3 CSwrtCatFile (csFILE* stream,Const struct cs_Ctdef_ *ctDefPtr)
{
	int st;

	st = 0;
	while (st >= 0 && ctDefPtr != NULL)
	{
		st = CSwrtCategory (stream,ctDefPtr);
		ctDefPtr = ctDefPtr->next;
	};
	return st;
}
void EXP_LVL3 CSrlsCategory (struct cs_Ctdef_ *ctDefPtr)
{
	if (ctDefPtr != NULL)
	{
		if (ctDefPtr->csNames != NULL)
		{
			CS_free (ctDefPtr->csNames);
		}
		CS_free (ctDefPtr);
	}
}

void EXP_LVL3 CSrlsCategoryList (struct cs_Ctdef_ *ctDefHead)
{
	struct cs_Ctdef_ *tmpPtr;
	
	while (ctDefHead != NULL)
	{
		tmpPtr = ctDefHead;
		ctDefHead = ctDefHead->next;
		CSrlsCategory (tmpPtr);
	}
}


void EXP_LVL3 CSrlsCategories(void)
{
	extern struct cs_Ctdef_* cs_CtDefHead;

	CSrlsCategoryList(cs_CtDefHead);
	cs_CtDefHead = NULL;
}

/**********************************************************************
**	err_cnt = CSctcomp (inpt,outp,flags,err_func);
**
**	char *inpt;					full pathe name to the ASCII source file.
**	char *outp;					full path name to the target file.
**	int flags;					bitmat of options, see REMARKS below.
**	int (*err_func)(char *mesg);function called top present error messages
**								to the user.
**	int err_cnt;				returns the number of errors reported.
**
**	The flags argument:
**	cs_CMPLR_WARN   -- emit warning messages.
**
**	Please excuse the rather crude nature of this program.  However,
**	it is necessary for this program to be compiled, linked, and
**	run in just about any environment, without requiring users
**	to license LEX/YACC.
**********************************************************************/

/*lint -esym(550,test,warn)       not accessed, but potentially useful */
int EXP_LVL9 CSctcomp (	Const char *inpt,Const char *outp,int flags,Const char *coordsys,
																		  int (*err_func)(char *mesg))
{
	extern struct cs_Ctdef_* cs_CtDefHead;
	extern char cs_DirsepC;

	int st;
	int demo;
	int test;
	int crypt;
	int warn;
	int cancel;
	int line_nbr;
	int err_cnt;
	int length;
	size_t rdCnt;
	size_t wrCnt;

	short csChk;

	char *cp;
	char *cp1;
	csFILE *inStrm;
	csFILE *outStrm;
	csFILE *csStrm;

	struct cs_Ctdef_ *ctDefCurrent = NULL;

	cs_magic_t magic;
	struct cs_CtItmName_ dummy;

	char err_seg [18];
	char buff [512];
	char err_msg [128];
	char catName [64];
	char itmName [sizeof (dummy.csName)];
	
	enum lineType {typCatName, typItmName, typBogus} lineType;

	/* Initialize */
	err_cnt = 0;
	cancel = FALSE;
	line_nbr = 0;
	warn = ((flags & cs_CMPLR_WARN) != 0);
	catName [0] = '\0';
	itmName [0] = '\0';

	demo  = ((flags & cs_CMPLR_DEMO)  != 0);
	crypt = ((flags & cs_CMPLR_CRYPT) != 0);
	test  = ((flags & cs_CMPLR_TEST)  != 0);
	warn  = ((flags & cs_CMPLR_WARN)  != 0);

	/* Release and reset any existing category list.  Should not be
	   necssary; rather defensive. */
	CSrlsCategories();

	/* OK, lets do it.  Open the source file. */
	inStrm = CS_fopen (inpt,_STRM_TXTRD);
	if (inStrm == NULL)
	{
		err_cnt += 1;
		sprintf (err_msg,"Couldn't open %s for input.",inpt);
		cancel = (*err_func)(err_msg);
		return err_cnt;
	}

	/* If we have been given a coordinate system file name, we open it now. */
	if (coordsys != NULL && *coordsys != '\0')
	{
		csStrm = CS_fopen (coordsys,_STRM_BINRD);
		if (csStrm == NULL)
		{
			sprintf (err_msg,"Couldn't open %s as a datum file.",coordsys);
			(*err_func)(err_msg);			/*lint !e534  ignoring return value */
			CS_fclose (inStrm);
			return (1);
		}
		rdCnt = CS_fread (&magic,1,sizeof (magic),csStrm);
		CS_bswap (&magic,"l");
		if (rdCnt != sizeof (magic) ||
			(demo && magic != cs_CSDEF_MAGIC) ||
			(!demo && magic != cs_CSDEF_MAGIC))
		{
			sprintf (err_msg,"%s is not a Coordinate System Dictionary file.",coordsys);
			(*err_func)(err_msg);			/*lint !e534  ignoring return value */
			CS_fclose (inStrm);
			CS_fclose (csStrm);
			return (1);
		}
	}
	else
	{
		csStrm = NULL;
	}

	/* Process each line in the source file. */
	while (CS_fgets (buff,sizeof (buff),inStrm) != NULL)
	{
		if (cancel)
		{
			CS_fclose (inStrm);
			if (csStrm != NULL) CS_fclose (csStrm);
			CSrlsCategories();
			return (err_cnt);
		}
		line_nbr += 1;

		/* Ignore comments and blank lines. */
		(void)CS_trim (buff);
		if (buff [0] == ';' || buff [0] == '\0')
		{
			continue;
		}
		cp = buff;
		while ((cp = strchr (cp,';')) != NULL)
		{
			if (*(cp + 1) != ';' &&
				*(cp - 1) != cs_DirsepC)
			{
				*cp = '\0';
				break;
			}
		}

		/* Determine the type of this line.  Since we have already handled
		   comment lines, the line type can be one of three types:
			1> Category name line, i.e. has [...]
			2> Item name, i.e. AAAA = ....
			3> Bogus line, i.e. anything else  */
		lineType = typBogus;				/* until we know different */
		cp = strchr (buff,'[');
		cp1 = strchr (buff,'=');
		if (cp != NULL)
		{
			/* It's a category name or it's bogus. */
			cp1 = strchr (buff,']');
			if (cp1 != NULL)
			{
				length = (int)(cp1 - cp) - 1;
				if (length > 1 && length < (int)sizeof (catName))
				{
					lineType = typCatName;
					cp += 1;
					CS_stncp (catName,cp,length + 1);
				}
			}
			/* Linetype already set to bogus. */
		}
		else if (cp1 != NULL)
		{
			/* Looks like an item name.  Let's see. */
			*cp1 = '\0';
			CS_trim (buff);
			if (strlen (buff) < sizeof (dummy.csName))
			{
				/* Looks good, should be an item name. */
				lineType = typItmName;
				CS_stncp (itmName,buff,sizeof (itmName));
			}
		}
		/* lineType already set to bogus above. */

		if (lineType == typBogus)
		{
			err_cnt += 1;
			CS_stncp (err_seg,buff,sizeof (err_seg));
			sprintf (err_msg,"Invalid format detected (%s) at line %d.",err_seg,line_nbr);
			cancel = (*err_func)(err_msg);
			continue;
		}

		/* OK, deal with the current line. */
		if (lineType == typCatName)
		{
			if (NULL == cs_CtDefHead)
			{
				//if this is the first category we've read, we've to 
				//set our extern [cs_CtDefHead] pointer below...
				ctDefCurrent = CSnewCategory(catName);
				st = (NULL == ctDefCurrent) ? -1 : 0;
			}
			else
			{
				//...otherwise: simply create a new category and add it to the in-memory list
				st = CSaddCategory(catName);
			}

			if (st)
			{
				sprintf (err_msg,"Category memory allocation failed at line %d.",line_nbr);
				cancel = (*err_func)(err_msg);
				continue;
			}
			else if (NULL == cs_CtDefHead)
				cs_CtDefHead = ctDefCurrent;
		}
		else if (lineType == typItmName)
		{
			/* Verify that we have a valid category started. */
			if (catName [0] == '\0')
			{
				err_cnt += 1;
				CS_stncp (err_seg,buff,sizeof (err_seg));
				sprintf (err_msg,"Item name detected outside of category at line %d (%s).",line_nbr,err_seg);
				cancel = (*err_func)(err_msg);
				continue;
			}

			/* Verify that the item name is that of a coordinate system. */
			csChk = CSctCompCsChk (csStrm,itmName);
			if (csChk < 0)
			{
				/* The coordinate system does not exists, for wehatever reason. */
				continue;
			}
			if (csChk > 0)
			{
				/* The coordinate system is classified as being in the LEGACY group. */
				if (CS_strnicmp (catName,"Obsolete",8))
				{
					/* The category name is not the Obsolete Coordinate System Group. */
					continue;
				}
			}

			/* Add the new item name to the current category. */
			st = CSaddItmName (catName,itmName);
			if (st != 0)
			{
				err_cnt += 1;
				CS_stncp (err_seg,buff,sizeof (err_seg));
				sprintf (err_msg,"Addition of item name (%s) at line %d failed.",err_seg,line_nbr);
				cancel = (*err_func)(err_msg);
				continue;
			}
		}
	}
	CS_fclose (inStrm);
	if (csStrm != NULL) CS_fclose (csStrm);

	if (!cancel)
	{
		/* Open the output file and write the magic number.  BINRW creats a
		   new file (truncates if necessary) with read write access.  We
		   need read/write as we will be sorting below. */
		outStrm = CS_fopen (outp,_STRM_BINWR);
		if (outStrm == NULL)
		{
			err_cnt += 1;
			sprintf (err_msg,"Couln't open %s for output.",outp);
			cancel = (*err_func)(err_msg);
			return err_cnt;
		}

		magic = cs_CTDEF_MAGIC;
		CS_bswap (&magic,"l");
		wrCnt = CS_fwrite (&magic,1,sizeof (magic),outStrm);
		if (wrCnt != sizeof (magic))
		{
			err_cnt += 1;
			sprintf (err_msg,"Write failure on %s.",outp);
			cancel = (*err_func)(err_msg);
			CS_fclose (outStrm);
			CS_remove (outp);										/*lint !e534 */
			return err_cnt;
		}

		st = 0;
		if (NULL != cs_CtDefHead)
		{
			st = CSwrtCatFile(outStrm, cs_CtDefHead);
		}

		CSrlsCategories();
		CS_fclose (outStrm);
		if (st != 0)
		{
			err_cnt += 1;
			sprintf (err_msg,"Category file write failure on %s.",outp);
			cancel = (*err_func)(err_msg);
			CS_remove (outp);										/*lint !e534 */
		}
	}

	/* Close up and get out. */
	return err_cnt;	
}
/*lint +esym(550,test,warn) */

/* The following verifies that the proposed coordinate system name exists.
   Returns -1 if it doesn't exist, +1 if it exists and is in the LEGACY
   group.  Otherwise a zero is returned.  In the absence of a coordinate
   system file stream, a zero is always returned.*/
short CSctCompCsChk (csFILE *csStrm,Const char* csName)
{
	short rtnValue;
	int flag;
	size_t rdCnt;
	struct cs_Csdef_ cs_def;

	rtnValue = 0;	
	if (csStrm != NULL)
	{
		CS_stncp (cs_def.key_nm,csName,sizeof (cs_def.key_nm));
		CS_nampp (cs_def.key_nm);			/*lint !e534   ignoring return value */
		cs_def.fill [0] = '\0';
		cs_def.fill [1] = '\0';
		flag = CS_bins (csStrm,(long32_t)sizeof (cs_magic_t),(long32_t)-1,sizeof (cs_def),&cs_def,(CMPFUNC_CAST)CS_cscmp);
		if (flag)
		{
			rdCnt = CS_fread (&cs_def,sizeof (cs_def),1,csStrm);
			if (rdCnt == 1)
			{
				if (!CS_stricmp (cs_def.group,"LEGACY"))
				{
					rtnValue = 1;
				}
			}
		}
		else
		{
			rtnValue = -1;
		}
	}
	return rtnValue;
}
