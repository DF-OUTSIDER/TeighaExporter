/******************************************************************************
*******************************************************************************
**                                                                           **
**               I M P O R T A N T    N O T I C E                            **
**                                                                           **
** As of this writing, it is as yet undetermined if it is permisible to      **
** publish polynomial coefficients related to the Danish System 34.  In the  **
** absence of these polynomial coefficients, all coordinate conversion       **
** related to the Danish System 34 coordinate systems will produce results   **
** which are intentionally outlandish; this be be sure no one accidently     **
** uses the results.                                                         **
**                                                                           **
** These coefficients are freely available from Kort-og Matrikelstyrelsen,   **
** Denmark.  The following information may be helpful in obtaining this      **
** data.                                                                     **
**                                                                           **
**	http://www.kms.dk/                                                       **
**	http://www.kms.dk/ENGLISH                                                **
**	kms@kms.dk                                                               **
**                                                                           **
** You are in need of a single file named 'tcts_u.c' in the KMSTransLib      **
** Source Code Distribution.  You do not need the entire distribution, that  **
** single file is sufficient.  Once this file has been obtained, select out  **
** the code as indicated below and paste it into this file, replacing the    **
** dummy definitions provided below.  Save this file as so modified and      **
** procede to build the CS-MAP library as directed by the build              **
** instructions. This will cause the correct implementation of the Danish    **
** System 34 coordinate systems.                                             **
**                                                                           **
** The portions of the 'tcts_u.c' file that you need are as follows.         **
** Line numbers accurate as of July 2008 are provided in parenthesis.  It is **
** recommended that you use the line numbers only as a guide, as the source  **
** code module is subject to change over time.                               **
**                                                                           **
** The definitions of the following preprocessor macros:                     **
**                                                                           **
**		S34JYMAX	(  48)                                                   **
**		S34JYMIN                                                             **
**		S34JXMAX                                                             **
**		S34JXMIN                                                             **
**                                                                           **
**		S34SYMAX                                                             **
**		S34SYMIN                                                             **
**		S34SXMAX                                                             **
**		S34SXMIN                                                             **
**                                                                           **
**		S45BYMAX                                                             **
**		S45BYMIN                                                             **
**		S45BXMAX                                                             **
**		S45BXMIN	(  61)                                                   **
**                                                                           **
**		T32JNMAX	(  67)                                                   **
**		T32JNMIN                                                             **
**		T32JEMAX                                                             **
**		T32JEMIN                                                             **
**                                                                           **
**		T32SNMAX                                                             **
**		T32SNMIN                                                             **
**		T32SEMAX                                                             **
**		T32SEMIN                                                             **
**                                                                           **
**		T32BNMAX                                                             **
**		T32BNMIN                                                             **
**		T32BEMAX                                                             **
**		T32BEMIN	(  64)                                                   **
**                                                                           **
**	The definitions of the arrays of of doubles with the following names:    **
**                                                                           **
**		C_ttj		(  177 ->  304)                                          **
**		C_jtt		(  310 ->  436)                                          **
**		C_tts		(  442 ->  520)                                          **
**		C_stt		(  526 ->  604)                                          **
**		C_ttb		(  610 ->  665)                                          **
**		C_btt		(  671 ->  726)                                          **
**                                                                           **
** Of course, you may include, or exclude, extraneous code which will not    **
** interfere with the correct compilation of the inserted code.              **
**                                                                           **
** There exist more than one version of these coefficient arrays.  These     **
** versions are distinguished by the attribute we have arbitraily named      **
** vintage.  The KMS distribution only includes the latest "vintage".  You   **
** may have access to coefficients of earlier "vintages", and provisions     **
** for including same are provided below.                                    **
**                                                                           **
** As of this writing (Juoly 2008), the "vintage" attribute refers to the    **
** following versions:                                                       **
**                                                                           **
** Pre 1999: Coefficients published in 1991 and used in the original CS-MAP  **
**           implementation of System34.  Names for coefficient arrays for   **
**           this vintage will have the '98' sequence in their names.        **
**     1999: Coefficients published in 1999 and incorporated into CS-MAP in  **
**           release 10.15 (Sept 2002).                                      **
**     2001: Coefficients currenly (July 2008) available from KMS which      **
**           includes a new set of coefficients for Bornholm only.           **
**           Coefficients for Jylland and Sjælland remained the same as the  **
**           1999 vintage.                                                   **
**                                                                           **
*******************************************************************************
******************************************************************************/

