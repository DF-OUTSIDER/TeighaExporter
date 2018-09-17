//
// Copyright (c) 2000 by Tech Soft America, LLC.
// The information contained herein is confidential and proprietary to
// Tech Soft America, LLC., and considered a trade secret as defined under
// civil and criminal statutes.  Tech Soft America shall pursue its civil
// and criminal remedies in the event of unauthorized use or misappropriation
// of its trade secrets.  Use of this information by anyone other than
// authorized employees of Tech Soft America, LLC. is granted only under a
// written non-disclosure agreement, expressly prescribing the scope and
// manner of such use.
//
//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//
//
// $Header: //DWF/Working_Area/Willie.Zhu/w3dtk/BOpcodeHandler.h#1 $
//

#ifndef BOPCODE_HANDLER
#define BOPCODE_HANDLER

#ifndef BBINFILETK_TOOLKIT
    #include "dwf/Toolkit.h"
    #include "dwf/w3dtk/BStream.h"
    #include "dwf/w3dtk/BStreamFileToolkit.h"
    #include "dwf/w3dtk/W3DOpcodeHandler.h"
    #include "dwf/w3dtk/utility/vlist.h"
#else
    #define _W3DTK_API
#endif

#include "OdPlatformSettings.h"
#ifdef ODA_BIGENDIAN
#define STREAM_BIGENDIAN
#else
#undef STREAM_BIGENDIAN
#endif


#ifndef POINTER_SIZED_INT
#if defined(WIN64) || defined(_M_X64) || defined(_WIN64)
#   define POINTER_SIZED_INT __int64
#   define POINTER_SIZED_UINT unsigned __int64
#else
#   define POINTER_SIZED_INT long
#   define POINTER_SIZED_UINT unsigned long
#endif
#endif

//! Contains datatypes which are used by BBaseOpcodeHandler.
/*! \file BOpcodeHandler.h
*/

//! The BBaseOpcodeHandler abstract class is used as a base for derived classes which manage logical pieces of HOOPS Stream File information 
/*! 
	BBaseOpcodeHandler provides virtual functions which are implemented by derived classes to handle reading, writing, 
	execution and interpretation of HOOPS Stream File information.  Execution refers to the process of populating application-specific data 
	structures with the information that has been read.  Interpretation refers to the process of extracting 
	application-specific data to prepare it for subsequent writing.

	Each opcode handler manages opcodes which represent the various HOOPS/3dGS scene-graph objects which include segments,
	attributes, and geometric primitives.

	The HSF file reading process involves reading the opcode at the start of each piece of binary information,
	and calling the Read method of the associated opcode handler. After the opcode handler reports 
	that reading is complete, the Execute method should be called. 
	(When using HoopsStream or BaseStream, the ParseBuffer method of the BStreamFileToolkit object encapsulates this process.)

	The HSF file writing process involves determining the proper opcode for each piece of binary 
	information, and calling the Interpret method of the associated opcode handler.  After interpretation is complete,  
	the Write method of the opcode handler should be called until writing is complete.  
	(When using HoopsStream, the GenerateBuffer method of the HStreamFileToolkit encapsulates this process.  However, 
	if BaseStream is used, the Interpret/Write process must be performed manually.)
*/
class BBINFILETK_API2 BBaseOpcodeHandler : public BControlledMemoryObject
                                         , public W3DOpcodeHandler {
	protected:
		int             m_stage;        //!< The writing stage
		int             m_progress;     //!< Tracks the amount of data that has been read/written so far
		unsigned char   m_opcode;       //!< The opcode being handled by this particular object
   		bool			m_needs_tag;	//!< Indicate if this object explicitly needs tagging


		int             m_debug_length;   /*!< Length of debug string name */
		int             m_debug_allocated;/*!< Size of currently available frbug string buffer */
		char *          m_debug_string;   /*!< Debug String */

		char *			m_ascii_buffer;
		int				m_ascii_size;
		int				m_ascii_length;

		int				m_ascii_stage;
		int				m_ascii_progress;	//!< Tracks the amount of data that has been read/written so far by GetAscii functions
		unsigned char   m_byte;       //!< The opcode being handled by this particular object
		unsigned short  m_unsigned_short;
		int				m_int;
		char			m_char;
	public:
		/*! 
		   Default constructor for an BBaseOpcodeHandler object 
				   \param op value denoting the opcode that the object supports.  This is necessary for opcode handlers that 
				   support more than one opcode, such as TK_Polypoint, which supports both TKE_Polygon and TKE_Polyline opcodes.
		*/      
		BBaseOpcodeHandler (int op)
			: m_stage (0), m_progress (0), m_opcode ((unsigned char)op), m_needs_tag (false),
			  m_debug_length (0), m_debug_allocated (0), m_debug_string (0),

			  m_ascii_buffer (0), m_ascii_size (0), m_ascii_length (0), m_ascii_stage (0), m_ascii_progress(0),
			  m_byte(0), m_unsigned_short(0), m_int(0), m_char('\0')
			   {}
		virtual ~BBaseOpcodeHandler ();


        ///
        ///\copydoc W3DOpcodeHandler::serialize
        ///
        virtual void serialize( const void* pTag = NULL )
            throw( DWFException );

		/*!
		   Reads data from the toolkit buffer, decodes/decompresses it, and maps it to the opcode handlers data members.  
		   User-defined classes which need to write out custom data
		   should utilize one of the available GetData() methods.
		   \param tk A reference to the BStreamFileToolkit object.
		   \return The result of the function call.
		*/
		virtual TK_Status   Read (BStreamFileToolkit & tk) alter = 0;

		/*!
		   Encodes/compresses data and writes data to the toolkit buffer.  User-defined classes which need to write out custom data 
		   should utilize one of the available PutData() methods, and first write out the opcode 
		   associated with the group of binary data followed by the data itself.
		   \param tk A reference to the BStreamFileToolkit object.
		   \return The result of the function call.
		*/
		virtual TK_Status   Write (BStreamFileToolkit & tk) alter = 0;

		/*!
		   Processes the binary data that has been read for the current object, which involves passing the data to 
		   application-specific data structures.        

		   \param tk A reference to the BStreamFileToolkit object.
		   \return The result of the function call.
		*/
		virtual TK_Status   Execute (BStreamFileToolkit & tk) alter;

		/*!
		   Extracts graphics data (typically from the application's scene-graph) and maps it to the 
		   opcode handler's data members in preparation for writing out an object to the file.
      
		   \param tk A reference to the BStreamFileToolkit object.
		   \param key An identifier to the object (for example, the HOOPS key of a geometry item)
		   \param variant optional specification of an alternate form, such as a Level-of-Detail representation
		   \return The result of the function call.
		*/
		virtual TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant = 0) alter;

		/*!
		   Extracts graphics data (typically from the application's scene-graph) and maps it to the 
		   opcode handler's data members in preparation for writing out an object to the file.
       
		   \param tk A reference to the BStreamFileToolkit object.
		   \param key An identifier to the object (for example, the HOOPS key of a geometry item)
		   \param special A string value of particular data
		   \return The result of the function call.
		*/
		virtual TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter;

		/*! 
		   Resets the current opcode handler.  This is called by the toolkit when it is done processing 
		   an opcode.  This method reinitializes any opcode handler variables and frees up temporary 
		   data.
		*/
		virtual void        Reset (void) alter;
    
		/*!
			Determines whether a stored instance of an object is compatible with the current object,
			and if it is, prepares the object to write the instance instead.
		*/
		virtual bool        Match_Instance (BStreamFileToolkit const & tk, Recorded_Instance alter * instance) alter;


		/*! Returns the current opcode */
		unsigned char       Opcode (void) const { return m_opcode; }

		/*! 
			The toolkit makes multiple passes of objects which have LOD representations in the HOOPS database.  This 
			function returns the number of the pass which the toolkit is currently on.  
		*/
		int                 Pass (BStreamFileToolkit & tk) const { return tk.pass(); }

		/*!  
			 Adds a Tag to the file for the current object, and also adds an entry to the toolkit's tag table
			 which stores an association between the tag (also called an 'index') and the current object's HOOPS key
		*/
		TK_Status           Tag (BStreamFileToolkit & tk, int variant= -1) const   { return tk.tag(variant); }

		/*!  
			 Provides a simple interface to check if the toolkit is set for Force_Tags.
		*/
		bool                Tagging (BStreamFileToolkit & tk) const   { return  m_needs_tag || tk.GetWriteFlags(TK_Force_Tags) != 0; }
		/*!  
			 Sets whether the object requests tagging
		*/
		//void                SetNeedsTag (bool n) alter	{ m_needs_tag = n; }

		/*!  
			 Determine if the object is set to request tagging
		*/
		//bool                NeedsTag (void) const	{ return m_needs_tag; }

		/*!  
		Copies the opcode handler
		\param tk A reference to the BStreamFileToolkit object.
		\param handler A pointer to the opcode handler object. Passed by reference.
		\return The result of the function call.
		*/
		virtual TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const   { *handler = 0; return tk.Error(); }

		/*! 
		  Set the debug string.  Makes sure the buffer is big enough to hold 
		  thedebug string, and copies it in 
		*/
		void            SetDebug (char const * segment) alter;

		/*! 
		  Set the debug string.  Makes sure the buffer is big enough to hold 
		  a string of 'length' characters 
		*/
		void            SetDebug (int length) alter;

		/*! 
		  Get the debug string.  Returns a pointer to the current debug string
		*/
		char const *    GetDebugString (void) const                           { return m_debug_string; }
		/*! 
		  Get the debug string buffer.  Returns a pointer to the current debug string buffer
		  which may then be modified directly
		*/
		char alter *    GetDebugString (void) alter               { return m_debug_string; }

		/*! 
		  write out any debug string (provided or previously recorded) and reset
		*/
		void            LogDebug (BStreamFileToolkit & tk, char const * string = 0) alter;

	protected:
		// various means of pulling data from the toolkit buffer
		// Note: format conversion is safe to do in output buffer

		/*! Gets n characters from the toolkit buffer and writes them into buffer b; returns result of function call. */
		TK_Status   GetData (BStreamFileToolkit & tk, char * b, int n) alter    { return tk.read (b, n); }

		/*! Gets n short integers from the toolkit buffer and writes them into buffer s; returns result of function call. */
		TK_Status   GetData (BStreamFileToolkit & tk, short * s, int n) alter   {
						TK_Status   status;
						if ((status = GetData (tk, (char *)s, n * (int)sizeof (short))) == TK_Normal)
							fix (s, n);
						return status;
					}

		/*! Gets n integers from the toolkit buffer and writes them into buffer i; returns result of function call. */
		TK_Status   GetData (BStreamFileToolkit & tk, int * i,   int n) alter   {
						TK_Status   status;
						if ((status = GetData (tk, (char *)i, n * (int)sizeof (int))) == TK_Normal)
							fix (i, n);
						return status;
					}

		/*! Gets n floats from the toolkit buffer and writes them into buffer f; returns result of function call.  */   
		TK_Status   GetData (BStreamFileToolkit & tk, float * f, int n) alter   {
						TK_Status   status;
						if ((status = GetData (tk, (char *)f, n * (int)sizeof (float))) == TK_Normal)
							fix_in (f, n);
						return status;
					}

		/*! Gets n unsigned characters from the toolkit buffer and writes them into buffer b; returns result of function call.*/
		TK_Status   GetData (BStreamFileToolkit & tk, unsigned char * b, int n) alter   { return GetData (tk, (char *)b,  n); }

		/*! Gets n unsigned shorts from the toolkit buffer and writes them into buffer s; returns result of function call. */    
		TK_Status   GetData (BStreamFileToolkit & tk, unsigned short * s, int n) alter  { return GetData (tk, (short *)s, n); }

		/*! Gets n unsigned integers from the toolkit buffer and writes them into buffer i; returns result of function call.*/
		TK_Status   GetData (BStreamFileToolkit & tk, unsigned int * i, int n) alter    { return GetData (tk, (int *)i,   n); } 

		/*! Gets a single characters from the toolkit buffer; returns result of function call.*/
		TK_Status   GetData (BStreamFileToolkit & tk, char & c) alter                   { return GetData (tk, &c, 1); }

		/*! Gets a single short integer from the toolkit buffer; returns result of function call.*/
		TK_Status   GetData (BStreamFileToolkit & tk, short & s) alter                  { return GetData (tk, &s, 1); }

		/*! Gets a single integer from the toolkit buffer; returns result of function call.*/
		TK_Status   GetData (BStreamFileToolkit & tk, int & i) alter                    { return GetData (tk, &i, 1); }

		/*! Gets a single unsigned character from the toolkit buffer; returns result of function call.*/
		TK_Status   GetData (BStreamFileToolkit & tk, unsigned char & b) alter          { return GetData (tk, &b, 1); }

		/*! Gets a single unsigned short integer from the toolkit buffer; returns result of function call. */
		TK_Status   GetData (BStreamFileToolkit & tk, unsigned short & s) alter         { return GetData (tk, &s, 1); }

		/*! Gets a single unsigned integer from the toolkit buffer; returns result of function call.*/
		TK_Status   GetData (BStreamFileToolkit & tk, unsigned int & i) alter           { return GetData (tk, &i, 1); }

		/*! Gets a single float from the toolkit buffer; returns result of function call. */
		TK_Status   GetData (BStreamFileToolkit & tk, float & f) alter                  { return GetData (tk, &f, 1); }

		TK_Status   SkipNewlineAndTabs(BStreamFileToolkit & tk, unsigned int* readSize=0) alter;
		TK_Status   ReadAsciiLine(BStreamFileToolkit & tk, unsigned int* readSize=0) alter;
		TK_Status	ReadAsciiWord(BStreamFileToolkit & tk, unsigned int* readSize=0) alter;
		TK_Status   ReadEndOpcode(BStreamFileToolkit & tk) alter;
		bool		RemoveAngularBrackets(char* string) alter;
		bool		RemoveQuotes(char* string) alter;
		TK_Status   Read_Referenced_Segment(BStreamFileToolkit & tk, int &i_progress) alter;

		//TK_Status   GetAsciiData(BStreamFileToolkit & tk, float * rFloats,	unsigned int n) alter;
		TK_Status   GetAsciiData(BStreamFileToolkit & tk, int * rInts,		unsigned int n) alter;
		//TK_Status   GetAsciiData(BStreamFileToolkit & tk, short * rShorts,	unsigned int n) alter;

		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag, unsigned char& value)alter;
		TK_Status	GetAsciiData(BStreamFileToolkit & tk, const char * tag, char& value)alter;
		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag, unsigned short& value)alter;
		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag, short& value)alter;
		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag, int& value)alter;
		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag, float& value)alter;

		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag, float * rFloats, unsigned int n) alter;
		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag, char * m_string, unsigned int n) alter;
		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag, unsigned char * m_string, unsigned int n) alter;
		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag, int * rInts,     unsigned int n) alter;
		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag, short * rShorts, unsigned int n) alter;
		TK_Status   GetAsciiData(BStreamFileToolkit & tk, const char * tag,	unsigned short * rShorts, unsigned int n) alter;
		TK_Status   GetAsciiHex(BStreamFileToolkit & tk,  const char * tag, unsigned char &value) alter;
		TK_Status   GetAsciiHex(BStreamFileToolkit & tk,  const char * tag, int &value) alter;
		TK_Status   GetAsciiHex(BStreamFileToolkit & tk,  const char * tag, char &value) alter;
		TK_Status   GetAsciiHex(BStreamFileToolkit & tk,  const char * tag, unsigned short &value) alter;
		TK_Status   GetAsciiImageData(BStreamFileToolkit & tk, const char * tag, unsigned char * rValues, unsigned int n) alter;

		/*! Examines a single unsigned character from the toolkit buffer (does not Get); returns result of function call.*/
		TK_Status   LookatData (BStreamFileToolkit & tk, unsigned char & b) alter       { return tk.lookat ((char &)b); }

		// various means of putting data into the toolkit buffer
		// Note: format conversion is NOT safe in input buffer -- use temps

		/*! Puts n characters from buffer b into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, char const * b, int n) alter    { return tk.write (b, n); }

		/*! Puts n short integers from buffer s into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, short const * s, int n) alter    {
						#ifdef STREAM_BIGENDIAN
							short *     buffer = new short [n];
							short *     tmp = buffer;
							TK_Status   status;
							int         i;
							for (i=0; i<n; ++i)
								*tmp++ = flip (*s++);
							status = PutData (tk, (char const *)buffer, n * (int)sizeof (short));
							delete [] buffer;
							if (status != TK_Normal)
								return status;
							return TK_Normal;
						#else
							return PutData (tk, (char const *)s, n * (int)sizeof (short));
						#endif
					}

		/*! Puts n integers from buffer i into the toolkit buffer; returns result of function call.  */      
		TK_Status   PutData (BStreamFileToolkit & tk, int const * i, int n) alter    {
						#ifdef STREAM_BIGENDIAN
							int *       buffer = new int [n];
							int *       tmp = buffer;
							TK_Status   status;
							int         j;
							for (j=0; j<n; ++j)
								*tmp++ = flip (*i++);
							status = PutData (tk, (char const *)buffer, n * (int)sizeof (int));
							delete [] buffer;
							if (status != TK_Normal)
								return status;
							return TK_Normal;
						#else
							return PutData (tk, (char const *)i, n * (int)sizeof (int));
						#endif
					}

		/*! Puts n floats from buffer f into the toolkit buffer; returns result of function call.*/             
		TK_Status   PutData (BStreamFileToolkit & tk, float const * f, int n) alter    {
						#if defined(NON_IEEE) || defined(STREAM_BIGENDIAN)
							float *     buffer = new float [n];
							float *     tmp = buffer;
							TK_Status   status;
							int         i;
							for (i=0; i<n; ++i) {
								*tmp = *f++;
								fix_out (tmp++, 1);
							}
							status = PutData (tk, (char const *)buffer, n * (int)sizeof (float));
							delete [] buffer;
							if (status != TK_Normal)
								return status;
							return TK_Normal;
						#else
							return PutData (tk, (char const *)f, n * (int)sizeof (float));
						#endif
					}

		/*! Puts n unsigned characters from buffer b into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, unsigned char const * b, int n) alter    { return PutData (tk, (char const *)b,  n); }

		/*! Puts n unsigned short integers from buffer s into the toolkit buffer; returns result of function call.      */
		TK_Status   PutData (BStreamFileToolkit & tk, unsigned short const * s, int n) alter   { return PutData (tk, (short const *)s, n); }
			
		/*! Puts n unsigned integers from buffer i into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, unsigned int const * i, int n) alter     { return PutData (tk, (int const *)i,   n); }

		/*! Puts a single character c into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, char const & c) alter                    { return PutData (tk, &c, 1); }

		/*! Puts a single short integer s into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, short const & s) alter                   { return PutData (tk, &s, 1); }

		/*! Puts a single integer i into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, int const & i) alter                     { return PutData (tk, &i, 1); }

		/*! Puts a single unsigned character b into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, unsigned char const & b) alter           { return PutData (tk, &b, 1); }

		/*! Puts a single unsigned short s into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, unsigned short const & s) alter          { return PutData (tk, &s, 1); }

		/*! Puts a single unsigned integer i into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, unsigned int const & i) alter            { return PutData (tk, &i, 1); }
		
		/*! Puts a single float f into the toolkit buffer; returns result of function call.*/
		TK_Status   PutData (BStreamFileToolkit & tk, float const & f) alter                   { return PutData (tk, &f, 1); }

		/*! Puts a the object's opcode into the toolkit buffer; returns result of function call.*/
		TK_Status   PutOpcode (BStreamFileToolkit & tk, int adjust = 1) alter {
						TK_Status       status;
						unsigned int    sequence;

						if ((status = PutData (tk, Opcode ())) != TK_Normal)
							return status;

						tk.adjust_written (adjust);

						sequence = tk.NextOpcodeSequence();
						if (tk.GetLogging())
							log_opcode (tk, sequence, Opcode());

						return status;
					}


		/*! Puts a the object's ASCII opcode into the toolkit buffer; returns result of function call.*/
		TK_Status   PutAsciiOpcode (BStreamFileToolkit & tk, int adjust = 1, bool is_end = false, bool want_newline = true) alter;
	//	TK_Status   PutAsciiOpcode (BStreamFileToolkit & tk, int adjust = 1) alter;

		/*! Puts n characters from buffer b into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, char const * b, int n) alter;

		/*! Puts n short integers from buffer s into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, short const * s, int n) alter;

		/*! Puts n integers from buffer i into the toolkit buffer as ASCII; returns result of function call.  */      
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, int const * i, int n) alter;

		/*! Puts n floats from buffer f into the toolkit buffer as ASCII; returns result of function call.*/             
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, float const * f, int n) alter;

		/*! Puts n unsigned characters from buffer b into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, unsigned char const * b, int n) alter;

		/*! Puts n unsigned short integers from buffer s into the toolkit buffer as ASCII; returns result of function call.      */
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, unsigned short const * s, int n) alter;

		/*! Puts n unsigned integers from buffer i into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, unsigned int const * i, int n) alter;

		/*! Puts a single character c into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, char const & c) alter;

		/*! Puts a single short integer s into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, short const & s) alter;

		/*! Puts a single integer i into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, int const & i) alter;

		/*! Puts a single integer i into the toolkit buffer as ASCII Hex; returns result of function call.*/
		TK_Status   PutAsciiFlag (BStreamFileToolkit & tk, char const *tag, int const & i) alter;

		/*! Puts a single unsigned character b into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, unsigned char const & b) alter;

		/*! Puts a single unsigned short s into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, unsigned short const & s) alter;

		/*! Puts a single unsigned integer i into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, unsigned int const & i) alter;

		/*! Puts a single float f into the toolkit buffer as ASCII; returns result of function call.*/
		TK_Status   PutAsciiData (BStreamFileToolkit & tk, char const *tag, float const & f) alter;

		TK_Status   PutAsciiMask (BStreamFileToolkit & tk,char const *tag, int const & i) alter;

		TK_Status   PutAsciiHex (BStreamFileToolkit & tk, char const *tag, int const & i) alter;

		TK_Status   PutStartXMLTag (BStreamFileToolkit & tk, char const *tag) alter ;

		TK_Status   PutEndXMLTag (BStreamFileToolkit & tk, char const *tag) alter ;


		/* note -- fix for int types will work during read OR write phase, but floats need separate
		   routines for native->IEEE and IEEE->native
		*/
		//! for internal use only
		short       flip (short s) {
						return (short)(((s >> 8) & 0x00FF) | (s << 8));
					}
		//! for internal use only
		int         flip (int i) {
						return ((i >> 24) & 0x000000FF) | ((i >> 8) & 0x0000FF00) |
							   ((i <<  8) & 0x00FF0000) |  (i << 24);
					}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  #ifndef UNREFERENCED
	#define UNREFERENCED(x) (void)(x)
  #endif
#endif 

		//! for internal use only
		void        fix (int * i, int n) {
						#ifdef STREAM_BIGENDIAN
						while (n--){
								*i = flip (*i);
								i++;
						}
						#else
						UNREFERENCED(i);
						UNREFERENCED(n);
						#endif
					}
		//! for internal use only
		void        fix (short * s, int n) {
						#ifdef STREAM_BIGENDIAN
							while (n--){
								*s = flip (*s);
								s++;
								}
						#else
						UNREFERENCED(s);
						UNREFERENCED(n);
						#endif
					}

		//! for internal use only
		void        fix_in (float * f, int n) {
						#ifdef NON_IEEE
							// need to re-interpret from IEEE to native format
						#endif

						#ifdef STREAM_BIGENDIAN
						int * i = (int*) f;
						while (n--){
								*i = flip (*i);
                            
								i++;
						}
						#else
						UNREFERENCED(f);
						UNREFERENCED(n);
						#endif
					}
		//! for internal use only
		void        fix_out (float * f, int n) {
						#ifdef NON_IEEE
							// need to re-interpret from native format to IEEE
						#endif

						#ifdef STREAM_BIGENDIAN
						int * i = (int*) f;
						while (n--){
								*i = flip (*i);
								i++;
						}
						#else
						UNREFERENCED(f);
						UNREFERENCED(n);
						#endif
					}

		//! for internal use only
		void        log_opcode (BStreamFileToolkit & tk, unsigned int sequence, unsigned char opcode) alter;


		/* common conversions
		   these two are for converting between floats [0.0,1.0] and unsigned chars [0,255]
		*/
		//! for internal use only
		void        floats_to_bytes (float const * in, unsigned char alter * out, int count) const {
						while (count-- > 0)
							*out++ = char (*in++ * 255.999f);
					}
		//! for internal use only
		void        bytes_to_floats (unsigned char const * in, float alter * out, int count) const {
						while (count-- > 0)
							*out++ = float (*in++) * (1.0f/255.0f);
					}

		// access to toolkit utility functions
		//! for internal use only
		void        add_segment (BStreamFileToolkit & tk, ID_Key key) alter     { tk.add_segment (key); }
		//! for internal use only
		ID_Key      remove_segment (BStreamFileToolkit & tk) alter              { return tk.remove_segment(); }
		//! sets the given key as "most recent" on the toolkit for the purposes of associating keys with indices 
		void        set_last_key (BStreamFileToolkit & tk, ID_Key key) alter    { tk.set_last_key (key); }
		//! obsolete
		ID_Key      last_key (BStreamFileToolkit & tk) const {
						if (tk.m_last_keys_used == 1) 
							return tk.m_last_keys[0];
						else 
							return -1; 
					} 
		//! for internal use only
		void        adjust_written (BStreamFileToolkit & tk, int count) alter   { tk.adjust_written (count); }
		//! for internal use only
		void        increase_nesting (BStreamFileToolkit & tk, int amount=1) alter  { tk.increase_nesting (amount); }
		//! for internal use only
		void        decrease_nesting (BStreamFileToolkit & tk, int amount=1) alter  { tk.decrease_nesting (amount); }

		/*!  
			 Indicates that an object requires further processing during a subsequent pass
		*/
		void        Revisit (BStreamFileToolkit & tk, float priority=0.0f, int variant=0) const  { tk.revisit (Opcode(), priority, variant); }

		/*!  
			 Allows an object to access handlers defined for other objects
		*/
		BBaseOpcodeHandler alter *  Opcode_Handler (BStreamFileToolkit & tk, unsigned char op) const
										{ return tk.opcode_handler (op); }

		/*! Allows us to later find objects which are a [transformed] copy of previous objects */
		void        Record_Instance (BStreamFileToolkit & tk, ID_Key key, int variant,
									 int val1, int val2, int val3) const {
						tk.record_instance (key, variant, this, val1, val2, val3);
					}
		/*! Searches for objects which are a [transformed] copy of previous objects */
		bool        Find_Instance (BStreamFileToolkit & tk, int val1, int val2, int val3) alter {
						return tk.find_instance (this, val1, val2, val3);
					}

		/*! Allows us to remember which segments have been processed (avoid multiple writes of inclusions) */
		void        Remember_Item (BStreamFileToolkit & tk, ID_Key key) const  { tk.remember_item(key); }
		/*! Determines if a segment has already been processed (avoid multiple writes of inclusions) */
		bool        Find_Item (BStreamFileToolkit & tk, ID_Key key) const      { return tk.find_item(key); }

		/*! sanity checking.  for internal use only */
		bool        validate_count (int count, int limit = 1<<24) const { return 0 <= count && count <= limit; }

	/*! Internal use: utility functions for conversion between float & string without dealing with
		locale issues
	*/
	static float read_float (char const *cp, char const * alter * newcpp = 0);
		//! for internal use only
	static float read_float (char const *cp, char alter * alter * newcpp)
					{ return read_float (cp, (char const **)newcpp); }
		//! for internal use only
	static char * write_float (char * buffer, double f);
};

//! macro to simplify normal implementation of the Clone function
#define IMPLEMENT_CLONE(class_name)																			\
			TK_Status class_name::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const {	\
				*newhandler = new class_name;																\
				if (*newhandler != null)																	\
					return TK_Normal;																		\
				else																						\
					return tk.Error ("memory allocation in" #class_name "::clone failed");					\
			}																							   //
//! a version of IMPLEMENT_CLONE for the classes that need to know the opcode
#define IMPLEMENT_CLONE_OPCODE(class_name)																	\
			TK_Status class_name::Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **newhandler) const {	\
				*newhandler = new class_name (Opcode());													\
				if (*newhandler != null)																	\
					return TK_Normal;																		\
				else																						\
					return tk.Error ("memory allocation in" #class_name "::clone failed");					\
}																										   //



//! Opcodes stored in the file.
/*! \enum TKE_Object_Types
	Each piece of logical information in the HOOPS Stream File is represented by an opcode.
		The opcode along with its data constitutes an 'opcode object'

  Note: the trailing ////s have been used internally as a visual indicator of the completeness
  of the implementation of an opcode (gaps in the pattern would indicate work remaining somewhere).
  Usually // for a reserved code, /// for partial implementation, //// when done.
*/
// Additions need to be reflected in the 'opcode_string' table in BOpcodeHandler.cpp
enum TKE_Object_Types {
	TKE_Termination					= '\x00',		//!< ////
	TKE_Pause						= '\x01',		//!< ////
													/////////
	TKE_Comment						= ';',			//!< ////
													/////////
	TKE_Font						= 'f',			//!< ////
	TKE_Texture						= 't',			//!< ////
													/////////
	TKE_Open_Segment				= '(',			//!< ////
	TKE_Close_Segment				= ')',			//!< ////
	TKE_Reopen_Segment				= 's',			//!< ////
	TKE_Include_Segment				= '<',			//!< ////
	TKE_Style_Segment				= '{',			//!< ////
													/////////
	TKE_Geometry_Attributes			= ':',			//!< ////
	TKE_Renumber_Key_Global			= 'K',			//!< ////
	TKE_Renumber_Key_Local			= 'k',			//!< ////
	TKE_Priority					= '0',			//!< ////
	TKE_Tag							= 'q',			//!< ////
													/////////
	TKE_Bounding					= 'b',			//!< ////
	TKE_Bounding_Info				= 'B',			//!< ////
	TKE_Callback					= '\x07',		//!< ////
	TKE_Camera						= '>',			//!< ////
	TKE_Conditions					= '?',			//!< ////
	TKE_Color						= '"',			//!< ////
	TKE_Color_By_Index				= '\x08',		//!< ////
	TKE_Color_By_Index_16			= '\x09',		//!< ////
	TKE_Color_By_FIndex				= '\x0A',		//!< ////
	TKE_Color_RGB					= '~',			//!< ////
	TKE_Color_By_Value				= '\x0B',		//!< ////
	TKE_Color_Map					= '\x0C',		//!< ////
	TKE_Edge_Pattern				= '\x0D',		//!< ////
	TKE_Edge_Weight					= '\x0E',		//!< ////
	TKE_Face_Pattern				= 'P',			//!< ////
	TKE_Handedness					= 'h',			//!< ////
	TKE_Heuristics					= 'H',			//!< ////
	TKE_Line_Pattern				= '-',			//!< ////
	TKE_Line_Weight					= '=',			//!< ////
	TKE_Marker_Size					= '+',			//!< ////
	TKE_Marker_Symbol				= '@',			//!< ////
	TKE_Modelling_Matrix			= '%',			//!< ////
	TKE_LOD							= '\x19',		//!< ////
	TKE_Rendering_Options			= 'R',			//!< ////
	TKE_Selectability				= '!',			//!< ////
	TKE_Text_Alignment				= '*',			//!< ////
	TKE_Text_Font					= 'F',			//!< ////
	TKE_Text_Path					= '|',			//!< ////
	TKE_Text_Spacing				= ' ',			//!< ////
	TKE_Texture_Matrix				= '$',			//!< ////
	TKE_Unicode_Options				= '\x16',		//!< ////
	TKE_User_Index					= 'n',			//!< ////
	TKE_User_Options				= 'U',			//!< ////
	TKE_User_Value					= 'v',			//!< ////
	TKE_Visibility					= 'V',			//!< ////
	TKE_Window						= 'W',			//!< ////
	TKE_Window_Frame				= '#',			//!< ////
	TKE_Window_Pattern				= 'p',			//!< ////
	TKE_Glyph_Definition			= 'j',			//!< ////
	TKE_Line_Style					= 'J',			//!< ////
													/////////
	TKE_Infinite_Line				= '`',		//!< ///
	TKE_Area_Light					= 'a',			//!< ////
	TKE_Circle						= 'C',			//!< ////
	TKE_Circular_Arc				= 'c',			//!< ////
	TKE_Circular_Chord				= '\\',			//!< ////
	TKE_Circular_Wedge				= 'w',			//!< ////
	TKE_Cutting_Plane				= '/',			//!< ////
	TKE_Cylinder					= 'Y',			//!< ////
	TKE_Distant_Light				= 'd',			//!< ////
	TKE_Ellipse						= 'E',			//!< ////
	TKE_Elliptical_Arc				= 'e',			//!< ////
	TKE_Grid						= 'g',			//!< ////
	TKE_Image						= 'i',			//!< ////
	TKE_Line						= 'l',			//!< ////
	TKE_Local_Light					= '.',			//!< ////
	TKE_Marker						= 'X',			//!< ////
	TKE_Mesh						= 'M',			//!< ////
	TKE_NURBS_Curve					= 'N',			//!< ////
	TKE_NURBS_Surface				= 'A',			//!< ////
	TKE_PolyCylinder				= 'Q',			//!< ////
	TKE_Polygon						= 'G',			//!< ////
	TKE_Polyline					= 'L',			//!< ////
	TKE_PolyPolyline				= '\x10',		//!< ////
	TKE_Reference					= 'r',			//!< //
	TKE_Shell						= 'S',			//!< ////
	TKE_Sphere						= '\x1a',		//!< ////
	TKE_Spot_Light					= '^',			//!< ////
	TKE_Text						= 'T',			//!< ////
	TKE_Text_With_Encoding			= 'x',			//!< ////
													/////////
	TKE_Start_User_Data				= '[',			//!< ////
	TKE_Stop_User_Data				= ']',			//!< ////
	TKE_XML							= '\x18',		//!< ////
	TKE_External_Reference			= '\x12',		//!< ////
	TKE_URL							= '\x15',		//!< ////
													/////////
	TKE_Start_Compression			= 'Z',			//!< ////
	TKE_Stop_Compression			= 'z',			//!< ////
													/////////
	TKE_Repeat_Object				= '&',			//!< ////
	TKE_View						= '}',			//!< ////
	TKE_Clip_Rectangle				= 'o',			//!< ////
	TKE_Clip_Region					= 'O',			//!< ////
	TKE_Complex_Clip_Region			= '\x0F',		//!< //
													/////////
	TKE_File_Info					= 'I',			//!< ////
	TKE_Dictionary					= 'D',			//!< ////
	TKE_Dictionary_Locater			= '_',			//!< ////
	TKE_Thumbnail					= '\x14',		//!< ////
	TKE_Delete_Object				= '\x7F',		//!< ////
													/////////
	TKE_Tag_Implicit				= TKE_Tag,		//!< //-- obsolete.  this alias provided for source compatibility with old code
	TKE_Streaming_Mode				= ',',			//!< //-- obsolete.  probably no files ever generated with this

	TKE_First_User_Opcode			= '\xA0',		//!< //// first opcode value reserved for private use
    
    //
    //
    // W3D Extensions
    //
    //

    TKE_HW3D_Image                  = 0xE0,

    //
    //
    //


	TKE_Last_User_Opcode			= '\xEF',		//!< //// last opcode value reserved for private use
													/////////
	TKE_Pseudo_Handler				= '\xFE',		//!< //// pseudo-handler (non-zero value)
	TKE_Extended_Code				= '\xFF',		//!< //// reserved for future expansion

	TKE_Close_Geometry_Attributes	= 0x39          //!< //// for ascii mode to close geometry attribut
};


////////////////////////////////////////////////////////////////////////////////


/*! 
	TK_Default provides a simple dispatcher used when reading/parsing HSF data.
	Its only purpose is to read a single opcode value and call out to the appropriate
	handler.
*/
class BBINFILETK_API TK_Default : public BBaseOpcodeHandler {
	
	protected:
		char		*m_opcode_buffer;
		int          m_buffer_count;

	public:
		/*! constructor */
		TK_Default () : BBaseOpcodeHandler (TKE_Pseudo_Handler) {m_opcode_buffer = 0, m_buffer_count = 0;}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		
		TK_Status   Write (BStreamFileToolkit & tk) alter;


		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

    private:
        TK_Default(const TK_Default&);
        TK_Default& operator=(const TK_Default&);
};

/*! 
	TK_Unavailable is a placeholder handler for undefined opcodes. It will generate
	an error return if used, primarily as a means to catch data corruption or flag errors
	due to incorrect behaviour of other handlers.
*/
class BBINFILETK_API TK_Unavailable : public BBaseOpcodeHandler {
	public:
		/*! constructor */
		TK_Unavailable (char opcode) : BBaseOpcodeHandler (opcode) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;

private:
    TK_Unavailable(const TK_Unavailable&);
    TK_Unavailable& operator=(const TK_Unavailable&);
};

////////////////////////////////////////////////////////////////////////////////
//! Utility class for managing HSF header information
/*! 
	TK_Header does not explicitly handle any opcodes, but rather serves as a utility
	class for writing header information to an HSF file.  It will dynamically create
	TK_Comment and TK_File_Info opcode handlers (which in turn export TKE_Comment
	and TKE_File_Info opcode objects)
*/
class BBINFILETK_API TK_Header : public BBaseOpcodeHandler {
	protected:
		//! internal use
		BBaseOpcodeHandler *    m_current_object;

	public:
		/*! constructor */
		TK_Header () : BBaseOpcodeHandler (TKE_Pseudo_Handler), m_current_object (0) {}
		~TK_Header();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;


		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;


		void        Reset (void) alter;

private:
    TK_Header(const TK_Header&);
    TK_Header& operator=(const TK_Header&);
};


//! Handles the TKE_File_Info opcode.
/*!  
	TK_File_Info provides support for writing/reading the TKE_File_Info opcode object to/from
	an HSF file.  

	The TKE_File_Info opcode object contains information about how the file was written.
*/
class BBINFILETK_API TK_File_Info : public BBaseOpcodeHandler {
	protected:
		//! internal use
		int             m_flags;

	public:
		/*! constructor */
		TK_File_Info () : BBaseOpcodeHandler (TKE_File_Info), m_flags (0) {}

		
		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Execute (BStreamFileToolkit & tk) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
								{ return BBaseOpcodeHandler::Interpret(tk, key, special); }


		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;


		/*! Sets the flag indicating whether the reference segment should be processed itself */
		void            SetFlags (int f) alter                  { m_flags = f;      }
		/*! Returns the flag indicating whether the reference segment should be processed itself */
		int             GetFlags (void) alter                   { return m_flags;   }

private:
    TK_File_Info(const TK_File_Info&);
    TK_File_Info& operator=(const TK_File_Info&);
};


//! Handles the TKE_Comment opcode.
/*! 
	TK_Comment provides support for writing/reading the TK_Comment opcode object to/from
	an HSF file.  Take care when overloading this one, since the first comment in the file 
	contains important information about the file (such file version and the flags that 
	were used at the time of file write).

	The TKE_Comment opcode object contains a comment for the file. 
*/
class BBINFILETK_API TK_Comment : public BBaseOpcodeHandler {
    private:
        TK_Comment(const TK_Comment&);
        TK_Comment& operator=(const TK_Comment&);

	protected:
		//! internal use
		int             m_length;
		//! internal use
		char *          m_comment;
		
		//! internal use
		void    set_comment (char const * comment) alter;
		//! internal use
		void    set_comment (int length) alter;

	public:
		/*! constructor */
		TK_Comment (char const * comment = 0);
		~TK_Comment();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Execute (BStreamFileToolkit & tk) alter;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		TK_Status   ExecuteAscii (BStreamFileToolkit & tk) alter;
		/*! The only real reason we need an Interpret function here is so that we can register 
			this class as a PreWalk or PostWalk handler */
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant) {
						(void)tk; (void)key; (void)variant;
						return TK_Normal;
					}
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
								{ return BBaseOpcodeHandler::Interpret(tk, key, special); }
		void        Reset (void) alter;

		/*! 
		  Set the comment string.  Makes sure the buffer is big enough to hold 
		  the string, and copies it in 
		*/
		void            SetComment (char const * comment) alter { set_comment (comment); }
		/*! 
		  Set the comment.  Makes sure the buffer is big enough to hold a string
		  of 'length' characters
		*/
		void            SetComment (int length) alter           { set_comment (length);  }
		/*! 
		  Get the comment string.  Returns a pointer to the current comment
		*/
		char const *    GetComment (void) const                 { return m_comment; }
		/*! 
		  Get the comment string buffer.  Returns a pointer to the current comment
		  buffer, which may then be modified directly
		*/
		char alter *    GetComment (void) alter                 { return m_comment; }
};


//! Handles the TKE_Termination and TKE_Pause opcodes.
/*! 
	TK_Terminator provides support for writing/reading the TKE_Termination and TKE_Pause 
	opcode object to/from an HSF file.  

	The TKE_Termination opcode object denotes the end of the HSF file.  The TKE_Pause opcode
	is used to signal the end of the main scene-graph objects, which might then be followed
	by LOD representations of objects. 
*/
class BBINFILETK_API TK_Terminator : public BBaseOpcodeHandler {
    private:
        TK_Terminator(const TK_Terminator&);
        TK_Terminator& operator=(const TK_Terminator&);

	public:
		/*! constructor */
		TK_Terminator (char opcode, bool is_file_terminator = true) : BBaseOpcodeHandler (opcode), 
																	m_terminate_file(is_file_terminator) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Execute (BStreamFileToolkit & tk) alter;


		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

	protected:
		//! internal use for hsx read-write only. This indicates if the TKE_Terminate is 
		// meant to terminate the file or something else (viz. LOD collection)
		bool		m_terminate_file;
};


//! Handles the TKE_Start_Compression and TKE_Stop_Compression opcodes.
/*! 
	TK_Compression provides support for writing/reading the TKE_Start_Compression and TKE_Stop_Compression 
	opcode object to/from an HSF file.  

	The TKE_Start_Compression and TKE_Stop_Compression opcode objects denote the beginning and end 
	of global file compression.  
*/
class BBINFILETK_API TK_Compression : public BBaseOpcodeHandler {
    private:
        TK_Compression(const TK_Compression&);
        TK_Compression& operator=(const TK_Compression&);

	public:
		/*! constructor */
		TK_Compression (char opcode) : BBaseOpcodeHandler (opcode) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		TK_Status   Execute (BStreamFileToolkit & tk) alter;
		TK_Status   ExecuteAscii (BStreamFileToolkit & tk) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
								{ return BBaseOpcodeHandler::Interpret(tk, key, special); }
};

////////////////////////////////////////////////////////////////////////////////

//! Handles the TKE_Open_Segment opcode.
/*! 
	TK_Open_Segment provides support for writing/reading the TKE_Open_Segment opcode object to/from
	an HSF file.  
    
	The nodes in the HOOPS/3dGS scene-graph are called 'segments' which store, geometry, attributes, and 
	subsegments.  After a HOOPS segment is open, geometry which should reside in that segment should be exported.
	Similarly, any attributes which need to affect the geometry in that segment should be exported.

	This should be matched with a TKE_Close_Segment opcode object.
*/
class BBINFILETK_API TK_Open_Segment : public BBaseOpcodeHandler {
    private:
        TK_Open_Segment(const TK_Open_Segment&);
        TK_Open_Segment& operator=(const TK_Open_Segment&);

	protected:
		int             m_length;   /*!< Length of segment string name */
		int             m_allocated;/*!< Size of currently available string buffer */
		char *          m_string;   /*!< Segment name */

		//! internal use
		void    set_segment (char const * segment) alter;
		//! internal use
		void    set_segment (int length) alter;

	public:
		/*! constructor */
		TK_Open_Segment () : BBaseOpcodeHandler (TKE_Open_Segment), m_length (0), m_allocated (0), m_string (0) {}
		~TK_Open_Segment();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		void        Reset (void) alter;

		/*! 
		  Set the segment name.  Makes sure the buffer is big enough to hold 
		  the segment name, and copies it in 
		*/
		void            SetSegment (char const * segment) alter { set_segment (segment); }

		/*! 
		  Set the segment name.  Makes sure the buffer is big enough to hold 
		  a string of 'length' characters 
		*/
		void            SetSegment (int length) alter           { set_segment (length); }

		/*! 
		  Get the segment name.  Returns a pointer to the current segment name
		*/
		char const *    GetSegment (void) const                 { return m_string; }
		/*! 
		  Get the segment name buffer.  Returns a pointer to the current segment name buffer
		  which may then be modified directly
		*/
		char alter *    GetSegment (void) alter                 { return m_string; }
};


//! Handles the TKE_Close_Segment opcode.
/*! 
	TK_Open_Segment provides support for writing/reading the TKE_Close_Segment opcode object to/from
	an HSF file.   

	The HOOPS/3dGS scene-graph nodes are called 'segments' which store, geometry, attributes, and 
	subsegments.

	This should be matched with a previous TKE_Open_Segment opcode object.
*/
class BBINFILETK_API TK_Close_Segment : public BBaseOpcodeHandler {
    private:
        TK_Close_Segment(const TK_Close_Segment&);
        TK_Close_Segment& operator=(const TK_Close_Segment&);

	public:
		/*! constructor */
		TK_Close_Segment () : BBaseOpcodeHandler (TKE_Close_Segment) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
};



//! Handles the TKE_Reopen_Segment opcode.
/*! 
	TKE_Reopen_Segment provides support for writing/reading the TKE_Close_Segment opcode object to/from
	an HSF file.  

	The HOOPS/3dGS scene-graph nodes are called 'segments' which store, geometry, attributes, and 
	subsegments.

	The TKE_Reopen_Segment opcode indicates that an existing segment should be reopened to add more 
	geometry, set additional attributes, etc...

	The m_index variable represents the unique persistent identifier of the segment to be reopened.
*/
class BBINFILETK_API TK_Reopen_Segment : public BBaseOpcodeHandler {
    private:
        TK_Reopen_Segment(const TK_Reopen_Segment&);
        TK_Reopen_Segment& operator=(const TK_Reopen_Segment&);

	protected:
		int             m_index;   /*!< Represents the unique persistent identifier of the segment to be reopened */

	public:
		/*! constructor */
		TK_Reopen_Segment () : BBaseOpcodeHandler (TKE_Reopen_Segment), m_index (-1) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the index of the segment */
		void         SetIndex (int i) alter      { m_index = i;      }
		/*! Returns the index of the segment */
		int          GetIndex (void) const       { return m_index;   }
};


//! Handles the TKE_Inlude_Segment and TKE_Style_Segment opcodes.
/*!  
	TK_Referenced_Segment provides support for writing/reading the TKE_Inlude_Segment and
	TKE_Style_Segment opcode objects to/from an HSF file.  

	The HOOPS/3dGS scene-graph nodes are called 'segments' which store, geometry, attributes, and subsegments.
	The scene-graph provides support for referencing segments where they can either be 'included' 
	as virtual subsegments, or they can be 'styled' so that only the attributes are referenced.
*/
class BBINFILETK_API TK_Referenced_Segment : public BBaseOpcodeHandler {
    private:
        TK_Referenced_Segment(const TK_Referenced_Segment&);
        TK_Referenced_Segment& operator=(const TK_Referenced_Segment&);

	protected:
		int             m_length;               /*!< Length of segment string name.  for internal use only */
		int             m_allocated;            /*!< Size of currently available string buffer.  for internal use only */
		char *          m_string;               /*!< Segment name.  for internal use only */
		int             m_cond_length;          /*!< Length of condition.  for internal use only */
		int             m_cond_allocated;       /*!< Size of currently available condition buffer.  for internal use only */
		char *          m_condition;            /*!< Condition.  for internal use only */

		ID_Key          m_key;                  //!< for internal use only
		BBaseOpcodeHandler *     m_referee;     //!< for internal use only
		bool            m_follow;               //!< for internal use only

		void    set_segment (char const * segment) alter;   //!< for internal use only
		void    set_segment (int length) alter;             //!< for internal use only

	public:
		/*! constructor */
		TK_Referenced_Segment (unsigned char opcode);
		~TK_Referenced_Segment();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		void        Reset (void) alter;

		/*! 
		  Set the segment name.  Makes sure the buffer is big enough to hold 
		  the segment name, and copies it in 
		*/
		void            SetSegment (char const * segment) alter { set_segment (segment); }
		/*! 
		  Set the segment name.  Makes sure the buffer is big enough to hold 
		  a string of 'length' characters 
		*/
		void            SetSegment (int length) alter           { set_segment (length); }
		/*! 
		  Get the segment name.  Returns a pointer to the current segment name
		*/
		char const *    GetSegment (void) const                 { return m_string; }
		/*! 
		  Get the segment name buffer.  Returns a pointer to the current segment name buffer
		  which may then be modified directly
		*/
		char alter *    GetSegment (void) alter                 { return m_string; }


		/*! 
		  Set the condition string.  Makes sure the buffer is big enough to hold 
		  the condition, and copies it in 
		*/
		void            SetCondition (char const * condition) alter;
		/*! 
		  Set the condition string.  Makes sure the buffer is big enough to hold 
		  a string of 'length' characters 
		*/
		void            SetCondition (int length) alter;
		/*! 
		  Get the condition string.  Returns a pointer to the current condition
		*/
		char const *    GetCondition (void) const                 { return m_condition; }
		/*! 
		  Get the condition string buffer.  Returns a pointer to the current condition buffer
		  which may then be modified directly
		*/
		char alter *    GetCondition (void) alter                 { return m_condition; }


		/*! Sets the flag indicating whether the reference segment should be processed itself */
		void            SetFollow (bool f) alter                { m_follow = f;     }
		/*! Returns the flag indicating whether the reference segment should be processed itself */
		bool            GetFollow (void) alter                  { return m_follow;  }
};


class BBINFILETK_API TK_Reference : public BBaseOpcodeHandler {
	private:
        TK_Reference(const TK_Reference&);
        TK_Reference& operator=(const TK_Reference&);

	protected:
		int				m_index;				/*!< tag index of referenced item.  for internal use only */
		int             m_cond_length;          /*!< Length of condition.  for internal use only */
		int             m_cond_allocated;       /*!< Size of currently available condition buffer.  for internal use only */
		char *          m_condition;            /*!< Condition.  for internal use only */

		ID_Key          m_this_key;             //!< for internal use only
		ID_Key          m_key;                  //!< for internal use only
		BBaseOpcodeHandler *     m_referee;     //!< for internal use only
		bool            m_follow;               //!< for internal use only

	public:
		/*! constructor */
		TK_Reference ();
		~TK_Reference();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		void        Reset (void) alter;

		/*! Sets the id of the referenced item */
		void            SetIndex (int index) alter             { m_index = index;	}
		/*! Returns the id of the referenced item */
		ID_Key          GetIndex (void) alter                  { return m_index;	}

		/*! 
		  Set the condition string.  Makes sure the buffer is big enough to hold 
		  the condition, and copies it in 
		*/
		void            SetCondition (char const * condition) alter;
		/*! 
		  Set the condition string.  Makes sure the buffer is big enough to hold 
		  a string of 'length' characters 
		*/
		void            SetCondition (int length) alter;
		/*! 
		  Get the condition string.  Returns a pointer to the current condition
		*/
		char const *    GetCondition (void) const                 { return m_condition; }
		/*! 
		  Get the condition string buffer.  Returns a pointer to the current condition buffer
		  which may then be modified directly
		*/
		char alter *    GetCondition (void) alter                 { return m_condition; }


		/*! Sets the flag indicating whether the referenced item should be processed itself */
		void            SetFollow (bool f) alter                { m_follow = f;     }
		/*! Returns the flag indicating whether the referenced item should be processed itself */
		bool            GetFollow (void) alter                  { return m_follow;  }
};
/*! \enum Instance_Options
	options for the TKE_Repeat_Object opcode
*/
enum Instance_Options {
	Instance_By_Tristrip    = 0x01  //!< polyhedra will be instanced using their tristrip information
};


//! Handles the TKE_Repeat_Object opcode.
/*!  
	As its name implies, the TKE_Repeat_Object is an instruction to make a copy of a previously given object.
	Given an index and a modelling matrix, make a copy of an object, transform the points and reinsert.  
	If you would like to ignore it, you can just implement a subclass that just returns TK_Normal on
	TK_Instance::Execute().  Of course, a better solution is to provide a real implementation, so that
	there will be no missing geometry.  An example of such an implementation is provided in source form
	in HTK_Instance::Execute, which currently lives in HOpcodeHandler.cpp.
*/
class BBINFILETK_API TK_Instance : public BBaseOpcodeHandler {
    private:
        TK_Instance(const TK_Instance&);
        TK_Instance& operator=(const TK_Instance&);

	protected:
		int             m_from_index;   //!< internal use
		int             m_from_variant; //!< internal use
		int             m_to_index;		//!< internal use
		int             m_to_variant;	//!< internal use
		int             m_options;		//!< internal use
		float           m_matrix[16];	//!< internal use

	public:
		/*! constructor */
		TK_Instance (int from_index=0, int from_variant=0, int to_index=0, int to_variant=0,
					 int options=0, float const * xform=0);  

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset () alter;
};

//! Handles the TKE_Delete_Object opcode.
/*!  
	As its name implies, the TKE_Delete_Object is an instruction to remove a previously given object.
*/
class BBINFILETK_API TK_Delete_Object : public BBaseOpcodeHandler {
    private:
        TK_Delete_Object(const TK_Delete_Object&);
        TK_Delete_Object& operator=(const TK_Delete_Object&);

	protected:
		int             m_index;   //!< internal use

	public:
		/*! constructor */
		TK_Delete_Object () : BBaseOpcodeHandler (TKE_Delete_Object), m_index (-1) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the index of the item to be deleted */
		void		SetIndex (int i) alter					{ m_index = i;		}
		/*! Returns the index of the item to be deleted */
		int			GetIndex (void) alter					{ return m_index;	}
};


////////////////////////////////////////


//! Handles the TKE_LOD opcode.
/*! 
	Handles segment-level LODs only.  LODs attached to Shells are handled by TK_Shell.
*/
class BBINFILETK_API TK_LOD : public BBaseOpcodeHandler {
    private:
        TK_LOD(const TK_LOD&);
        TK_LOD& operator=(const TK_LOD&);

	protected:
		int *m_num_primitives;              //!<an array of ints to indicate the length of each row in m_primitives
		BBaseOpcodeHandler ***m_primitives; //!<for each level, an array of opcode handler pointers that store the primitives
		int m_highest_level;                //!<keeps track of highest level lod that has been seen so far
		int m_levels_allocated;             //!<the number of entries allocated in m_num_primitives and m_primitives
		int m_substage;                     //!<tracks progress of reading individual opcode handler arrays.
		struct vlist_s *m_current_working;  //!<holds the geometry list in progress.  struct vlist_s the incomplete type for vlist_t (used until vlist.h header file is read in)
		int m_current_level;		    //!<the index of the level currently in progress.

		TK_Status   ReadOneList (BStreamFileToolkit & tk) alter;  //!<reads the list of primitives into m_primitives[m_current_level], terminated with a TKE_Termination

	public:
		/*! constructor */
		TK_LOD () : BBaseOpcodeHandler (TKE_LOD) {
			m_num_primitives = 0;
			m_primitives = 0;
			m_highest_level = 0;
			m_levels_allocated = 0;
			m_substage = 0;
			m_current_working = 0;
			m_current_level = 0;
		}
		~TK_LOD();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;
};
//! terminates the list of all LOD levels.  TKE_Termination could not be used, since 0 is a valid value for LOD level.
#define TKLOD_ESCAPE 255


////////////////////////////////////////

//! Handles the TKE_Geometry_Attributes opcode.
/*!  
	Tags are a means of instructing the toolkit that an object should be 'recorded' during file import and/or export.
	Things that may be revisited later for refinement should be tagged.
	See the Programming Guide for more detailed information.
*/
class BBINFILETK_API TK_Geometry_Attributes : public BBaseOpcodeHandler {
    private:
        TK_Geometry_Attributes(const TK_Geometry_Attributes&);
        TK_Geometry_Attributes& operator=(const TK_Geometry_Attributes&);

	protected:

	public:
		/*! constructor */
		TK_Geometry_Attributes () : BBaseOpcodeHandler (TKE_Geometry_Attributes) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		TK_Status   Execute (BStreamFileToolkit & tk) alter;
};


class BBINFILETK_API TK_Close_Geometry_Attributes : public BBaseOpcodeHandler {
    private:
        TK_Close_Geometry_Attributes(const TK_Close_Geometry_Attributes&);
        TK_Close_Geometry_Attributes& operator=(const TK_Close_Geometry_Attributes&);

	protected:

	public:
		/*! constructor */
		TK_Close_Geometry_Attributes () : BBaseOpcodeHandler (TKE_Close_Geometry_Attributes) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		TK_Status   Execute (BStreamFileToolkit & tk) alter;
};

//! Handles the TKE_Renumber_Key_Global, TKE_Renumber_Key_Local, and TKE_Priority opcodes.
/*!  
	TK_Renumber provides support for writing/reading the TKE_Renumber_Key_Global and
	TKE_Renumber_Key_Local opcode objects to/from an HSF file.  This action operates on
	whatever entity was specified in the previous opcode.

	The HOOPS/3dGS scene-graph supports the concept of renumbered keys.  (Keys can refer to either segments or 
	geometric primitives.)  HOOPS entities that do not have keys (e.g. rendering options and other attributes) 
	may not be renumbered.  HOOPS/3DGS segments may hav a priority value assigned which overrides the default
	alphabetic sorting.
*/
class BBINFILETK_API TK_Renumber : public BBaseOpcodeHandler {
    private:
        TK_Renumber(const TK_Renumber&);
        TK_Renumber& operator=(const TK_Renumber&);

	protected:
		ID_Key          m_key; //!< for internal use only

	public:
		/*! same as the default constructor in BBaseOpcodeHandler, except that a new key can 
			optionally be provided directly at the time of construction 
		*/
		TK_Renumber (unsigned char opcode, ID_Key key = 0) : BBaseOpcodeHandler (opcode), m_key (key) {}  

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		/*! Sets the key (identifier) value */
		void            SetKey (ID_Key k) alter     { m_key = k;    }
		/*! Returns the key (identifier) value */
		ID_Key          GetKey (void) const         { return m_key; }
};


//! Handles the TKE_Tag opcode.
/*!  
	Tags are a means of instructing the toolkit that an object should be 'recorded' during file import and/or export.
	Things that may be revisited later for refinement should be tagged.
	See the Programming Guide for more detailed information.
*/
class BBINFILETK_API TK_Tag : public BBaseOpcodeHandler {
    private:
        TK_Tag(const TK_Tag&);
        TK_Tag& operator=(const TK_Tag&);

	protected:

	public:
		/*! constructor */
		TK_Tag (unsigned char opcode = TKE_Tag) : BBaseOpcodeHandler (opcode) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		TK_Status   Execute (BStreamFileToolkit & tk) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
								{ return BBaseOpcodeHandler::Interpret(tk, key, special); }
};

//! Handles the TKE_Dictionary opcode.
/*!  
	TK_Dictionary provides support for writing/reading the TKE_Dictionary opcode object to/from an HSF file.  

	The dictionary provides a mean for locating specific entities in the file.
	See the Programming Guide for more detailed information.
*/

// Note: unlike most opcode handlers, this one does not contain its own data, it is primarily a
// wrapper around the key <-> index translation table in the toolkit.
class BBINFILETK_API TK_Dictionary : public BBaseOpcodeHandler {
    private:
        TK_Dictionary(const TK_Dictionary&);
        TK_Dictionary& operator=(const TK_Dictionary&);

	protected:
		unsigned char   m_format;		//!< internal use
		int             m_placeholder;		//!< internal use
		unsigned char   m_present;		//!< internal use
		int             m_number_of_items;	//!< internal use

		Internal_Translator::Index_Key_Pair alter * m_item; //!< internal use; cache lookup in Pending cases

	public:
		/*! constructor */
		TK_Dictionary () : BBaseOpcodeHandler (TKE_Dictionary), m_format (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		TK_Status   Execute (BStreamFileToolkit & tk) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
								{ return BBaseOpcodeHandler::Interpret(tk, key, special); }
		void        Reset (void) alter;
};


//! Handles the TKE_Dictionary_Locater opcode.
/*!  
	TK_Dictionary_Locater provides support for writing/reading the TKE_Dictionary_Locater opcode object to/from an HSF file.  

	The dictionary provides a mean for locating specific entities in the file, the locater will be placed at the end
	of the file if the dictionary itself isn't.
	See the Programming Guide for more detailed information.
*/
class BBINFILETK_API TK_Dictionary_Locater : public BBaseOpcodeHandler {
    private:
        TK_Dictionary_Locater( const TK_Dictionary_Locater& );
        TK_Dictionary_Locater& operator=(const TK_Dictionary_Locater&);

	protected:
		int             m_size;			//!< internal use
		int             m_offset;		//!< internal use

	public:
		/*! constructor */
		TK_Dictionary_Locater () : BBaseOpcodeHandler (TKE_Dictionary_Locater), m_offset (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		TK_Status   Execute (BStreamFileToolkit & tk) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant=0) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
								{ return BBaseOpcodeHandler::Interpret(tk, key, special); }
		void        Reset (void) alter;

		/*! Sets the size value */
	void	    SetSize (int size) alter	    { m_size = size; }
		/*! Returns the size value */
	int	    GetSize (void) const	    { return m_size;   }
		/*! Sets the offset value */
	void	    SetOffset (int offset) alter    { m_offset = offset; }
		/*! Returns the offset value */
	int	    GetOffset (void) const	    { return m_offset;   }
};


////////////////////////////////////////////////////////////////////////////////

/*! \enum TKO_Geometry_Bits
  Denotes how various attributes are applied to geometry types 
*/
enum TKO_Geometry_Bits {
	// first byte is common/shared items, plus flag for extended bits
	TKO_Geo_Face            = 0x00000001, //!< common/shared items; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Edge            = 0x00000002, //!< common/shared items; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Line            = 0x00000004, //!< common/shared items; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Marker          = 0x00000008, //!< common/shared items; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Text            = 0x00000010, //!< common/shared items; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Window          = 0x00000020, //!< common/shared items; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Image           = 0x00000040, //!< common/shared items; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Extended        = 0x00000080, //!< extended bits for common/shared items
	TKO_Geo_Extended_Mask   = 0xFFFFFF00, //!< internal use, indicates bits which require TKO_Geo_Extended

	// extras for color
	TKO_Geo_Ambient         = 0x00000100, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Light           = 0x00000200, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Face_Contrast   = 0x00000400, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Window_Contrast = 0x00000800, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Front           = 0x00001000, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Back            = 0x00002000, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Vertex          = 0x00004000, //!< extra item for color; refer to ::HC_Set_Color for a description

	TKO_Geo_Geom_Colors     = 0x0000701F, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Every_Colors    = 0x000073BF, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Extended_Colors = 0x00008000, //!< extended bits for color
	TKO_Geo_Extended_Colors_Mask
							= 0xFFFF0000, //!< internal use, indicates bits which require TKO_Geo_Extended_Colors

	TKO_Geo_Edge_Contrast   = 0x00010000, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Line_Contrast   = 0x00020000, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Marker_Contrast = 0x00040000, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Vertex_Contrast = 0x00080000, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Cut_Edge        = 0x00100000, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Simple_Reflection=0x00200000, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Cut_Face        = 0x00400000, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_Extended2       = 0x00800000, //!< secondary extended bits
	TKO_Geo_Extended2_Mask  = 0xFF000000, //!< internal use, indicates bits which require TKO_Geo_Extended2
	TKO_Geo_Text_Contrast   = 0x01000000, //!< extra item for color; refer to ::HC_Set_Color for a description
	TKO_Geo_All_Colors      = 0x017F7F7F, //!< extra item for color; refer to ::HC_Set_Color for a description

	//extras for selectability (light is same as for color)
	TKO_Geo_String_Cursor   = 0x00000100, //!< extra item for selectability; refer to ::HC_Set_Selectability for a description

	TKO_Geo_Geom_Selects    = 0x0000435F, //!< extra item for selectability; refer to ::HC_Set_Selectability for a description
	TKO_Geo_All_Selects     = 0x0000437F, //!< extra item for selectability; refer to ::HC_Set_Selectability for a description

	// extras for visibility (string cursor same as selectability)
	TKO_Geo_Face_Lighting   = 0x00000200, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Edge_Lighting   = 0x00000400, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Marker_Lighting = 0x00000800, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Light_Visibles  = 0x00000E00, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description 

	TKO_Geo_Silhouette_Edge = 0x00001000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Perimeter_Edge  = 0x00002000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Mesh_Quad       = 0x00004000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Hard_Edge       = 0x00008000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description

	TKO_Geo_Cutting_Plane   = 0x00010000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Shadow_Emit     = 0x00020000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Shadow_Cast     = 0x00040000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Shadow_Receive  = 0x00080000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Shadow_Visibles = 0x000E0000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Vertex_Vis      = 0x00200000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Cut_Geometry    = 0x00500000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Adjacent_Edge   = 0x01000000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_NonCulled_Edge  = 0x02000000, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
	TKO_Geo_Edge_Visibles   = 0x0300F002, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description


	TKO_Geo_Geom_Visibles   = 0x0301FFFF, //!< extra item for visibility; refer to ::HC_Set_Visibility for a description



	TKO_Geo_All_Visibles    = 0x037FFF7F  //!< extra item for visibility; refer to ::HC_Set_Visibility for a description
};


/*! \enum TKO_Color_Channels
  Defines color channels; refer to ::HC_Set_Color for details
*/
enum TKO_Color_Channels {
	TKO_Channel_Diffuse         = 0, //!< self-explanatory
	TKO_Channel_Specular        = 1, //!< self-explanatory
	TKO_Channel_Mirror          = 2, //!< self-explanatory
	TKO_Channel_Transmission    = 3, //!< self-explanatory
	TKO_Channel_Emission        = 4, //!< self-explanatory
	TKO_Channel_Gloss           = 5, //!< self-explanatory
	TKO_Channel_Index           = 6, //!< color by index
	TKO_Channel_Extended        = 7, //!< extended bit
	TKO_Channel_Environment     = 8, //!< env map
	TKO_Channel_Bump            = 9, //!< bump map

	TKO_Channel_Count           = 10 
};


//! Handles the TKE_Color opcode.
/*!  
	TK_Color provides support for writing/reading the TKE_Color opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph segments can have a 'named' color (i.e. "faces = red, lines = blue")
*/
class BBINFILETK_API TK_Color : public BBaseOpcodeHandler {
	protected:
		int             m_mask;		//!< internal use
		short           m_channels;	//!< internal use

		/*!  
		   This class stores the color information for a single color channel  
		*/
		class BBINFILETK_API channel {
			public:
				float   m_rgb[3];      //!< The RGB value of the color for this channel
				char *  m_name;        //!< The name of the color channel

				channel() : m_name (0) {}
				~channel() { ::delete [] m_name; }
				void    Reset (void) alter  { ::delete [] m_name;  m_name = 0; }	//!< internal use
		};

		channel         m_diffuse;		//!< internal use
		channel         m_specular;		//!< internal use
		channel         m_mirror;		//!< internal use
		channel         m_transmission;	//!< internal use
		channel         m_emission;		//!< internal use
		channel         m_environment;  //!< internal use; note: environment & bump are never a simple RGB type color
		channel         m_bump;			//!< internal use
		float           m_gloss;		//!< internal use
		float           m_index;		//!< internal use
		int             m_substage;		//!< internal use; To track the subcases

		//! internal use
		void    set_channel_rgb (channel alter & c, float r, float g, float b, int which_channel = -1) alter {
					c.m_rgb[0] = r;     c.m_rgb[1] = g;     c.m_rgb[2] = b;
					if (which_channel != -1) {
						m_channels |= (1 << which_channel);
						if (which_channel > TKO_Channel_Extended)
						m_channels |= (1 << TKO_Channel_Extended);
					}
				}
		//! internal use
		void    set_channel_name (channel alter & c, char const * name, int which_channel = -1) alter;
		//! internal use
		void    set_channel_name (channel alter & c, int length, int which_channel = -1) alter;

	public:
		TK_Color ();
		~TK_Color ();
        TK_Color(const TK_Color& rColor)
            : BBaseOpcodeHandler( rColor.m_opcode )
            , m_mask(0)
            , m_channels(0)
        {
            SetGeometry( rColor.GetGeometry() );

            int channels = rColor.GetChannels();

            const char *name;

            if(channels & (1<<TKO_Channel_Diffuse))
            {
                name = rColor.GetDiffuseName();
                if(name==NULL)
                {
                    SetDiffuse( rColor.GetDiffuse() );
                }
                else
                {
                    SetDiffuseName( name );
                }
            }

            if(channels & (1<<TKO_Channel_Specular))
            {
                name = rColor.GetSpecularName();
                if(name==NULL)
                {
                    SetSpecular( rColor.GetSpecular() );
                }
                else
                {
                    SetSpecularName( name );
                }
            }

            if(channels & (1<<TKO_Channel_Mirror))
            {
                name = rColor.GetMirrorName();
                if(name==NULL)
                {
                    SetMirror( rColor.GetMirror() );
                }
                else
                {
                    SetMirrorName( name );
                }
            }

            if(channels & (1<<TKO_Channel_Transmission))
            {
                name = rColor.GetTransmissionName();
                if(name==NULL)
                {
                    SetTransmission( rColor.GetTransmission() );
                }
                else
                {
                    SetTransmissionName( name );
                }
            }

            if(channels & (1<<TKO_Channel_Emission))
            {
                name = rColor.GetEmissionName();
                if(name==NULL)
                {
                    SetEmission( rColor.GetEmission() );
                }
                else
                {
                    SetEmissionName( name );
                }
            }

            if(channels & (1<<TKO_Channel_Bump))
            {
                SetBumpName( rColor.GetBumpName() );
            }
            if(channels & (1<<TKO_Channel_Environment))
            {
                SetEnvironmentName( rColor.GetEnvironmentName() );
            }
            if(channels & (1<<TKO_Channel_Gloss))
            {
                SetGloss( rColor.GetGloss() );
            }
            if(channels & (1<<TKO_Channel_Index))
            {
                SetIndex( rColor.GetIndex() );
            }

            SetChannels( channels ); // should be last, just to be safe.
        }
        TK_Color& operator=(const TK_Color& rColor)
        {
            if(&rColor != this)
            {
                m_opcode    = rColor.m_opcode;
                m_mask      = rColor.m_mask;
                m_channels  = rColor.m_channels;

                SetGeometry( rColor.GetGeometry() );

                int channels = rColor.GetChannels();

                const char *name;

                if(channels & (1<<TKO_Channel_Diffuse))
                {
                    name = rColor.GetDiffuseName();
                    if(name==NULL)
                    {
                        SetDiffuse( rColor.GetDiffuse() );
                    }
                    else
                    {
                        SetDiffuseName( name );
                    }
                }

                if(channels & (1<<TKO_Channel_Specular))
                {
                    name = rColor.GetSpecularName();
                    if(name==NULL)
                    {
                        SetSpecular( rColor.GetSpecular() );
                    }
                    else
                    {
                        SetSpecularName( name );
                    }
                }

                if(channels & (1<<TKO_Channel_Mirror))
                {
                    name = rColor.GetMirrorName();
                    if(name==NULL)
                    {
                        SetMirror( rColor.GetMirror() );
                    }
                    else
                    {
                        SetMirrorName( name );
                    }
                }

                if(channels & (1<<TKO_Channel_Transmission))
                {
                    name = rColor.GetTransmissionName();
                    if(name==NULL)
                    {
                        SetTransmission( rColor.GetTransmission() );
                    }
                    else
                    {
                        SetTransmissionName( name );
                    }
                }

                if(channels & (1<<TKO_Channel_Emission))
                {
                    name = rColor.GetEmissionName();
                    if(name==NULL)
                    {
                        SetEmission( rColor.GetEmission() );
                    }
                    else
                    {
                        SetEmissionName( name );
                    }
                }

                if(channels & (1<<TKO_Channel_Bump))
                {
                    SetBumpName( rColor.GetBumpName() );
                }
                if(channels & (1<<TKO_Channel_Environment))
                {
                    SetEnvironmentName( rColor.GetEnvironmentName() );
                }
                if(channels & (1<<TKO_Channel_Gloss))
                {
                    SetGloss( rColor.GetGloss() );
                }
                if(channels & (1<<TKO_Channel_Index))
                {
                    SetIndex( rColor.GetIndex() );
                }

                SetChannels( channels ); // should be last, just to be safe.
            }
            return *this;
        }


		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the mask value which indicates which types of geometry are affected by this color setting.  Geometry types are specified by #TKO_Geometry_Bits.  */
		void            SetGeometry (int m) alter {
							m_mask = m & TKO_Geo_All_Colors;
							if ((m & TKO_Geo_Extended_Mask) != 0) {
								m_mask |= TKO_Geo_Extended;
								if ((m & TKO_Geo_Extended_Colors_Mask) != 0) {
									m_mask |= TKO_Geo_Extended_Colors;
									if ((m & TKO_Geo_Extended2_Mask) != 0)
										m_mask |= TKO_Geo_Extended2;
								}
							}
						}
		/*! Returns the mask value which indicates which types of geometry are affected by this color setting.  Geometry types are specified by #TKO_Geometry_Bits.*/
		int             GetGeometry (void) const                        { return m_mask;                        }
		/*! Sets the mask indicating which color/material components are valid.  Color channels are specified by #TKO_Color_Channels. Note that #TKO_Color_Channels are **NOT** bit masks, but rather indices, so you should pass (1<<TKO_Channel_Diffuse) here, for example. */
		void            SetChannels (int c) alter  {
							m_channels = (short)c;
							if ((c & (~0 << (TKO_Channel_Extended+1))) != 0)
								m_channels |= (1 << TKO_Channel_Extended);
						}
		/*! Returns the mask indicating which color/material components are valid.  Color channels are specified by #TKO_Color_Channels. */
		int             GetChannels (void) const                        { return (int)m_channels;               }

		/*! Sets the diffuse color using discrete rgb values */
		void            SetDiffuse (float r, float g, float b) alter    { set_channel_rgb (m_diffuse, r, g, b, TKO_Channel_Diffuse); }
		/*! Sets the diffuse color using a float triplet*/
		void            SetDiffuse (float const * rgb) alter            { SetDiffuse (rgb[0], rgb[1], rgb[2]);  }
		/*! Sets the diffuse color as a string value.  Allocates the buffer and copies the string*/
		void            SetDiffuseName (char const * name) alter        { set_channel_name (m_diffuse, name, TKO_Channel_Diffuse);	 }
		/*! Sets the diffuse color string buffer.  Allocates a buffer to hold a string of 'length' characters */
		void            SetDiffuseName (int length) alter               { set_channel_name (m_diffuse, length, TKO_Channel_Diffuse); }
		/*! Returns the diffuse color as the address of a float triplet*/
		float const *   GetDiffuse (void) const                         { return m_diffuse.m_rgb;               }
		/*! Returns the address of the diffuse color name string.*/
		char const *    GetDiffuseName (void) const                     { return m_diffuse.m_name;              }
		/*! Returns the address of the diffuse color name buffer, which may be modified directly.*/
		char alter *    GetDiffuseName (void) alter                     { return m_diffuse.m_name;              }

		/*! Sets the specular color using discrete rgb values*/
		void            SetSpecular (float r, float g, float b) alter   { set_channel_rgb (m_specular, r, g, b, TKO_Channel_Specular);}
		/*! Sets the specular color using a float triplet*/
		void            SetSpecular (float const * rgb) alter           { SetSpecular (rgb[0], rgb[1], rgb[2]); }
		/*! Sets the specular color as a string value.  Allocates the buffer and copies the string*/
		void            SetSpecularName (char const * name) alter       { set_channel_name (m_specular, name, TKO_Channel_Specular);  }
		/*! Sets the specular color string buffer.  Allocates a buffer to hold a string of 'length' characters*/
		void            SetSpecularName (int length) alter              { set_channel_name (m_specular, length, TKO_Channel_Specular);}
		/*! Returns the specular color as the address of a float triplet*/
		float const *   GetSpecular (void) const                        { return m_specular.m_rgb;              }
		/*! Returns the address of the specular color name string. */
		char const *    GetSpecularName (void) const                    { return m_specular.m_name;             }
		/*! Returns the address of the specular color name buffer, which may be modified directly. */
		char alter *    GetSpecularName (void) alter                    { return m_specular.m_name;             }

		/*! Sets the mirror color using discrete rgb values */
		void            SetMirror (float r, float g, float b) alter     { set_channel_rgb (m_mirror, r, g, b, TKO_Channel_Mirror);  }
		/*! Sets the mirror color using a float triplet */
		void            SetMirror (float const * rgb) alter             { SetMirror (rgb[0], rgb[1], rgb[2]);   }
		/*! Sets the mirror color as a string value.  Allocates the buffer and copies the string */
		void            SetMirrorName (char const * name) alter         { set_channel_name (m_mirror, name, TKO_Channel_Mirror);    }
		/*! Sets the mirror color string buffer.  Allocates a buffer to hold a string of 'length' characters */
		void            SetMirrorName (int length) alter                { set_channel_name (m_mirror, length, TKO_Channel_Mirror);  }
		/*! Returns the mirror color as the address of a float triplet */
		float const *   GetMirror (void) const                          { return m_mirror.m_rgb;                }
		/*! Returns the address of the mirror color name string. */
		char const *    GetMirrorName (void) const                      { return m_mirror.m_name;               }
		/*! Returns the address of the mirror color name buffer, which may be modified directly. */
		char alter *    GetMirrorName (void) alter                      { return m_mirror.m_name;               }

		/*! Sets the transmission color using discrete rgb values */
		void            SetTransmission (float r, float g, float b) alter { set_channel_rgb (m_transmission, r, g, b, TKO_Channel_Transmission); }
		/*! Sets the transmission color using a float triplet */
		void            SetTransmission (float const * rgb) alter         { SetTransmission (rgb[0], rgb[1], rgb[2]);  }
		/*! Sets the transmission color as a string value.  Allocates the buffer and copies the string */
		void            SetTransmissionName (char const * name) alter     { set_channel_name (m_transmission, name, TKO_Channel_Transmission);   }
		/*! Sets the transmission color string buffer.  Allocates a buffer to hold a string of 'length' characters */
		void            SetTransmissionName (int length) alter            { set_channel_name (m_transmission, length, TKO_Channel_Transmission); }
		/*! Returns the transmission color as the address of a float triplet */
		float const *   GetTransmission (void) const                      { return m_transmission.m_rgb;               }
		/*! Returns the address of the transmission color name string. */
		char const *    GetTransmissionName (void) const                  { return m_transmission.m_name;              }
		/*! Returns the address of the transmission color name buffer, which may be modified directly. */
		char alter *    GetTransmissionName (void) alter                  { return m_transmission.m_name;              }

		/*! Sets the emission color using discrete rgb values */
		void            SetEmission (float r, float g, float b) alter   { set_channel_rgb (m_emission, r, g, b, TKO_Channel_Emission);}
		/*! Sets the emission color using a float triplet */
		void            SetEmission (float const * rgb) alter           { SetEmission (rgb[0], rgb[1], rgb[2]); }
		/*! Sets the emission color as a string value.  Allocates the buffer and copies the string */
		void            SetEmissionName (char const * name) alter       { set_channel_name (m_emission, name, TKO_Channel_Emission);  }
		/*! Sets the emission color string buffer.  Allocates a buffer to hold a string of 'length' characters */
		void            SetEmissionName (int length) alter              { set_channel_name (m_emission, length, TKO_Channel_Emission);}
		/*! Returns the emission color as the address of a float triplet */
		float const *   GetEmission (void) const                        { return m_emission.m_rgb;              }
		/*! Returns the address of the emission color name string. */
		char const *    GetEmissionName (void) const                    { return m_emission.m_name;             }
		/*! Returns the address of the emission color name buffer, which may be modified directly. */
		char alter *    GetEmissionName (void) alter                    { return m_emission.m_name;             }

		/*! Sets the environment texture as a string value.  Allocates the buffer and copies the string */
		void            SetEnvironmentName (char const * name) alter    { set_channel_name (m_environment, name, TKO_Channel_Environment);   }
		/*! Sets the environment texture string buffer.  Allocates a buffer to hold a string of 'length' characters */
		void            SetEnvironmentName (int length) alter           { set_channel_name (m_environment, length, TKO_Channel_Environment); }
		/*! Returns the address of the environment texture name string. */
		char const *    GetEnvironmentName (void) const                 { return m_environment.m_name;              }
		/*! Returns the address of the environment texture name buffer, which may be modified directly. */
		char alter *    GetEnvironmentName (void) alter                 { return m_environment.m_name;              }

		/*! Sets the bump texture as a string value.  Allocates the buffer and copies the string */
		void            SetBumpName (char const * name) alter           { set_channel_name (m_bump, name, TKO_Channel_Bump);      }
		/*! Sets the bump texture string buffer.  Allocates a buffer to hold a string of 'length' characters */
		void            SetBumpName (int length) alter                  { set_channel_name (m_bump, length, TKO_Channel_Bump);    }
		/*! Returns the address of the bump texture name string. */
		char const *    GetBumpName (void) const                        { return m_bump.m_name;                 }
		/*! Returns the address of the bump texture name buffer, which may be modified directly. */
		char alter *    GetBumpName (void) alter                        { return m_bump.m_name;                 }

		/*! Sets the gloss value */
		void            SetGloss (float g) alter                        { m_gloss = g; m_channels |= (1<<TKO_Channel_Gloss); }
		/*! Returns the gloss value */
		float           GetGloss (void) const                           { return m_gloss;                       }
		/*! Sets the index of refraction */
		void            SetIndex (float i) alter                        { m_index = i; m_channels |= (1<<TKO_Channel_Index); }
		/*! Returns the index of refraction */
		float           GetIndex (void) const                           { return m_index;                       }
};


//! Handles the TKE_Color opcode.
/*!  
	TK_Color provides support for writing/reading the TKE_Color opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph segments can have a 'named' color (i.e. "faces = red, lines = blue")
*/
class BBINFILETK_API TK_Color_RGB : public BBaseOpcodeHandler {
    private:
        TK_Color_RGB(const TK_Color_RGB&);
        TK_Color_RGB& operator=(const TK_Color_RGB&);

	protected:
		int             m_mask;		//!< internal use	
		float           m_rgb[3];	//!< internal use

	public:
		/*! constructor */
		TK_Color_RGB () : BBaseOpcodeHandler (TKE_Color_RGB), m_mask (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*!
			Sets the mask value which indicates which types of geometry are affected by this color setting. Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		void            SetGeometry (int m) alter {
							m_mask = m & TKO_Geo_All_Colors;
							if ((m & TKO_Geo_Extended_Mask) != 0) {
								m_mask |= TKO_Geo_Extended;
								if ((m & TKO_Geo_Extended_Colors_Mask) != 0) {
									m_mask |= TKO_Geo_Extended_Colors;
									if ((m & TKO_Geo_Extended2_Mask) != 0)
										m_mask |= TKO_Geo_Extended2;
								}
							}
						}
		/*!
			Returns the mask value which indicates which types of geometry are affected by this color setting. Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		int             GetGeometry (void) const                    { return m_mask;                        }

		/*! Sets the rgb color value as discrete floats */
		void            SetRGB (float r, float g, float b) alter    { m_rgb[0] = r; m_rgb[1] = g; m_rgb[2] = b; }
		/*! Sets the rgb color value as a float triplet */
		void            SetRGB (float const * rgb) alter            { SetRGB (rgb[0], rgb[1], rgb[2]);          }
		/*! Returns the rgb color value as the address of a float triplet */
		float const *   GetRGB (void) const                         { return m_rgb;                             }
};


//! Handles the TKE_Color_By_Value opcode.
/*!  
	TK_Color_By_Value provides support for writing/reading the TKE_Color_By_Value opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph segments can have colors set by value (RGB, HSV, HIC, etc...)
*/
class BBINFILETK_API TK_Color_By_Value : public BBaseOpcodeHandler {
    private:
        TK_Color_By_Value(const TK_Color_By_Value&);
        TK_Color_By_Value& operator=(const TK_Color_By_Value&);

	protected:
		int             m_mask;			//!< internal use
		float           m_value[3];		//!< internal use
		char            m_space;		//!< internal use

	public:
		/*! constructor */
		TK_Color_By_Value () : BBaseOpcodeHandler (TKE_Color_By_Value), m_mask (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*!
			Sets the mask value which indicates which types of geometry are affected by this color setting. Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		void            SetGeometry (int m) alter {
							m_mask = m & TKO_Geo_All_Colors;
							if ((m & TKO_Geo_Extended_Mask) != 0) {
								m_mask |= TKO_Geo_Extended;
								if ((m & TKO_Geo_Extended_Colors_Mask) != 0) {
									m_mask |= TKO_Geo_Extended_Colors;
									if ((m & TKO_Geo_Extended2_Mask) != 0)
										m_mask |= TKO_Geo_Extended2;
								}
							}
						}
		/*!
			Returns the mask value which indicates which types of geometry are affected by this color setting. Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		int             GetGeometry (void) const                { return m_mask;                                }

		/*! Sets the type of color space used to interpret the values */
		void            SetSpace (int s) alter                  { m_space = (char)s;                            }
		/*! Returns the type of color space used to interpret the values */
		int             GetSpace (void) const                   { return (int)m_space;                          }

		/*! Sets the color value as discrete floats */
		void            SetValue (float a, float b, float c) alter  {
								m_value[0] = a;     m_value[1] = b;     m_value[2] = c;
							}
		/*! Sets the color value as a float triplet */
		void            SetValue (float const * triple) alter   { SetValue (triple[0], triple[1], triple[2]);   }
		/*! Returns the color value as the address of a float triplet */
		float const *   GetValue (void) const                   { return m_value;                               }
};


//! Handles the TKE_Color_By_Index and TKE_Color_By_Index_16 opcode.
/*!  
	TK_Color_By_Index provides support for writing/reading the TKE_Color_By_Index and TKE_Color_By_Index_16 
	opcode objects to/from an HSF file.  (The first being an 8-bit index, the second being a 16-bit index)

	The HOOPS/3dGS scene-graph segments can have colors set by index into a colormap
*/
class BBINFILETK_API TK_Color_By_Index : public BBaseOpcodeHandler {
    private:
        TK_Color_By_Index(const TK_Color_By_Index&);
        TK_Color_By_Index& operator=(const TK_Color_By_Index&);

	protected:
		int             m_mask;		//!< internal use
		int             m_index;	//!< internal use

	public:
		/*! constructor */
		TK_Color_By_Index (unsigned char opcode) : BBaseOpcodeHandler (opcode), m_mask (0), m_index (-1) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*!
			Sets the mask value which indicates which types of geometry are affected by this color setting. Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		void            SetGeometry (int m) alter {
							m_mask = m & TKO_Geo_All_Colors;
							if ((m & TKO_Geo_Extended_Mask) != 0) {
								m_mask |= TKO_Geo_Extended;
								if ((m & TKO_Geo_Extended_Colors_Mask) != 0) {
									m_mask |= TKO_Geo_Extended_Colors;
									if ((m & TKO_Geo_Extended2_Mask) != 0)
										m_mask |= TKO_Geo_Extended2;
								}
							}
						}
		/*!
			Returns the mask value which indicates which types of geometry are affected by this color setting. Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		int             GetGeometry (void) const    { return m_mask;    }

		/*! Sets the color value as an index into the effective color map */
		void            SetIndex (int i) alter      { m_index = i;      }
		/*! Returns the color value as an index into the effective color map */
		int             GetIndex (void) const       { return m_index;   }
};

//! Handles the TKE_Color_By_FIndex opcode.
/*!  
	TK_Color_By_FIndex provides support for writing/reading the TKE_Color_By_FIndex opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph segments can have colors set by floating-point index into a colormap
*/
class BBINFILETK_API TK_Color_By_FIndex : public BBaseOpcodeHandler {
    private:
        TK_Color_By_FIndex(const TK_Color_By_FIndex&);
        TK_Color_By_FIndex& operator=(const TK_Color_By_FIndex&);

	protected:
		int             m_mask;		//!< internal use
		float           m_index;	//!< internal use

	public:
		/*! constructor */
		TK_Color_By_FIndex () : BBaseOpcodeHandler (TKE_Color_By_FIndex), m_mask (0), m_index (-1.0f) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*!
			Sets the mask value which indicates which types of geometry are affected by this color setting. Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		void            SetGeometry (int m) alter {
							m_mask = m & TKO_Geo_All_Colors;
							if ((m & TKO_Geo_Extended_Mask) != 0) {
								m_mask |= TKO_Geo_Extended;
								if ((m & TKO_Geo_Extended_Colors_Mask) != 0) {
									m_mask |= TKO_Geo_Extended_Colors;
									if ((m & TKO_Geo_Extended2_Mask) != 0)
										m_mask |= TKO_Geo_Extended2;
								}
							}
						}
		/*!
			Returns the mask value which indicates which types of geometry are affected by this color setting. Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		int             GetGeometry (void) const    { return m_mask;    }

		/*! Sets the color value as an index into the effective color map */
		void            SetIndex (float val) alter	{ m_index = val;      }
		/*! Returns the color value as an index into the effective color map */
		float           GetIndex (void) const       { return m_index;   }
};

/*! \enum TKO_Map_Format
   Defines colormap format
*/
enum TKO_Map_Format {
	TKO_Map_RGB_Values,     /*!< self-explanatory */
	TKO_Map_String			/*!< self-explanatory */
};

////////////////////
//! Handles the TKE_Color_Map opcode.
/*!  
	TK_Color_Map provides support for writing/reading the TKE_Color_Map opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph can store local colormaps
*/
class BBINFILETK_API TK_Color_Map : public BBaseOpcodeHandler {
    private:
        TK_Color_Map(const TK_Color_Map&);
        TK_Color_Map& operator=(const TK_Color_Map&);

	protected:
		int             m_length;			//!< internal use
		float           m_values[3*256];	//!< internal use
		char *			m_string;			//!< internal use
		unsigned char   m_format;			//!< internal use

        /*! \enum TKO_Map_Format
           Defines colormap format
        */
        enum TKO_Map_Format {
            TKO_Map_RGB_Values,     /*!< self-explanatory; only this one is supported for now; the rest are possible future formats */
            TKO_Map_Complex_Colors,
            TKO_Map_Textures,
            TKO_Map_Mixed
        };

		//! internal use
		void    set_values (int length, float const * values = 0) alter;

	public:
		/*! constructor */
		TK_Color_Map ()
			: BBaseOpcodeHandler (TKE_Color_Map), m_length (0), m_string (0), m_format (TKO_Map_RGB_Values) {}
		~TK_Color_Map();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the format of the color map.  The format is specified by #TKO_Map_Format. */
		void            SetFormat (int f) alter                                 { m_format = (unsigned char)f;  }
		/*! Returns the format of the color map.  The format is specified by #TKO_Map_Format. */
		int             GetFormat (void) const                                  { return (int)m_format;         }

		/*!
			Sets the color map values.  Allocates a buffer to hold 'count' float triplets of rgb values, and if
			'values' is specified, copies the array into the buffer
		*/
		void            SetValues (int count, float const * values = 0) alter   { set_values (count, values);   }
		/*! Returns the color map values (rgb triplets) */
		float const *   GetValues (void) const                                  { return m_values;              }
		/*! Returns the color map values buffer, which may be modified directly */
		float alter *   GetValues (void) alter                                  { return m_values;              }
		/*! Returns the size of the color map (or length of the string buffer) */
		int             GetLength (void) const                                  { return m_length;              }

		/*! 
		  Set the color map string.  Makes sure the buffer is big enough to hold 
		  the string, and copies it in 
		*/
		void            SetString (char const * string) alter;
		/*! 
		  Set the color map.  Makes sure the buffer is big enough to hold a string
		  of 'length' characters
		*/
		void            SetString (int length) alter;
		/*! 
		  Get the colro map string.  Returns a pointer to the current string
		*/
		char const *    GetString (void) const                 { return m_string; }
		/*! 
		  Get the color map string buffer.  Returns a pointer to the current string
		  buffer, which may then be modified directly
		*/
		char alter *    GetString (void) alter                 { return m_string; }
};

////////////////////

////////////////////
//! Handles the TKE_Callback opcode.
/*!  
	TK_Callback provides support for writing/reading the TKE_Callback opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph can store callback requests (functions triggered at certain points during
	drawing or selection of a scene).
*/
class BBINFILETK_API TK_Callback : public BBaseOpcodeHandler {
    private:
        TK_Callback(const TK_Callback&);
        TK_Callback& operator=(const TK_Callback&);

	protected:
		int                     m_length;       /*!< Length of text string */
		char *                  m_string;       /*!< Text string */

	
		void    set_callback (char const * callback) alter;  //!< internal use

		void    set_callback (int length) alter;			 //!< internal use

	public:
		/*! constructor */
		TK_Callback () : BBaseOpcodeHandler (TKE_Callback), m_length (0), m_string (0) {}
		~TK_Callback();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the callback string.  Allocates the buffer and copies the specified string */
		void            SetCallback (char const * callback) alter   { set_callback (callback);  }
		/*! Sets the callback string buffer.  Allocates a buffer which will hold a string of 'length' characters */
		void            SetCallback (int length) alter              { set_callback (length);    }
		/*! Returns the callback string. */
		char const *    GetCallback (void) const                    { return m_string;          }
		/*! Returns the callback string buffer, which may be modified directly. */
		char alter *    GetCallback (void) alter                    { return m_string;          }
};

////////////////////


/*! \def TKO_Rendo_Extended
	Extended bit for Rendering Options
*/

/*! \enum TKO_Rendering_Option_Bits
  Represents various rendering properties
*/
enum TKO_Rendering_Option_Bits {
	TKO_Interp_Texture_Faces            = 0x00000001,  //!< texture interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Texture_Edges            = 0x00000002,  //!< texture interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Texture_Markers          = 0x00000004,  //!< texture interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Texture                  = 0x00000007,  //!< texture interpolation value; refer to ::HC_Set_Rendering_Options for description

	TKO_Interp_Color_Faces              = 0x00000008,  //!< color interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Color_Edges              = 0x00000010,  //!< color interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Color_Markers            = 0x00000020,  //!< color interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Color                    = 0x00000038,  //!< color interpolation value; refer to ::HC_Set_Rendering_Options for description

	TKO_Interp_Index_Faces              = 0x00000040,  //!< color index interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Index_Edges              = 0x00000080,  //!< color index interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Index                    = 0x000000C0,  //!< color index interpolation value; refer to ::HC_Set_Rendering_Options for description

	TKO_Interp_Lighting_Faces_Gouraud   = 0x00000100,  //!< lighting interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Lighting_Faces_Phong     = 0x00000200,  //!< lighting interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Lighting_Edges_Gouraud   = 0x00000400,  //!< lighting interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Lighting_Edges_Phong     = 0x00000800,  //!< lighting interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Lighting_Faces           = 0x00000300,  //!< lighting interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Lighting_Edges           = 0x00000C00,  //!< lighting interpolation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Interp_Lighting                 = 0x00000F00,  //!< lighting interpolation value; refer to ::HC_Set_Rendering_Options for description

	TKO_Rendo_HSR_Algorithm             = 0x00001000,  //!< HSR algorithm; refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_THSR_Algorithm            = 0x00002000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Any_HSR                   = 0x00003000,  //!< refer to ::HC_Set_Rendering_Options for description

	TKO_Rendo_Local_Viewer              = 0x00004000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Perspective_Correction    = 0x00008000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Display_Lists             = 0x00010000,  //!< refer to ::HC_Set_Rendering_Options for description

	TKO_Rendo_Debug                     = 0x00020000,  //!< refer to ::HC_Set_Rendering_Options for description

	TKO_Rendo_Technology                = 0x00040000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Quantization              = 0x00080000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_TQ                        = 0x000C0000,  //!< refer to ::HC_Set_Rendering_Options for description

	TKO_Rendo_Attribute_Lock            = 0x00100000,  //!< refer to ::HC_Set_Rendering_Options for description

	TKO_Rendo_Face_Displacement         = 0x00200000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Fog                       = 0x00400000,  //!< refer to ::HC_Set_Rendering_Options for description

	TKO_Rendo_Buffer_Options            = 0x00800000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Hidden_Line_Options       = 0x01000000,  //!< refer to ::HC_Set_Rendering_Options for description

	TKO_Rendo_LOD                       = 0x02000000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_LOD_Options               = 0x04000000,  //!< refer to ::HC_Set_Rendering_Options for description

	TKO_Rendo_NURBS_Curve_Options       = 0x08000000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_NURBS_Surface_Options     = 0x10000000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_NURBS_Options             = 0x18000000,  //!< refer to ::HC_Set_Rendering_Options for description

	TKO_Rendo_Stereo                    = 0x20000000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Stereo_Separation         = 0x40000000,  //!< refer to ::HC_Set_Rendering_Options for description

// hpux doesn't like the high bit set as part of the enumerated type
    //TKO_Rendo_Extended              = 0x80000000,
#ifndef SWIG
#define TKO_Rendo_Extended                0x80000000   
#else
	TKO_Rendo_Extended					= 0x8000000,
#endif

	// extended settings
	TKO_Rendo_Tessellation              = 0x00000001,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Transparency_Style        = 0x00000002,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Transparency_Hardware     = 0x00000004,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Cut_Geometry              = 0x00000008,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Depth_Range               = 0x00000010,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Mask_Transform            = 0x00000020,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Image_Scale               = 0x00000040,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Local_Cutting_Planes      = 0x00000080,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Simple_Shadow				= 0x00000100,
	TKO_Rendo_Geometry_Options			= 0x00000200,
	TKO_Rendo_Image_Tint				= 0x00000400,
	TKO_Interp_Index_Face_Isolines		= 0x00000800,
	TKO_Rendo_Force_Grayscale			= 0x00001000,
	TKO_Rendo_Transparency_Options      = 0x00002000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_General_Displacement		= 0x00004000,
	TKO_Rendo_Join_Cutoff_Angle			= 0x00008000,
	TKO_Rendo_Screen_Range				= 0x00010000,
	TKO_Rendo_Stereo_Distance           = 0x00020000,  //!< refer to ::HC_Set_Rendering_Options for description
	TKO_Rendo_Shadow_Map				= 0x00040000,
	TKO_Rendo_Simple_Reflection			= 0x00080000,

	// type for specific fields
	TKO_HSR_Hardware                    = 0,   //!< type for HSR field; refer to ::HC_Set_Rendering_Options for description
	TKO_HSR_SZB                         = 1,   //!< type for HSR field; refer to ::HC_Set_Rendering_Options for description
	TKO_HSR_Painters                    = 2,   //!< type for HSR field; refer to ::HC_Set_Rendering_Options for description
	TKO_HSR_Z_Sort_Only                 = 3,   //!< type for HSR field; refer to ::HC_Set_Rendering_Options for description
	TKO_HSR_Priority                    = 4,   //!< type for HSR field; refer to ::HC_Set_Rendering_Options for description
	TKO_HSR_Spider_Web                  = 5,   //!< type for HSR field; refer to ::HC_Set_Rendering_Options for description
	TKO_HSR_Hidden_Line                 = 6,   //!< type for HSR field; refer to ::HC_Set_Rendering_Options for description
	TKO_HSR_None                        = 7,   //!< type for HSR field; refer to ::HC_Set_Rendering_Options for description
	TKO_HSR_Fast_Hidden_Line            = 8,   //!< type for HSR field; refer to ::HC_Set_Rendering_Options for description
	TKO_HSR_Depth_Peeling	            = 9,   //!< type for HSR field; refer to ::HC_Set_Rendering_Options for description
	TKO_HSR_Mask                        = 0x0F,   //!< mask for HSR field
	TKO_THSR_Mask                       = 0xF0,   //!< mask for HSR field

	TKO_Transparency_None               = 0x00,   //!< type for transparency field; refer to ::HC_Set_Rendering_Options for description
	TKO_Transparency_Blending           = 0x01,   //!< type for transparency field; refer to ::HC_Set_Rendering_Options for description
	TKO_Transparency_Screen_Door        = 0x02,   //!< type for transparency field; refer to ::HC_Set_Rendering_Options for description
	TKO_Transparency_Style_Mask         = 0x0F,   //!< type for transparency field; refer to ::HC_Set_Rendering_Options for description

	TKO_Transparency_Peeling_Layers		= 0x10,   //!< type for transparency field; refer to ::HC_Set_Rendering_Options for description
	TKO_Transparency_Peeling_Min_Area	= 0x20,   //!< type for transparency field; refer to ::HC_Set_Rendering_Options for description

	TKO_Cut_Geometry_Level              = 0x01,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Cut_Geometry_Tolerance          = 0x02,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Cut_Geometry_Match_Color        = 0x04,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Cut_Geometry_Level_Entity       = 0,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Cut_Geometry_Level_Segment      = 1,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Cut_Geometry_Level_Segment_Tree = 2,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Cut_Geometry_Match_Color_Off    = 0,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Cut_Geometry_Match_Color_Current= 1,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Cut_Geometry_Match_Color_First  = 2,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Cut_Geometry_Match_Color_Last   = 3,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description

	TKO_Display_List_Level_Entity       = 0,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Display_List_Level_Segment      = 1,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Display_List_Level_Segment_Tree = 2,   //!< type for 'cut geometry' field; refer to ::HC_Set_Rendering_Options for description

	TKO_Simple_Shadow_On				= 0x0001,   //!< type for 'simple shadow' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Shadow_Off				= 0x0002,   //!< type for 'simple shadow' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Shadow_Plane				= 0x0004,   //!< type for 'simple shadow' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Shadow_Light_Direction	= 0x0008,   //!< type for 'simple shadow' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Shadow_Color				= 0x0010,   //!< type for 'simple shadow' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Shadow_Resolution		= 0x0020,   //!< type for 'simple shadow' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Shadow_Blur				= 0x0040,   //!< type for 'simple shadow' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Shadow_Extended			= 0x0080,   // internal use, indicates presence of extended bits
	TKO_Simple_Shadow_Extended_Mask		= 0xFF00,   // internal use, indicates bits which require TKO_Simple_Shadow_Extended
	TKO_Simple_Shadow_Auto				= 0x0100,   //!< type for 'simple shadow' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Shadow_Opacity			= 0x0200,   //!< type for 'simple shadow' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Shadow_Extended2			= 0x8000,   // reserved for future expansion

	TKO_Shadow_Map_On					= 0x0001,   //!< type for 'shadow map' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Shadow_Map_Off					= 0x0002,   //!< type for 'shadow map' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Shadow_Map_Resolution			= 0x0004,   //!< type for 'shadow map' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Shadow_Map_Samples				= 0x0008,   //!< type for 'shadow map' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Shadow_Map_Jitter_On			= 0x0010,   //!< type for 'shadow map' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Shadow_Map_Jitter_Off			= 0x0020,   //!< type for 'shadow map' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Shadow_Map_Extended				= 0x0080,   // reserved for future expansion

	TKO_Simple_Reflection_On			= 0x0001,   //!< type for 'simple reflection' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Reflection_Off			= 0x0002,   //!< type for 'simple reflection' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Reflection_Plane			= 0x0004,   //!< type for 'simple reflection' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Reflection_Opacity		= 0x0008,   //!< type for 'simple reflection' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Reflection_Fading_On		= 0x0010,   //!< type for 'simple reflection' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Reflection_Fading_Off	= 0x0020,   //!< type for 'simple reflection' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Reflection_Blur			= 0x0040,   //!< type for 'simple reflection' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Reflection_Extended		= 0x0080,   // reserved for future expansion
	TKO_Simple_Reflection_Attenuation	= 0x0100,   //!< type for 'simple reflection' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Simple_Reflection_Extended2		= 0x8000,   // reserved for future expansion

	TKO_Mask_None               = 0x00,   //!< type for 'mask transform' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Mask_Camera_Rotation    = 0x01,   //!< type for 'mask transform' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Mask_Camera_Scale       = 0x02,   //!< type for 'mask transform' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Mask_Camera_Translation = 0x04,   //!< type for 'mask transform' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Mask_Camera_Perspective = 0x08,   //!< type for 'mask transform' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Mask_Model_Rotation     = 0x10,   //!< type for 'mask transform' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Mask_Model_Scale        = 0x20,   //!< type for 'mask transform' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Mask_Model_Translation  = 0x40,   //!< type for 'mask transform' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Mask_Camera             = 0x0F,   //!< type for 'mask transform' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Mask_Model              = 0x70,   //!< type for 'mask transform' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Mask_All                = 0x7F,   //!< mask for all 'mask transform' field; refer to ::HC_Set_Rendering_Options for description

	TKO_Technology_Standard             = 0x01,   //!< type for 'technology' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Technology_Soft_Frame_Buffer    = 0x02,   //!< type for 'technology' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Technology_Radiosity            = 0x04,   //!< type for 'technology' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Technology_Ray_Trace            = 0x08,   //!< type for 'technology' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Technology_Mask                 = 0x0F,   //!< type for 'technology' field; refer to ::HC_Set_Rendering_Options for description

	TKO_Quantization_Threshold          = 0x10,   //!< type for 'quantization' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Quantization_Dither             = 0x20,   //!< type for 'quantization' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Quantization_Error_Diffusion    = 0x40,   //!< type for 'quantization' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Quantization_Mask               = 0xF0,   //!< type for 'quantization' field; refer to ::HC_Set_Rendering_Options for description

	TKO_Buffer_Size_Limit               = 0x01,   //!< type for 'buffer options' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Buffer_Retention                = 0x02,   //!< type for 'buffer options' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Buffer_Color_Depth_Match        = 0x04,   //!< type for 'buffer options' field; refer to ::HC_Set_Rendering_Options for description
	TKO_Buffer_Color_Depth_Full         = 0x08,   //!< type for 'buffer options' field; refer to ::HC_Set_Rendering_Options for description

	TKO_Hidden_Line_Visibility_On           = 0x00000001,    //!< type for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Visibility_Off          = 0x00000002,    //!< type for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Pattern                 = 0x00000004,    //!< type for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Face_Displacement       = 0x00000008,    //!< type for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Dim_Factor              = 0x00000010,    //!< type for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Render_Faces_On         = 0x00000020,    //!< type for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Render_Faces_Off        = 0x00000040,    //!< type for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Extended                = 0x00000080,    //!< type for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Extended_Mask           = 0xFFFFFF00,    //!< mask for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Silhouette_Cleanup_On   = 0x00000100,    //!< type for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Silhouette_Cleanup_Off  = 0x00000200,    //!< type for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Extended2               = 0x00008000,    //!< extended bit for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Extended2_Mask          = 0xFFFF0000,    //!< mask for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Color                   = 0x00010000,    //!< extended bit for HLR suboptions; refer to ::HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Weight                  = 0x00020000,    //!< extended bit for HLR suboptions; refer to HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Image_Outline_On        = 0x00040000,    //!< extended bit for HLR suboptions; refer to HC_Set_Rendering_Options for description
	TKO_Hidden_Line_Image_Outline_Off       = 0x00080000,    //!< extended bit for HLR suboptions; refer to HC_Set_Rendering_Options for description
	TKO_Hidden_Line_HSR_Algorithm           = 0x00100000,    //!< extended bit for HLR suboptions; refer to HC_Set_Rendering_Options for description

	TKO_LOD_Conserve_Memory             = 0x00000001,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Screen_Space                = 0x00000002,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Physical                    = 0x00000004,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Tolerance_FRU               = 0x00000008,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Tolerance_ORU               = 0x00000010,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Preprocess                  = 0x00000020,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Bounding_Current            = 0x00000040,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Bounding_Explicit           = 0x00000080,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Ratio                       = 0x00000100,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Threshold                   = 0x00000200,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Min_Triangle_Count          = 0x00000400,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Clamp                       = 0x00000800,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Num_Levels                  = 0x00001000,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Max_Degree                  = 0x00002000,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Tolerance                   = 0x00004000,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Usefulness_Heuristic        = 0x00008000,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Calculation_Cutoff          = 0x00010000,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Fallback                    = 0x00020000,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Collapse_Vertices           = 0x00040000,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Algorithm                   = 0x00080000,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Mode_Segment                = 0x00100000,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description

	TKO_LOD_Threshold_Tris_Per_Pix_Sq   = 1,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Threshold_Tris_Per_CM_Sq    = 2,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Threshold_Percent_Area      = 3,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Threshold_Distance			= 4,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description

	TKO_LOD_Algorithm_Fast              = 1,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Algorithm_Nice              = 2,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description

	TKO_LOD_Heur_Is_Diagonal            = 0x00,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Is_Per_Triangle        = 0x01,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Is_Ratio               = 0x02,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Is_Volume              = 0x04,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Triangle_Size          = 0x08,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description

	TKO_LOD_Heur_Diag                   = 0,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Tri_Diag               = 1,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Diag_Ratio             = 2,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Tri_Diag_Ratio         = 3,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Vol                    = 4,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Tri_Vol                = 5,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Vol_Ratio              = 6,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Heur_Tri_Vol_Ratio          = 7,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description

	TKO_LOD_Fallback_None               = 0,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Fallback_Bounding           = 1,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Fallback_Coarsest           = 2,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Fallback_Coarsest_None      = 3,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Fallback_Coarsest_Bounding  = 4,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description
	TKO_LOD_Fallback_Bounding_None		= 5,    //!< type for LOD options; refer to ::HC_Set_Rendering_Options for description

	TKO_NURBS_Curve_Budget              = 0x0001,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Curve_Continued_Budget    = 0x0002,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Curve_View_Dependent      = 0x0004,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Curve_Max_Deviation       = 0x0008,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Surface_Budget            = 0x0010,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Surface_Trim_Budget       = 0x0020,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Surface_Max_Facet_Width   = 0x0040,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Curve_Max_Angle           = 0x1000,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Curve_Max_Length          = 0x2000,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description

	TKO_NURBS_Extended					= 0x0080,  //!< indicates that the 2nd byte should be written
	TKO_NURBS_Extended_Mask				= 0xFF00,  //!< mask of bits in second byte

	TKO_NURBS_Surface_Max_Facet_Angle	= 0x0100,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Surface_Max_Facet_Deviation
										= 0x0200,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Surface_Max_Trim_Curve_Deviation
										= 0x0400,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description

	TKO_NURBS_Curve_Mask                = 0xF00F,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description
	TKO_NURBS_Surface_Mask              = 0x0FF0,    //!< type for NURBS curve options; refer to ::HC_Set_Rendering_Options for description

	TKO_Tessellation_Cylinder           = 0x01,   //!< type for cylinder tesselation value; refer to ::HC_Set_Rendering_Options for description
	TKO_Tessellation_Sphere             = 0x02,   //!< type for sphere tesselation value; refer to ::HC_Set_Rendering_Options for description

	TKO_Geometry_Options_Dihedral		= 0x01,   //!< hard edge angle limit; refer to ::HC_Set_Rendering_Options for description
	TKO_Geometry_Options_Reverse_PolyCylinder_Radii		= 0x02,   //!< hard edge angle limit; refer to ::HC_Set_Rendering_Options for description
	TKO_Geometry_Options_No_Reverse_PolyCylinder_Radii	= 0x04,   //!< hard edge angle limit; refer to ::HC_Set_Rendering_Options for description
	TKO_Geometry_Options_Reverse_PolyCylinder_Colors	= 0x08,   //!< hard edge angle limit; refer to ::HC_Set_Rendering_Options for description
	TKO_Geometry_Options_No_Reverse_PolyCylinder_Colors	= 0x10    //!< hard edge angle limit; refer to ::HC_Set_Rendering_Options for description
};

/*! \enum TKO_Attribute_Lock_Bits
  Represents attribute lock types for various attributes; refer to the 'attribute lock' section of ::HC_Set_Rendering_Options for a description
*/
enum TKO_Attribute_Lock_Bits {
	TKO_Lock_Callback                   = 0x00000001,  //!< self-explanatory
	TKO_Lock_Camera                     = 0x00000002,  //!< self-explanatory
	TKO_Lock_Color                      = 0x00000004,  //!< self-explanatory
	TKO_Lock_Color_Map                  = 0x00000008,  //!< self-explanatory
	TKO_Lock_Driver                     = 0x00000010,  //!< self-explanatory
	TKO_Lock_Driver_Options             = 0x00000020,  //!< self-explanatory
	TKO_Lock_Edge_Pattern               = 0x00000040,  //!< self-explanatory
	TKO_Lock_Edge_Weight                = 0x00000080,  //!< self-explanatory
	TKO_Lock_Face_Pattern               = 0x00000100,  //!< self-explanatory
	TKO_Lock_Handedness                 = 0x00000200,  //!< self-explanatory
	TKO_Lock_Heuristics                 = 0x00000400,  //!< self-explanatory
	TKO_Lock_Line_Pattern               = 0x00000800,  //!< self-explanatory
	TKO_Lock_Line_Weight                = 0x00001000,  //!< self-explanatory
	TKO_Lock_Marker_Size                = 0x00002000,  //!< self-explanatory
	TKO_Lock_Marker_Symbol              = 0x00004000,  //!< self-explanatory
	TKO_Lock_Metafile                   = 0x00008000,  //!< self-explanatory
	TKO_Lock_Modelling_Matrix           = 0x00010000,  //!< self-explanatory
	TKO_Lock_Rendering_Options          = 0x00020000,  //!< self-explanatory
	TKO_Lock_Selectability              = 0x00040000,  //!< self-explanatory
	TKO_Lock_Styles                     = 0x00080000,  //!< self-explanatory
	TKO_Lock_Text_Alignment             = 0x00100000,  //!< self-explanatory
	TKO_Lock_Text_Font                  = 0x00200000,  //!< self-explanatory
	TKO_Lock_Text_Path                  = 0x00400000,  //!< self-explanatory
	TKO_Lock_Text_Spacing               = 0x00800000,  //!< self-explanatory
	TKO_Lock_User_Options               = 0x01000000,  //!< self-explanatory
	TKO_Lock_User_Value                 = 0x02000000,  //!< self-explanatory
	TKO_Lock_Texture_Matrix             = 0x04000000,  //!< self-explanatory
	TKO_Lock_Visibility                 = 0x08000000,  //!< self-explanatory
	TKO_Lock_Window                     = 0x10000000,  //!< self-explanatory
	TKO_Lock_Window_Frame               = 0x20000000,  //!< self-explanatory
	TKO_Lock_Window_Pattern             = 0x40000000,  //!< self-explanatory
	TKO_Lock_All                        = 0x7FFFFFFF   //!< self-explanatory

};

/*! \enum TKO_Color_Channel_Lock_Bits
  Represents color channel lock types for geometries; refer to the 'attribute lock' section of ::HC_Set_Rendering_Options for a description
*/
enum TKO_Color_Channel_Lock_Bits {
	TKO_Lock_Channel_Diffuse        = 0x0001,	//!< self-explanatory
	TKO_Lock_Channel_Specular       = 0x0002,	//!< self-explanatory
	TKO_Lock_Channel_Mirror         = 0x0004,	//!< self-explanatory
	TKO_Lock_Channel_Transmission   = 0x0008,	//!< self-explanatory
	TKO_Lock_Channel_Emission       = 0x0010,	//!< self-explanatory
	TKO_Lock_Channel_Gloss          = 0x0020,	//!< self-explanatory
	TKO_Lock_Channel_Index          = 0x0040,	//!< color by index
	TKO_Lock_Channel_Environment    = 0x0080,	//!< env map
	TKO_Lock_Channel_Bump           = 0x0100,	//!< bump map
	TKO_Lock_Channel_ALL			= 0x01FF	//!< all
};


#if 0
class BBINFILETK_API TK_Radiosity_RayTrace_Options : public BBaseOpcodeHandler {
	protected:

	public:
		TK_Radiosity_RayTrace_Options () : BBaseOpcodeHandler (TKE_Radiosity_RayTrace_Options) {}
		~TK_Radiosity_RayTrace_Options () {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
};
#endif


//! Handles the TKE_Rendering_Options opcode.
/*!  
	TK_Rendering_Options provides support for writing/reading the TKE_Rendering_Options opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports an attribute called Rendering_Options which control how the scene is rendered.
	Refer to the HOOPS/3dGS documentation for details of this attribute.
*/
class BBINFILETK_API TK_Rendering_Options : public BBaseOpcodeHandler {
    private:
        TK_Rendering_Options(const TK_Rendering_Options&);
        TK_Rendering_Options& operator=(const TK_Rendering_Options&);

	protected:
		int             m_mask[2];  //!< internal use; specifies which rendering options are active (and hence, which are valid).  For internal use only.
		int             m_value[2]; //!< internal use; specifies what values to set for boolean options.  For internal use only.

		unsigned char   m_hsr;      //!< internal use; low half hsr, high half thsr.  For internal use only.
		unsigned char   m_tq;       //!< internal use; low half technology, high half quantization.  For internal use only.
		int             m_debug;                //!< For internal use only.
		int             m_face_displacement;    //!< For internal use only.

		float           m_fog_limits[2];        //!< For internal use only.

		int             m_lock_mask;            //!< For internal use only.
		int             m_lock_value;           //!< For internal use only.
		int             m_lock_color_mask;      //!< For internal use only.
		int             m_lock_color_value;     //!< For internal use only.
		short           m_lock_color_face_mask;      //!< For internal use only.
		short           m_lock_color_face_value;     //!< For internal use only.
		short           m_lock_color_edge_mask;      //!< For internal use only.
		short           m_lock_color_edge_value;     //!< For internal use only.
		short           m_lock_color_line_mask;      //!< For internal use only.
		short           m_lock_color_line_value;     //!< For internal use only.
		short           m_lock_color_marker_mask;      //!< For internal use only.
		short           m_lock_color_marker_value;     //!< For internal use only.
		short           m_lock_color_text_mask;      //!< For internal use only.
		short           m_lock_color_text_value;     //!< For internal use only.
		short           m_lock_color_window_mask;      //!< For internal use only.
		short           m_lock_color_window_value;     //!< For internal use only.
		short           m_lock_color_face_contrast_mask;      //!< For internal use only.
		short           m_lock_color_face_contrast_value;     //!< For internal use only.
		short           m_lock_color_window_contrast_mask;      //!< For internal use only.
		short           m_lock_color_window_contrast_value;     //!< For internal use only.
		short           m_lock_color_back_mask;      //!< For internal use only.
		short           m_lock_color_back_value;     //!< For internal use only.
		short           m_lock_color_vertex_mask;      //!< For internal use only.
		short           m_lock_color_vertex_value;     //!< For internal use only.
		short           m_lock_color_edge_contrast_mask;      //!< For internal use only.
		short           m_lock_color_edge_contrast_value;     //!< For internal use only.
		short           m_lock_color_line_contrast_mask;      //!< For internal use only.
		short           m_lock_color_line_contrast_value;     //!< For internal use only.
		short           m_lock_color_marker_contrast_mask;      //!< For internal use only.
		short           m_lock_color_marker_contrast_value;     //!< For internal use only.
		short           m_lock_color_vertex_contrast_mask;      //!< For internal use only.
		short           m_lock_color_vertex_contrast_value;     //!< For internal use only.
		short           m_lock_color_text_contrast_mask;      //!< For internal use only.
		short           m_lock_color_text_contrast_value;     //!< For internal use only.
		short           m_lock_color_simple_reflection_mask;      //!< For internal use only.
		short           m_lock_color_simple_reflection_value;     //!< For internal use only.
		short           m_lock_color_cut_face_mask;      //!< For internal use only.
		short           m_lock_color_cut_face_value;     //!< For internal use only.
		short           m_lock_color_cut_edge_mask;      //!< For internal use only.
		short           m_lock_color_cut_edge_value;     //!< For internal use only.
		int             m_lock_visibility_mask; //!< For internal use only.
		int             m_lock_visibility_value;//!< For internal use only.

		unsigned char   m_buffer_options_mask;  //!< For internal use only.
		unsigned char   m_buffer_options_value; //!< For internal use only.
		int             m_buffer_size_limit;    //!< For internal use only.

		int				m_hlr_options;          //!< For internal use only.
		float           m_hlr_dim_factor;       //!< For internal use only.
		float           m_hlr_face_displacement;//!< For internal use only.
		int             m_hlr_line_pattern;     //!< For internal use only.
		float			m_hlr_color[3];		//!< for internal use only.
		float           m_hlr_weight;		//!< for internal use only.
		unsigned char   m_hlr_weight_units;	//!< for internal use only.
		unsigned char   m_hlr_hsr_algorithm;	//!< for internal use only.

		int             m_lod_options_mask;     //!< For internal use only.
		int             m_lod_options_value;    //!< For internal use only.
		char            m_lod_algorithm;        //!< For internal use only.
		char            m_num_ratios;           //!< For internal use only.
		float           m_ratio[8];             //!< For internal use only.
		char            m_num_thresholds;       //!< For internal use only.
		float           m_threshold[8];         //!< For internal use only.     
		char            m_threshold_type;       //!< For internal use only.     
		int             m_min_triangle_count;   //!< For internal use only.
		unsigned char   m_clamp;                //!< For internal use only.
		unsigned char   m_num_levels;           //!< For internal use only.
		int             m_max_degree;           //!< For internal use only.
		float           m_tolerance;            //!< For internal use only.
		float           m_bounding[6];          //!< For internal use only. 
		char            m_num_cutoffs;          //!< For internal use only.
		float           m_cutoff[8];            //!< For internal use only.
		unsigned char   m_heuristic;            //!< For internal use only.
		unsigned char   m_fallback;             //!< For internal use only.

		int				m_nurbs_options_mask;   //!< For internal use only.
		int				m_nurbs_options_value;  //!< For internal use only.
		int             m_curve_budget;         //!< For internal use only.
		int             m_curve_continued_budget;//!< For internal use only.
		int             m_surface_budget;       //!< For internal use only.
		int             m_surface_trim_budget;  //!< For internal use only.
		float			m_surface_max_trim_curve_deviation;//!< For internal use only.
		float			m_surface_max_facet_angle;//!< For internal use only.
		float			m_surface_max_facet_deviation;//!< For internal use only.
		float			m_surface_max_facet_width;//!< For internal use only.
		float			m_curve_max_angle;		//!< For internal use only.
		float			m_curve_max_deviation;	//!< For internal use only.
		float			m_curve_max_length;		//!< For internal use only.

		float           m_stereo_separation;    //!< For internal use only.
		float           m_stereo_distance;    //!< For internal use only.

		unsigned char   m_tessellations;        //!< For internal use only.
		char            m_num_cylinder;         //!< For internal use only.
		char            m_cylinder[8];          //!< For internal use only.
		char            m_num_sphere;           //!< For internal use only.
		char            m_sphere[8];            //!< For internal use only.

		unsigned char   m_transparency_options; //!< internal use; low half style, high half flags

		unsigned char   m_cut_geometry;         //!< For internal use only.
		unsigned char   m_cut_geometry_level;   //!< For internal use only.
		unsigned char   m_cut_geometry_match;   //!< For internal use only.
		float           m_cut_geometry_tolerance;//!< For internal use only.

		unsigned short  m_simple_shadow;        //!< For internal use only.
		unsigned char   m_simple_shadow_blur;   //!< For internal use only.
		unsigned short	m_simple_shadow_resolution; //!< For internal use only.
		float			m_simple_shadow_plane[4];   //!< For internal use only.
		float			m_simple_shadow_light[3];   //!< For internal use only.
		float			m_simple_shadow_color[3];   //!< For internal use only.
		float			m_simple_shadow_opacity;

		unsigned char	m_shadow_map;			//!< For internal use only.
		unsigned short	m_shadow_map_resolution; //!< For internal use only.
		unsigned char	m_shadow_map_samples; //!< For internal use only.

		unsigned short	m_simple_reflection;			//!< For internal use only.
		float			m_simple_reflection_plane[4];   //!< For internal use only.
		float			m_simple_reflection_opacity;	//!< For internal use only.
		int				m_simple_reflection_blur;		//!< For internal use only.
		float			m_simple_reflection_hither;		//!< For internal use only.
		float			m_simple_reflection_yon;		//!< For internal use only.

		float           m_depth_range[2];       //!< For internal use only.
		float           m_screen_range[4];      //!< For internal use only.
		float           m_image_scale[2];       //!< For internal use only.
		unsigned char   m_mask_transform;       //!< For internal use only.

		unsigned char	m_geometry_options;		//!< For internal use only.
		float			m_dihedral;				//!< For internal use only.

		float			m_image_tint_color[3];   //!< For internal use only.
		unsigned char	m_depth_peeling_layers;  //!< For internal use only.
		float			m_depth_peeling_min_area;//!< For internal use only.

		int             m_general_displacement;    //!< For internal use only.
		int             m_join_cutoff_angle;    //!< For internal use only.
		unsigned char	m_display_list_level;    //!< For internal use only.

#if 0
		TK_Radiosity_RayTrace_Options   *m_rrt; //!< For internal use only.
#endif

	public:
		/*! constructor */
		TK_Rendering_Options ();
		~TK_Rendering_Options ();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the mask indicating which options have a setting */
		void            SetMask (int m0, int m1=0) alter    {
							m_mask[0] = m0;
							m_mask[1] = m1;
							if (m1 != 0)
								m_mask[0] |= TKO_Rendo_Extended;
						}
		/*! Returns the mask indicating which options have a setting */
		int             GetMask (int index=0) const         { return m_mask[index];             }

		/*! Sets the mask indicating which options are set on/active */
		void            SetValue (int v0, int v1=0) alter   { m_value[0] = v0; m_value[1] = v1; }
		/*! Returns the mask indicating which options are set on/active */
		int             GetValue (int index=0) const        { return m_value[index];            }

		/*! Sets the Hidden Surface Removal algorithm */
		void            SetHSR (int h) alter            { m_hsr &= 0xF0; m_hsr |= (unsigned char)h & 0x0F;  }
		/*! Returns the Hidden Surface Removal algorithm */
		int             GetHSR (void) const             { return (int)(m_hsr & 0x0F);                       }

		/*! Sets the drawing style used for transparent components */
		void            SetTransparentHSR (int t) alter { m_hsr &= 0x0F; m_hsr |= (unsigned char)t << 4;    }
		/*! Returns the drawing style used for transparent components */
		int             GetTransparentHSR (void) const  { return (int)(m_hsr >> 4);                         }

		/*! Sets the Hidden Surface Removal algorithm used for transparent components */
		void            SetTransparentStyle (int s) alter { m_transparency_options = (unsigned char)s;      }
		/*! Returns the Hidden Surface Removal algorithm used for transparent components */
		int             GetTransparentStyle (void) const  { return (int)m_transparency_options;             }

		/*! Sets the general technology used for drawing */
		void            SetTechnology (int t) alter     { m_tq &= 0xF0; m_tq |= (unsigned char)t & 0x0F;    }
		/*! Returns the general technology used for drawing */
		int             GetTechnology (void) const      { return (int)(m_tq & 0x0F);                        }

		/*! Sets the quantization methods used when displaying on color-limited devices */
		void            SetQuantization (int q) alter   { m_tq &= 0x0F; m_tq |= (unsigned char)q << 4;      }
		/*! Returns the quantization methods used when displaying on color-limited devices */
		int             GetQuantization (void) const    { return (int)(m_tq >> 4);                          }

		/*! Sets the debug options */
		void            SetDebug (int d) alter          { m_debug = d;                                      }
		/*! Returns the debug options */
		int             GetDebug (void) const           { return m_debug;                                   }

		/*! Sets the face displacement value */
		void            SetFaceDisplacement (int d) alter   { m_face_displacement = d;                      }
		/*! Returns the face displacement value */
		int             GetFaceDisplacement (void) const    { return m_face_displacement;                   }

		/*! Sets the general displacement value */
		void            SetGeneralDisplacement (int d) alter   { m_general_displacement = d;                      }
		/*! Returns the general displacement value */
		int             GetGeneralDisplacement (void) const    { return m_general_displacement;                   }

		/*! Sets the join cutoff limit value */
		void            SetJoinCutoffAngle (int d) alter   { m_join_cutoff_angle = d;						}
		/*! Returns the join cutoff limit value */
		int             GetJoinCutoffAngle (void) const    { return m_join_cutoff_angle;					}

		/*! Sets the fog limits using discrete values */
		void            SetFogLimits (float n, float f) alter   { m_fog_limits[0] = n; m_fog_limits[1] = f; }
		/*! Sets the fog limits using a float pair */
		void            SetFogLimits (float const * l) alter    { SetFogLimits (l[0], l[1]);                }
		/*! Returns the fog limits as the address of a float pair */
		float const *   GetFogLimits (void) const               { return m_fog_limits;                      }


		/*! Sets the mask indicating which attributes have a lock setting. Lock settings are specified by #TKO_Attribute_Lock_Bits. */
		void            SetLockMask (int m) alter               { m_lock_mask = m;                  }
		/*! Returns the mask indicating which attributes have a lock setting. Lock settings are specified by #TKO_Attribute_Lock_Bits. */
		int             GetLockMask (void) const                { return m_lock_mask;               }

		/*! Sets the mask indicating which attributes are locked. Lock settings are specified by #TKO_Attribute_Lock_Bits. */
		void            SetLockValue (int v) alter              { m_lock_value = v;                 }
		/*! Returns the mask indicating which attributes are locked. Lock settings are specified by #TKO_Attribute_Lock_Bits. */
		int             GetLockValue (void) const               { return m_lock_value;              }

		/*!
			Sets the mask indicating which types of geometry have a visibility lock setting. Geometry types are 
			specified by #TKO_Geometry_Bits.
		*/
		void            SetVisibilityLockMask (int m) alter     { m_lock_visibility_mask = m;       }
		/*!
			Returns the mask indicating which types of geometry have a visibility lock setting. Geometry types are 
			specified by #TKO_Geometry_Bits.
		*/
		int             GetVisibilityLockMask (void) const      { return m_lock_visibility_mask;    }

		/*!
			Sets the mask indicating which geometry visibilities are locked. Geometry types are 
			specified by #TKO_Geometry_Bits.
		*/
		void            SetVisibilityLockValue (int v) alter    { m_lock_visibility_value = v;      }
		/*!
			Returns the mask indicating which geometry visibilities are locked. Geometry types are 
			specified by #TKO_Geometry_Bits.
		*/
		int             GetVisibilityLockValue (void) const     { return m_lock_visibility_value;   }


		/*!
			Sets the mask indicating which types of geometry have a color lock setting. Geometry types are 
			specified by #TKO_Geometry_Bits.
		*/
		void            SetColorLockMask (int m) alter     { m_lock_color_mask = m;       }
		/*!
			Returns the mask indicating which types of geometry have a color lock setting. Geometry types are 
			specified by #TKO_Geometry_Bits.
		*/
		int             GetColorLockMask (void) const      { return m_lock_color_mask;    }

		/*!
			Sets the mask indicating which geometry colors are locked. Geometry types are 
			specified by #TKO_Geometry_Bits.
		*/
		void            SetColorLockValue (int v) alter    { m_lock_color_value = v;      }
		/*!
			Returns the mask indicating which geometry colors are locked. Geometry types are 
			specified by #TKO_Geometry_Bits.
		*/
		int             GetColorLockValue (void) const     { return m_lock_color_value;   }


		/*!
			Sets the mask indicating which face color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorFaceLockMask (int m) alter          { m_lock_color_face_mask = (short)m;     }
		/*!
			Returns the mask indicating which face color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorFaceLockMask (void) const           { return m_lock_color_face_mask;         }

		/*!
			Sets the value indicating which face color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorFaceLockValue (int v) alter         { m_lock_color_face_value = (short)v;    }
		/*!
			Returns the value indicating which face color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorFaceLockValue (void) const          { return m_lock_color_face_value;        }


		/*!
			Sets the mask indicating which edge color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorEdgeLockMask (int m) alter          { m_lock_color_edge_mask = (short)m;     }
		/*!
			Returns the mask indicating which edge color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorEdgeLockMask (void) const           { return m_lock_color_edge_mask;         }

		/*!
			Sets the value indicating which edge color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorEdgeLockValue (int v) alter         { m_lock_color_edge_value = (short)v;    }
		/*!
			Returns the value indicating which edge color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorEdgeLockValue (void) const          { return m_lock_color_edge_value;        }


		/*!
			Sets the mask indicating which line color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorLineLockMask (int m) alter          { m_lock_color_line_mask = (short)m;     }
		/*!
			Returns the mask indicating which line color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorLineLockMask (void) const           { return m_lock_color_line_mask;         }

		/*!
			Sets the value indicating which line color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorLineLockValue (int v) alter         { m_lock_color_line_value = (short)v;    }
		/*!
			Returns the value indicating which line color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorLineLockValue (void) const          { return m_lock_color_line_value;        }


		/*!
			Sets the mask indicating which marker color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorMarkerLockMask (int m) alter          { m_lock_color_marker_mask = (short)m;     }
		/*!
			Returns the mask indicating which marker color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorMarkerLockMask (void) const           { return m_lock_color_marker_mask;         }

		/*!
			Sets the value indicating which marker color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorMarkerLockValue (int v) alter         { m_lock_color_marker_value = (short)v;    }
		/*!
			Returns the value indicating which marker color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorMarkerLockValue (void) const          { return m_lock_color_marker_value;        }


		/*!
			Sets the mask indicating which text color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorTextLockMask (int m) alter          { m_lock_color_text_mask = (short)m;     }
		/*!
			Returns the mask indicating which text color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorTextLockMask (void) const           { return m_lock_color_text_mask;         }

		/*!
			Sets the value indicating which text color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorTextLockValue (int v) alter         { m_lock_color_text_value = (short)v;    }
		/*!
			Returns the value indicating which text color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorTextLockValue (void) const          { return m_lock_color_text_value;        }


		/*!
			Sets the mask indicating which window color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorWindowLockMask (int m) alter          { m_lock_color_window_mask = (short)m;     }
		/*!
			Returns the mask indicating which window color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorWindowLockMask (void) const           { return m_lock_color_window_mask;         }

		/*!
			Sets the value indicating which window color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorWindowLockValue (int v) alter         { m_lock_color_window_value = (short)v;    }
		/*!
			Returns the value indicating which window color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorWindowLockValue (void) const          { return m_lock_color_window_value;        }


		/*!
			Sets the mask indicating which face_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorFaceContrastLockMask (int m) alter          { m_lock_color_face_contrast_mask = (short)m;     }
		/*!
			Returns the mask indicating which face_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorFaceContrastLockMask (void) const           { return m_lock_color_face_contrast_mask;         }

		/*!
			Sets the value indicating which face_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorFaceContrastLockValue (int v) alter         { m_lock_color_face_contrast_value = (short)v;    }
		/*!
			Returns the value indicating which face_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorFaceContrastLockValue (void) const          { return m_lock_color_face_contrast_value;        }


		/*!
			Sets the mask indicating which window_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorWindowContrastLockMask (int m) alter          { m_lock_color_window_contrast_mask = (short)m;     }
		/*!
			Returns the mask indicating which window_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorWindowContrastLockMask (void) const           { return m_lock_color_window_contrast_mask;         }

		/*!
			Sets the value indicating which window_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorWindowContrastLockValue (int v) alter         { m_lock_color_window_contrast_value = (short)v;    }
		/*!
			Returns the value indicating which window_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorWindowContrastLockValue (void) const          { return m_lock_color_window_contrast_value;        }


		/*!
			Sets the mask indicating which back color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorBackLockMask (int m) alter          { m_lock_color_back_mask = (short)m;     }
		/*!
			Returns the mask indicating which back color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorBackLockMask (void) const           { return m_lock_color_back_mask;         }

		/*!
			Sets the value indicating which back color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorBackLockValue (int v) alter         { m_lock_color_back_value = (short)v;    }
		/*!
			Returns the value indicating which back color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorBackLockValue (void) const          { return m_lock_color_back_value;        }


		/*!
			Sets the mask indicating which vertex color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorVertexLockMask (int m) alter          { m_lock_color_vertex_mask = (short)m;     }
		/*!
			Returns the mask indicating which vertex color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorVertexLockMask (void) const           { return m_lock_color_vertex_mask;         }

		/*!
			Sets the value indicating which vertex color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorVertexLockValue (int v) alter         { m_lock_color_vertex_value = (short)v;    }
		/*!
			Returns the value indicating which vertex color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorVertexLockValue (void) const          { return m_lock_color_vertex_value;        }


		/*!
			Sets the mask indicating which edge_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorEdgeContrastLockMask (int m) alter          { m_lock_color_edge_contrast_mask = (short)m;     }
		/*!
			Returns the mask indicating which edge_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorEdgeContrastLockMask (void) const           { return m_lock_color_edge_contrast_mask;         }

		/*!
			Sets the value indicating which edge_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorEdgeContrastLockValue (int v) alter         { m_lock_color_edge_contrast_value = (short)v;    }
		/*!
			Returns the value indicating which edge_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorEdgeContrastLockValue (void) const          { return m_lock_color_edge_contrast_value;        }


		/*!
			Sets the mask indicating which line_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorLineContrastLockMask (int m) alter          { m_lock_color_line_contrast_mask = (short)m;     }
		/*!
			Returns the mask indicating which line_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorLineContrastLockMask (void) const           { return m_lock_color_line_contrast_mask;         }

		/*!
			Sets the value indicating which line_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorLineContrastLockValue (int v) alter         { m_lock_color_line_contrast_value = (short)v;    }
		/*!
			Returns the value indicating which line_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorLineContrastLockValue (void) const          { return m_lock_color_line_contrast_value;        }


		/*!
			Sets the mask indicating which marker_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorMarkerContrastLockMask (int m) alter          { m_lock_color_marker_contrast_mask = (short)m;     }
		/*!
			Returns the mask indicating which marker_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorMarkerContrastLockMask (void) const           { return m_lock_color_marker_contrast_mask;         }

		/*!
			Sets the value indicating which marker_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorMarkerContrastLockValue (int v) alter         { m_lock_color_marker_contrast_value = (short)v;    }
		/*!
			Returns the value indicating which marker_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorMarkerContrastLockValue (void) const          { return m_lock_color_marker_contrast_value;        }


		/*!
			Sets the mask indicating which vertex_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorVertexContrastLockMask (int m) alter          { m_lock_color_vertex_contrast_mask = (short)m;     }
		/*!
			Returns the mask indicating which vertex_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorVertexContrastLockMask (void) const           { return m_lock_color_vertex_contrast_mask;         }

		/*!
			Sets the value indicating which vertex_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorVertexContrastLockValue (int v) alter         { m_lock_color_vertex_contrast_value = (short)v;    }
		/*!
			Returns the value indicating which vertex_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorVertexContrastLockValue (void) const          { return m_lock_color_vertex_contrast_value;        }


		/*!
			Sets the mask indicating which text_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorTextContrastLockMask (int m) alter          { m_lock_color_text_contrast_mask = (short)m;     }
		/*!
			Returns the mask indicating which text_contrast color channels have a lock setting. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorTextContrastLockMask (void) const           { return m_lock_color_text_contrast_mask;         }

		/*!
			Sets the value indicating which text_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		void            SetColorTextContrastLockValue (int v) alter         { m_lock_color_text_contrast_value = (short)v;    }
		/*!
			Returns the value indicating which text_contrast color channels are locked. Color channels are 
			specified by #TKO_Color_Channel_Lock_Bits.
		*/
		int             GetColorTextContrastLockValue (void) const          { return m_lock_color_text_contrast_value;        }




		/*! Sets the mask indicating which buffer options are set. */
		void            SetBufferOptionsMask (int v) alter      { m_buffer_options_mask = (unsigned char)v;        }
		/*! Returns the mask indicating which buffer options are set. */
		int             GetBufferOptionsMask (void) const       { return m_buffer_options_mask;     }
		/*! Sets the mask indicating which buffer options are "on". */
		void            SetBufferOptionsValue (int v) alter     { m_buffer_options_value = (unsigned char) v; }
		/*! Returns the mask indicating which buffer options are "on". */
		int             GetBufferOptionsValue (void) const      { return m_buffer_options_value;    }
		/*! Sets the size limit on the software frame buffer. */
		void            SetBufferSizeLimit (int l) alter        { m_buffer_size_limit = l;          }
		/*! Returns the size limit on the software frame buffer. */
		int             GetBufferSizeLimit (void) const         { return m_buffer_size_limit;       }


		/*! Sets the eye separation for stereo viewing. */
		void            SetStereoSeparation (float s) alter     { m_stereo_separation = s;          }
		/*! Returns the eye separation for stereo viewing. */
		float           GetStereoSeparation (void) const        { return m_stereo_separation;       }
		/*! Sets the focal distance for stereo viewing. */
		void            SetStereoDistance (float d) alter     { m_stereo_distance = d;          }
		/*! Returns the focal distance for stereo viewing. */
		float           GetStereoDistance (void) const        { return m_stereo_distance;       }


		/*! Sets the mask indicating which hlr options are set. */
		void            SetHlrOptions (int o) alter {
							m_hlr_options = o;
							if ((o & TKO_Hidden_Line_Extended_Mask) != 0) {
								m_hlr_options |= TKO_Hidden_Line_Extended;
							if ((o & TKO_Hidden_Line_Extended2_Mask) != 0)
								m_hlr_options |= TKO_Hidden_Line_Extended2;
						}
			}
		/*! Returns the mask indicating which hlr options are set. */
		int             GetHlrOptions (void) const              { return m_hlr_options;             }
		/*! Sets the dim factor used when hidden lines are displayed. */
		void            SetHlrDimFactor (float d) alter         { m_hlr_dim_factor = d;             }
		/*! Returns the dim factor used when hidden lines are displayed. */
		float           GetHlrDimFactor (void) const            { return m_hlr_dim_factor;          }
		/*! Sets the displacement used for faces during hidden line calculations. */
		void            SetHlrFaceDisplacement (float d) alter  { m_hlr_face_displacement = d;      }
		/*! Returns the displacement used for faces during hidden line calculations. */
		float           GetHlrFaceDisplacement (void) const     { return m_hlr_face_displacement;   }
		/*! Sets the mask indicating which hlr options are set. */
		void            SetHlrLinePattern (int p) alter         { m_hlr_line_pattern = p;           }
		/*! Returns the mask indicating which hlr options are set. */
		int             GetHlrLinePattern (void) const          { return m_hlr_line_pattern;        }
		/*! Sets the displacement used for faces during hidden line calculations. */
		void            SetHlrFaceSortingAlgorithm (int a) alter { m_hlr_hsr_algorithm = (unsigned char)a;  }
		/*! Returns the displacement used for faces during hidden line calculations. */
		float           GetHlrFaceSortingAlgorithm (void) const { return m_hlr_hsr_algorithm;		    }


		/*! Sets the mask indicating which NURBS options are set. */
		void            SetNURBSOptionsMask (int m) alter {
							m_nurbs_options_mask = m;
							if ((m & TKO_NURBS_Extended_Mask) != 0)
								m_nurbs_options_mask |= TKO_NURBS_Extended;
						}
		/*! Returns the mask indicating which NURBS options are set. */
		int             GetNURBSOptionsMask (void) const        { return m_nurbs_options_mask;     }
		/*! Sets the mask indicating which NURBS options are "on". */
		void            SetNURBSOptionsValue (int v) alter      { m_nurbs_options_value = v;       }
		/*! Returns the mask indicating which NURBS options are "on". */
		int             GetNURBSOptionsValue (void) const       { return m_nurbs_options_value;    }
		/*! Sets the point budget for NURBS curve tessellation. */
		void            SetNURBSCurveBudget (int b) alter       { m_curve_budget = b;               }
		/*! Returns the point budget for NURBS curve tessellation. */
		int             GetNURBSCurveBudget (void) const        { return m_curve_budget;            }
		/*! Sets the point budget for NURBS curve tessellation. */
		void            SetNURBSCurveContinuedBudget (int b) alter  { m_curve_continued_budget = b;     }
		/*! Returns the point budget for NURBS curve tessellation. */
		int             GetNURBSCurveContinuedBudget (void) const   { return m_curve_continued_budget;  }
		/*! Sets the point budget for NURBS surface tessellation. */
		void            SetNURBSSurfaceBudget (int b) alter     { m_curve_budget = b;               }
		/*! Returns the point budget for NURBS surface tessellation. */
		int             GetNURBSSurfaceBudget (void) const      { return m_curve_budget;            }
		/*! Sets the point budget for NURBS trim curve tessellation. */
		void            SetNURBSSurfaceTrimBudget (int b) alter { m_surface_trim_budget = b;        }
		/*! Returns the point budget for NURBS trim curve tessellation. */
		int             GetNURBSSurfaceTrimBudget (void) const  { return m_surface_trim_budget;     }


		/*! Sets the mask indicating which LOD options are set. */
		void            SetLodOptionsMask (int v) alter         { m_lod_options_mask = v;           }
		/*! Returns the mask indicating which LOD options are set. */
		int             GetLodOptionsMask (void) const          { return m_lod_options_mask;        }
		/*! Sets the mask indicating which LOD options are "on". */
		void            SetLodOptionsValue (int v) alter        { m_lod_options_value = v;          }
		/*! Returns the mask indicating which LOD options are "on". */
		int             GetLodOptionsValue (void) const         { return m_lod_options_value;       }
		/*! Sets the type of algorithm used for LOD generation */
		void            SetLodAlgorithm (int v) alter           { m_lod_algorithm = (char)v;        }
		/*! Returns the type of algorithm used for LOD generation */
		int             GetLodAlgorithm (void) const            { return m_lod_algorithm;           }
		/*! Sets the minimum triangle count for LOD generation */
		void            SetLodMinimumTriangleCount (int v) alter    { m_min_triangle_count = v;     }
		/*! Returns the minimum triangle count for LOD generation */
		int             GetLodMinimumTriangleCount (void) const     { return m_min_triangle_count;  }
		/*! Sets the number of LOD levels to generate */
		void            SetLodNumLevels (int v) alter           { m_num_levels = (unsigned char)v;  }
		/*! Returns the number of LOD levels to generate */
		int             GetLodNumLevels (void) const            { return m_num_levels;              }
		/*! Sets the clamp level for LOD display */
		void            SetLodClamp (int v) alter               { m_clamp = (unsigned char)v;       }
		/*! Returns the clamp level for LOD display */
		int             GetLodClamp (void) const                { return m_clamp;                   }
		/*! Sets the maximum degree */
		void            SetLodMaxDegree (int v) alter           { m_max_degree = v;                 }
		/*! Returns the maximum degree */
		int             GetLodMaxDegree (void) const            { return m_max_degree;              }
		/*! Sets the tolerance for collapsing points */
		void            SetLodTolerance (float v) alter         { m_tolerance = v;                  }
		/*! Returns the tolerance for collapsing points */
		float           GetLodTolerance (void) const            { return m_tolerance;               }
		/*! Sets the fallback method for LOD display selection */
		void            SetLodFallback (int v) alter            { m_fallback = (char)v;             }
		/*! Returns the fallback method for LOD display selection */
		int             GetLodFallback (void) const             { return m_fallback;                }

		/*! Sets the explicit LOD bounding volume as discrete float values */
		void            SetLodBounding (float x1, float y1, float z1, float x2, float y2, float z2) alter {
							m_bounding[0] = x1;  m_bounding[1] = y1;  m_bounding[2] = z1;
							m_bounding[3] = x2;  m_bounding[4] = y2;  m_bounding[5] = z2;
						}
		/*! Sets the explicit LOD bounding volume using discrete float triplets */
		void            SetLodBounding (float const * s, float const * e) alter {
							SetLodBounding (s[0], s[1], s[2],  e[0], e[1], e[2]);
						}
		/*! Sets the explicit LOD bounding volume using an array of 6 floats */
		void            SetLodBounding (float const * p) alter  { SetLodBounding (&p[0], &p[3]);    }
		/*! Returns the explicit LOD bounding volume as the address of an array of six floats */
		float const *   GetLodBounding (void) const             { return m_bounding;                }

		/*! Sets a single ratio used for LOD generation */
		void            SetLodRatio (float r) alter             { m_num_ratios = 1; m_ratio[0] = r; }
		/*! Sets the ratios used for LOD generation */
		void            SetLodRatios (int c, float const * r = 0) alter {
							m_num_ratios = (char)c;
							if (r != 0) {
								int i;
								for (i=0; i<c; ++i)
									m_ratio[i] = r[i];
							}
						}
		/*! Returns the count of ratios used for LOD generation */
		int             GetLodNumRatios (void) const            { return m_num_ratios;              }
		/*! Returns the ratios used for LOD generation */
		float const *   GetLodRatios (void) const               { return m_ratio;                   }
		/*! Returns the ratios used for LOD generation as the address of a buffer which may be modified */
		float alter *   GetLodRatios (void) alter               { return m_ratio;                   }

		/*! Sets the method used to determine when a LOD is not worth generating */
		void            SetLodThresholdType (int v) alter       { m_threshold_type = (char)v; }
		/*! Returns the method used to determine when a LOD is not worth generating */
		int             GetLodThresholdType (void) const        { return m_threshold_type; }
		/*! Sets a single threshold used for LOD generation */
		void            SetLodThreshold (float r) alter             { m_num_thresholds = 1; m_threshold[0] = r;    }
		/*! Sets the thresholds used for LOD generation */
		void            SetLodThresholds (int c, float const * r = 0) alter {
							m_num_thresholds = (char)c;
							if (r != 0) {
								int i;
								for (i=0; i<c; ++i)
									m_threshold[i] = r[i];
							}
						}
		/*! Returns the count of thresholds used for LOD generation */
		int             GetLodNumThresholds (void) const            { return m_num_thresholds;              }
		/*! Returns the thresholds used for LOD generation */
		float const *   GetLodThresholds (void) const               { return m_threshold;                  }
		/*! Returns the thresholds used for LOD generation as the address of a buffer which may be modified */
		float alter *   GetLodThresholds (void) alter               { return m_threshold;                  }

		/*! Sets a single cutoff used for LOD generation */
		void            SetLodCutoff (float r) alter             { m_num_cutoffs = 1; m_cutoff[0] = r;    }
		/*! Sets the cutoffs used for LOD generation */
		void            SetLodCutoffs (int c, float const * r = 0) alter {
							m_num_cutoffs = (char)c;
							if (r != 0) {
								int i;
								for (i=0; i<c; ++i)
									m_cutoff[i] = r[i];
							}
						}
		/*! Returns the count of cutoffs used for LOD generation */
		int             GetLodNumCutoffs (void) const           { return m_num_cutoffs;             }
		/*! Returns the cutoffs used for LOD generation */
		float const *   GetLodCutoffs (void) const              { return m_cutoff;                  }
		/*! Returns the cutoffs used for LOD generation as the address of a buffer which may be modified */
		float alter *   GetLodCutoffs (void) alter              { return m_cutoff;                  }


		/*! Sets the mask indicating which tessellation options are set. */
		void            SetTessellationMask (int m) alter       { m_tessellations = (unsigned char)m;          }
		/*! Returns the mask indicating which tessellation options are set. */
		int             GetTessellationMask (void) const        { return m_tessellations;       }
		/*! Sets a single cylinder tessellation value used for surface generation */
		void            SetCylinderTessellation (int n) alter    { m_num_cylinder = (char)1; m_cylinder[0] = (char)n;    }
		/*! Sets the cylinder tessellation values used for surface generation */
		void            SetCylinderTessellations (int c, char const * n = 0) alter {
							m_num_cylinder = (char)c;
							if (n != 0) {
								int i;
								for (i=0; i<c; ++i)
									m_cylinder[i] = n[i];
							}
						}
		/*! Returns the count of cylinder tessellation values used for surface generation */
		int             GetNumCylinderTessellations (void) const { return m_num_cylinder;           }
		/*! Returns the cylinder tessellation values used for surface generation */
		char const *    GetCylinderTessellations (void) const    { return m_cylinder;               }
		/*! Returns the cylinder tessellation values for surface generation as the address of a buffer which may be modified */
		char alter *    GetCylinderTessellations (void) alter    { return m_cylinder;               }
		/*! Sets a single sphere tessellation value used for surface generation */
		void            SetSphereTessellation (int n) alter    { m_num_sphere = (char)1; m_sphere[0] = (char)n;    }
		/*! Sets the sphere tessellation values used for surface generation */
		void            SetSphereTessellations (int c, char const * n = 0) alter {
							m_num_sphere = (char)c;
							if (n != 0) {
								int i;
								for (i=0; i<c; ++i)
									m_sphere[i] = n[i];
							}
						}
		/*! Returns the count of sphere tessellation values used for surface generation */
		int             GetNumSphereTessellations (void) const { return m_num_sphere;           }
		/*! Returns the sphere tessellation values used for surface generation */
		char const *    GetSphereTessellations (void) const    { return m_sphere;               }
		/*! Returns the sphere tessellation values for surface generation as the address of a buffer which may be modified */
		char alter *    GetSphereTessellations (void) alter    { return m_sphere;               }

		/*! Sets the mask indicating which geometry options are set. */
		void            SetGeometryOptionsMask (int m) alter       { m_geometry_options = (unsigned char)m;	}
		/*! Returns the mask indicating which geometry options are set. */
		int             GetGeometryOptionsMask (void) const        { return m_geometry_options;	}

		/*! Sets the value of the hard edge angle limit. */
		void            SetHardEdgeAngle (int m) alter       { m_dihedral = (unsigned char)m;	}
		/*! Returns the hard edge angle limit. */
		float           GetHardEdgeAngle (void) const        { return m_dihedral;	}

		/*! Sets the mask indicating which transform components should be blocked. */
		void            SetMaskTransform (int m) alter      { m_mask_transform = (unsigned char)m;  }
		/*! Returns the mask indicating which transform components should be blocked. */
		int             GetMaskTransform (void) const       { return (int)m_mask_transform;         }


		/*! Sets the mask indicating which cut geometry options to use. */
		void            SetCutGeometry (int m) alter	    { m_cut_geometry = (unsigned char)m;  }
		/*! Returns the mask indicating which cut geometry options are set. */
		int             GetCutGeometry (void) const	    { return (int)m_cut_geometry;         }

		/*! Sets the cut geometry level. */
		void            SetCutGeometryLevel (int m) alter	{ m_cut_geometry_level = (unsigned char)m;  }
		/*! Returns the geometry level. */
		int             GetCutGeometryLevel (void) const	{ return (int)m_cut_geometry_level;         }

		/*! Sets the cut geometry color match option. */
		void            SetCutGeometryColorMatch (int m) alter	{ m_cut_geometry_match = (unsigned char)m;  }
		/*! Returns the cut geometry color match option. */
		int             GetCutGeometryColorMatch (void) const	{ return (int)m_cut_geometry_match;         }

		/*! Sets the cut geometry point matching tolerance. */
		void            SetCutGeometryTolerance (float m) alter	    { m_cut_geometry_tolerance = m;	}
		/*! Returns the cut geometry point matching tolerance. */
		float           GetCutGeometryTolerance (void) const	    { return m_cut_geometry_tolerance;  }


		/*! Sets the display list level. */
		void            SetDisplayListLevel (int m) alter	{ m_display_list_level = (unsigned char)m;  }
		/*! Returns the display list level. */
		int             GetDisplayListLevel (void) const	{ return (int)m_display_list_level;         }

		/*! Sets the mask indicating which simple shadow options to use. */
		void            SetSimpleShadow (int m) alter {
							m_simple_shadow = (unsigned short)m;
							if ((m & TKO_Simple_Shadow_Extended_Mask) != 0)
								m_simple_shadow |= TKO_Simple_Shadow_Extended;
						}
		/*! Returns the mask indicating which simple shadow options are set. */
		int             GetSimpleShadow (void) const	    { return (int)m_simple_shadow;         }

		/*! Sets the simple shadow blur (softness) level. */
		void            SetSimpleShadowBlur (int m) alter	    { m_simple_shadow_blur = (unsigned char)m;  }
		/*! Returns simple shadow blur (softness) level. */
		int             GetSimpleShadowBlur (void) const	    { return (int)m_simple_shadow_blur;         }

		/*! Sets the simple shadow image resolution. */
		void            SetSimpleShadowResolution (int m) alter	    { m_simple_shadow_resolution = (unsigned short)m;  }
		/*! Returns simple shadow image resolution. */
		int             GetSimpleShadowResolution (void) const	    { return (int)m_simple_shadow_resolution;         }

		/*! Sets the simple shadow light direction using discrete values */
		void            SetSimpleShadowLight (float x, float y, float z) alter {
							m_simple_shadow_light[0] = x;
							m_simple_shadow_light[1] = y;
							m_simple_shadow_light[2] = z;
						}
		/*! Sets the simple shadow light direction using a float pair */
		void            SetSimpleShadowLight (float const * l) alter    { SetSimpleShadowLight (l[0], l[1], l[2]); }
		/*! Returns the simple shadow light direction as the address of a float array */
		float const *   getSimpleShadowLight (void) const               { return m_simple_shadow_light;	}

		/*! Sets the simple shadow projection plane using discrete values */
		void            SetSimpleShadowPlane (float a, float b, float c, float d) alter {
							m_simple_shadow_plane[0] = a;
							m_simple_shadow_plane[1] = b;
							m_simple_shadow_plane[2] = c;
							m_simple_shadow_plane[3] = d;
						}
		/*! Sets the simple shadow projection plane using a float pair */
		void            SetSimpleShadowPlane (float const * p) alter    { SetSimpleShadowPlane (p[0], p[1], p[2], p[3]); }
		/*! Returns the simple shadow projection plane as the address of a float array */
		float const *   GetSimpleShadowPlane (void) const               { return m_simple_shadow_plane;	}

		/*! Sets the rgb color value as discrete floats */
		void            SetSimpleShadowColor (float r, float g, float b) alter
				{ m_simple_shadow_color[0] = r; m_simple_shadow_color[1] = g; m_simple_shadow_color[2] = b; }
		/*! Sets the rgb color value as a float triplet */
		void            SetSimpleShadowColor (float const * rgb) alter	    { SetSimpleShadowColor (rgb[0], rgb[1], rgb[2]);	}
		/*! Returns the rgb color value as the address of a float triplet */
		float const *   GetSimpleShadowColor (void) const                   { return m_simple_shadow_color;     }

		/*! Sets the simple shadow opacity. */
		void            SetSimpleShadowOpacity (float o) alter	{ m_simple_shadow_opacity = o;		}
		/*! Returns simple shadow opacity. */
		float           GetSimpleShadowOpacity (void) const	    { return m_simple_shadow_opacity;	}

  
		/*! Sets the mask indicating which shadow map options to use. */
		void            SetShadowMap (int m) alter		{ m_shadow_map = (unsigned char)m;	}
		/*! Returns the mask indicating which shadow map options are set. */
		int             GetShadowMap (void) const	    { return (int)m_shadow_map;         }

		/*! Sets the shadow map image resolution. */
		void            SetShadowMapResolution (int m) alter	{ m_shadow_map_resolution = (unsigned short)m;  }
		/*! Returns shadow map image resolution. */
		int             GetShadowMapResolution (void) const	    { return (int)m_shadow_map_resolution;         }

		/*! Sets the shadow map image samples. */
		void            SetShadowMapSamples (int m) alter		{ m_shadow_map_samples = (unsigned char)m;  }
		/*! Returns shadow map image resolution. */
		int             GetShadowMapSamples (void) const	    { return (int)m_shadow_map_samples;         }


		/*! Sets the mask indicating which simple reflection options to use. */
		void            SetSimpleReflection (int m) alter		{ m_simple_reflection = (unsigned short)m;	}
		/*! Returns the mask indicating which simple reflection options are set. */
		int             GetSimpleReflection (void) const	    { return (int)m_simple_reflection;			}

		/*! Sets the simple reflection projection plane using discrete values */
		void            SetSimpleReflectionPlane (float a, float b, float c, float d) alter {
							m_simple_reflection_plane[0] = a;
							m_simple_reflection_plane[1] = b;
							m_simple_reflection_plane[2] = c;
							m_simple_reflection_plane[3] = d;
						}
		/*! Sets the simple reflection projection plane using a float pair */
		void            SetSimpleReflectionPlane (float const * p) alter    { SetSimpleReflectionPlane (p[0], p[1], p[2], p[3]); }
		/*! Returns the simple reflection projection plane as the address of a float array */
		float const *   GetSimpleReflectionPlane (void) const               { return m_simple_reflection_plane;	}

		/*! Sets the simple reflection opacity. */
		void            SetSimpleReflectionOpacity (float o) alter	{ m_simple_reflection_opacity = o;		}
		/*! Returns simple reflection opacity. */
		float           GetSimpleReflectionOpacity (void) const	    { return m_simple_reflection_opacity;	}


		/*! Sets the depth range using discrete values */
		void            SetDepthRange (float n, float f) alter   { m_depth_range[0] = n; m_depth_range[1] = f; }
		/*! Sets the depth range using a float pair */
		void            SetDepthRange (float const * l) alter    { SetDepthRange (l[0], l[1]);                }
		/*! Returns the depth range as the address of a float pair */
		float const *   GetDepthRange (void) const               { return m_depth_range;                      }


		/*! Sets the screen range using discrete values */
		void            SetScreenRange (float l, float r, float b, float t) alter
							{ m_screen_range[0] = l; m_screen_range[1] = r; m_screen_range[2] = b; m_screen_range[3] = t; }
		/*! Sets the screen range using a float pair */
		void            SetScreenRange (float const * l) alter    { SetScreenRange (l[0], l[1], l[2], l[3]);    }
		/*! Returns the screen range as the address of a float pair */
		float const *   GetScreenRange (void) const               { return m_screen_range;                      }


		/*! Sets the image scale factors using discrete values */
		void            SetImageScale (float x, float y) alter   { m_image_scale[0] = x; m_image_scale[1] = y; }
		/*! Sets the image scale factors using a float pair */
		void            SetImageScale (float const * s) alter    { SetImageScale (s[0], s[1]);                }
		/*! Returns the image scale factors as the address of a float pair */
		float const *   GetImageScale (void) const               { return m_image_scale;                      }


		/*! Sets the rgb color value as discrete floats */
		void            SetImageTintColor (float r, float g, float b) alter
				{ m_image_tint_color[0] = r; m_image_tint_color[1] = g; m_image_tint_color[2] = b; }
		/*! Sets the rgb color value as a float triplet */
		void            SetImageTintColor (float const * rgb) alter	    { SetImageTintColor (rgb[0], rgb[1], rgb[2]);	}
		/*! Returns the rgb color value as the address of a float triplet */
		float const *   GetImageTintColor (void) const                   { return m_image_tint_color;     }

};

////////////////////

/*! \enum TKO_Heuristic_Bits
  Represents various Heuristics which typically relate to rendering hints or optimizations; refer to ::HC_Set_Heuristics for details
*/
enum TKO_Heuristic_Bits {
	TKO_Heuristic_Hidden_Surfaces       	= 0x00000001,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Backplane_Cull        	= 0x00000002,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Polygon_Handedness    	= 0x00000004,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Quick_Moves           	= 0x00000008,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Partial_Erase         	= 0x00000010,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Memory_Purge          	= 0x00000020,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Related_Select_Limit  	= 0x00000040,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Internal_Shell_Limit 		= 0x00000080,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Clipping              	= 0x00000100,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Transformations       	= 0x00000200,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Intersecting_Polygons 	= 0x00000400,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Polygon_Crossings     	= 0x00000800,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Concave_Polygons      	= 0x00001000,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Incremental_Updates   	= 0x00002000,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Selection_Sorting     	= 0x00004000,  //!< refer to ::HC_Set_Heuristics

	TKO_Heuristic_Extended              	= 0x00008000,  //!< extended bit

	TKO_Heuristic_Culling               	= 0x00010000,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Exclude_Bounding      	= 0x00020000,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Detail_Selection      	= 0x00040000,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Ordered_Drawing			= 0x00080000,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Ordered_Unit				= 0x00100000,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Ordered_Weights			= 0x00200000,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Internal_Polyline_Limit	= 0x00400000,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Ordered_Grid				= 0x00800000,  //!< refer to ::HC_Set_Heuristics

	TKO_Heuristic_Selection_Level		 	= 0x01000000,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Static				 	= 0x02000000,  //!< refer to ::HC_Set_Heuristics

	TKO_Heuristic_Internal_Select_Limit = TKO_Heuristic_Internal_Shell_Limit | TKO_Heuristic_Internal_Polyline_Limit,  //!< refer to ::HC_Set_Heuristics
	TKO_Heuristic_Extras                = TKO_Heuristic_Polygon_Handedness | TKO_Heuristic_Quick_Moves,  //!< refer to ::HC_Set_Heuristics

	TKO_Heur_Extra_Left_Handed_Polys    = 0x01,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Extra_Quick_Move_Spriting  = 0x02,			//!< refer to ::HC_Set_Heuristics

	TKO_Heur_View_Frustum_Culling		= 0x0001,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Obscuration_Culling		= 0x0002,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Extent_Culling				= 0x0004,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_View_Frustum_Culling_Off	= 0x0010,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Obscuration_Culling_Off	= 0x0020,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Extent_Culling_Off			= 0x0040,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Culling_Extended			= 0x0080,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Obscuration_Use_Octree		= 0x0100,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Maximum_Extent_Mode		= 0x0200,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Vector_Culling				= 0x0400,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Vector_Tolerance			= 0x0800,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Vector_Culling_Off			= 0x1000,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Vector_Tolerance_Off		= 0x2000,			//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Culling_Extended_Mask		= 0xFF00,			//!< refer to ::HC_Set_Heuristics

	TKO_Heur_Max_Extent_Mode_None		= 0,				//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Max_Extent_Mode_Dot		= 1,				//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Max_Extent_Mode_Bounding	= 2,				//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Max_Extent_Mode_Defer		= 3,				//!< refer to ::HC_Set_Heuristics

	TKO_Heur_Order_World_Volume		= 0,				//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Order_Screen_Extent	= 1,				//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Order_Distance			= 2,				//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Order_Divergence		= 3,				//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Order_Density			= 4,				//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Order_Priority			= 5,				//!< refer to ::HC_Set_Heuristics
	TKO_Heur_Order_Count			= 6,				//!< refer to ::HC_Set_Heuristics

	TKO_Heur_Selection_Level_Entity       = 0,   //!< refer to ::HC_Set_Heuristics for description
	TKO_Heur_Selection_Level_Segment      = 1,   //!< refer to ::HC_Set_Heuristics for description
	TKO_Heur_Selection_Level_Segment_Tree = 2,   //!< refer to ::HC_Set_Heuristics for description

	TKO_Heur_Force_Enum_Signed_Dummy_Value = -1			//!< hack: prevent some compilers from assuming enum is unsigned
};

#define TKO_Heuristic_Extended_Mask ((int)0xFFFF0000)  //!< internal use, indicates bits which require TKO_Heuristic_Extended


//! Handles the TKE_Heuristics opcode.
/*!  
	TK_Heuristics provides support for writing/reading the TKE_Heuristics opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports an attribute called Heuristics which provide instructions about 
	scene-graph optimizations or required calculations 
*/
class BBINFILETK_API TK_Heuristics : public BBaseOpcodeHandler {
    private:
        TK_Heuristics(const TK_Heuristics&);
        TK_Heuristics& operator=(const TK_Heuristics&);

	protected:
		int             m_mask;		//!< internal use
		int             m_value;	//!< internal use

		int             m_related;	//!< internal use
		int             m_internal_shell;	//!< internal use
		int             m_internal_polyline;//!< internal use

		unsigned char   m_extras;       //!< internal use; low bit set == left handed, second bit set == spriting
		unsigned short  m_culling;      //!< internal use; culling options
		int				m_pixel_threshold; //!< internal use; pixel threshold
		int				m_maximum_extent; //!< internal use; maximum extent
		int				m_maximum_extent_mode; //!< internal use; maximum extent mode
		float			m_vector[3];			//!< internal use; culling vector
		float			m_vector_tolerance;		//!< internal use; culling vector tolerance

		unsigned char	m_ordered_weights_mask;
		float			m_ordered_weights[TKO_Heur_Order_Count];
		unsigned char	m_selection_level;

	public:
		/*! constructor */
		TK_Heuristics () : BBaseOpcodeHandler (TKE_Heuristics),
								  m_mask (0), m_value (0), m_culling(0), m_pixel_threshold (0), m_maximum_extent (0), m_maximum_extent_mode(0) {}
		~TK_Heuristics ();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the mask indicating which heuristics have a setting.  Options are specified by #TKO_Heuristic_Bits. */
		void            SetMask (int m) alter {
							m_mask = m;
							if ((m & TKO_Heuristic_Extended_Mask) != 0)
								m_mask |= TKO_Heuristic_Extended;
						}
		/*! Returns the mask indicating which heuristics have a setting.  Options are specified by #TKO_Heuristic_Bits. */
		int             GetMask (void) const            { return m_mask;    }

		/*! Sets the mask indicating which heuristics are set on/active.  Options are specified by #TKO_Heuristic_Bits. */
		void            SetValue (int v) alter          { m_value = v;      }
		/*! Returns the mask indicating which heuristics are set on/active.  Options are specified by #TKO_Heuristic_Bits. */
		int             GetValue (void) const           { return m_value;   }

		/*! Sets the related selection limit */
		void            SetRelatedSelectionLimit (int r) alter  { m_related = r;      }
		/*! Returns the related selection limit */
		int             GetRelatedSelectionLimit (void) const   { return m_related;   }

		/*! Sets the internal selection limit */
		void            SetInternalSelectionLimit (int i) alter { m_internal_shell = m_internal_polyline = i;     }
		/*! Returns the internal selection limit */
		int             GetInternalSelectionLimit (void) const  { return m_internal_shell;  }

		/*! Sets the internal shell selection limit */
		void            SetInternalShellSelectionLimit (int i) alter { m_internal_shell = i;     }
		/*! Returns the internal shell selection limit */
		int             GetInternalShellSelectionLimit (void) const  { return m_internal_shell;  }

		/*! Sets the internal polyline selection limit */
		void            SetInternalPolylineSelectionLimit (int i) alter { m_internal_polyline = i;     }
		/*! Returns the internal polyline selection limit */
		int             GetInternalPolylineSelectionLimit (void) const  { return m_internal_polyline;  }

		/*! Sets the 'extra' bits.  These bits are used to indicate an alternate form for some of the options. */
		void            SetExtras (int e) alter         { m_extras = (unsigned char)e;  }
		/*! Returns the 'extra' bits. */
		int             GetExtras (void) const          { return (int)m_extras;         }

		/*! Sets the culling mode.  These bits indicate forms of scene culling which may be used. */
		void            SetCulling (int c) alter        { m_culling = (unsigned short)c; }
		/*! Returns the culling mode. */
		int             GetCulling (void) const         { return (int)m_culling;        }
		/*! Sets the minimum number of pixels that must be touched under an occlusion culling node for it to be considered for drawing */
		void            SetPixelThreshold (int c) alter        { m_pixel_threshold = c;		}
		/*! Gets the Pixel Threshold */
		int             GetPixelThreshold (void) const         { return m_pixel_threshold;	}
		/*! Sets the upper limit for extent culling */
		void            SetMaximumExtent (int c) alter         { m_maximum_extent = c;		}
		/*! Gets the Maximum Extent */
		int             GetMaximumExtent (void) const          { return m_maximum_extent;	}
		/*! Gets the Maximum Extent Mode */
		int				GetMaximumExtentMode (void) const	   { return m_maximum_extent_mode; }
		/*! Sets the Maximum Extent Mode */
		void            SetMaximumExtentMode (int c) alter         { m_maximum_extent_mode = (unsigned char)c;		}
		float const *	GetVector (void) const						{ return m_vector; }
		void			SetVector (float x, float y, float z) alter	{
							m_vector[0] = x;
							m_vector[1] = y;
							m_vector[2] = z;
 						}
		void			SetVector (float const * v) alter			{ SetVector(v[0], v[1], v[2]); }
		float			GetVectorTolerance (void) const				{ return m_vector_tolerance; }
		void			SetVectorTolerance (float tol) alter		{ m_vector_tolerance = tol; }

		/*! Sets the mask of drawing order weight values which are interesting */
		void            SetOrderedWeightsMask (int c) alter    { m_ordered_weights_mask = (unsigned char)c;	}
		/*! Gets the mask of drawing order weight values which are interesting */
		int             GetOrderedWeightsMask (void) const     { return (int)m_ordered_weights_mask;		}

		/*! Sets a specified drawing order weighting factor */
		void			SetOrderedWeight (int index, float weight) alter	{
							m_ordered_weights[index] = weight;
							m_ordered_weights_mask |= 1<<index;
						}
		/*! Return a single drawing order weighting factor */
		float			GetOrderedWeight (int index) const			{ return m_ordered_weights[index];	}
		/*! Returns the array of drawing order weighting factors */
		float const *	GetOrderedWeights (void) const				{ return m_ordered_weights;			}
		/*! Returns the array of drawing order weighting factors, which may be modified directly */
		float alter *	GetOrderedWeights (void) alter				{ return m_ordered_weights;			}

		/*! Sets the selection level mode. */
		void            SetSelectionLevel (int l) alter        { m_selection_level = (unsigned char)l; }
		/*! Returns the selection level mode. */
		int             GetSelectionLevel (void) const         { return (int)m_selection_level;        }
};

////////////////////
//! Handles the TKE_Visibility opcode.
/*!  
	TK_Visibility provides support for writing/reading the TKE_Visibility opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports an attribute called Visibility which affects the display of objects.
*/
class BBINFILETK_API TK_Visibility : public BBaseOpcodeHandler {
	protected:
		int             m_mask;     //!< internal use; specifies which visibility settings are active (and hence, which are valid).  For internal use only.
		int             m_value;    //!< internal use; specifies what values to set for boolean options.  For internal use only.

	public:
		/*! constructor */
		TK_Visibility (void)
			: BBaseOpcodeHandler (TKE_Visibility), m_mask (0), m_value (0) {}
        TK_Visibility(const TK_Visibility& rVis) : BBaseOpcodeHandler(TKE_Visibility) 
        {
            SetGeometry( rVis.GetGeometry() );
            SetValue( rVis.GetValue() );
        }
        TK_Visibility& operator=(const TK_Visibility& rVis)
        {
            if(&rVis != this)
            {
                m_opcode = rVis.m_opcode;
                SetGeometry( rVis.GetGeometry() );
                SetValue( rVis.GetValue() );
            }
            return *this;
        }


		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		/*!
			Sets the mask indicating which geometries have a visibility setting.  Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		void            SetGeometry (int m) alter {
							m_mask = m & TKO_Geo_All_Visibles;
							if ((m & TKO_Geo_Extended_Mask) != 0) {
								m_mask |= TKO_Geo_Extended;
							if ((m & TKO_Geo_Extended2_Mask) != 0)
								m_mask |= TKO_Geo_Extended2;
							}
						}
		/*!
			Returns the mask indicating which geometries have a visibility setting.  Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		int             GetGeometry (void) const        { return m_mask;    }

		/*!
			Sets the mask indicating which geometries have visibility set on.  Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		void            SetValue (int m) alter          { m_value = m;       }
		/*!
			Returns the mask indicating which geometries have visibility set on.  Geometry types are 
			specified by TKO_Geometry_Bits.
		*/
		int             GetValue (void) const           { return m_value;    }
};

////////////////////
//! Handles the TKE_Selectability opcode.
/*!  
	TK_Selectability provides support for writing/reading the TKE_Selectability opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports an attribute called Selectability which affects the selection of objects.
	HOOPS' selection is kicked off with an ::HC_Compute_Selection_By_[something], at which point one of several possible
	actions is given.  Selectability attributes in HOOPS/3dGS allow for separate settings for each of those actions.
*/
class BBINFILETK_API TK_Selectability : public BBaseOpcodeHandler {
    private:
        TK_Selectability(const TK_Selectability&);
        TK_Selectability& operator=(const TK_Selectability&);

	protected:
		int             m_mask;         //!< internal use; specifies which selectability settings are active (and hence, which are valid).  For internal use only.
		int             m_down;         //!< internal use; specifies what geometry is selectable on mouse button down.  For internal use only.
		int             m_up;           //!< internal use; specifies what geometry is selectable on mouse button up.  For internal use only.
		int             m_move_down;    //!< internal use; specifies what geometry is selectable on mouse button down and move.  For internal use only.
		int             m_move_up;      //!< internal use; specifies what geometry is selectable on mouse move without buttons down.  For internal use only.
		int             m_invisible;    //!< internal use; specifies what geometry is selectable even when invisible.  For internal use only.

	public:
		/*! constructor */
		TK_Selectability (void)
			: BBaseOpcodeHandler (TKE_Selectability),
			m_mask (0), m_down (0), m_up (0), m_move_down (0), m_move_up (0), m_invisible (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		/*!
			Sets the mask indicating which geometries have a selectability setting.  Geometry types are 
			specified by #TKO_Geometry_Bits.
		*/
		void            SetGeometry (int m) alter {
							m_mask = m & TKO_Geo_All_Selects;
							if ((m & TKO_Geo_Extended_Mask) != 0)
								m_mask |= TKO_Geo_Extended;
						}
		/*!
			Returns the mask indicating which geometries have a selectability setting.  Geometry types are 
			specified by #TKO_Geometry_Bits.
		*/
		int             GetGeometry (void) const        { return m_mask;        }

		/*!
			Sets the mask indicating which geometries are selected by a downward transition
			of the pointer (mouse click).  Geometry types are specified by #TKO_Geometry_Bits.
		*/
		void            SetDown (int m) alter           { m_down = m;           }
		/*!
			Returns the mask indicating which geometries have selectability set "down".
			Geometry types are specified by #TKO_Geometry_Bits.
		*/
		int             GetDown (void) const            { return m_down;        }

		/*!
			Sets the mask indicating which geometries are selected by an upward transition
			of the pointer (mouse release).  Geometry types are specified by #TKO_Geometry_Bits.
		*/
		void            SetUp (int m) alter             { m_up = m;             }
		/*!
			Returns the mask indicating which geometries have selectability set "up".
			Geometry types are specified by #TKO_Geometry_Bits.
		*/
		int             GetUp (void) const              { return m_up;          }

		/*!
			Sets the mask indicating which geometries are selected by motion of the pointer
			in the "down" state.  Geometry types are specified by #TKO_Geometry_Bits.
		*/
		void            SetMoveDown (int m) alter       { m_move_down = m;      }
		/*!
			Returns the mask indicating which geometries have selectability set "move down".
			Geometry types are specified by #TKO_Geometry_Bits.
		*/
		int             GetMoveDown (void) const        { return m_move_down;   }

		/*!
			Sets the mask indicating which geometries are selected by motion of the pointer
			in the "up" state.  Geometry types are specified by #TKO_Geometry_Bits.
		*/
		void            SetMoveUp (int m) alter         { m_move_up = m;        }
		/*!
			Returns the mask indicating which geometries have selectability set "move up".
			Geometry types are specified by #TKO_Geometry_Bits.
		*/
		int             GetMoveUp (void) const          { return m_move_up;     }

		/*!
			Sets the mask indicating which geometries are selectable even when not visible.
			Geometry types are specified by #TKO_Geometry_Bits.
		*/
		void            SetWhenInvisible (int m) alter  { m_invisible = m;      }
		/*!
			Returns the mask indicating which geometries are selectable when not visible.
			Geometry types are specified by #TKO_Geometry_Bits.
		*/
		int             GetWhenInvisible (void) const   { return m_invisible;   }
};

//! Handles the TKE_Modelling_Matrix and TKE_Texture_Matrix opcodes.
/*!  
	TK_Matrix provides support for writing/reading the TKE_Modelling_Matrix and TKE_Texture_Matrix opcode objects 
	to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports modelling and texture matrices.
*/
class BBINFILETK_API TK_Matrix : public BBaseOpcodeHandler {
	protected:
		float           m_matrix[16];  //!< internal use

	public:
		/*! constructor */
		TK_Matrix (unsigned char opcode)
			: BBaseOpcodeHandler (opcode) {}
        TK_Matrix(const TK_Matrix& rMatrix) : BBaseOpcodeHandler(rMatrix.m_opcode)
        {
            SetMatrix( rMatrix.m_matrix );
        }

        TK_Matrix& operator=(const TK_Matrix& rMatrix)
        {
            if(&rMatrix != this)
            {
                m_opcode = rMatrix.m_opcode;
                SetMatrix( rMatrix.m_matrix );
            }
            return *this;
        }


		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the matrix to the specified values */
		void            SetMatrix (float const * m) alter   {
							int i;  for (i=0; i<16; i++) m_matrix[i] = m[i];
						}
		/*! Returns the matrix as the address of the array of floats */
		float const *   GetMatrix (void) const              { return m_matrix;  }
		/*! Returns the matrix as the address of the array of floats, which may be modified directly */
		float alter *   GetMatrix (void) alter              { return m_matrix;  }
};


/*! \enum TKO_Enumerations
		Defines the values for the TK_Enumerated forms used by the HOOPS 3DGS
*/
enum TKO_Enumerations {
	TKO_Line_Pattern_Solid      = 0,  //!< refer to ::HC_Set_Line_Pattern
	TKO_Line_Pattern_Dash_Dot   = 1,  //!< refer to ::HC_Set_Line_Pattern
	TKO_Line_Pattern_Dashed     = 2,  //!< refer to ::HC_Set_Line_Pattern
	TKO_Line_Pattern_Dotted     = 3,  //!< refer to ::HC_Set_Line_Pattern
	TKO_Line_Pattern_Dash_2Dot  = 4,  //!< refer to ::HC_Set_Line_Pattern
	TKO_Line_Pattern_Dash_3Dot  = 5,  //!< refer to ::HC_Set_Line_Pattern
	TKO_Line_Pattern_Long_Dash  = 6,  //!< refer to ::HC_Set_Line_Pattern
	TKO_Line_Pattern_Center     = 7,  //!< refer to ::HC_Set_Line_Pattern
	TKO_Line_Pattern_Phantom    = 8,  //!< refer to ::HC_Set_Line_Pattern
	TKO_Line_Pattern_Find_Dots  = 9,  //!< refer to ::HC_Set_Line_Pattern

	TKO_Fill_Pattern_Hash       = 0,  //!< refer to ::HC_Set_Face_Pattern
	TKO_Fill_Pattern_Vertical   = 1,  //!< refer to ::HC_Set_Face_Pattern
	TKO_Fill_Pattern_Horizontal = 2,  //!< refer to ::HC_Set_Face_Pattern
	TKO_Fill_Pattern_Right      = 3,  //!< refer to ::HC_Set_Face_Pattern
	TKO_Fill_Pattern_Left       = 4,  //!< refer to ::HC_Set_Face_Pattern
	TKO_Fill_Pattern_Diamond    = 5,  //!< refer to ::HC_Set_Face_Pattern
	TKO_Fill_Pattern_Dots       = 6,  //!< refer to ::HC_Set_Face_Pattern
	TKO_Fill_Pattern_Boxes      = 7,  //!< refer to ::HC_Set_Face_Pattern
	TKO_Fill_Pattern_Solid      = 8,  //!< refer to ::HC_Set_Face_Pattern
	TKO_Fill_Pattern_Clear      = 9,  //!< refer to ::HC_Set_Window_Pattern
	TKO_Fill_Pattern_Gradient_N = 10,  //!< refer to ::HC_Set_Window_Pattern
	TKO_Fill_Pattern_Gradient_NE= 11,  //!< refer to ::HC_Set_Window_Pattern
	TKO_Fill_Pattern_Gradient_E = 12,  //!< refer to ::HC_Set_Window_Pattern
	TKO_Fill_Pattern_Gradient_SE= 13,  //!< refer to ::HC_Set_Window_Pattern
	TKO_Fill_Pattern_Gradient_S = 14,  //!< refer to ::HC_Set_Window_Pattern
	TKO_Fill_Pattern_Gradient_SW= 15,  //!< refer to ::HC_Set_Window_Pattern
	TKO_Fill_Pattern_Gradient_W = 16,  //!< refer to ::HC_Set_Window_Pattern
	TKO_Fill_Pattern_Gradient_NW= 17,  //!< refer to ::HC_Set_Window_Pattern
	TKO_Fill_Pattern_Blend	= 18,  //!< refer to ::HC_Set_Window_Pattern
	TKO_Fill_Pattern_Invisible	= 19,  //!< refer to ::HC_Set_Window_Pattern

	TKO_Marker_Circle                       = 0,  //!< refer to ::HC_Set_Marker_Symbol
	TKO_Marker_Circle_Dot                   = 1,  //!< ""
	TKO_Marker_Circle_Plus                  = 2,  //!< ""
	TKO_Marker_Circle_X                     = 3,  //!< ""
	TKO_Marker_Circle_Circle                = 4,  //!< ""
	TKO_Marker_Circle_Filled                = 5,  //!< ""
	TKO_Marker_Dot                          = 6,  //!< ""
	TKO_Marker_Plus                         = 7,  //!< ""
	TKO_Marker_X                            = 8,  //!< ""
	TKO_Marker_Star                         = 9,  //!< ""
	TKO_Marker_Box                          = 10,  //!< ""
	TKO_Marker_Box_Dot                      = 11,  //!< ""
	TKO_Marker_Box_X                        = 12,  //!< ""
	TKO_Marker_Box_Filled                   = 13,  //!< ""
	TKO_Marker_Diamond                      = 14,  //!< ""
	TKO_Marker_Diamond_Dot                  = 15,  //!< ""
	TKO_Marker_Diamond_Plus                 = 16,  //!< ""
	TKO_Marker_Diamond_Filled               = 17,  //!< ""
	TKO_Marker_Triangle_Up                  = 18,  //!< ""
	TKO_Marker_Triangle_Up_Vertex           = 19,  //!< ""
	TKO_Marker_Triangle_Up_Dot              = 20,  //!< ""
	TKO_Marker_Triangle_Up_Filled           = 21,  //!< ""
	TKO_Marker_Triangle_Up_Filled_Vertex    = 22,  //!< ""
	TKO_Marker_Triangle_Down                = 23,  //!< ""
	TKO_Marker_Triangle_Down_Vertex         = 24,  //!< ""
	TKO_Marker_Triangle_Down_Dot            = 25,  //!< ""
	TKO_Marker_Triangle_Down_Filled         = 26,  //!< ""
	TKO_Marker_Triangle_Down_Filled_Vertex  = 27,  //!< ""
	TKO_Marker_Triangle_Right               = 28,  //!< ""
	TKO_Marker_Triangle_Right_Vertex        = 29,  //!< ""
	TKO_Marker_Triangle_Right_Dot           = 30,  //!< ""
	TKO_Marker_Triangle_Right_Filled        = 31,  //!< ""
	TKO_Marker_Triangle_Right_Filled_Vertex = 32,  //!< ""
	TKO_Marker_Triangle_Left                = 33,  //!< ""
	TKO_Marker_Triangle_Left_Vertex         = 34,  //!< ""
	TKO_Marker_Triangle_Left_Dot            = 35,  //!< ""
	TKO_Marker_Triangle_Left_Filled         = 36,  //!< ""
	TKO_Marker_Triangle_Left_Filled_Vertex  = 37,  //!< ""
	TKO_Marker_Hash                         = 38,  //!< ""
	TKO_Marker_Wide_Plus                    = 39,  //!< ""
	TKO_Marker_Open_Arrow                   = 40,  //!< ""
	TKO_Marker_Closed_Arrow                 = 41,  //!< ""
	TKO_Marker_Vertical_Bar                 = 42,  //!< ""
	TKO_Marker_Half_Arrow_Left              = 43,  //!< ""
	TKO_Marker_Half_Arrow_Right             = 44,  //!< ""
	TKO_Marker_Wide_Arrow                   = 45,  //!< ""
	TKO_Marker_Double_Arrow                 = 46,  //!< ""
	TKO_Marker_Y                            = 47,  //!< ""
	TKO_Marker_Z                            = 48,  //!< ""

	TKO_Text_Alignment_Lower_Left       = 0,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Upper_Left       = 1,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Middle_Left      = 2,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Lower_Right      = 3,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Upper_Right      = 4,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Middle_Right     = 5,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Lower_Center     = 6,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Upper_Center     = 7,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Middle_Center    = 8,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Insertion_Left   = 9,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Insertion_Right  = 10, //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Insertion_Center = 11, //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Alignment_Insertion        = 9,  //!< refer to ::HC_Set_Text_Alignment

	TKO_Text_Justification_Unspecified  = 0,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Justification_Left         = 1,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Justification_Center       = 2,  //!< refer to ::HC_Set_Text_Alignment
	TKO_Text_Justification_Right        = 3,  //!< refer to ::HC_Set_Text_Alignment

	TKO_Window_Frame_Off    = 0,  //!< refer to ::HC_Set_Visibility
	TKO_Window_Frame_On     = 1,  //!< refer to ::HC_Set_Visibility

	TKO_Handedness_Left     = 0,  //!< refer to ::HC_Set_Rendering_Options
	TKO_Handedness_Right    = 1  //!< refer to ::HC_Set_Rendering_Options
};

/*!  
	TK_Enumerated provides support for writing/reading opcode objects which represent a simple choice from an
	enumerated set (#TKO_Enumerations). This currently includes the opcodes:
		TKE_Face_Pattern, TKE_Window_Pattern, TKE_Text_Alignment, TKE_Window_Frame, TKE_Handedness
*/
class BBINFILETK_API TK_Enumerated : public BBaseOpcodeHandler {
    private:
        TK_Enumerated(const TK_Enumerated&);
        TK_Enumerated& operator=(const TK_Enumerated&);

	protected:
		char            m_index;	//!< internal use 

	public:
		/*! constructor */
		TK_Enumerated (unsigned char opcode)
			: BBaseOpcodeHandler (opcode), m_index (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		/*! Sets the index value */
		void            SetIndex (int i) alter  { m_index = (char)i;   }
		/*! Returns the index value */
		int             GetIndex (void) const   { return (int)m_index;  }
};

/*! \enum TKO_Generic_Size_Units
		Defines the units used for general sizing; refer to ::HC_Compute_Coordinates for detais
*/
enum TKO_Generic_Size_Units {
	TKO_Generic_Size_Object,       //!< Object Space
	TKO_Generic_Size_Screen,       //!< Screen Space
	TKO_Generic_Size_Window,       //!< Window Space
	TKO_Generic_Size_Points,       //!< Points 
	TKO_Generic_Size_Pixels,       //!< Pixels 
	TKO_Generic_Size_Percent,	   //!< only used by certain handlers
	TKO_Generic_Size_Unspecified   //!< self-explanatory; (internal note: keep this listed last)
};

/*!  
	TK_Size provides support for writing/reading opcode objects which represent a single value representing
	a scale or size, possibly including a specification of the reference system. This includes opcodes such
	as TKE_Line_Weight, TKE_Edge_Weight, TKE_Marker_Size and TKE_Text_Spacing
*/
class BBINFILETK_API TK_Size : public BBaseOpcodeHandler {
    private:
        TK_Size(const TK_Size&);
        TK_Size& operator=(const TK_Size&);

	protected:
		float           m_value;  //!< for internal use only.
		unsigned char   m_units;  //!< for internal use only.

	public:
		/*! constructor */
		TK_Size (unsigned char opcode)
			: BBaseOpcodeHandler (opcode), m_value (0.0f), m_units (TKO_Generic_Size_Unspecified) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the size value & units (value may not be negative, units only meaningful if positive) */
		void            SetSize (float value, int units = TKO_Generic_Size_Unspecified) alter {
							m_value = (value > 0.0f) ? value : 0.0f;
							m_units = (m_value > 0.0f) ? (unsigned char) units : (unsigned char) TKO_Generic_Size_Unspecified;
						}
		/*! Returns the size value */
		float           GetSize (void) const    { return m_value;  }
		/*! Returns the size value */
		int             GetUnits (void) const   { return m_units;  }
};

/*!  
	TK_Linear_Pattern provided support for writing/reading the TKE_Line_Pattern and TKE_Edge_Pattern opcode objects,
	which represent a composite of the dashing pattern, cap style, and join style.  This has been superceded by TK_Named.
*/
class BBINFILETK_API TK_Linear_Pattern : public BBaseOpcodeHandler {
    private:
        TK_Linear_Pattern(const TK_Linear_Pattern&);
        TK_Linear_Pattern& operator=(const TK_Linear_Pattern&);

	protected:
		unsigned short  m_pattern;	//!< internal use 

	public:
		/*! constructor */
		TK_Linear_Pattern (unsigned char opcode)
			: BBaseOpcodeHandler (opcode), m_pattern (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the pattern value */
		void            SetPattern (int p) alter    { m_pattern = (unsigned short)p;    }
		/*! Returns the pattern value */
		int             GetPattern (void) const     { return (int)m_pattern;            }
};

/*!  
	TK_Named provides support for writing/reading opcode objects which represent a simple string. For compatibility, it
		also incorporates forms for certain recognised values.  This currently includes the opcodes:
		TKE_Marker_Symbol, TKE_Line_Pattern, TKE_Edge_Pattern
*/
class BBINFILETK_API TK_Named : public BBaseOpcodeHandler {
private:
        TK_Named( const TK_Named& );
        TK_Named& operator=(const TK_Named&);

	protected:
	int		m_name_length;	//!< internal use: length of name
	char *		m_name;		//!< internal use: name
		int		m_index;	//!< internal use: simple value for recognised old forms

	public:
		/*! constructor */
		TK_Named (unsigned char opcode)
			: BBaseOpcodeHandler (opcode), m_name_length (0), m_name (0), m_index (0) {}
		~TK_Named();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the name.  Allocates the buffer and copies the name string */
		void            SetName (char const * name) alter;
		/*! Sets the name buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetName (int length) alter;
		/*! Returns the name */
		char const *    GetName (void) const	    { return m_name;		}
		/*! Returns the name buffer, which can be modified directly */
		char alter *    GetName (void) alter	    { return m_name;		}

		/*! Sets the index value */
		void            SetIndex (int i) alter  { Reset(); m_index = (char)i;   }
		/*! Returns the index value */
		int             GetIndex (void) const   { return (int)m_index;		}
};



/*!  
	TK_Streaming provides support for writing/reading the TKE_Streaming_Mode opcode.

	The HOOPS/3dGS scene-graph supports a "streaming mode" in which the changes to a segment are treated
	as a procedural series instead of the normal mode where changes affect the state of the segment.
*/
class BBINFILETK_API TK_Streaming : public BBaseOpcodeHandler {
    private:
        TK_Streaming(const TK_Streaming&);
        TK_Streaming& operator=(const TK_Streaming&);

	protected:
		bool            m_flag;  //!< for internal use only

	public:
		/*! constructor */
		TK_Streaming () : BBaseOpcodeHandler (TKE_Streaming_Mode) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		/*! Sets the flag indicating streaming mode */
		void		SetStreaming (bool s) alter { m_flag = s;       }
		/*! Returns the flag indicating streaming mode */
		bool		GetStreaming (void) const   { return m_flag;    }
};

////////////////////////////////////////////////////////////////////////////////
//! Handles the TKE_Conditions opcode.
/*!  
	TK_Conditions provides support for writing/reading the TKE_Conditions opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports conditional use of stye & include segments.  A condition is simply a string
	containing a comma-separated list of condition flags being turned on or off
*/
class BBINFILETK_API TK_Conditions : public BBaseOpcodeHandler {
private:
        TK_Conditions( const TK_Conditions& );
        TK_Conditions& operator=(const TK_Conditions&);
	protected:
		int                     m_length;       /*!< Length of text string */
		char *                  m_string;       /*!< Text string */

	public:
		/*! constructor */
		TK_Conditions () : BBaseOpcodeHandler (TKE_Conditions), m_length (0), m_string (0) {}
		~TK_Conditions();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the conditions string.  Allocates the buffer and copies the string */
		void            SetConditions (char const * options) alter;
		/*! Sets the conditions string buffer.  Allocates a buffer which can hold a srting of 'length' characters */
		void            SetConditions (int length) alter;
		/*! Returns the conditions string. */
		char const *    GetConditions (void) const                 { return m_string;          }
		/*! Returns the conditions string buffer, which may be modified directly */
		char alter *    GetConditions (void) alter                 { return m_string;          }
		/*! Returns the size of the string buffer (number of chars, not including any space for trailing null) */
		int             GetLength(void) alter                   { return m_length;          }
};

////////////////////////////////////////////////////////////////////////////////
//! Handles the TKE_User_Options opcode.
/*!  
	TK_User_Options provides support for writing/reading the TKE_User_Options opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports storing non-graphical data per segment.  A user-option is simply a string
	containing a comma-separated set of string pairs.
*/
class BBINFILETK_API TK_User_Options : public BBaseOpcodeHandler {
    private:
        TK_User_Options(const TK_User_Options&);
        TK_User_Options& operator=(const TK_User_Options&);

	protected:
		int                     m_length;       /*!< Length of text string */
		char *                  m_string;       /*!< Text string */
		BBaseOpcodeHandler *    m_indices;      /*!< Index handler */
		BBaseOpcodeHandler *    m_unicode;      /*!< Unicode handler */

		void    set_options (char const * options) alter;       //!< for internal use only
		void    set_options (int length) alter;                 //!< for internal use only

	public:
		/*! constructor */
		TK_User_Options () : BBaseOpcodeHandler (TKE_User_Options), m_length (0), m_string (0),
							 m_indices (0), m_unicode (0) {}
		~TK_User_Options();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the user options string.  Allocates the buffer and copies the string */
		void            SetOptions (char const * options) alter { set_options (options);    }
		/*! Sets the user options string buffer.  Allocates a buffer which can hold a srting of 'length' characters */
		void            SetOptions (int length) alter           { set_options (length);     }
		/*! Returns the user options string. */
		char const *    GetOptions (void) const                 { return m_string;          }
		/*! Returns the user options string buffer, which may be modified directly */
		char alter *    GetOptions (void) alter                 { return m_string;          }
		/*! Returns the size of the string buffer (number of chars, not including any space for trailing null) */
		int             GetLength(void) alter                   { return m_length;          }
};

////////////////////////////////////////////////////////////////////////////////
//! Handles the TKE_Unicode_Options opcode.
/*!  
	TK_User_Options provides support for writing/reading the TKE_Unicode_Options opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports storing non-graphical data per segment.  A unicode-option is simply a
	wide character (2-bytes per character) string containing a comma-separated set of string pairs.
*/
class BBINFILETK_API TK_Unicode_Options : public BBaseOpcodeHandler {
    private:
        TK_Unicode_Options(const TK_Unicode_Options&);
        TK_Unicode_Options& operator=(const TK_Unicode_Options&);

	protected:
		int                     m_length;       /*!< Length of text string */
		unsigned short *        m_string;       /*!< Text string */

	public:
		/*! constructor */
		TK_Unicode_Options () : BBaseOpcodeHandler (TKE_Unicode_Options), m_length (0), m_string (0) {}
		~TK_Unicode_Options();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the user options string.  Allocates the buffer and copies the string */
		void                    SetOptions (unsigned short const * options) alter;
		/*! Sets the user options string buffer.  Allocates a buffer which can hold a srting of 'length' characters */
		void                    SetOptions (int length) alter;
		/*! Returns the user options string. */
		unsigned short const *  GetOptions (void) const                 { return m_string;          }
		/*! Returns the user options string buffer, which may be modified directly */
		unsigned short alter *  GetOptions (void) alter                 { return m_string;          }
		/*! Returns the size of the string buffer (number of shorts, not including any space for trailing null) */
		int                     GetLength(void) alter                   { return m_length;          }
};

//! Handles the TKE_User_Index opcode.
/*!  
	TK_User_Index provides support for writing/reading the TKE_User_Index opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports storing non-graphical data per segment.  A user-index is an index into 
	a list of user options.
*/
class BBINFILETK_API TK_User_Index : public BBaseOpcodeHandler {
    private:
        TK_User_Index(const TK_User_Index&);
        TK_User_Index& operator=(const TK_User_Index&);

	protected:
		int             m_count;            //!< for internal use only
		int *           m_indices;          //!< for internal use only
		long *          m_values;           //!< for internal use only
		int				m_current_value;		//!< for internal use only
		void    set_indices (int count, int const * indices, long const * values) alter;  //!< for internal use only
		void    set_indices (int count) alter;                                            //!< for internal use only

	public:
		/*! constructor */
		TK_User_Index ()
			: BBaseOpcodeHandler (TKE_User_Index), m_count (0), m_indices (0), m_values (0), m_current_value(0) {}
		~TK_User_Index();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the index-value pairs.  Allocates buffers and copies the indices and values */
		void            SetIndices (int count, int const * indices, long const * values) alter
														{ set_indices (count, indices, values); }
		/*! Sets the index-value pair buffers.  Allocates buffers which can hold 'count' index-value pairs */
		void            SetIndices (int count) alter    { set_indices (count);                  }
		/*! Returns the current size of the buffers as the number of index-value pairs they can contain */
		int             GetCount (void) const           { return m_count;                       }
		/*! Returns the index array */
		int const *     GetIndices (void) const         { return m_indices;                     }
		/*! Returns the index array buffer, which may be modified directly */
		int alter *     GetIndices (void) alter         { return m_indices;                     }
		/*! Returns the value array */
		long const *    GetValues (void) const          { return m_values;                      }
		/*! Returns the value array buffer, which may be modified directly */
		long alter *    GetValues (void) alter          { return m_values;                      }
};

//! Handles the TKE_User_Value opcode.
/*!  
	TK_User_Value provides support for writing/reading the TKE_User_Value opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports storing non-graphical data per segment.  A user-value is a long integer.
*/
class BBINFILETK_API TK_User_Value : public BBaseOpcodeHandler {
    private:
        TK_User_Value(const TK_User_Value&);
        TK_User_Value& operator=(const TK_User_Value&);

	protected:
		long            m_value;  //!< for internal use only

	public:
		/*! constructor */
		TK_User_Value ()
			: BBaseOpcodeHandler (TKE_User_Value), m_value (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the user value */
		void            SetValue (long v) alter     { m_value = v;      }
		/*! Returns the user value */
		long            GetValue (void) const       { return m_value;   }
};

/*! \enum TKO_Camera_Projection
		Defines camera projection; refer to ::HC_Set_Camera for details
*/
enum TKO_Camera_Projection {
	TKO_Camera_Perspective_Bit  = 0x01,  //!< perspective bit setting
	TKO_Camera_Stretched_Bit    = 0x02,  //!< stretched bit setting
	TKO_Camera_Projection_Mask  = 0x03,  //!< self-explanatory

	TKO_Camera_Orthographic     = 0x00,  //!< orthographic projection
	TKO_Camera_Perspective      = 0x01,  //!< perspective projection
	TKO_Camera_Stretched        = 0x02,  //!< stretched projection

	TKO_Camera_Oblique_Y        = 0x04,  //!< oblique y setting
	TKO_Camera_Oblique_X        = 0x08,  //!< oblique x setting
	TKO_Camera_Oblique_Mask     = 0x0C,  //!< self-explanatory

	TKO_Camera_Near_Limit       = 0x10   //!< near limit setting
};

//! Handles the TKE_Camera opcode.
/*!  
	TK_Camera provides support for writing/reading the TKE_Camera opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports a camera, which is a segment attribute defining a specific view of world space.
*/
class BBINFILETK_API2 TK_Camera : public BBaseOpcodeHandler {
    private:
        TK_Camera(const TK_Camera&);
        TK_Camera& operator=(const TK_Camera&);

	protected:
		/*! internal use; stored in the following order: position(3), target(3), up(3), width, height, oblique(2), near_limnit
			Position, target, and up are all object space.
			Width and height define the span of the minimum area around the target to be visible */
		float           m_settings[14];	
		char            m_projection;	//!< internal use 
		int             m_length;       /*!< Length of view name */
		char *          m_name;         /*!< View name */

		 
		void    set_name (char const * name) alter;	//!< internal use
	
		void    set_name (int length) alter;		//!< internal use

	public:
		/*! constructor */
		TK_Camera (unsigned char opcode = TKE_Camera)
			: BBaseOpcodeHandler (opcode), m_length (0), m_name (0) {
				int i;
				int count = (int)(sizeof(m_settings) / sizeof(m_settings[0]));
				for (i = 0; i < count; i++)
					m_settings[i] = 0;
		}
		~TK_Camera();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the position of the camera using discrete float values */
		void            SetPosition (float x, float y, float z) alter
										{ m_settings[0] = x;  m_settings[1] = y;  m_settings[2] = z;    }
		/*! Sets the position of the camera using a float triplet */
		void            SetPosition (float const * p) alter     { SetPosition (p[0], p[1], p[2]);       }
		/*! Returns the position of the camera as the adress of a float triplet */
		float const *   GetPosition (void) const                { return &m_settings[0];                }

		/*! Sets the target of the camera using discrete float values */
		void            SetTarget (float x, float y, float z) alter
										{ m_settings[3] = x;  m_settings[4] = y;  m_settings[5] = z;    }
		/*! Sets the target of the camera using a float triplet */
		void            SetTarget (float const * t) alter       { SetTarget (t[0], t[1], t[2]);         }
		/*! Returns the target of the camera as the adress of a float triplet */
		float const *   GetTarget (void) const                  { return &m_settings[3];                }

		/*! Sets the up-vector of the camera using discrete float values */
		void            SetUpVector (float x, float y, float z) alter
										{ m_settings[6] = x;  m_settings[7] = y;  m_settings[8] = z;    }
		/*! Sets the up-vector of the camera using a float triplet */
		void            SetUpVector (float const * u) alter     { SetUpVector (u[0], u[1], u[2]);       }
		/*! Returns the up-vector of the camera as the adress of a float triplet */
		float const *   GetUpVector (void) const                { return &m_settings[6];                }

		/*! Sets the camera field size using discrete float values */
		void            SetField (float w, float h) alter       { m_settings[9] = w;  m_settings[10] = h; }
		/*! Sets the camera field size using a float pair */
		void            SetField (float const * f) alter        { SetField (f[0], f[1]);                }
		/*! Returns the camera field size as the address of a float pair */
		float const *   GetField (void) const                   { return &m_settings[9];                }

		/*! Sets the camera oblique angles (degrees) using discrete float values */
		void            SetOblique (float h, float v) alter     { m_settings[11] = h;  m_settings[12] = v;
																  m_projection &= ~TKO_Camera_Oblique_Mask;
																  if (h != 0.0f) m_projection |= TKO_Camera_Oblique_Y;
																  if (v != 0.0f) m_projection |= TKO_Camera_Oblique_Mask;
																}
		/*! Sets the camera oblique angles (degrees) using a float pair */
		void            SetOblique (float const * o) alter      { SetOblique (o[0], o[1]);              }
		/*! Returns the camera oblique angles as the address of a float pair */
		float const *   GetOblique (void) const                 { return &m_settings[11];               }

		/*! Sets the projection type.  Projection types are specified by #TKO_Camera_Projection. */
		void            SetProjection (int p) alter             { m_projection = (char)p;   }
		/*! Returns the projection type.  Projection types are specified by #TKO_Camera_Projection. */
		int             GetProjection (void) const              { return (int)m_projection; }

		/*! Sets the camera field size using discrete float values */
		void            SetNearLimit (float l) alter            { m_settings[13] = l;
																  m_projection &= ~TKO_Camera_Near_Limit;
																  if (l != 0.0f) m_projection |= TKO_Camera_Near_Limit;
																}
		/*! Returns the camera field size as the address of a float pair */
		float           GetNearLimit (void) const               { return m_settings[13];    }

		/*! Sets the view name.  Allocates the buffer and copies the string */
		void            SetView (char const * name) alter       { set_name (name);          }
		/*! Sets the view name buffer.  Allocates a buffer which can hold a srting of 'length' characters */
		void            SetView (int length) alter              { set_name (length);        }
		/*! Returns the view name. */
		char const *    GetView (void) const                    { return m_name;            }
		/*! Returns the view name buffer, which may be modified directly */
		char alter *    GetView (void) alter                    { return m_name;            }
};

//! Handles the TKE_Window opcode.
/*!  
	TK_Window provides support for writing/reading the TKE_Window opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports a window attributes, which defines a local viewport with it's own unique camera.
*/
class BBINFILETK_API TK_Window : public BBaseOpcodeHandler {
    private:
        TK_Window(const TK_Window&);
        TK_Window& operator=(const TK_Window&);

	protected:
		float           m_window[4];  //!< for internal use only

	public:
		/*! constructor */
		TK_Window ()
			: BBaseOpcodeHandler (TKE_Window) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the subwindow region using discrete float values */
		void            SetWindow (float l, float r, float b, float t) alter
								{ m_window[0] = l;  m_window[1] = r;  m_window[2] = b;  m_window[3] = t;    }
		/*! Sets the subwindow region using an array of four floats */
		void            SetWindow (float const * w) alter      { SetWindow (w[0], w[1], w[2], w[3]);        }
		/*! Returns the subwindow region as the address of an array of four floats */
		float const *   GetWindow (void) const                 { return m_window;                           }
};


/*! \enum TKO_Font_Options
		When written to m_mask, defines which font options are present in the current opcode (all others are ignored)
		When written to m_value, defines which font options are turned on; refer to ::HC_Set_Text_Font for details
*/
enum TKO_Font_Options {
	TKO_Font_Names				= 0x00000001,   //!< the name of the font (i.e. which font to use)
	TKO_Font_Size				= 0x00000002,   //!< the size at which to draw characters
	TKO_Font_Size_Tolerance		= 0x00000004,   //!< the size tolerance outside of which fonts must be regenerated 
	TKO_Font_Transforms			= 0x00000008,   //!< transformation mode (set with a bit from TKO_Font_Transforms)
	TKO_Font_Rotation			= 0x00000010,   //!< character rotation, specified in degrees
	TKO_Font_Slant				= 0x00000020,   //!< character slant 
	TKO_Font_Width_Scale		= 0x00000040,   //!< scale factor for width
	TKO_Font_Extended			= 0x00000080,   //!< extends font options to a second byte
	TKO_Font_Extended_Mask		= 0xFFFFFF00,   //   internal use, indicates bits which require TKO_Font_Extended
	TKO_Font_Extra_Space		= 0x00000100,   //!< in addition to the spacing specified within the font itself, the extra space to add between characters
	TKO_Font_Line_Spacing		= 0x00000200,   //!< extra spacing between lines
	TKO_Font_Outline			= 0x00000400,   //!< draw only the outline (i.e. don't fill)
	TKO_Font_Underline			= 0x00000800,   //!< add an underline to the font
	TKO_Font_Strikethrough		= 0x00001000,   //!< add a strikethrough to the font
	TKO_Font_Overline			= 0x00002000,   //!< add an overline to the font
	TKO_Font_Uniform_Spacing	= 0x00004000,   //!< force non-proportional spacing
	TKO_Font_Extended2			= 0x00008000,   //!< for further expansion
	TKO_Font_Extended2_Mask		= 0xFFFF0000,   //!< for further expansion
	TKO_Font_Greeking_Limit		= 0x00010000,	//!< limit at which text may be replaced with a crude representation
	TKO_Font_Fill_Edges			= 0x00020000,	//!< fill edges of characters to improve appearance ar small sizes
	TKO_Font_Bold				= 0x00040000,	//!< choose or simulate a bold variation
	TKO_Font_Italic				= 0x00080000,	//!< choose or simulate an italic variation
	TKO_Font_Renderer			= 0x00100000,	//!< limit font source
	TKO_Font_Greeking_Mode		= 0x00200000,	//!< select how to draw (or not) greeked text
	TKO_Font_Preference			= 0x00400000	//!< select preferred drawing modes
};

/*! \enum TKO_Font_Size_Units
		Defines the size units used for fonts; refer to ::HC_Set_Text_Font for details
*/
enum TKO_Font_Size_Units {
	TKO_Font_Size_Object = 0,  //!< object space
	TKO_Font_Size_Screen = 1,  //!< screen space
	TKO_Font_Size_Window = 2,  //!< window space
	TKO_Font_Size_Points = 3,  //!< points 
	TKO_Font_Size_Pixels = 4,  //!< pixels 
	TKO_Font_Size_Percent = 5  //!< used by tolerance
};


/*! 
	\enum TKO_Font_Transforms
	Defines font transform mode for the TKE_Text_Font opcode.  Refer to ::HC_Set_Text_Font for details
	There is no need for an "off" because that would be specified by m_value.
*/
enum TKO_Font_Transforms {
	TKO_Font_Transform_Position_Only = 0,  //!< transform position only
	TKO_Font_Transform_Full = 1            //!< full transforms
};


/*! \enum TKO_Font_Renderers
		Limits fonts to a particular type ::HC_Set_Text_Font for details
*/
enum TKO_Font_Renderers {
	TKO_Font_Renderer_Undefined	= -1,	//!< not sapecified
	TKO_Font_Renderer_Default	= 0,	//!< use any available fonts
	TKO_Font_Renderer_Driver	= 1,	//!< only use hardware fonts
	TKO_Font_Renderer_Truetype	= 2,	//!< only use Truetype (or similar) fonts
	TKO_Font_Renderer_Defined	= 3		//!< only use Hoops defined (stroked) fonts
};

/*! \enum TKO_Font_Preferences
		Limits fonts to a particular type ::HC_Set_Text_Font for details
*/
enum TKO_Font_Preferences {
	TKO_Font_Preference_Undefined = -1,	//!< not specified
	TKO_Font_Preference_Default	= 0,	//!< use whatever the display device prefers
	TKO_Font_Preference_Bitmap	= 1,	//!< try to use bitmaps
	TKO_Font_Preference_Outline	= 2		//!< try to use polygonal (outline) representations
};

/*! \enum TKO_Font_Greeking_Modes
		Defines the way text below the "greeking limit" is drawn; refer to ::HC_Set_Text_Font for details
*/
enum TKO_Font_Greeking_Modes {
	TKO_Font_Greeking_Mode_None  = 0, //!< don't draw
	TKO_Font_Greeking_Mode_Lines = 1, //!< replace with a grid of lines
	TKO_Font_Greeking_Mode_Box   = 2  //!< replace with a box (probably a halftone stipple)
};

//! Handles the TKE_Text_Font opcode.
/*!  
	TK_Text_Font provides support for writing/reading the TKE_Text_Font opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports various text attributes.   Text_Font contains information about the font,
	including name, size, size units, spacing, spacing units, tolerance, tolerance units, slant, transforms, 
	rotation and width scaling.
*/
class BBINFILETK_API TK_Text_Font : public BBaseOpcodeHandler {
    private:
        TK_Text_Font(const TK_Text_Font&);
        TK_Text_Font& operator=(const TK_Text_Font&);

	protected:
		int             m_mask;  //!< specifies which rendering options are active (and hence, which are valid).  For internal use only.
		int             m_value;  //!< specifies what values to set for boolean options.  For internal use only.
		int             m_names_length; //!< for internal use only
		char *          m_names;        //!< for internal use only
		float           m_size;         //!< for internal use only
		float           m_tolerance;    //!< for internal use only
		float           m_rotation;     //!< for internal use only
		float           m_slant;        //!< for internal use only
		float           m_width_scale;  //!< for internal use only  
		float           m_extra_space;  //!< for internal use only
		float           m_line_spacing; //!< for internal use only
		float			m_greeking_limit;//!< for internal use only
		float			m_renderer_cutoff;//!< for internal use only
		float			m_preference_cutoff;//!< for internal use only
		int				m_renderers[2];   //!< for internal use only
		int				m_preferences[2];   //!< for internal use only
		unsigned char   m_size_units;   //!< for internal use only
		unsigned char   m_tolerance_units;//!< for internal use only
		unsigned char   m_space_units;  //!< for internal use only
		unsigned char   m_greeking_units;  //!< for internal use only
		unsigned char	m_greeking_mode;   //!< for internal use only
		unsigned char   m_transforms;   //!< for internal use only
		unsigned char   m_renderer_cutoff_units;  //!< for internal use only
		unsigned char   m_preference_cutoff_units;  //!< for internal use only

		void    set_names (int length) alter;           //!< for internal use only
		void    set_names (char const * names) alter;   //!< for internal use only

	public:
		/*! constructor */
		TK_Text_Font ()
			: BBaseOpcodeHandler (TKE_Text_Font), m_names_length (0), m_names (0) {}
		~TK_Text_Font ();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the mask indicating which font options are specified.  Options are defined in #TKO_Font_Options. */
		void            SetMask (int m) alter {
							m_mask = m;
							if ((m & TKO_Font_Extended2_Mask) != 0)
								m_mask |= TKO_Font_Extended2;
							if ((m & TKO_Font_Extended_Mask) != 0)
								m_mask |= TKO_Font_Extended;
						}
		/*! Returns the mask indicating which font options are specified.  Options are defined in #TKO_Font_Options. */
		int             GetMask (void) const                    { return m_mask;        }

		/*! Sets the mask indicating which font options are on/active.  Options are defined in #TKO_Font_Options. */
		void            SetValue (int v) alter                  { m_value = v;          }
		/*! Returns the mask indicating which font options are on/active.  Options are defined in #TKO_Font_Options. */
		int             GetValue (void) const                   { return m_value;       }

		/*! Sets the font names.  Allocates the buffer and copies the names string */
		void            SetNames (char const * names) alter     { set_names (names);    }
		/*! Sets the font names buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetNames (int length) alter             { set_names (length);   }
		/*! Returns the font names string */
		char const *    GetNames (void) const                   { return m_names;       }
		/*! Returns the font names string buffer, which may be modified directly */
		char alter *    GetNames (void) alter                   { return m_names;       }

		/*! Sets the text size */
		void            SetSize (float s) alter                 { m_size = s;                           }
		/*! Returns the text size */
		float           GetSize (void) const                    { return m_size;                        }

		/*! Sets the units used to interpret the size.  Units are defined in #TKO_Font_Size_Units. */
		void            SetSizeUnits (int u) alter              { m_size_units = (unsigned char)u;      }
		/*! Returns the units used to interpret the size.  Units are defined in #TKO_Font_Size_Units. */
		int             GetSizeUnits (void) const               { return (int)m_size_units;             }

		/*! Sets the text size tolerance */
		void            SetTolerance (float t) alter            { m_tolerance = t;                      }
		/*! Returns the text size tolerance */
		float           GetTolerance (void) const               { return m_tolerance;                   }

		/*! Sets the units used to interpret the size tolerance.  Units are defined in #TKO_Font_Size_Units. */
		void            SetToleranceUnits (int u) alter         { m_tolerance_units = (unsigned char)u; }
		/*! Returns the units used to interpret the size tolerance.  Units are defined in #TKO_Font_Size_Units. */
		int             GetToleranceUnits (void) const          { return (int)m_tolerance_units;        }

		/*! Sets the rotation angle of characters */
		void            SetRotation (float r) alter             { m_rotation = r;                       }
		/*! Returns the rotation angle of characters */
		float           GetRotation (void) const                { return m_rotation;                    }

		/*! Sets the slant factor applied to characters */
		void            SetSlant (float s) alter                { m_slant = s;                          }
		/*! Returns the slant factor applied to characters */
		float           GetSlant (void) const                   { return m_slant;                       }

		/*! Sets the horizontal scaling applied to characters */
		void            SetWidthScale (float s) alter           { m_width_scale = s;                    }
		/*! Returns the horizontal scaling applied to characters */
		float           GetWidthScale (void) const              { return m_width_scale;                 }

		/*! Sets the amount of extra space inserted between characters */
		void            SetExtraSpace (float s) alter           { m_extra_space = s;                    }
		/*! Returns the amount of extra space inserted between characters */
		float           GetExtraSpace (void) const              { return m_extra_space;                 }

		/*! Sets the units used to interpret the extra space.  Units are defined in #TKO_Font_Size_Units. */
		void            SetExtraSpaceUnits (int u) alter        { m_space_units = (unsigned char)u;     }
		/*! Returns the units used to interpret the extra space.  Units are defined in #TKO_Font_Size_Units. */
		int             GetExtraSpaceUnits (void) const         { return (int)m_space_units;            }

		/*! Sets the spacing between lines of multi-line strings */
		void            SetLineSpacing (float s) alter          { m_line_spacing = s;                   }
		/*! Returns the spacing between lines of multi-line strings */
		float           GetLineSpacing (void) const             { return m_line_spacing;                }

		/*! Sets how the string is affected by any modeling transforms.  Transforms are defined in #TKO_Font_Transforms. */
		void            SetTransforms (int t) alter             { m_transforms = (unsigned char)t;      }
		/*! Returns how the string is affected by any modeling transforms.  Transforms are defined in #TKO_Font_Transforms. */
		int             GetTransforms (void) const              { return (int)m_transforms;             }

		/*! Sets the limit below which text drawing will be "greeked" (not drawn or drawn as simple geometry) */
		void            SetGreekingLimit (float s) alter           { m_greeking_limit = s;                    }
		/*! Returns the greeking limit */
		float           GetGreekingLimit (void) const              { return m_greeking_limit;                 }

		/*! Sets the units used to interpret the greeking limit.  Units are defined in #TKO_Font_Size_Units. */
		void            SetGreekingLimitUnits (int u) alter        { m_greeking_units = (unsigned char)u;     }
		/*! Returns the units used to interpret the greeking limit.  Units are defined in #TKO_Font_Size_Units. */
		int             GetGreekingLimitUnits (void) const         { return (int)m_greeking_units;            }

		/*! Sets the method used for handling greeked text. */
		void            SetGreekingMode (int m) alter        { m_greeking_mode = (unsigned char)m;     }
		/*! Returns the method used for handling greeked text. */
		int             GetGreekingMode (void) const         { return (int)m_greeking_mode;            }


		/*! Sets which types of fonts may be used.  Renderers are defined in #TKO_Font_Renderers. */
		void            SetRenderer (int r) alter             { m_renderers[0] = m_renderers[1] = r;    }
		/*! Returns which types of fonts may be used.  Renderers are defined in #TKO_Font_Renderers. */
		int             GetRenderer (void) const              { return m_renderers[0];					}

		/*! Sets which types of fonts may be used.  Renderers are defined in #TKO_Font_Renderers. */
		void            SetRenderers (int r1, int r2) alter   { m_renderers[0] = r1; m_renderers[1] = r2;	}
		/*! Returns which types of fonts may be used.  Renderers are defined in #TKO_Font_Renderers. */
		int const *      GetRenderers (void) const            { return m_renderers;							}

		/*! Sets the limit below which text drawing will use the "lower" renderer */
		void            SetRendererCutoff (float s) alter     { m_renderer_cutoff = s;                    }
		/*! Returns the renderer cutoff limit */
		float           GetRendererCutoff (void) const        { return m_renderer_cutoff;                 }

		/*! Sets the units used to interpret the renderer cutoff.  Units are defined in #TKO_Font_Size_Units. */
		void            SetRendererCutoffUnits (int u) alter        { m_renderer_cutoff_units = (unsigned char)u;     }
		/*! Returns the units used to interpret the renderer cutoff.  Units are defined in #TKO_Font_Size_Units. */
		int             GetRendererCutoffUnits (void) const         { return (int)m_renderer_cutoff_units;            }


		/*! Sets which drawing modes may be used.  Preferences are defined in #TKO_Font_Preferences. */
		void            SetPreference (int r) alter             { m_preferences[0] = m_preferences[1] = r;    }
		/*! Returns which types of fonts may be used.  Preferences are defined in #TKO_Font_Preferences. */
		int             GetPreference (void) const              { return m_preferences[0];					}

		/*! Sets which drawing modes may be used.  Preferences are defined in #TKO_Font_Preferences. */
		void            SetPreferences (int r1, int r2) alter   { m_preferences[0] = r1; m_preferences[1] = r2;	}
		/*! Returns which types of fonts may be used.  Preferences are defined in #TKO_Font_Preferences. */
		int const *      GetPreferences (void) const            { return m_preferences;							}

		/*! Sets the limit below which text drawing will use the "lower" preference */
		void            SetPreferenceCutoff (float s) alter     { m_preference_cutoff = s;                    }
		/*! Returns the preference cutoff limit */
		float           GetPreferenceCutoff (void) const        { return m_preference_cutoff;                 }

		/*! Sets the units used to interpret the preference cutoff.  Units are defined in #TKO_Font_Size_Units. */
		void            SetPreferenceCutoffUnits (int u) alter        { m_preference_cutoff_units = (unsigned char)u;     }
		/*! Returns the units used to interpret the preference cutoff.  Units are defined in #TKO_Font_Size_Units. */
		int             GetPreferenceCutoffUnits (void) const         { return (int)m_preference_cutoff_units;            }
};

////////////////////////////////////////////////////////////////////////////////

//! Handles the TKE_Bounding and TKE_Bounding_Info opcodes.
/*!  
	TK_Bounding provides support for writing/reading the TKE_Bounding and TKE_Bounding_Info opcode objects to/from an HSF file.  

	The HOOPS/3dGS scene-graph maintain bounding volume information which denotes the extents of the scene.   
	Exporting bounding information (typically at the beginning of the HSF file) is useful so that a reading app
	can quickly find out the scene-extents and set an appropriate camera.  

*/

/*! \enum TKO_Bounding_Type_Options
		Defines the class of bounding volume that is specified
*/
enum TKO_Bounding_Type_Options {
	TKO_Bounding_Type_Cuboid    = 0,  //!< self-explanatory
	TKO_Bounding_Type_Sphere    = 1   //!< self-explanatory
};



//! Handles the TKE_Bounding, and TKE_Bounding_Info opcodes.
/*! 
	TK_Bounding provides support for writing/reading the TKE_Bounding TKE_Bounding_Info opcode objects to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports 'marker' primitives which consist of a 3D point, as well as distant and local lights which are also denoted
	by a single 3D point. 

	Two different opcodes are used as follows:
		The TKE_Bounding opcode will exporta bounding volume that was explicitly set on a HOOPS segment (if any).
		The TKE_Bounding_Info will cause an explicit computation of the HOOPS/3dGS bounding information and export it. 

*/
class BBINFILETK_API2 TK_Bounding : public BBaseOpcodeHandler {
    private:
        TK_Bounding(const TK_Bounding&);
        TK_Bounding& operator=(const TK_Bounding&);

	protected:
		float           m_values[6];		//!< internal use 
		char            m_type;				//!< internal use 
			
	public:
		/*! default constructor */
		TK_Bounding (unsigned char opcode)
			: BBaseOpcodeHandler (opcode) {}
		/*! constructor variant that accepts min and max values*/
		TK_Bounding (unsigned char opcode, float * min, float * max)
			: BBaseOpcodeHandler (opcode), m_type (TKO_Bounding_Type_Cuboid) {
				m_values[0] = min[0]; m_values[1] = min[1]; m_values[2] = min[2];
				m_values[3] = max[0]; m_values[4] = max[1]; m_values[5] = max[2];
			}
		/*! constructor variant that accepts center and radius values*/
		TK_Bounding (unsigned char opcode, float * center, float radius)
			: BBaseOpcodeHandler (opcode), m_type (TKO_Bounding_Type_Sphere) {
				m_values[0] = center[0]; m_values[1] = center[1]; m_values[2] = center[2];
				m_values[3] = radius;
			}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
};

///////////

/*! \enum TKO_Light_Options
		defines options that can be attached to lights.  See TK_Point.
*/
enum TKO_Light_Options {
	TKO_Light_Camera_Relative = 0x1
};


/////////////////////////////////////////////////////////////////////

//! Handles the TKE_Marker, TKE_Text_Path TKE_Distant_Light, and TKE_Local_Light opcodes.
/*! 
	TK_Point provides support for writing/reading the TKE_Marker, TKE_Distant_Light, and TKE_Local_Light opcode objects to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports 'marker' primitives which consist of a 3D point, as well as distant and local lights which are also denoted
	by a single 3D point. 
*/
class BBINFILETK_API TK_Point : public BBaseOpcodeHandler {
    private:
        TK_Point(const TK_Point&);
        TK_Point& operator=(const TK_Point&);

	protected:
		float           m_point[3];  /*!< internal use; an array of three floats defining the x,y,z coordinates of the marker */
		char            m_options;   //!< relevant to TKE_Distant_Light and TKE_Local_Light only.  See TKO_Light_Options.

	public:
		/*! constructor */
		TK_Point (unsigned char opcode)
			: BBaseOpcodeHandler (opcode) {
			m_point[0] = m_point[1] = m_point[2] = 0; 
			m_options = 0; 
		};

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void	    Reset(void) { 
						m_point[0] = m_point[1] = m_point[2] = 0; 
						m_options = 0; 
						BBaseOpcodeHandler::Reset(); 
					};



		/*! Sets the point using discrete float values */
		void            SetPoint (float x, float y, float z) alter  { m_point[0] = x; m_point[1] = y; m_point[2] = z; }
		/*! Sets the point using a float triplet */
		void            SetPoint (float const * p) alter            { SetPoint (p[0], p[1], p[2]);  }
		/*! Returns the point as the address of a float triplet */
		float const *   GetPoint (void) const                       { return m_point;               }

		/*! Sets the options (for lights only) */
		void	SetOptions (int o) alter	{ m_options = (char)o;		}
		/*! Returns the options (for lights only) */
		int		GetOptions (void) const		{ return (int)m_options;	}
};



//! Handles the TKE_Line opcode.
/*! 
	TK_Line provides support for writing/reading the TKE_Line opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports 'line' primitives which consist of two 3D points. 
*/
class BBINFILETK_API TK_Line : public BBaseOpcodeHandler {
    private:
        TK_Line(const TK_Line&);
        TK_Line& operator=(const TK_Line&);

	protected:
		 /*! internal use; array of floats which denote the two sets of x,y,z values that define the line */
		float           m_points[6];    

	public:
		/*! default constructor */
		TK_Line ()
			: BBaseOpcodeHandler (TKE_Line) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the line using discrete float values */
		void            SetPoints (float x1, float y1, float z1, float x2, float y2, float z2) alter {
								m_points[0] = x1;  m_points[1] = y1;  m_points[2] = z1;
								m_points[3] = x2;  m_points[4] = y2;  m_points[5] = z2;
							}
		/*! Sets the line using discrete float triplets */
		void            SetPoints (float const * s, float const * e) alter {
								SetPoints (s[0], s[1], s[2],  e[0], e[1], e[2]);
							}
		/*! Sets the line using an array of 6 floats */
		void            SetPoints (float const * p) alter           { SetPoints (&p[0], &p[3]); }
		/*! Returns the point as the address of an array of six floats */
		float const *   GetPoints (void) const                      { return m_points;          }
};


//! Handles the TKE_Infinite_Line opcode.
/*! 
	TK_Infinite_Line provides support for writing/reading the TKE_Infinite_Line opcode object to/from an HSF file.  

	The HOOPS/3dGS scene-graph supports 'infinite line' primitives which are defined by two 3D points. 
*/
class BBINFILETK_API TK_Infinite_Line : public BBaseOpcodeHandler {
	private:
        TK_Infinite_Line(const TK_Infinite_Line&);
        TK_Infinite_Line& operator=(const TK_Infinite_Line&);
	protected:
		 /*! internal use; array of floats which denote the two sets of x,y,z values that define the line */
		float           m_points[6];    

	public:
		/*! default constructor */
		TK_Infinite_Line ()
			: BBaseOpcodeHandler (TKE_Infinite_Line) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the line using discrete float values */
		void            SetPoints (float x1, float y1, float z1, float x2, float y2, float z2) alter {
								m_points[0] = x1;  m_points[1] = y1;  m_points[2] = z1;
								m_points[3] = x2;  m_points[4] = y2;  m_points[5] = z2;
							}
		/*! Sets the line using discrete float triplets */
		void            SetPoints (float const * s, float const * e) alter {
								SetPoints (s[0], s[1], s[2],  e[0], e[1], e[2]);
							}
		/*! Sets the line using an array of 6 floats */
		void            SetPoints (float const * p) alter           { SetPoints (&p[0], &p[3]); }
		/*! Returns the point as the address of an array of six floats */
		float const *   GetPoints (void) const                      { return m_points;          }
};




//! Handles the TKE_Polyline and TKE_Polygon opcodes.
/*! 
	TK_Polypoint provides support for writing/reading the TKE_Polyline and TKE_Polygon opcode objects to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports 'polyline' and 'polygon' primitives which are both defined by a point count and points array.

	See also TK_PolyPolypoint.
*/
class BBINFILETK_API TK_Polypoint : public BBaseOpcodeHandler {
    private:
        TK_Polypoint(const TK_Polypoint&);
        TK_Polypoint& operator=(const TK_Polypoint&);

	protected:
		int             m_count;    /*!< internal use; number of 3D points that define this polypoint object  */
		int             m_allocated;/*!< internal use; Number of points the allocated array can contain */
		float *         m_points;   /*!< internal use; Array of floats which denote the point coordinates. */

		/*! for source compatibility in older versions */
		void    set_points (int count, float const * points = 0) alter      { SetPoints (count, points); }		
	public:
		/*! constructor */


		TK_Polypoint (unsigned char opcode)
			: BBaseOpcodeHandler (opcode) { m_points = 0; m_allocated = m_count = 0; }
		~TK_Polypoint();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the point array.  Allocates a buffer which will hold 'count' float triplets, and if specified,
			copies the 'points' into the buffer */
		void            SetPoints (int count, float const * points = 0) alter;
		/*! Returns the address of the point buffer */
		float const *   GetPoints (void) const                                  { return m_points;              }
		/*! Returns the address of the point buffer, which may be modified directly */
		float alter *   GetPoints (void) alter                                  { return m_points;              }
		/*! Returns the number of points in the buffer */
		int             GetCount (void) const                                   { return m_count;               }
};




#define NC_HAS_WEIGHTS 0x01     //!< an array of floats for the weights is specified with the TK_NURBS_Curve
#define NC_HAS_KNOTS   0x02     //!< an array of floats for the knots is specified with the TK_NURBS_Curve
#define NC_HAS_START   0x04     //!< a float is specified for where the TK_NURBS_Curve starts in parametric [0,1] space 
#define NC_HAS_END     0x08     //!< a float is specified for where the TK_NURBS_Curve ends in parametric [0,1] space 

//! Handles the TKE_NURBS_Curve opcode.
/*! 
	TK_NURBS_Curve provides support for writing/reading the TKE_NURBS_Curve opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports NURBS curve primitives 
*/
class BBINFILETK_API TK_NURBS_Curve : public BBaseOpcodeHandler {
    private:
        TK_NURBS_Curve(const TK_NURBS_Curve&);
        TK_NURBS_Curve& operator=(const TK_NURBS_Curve&);

	protected:

		unsigned char m_optionals;  /*!< internal use; bit field to show which optional fields are present  */  
		unsigned char m_degree;     /*!< internal use; degree of the curve */
		int m_control_point_count;  /*!< internal use; number of control points */
		int m_knot_count_implicit;  /*!< internal use; always (m_controlpointcount + m_degree + 1)  */ 
		float *m_control_points;    /*!< internal use; array of floats for control point coordinates */
		float *m_weights;           /*!< internal use; array of weights to apply to the control points */
		float *m_knots;             /*!< internal use; array of floats for the curve knot vector */
		float m_start;              /*!< internal use; where the curve starts in parametric [0,1] space */
		float m_end;                /*!< internal use; where the curve ends in parametric [0,1] space */

		//! internal use
		void    set_curve (int degree, int control_count, float const * points = 0,
						   float const * weights = 0, float const * knots = 0,
						   float start = 0.0f, float end = 1.0f) alter;
	public:
		TK_NURBS_Curve();
		~TK_NURBS_Curve();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		//! sets the curve properties
		void            SetCurve (int degree, int control_count, float const * points = 0,
								  float const * weights = 0, float const * knots = 0,
								  float start = 0.0f, float end = 1.0f) alter {
							set_curve (degree, control_count, points, weights, knots, start, end);
						}

		int             GetDegree (void) const          { return m_degree;                  } /*!< Gets the degree of the curve. */
		int             GetCount (void) const           { return m_control_point_count;     } /*!< Gets the control point count. */
		float const *   GetPoints (void) const          { return m_control_points;          } /*!< Gets a constant pointer to the control points */
		float alter *   GetPoints (void) alter          { return m_control_points;          } /*!< Gets a mutable pointer to the control points */
		float const *   GetWeights (void) const         { return m_weights;                 } /*!< Gets a constant to the control point weights */
		float alter *   GetWeights (void) alter         { return m_weights;                 } /*!< Gets a mutable pointer to the control point weights */
		float const *   GetKnots (void) const           { return m_knots;                   } /*!< Gets a constant pointer to the curve knots */
		float alter *   GetKnots (void) alter           { return m_knots;                   } /*!< Gets a mutable pointer to the knots */

		void            SetStart (float s) alter        { m_start = s;                      } /*!< Sets the start point of the curve.  This, unlike some solid modelers, is parameterized from 0 to 1, an not with respect to whatever is in the knot vector. */
		float           GetStart (void) const           { return m_start;                   } /*!< Gets the start point of the curve. */
		void            SetEnd (float e) alter          { m_end = e;                        } /*!< Sets the end point of the curve. */
		float           GetEnd (void) const             { return m_end;                     } /*!< Gets the end point of the curve. */

		void            SetOptions (int o) alter        { m_optionals = (unsigned char)o;   } /*!< Sets the bit flags to declare which optional values are present. */
		int             GetOptions (void) const         { return m_optionals;               } /*!< Gets the bit flags to show what optional values are present. */
};





#define NS_HAS_WEIGHTS 0x01   //!< an array of floats for the weights is specified with the TK_NURBS_Surface
#define NS_HAS_KNOTS   0x02   //!< an array of floats for the knots is specified with the TK_NURBS_Surface
#define NS_HAS_TRIMS   0x04   //!< the TK_NURBS_Surface contains a list of trims

#define NS_TRIM_END         0       //!< terminates an NS_TRIM_COLLECTION if one is active, otherwise terminates the list of trims
#define NS_TRIM_POLY        1       //!< the next trim is a polyline (closed automatically if not already a closed loop)
#define NS_TRIM_CURVE       2       //!< the next trim is a nurbs curve in parametric space
#define NS_TRIM_COLLECTION  3       //!< all trim objects up to the next NS_TRIM_END should be combined as one.
#define NS_TRIM_LAST_KNOWN_TYPE 3   //!< the last known trim type defined as of the current version of the toolkit

#define NS_TRIM_KEEP        0x01 //!< instead of the usual cutting away the enclosed area, cut away everything but
#define NS_TRIM_HAS_WEIGHTS 0x02 //!< applies only to trims of type NS_TRIM_CURVE: an array of floats for the weights is specified with the trim curve
#define NS_TRIM_HAS_KNOTS   0x04 //!< applies only to trims of type NS_TRIM_CURVE: an array of floats for the knots is specified with the trim curve

//! Does not handle any top level opcodes, but rather only the trim types allowable on nurbs surfaces.
/*! 
	HT_NURBS_Trim is a helper class to handle reading and writing of the nurbs surface trims.  Trims may be of type NS_TRIM_POLY, NS_TRIM_CURVE, or NS_TRIM_COLLECTION.  In any case, though, all coordinates
	are specified with respect to the parameterization of the NURBS surface.  This parameterization is always from 0 to 1 in each direction.  Some solid modelers parameterize with respect to the contents of
	their knot vectors, meaning that coordinates may need to be rescaled.  To do so, the formula is "u = (u - u_knots[u_degree]) / (u_knots[u_control_point_count] - knots[u_degree])".  Values coming out of
	HOOPS will already be properly scaled.  
*/
class BBINFILETK_API HT_NURBS_Trim : public BBaseOpcodeHandler  {
    private:
        HT_NURBS_Trim(const HT_NURBS_Trim&);
        HT_NURBS_Trim& operator=(const HT_NURBS_Trim&);

	friend class TK_NURBS_Surface;
	protected:
		//first 5 are relevant to polys and curves
		int             m_substage;             /*!< internal use; tracks progress during reading and writing */
		HT_NURBS_Trim * m_next;                 /*!< internal use; the next trim in the list. The linked list is null-terminated, at which point a NS_TRIM_END will be written to the file.  */
		unsigned char   m_type;                 /*!< internal use; an enumerated type to dictate which sort of trim we are dealing with.  Legal values are NS_TRIM_POLY, NS_TRIM_CURVE and NS_TRIM_COLLECTION. */
		int             m_count;                /*!< internal use; (relevant to NS_TRIM_CURVE and NS_TRIM_POLY only), this indicates either the number of points the primitive has */
		float *         m_points;               /*!< internal use; (relevant to NS_TRIM_CURVE and NS_TRIM_POLY only), an array of floats for the points (uv pairs) of a trim curve or polyline.  Should be of length 2*m_count. */
		//next 6 are specific to curves
		unsigned char   m_degree;               /*!< internal use; (relevant to NS_TRIM_CURVE only), indicates the degree of the trim curve.  Note that this is not necessarily the same as the degree of the surface it is trimming. */
		unsigned char   m_options;              /*!< internal use; (relevant to NS_TRIM_CURVE only), a bit field to indicate the presence of the various nurbs curve attributes.  Anything not present is assumed to fall back to its default value: 0,1 for start and end, linearly increasing knots, and uniform weights.   */
		float *         m_weights;              /*!< internal use; (relevant to NS_TRIM_CURVE only), the control point weights of the trim curve.  Present only if options contains the NS_TRIM_HAS_WEIGHTS bit */
		float *         m_knots;                /*!< internal use; (relevant to NS_TRIM_CURVE only), the knots of the trim curve.  Present only if options contains the NS_TRIM_HAS_KNOTS bit */
		float           m_start_u;              /*!< internal use; (relevant to NS_TRIM_CURVE only), the start point of the trim curve.  Parameter is specified in the range of 0 to 1.  Valid only if options contains the NS_TRIM_HAS_START bit. */
		float           m_end_u;                /*!< internal use; (relevant to NS_TRIM_CURVE only), the end point of the trim curve.  Parameter is specified in the range of 0 to 1.  Valid only if options contains the NS_TRIM_HAS_START bit. */
		HT_NURBS_Trim * m_list;                 /*!< internal use; (relevant to NS_TRIM_COLLECTION only), the linked list of trims that make up the collection */
		HT_NURBS_Trim * m_current_trim;         /*!< internal use; (relevant to NS_TRIM_COLLECTION only), tracks progress during the read or write of a trim collection. */

		HT_NURBS_Trim();
		TK_Status read_collection(BStreamFileToolkit & tk);  /*!< internal use; reads the pieces of a collection */
		TK_Status write_collection(BStreamFileToolkit & tk); /*!< internal use; writes the pieces of a collection */


	public:
		~HT_NURBS_Trim();
		void    SetPoly (int count, float const * points = 0) alter;    /*!< allocates space for the coordinates (polyline vertices or curve control points), and copies values if they were given.  Also handles initializing the type of this trim to NS_TRIM_POLY. */
		void    SetCurve (int degree, int control_count, float const * points = 0, 
						  float const * weights = 0, float const * knots = 0, float start_u = 0, float end_u = 1) alter; /*!< Configures *this to handle the reading or writing of a trim curve.  Also handles initializing the type of this trim to NS_TRIM_CURVE. */
		void    SetCollection () alter;                                 /*!< Configures *this to read or write a trim collection */
		void    SetOptions (int o) alter { m_options = (unsigned char)o; }             /*!< sets the options bit flags to indicate the presence of all of the optional fields */
		void    SetList (HT_NURBS_Trim *node) alter { m_list = node; }  /*!< sets the head of the list of a NS_TRIM_COLLECTION. */
		void    SetNext (HT_NURBS_Trim *next) { m_next = next; }        /*!< sets the next pointer of a trim.  Used for linked list management */

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		
		TK_Status   read_collection_ascii(BStreamFileToolkit & tk) alter ;
		TK_Status   write_collection_ascii(BStreamFileToolkit & tk) alter ;

		/*! gets the next trim in the list */
		HT_NURBS_Trim * GetNext (void)              { return m_next;    } 
		/*! gets an identifier for the trim type.  Legal values are NS_TRIM_POLY, NS_TRIM_CURVE, and NS_TRIM_COLLECTION */
		int             GetType (void) const        { return m_type;    } 
		/*! (relevant to NS_TRIM_CURVE and NS_TRIM_POLY only), gets the number of vertices in the trim. */
		int             GetCount (void) const       { return m_count;   } 
		/*! (relevant to NS_TRIM_CURVE and NS_TRIM_POLY only), returns a const pointer to the vertices */
		float const *   GetPoints (void) const      { return m_points;  } 
		/*! (relevant to NS_TRIM_CURVE and NS_TRIM_POLY only), returns a mutable pointer to the vertices */
		float alter *   GetPoints (void) alter      { return m_points;  } 
		/*! (relevant to NS_TRIM_CURVE only), returns the degree of the trim curve.  Note that this is not necessarily the same as the degree of the surface to be trimmed. */
		int             GetDegree (void) const      { return m_degree;  } 
		/*! (relevant to NS_TRIM_CURVE only), returns the options bit flags to indicate the presence of all of the optional fields */
		int             GetOptions (void) const     { return m_options; } 
		/*! (relevant to NS_TRIM_CURVE only), returns a const pointer to the control point weights */
		float const *   GetWeights (void) const     { return m_weights; } 
		/*! (relevant to NS_TRIM_CURVE only), returns a mutable pointer to the control point weights*/
		float alter *   GetWeights (void) alter     { return m_weights; } 
		/*! (relevant to NS_TRIM_CURVE only), returns a const pointer to the trim curve's knots */
		float const *   GetKnots (void) const       { return m_knots;   } 
		/*! (relevant to NS_TRIM_CURVE only), returns a mutable pointer to the trim curve's knots */
		float alter *   GetKnots (void) alter       { return m_knots;   } 
		/*! (relevant to NS_TRIM_COLLECTION only), returns a const pointer the collection's linked list */
		HT_NURBS_Trim const *GetList (void) const   { return m_list;    } 
		/*! (relevant to NS_TRIM_COLLECTION only), returns a mutable pointer the collection's linked list. Note that SetList, not this function, must be used to make the head of the list point somewhere else. */
		HT_NURBS_Trim *GetList (void) alter         { return m_list;    } 
    
};

//! Handles the TKE_NURBS_Surface opcode.
/*! 
	TK_NURBS_Surface provides support for writing/reading the TKE_NURBS_Surface opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports NURBS surface primitives 
*/
class BBINFILETK_API TK_NURBS_Surface : public BBaseOpcodeHandler {
    private:
        TK_NURBS_Surface(const TK_NURBS_Surface&);
        TK_NURBS_Surface& operator=(const TK_NURBS_Surface&);

	protected:
		unsigned char   m_optionals;        /*!< internal use; bit field to show which optional fields are present  */  
		unsigned char   m_degree[2];        /*!< internal use; degree of the curve (u,v) */
		int             m_size[2];          /*!< internal use; number of control points (u,v) */
		float *         m_control_points;   /*!< internal use; array of floats for control point coordinates */
		float *         m_weights;          /*!< internal use; array of weights to apply to the control points */
		float *         m_u_knots;          /*!< internal use; array of floats for the surface knot vector in the u direction. Of length GetUDegree() + GetUSize() + 1. */
		float *         m_v_knots;          /*!< internal use; array of floats for the surface knot vector in the v direction. Of length GetVDegree() + GetVSize() + 1. */ 

		HT_NURBS_Trim * m_trims;            /*!< internal use; a linked list of trimming objects.  These may be of type NS_TRIM_POLY, NS_TRIM_CURVE or NS_TRIM_COLLECTION */
		HT_NURBS_Trim * m_current_trim;     /*!< internal use; tracks the progress of reading or writing the triming objects */


	public:
		TK_NURBS_Surface();
		~TK_NURBS_Surface();

		TK_Status   Read (BStreamFileToolkit & tk) alter;   
		TK_Status   Write (BStreamFileToolkit & tk) alter;  
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter; /*!< prepares the handler for reuse during reading or writing */

		void            SetSurface (int u_degree, int v_degree,  int u_size, int v_size,
									float const * points = 0,    float const * weights = 0,
									float const * u_knots = 0,   float const * v_knots = 0) alter;  /*!< allocates all of the arrays that could be present in a nurbs surface (even if they are eventually left unused), and copies whatever data is given. */ 

		/*! gets the degree in the U direction of the NURBS Surface.  */
		int             GetUDegree (void) const         { return m_degree[0];               } 
		/*! gets the degree in the V direction of the NURBS Surface.  Note that it need not be the same as the U direction (e.g. a cylinder is linear in one direction) */
		int             GetVDegree (void) const         { return m_degree[1];               } 
		/*! gets the number of control points in the U direction */
		int             GetUSize (void) const           { return m_size[0];                 } 
		/*! gets the number of control points in the V direction */
		int             GetVSize (void) const           { return m_size[1];                 } 
		/*! returns a const pointer to the array of floats for control point coordinates */
		float const *   GetPoints (void) const          { return m_control_points;          } 
		/*! returns a mutable pointer to the array of floats for control point coordinates */
		float alter *   GetPoints (void) alter          { return m_control_points;          } 
		/*! returns a const pointer to the array of weights to apply to the control points */
		float const *   GetWeights (void) const         { return m_weights;                 } 
		/*! returns a mutable pointer to the array of weights to apply to the control points */
		float alter *   GetWeights (void) alter         { return m_weights;                 } 
		/*! returns a const pointer to the array of floats for the surface knot vector in the u direction.  Of length GetUDegree() + GetUSize() + 1. */
		float const *   GetUKnots (void) const          { return m_u_knots;                 } 
		/*! returns a mutable pointer to the array of floats for the surface knot vector in the u direction.  Of length GetUDegree() + GetUSize() + 1. */
		float alter *   GetUKnots (void) alter          { return m_u_knots;                 } 
		/*! returns a const pointer to the array of floats for the surface knot vector in the v direction.  Of length GetVDegree() + GetVSize() + 1. */
		float const *   GetVKnots (void) const          { return m_v_knots;                 } 
		/*! returns a mutable pointer to the array of floats for the surface knot vector in the v direction.  Of length GetVDegree() + GetVSize() + 1. */
		float alter *   GetVKnots (void) alter          { return m_v_knots;                 } 

		/*! sets the bit flags to indicate the presence of the various optional values of the NURBS Surface.  Currently defined bit values are NS_HAS_WEIGHTS, NS_HAS_KNOTS, and NS_HAS_TRIMS. */
		void            SetOptions (int o) alter        { m_optionals = (unsigned char)o;   } 
		/*! gets the bit flags to indicate the presence of the various optional values of the NURBS Surface.  Currently defined bit values are NS_HAS_WEIGHTS, NS_HAS_KNOTS, and NS_HAS_TRIMS. */
		int             GetOptions (void) const         { return m_optionals;               } 

		/*! creates a new instance of an HT_NURBS_Trim object to handle trims */
		HT_NURBS_Trim * NewTrim (int type = NS_TRIM_END) alter;                                 
		/*! returns a mutable pointer to the head of the linked list of trims */
		HT_NURBS_Trim * GetTrims (void) alter           { return m_trims;                   }
};

//! Handles the TKE_Area_Light opcode.
/*! 
	TK_Area_Light provides support for writing/reading the TKE_Area_Light opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports 'area-lights' which define a polygon that emits light
*/
class BBINFILETK_API TK_Area_Light : public BBaseOpcodeHandler {
    private:
        TK_Area_Light(const TK_Area_Light&);
        TK_Area_Light& operator=(const TK_Area_Light&);

	protected:
		int             m_count;	//!< internal use 
		float *         m_points;	//!< internal use
		char            m_options;	//!< internal use

		//! internal use
		void    set_points (int count, float const * points = 0) alter;

	public:
		/*! constructor */
		TK_Area_Light ()
			: BBaseOpcodeHandler (TKE_Area_Light), m_count (0), m_points (0), m_options (0) {}
		~TK_Area_Light();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*!
			Sets the point array.  Allocates a buffer which will hold 'count' float triplets, and if specified,
			copies the 'points' into the buffer
		*/
		void            SetPoints (int count, float const * points = 0) alter   { set_points (count, points);   }
		/*! Returns the address of the point buffer */
		float const *   GetPoints (void) const                                  { return m_points;              }
		/*! Returns the address of the point buffer, which may be modified directly */
		float alter *   GetPoints (void) alter                                  { return m_points;              }
		/*! Returns the number of points in the buffer */
		int             GetCount (void) const                                   { return m_count;               }

		/*! Sets the options for the light */
		void            SetOptions (int o) alter                                { m_options = (char)o;          }
		/*! Returns the options for the light */
		int             GetOptions (void) const                                 { return (int)m_options;        }
};


/*! \enum TKO_Spot_Light_Options
		Defines properties of a spot light; refer to ::HC_Insert_Spot_Light for details
*/
enum TKO_Spot_Light_Options {
	TKO_Spot_Outer_Degrees      = 0x01,  //!< self-explanatory
	TKO_Spot_Outer_Field        = 0x02,  //!< self-explanatory

	TKO_Spot_Inner_Degrees      = 0x04,  //!< self-explanatory
	TKO_Spot_Inner_Field        = 0x08,  //!< self-explanatory
	TKO_Spot_Inner_Percent      = 0x0C,  //!< self-explanatory

	TKO_Spot_Outer_Mask         = 0x03,  //!< self-explanatory
	TKO_Spot_Inner_Mask         = 0x0C,  //!< self-explanatory

	TKO_Spot_Camera_Relative    = 0x10,  //!< self-explanatory

	TKO_Spot_Concentration      = 0x20  //!< self-explanatory
};


//! Handles the TKE_Spot_Light opcode.
/*! 
	TK_Spot_Light provides support for writing/reading the TKE_Spot_Light opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports 'spot-lights' which define a light cone
*/
class BBINFILETK_API TK_Spot_Light : public BBaseOpcodeHandler {
    private:
        TK_Spot_Light(const TK_Spot_Light&);
        TK_Spot_Light& operator=(const TK_Spot_Light&);

	protected:
		float           m_position[3];      //!< for internal use only
		float           m_target[3];        //!< for internal use only
		float           m_outer;            //!< for internal use only
		float           m_inner;            //!< for internal use only
		float           m_concentration;    //!< for internal use only
		char            m_options;          //!< for internal use only

	public:
		/*! constructor */
		TK_Spot_Light ()
			: BBaseOpcodeHandler (TKE_Spot_Light), m_options (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the position of the light using discrete float values */
		void            SetPosition (float x, float y, float z) alter
										{ m_position[0] = x; m_position[1] = y; m_position[2] = z;  }
		/*! Sets the position of the light using a float triplet */
		void            SetPosition (float const * p) alter     { SetPosition (p[0], p[1], p[2]);   }
		/*! Returns the position of the lihgt as the address of a float triplet */
		float const *   GetPosition (void) const                { return m_position;                }

		/*! Sets the target of the light using discrete float values */
		void            SetTarget (float x, float y, float z) alter
										{ m_target[0] = x; m_target[1] = y; m_target[2] = z;        }
		/*! Sets the target of the light using a float triplet */
		void            SetTarget (float const * t) alter       { SetTarget (t[0], t[1], t[2]);     }
		/*! Returns the target of the lihgt as the address of a float triplet */
		float const *   GetTarget (void) const                  { return m_target;                  }

		/*! Sets the outer cone angle of the light */
		void            SetOuter (float o) alter                { m_outer = o;                      }
		/*! Returns the outer cone angle of the light */
		float           GetOuter (void) const                   { return m_outer;                   }

		/*! Sets the inner cone angle of the light */
		void            SetInner (float i) alter                { m_inner = i;                      }
		/*! Returns the inner cone angle of the light */
		float           GetInner (void) const                   { return m_inner;                   }

		/*! Sets the concentration of the light */
		void            SetConcentration (float c) alter        { m_concentration = c;              }
		/*! Returns the concentration of the light */
		float           GetConcentration (void) const           { return m_concentration;           }

		/*! Sets the options for the light.  Options are specified by #TKO_Spot_Light_Options. */
		void            SetOptions (int o) alter                { m_options = (char)o;              }
		/*! Returns the options for the light.  Options are specified by #TKO_Spot_Light_Options. */
		int             GetOptions (void) const                 { return (int)m_options;            }
};


//! Handles the TKE_Cutting_Plane opcode.
/*! 
	TK_Cutting_Plane provides support for writing/reading the TKE_Cutting_Plane opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports 'cutting planes' which define a plane that will cut away part of the scene
*/
class BBINFILETK_API TK_Cutting_Plane : public BBaseOpcodeHandler {
	protected:
		float           *m_planes;		//!< internal use
		int				m_count;		//!< internal use

	public:
		/*! constructor */
		TK_Cutting_Plane ()
			: BBaseOpcodeHandler (TKE_Cutting_Plane), m_planes (0), m_count (0) {}
		~TK_Cutting_Plane ();
        TK_Cutting_Plane(const TK_Cutting_Plane& rPlane) : BBaseOpcodeHandler (TKE_Cutting_Plane)
        {
            m_count = 0;
            m_planes = 0;
            if ((rPlane.m_count > 0) && (rPlane.m_planes != NULL))
            {
                SetPlanes( rPlane.m_count, rPlane.m_planes );
            }
        }

        TK_Cutting_Plane& operator=(const TK_Cutting_Plane& rPlane)
        {
            if(&rPlane != this)
            {
                m_opcode = rPlane.m_opcode;
                if ((m_count > 0) && (rPlane.m_planes != NULL))
                {
                    SetPlanes( m_count, rPlane.m_planes );
                }
            }
            return *this;
        }


		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void		Reset (void) alter;

		/*! Sets the cutting planes and/or allocates space */
		void			SetPlanes (int count, float const * p=0) alter;

		/*! Sets the cutting plane using discrete float values */
		void            SetPlane (float a, float b, float c, float d) alter
									{ SetPlanes(1);
										m_planes[0] = a;  m_planes[1] = b;  m_planes[2] = c;  m_planes[3] = d;    }
		/*! Sets the cutting plane using an array of four floats */
		void            SetPlane (float const * p) alter            { SetPlanes (1, p);    }
		/*! Returns the cutting plane as the address of an array of four floats */
		float const *   GetPlane (void) const                       { return m_planes;                       }

		/*! Returns the cutting planes as the address of an array of four floats per plane */
		float const *   GetPlanes (void) const                      { return m_planes;                       }
		/*! Returns the cutting planes buffer which may be modified directly */
		int				GetCount (void) const                       { return m_count;                        }
		/*! Resturns the number of cutting planes */
};


/*! \enum TKO_Circular_Options
		Defines properties of circular geometries; refer to ::HC_Set_Circular_Center for details
*/
enum TKO_Circular_Options {
	TKO_Circular_Center		= 0x01  //!< self-explanatory
};

//! Handles the TKE_Circle, TKE_Circular_Arc, TKE_Circular_Chord and TKE_Circular_Wedge opcodes.
/*! 
	TK_Circle provides support for writing/reading the TKE_Circle, TKE_Circular_Arc, 
	TKE_Circular_Chord and TKE_Circular_Wedge opcode objects to/from an HSF file. 

	The HOOPS/3dGS scene-graph can contain various forms of a 'circle' primitive, defined by 
	three non-colinear points.  
*/
class BBINFILETK_API TK_Circle : public BBaseOpcodeHandler {
    private:
        TK_Circle(const TK_Circle&);
        TK_Circle& operator=(const TK_Circle&);

	protected:
		float           m_start[3];     /*!< internal use; Starting point  */
		float           m_middle[3];    /*!< internal use; Middle point    */
		float           m_end[3];       /*!< internal use; End point       */
		float			m_center[3];    /*!< internal use; explicit Center point    */
		unsigned char	m_flags;		/*!< internal use; flag if Center point is specified */

	public:
		/*! constructor */
		TK_Circle (unsigned char opcode)
			: BBaseOpcodeHandler (opcode), m_flags (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void		Reset (void) alter;

		/*! Sets the start point of the arc using discrete float values */
		void            SetStart (float x, float y, float z) alter {
							m_start[0] = x;     m_start[1] = y;     m_start[2] = z;
						}
		/*! Sets the start point of the arc using a float triplet */
		void            SetStart (float const * s) alter {
							SetStart (s[0], s[1], s[2]);
						}
		/*! Sets the intermediate point of the arc using discrete float values */
		void            SetMiddle (float x, float y, float z) alter {
							m_middle[0] = x;    m_middle[1] = y;    m_middle[2] = z;
						}
		/*! Sets the intermediate point of the arc using a float triplet */
		void            SetMiddle (float const * m) alter {
							SetMiddle (m[0], m[1], m[2]);
						}
		/*! Sets the end point of the arc using discrete float values */
		void            SetEnd (float x, float y, float z) alter {
							m_end[0] = x;       m_end[1] = y;       m_end[2] = z;
						}
		/*! Sets the end point of the arc using a float triplet */
		void            SetEnd (float const * e) alter {
							SetEnd (e[0], e[1], e[2]);
						}
		/*! Sets the end point of the arc using discrete float values */
		void            SetCenter (float x, float y, float z) alter {
							m_center[0] = x;	m_center[1] = y;	m_center[2] = z;
							m_flags = TKO_Circular_Center;
						}
		/*! Sets the end point of the arc using a float triplet */
		void            SetCenter (float const * c) alter {
							if (c) SetCenter (c[0], c[1], c[2]);
							else m_flags = 0;
						}
		/*! Sets the three points describing the arc using discrete point triplets */
		void            SetPoints (float const * s, float const * m, float const * e,
								   float const * c = 0) alter {
							SetStart (s); SetMiddle (m); SetEnd (e); SetCenter (c);
						}
		/*! Returns the start point of the arc as the address of a float triplet */
		float const *   GetStart (void) const   { return m_start;   }
		/*! Returns the intermediate point of the arc as the address of a float triplet */
		float const *   GetMiddle (void) const  { return m_middle;  }
		/*! Returns the end point of the arc as the address of a float triplet */
		float const *   GetEnd (void) const     { return m_end;     }
		/*! Returns the center point of the arc as the address of a float triplet, null if not present */
		float const *   GetCenter (void) const  { return (m_flags & TKO_Circular_Center) ? m_center : 0;	}
};


//! Handles the TKE_Ellipse and TKE_Elliptical_Arc opcodes.
/*! 
	TK_Ellipse provides support for writing/reading the TKE_Ellipse and TKE_Elliptical_Arc 
	opcode objects to/from an HSF file. 

	The HOOPS/3dGS scene-graph can contain 'ellipse' and 'elliptical arc' primitives which are defined by center, major and minor 
	points, as well as two parameterization values in the case of an elliptical arc.
*/
class BBINFILETK_API TK_Ellipse : public BBaseOpcodeHandler {
    private:
        TK_Ellipse(const TK_Ellipse&);
        TK_Ellipse& operator=(const TK_Ellipse&);

	protected:
		float           m_center[3];    /*!< internal use; center point */
		float           m_major[3];     /*!< internal use; Major axis */
		float           m_minor[3];     /*!< internal use; Minor axis */
		float           m_limits[2];    /*!< internal use; Parameterization values */

	public:
		/*! constructor */
		TK_Ellipse (unsigned char opcode)
			: BBaseOpcodeHandler (opcode) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the center of the arc using discrete float values */
		void            SetCenter (float x, float y, float z) alter {
							m_center[0] = x;    m_center[1] = y;    m_center[2] = z;
						}
		/*! Sets the center of the arc using a float triplet */
		void            SetCenter (float const * s) alter   { SetCenter (s[0], s[1], s[2]); }
		/*! Returns the center of the arc as the address of a float triplet */
		float const *   GetCenter (void) const              { return m_center;              }

		/*! Sets the major axis of the arc using discrete float values */
		void            SetMajor (float x, float y, float z) alter {
							m_major[0] = x;     m_major[1] = y;     m_major[2] = z;
						}
		/*! Sets the major axis of the arc using a float triplet */
		void            SetMajor (float const * m) alter    { SetMajor (m[0], m[1], m[2]);  }
		/*! Returns the major axis of the arc as the address of a float triplet */
		float const *   GetMajor (void) const               { return m_major;               }

		/*! Sets the minor axis of the arc using discrete float values */
		void            SetMinor (float x, float y, float z) alter {
							m_minor[0] = x;     m_minor[1] = y;     m_minor[2] = z;
						}
		/*! Sets the minor axis of the arc using a float triplet */
		void            SetMinor (float const * m) alter    { SetMinor (m[0], m[1], m[2]);  }
		/*! Returns the minor axis of the arc as the address of a float triplet */
		float const *   GetMinor (void) const               { return m_minor;               }

		/*! Sets the parametric limits of the arc using discrete values */
		void            SetLimits (float s, float e) alter  {
							m_limits[0] = s;    m_limits[1] = e;
						}
		/*! Returns the parametric limits of the arc as the address of a float pair */
		float const *   GetLimits (void) const              { return m_limits;              }
};


//! Handles the TKE_Sphere opcode.
/*! 
	TK_Sphere provides support for writing/reading the TKE_Sphere 
	opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph can contain 'sphere' primitives which are defined by the center point
	the radius, two vectors to dictate where tessellation should begin.  
*/
class BBINFILETK_API TK_Sphere : public BBaseOpcodeHandler {
private:
        TK_Sphere( const TK_Sphere& );
        TK_Sphere& operator=(const TK_Sphere&);
	protected:
		unsigned char   m_flags;        /*!< internal use; suboptions */
		float           m_center[3];    /*!< internal use; center */
		float           m_radius;       /*!< internal use; Radius */
		float           m_axis[3];      /*!< internal use; direction vector to the north pole */
		float           m_ortho[3];     /*!< internal use; direction vector to equator at longitude=0 */

	public:
		/*! constructor */
		TK_Sphere ()
			: BBaseOpcodeHandler (TKE_Sphere) { Reset(); }

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the center of the sphere using discrete float values */
		void            SetCenter (float x, float y, float z) alter {
							m_center[0] = x;     m_center[1] = y;     m_center[2] = z;
						}
		/*! Sets the center of the sphere using a float array */
		void            SetCenter (float const * s) alter    { SetCenter (s[0], s[1], s[2]); }
		/*! Returns the center as the address of a float array */
		float const *   GetCenter (void) const                { return m_center;            }


		/*! Sets the radius of the sphere */
		void            SetRadius (float r) alter   { m_radius = r;     }
		/*! Returns the radius of the sphere */
		float           GetRadius (void) const      { return m_radius;  }

		/*! Sets the axis of the sphere using discrete float values */
		void            SetAxis (float x, float y, float z) alter {
							m_axis[0] = x;     m_axis[1] = y;     m_axis[2] = z;
							if (x != 0.0f || y != 1.0f || z != 0.0f)
								m_flags &= ~TKSPH_NULL_AXIS;
						}
		/*! Sets the axis of the sphere using a float array */
		void            SetAxis (float const * s) alter    { SetAxis (s[0], s[1], s[2]); }
		/*! Returns the axis as the address of a float array */
		float const *   GetAxis (void) const                { return m_axis;            }


		/*! Sets the ortho of the sphere using discrete float values */
		void            SetOrtho (float x, float y, float z) alter {
							m_ortho[0] = x;     m_ortho[1] = y;     m_ortho[2] = z;
							if (x != 1.0f || y != 0.0f || z != 0.0f)
								m_flags &= ~TKSPH_NULL_AXIS;
						}
		/*! Sets the ortho of the sphere using a float array */
		void            SetOrtho (float const * s) alter    { SetOrtho (s[0], s[1], s[2]); }
		/*! Returns the ortho as the address of a float array */
		float const *   GetOrtho (void) const                { return m_ortho;            }

		/*! \enum Flags
			   Defines various capping options
		*/
		enum Flags {
			TKSPH_NONE		= 0x0,	 //!< no special handling
			TKSPH_NULL_AXIS	= 0x1    //!< omit axis and ortho, in favor of default values of <0,1,0>,<1,0,0>
		};

};


//! Handles the TKE_Cylinder opcode.
/*! 
	TK_Cylinder provides support for writing/reading the TKE_Cylinder 
	opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph can contain 'cylinder' primitives which are defined by two points
	representing the axis, the radius, and flags indicating which ends are 'capped'.
*/
class BBINFILETK_API TK_Cylinder : public BBaseOpcodeHandler {
    private:
        TK_Cylinder(const TK_Cylinder&);
        TK_Cylinder& operator=(const TK_Cylinder&);

	protected:
		float           m_axis[6];      /*!< internal use; Axis endpoints */
		float           m_radius;       /*!< internal use; Radius */
		unsigned char   m_flags;        /*!< internal use; Caps */

	public:
		/*! constructor */
		TK_Cylinder ()
			: BBaseOpcodeHandler (TKE_Cylinder) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the axis of the cylinder using discrete float values */
		void            SetAxis (float x1, float y1, float z1, float x2, float y2, float z2) alter {
							m_axis[0] = x1;     m_axis[1] = y1;     m_axis[2] = z1;
							m_axis[3] = x2;     m_axis[4] = y2;     m_axis[5] = z2;
						}
		/*! Sets the axis of the cylinder using a 2 float triplets */
		void            SetAxis (float const * s, float const * e) alter    { SetAxis (s[0], s[1], s[2], e[0], e[1], e[2]); }
		/*! Sets the axis of the cylinder using a float array */
		void            SetAxis (float const * a) alter     { SetAxis (&a[0], &a[3]);   }
		/*! Returns the axis as the address of a float array */
		float const *   GetAxis (void) const                { return m_axis;            }
		/*! Returns the axis start point as the address of a float array */
		float const *   GetStart (void) const               { return &m_axis[0];        }
		/*! Returns the axis end point as the address of a float array */
		float const *   GetEnd (void) const                 { return &m_axis[3];        }

		/*! Sets the radius of the cylinder */
		void            SetRadius (float r) alter   { m_radius = r;     }
		/*! Returns the radius of the cylinder */
		float           GetRadius (void) const      { return m_radius;  }

		/*! Sets the capping options of the cylinder */
		void            SetCaps (int f) alter       { m_flags = (unsigned char)f;      }
		/*! Returns the capping_options of the cylinder */
		int             GetCaps (void) const        { return m_flags;   }

		/*! \enum Capping_Options
					Defines various capping options
		*/
		enum Capping_Options {
			TKCYL_NONE   = 0,  //!< no caps
			TKCYL_FIRST  = 1,  //!< cap 'first' side
			TKCYL_SECOND = 2,  //!< cap 'second' side 
			TKCYL_BOTH   = 3   //!< cap both sides
		};

};


//! Handles the TKE_PolyCylinder opcode.
/*! 
	TK_PolyCylinder provides support for writing/reading the TKE_PolyCylinder 
	opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph can contain 'polycylinder' primitives which are defined by a series of points
	representing the centerline, radius at each point, and flags indicating which ends are 'capped'.
*/
#include "BPolyhedron.h"

class BBINFILETK_API TK_PolyCylinder : public TK_Polyhedron {
    private:
        TK_PolyCylinder(const TK_PolyCylinder&);
        TK_PolyCylinder& operator=(const TK_PolyCylinder&);

	protected:
		int             m_count;        /*!< internal use; Number of 3D points that define this polycylinder object  */
		float *         m_points;       /*!< internal use; Array of floats which denote the point coordinates. */
		int             m_radius_count; /*!< internal use; Number of radii associated with the points */
		float *         m_radii;        /*!< internal use; Array of floats which denote the radii. */
		unsigned char   m_flags;        /*!< internal use; Caps, normals, colors */
		float           m_normals[6];   /*!< internal use; Normals of end caps */

	public:
		/*! constructor */
		TK_PolyCylinder ()
			: TK_Polyhedron (TKE_PolyCylinder), m_count (0), m_points (0), m_radius_count (0), m_radii (0) {}
		~TK_PolyCylinder();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! \enum Capping_Options
			Cylinder capping options
		*/
		enum Capping_Options {
			TKCYL_NORMAL_FIRST  = 0x04,  //!< cap 'first' side
			TKCYL_NORMAL_SECOND = 0x08,  //!< cap 'second' side
			TKCYL_OPTIONALS     = 0x10   //!< has optionals
		};

		/*!
			Sets the point array.  Allocates a buffer which will hold 'count' float triplets, and if specified,
			copies the 'points' into the buffer
		*/
		void            SetPoints (int count, float const * points = 0) alter;
		/*! Returns the address of the point buffer */
		float const *   GetPoints (void) const              { return m_points;              }
		/*! Returns the address of the point buffer, which may be modified directly */
		float alter *   GetPoints (void) alter              { return m_points;              }
		/*! Returns the number of points in the buffer */
		int             GetCount (void) const               { return m_count;               }


		/*!
			Sets the radius array.  Allocates a buffer which will hold 'count' floats, and if specified,
			copies the 'radii' into the buffer
		*/
		void            SetRadii (int count, float const * radii = 0) alter;
		/*! Convenience function to set a single radius */
		void            SetRadius (float radius) alter      { SetRadii (1, &radius);        }
		/*! Returns the address of the point buffer */
		float const *   GetRadii (void) const               { return m_radii;               }
		/*! Returns the address of the point buffer, which may be modified directly */
		float alter *   GetRadii (void) alter               { return m_radii;               }
		/*! Returns the number of points in the buffer */
		int             GetRadiusCount (void) const         { return m_radius_count;        }


		/*! Sets the capping options of the cylinder */
		void            SetCaps (int f) alter               { m_flags &= ~0x03; m_flags |= f;   }
		/*! Returns the capping options of the cylinder */
		int             GetCaps (void) const                { return m_flags & 0x03;            }

		/*! Sets (or clears) the normal associated with an end of the polycylinder */
		void            SetEndNormal (int index, float const * normal = 0) alter {
							int     mask = 0x40 << index;
							if (normal == 0)
								m_flags &= ~mask;
							else {
								m_flags |= mask;
								m_normals[3*index+0] = normal[0];
								m_normals[3*index+1] = normal[1];
								m_normals[3*index+2] = normal[2];
							}
						}
		/*! Returns the normal associated with an end of the polycylinder (0 if none) */
		float const *   GetEndNormal (int index) const      {
							int     mask = 0x40 << index;
							if (m_flags & mask)
								return &m_normals[3*index];
							else
								return 0;
						}
};


//! Handles the TKE_Grid opcode.
/*! 
	TK_Grid provides support for writing/reading the TKE_Grid opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports 'grid' primitives which are defined by a grid type, origin, and points which define
	grid spacing.
*/
class BBINFILETK_API TK_Grid : public BBaseOpcodeHandler {
    private:
        TK_Grid(const TK_Grid&);
        TK_Grid& operator=(const TK_Grid&);

	protected:
		char            m_type;      /*!< internal use; Type of grid  */
		float           m_origin[3]; /*!< internal use; Grid origin  */
		float           m_ref1[3];   /*!< internal use; First point to the 'right' of the origin  */
		float           m_ref2[3];   /*!< internal use; First point 'above' the origin */
		int             m_counts[2]; /*!< internal use; Number of units along the ref1 and ref2 dimensions */

	public:
		/*! constructor */
		TK_Grid ()
			: BBaseOpcodeHandler (TKE_Grid) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		/*! Sets the origin point of the grid using discrete float values */
		void            SetOrigin (float x, float y, float z) alter {
							m_origin[0] = x;    m_origin[1] = y;    m_origin[2] = z;
						}
		/*! Sets the origin point of the grid using a float triplet */
		void            SetOrigin (float const * o) alter   { SetOrigin (o[0], o[1], o[2]); }
		/*! Returns the origin point of the grid as the address of a float triplet */
		float const *   GetOrigin (void) const              { return m_origin;              }

		/*! Sets the first reference point of the grid using discrete float values */
		void            SetRef1 (float x, float y, float z) alter {
							m_ref1[0] = x;      m_ref1[1] = y;      m_ref1[2] = z;
						}
		/*! Sets the first reference point of the grid using a float triplet */
		void            SetRef1 (float const * r) alter     { SetRef1 (r[0], r[1], r[2]);   }
		/*! Returns the first reference point of the grid as the address of a float triplet */
		float const *   GetRef1 (void) const                { return m_ref1;                }

		/*! Sets the second reference point of the grid using discrete float values */
		void            SetRef2 (float x, float y, float z) alter {
							m_ref2[0] = x;      m_ref2[1] = y;      m_ref2[2] = z;
						}
		/*! Sets the second reference point of the grid using a float triplet */
		void            SetRef2 (float const * r) alter     { SetRef2 (r[0], r[1], r[2]);   }
		/*! Returns the second reference point of the grid as the address of a float triplet */
		float const *   GetRef2 (void) const                { return m_ref2;                }

		/*! Sets the extent counts of the grid using discrete integers */
		void            SetCounts (int c1, int c2) alter    {
							m_counts[0] = c1;   m_counts[1] = c2;
						}
		/*! Returns the extent counts of the grid as the address of a pair of integers */
		int const *     GetCounts (void) const              { return m_counts;              }

		/*! Sets the type of the grid */
		void            SetType (int t)                     { m_type = (char)t;             }
		/*! Returns the type of the grid */
		int             GetType (void) const                { return (int)m_type;           }
};

////////////////////////////////////////////////////////////////////////////////

/*! \enum TKO_Text_Encodings
		Defines text encoding types; refer to ::HC_Insert_Text
*/
enum TKO_Text_Encodings {
	TKO_Enc_ISO_Latin_One,	//!< self-explanatory
	TKO_Enc_ISO_Latin,		//!< self-explanatory
	TKO_Enc_JEC,			//!< self-explanatory
	TKO_Enc_EUC,			//!< self-explanatory
	TKO_Enc_Raw_16,			//!< self-explanatory
	TKO_Enc_Unicode,		//!< self-explanatory
	TKO_Enc_Unicode32		//!< self-explanatory
};

/*! \enum TKO_Text_Options
		Defines text option types; refer to ::HC_Set_Text_Region
*/
enum TKO_Text_Options {
	TKO_Text_Option_Region					= 0x01,	//!< self-explanatory
	TKO_Text_Option_Character_Attributes	= 0x02	//!< self-explanatory
};

/*! \enum TKO_Text_Region_Options
		Defines text region option types
*/
enum TKO_Text_Region_Options {
	TKO_Text_Region_Window		= 0x01,	//!< window space
	TKO_Text_Region_Relative	= 0x02, //!< relative sizing
	TKO_Text_Region_Adjusted	= 0x04, //!< adjust region left-to-right
	TKO_Text_Region_Center		= 0x08, //!< text centered across region
	TKO_Text_Region_Top			= 0x10  //!< top of text is along region
};


/*! \enum TKO_Character_Attributes
		Specifies the set of attributes that can be set on individual characters
*/
enum TKO_Character_Attributes {
	TKO_Character_Name				= 0x0001,  //!< the font name
	TKO_Character_Size				= 0x0002,  //!< the character size
	TKO_Character_Vertical_Offset	= 0x0004,  //!< the offset from the standard position
	TKO_Character_Omitted			= 0x0008,  //!< character is skipped
	TKO_Character_Invisible			= 0x0010,  //!< character is invisible
	TKO_Character_Slant				= 0x0020,  //!< slant, specified in degrees clockwise
	TKO_Character_Width_Scale		= 0x0040,  //!< adjustment to character width
	TKO_Character_Rotation			= 0x0080,  //!< rotation, specified in degrees clockwise
	TKO_Character_Rotation_Fixed	= 0x0100,
	TKO_Character_Extended			= 0x8000   //!< room for expansion
};

/*!  defines all of the attributes that can be set on individual characters */
struct TK_Character_Attribute {
	char *				name;					//!< the font name

	float				size;					//!< the size.  Units are specified separately in size_units
	float				vertical_offset;		//!< offset, positive or negative, from the standard position.  units are specified separately in vertical_offset_units
	float				slant;					//!< the angle (in degrees) that text is slanted (e.g. for italic).  Positive numbers correspond to clockwise.
	float				rotation;				//!< the angle (in degrees) that text is rotated
	float				width_scale;			//!< adjustment to character width

	unsigned short		mask;					//!< specifies which settings are active (i.e. the attributes for which we have an opinion at this point) 
	unsigned short		value;					//!< for active settings, on or off

	unsigned char		size_units;				//!< specified with enum TKO_Font_Size_Units
	unsigned char		vertical_offset_units;	//!< specified with enum TKO_Font_Size_Units
};


//! Handles the TKE_Text and TKE_Text_With_Encoding opcodes.
/*! 
	TK_Text provides support for writing/reading the TKE_Text and TKE_Text_With_Encoding opcode objects to/from an HSF file. 

	The HOOPS/3dGS scene-graph can contain 'text' primitives which are defined by a position and character string,
	and possibly by an encoding value for multibyte (non-ASCII) text.
*/
class BBINFILETK_API TK_Text : public BBaseOpcodeHandler {
    private:
        TK_Text(const TK_Text&);
        TK_Text& operator=(const TK_Text&);

	protected:
		float           m_position[3];  /*!< internal use; Insertion point */
		int             m_length;       /*!< internal use; Length of text string */
		int             m_allocated;    /*!< internal use; Size of allocated string buffer */
		char *          m_string;       /*!< internal use; Text string */
		unsigned char   m_encoding;     /*!< internal use; Encoding of characters (only used by TKE_Text_With_Encoding) */
		unsigned char	m_options;		/*!< internal use: Additional options such as attached Text_Region */
		unsigned char	m_region_options;/*!< internal use: Text Region Controls */
		unsigned char	m_region_count;	/*!< internal use: Text Region Point Count */
		float			m_region[4*3];	/*!< internal use: Text Region Points */
		int				m_count;		/*!< internal use: Number of characters (length is number of bytes to store)  */
		TK_Character_Attribute	*m_character_attributes;	/*!< internal use: Character Attribute array */
		int				m_substage;		/*!< internal use: write/read progress */
		int				m_tmp;			/*!< internal use: temp value */


		void    set_string (char const * string) alter;     //!< for internal use only
		void    set_string (int length) alter;              //!< for internal use only

	public:
		/*! constructor */
		TK_Text (unsigned char opcode);
		~TK_Text();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the text string.  Allocates the buffer and copies the string */
		void            SetString (char const * string) alter   { set_string (string);  }
		/*! Sets the text string using Unicode data.  Allocates the buffer and copies the string, and sets encoding */
		void            SetString (unsigned short const * string) alter;
		/*! Sets the text string using wide Unicode data.  Allocates the buffer and copies the string, and sets encoding */
		void            SetString (unsigned int const * string) alter;
		/*! Sets the text string buffer.  Allocates a buffer large enough to hold a string of 'length' bytes */
		void            SetString (int length) alter            { set_string (length);  }
		/*! Returns the text string (if encoding is a form of Unicode, the pointer should be recast to the correct type) */
		char const *    GetString (void) const                  { return m_string;      }
		/*! Returns the text string buffer, which may be modified directly (if encoding is a form of Unicode, the pointer should be recast to the correct type) */
		char alter *    GetString (void) alter                  { return m_string;      }

		/*! Sets the text position using discrete float values */
		void            SetPosition (float x, float y, float z) alter
										{ m_position[0] = x;  m_position[1] = y;  m_position[2] = z;    }
		/*! Sets the text position using a float triplet */
		void            SetPosition (float const * p) alter     { SetPosition (p[0], p[1], p[2]);       }
		/*! Returns the text position as the address of a float triplet */
		float const *   GetPosition (void) const                { return &m_position[0];                }

		/*! Set the type of encoding for this string.  Encodings are specified by #TKO_Text_Encodings. */
		void            SetEncoding (int e) alter               { m_encoding = (unsigned char)e;        }
		/*! Returns the type of encoding for this string.  Encodings are specified by #TKO_Text_Encodings. */
		int             GetEncoding (void) const                { return (int)m_encoding;               }

		/*! Set the text region for this string. */
		void            SetTextRegion (int c, float const * p, int o=0) alter;
		/*! Returns the number of points defining the text region. */
		int             GetTextRegionCount (void) const         { return (int)m_region_count;           }
		/*! Returns the points defining the text region. */
		float const *   GetTextRegionPoints (void) const        { return m_region;			}
		/*! Returns the options for the text region. */
		int             GetTextRegionOptions (void) const       { return (int)m_region_options;         }
};

////////////////////////////////////////////////////////////////////////////////

//! Handles the TKE_Font opcodes.
/*! 
	TK_Font provides support for writing/reading the TKE_Font opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph can contain user-defined 'fonts' which provide a desscription of how to draw
	individual characters in text string.
*/
enum TKO_Font_Type {
	TKO_Font_HOOPS_Stroked      // data represents a HOOPS stroked font definition
};


//! Handles the TKE_Font opcode.
/*! 
	TK_Font provides support for writing/reading the TKE_Font opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports user-defined fonts.  Refer to the function ::HC_Define_Font
	in the HOOPS/3dGS Reference Manual
*/
class BBINFILETK_API TK_Font : public BBaseOpcodeHandler {
    private:
        TK_Font(const TK_Font&);
        TK_Font& operator=(const TK_Font&);

	protected:
		char *          m_name;         /*!< internal use; Name of the font */
		char *          m_lookup;       /*!< internal use; Name of font character lookup function */
		char *          m_bytes;        /*!< internal use; Font description data */
		int             m_name_length;  /*!< internal use; Length of name */
		int             m_lookup_length;/*!< internal use; Length of lookup name */
		int             m_length;       /*!< internal use; Length font description */
		unsigned char   m_type;         /*!< internal use; Type of font */
		unsigned char   m_encoding;     /*!< internal use; Encoding associated with the font */

		//! internal use
		void    set_bytes (int size, char const * bytes = 0) alter;
		//! internal use
		void    set_name (char const * string) alter;
		//! internal use
		void    set_name (int length) alter;
		//! internal use
		void    set_lookup (char const * string) alter;
		//! internal use
		void    set_lookup (int length) alter;

	public:
		/*! constructor */
		TK_Font () : BBaseOpcodeHandler (TKE_Font),
			m_name (0), m_lookup (0), m_bytes (0), m_name_length (0), m_lookup_length (0), m_length (0),
			m_type (0), m_encoding (0) {}
		~TK_Font();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;


		/*! Set the type of the font (HOOPS stroked font definition is the only one supported so far) */
		void            SetType (int t) alter                               { m_type = (unsigned char)t;}
		/*! Returns the type of the font */
		int             GetType (void) const                                { return (int)m_type;       }

		/*!
			Sets the font data.  Allocates a buffer which can hold 'size' bytes, and if specified,
			copies the data from 'bytes' to the buffer
		*/
		void            SetBytes (int size, char const * bytes = 0) alter   { set_bytes (size, bytes);  }
		/*! Returns the size of the data buffer */
		int             GetBytesCount (void) const                          { return m_length;          }
		/*! Returns the address of the data buffer */
		char const *    GetBytes (void) const                               { return m_bytes;           }
		/*! Returns the address of the data buffer, which may be modified directly */
		char alter *    GetBytes (void) alter                               { return m_bytes;           }

		/*! Sets the name of the font.  Allocates the buffer and copies the string */
		void            SetName (char const * string) alter                 { set_name (string);        }
		/*! Sets the name buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetName (int length) alter                          { set_name (length);        }
		/*! Returns the font name string */
		char const *    GetName (void) const                                { return m_name;            }
		/*! Returns the font name string buffer, which may be modified directly */
		char alter *    GetName (void) alter                                { return m_name;            }

		/*! Sets the name of the lookup function.  Allocates the buffer and copies the string */
		void            SetLookup (char const * string) alter               { set_lookup (string);      }
		/*! Sets the lookup name buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetLookup (int length) alter                        { set_lookup (length);      }
		/*! Returns the lookup function name string */
		char const *    GetLookup (void) const                              { return m_lookup;          }
		/*! Returns the lookup name string buffer, which may be modified directly */
		char alter *    GetLookup (void) alter                              { return m_lookup;          }

		/*! Set the type of encoding for this font */
		void            SetEncoding (int e) alter                       { m_encoding = (unsigned char)e;}
		/*! Returns the type of encoding for this font */
		int             GetEncoding (void) const                        { return (int)m_encoding;       }
};

////////////////////////////////////////////////////////////////////////////////

/*! \enum TKO_Image_Formats
		Defines image formats; refer to ::HC_Insert_Image
*/
enum TKO_Image_Formats {
	TKO_Image_Mapped    = 0,         //!< 8-bit colormap indices
	TKO_Image_Mapped_16 = 1,         //!< 16-bit colormap indices
	TKO_Image_RGB       = 2,         //!< self-explanatory
	TKO_Image_RGBA      = 3,         //!< self-explanatory
	TKO_Image_BGRA      = 4,         //!< self-explanatory
	TKO_Image_DEPTH     = 5,         //!< z values, 32-bit floats in [0..1] range
	TKO_Image_Format_Mask   = 0x0F,  //!< format mask

	TKO_Image_Explicit_Size = 0x10,         //!< self-explanatory
	TKO_Image_Local_Texture = 0x20,         //!< self-explanatory
	TKO_Image_Is_Named      = 0x80,         //!< self-explanatory
	TKO_Image_Options_Mask  = 0xF0,         //!< options mask

	TKO_Image_Invalid       = 0xFF          //!< self-explanatory
};

//! Specifies the number of bytes per pixel for each format
extern const int TK_Image_Bytes_Per_Pixel[];

/*! \enum TKO_Compression
		Defines compression type
*/
enum TKO_Compression {
	TKO_Compression_None = 0,	//!< self-explanatory
	TKO_Compression_RLE  = 1,	//!< self-explanatory
	TKO_Compression_JPEG = 2,	//!< self-explanatory
	TKO_Compression_Reference = 99	//!< empty placeholder image, no real data, possible reference instead
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS

class BBINFILETK_API2 TK_Image_Data_Buffer {
	protected:
		unsigned char *                 m_buffer;	//!< internal use
		unsigned int                    m_allocated;	//!< internal use
		unsigned int                    m_used;		//!< internal use

	public:
		/*! constructor */
		TK_Image_Data_Buffer() : m_buffer (0),  m_allocated (0), m_used (0) {}
		~TK_Image_Data_Buffer();

		void    Resize (unsigned int size) alter;
		void    Expand (unsigned int size) alter        { Resize (Size() + size);   }
		void    Reset (void) alter;

		unsigned int const &     Size (void) const      { return m_allocated;   }
		unsigned int const &     Used (void) const      { return m_used;        }
		unsigned int alter &     Used (void) alter      { return m_used;        }
		unsigned char const *    Buffer (void) const    { return m_buffer;      }
		unsigned char alter *    Buffer (void) alter    { return m_buffer;      }
};

#endif /* DOXYGEN_SHOULD_SKIP_THIS */


//! Handles the TKE_Image opcode.
/*! 
	TK_Image provides support for writing/reading the TKE_Image opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports 'image' primitives which are defined by a position, size (width and height),
	and an array of bytes which is interpretted according to the specified format.
*/
class BBINFILETK_API TK_Image : public BBaseOpcodeHandler {
    private:
        TK_Image(const TK_Image&);
        TK_Image& operator=(const TK_Image&);

	protected:
		char *                  m_bytes;        /*!< internal use; Image data */
		char *                  m_name;         /*!< internal use; name applied to image (if any) */
		char *                  m_reference;    /*!< internal use; external reference to image data (if any) */
		float                   m_position[3];  /*!< internal use; Insertion point */
		int                     m_size[2];      /*!< internal use; Width & Height */
		int                     m_name_length;  /*!< internal use; Length of name */
		int                     m_reference_length;	/*!< internal use; Length of reference */
		unsigned char           m_format;       /*!< internal use; Format of data */
		unsigned char			m_options;	/*!< internal use; Extra options (named, explicit size) */
		unsigned char           m_compression;  /*!< internal use; How (or if) data is compressed in stream */
		unsigned char           m_bytes_format; /*!< internal use; Indicates if input data is already compressed */
		float					m_explicit_size[2];	/*!< internal use; non-pixel sizing */
		unsigned char			m_explicit_units[2];	/*!< internal use; non-pixel units */
		TK_Image_Data_Buffer    m_work_area[2];    /*!< internal use; Buffers for compression data */

		//! internal use
		void    set_data (int size, char const * bytes = 0, unsigned char data_format = TKO_Compression_None) alter;
		//! internal use
		void    set_name (char const * string) alter;
		//! internal use
		void    set_name (int length) alter;

		//! internal use
		TK_Status compress_image (BStreamFileToolkit & tk, int active_work_area = 0) alter;
		//! internal use
		TK_Status decompress_image (BStreamFileToolkit & tk, int active_work_area = 0) alter;
		//! internal use
		TK_Status read_jpeg_header (void) alter;

	public:
		/*! constructor */
		TK_Image ();
		~TK_Image();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		TK_Status   compress_image_ascii (BStreamFileToolkit & tk) alter;


		void        Reset (void) alter;

		/*!
			Sets the image data.  Allocates a buffer which can hold 'size' bytes, and if specified,
			copies the data from 'bytes' to the buffer
		*/
		void            SetBytes (int size, char const * bytes = 0,
								  unsigned char data_format = TKO_Compression_None) alter
																			{ set_data (size, bytes, data_format);  }
		/*! Returns the address of the data buffer */
		char const *    GetBytes (void) const                               { return m_bytes;           }
		/*! Returns the address of the data buffer, which may be modified directly */
		char alter *    GetBytes (void) alter                               { return m_bytes;           }

		/*! Sets the name of the image.  Allocates the buffer and copies the string */
		void            SetName (char const * string) alter                 { set_name (string);        }
		/*! Sets the name buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetName (int length) alter                          { set_name (length);        }
		/*! Returns the image name string */
		char const *    GetName (void) const                                { return m_name;            }
		/*! Returns the image name string buffer, which may be modified directly */
		char alter *    GetName (void) alter                                { return m_name;            }

		/*! Sets the reference to the image data.  Allocates the buffer and copies the string */
		void            SetReference (char const * string) alter;
		/*! Sets the reference buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetReference (int length) alter;
		/*! Returns the reference string */
		char const *    GetReference (void) const                           { return m_reference;       }
		/*! Returns the reference string buffer, which may be modified directly */
		char alter *    GetReference (void) alter                           { return m_reference;       }

		/*! Sets the text position using discrete float values */
		void            SetPosition (float x, float y, float z) alter
										{ m_position[0] = x;  m_position[1] = y;  m_position[2] = z;    }
		/*! Sets the text position using a float triplet */
		void            SetPosition (float const * p) alter     { SetPosition (p[0], p[1], p[2]);       }
		/*! Returns the text position as the address of a float triplet */
		float const *   GetPosition (void) const                { return &m_position[0];                }

		/*! Sets the size of the image using discrete integer values */
		void            SetSize (int w, int h) alter            { m_size[0] = w; m_size[1] = h;         }
		/*! Sets the size of the image using an array of integer values */
		void            SetSize (int const * s) alter           { m_size[0] = s[0]; m_size[1] = s[1];   }
		/*! Returns the size of the image as the address of a pair of integers */
		int const *     GetSize (void) const                    { return m_size;                        }

		/*! Sets the format of the image.  Formats are specified in #TKO_Image_Formats. */
		void            SetFormat (int f) alter                 { m_format = (unsigned char)(f & TKO_Image_Format_Mask);    }
		/*! Returns the format of the image.  Formats are specified in #TKO_Image_Formats. */
		int             GetFormat (void) const                  { return (int)m_format;			}

		/*! Sets the options of the image.  Options are specified in #TKO_Image_Formats. */
		void            SetOptions (int f) alter                 { m_options = (unsigned char)(f & TKO_Image_Options_Mask); }
		/*! Returns the options of the image.  Options are specified in #TKO_Image_Formats. */
		int             GetOptions (void) const                  { return (int)m_options;		}

		/*! Sets how the image data is compressed.  Compression types are specified in #TKO_Compression. */
		void            SetCompression (int c) alter            { m_compression = (unsigned char)c;     }
		/*! Returns how the image data is compressed.  Compression types are specified in #TKO_Compression. */
		int             GetCompression (void) const             { return (int)m_compression;            }
};


////////////////////////////////////////////////////////////////////////////////


/*! \enum TKO_Texture_Option_Bits
		Defines texture options; refer to ::HC_Define_Texture
*/
enum TKO_Texture_Option_Bits {
	TKO_Texture_Param_Source	= 0x00000001,   //!< refer to ::HC_Define_Texture
	TKO_Texture_Tiling			= 0x00000002,   //!< ""
	TKO_Texture_Interpolation	= 0x00000004,   //!< ""
	TKO_Texture_Decimation		= 0x00000008,   //!< ""
	TKO_Texture_Red_Mapping		= 0x00000010,   //!< ""
	TKO_Texture_Green_Mapping	= 0x00000020,   //!< ""
	TKO_Texture_Blue_Mapping	= 0x00000040,   //!< ""
	TKO_Texture_Alpha_Mapping	= 0x00000080,   //!< ""
	TKO_Texture_Param_Function	= 0x00000100,   //!< ""
	TKO_Texture_Layout			= 0x00000200,   //!< ""
	TKO_Texture_Transform		= 0x00000400,   //!< ""
	TKO_Texture_Value_Scale		= 0x00000800,   //!< ""
	TKO_Texture_Caching			= 0x00001000,   //!< ""
	TKO_Texture_DownSample		= 0x00002000,   //!< ""
	TKO_Texture_No_DownSample	= 0x00004000,   //!< ""
	TKO_Texture_Extended		= 0x00008000,   //!< extended bit
	TKO_Texture_Extended_Mask	= 0xFFFF0000,   //   internal use, indicates bit which require TKO_Texture_Extended
	TKO_Texture_Decal			= 0x00010000,   //!< ""
	TKO_Texture_Modulate		= 0x00020000,   //!< ""
	TKO_Texture_Param_Offset	= 0x00040000    //!< ""
};

/*! \enum TKO_Texture_Param_Sources
		Defines texture parameterization sources; refer to ::HC_Define_Texture
*/
enum TKO_Texture_Param_Sources {
	TKO_Texture_Param_Source_U,   //!< refer to ::HC_Define_Texture
	TKO_Texture_Param_Source_UV,   //!< ""
	TKO_Texture_Param_Source_UVW,   //!< ""
	TKO_Texture_Param_Source_Object,   //!< ""
	TKO_Texture_Param_Source_World,   //!< ""
	TKO_Texture_Param_Source_Surface_Normal,   //!< ""
	TKO_Texture_Param_Source_Reflection_Vector,   //!< ""
	TKO_Texture_Param_Source_Natural_UV,   //!< ""
	TKO_Texture_Param_Source_Local_Pixels,   //!< ""
	TKO_Texture_Param_Source_Outer_Pixels,   //!< ""
	TKO_Texture_Param_Source_Local_Window,   //!< ""
	TKO_Texture_Param_Source_Outer_Window,   //!< ""
	TKO_Texture_Param_Source_Transmission_Vector,   //!< ""
	TKO_Texture_Param_Source_Sphere_Map,   //!< ""
	TKO_Texture_Param_Source_Cylinder_Map,  //!< ""
	TKO_Texture_Param_Source_Physical_Reflection_Vector   //!< ""
};


/*! \enum TKO_Texture_Param_Functions
		Defines texture parameterization functions; refer to ::HC_Define_Texture
*/
enum TKO_Texture_Param_Functions {
	TKO_Texture_Param_Function_None,   //!< refer to ::HC_Define_Texture
	TKO_Texture_Param_Function_Sphere,   //!< ""
	TKO_Texture_Param_Function_Cylinder,   //!< ""
	TKO_Texture_Param_Function_Box   //!< ""
};


/*! \enum TKO_Texture_Layouts
		Defines texture layouts; refer to ::HC_Define_Texture
*/
enum TKO_Texture_Layouts {
	TKO_Texture_Layout_Rectilinear,   //!< refer to ::HC_Define_Texture
	TKO_Texture_Layout_Spherical,   //!< ""
	TKO_Texture_Layout_Hemispherical,   //!< ""
	TKO_Texture_Layout_Cubic_Faces   //!< ""
};

/*! \enum TKO_Texture_Tilings
		Defines texture tilings; refer to ::HC_Define_Texture
*/
enum TKO_Texture_Tilings {
	TKO_Texture_Tiling_None,   //!< refer to ::HC_Define_Texture
	TKO_Texture_Tiling_Clamp,   //!< ""
	TKO_Texture_Tiling_Repeat,   //!< ""
	TKO_Texture_Tiling_Mirror   //!< ""
};


/*! \enum TKO_Texture_Filters
		Defines texture interpolation filters; refer to ::HC_Define_Texture
*/
enum TKO_Texture_Filters {
	TKO_Texture_Filter_None,   //!< refer to ::HC_Define_Texture
	TKO_Texture_Filter_Bilinear,   //!< ""
	TKO_Texture_Filter_Trilinear,   //!< ""
	TKO_Texture_Filter_MipMap,   //!< ""
	TKO_Texture_Filter_Summed_Areas,   //!< ""
	TKO_Texture_Filter_Gaussian,   //!< ""
	TKO_Texture_Filter_Stochastic,   //!< ""
	TKO_Texture_Filter_Anisotropic	//!< ""
};


/*! \enum TKO_Texture_Channel_Mappings
		Defines texture channel mappings; refer to ::HC_Define_Texture
*/
enum TKO_Texture_Channel_Mappings {
	TKO_Texture_Channel_Mapping_Red,   //!< refer to ::HC_Define_Texture
	TKO_Texture_Channel_Mapping_Green,   //!< ""
	TKO_Texture_Channel_Mapping_Blue,   //!< ""
	TKO_Texture_Channel_Mapping_Alpha,   //!< ""
	TKO_Texture_Channel_Mapping_Zero,   //!< ""
	TKO_Texture_Channel_Mapping_One,   //!< ""
	TKO_Texture_Channel_Mapping_Luminance,   //!< ""
	TKO_Texture_Channel_Mapping_None   //!< ""
};


/*! \enum TKO_Texture_Application_Modes
		Defines texture decal & modulatge settings; refer to ::HC_Define_Texture
*/
enum TKO_Texture_Application_Modes {
	TKO_Texture_Modulate_Set	= 0x01,	    //!< refer to ::HC_Define_Texture
	TKO_Texture_Decal_Set	= 0x02	    //!< ""
};


//! Handles the TKE_Texture opcode.
/*! 
	TK_Texture provides support for writing/reading the TKE_Texture opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports 'textures', which define an image that can be applied to a HOOPS/3dGS
	shell or mesh primitive.
*/
class BBINFILETK_API2 TK_Texture : public BBaseOpcodeHandler {
    private:
        TK_Texture(const TK_Texture&);
        TK_Texture& operator=(const TK_Texture&);

	protected:
		int             m_name_length;      /*!< internal use; current name length */
		int             m_image_length;     /*!< internal use; Current image name length */
		char *          m_name;             /*!< internal use; Texture name */
		char *          m_image;            /*!< internal use; Source image name */
		int             m_flags;            /*!< internal use; Flags indicating presence of optional values */
		int				m_substage;			/*!< internal use: write/read progress */

		char            m_param_source;     /*!< internal use; Source of parameters used for texture lookup */
		char            m_interpolation;    /*!< internal use; Method of pixel interpolation (pixel falls between texture samples) */
		char            m_decimation;       /*!< internal use; Method of pixel decimation (pixel covers large portion of texture) */
		char            m_red_mapping;      /*!< internal use; Method for remapping red channel value */
		char            m_green_mapping;    /*!< internal use; Method for remapping green channel value */
		char            m_blue_mapping;     /*!< internal use; Method for remapping blue channel value */
		char            m_alpha_mapping;    /*!< internal use; Method for remapping alpha channel value */
		char            m_param_function;   /*!< internal use; Basic parameter remapping methods */
		char            m_layout;           /*!< internal use; Format of "picture" within source image */
		char            m_tiling;           /*!< internal use; Handling of parametric values outside 0 to 1 range */
		float           m_value_scale[2];   /*!< internal use; "Brightness & Contrast" scaling of texture values*/
		char *          m_transform;        /*!< internal use; Segment containing transform matrix */
		char			m_apply_mode;	    /*!< internal use; bits showing setting of decal and/or modulate */
		char			m_param_offset;	    /*!< internal use; offset into parameter value array */

		void    set_name (int length) alter;                    //!< for internal use only
		void    set_name (char const * name) alter;             //!< for internal use only
		void    set_image (int length) alter;                   //!< for internal use only
		void    set_image (char const * image) alter;           //!< for internal use only
		void    set_transform (int length) alter;               //!< for internal use only
		void    set_transform (char const * transform) alter;   //!< for internal use only

	public:
		/*! constructor */
		TK_Texture () : BBaseOpcodeHandler (TKE_Texture),
			m_name_length (0), m_image_length (0),
			m_name (0), m_image (0),
			m_transform (0) {
				Reset();
			}
		~TK_Texture();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the texture name.  Allocates the buffer and copies the name string */
		void            SetName (char const * name) alter           { set_name (name);              }
		/*! Sets the texture name buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetName (int length) alter                  { set_name (length);            }
		/*! Returns the texture name */
		char const *    GetName (void) const                        { return m_name;                }
		/*! Returns the texture name buffer, which can be modified directly */
		char alter *    GetName (void) alter                        { return m_name;                }

		/*! Sets the image name.  Allocates the buffer and copies the name string */
		void            SetImage (char const * image) alter         { set_image (image);            }
		/*! Sets the image name buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetImage (int length) alter                 { set_image (length);           }
		/*! Returns the image name */
		char const *    GetImage (void) const                       { return m_image;               }
		/*! Returns the image name buffer, which can be modified directly */
		char alter *    GetImage (void) alter                       { return m_image;               }

		/*! Sets the mask indicating which texture options are specified.  Options are defined in #TKO_Texture_Option_Bits. */
		void            SetFlags (int f) alter  {
							m_flags = f;
							if ((f & TKO_Texture_Extended_Mask) != 0)
								m_flags |= TKO_Texture_Extended;
						}
		/*! Returns the mask indicating which texture options are specified.  Options are defined in #TKO_Texture_Option_Bits. */
		int             GetFlags (void) const                       { return m_flags;		    }

		/*! Sets the parameterization source.  Options are defined in #TKO_Texture_Param_Sources */
		void            SetParameterSource (int p) alter            { m_param_source = (char)p;     }
		/*! Returns the parameterization source.  Options are defined in #TKO_Texture_Param_Sources */
		int             GetParameterSource (void) const             { return (int)m_param_source;   }

		/*! Sets the interpolation filter.  Filter types are defined in #TKO_Texture_Filters. */
		void            SetInterpolation (int p) alter              { m_interpolation = (char)p;    }
		/*! Returns the interpolation filter.  Filter types are defined in #TKO_Texture_Filters. */
		int             GetInterpolation (void) const               { return (int)m_interpolation;  }

		/*! Sets the decimation filter.  Filter types are defined in #TKO_Texture_Filters. */
		void            SetDecimation (int p) alter                 { m_decimation = (char)p;       }
		/*! Returns the decimation filter.  Filter types are defined in #TKO_Texture_Filters. */
		int             GetDecimation (void) const                  { return (int)m_decimation;     }

		/*! Sets the red channel mapping.  Mappings are defined in #TKO_Texture_Channel_Mappings. */
		void            SetRedMapping (int p) alter                 { m_red_mapping = (char)p;      }
		/*! Returns the red channel mapping.  Mappings are defined in #TKO_Texture_Channel_Mappings. */
		int             GetRedMapping (void) const                  { return (int)m_red_mapping;    }

		/*! Sets the green channel mapping.  Mappings are defined in #TKO_Texture_Channel_Mappings. */
		void            SetGreenMapping (int p) alter               { m_green_mapping = (char)p;    }
		/*! Returns the green channel mapping.  Mappings are defined in #TKO_Texture_Channel_Mappings. */
		int             GetGreenMapping (void) const                { return (int)m_green_mapping;  }

		/*! Sets the blue channel mapping.  Mappings are defined in #TKO_Texture_Channel_Mappings. */
		void            SetBlueMapping (int p) alter                { m_blue_mapping = (char)p;     }
		/*! Returns the blue channel mapping.  Mappings are defined in #TKO_Texture_Channel_Mappings. */
		int             GetBlueMapping (void) const                 { return (int)m_blue_mapping;   }

		/*! Sets the alpha channel mapping.  Mappings are defined in #TKO_Texture_Channel_Mappings. */
		void            SetAlphaMapping (int p) alter               { m_alpha_mapping = (char)p;    }
		/*! Returns the alpha channel mapping.  Mappings are defined in #TKO_Texture_Channel_Mappings. */
		int             GetAlphaMapping (void) const                { return (int)m_alpha_mapping;  }

		/*! Sets the parameterization function.  Functions are defined in #TKO_Texture_Param_Functions. */
		void            SetParameterFunction (int p) alter          { m_param_function = (char)p;   }
		/*! Returns the parameterization function.  Functions are defined in #TKO_Texture_Param_Functions. */
		int             GetParameterFunction (void) const           { return (int)m_param_function; }

		/*! Sets the layout of the data in the image.  Layouts are specified in #TKO_Texture_Layouts. */
		void            SetLayout (int p) alter                     { m_layout = (char)p;           }
		/*! Returns the layout of the data in the image.  Layouts are specified in #TKO_Texture_Layouts. */
		int             GetLayout (void) const                      { return (int)m_layout;         }

		/*! Sets the texture tiling mode.  Tiling modes are specified in #TKO_Texture_Tilings. */
		void            SetTiling (int p) alter                     { m_tiling = (char)p;           }
		/*! Returns the texture tiling mode.  Tiling modes are specified in #TKO_Texture_Tilings. */
		int             GetTiling (void) const                      { return (int)m_tiling;         }

		/*! Sets the value scale & offset factors */
		void            SetValueScale (float v1, float v2) alter    { m_value_scale[0] = v1; m_value_scale[1] = v2; }
		/*! Returns the value scale & offset factors */
		float const *   GetValueScale (void) const                  { return m_value_scale;         }

		/*! Sets the decal & modulate modes.  Modes are specified in TKO_Texture_Application_Modes. */
		void            SetApplicationMode (int p) alter            { m_apply_mode = (char)p;       }
		/*! Returns the decal & modulate mode.  Tiling modes are specified in TKO_Texture_Application_Modes. */
		int             GetApplicationMode (void) const             { return (int)m_tiling;         }

		/*! Sets the parameter offset. */
		void            SetParameterOffset (int p) alter            { m_param_offset = (char)p;     }
		/*! Returns the parameter offset. */
		int             GetParameterOffset (void) const             { return (int)m_param_offset;   }

		/*!
			Sets the name of the segment containing a texture transform matrix.  Allocates the buffer and
			copies the string
		*/
		void            SetTransform (char const * transform) alter { set_transform (transform);    }
		/*!
			Sets the buffer to contain the name of a transform segment.  Allocates a buffer large enough
			to hold 'length' characters
		*/
		void            SetTransform (int length) alter             { set_transform (length);       }
		/*! Returns the transform segment name */
		char const *    GetTransform (void) const                   { return m_transform;           }
		/*! Returns the transform segment name buffer, which may be modified directly */
		char alter *    GetTransform (void) alter                   { return m_transform;           }
};


/*! \enum TKO_Thumbnail_Formats
		Defines thumbnail formats
*/
enum TKO_Thumbnail_Formats {
	TKO_Thumbnail_RGB       = 0,         //!< self-explanatory
	TKO_Thumbnail_RGBA      = 1,         //!< self-explanatory

	TKO_Thumbnail_Invalid   = 0xFF       //!< self-explanatory
};

//! Handles the TKE_Thumbnail opcode.
/*! 
	TK_Thumbnail provides support for writing/reading the TKE_Thumbnail opcode object to/from an HSF file. 

	Many file systems and part viewers provide for a thumbnail -- a small image representing the data.
	This is essentially an extremely simplified version of the TK_Image handler.
*/
class BBINFILETK_API2 TK_Thumbnail : public BBaseOpcodeHandler {
private:
        TK_Thumbnail( const TK_Thumbnail& );
        TK_Thumbnail& operator=(const TK_Thumbnail&);
	protected:
		char *                  m_bytes;        /*!< internal use; Image data */
		int                     m_size[2];      /*!< internal use; Width & Height */
		unsigned char           m_format;       /*!< internal use; Format of data */

	public:
		/*! constructor */
	TK_Thumbnail() : BBaseOpcodeHandler (TKE_Thumbnail), m_bytes (0), m_format (TKO_Thumbnail_Invalid) {}
		~TK_Thumbnail();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		TK_Status   Execute (BStreamFileToolkit & tk) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, int variant) alter;
		TK_Status   Interpret (BStreamFileToolkit & tk, ID_Key key, char const * special) alter
								{ return BBaseOpcodeHandler::Interpret(tk, key, special); }
		void        Reset (void) alter;

		/*!
			Sets the image data.  Allocates a buffer which can hold 'size' bytes, and if specified,
			copies the data from 'bytes' to the buffer
		*/
		void            SetBytes (int size, unsigned char const * bytes = 0) alter;
		/*! Returns the address of the data buffer */
		char const *    GetBytes (void) const                   { return m_bytes;			}
		/*! Returns the address of the data buffer, which may be modified directly */
		char alter *    GetBytes (void) alter                   { return m_bytes;			}

		/*! Sets the size of the thumbnail using discrete integer values */
		void            SetSize (int w, int h) alter            { m_size[0] = w; m_size[1] = h;		}
		/*! Sets the size of the image using an array of integer values */
		void            SetSize (int const * s) alter           { m_size[0] = s[0]; m_size[1] = s[1];	}
		/*! Returns the size of the image as the address of a pair of integers */
		int const *     GetSize (void) const                    { return m_size;			}

		/*! Sets the format of the image.  Formats are specified in #TKO_Thumbnail_Formats. */
		void            SetFormat (int f) alter                 { m_format = (unsigned char)f;		}
		/*! Returns the format of the image.  Formats are specified in #TKO_Thumbnail_Formats. */
		int             GetFormat (void) const                  { return (int)m_format;			}
};


////////////////////////////////////////////////////////////////////////////////

//! Handles the TKE_Glyph_Definition opcode.
/*! 
	TK_Glyph_Definition provides support for writing/reading the TKE_Glyph_Definition opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports 'glyph definitions', which define how to draw markers and components of line patterns.
*/
class BBINFILETK_API2 TK_Glyph_Definition : public BBaseOpcodeHandler {
private:
        TK_Glyph_Definition( const TK_Glyph_Definition& );
        TK_Glyph_Definition& operator=(const TK_Glyph_Definition&);
	protected:
		int             m_name_length;      /*!< internal use; Glyph name length */
		int             m_size;		    /*!< internal use; Data length */
		char *          m_name;             /*!< internal use; Glyph name */
		char *          m_data;		    /*!< internal use; Definition */

	public:
		/*! constructor */
		TK_Glyph_Definition () : BBaseOpcodeHandler (TKE_Glyph_Definition),
			m_name_length (0), m_size (0),
			m_name (0), m_data (0) {}
		~TK_Glyph_Definition();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the glyph name.  Allocates the buffer and copies the name string */
		void            SetName (char const * name) alter;
		/*! Sets the glyph name buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetName (int length) alter;
		/*! Returns the glyph name */
		char const *    GetName (void) const		{ return m_name;	}
		/*! Returns the glyph name buffer, which can be modified directly */
		char alter *    GetName (void) alter		{ return m_name;	}

		/*! Sets the definition.  Allocates the buffer and copies the data if provided */
		void            SetDefinition (int size, char const * data = 0) alter;
		/*! Returns the definition size */
		int		GetDefinitionSize (void) const	{ return m_size;	}
		/*! Returns the definition buffer */
		char const *    GetDefinition (void) const	{ return m_data;	}
		/*! Returns the definition buffer, which can be modified directly */
		char alter *    GetDefinition (void) alter	{ return m_data;	}
};

//! Handles the TKE_Line_Style opcode.
/*! 
	TK_Line_Style provides support for writing/reading the TKE_Line_Style opcode object to/from an HSF file. 

	The HOOPS/3dGS scene-graph supports 'line pattern definitions', which define how to draw lines.
*/
class BBINFILETK_API2 TK_Line_Style : public BBaseOpcodeHandler {
private:
        TK_Line_Style( const TK_Line_Style& );
        TK_Line_Style& operator=(const TK_Line_Style&);
	protected:
		int             m_name_length;      /*!< internal use; Pattern name length */
		int             m_definition_length;/*!< internal use; definition length */
		char *          m_name;             /*!< internal use; Line Pattern name */
		char *          m_definition;	    /*!< internal use; definition */

	public:
		/*! constructor */
		TK_Line_Style () : BBaseOpcodeHandler (TKE_Line_Style),
			m_name_length (0), m_definition_length (0),
			m_name (0), m_definition (0) {}
		~TK_Line_Style();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the pattern name.  Allocates the buffer and copies the name string */
		void            SetName (char const * name) alter;
		/*! Sets the pattern name buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetName (int length) alter;
		/*! Returns the pattern name */
		char const *    GetName (void) const	    { return m_name;		}
		/*! Returns the pattern name buffer, which can be modified directly */
		char alter *    GetName (void) alter	    { return m_name;		}

		/*! Sets the definition.  Allocates the buffer and copies the definition */
		void            SetDefinition (char const * def) alter;
		/*! Sets the definition buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
		void            SetDefinition (int length) alter;
		/*! Returns the definition buffer */
		char const *    GetDefinition (void) const  { return m_definition;	}
		/*! Returns the definition buffer, which can be modified directly */
		char alter *    GetDefinition (void) alter  { return m_definition;	}
};

////////////////////////////////////////////////////////////////////////////////

//! Handles the TKE_Clip_Rectangle opcode.
/*! 
	TK_Clip_Rectangle provides support for writing/reading the TKE_Clip_Rectangle opcode  to/from an HSF file. 
	A clip rectangle defines a 2d (dc) clip rectangle which should be applied to the contents of the current segment and
	all subsegments (just like any HOOPS/3dGS attribute)
*/
class BBINFILETK_API TK_Clip_Rectangle : public BBaseOpcodeHandler {
    private:
        TK_Clip_Rectangle(const TK_Clip_Rectangle&);
        TK_Clip_Rectangle& operator=(const TK_Clip_Rectangle&);

	protected:
		char            m_options;  /*!< internal use; optional settings */
		float           m_rect[4];  /*!< internal use; description of a rectangular clip region */

	public:
		/*! constructor */
		TK_Clip_Rectangle ()
			: BBaseOpcodeHandler (TKE_Clip_Rectangle), m_options (0) {}

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the clip region using individual float values */
		void            SetRectangle (float left, float right, float bottom, float top) alter
							{ m_rect[0] = left;  m_rect[1] = right;  m_rect[2] = bottom;  m_rect[3] = top; }
		/*! Sets the clip region using an array of float values */
		void            SetRectangle (float const * rect) alter
							{ SetRectangle (rect[0], rect[1], rect[2], rect[3]); }
		/*! Returns the address of the rectangle */
		float const *   GetRectangle (void) const       { return m_rect;            }

		/*! Sets the option flags */
		void            SetOptions (int o) alter        { m_options = (char)o;      }
		/*! Returns the option flags */
		int             GetOptions (void) const         { return (int)m_options;    }
};

////////////////////////////////////////////////////////////////////////////////

/*! \enum TKO_Clip_Region_Options
	options for TKE_Clip_Region opcode
*/
enum TKO_Clip_Region_Options {
	TKO_Clip_Region_Window_Space = 0x01   //!< clip region is to be specified in window space {[0..1],[0..1]}. Default is world space
};

//! Handles the TKE_Clip_Region opcodes.
/*! 
	TK_Clip_Region provides support for writing/reading the TKE_Clip_Region opcode  to/from an HSF file.
	A clip region defines a convex clip region which should be applied to the contents of the current segment and
	all subsegments (just like any HOOPS/3dGS attribute)
*/
class BBINFILETK_API TK_Clip_Region : public BBaseOpcodeHandler {
    private:
        TK_Clip_Region(const TK_Clip_Region&);
        TK_Clip_Region& operator=(const TK_Clip_Region&);

	protected:
		char            m_options;  /*!< internal use; type of region and optional settings */
		int             m_count;    /*!< internal use; Number of 3D points that define this clip region object  */
		float *         m_points;   /*!< internal use; Array of floats which denote the point coordinates. */

	public:
		/*! constructor */
		TK_Clip_Region ()
			: BBaseOpcodeHandler (TKE_Clip_Region), m_options (0), m_count (0), m_points (0) {}
		~TK_Clip_Region();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*!
			Sets the point array.  Allocates a buffer which will hold 'count' float triplets, and if specified,
			copies the 'points' into the buffer
		*/
		void            SetPoints (int count, float const * points = 0) alter;
		/*! Returns the address of the point buffer */
		float const *   GetPoints (void) const                                  { return m_points;              }
		/*! Returns the address of the point buffer, which may be modified directly */
		float alter *   GetPoints (void) alter                                  { return m_points;              }
		/*! Returns the number of points in the buffer */
		int             GetCount (void) const                                   { return m_count;               }

		/*! Sets the option flags */
		void            SetOptions (int o) alter        { m_options = (char)o;      }
		/*! Returns the option flags */
		int             GetOptions (void) const         { return (int)m_options;    }
};


////////////////////////////////////////////////////////////////////////////////

//! Handles the TKE_Start_User_Data opcode.
/*! 
	The HOOPS Stream File can contain user-data, which is denoted by TKE_Start_User_Data.  
	TK_User_Data will handle the TKE_Start_User_Data opcode by simply reading the data, and not doing anything with it.  
	Therefore, this class must be overloaded in order to both import (and handle), as well as export user-data.  
    
	To ensure that user data will be gracefully handled (skipped-over) by non-custom handlers (meaning, an application that is 
	using the default toolkit and doesn't understand the custom user data), custom user data must be written out by exporting the 
	following elements in order:

		1.  the TKE_Start_User_Data opcode
		2.  the number of bytes of user data that will follow
		3.  the user data itself
		4.  the TKE_Stop_User_Data_Opcode

	See the HOOPS/Stream Programming Guide for more detailed information about how to handle user data and create custom files.
*/
class BBINFILETK_API2 TK_User_Data : public BBaseOpcodeHandler {
    private:
        TK_User_Data(const TK_User_Data&);
        TK_User_Data& operator=(const TK_User_Data&);

	protected:
		int             m_size;  /*!< internal use; Number of bytes of user data */
		char *          m_data;  /*!< internal use; User data */
		int				m_buffer_size;   /*!< internal use; Real m_data size */

		//! internal use
		void    set_data (int size, char const * bytes = 0) alter;  //!< for internal use only

	public:
		/*! constructor */
		TK_User_Data ()
			: BBaseOpcodeHandler (TKE_Start_User_Data), m_size (0), m_data (0), m_buffer_size(0) {}
		~TK_User_Data();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		TK_Status   Execute (BStreamFileToolkit & tk) alter;
		void        Reset (void) alter;

		/*!
			Sets the data buffer.  Allocates a buffer which can hold 'size' bytes and, if specified, copies
			'bytes' into the buffer
		*/
		void            SetUserData (int size, char const * bytes = 0) alter    { set_data (size, bytes);   }
		/*! Returns the address of the data buffer */
		char const *    GetUserData (void) const                                { return m_data;            }
		/*! Returns the address of the data buffer, which may be modified directly */
		char alter *    GetUserData (void) alter                                { return m_data;            }
		/*! Returns the size of the data buffer */
		int             GetSize (void) const                                    { return m_size;            }

		/*! Change the size of the data buffer */
		void			Resize (int size) alter;  

		/*! Sets the size of the data buffer. */
		void			SetSize (int size) alter;
};

//! Handles the TKE_XML opcode.
/*! 
	The HOOPS Stream File can contain XML code, which is denoted by TKE_XML.  
	TK_XML will handle the TKE_XML opcode by simply reading the data, and not doing anything with it.  
	Therefore, this class must be overloaded in order to both import (and handle), as well as export XML.
*/
class BBINFILETK_API TK_XML : public BBaseOpcodeHandler {
    private:
        TK_XML(const TK_XML&);
        TK_XML& operator=(const TK_XML&);

	protected:
		int             m_size;  /*!< internal use; Number of bytes of XML data */
		char *          m_data;  /*!< internal use; XML data */

	public:
		/*! constructor */
		TK_XML (): BBaseOpcodeHandler (TKE_XML), m_size (0), m_data (0) {}
		~TK_XML();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		TK_Status   Execute (BStreamFileToolkit & tk) alter;
		void        Reset (void) alter;

		/*!
			Sets the data buffer.  Allocates a buffer which can hold 'size' bytes and if, specified, copies
			'bytes' into the buffer
		*/
		void            SetXML (int size, char const * data = 0) alter;
		/*!
			Expands the buffer by 'size' bytes and, if specified, copies 'bytes' into the buffer
		*/
		void            AppendXML (int size, char const * data = 0) alter;
		/*! Returns the address of the data buffer */
		char const *    GetXML (void) const                     { return m_data;            }
		/*! Returns the address of the data buffer, which may be modified directly */
		char alter *    GetXML (void) alter                     { return m_data;            }
		/*! Returns the size of the data buffer */
		int             GetSize (void) const                    { return m_size;            }
};



//! Handles the TKE_URL opcodes.
/*! 
	TK_URL provides support for writing/reading the TKE_URL opcode object to/from an HSF file. 

	An URL in a stream file is another form of user/application data.  Its intent is to provide informational
	links corresponding to data (as opposed to External_Reference which provides additional content).
*/
class BBINFILETK_API TK_URL : public BBaseOpcodeHandler {
    private:
        TK_URL(const TK_URL&);
        TK_URL& operator=(const TK_URL&);

	protected:
		int             m_length;       /*!< internal use; Lengths of text string */
		int             m_allocated;    /*!< internal use; Size of allocated string buffer */
		char *          m_string;       /*!< internal use; Text string */

	public:
		/*! constructor */
		TK_URL () : BBaseOpcodeHandler (TKE_URL),
									m_length (0), m_allocated (0), m_string (0) {}
		~TK_URL();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		void        Reset (void) alter;

		/*! Sets the reference string.  Allocates the buffer and copies the string */
		void            SetString (char const * string) alter;
		/*! Sets the reference string buffer.  Allocates a buffer large enough to hold a string of 'length' bytes */
		void            SetString (int length) alter;
		/*! Returns the reference string */
		char const *    GetString (void) const                  { return m_string;      }
		/*! Returns the reference string buffer, which may be modified directly */
		char alter *    GetString (void) alter                  { return m_string;      }
};


//! Handles the TKE_External_Reference opcodes.
/*! 
	TK_External_Reference provides support for writing/reading the TKE_External_Reference opcode object to/from an HSF file. 

	An External Reference represents another data stream which whould be further interpreted as a sub-part of the
	current data stream (similar to an "include" directive in a programming language).
*/
class BBINFILETK_API TK_External_Reference : public BBaseOpcodeHandler {
    private:
        TK_External_Reference(const TK_External_Reference&);
        TK_External_Reference& operator=(const TK_External_Reference&);

	protected:
		int             m_length;       /*!< internal use; Length of text string */
		int             m_allocated;    /*!< internal use; Size of allocated string buffer */
		char *          m_string;       /*!< internal use; Text string */

	public:
		TK_External_Reference () : BBaseOpcodeHandler (TKE_External_Reference),
									m_length (0), m_allocated (0), m_string (0) {}
		~TK_External_Reference();

		TK_Status   Read (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		TK_Status   Clone (BStreamFileToolkit & tk, BBaseOpcodeHandler **handler) const;

		TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;

		TK_Status   Execute (BStreamFileToolkit & tk) alter;
		void        Reset (void) alter;

		/*! Sets the reference string.  Allocates the buffer and copies the string */
		void            SetString (char const * string) alter;
		/*! Sets the reference string buffer.  Allocates a buffer large enough to hold a string of 'length' bytes */
		void            SetString (int length) alter;
		/*! Returns the reference string */
		char const *    GetString (void) const                  { return m_string;      }
		/*! Returns the reference string buffer, which may be modified directly */
		char alter *    GetString (void) alter                  { return m_string;      }
};


//
//
// W3D Extensions
//
//

///
///\interface   W3D_Image   dwf/w3dtk/BOpcodeHandler.h     "dwf/w3dtk/BOpcodeHandler.h"
///\brief       Handles externally referenced named images.
///\since       1.0.1000
///
///             This class has been added to handle the extended TKE_HW3D_Image op-code.
///             This op-code was added for use in the W3D stream to work with the DWF package format
///             preference of storing images (especially for textures) as section resources
///             rather than embedding them directly in the graphics stream.
///
class BBINFILETK_API W3D_Image : public BBaseOpcodeHandler
{
    private:
        W3D_Image(const W3D_Image&);
        W3D_Image& operator=(const W3D_Image&);

    protected:

        char *                  m_name;         /*!< internal use; name applied to image (if any) */
        int                     m_name_length;  /*!< internal use; Length of name */
        int                     m_size[2];
        char                    m_bpp;

        //! internal use
        void    set_name (char const * string) alter;
        //! internal use
        void    set_name (int length) alter;

    public:
        /*! constructor */
        W3D_Image () : BBaseOpcodeHandler ((unsigned char)TKE_HW3D_Image), m_name (0), m_name_length (0), m_bpp(0)
        { m_size[0] = m_size[1] = 0; }
        ~W3D_Image();

        TK_Status   ReadAscii (BStreamFileToolkit & tk) alter;
		TK_Status   Read (BStreamFileToolkit & tk) alter;		

        TK_Status   WriteAscii (BStreamFileToolkit & tk) alter;
		TK_Status   Write (BStreamFileToolkit & tk) alter;
		

        void        Reset (void) alter;

        /*! Sets the name of the image.  Allocates the buffer and copies the string */
        void            SetName (char const * string) alter                 { set_name (string);        }
        /*! Sets the name buffer.  Allocates a buffer large enough to hold a string of 'length' characters */
        void            SetName (int length) alter                          { set_name (length);        }
        /*! Returns the image name string */
        char const *    GetName (void) const                                { return m_name;            }
        /*! Returns the image name string buffer, which may be modified directly */
        char alter *    GetName (void) alter                                { return m_name;            }

        void            SetSize (int x, int y )                             { m_size[0] = x; m_size[1] = y ;}
        int const*      GetSize (void) const                                { return m_size;            }
        int alter*      GetSize (void) alter                                { return m_size;            }

        void            SetBitDepth( char bpp )                             { m_bpp = bpp;              }
        char const      GetBitDepth (void) const                            { return m_bpp;            }
        char alter      GetBitDepth (void) alter                            { return m_bpp;            }

};


#endif //BOPCODE_HANDLER

