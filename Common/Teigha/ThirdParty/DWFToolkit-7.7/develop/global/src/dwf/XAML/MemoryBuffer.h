//
//  Copyright (c) 1996-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE CORRECTNESS
//  OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE IT. AUTODESK
//  PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY DISCLAIMS ANY
//  LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//


#if !defined MEMORY_BUFFER_HEADER
#define MEMORY_BUFFER_HEADER

class tMemoryBuffer
{
public:
    tMemoryBuffer( size_t nAllocSize ) throw (DWFException)
        : _size(nAllocSize)
        , _buffer(NULL)
        , _currentStrlen(0)
    {
        _buffer = DWFCORE_ALLOC_MEMORY(WT_Byte, _size);
        if (_buffer == NULL)
        {
            _DWFCORE_THROW( DWFMemoryException, L"Cannot allocate buffer" );
        }
    }

    ~tMemoryBuffer()
    {
        if (_buffer)
        {
            DWFCORE_FREE_MEMORY( _buffer );
        }
    }

    const char * buffer() const
    {
        return (char *) _buffer;
    }

    size_t size() const
    {
        return _size;
    }

    void init()
    {
        if(buffer() != NULL)
        {
            DWFCORE_ZERO_MEMORY(buffer(), 4); //zero the first 4 bytes, because we're good like that
        }
        _currentStrlen = 0;
    }

    size_t strlen(bool bForceComputation=false)
    {
        if( (_currentStrlen == 0) || bForceComputation)
        {
            _currentStrlen = DWFCORE_ASCII_STRING_LENGTH((char *)_buffer);
        }
        return _currentStrlen;
    }

    void set(const char *pStr)
    {
        size_t inStrLen = DWFCORE_ASCII_STRING_LENGTH(pStr);
        DWFCORE_ASCII_STRING_COPY( ((char *)_buffer), pStr );
        _currentStrlen = inStrLen;
    }

    void concatenate(const char *pStr)
    {
        size_t inStrLen = DWFCORE_ASCII_STRING_LENGTH(pStr);
		DWFCORE_ASCII_STRING_CONCATENATE( ((char *)_buffer)+_currentStrlen, pStr );
        _currentStrlen += inStrLen;
    }

    void copy(tMemoryBuffer *pOtherBuffer)
    {
        size_t otherLen = pOtherBuffer->strlen();
        DWFCORE_COPY_MEMORY( buffer(), pOtherBuffer->buffer(), pOtherBuffer->size() ); // would like to just copy strlen, but might not be safe
        _currentStrlen = otherLen;
    }

private:
    //
    // The number of bytes allocated for this buffer
    //
    size_t _size;

    //
    // The bytes themselves
    //
    WT_Byte* _buffer;

    //
    // The string length of the bytes stored in the buffer. Note that
    // this is only valid if the set/concatenate/copy methods are used.
    // If someone grabs the buffer pointer and plays with it directly,
    // all bets are off.
    //
    size_t _currentStrlen;

};

typedef multimap< size_t, tMemoryBuffer* > tMemoryBufferMap;

#endif