/* NOTE: This file is not intended to be compiled independently.  It is
   'include'ed in the appropriate code (see CS_sys34.c) */

#define  S34JYMAX 0.0
#define  S34JYMIN 0.0
#define  S34JXMAX 0.0
#define  S34JXMIN 0.0

#define  S34SYMAX 0.0
#define  S34SYMIN 0.0
#define  S34SXMAX 0.0
#define  S34SXMIN 0.0

#define  S45BYMAX 0.0
#define  S45BYMIN 0.0
#define  S45BXMAX 0.0
#define  S45BXMIN 0.0

#define  T32JNMAX 0.0
#define  T32JNMIN 0.0
#define  T32JEMAX 0.0
#define  T32JEMIN 0.0

#define  T32SNMAX 0.0
#define  T32SNMIN 0.0
#define  T32SEMAX 0.0
#define  T32SEMIN 0.0

#define  T32BNMAX 0.0
#define  T32BNMIN 0.0
#define  T32BEMAX 0.0
#define  T32BEMIN 0.0

static double C_ttj [] = { 0.0, 0.0, 0.0 };		/* UTM50-32 to System34-Jylland  */
static double C_jtt [] = { 0.0, 0.0, 0.0 };		/* System34-Jylland to UTM50-32  */
static double C_tts [] = { 0.0, 0.0, 0.0 };		/* UTM50-32 to System34-Sjælland */
static double C_stt [] = { 0.0, 0.0, 0.0 };		/* System34-Sjælland to UTM50-32 */
static double C_ttb [] = { 0.0, 0.0, 0.0 };		/* 2001 UTM50-33 to System45-Bornholm */
static double C_btt [] = { 0.0, 0.0, 0.0 };		/* 2001 System45-Bornholm to UTM50-33 */

/******************************************************************************
*******************************************************************************
**                                                                           **
**               Pre 1999 Coefficients                                       **
**                                                                           **
** Should you have access to the polynominal coefficients that were used     **
** prior to the 1999 adjustment, replace the empty definitions provided      **
** below with those definitions.  Of course, that the names of the arrays    **
** will need to be those given below.                                        **
**                                                                           **
*******************************************************************************
******************************************************************************/

#define T33BNMAX 0.0
#define T33BNMIN 0.0
#define T33BEMAX 0.0
#define T33BEMIN 0.0

static double C_ttj98 [] = { 0.0, 0.0, 0.0 };		/* Pre 1999 - UTM50-32 to System34-Jylland  */
static double C_jtt98 [] = { 0.0, 0.0, 0.0 };		/* Pre 1999 - System34-Jylland to UTM50-32  */
static double C_tts98 [] = { 0.0, 0.0, 0.0 };		/* Pre 1999 - UTM50-32 to System34-Sjælland */
static double C_stt98 [] = { 0.0, 0.0, 0.0 };		/* Pre 1999 - System34-Sjælland to UTM50-32 */
static double C_ttb98 [] = { 0.0, 0.0, 0.0 };		/* Pre 1999 - UTM50-33 to System45-Bornholm */
static double C_btt98 [] = { 0.0, 0.0, 0.0 };		/* Pre 1999 - System45-Bornholm to UTM50-33 */

/******************************************************************************
*******************************************************************************
**                                                                           **
**               Bornholm 1999 Coefficients                                  **
**                                                                           **
** Should you have access to the polynominal coefficients of the 1999        **
** vintage that applied to Bornholm, replace the the empty definitions       **
** provided below with those definitions.  Of course, the names of the       **
** will need to be those given below or the compilation of the CS_sys34.c    **
** module will fail.                                                         **
**                                                                           **
** Note that at the time of this writing (July 2008) the 1999 vintage of the **
** coefficients for Jylland and Sjælland remain valid.  So, throughout the   **
** code, references to the SYSTM34-01 projection will refer to the same      **
** coefficient arrays for Jylland and Sjælland as the SYSTM34-99 projection. **
**                                                                           **
** In other words, the absence of arrays named C_ttj99, C_jtt99, C_tts99,    **
** C_stt99 is __NOT__ an error, but intentional.                             **
**                                                                           **
*******************************************************************************
******************************************************************************/

static double C_ttb99 [] = { 0.0, 0.0, 0.0 };		/* 1999 UTM50-33 to System45-Bornholm */
static double C_btt99 [] = { 0.0, 0.0, 0.0 };		/* 1999 System45-Bornholm to UTM50-33 */

