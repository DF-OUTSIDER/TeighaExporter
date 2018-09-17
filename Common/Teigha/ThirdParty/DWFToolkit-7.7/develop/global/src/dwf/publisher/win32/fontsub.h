/*************************************************************************
*                                                                        *
* fontsub.h -- font subsetting services (fontsub.dll)                    *
*                                                                        *
* (c) Microsoft Corporation. All Rights Reserved.                        *
*                                                                        *
*************************************************************************/

#ifndef FONTSUB_DOT_H_DEFINED
#define FONTSUB_DOT_H_DEFINED
  
#ifndef CONST
#define CONST const
#endif     

typedef void *(__cdecl *CFP_ALLOCPROC)(size_t);
typedef void *(__cdecl *CFP_REALLOCPROC)(void *, size_t);
typedef void (__cdecl *CFP_FREEPROC)(void *);

unsigned long __cdecl CreateFontPackage(
	CONST unsigned char * puchSrcBuffer, /* input TTF or TTC buffer */
	CONST unsigned long ulSrcBufferSize, /* size of input TTF or TTC buffer data */
	unsigned char ** ppuchFontPackageBuffer, /* output package buffer */
	unsigned long * pulFontPackageBufferSize, /* output package buffer size */
	unsigned long * pulBytesWritten,  /* output package buffer data length */
	CONST unsigned short usFlag, /* subset, compress, or both, TTF or TTC, Chars or Glyphs */
	CONST unsigned short usTTCIndex,	/* TTC Index, only used if TTC bit set */
	CONST unsigned short usSubsetFormat, /* Old Subset, Subset or Delta */
	CONST unsigned short usSubsetLanguage,	/* Language in Name table to keep */
	CONST unsigned short usSubsetPlatform,	/* if ListType is Character, Platform of cmap to use for glyph list */
	CONST unsigned short usSubsetEncoding,	/* if ListType is Character, Encoding of cmap to use for glyph list */
	CONST unsigned short *pusSubsetKeepList, /* List of Characters or Glyphs to keep */
	CONST unsigned short usSubsetListCount,	 /* number of elements in list */
	CFP_ALLOCPROC lpfnAllocate,	  /* call back function to allocate temp buffers and output buffers */
	CFP_REALLOCPROC lpfnReAllocate,	  /* call back function to reallocate temp and output buffers */
	CFP_FREEPROC lpfnFree,	 /* call back function to free buffer allocated with lpfnAllocate and lpfnReAllocate */
	void * lpvReserved);

/* for usSubsetFormat */
#define TTFCFP_SUBSET 0	  /* Straight Subset Font - Backward compatibility */
#define TTFCFP_SUBSET1 1	  /* Subset font with full TTO and Kern tables. For later merge */
#define TTFCFP_DELTA 2	  /* Delta font, for merge with a subset1 font */

/* for usSubsetPlatform ID values */
#define TTFCFP_UNICODE_PLATFORMID 0
#define TTFCFP_APPLE_PLATFORMID   1
#define TTFCFP_ISO_PLATFORMID     2
#define TTFCFP_MS_PLATFORMID      3

/* for usSubsetEncoding values */
#define TTFCFP_STD_MAC_CHAR_SET  0	/* goes with TTFSUB_APPLE_PLATFORMID */
#define TTFCFP_SYMBOL_CHAR_SET  0	/* goes with TTFSUB_MS_PLATFORMID */
#define TTFCFP_UNICODE_CHAR_SET  1	/* goes with TTFSUB_MS_PLATFORMID */
#define TTFCFP_DONT_CARE  0xFFFF

/* for usSubsetLanguage values */
#define TTFCFP_LANG_KEEP_ALL 0

/* for usFlags values */
#define TTFCFP_FLAGS_SUBSET 0x0001	/* if bit off, don't subset */
#define TTFCFP_FLAGS_COMPRESS 0x0002  /* if bit off, don't compress */
#define TTFCFP_FLAGS_TTC 0x0004  /* if bit off, its a TTF */
#define TTFCFP_FLAGS_GLYPHLIST 0x0008 /* if bit off, list is characters */

unsigned long __cdecl MergeFontPackage(CONST unsigned char * puchMergeFontBuffer, /* buffer containing font to merge with */
			CONST unsigned long ulMergeFontBufferSize,	/* size of buffer containing font to merge with */
			CONST unsigned char * puchFontPackageBuffer, /* buffer containing a font package to merge with the MergeFontBuffer */
			CONST unsigned long ulFontPackageBufferSize, /* length of FontPakageBuffer */
			unsigned char **ppuchDestBuffer, /* output: pointer to output buffer containing a TTF to install */
			unsigned long *pulDestBufferSize, /* output: length of output buffer containing TTF to install */
			unsigned long *pulBytesWritten, /* output: number of bytes in buffer used for TTF */
			CONST unsigned short usMode, /* kind of action to perform, see #defines below */ 
			CFP_ALLOCPROC lpfnAllocate,	  /* call back function to allocate output and intermediate buffers */
			CFP_REALLOCPROC lpfnReAllocate,	  /* call back function to allocate or reallocate output and intermediate buffers */
			CFP_FREEPROC lpfnFree,	 /* call back function to free buffer allocated with lpfnReAllocate */
			void *lpvReserved);

/* for usModes */
#define TTFMFP_SUBSET 0   /* copy a Straight Subset Font package to Dest buffer */
#define TTFMFP_SUBSET1 1  /* Expand a format 1 font into a format 3 font */
#define TTFMFP_DELTA 2	   /* Merge a format 2 with a format 3 font */

#endif /* FONTSUB_DOT_H_DEFINED */

