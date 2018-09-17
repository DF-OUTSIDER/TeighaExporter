//
// Copyright (c) 2000 by Tech Soft 3D, LLC.
// The information contained herein is confidential and proprietary to
// Tech Soft 3D, LLC., and considered a trade secret as defined under
// civil and criminal statutes.  Tech Soft 3D shall pursue its civil
// and criminal remedies in the event of unauthorized use or misappropriation
// of its trade secrets.  Use of this information by anyone other than
// authorized employees of Tech Soft 3D, LLC. is granted only under a
// written non-disclosure agreement, expressly prescribing the scope and
// manner of such use.
//
// $Header: //DWF/Working_Area/Willie.Zhu/w3dtk/BByteStream.h#1 $
//
// This header file is a completely self-contained class that defines all of the
// member functions that it needs inside the declaration.  It is just a utility class
// to stuff unaligned data values into bytes.
//

#ifndef _B_BYTE_STREAM_H_
#define _B_BYTE_STREAM_H_

///
///\file        dwf/w3dtk/BByteStream.h
///


#ifndef STATUS_ERROR
#  define STATUS_ERROR 0
#endif
#ifndef STATUS_NORMAL
#  define STATUS_NORMAL 1
#endif

#include "OdPlatformSettings.h"
#ifdef ODA_BIGENDIAN
#define STREAM_BIGENDIAN
#else
#undef STREAM_BIGENDIAN
#endif

//! Handles transfer of certain unaligned bit lengths via unsigned char arrays
/*! 
    We determined that visual artifacts introduced in sending 8-bit
    normals were too noticeable, and that higher accuracy was required.
    At the same time, however, we wanted better compression than 16-bit.
    Thus, the BByteStream class was born.

    For efficiency, this class makes certain assumptions about the
    number of bits per pixel: 1) that samples are at least 1 byte 
    (see logic controlling byteplace incrementing); 2) that samples
    never span 3 bytes in the character array (ruling out lengths of
    11, 13, 14, 15, and 17+.  Additionally, the bit shifting operations
    here are needless overhead for any length divisible by 8, meaning
    that this class is not the best way to do it.  These
    assumptions and observations restrict us (at least for the moment)
    to 9, 10, and 12 as allowable sample lengths.

    This class is a state machine that is intended to match the semantics
    of a FILE* (with the obvious exception that it goes into a uchar*
    rather than to a real file.  With that in mind, it does not support
    random access through the data.  Only a linear walk from front to back
    is allowed.
*/
class BByteStream {
  private:
    int byteplace;      /*!< Current position from which to read/write */
    int bitplace;       /*!< Distance, in bits, from the rightmost bit of the second byte. range:0..7 */
    int bitsperval;     /*!< Number of bits per sample.  Must be 9, 10, or 12 */
    int allocated;      /*!< Size of data.  Used for bounds checking in debug mode. */
    unsigned char *data;    /*!< The raw data to read from or write to. */
    unsigned short mask;    /*!< For convenience, a mask to remove garbage above the most significant bit. */
  public:
    BByteStream(int size, unsigned char *cptr, int bpv) {
        if( bpv == 9 || bpv == 10 || bpv == 12 ) {
            allocated = size;
            data = cptr; 
            bitsperval = bpv;
            byteplace = 0;
            bitplace = 16 - bitsperval; 
            mask = (unsigned short)((1<<bitsperval)-1);
        }
        else {
            //do nothing.  we deserve the crash.
            data = 0;
        }
    };
    BByteStream() { data = NULL; };
    /*< write a sample */
    void put(unsigned short value) alter {
        data[byteplace] |= (value >> (8 - bitplace));
        data[byteplace+1] |= (value << bitplace);
        bitplace -= bitsperval - 8;
        if( bitplace < 0 ) {
            byteplace += 2; // consume two bytes
            bitplace += 8;
        }
        else
            byteplace++; // consume one byte
    };	    
    /*!< read a sample */
    void get(unsigned short alter &value) {
        value = (unsigned short)((data[byteplace] << 8) | (data[byteplace + 1]));
        value = (unsigned short)((value >> bitplace) & mask );
        bitplace -= bitsperval - 8;
        if( bitplace < 0 ) {
            byteplace += 2; // consume two bytes
            bitplace += 8;
        }
        else
            byteplace++; // consume one byte
    }; 

}; //end declaration of class BByteStream



/*!
    BVarStream encodes/decodes samples of variable length.  Given an integer sample 
    value and a cascading sequence of bit lengths to try, it will write escape
    sequences until it encounters a bit length long enough to encapsulate the value.

    It was ported to C because it was a step along the way in the implementation of class BPack.
    It was put into this file in case it ever became useful.  Buyer beware, however,
    since it has been neither used nor tested since the port to C++.
*/
class BVarStream {
  private:
    unsigned int *m_data;   /*!< payload */
    int m_allocated;        /*!< amount of data allocated */
    int m_used;             /*!< current int being written */
    int m_bit;              /*!< current bit getting written */
    int m_rused;            /*!< next int to be read */
    int m_rbit;             /*!< next bit to be read */
    int m_can_reallocate;   /*!< if we don't own the pointer, we can't reallocate it */
    int m_status;           /*!< a flag indicating an error condition */
    unsigned int m_mask[33];  /*!< precalculated values to efficiently mask reads */
    unsigned int m_range[33]; /*!< precalculated values maximum possible value given bit length  */


    /*! Set all of the member variables to known values */
    void Reset() {
        int i;
        m_data = 0;
        m_allocated = 0;
        m_used = 0;
        m_bit = 0;
        m_rused = 0;
        m_rbit = 0;
        m_can_reallocate = 0;
        m_status = STATUS_NORMAL;
        m_mask[0] = 0;
        m_range[0] = 0;
        for( i = 1 ; i <= 32 ; i++ ) {
            m_mask[i] = ((unsigned int)(-1)) >> (32-i);
            m_range[i] = (0x1 << (i-1)) - 1;
        }
    };

    /*! Account for big endian systems. */
    #ifdef STREAM_BIGENDIAN
        #ifndef SWAP32
            #define SWAP32(i) (\
                (((i) >> 24) & 0x000000ff) |\
                (((i) & 0x00ff0000) >> 8) |\
                (((i) & 0x0000ff00) << 8) |\
                ((i) << 24) \
                )
            #endif
    void SwapBytes( ) 
    {
        int i;
        for( i = 0 ; i < m_allocated ; i++ )
            m_data[i] = SWAP32( m_data[i] );
    };
#   else
    inline void SwapBytes() {};
#   endif

    /*! helper */
    void Reallocate( )
    {

        if( m_can_reallocate ) {
            unsigned int *temp;
            m_allocated *= 2;
            temp = new unsigned int[ m_allocated ];
            if( temp == NULL ) {
                /* Allocation failed.  Declare error, jettison all data to avoid a seg fault */
                m_status = STATUS_ERROR;
                m_used = 0; 
            }
            else {
                memcpy( temp, m_data, (m_used+1) * sizeof( unsigned int ) );
                delete [] m_data;
                m_data = temp;
            }
        }
        else {
            /* We don't own the poitner. Declare error, jettison all data to avoid a seg fault */
            m_status = STATUS_ERROR;
            m_used = 0;
        }
    };

    /*! helper */
    void Put2( int numbits, int val ) 
    {
        if( m_bit + numbits <= 32 ) {
            m_data[m_used] |= val << (32 - m_bit - numbits);
            m_bit += numbits;
        }
        else {
            int shift = (numbits + m_bit - 32);

            if( m_used + 1 >= m_allocated )
                Reallocate( );
            m_data[m_used++] |= val >> shift;
            m_data[m_used] = val << (32-shift);
            m_bit += numbits - 32;
        }
    };

    /*! helper */
    void Get2( int numbits, int *val ) 
    {
        if( m_rbit + numbits <= 32 ) {
            *val = (m_data[m_rused] >> (32 - m_rbit - numbits)) & m_mask[numbits];
            m_rbit += numbits;
        }
        else {
            int shift = (numbits + m_rbit - 32);
            *val = (m_data[m_rused++] << shift) & m_mask[numbits];
            *val |= m_data[m_rused] >> (32-shift);
            m_rbit += numbits - 32;
        }
    };


  public:

    BVarStream() {
        m_can_reallocate = 0;
        m_data = NULL;
    };

    ~BVarStream( )
    {
        if( m_data != NULL ) {
            if( m_can_reallocate )
                delete [] m_data;
            m_data = NULL;
        }
    };

    /*! We already have a buffer that we would like to use varstream to fill w/ data.
     * matching this call with a vsfree is unnecessary (but would do no harm).
     * pointer must be 4-byte aligned. 
     */
    void InitWrite( int size, void *pointer ) {
        Reset();
        m_allocated = size/4;
        m_data = (unsigned int *) pointer;
        m_data[0] = 0;
        m_can_reallocate = 1;
    };

    /*! We have a buffer that we would like to use varstream to read from.
     * matching this call with a vsfree is unnecessary (but would do no harm). 
     * pointer must be 4-byte aligned. 
     */
    void InitRead( int size, const void *pointer )
    {
        Reset();
        m_allocated = size/4;
        m_data = (unsigned int *) pointer;
        m_can_reallocate = 0;
    };

    /*! The numbits_array is an array of bit lengths which should be attempted, in order.
     *  It is assumed that at least one of the lengths is sufficient for the given val.
     *  For EdgeBreaker, we used {2,6,10,14,18,22,26,31}
     */
    int Put( int *numbits_array, int val )
    {
        int temprange = 0, i;

        i = 0;
        for(;;){
            temprange = m_range[ numbits_array[i] ];
            if( val < -temprange || val > temprange ) {
                /* Put an escape sequence as a signal to "fall back to the next level" */
                Put2( numbits_array[i], m_mask[ numbits_array[i] ] );
                i++;
            }
            else {
                /* Put the real data.  We've found a range that works, so we're done. */
                Put2( numbits_array[i], val+temprange );
                break;
            }
        }
        return m_status;
    };

    /*! Decode a sample based on a given sequence of bit lengths.  For each bit length,
     *  a value equal to the mask (all 1's) is interpreted as an escape sequence.  Every 
     *  time an escape sequence encountered, we jump to the next value in numbits_array
     *  to try to interpret the value. This sequence of bit lengths must match what was used
     *  on the write side.
     */
    int Get( int *numbits_array ) 
    {
        int i = 0;
        int val;

        for(;;) {
            Get2( numbits_array[i], &val );
            if( val == (int)m_mask[ numbits_array[i] ] )
                i++;
            else
                break;
        }
        val -= m_range[ numbits_array[i] ];
        return val;
    };

}; /* end definition of class BVarStream */




/*!
    The BPack class is very similar to BByteStream, but without the 
    restrictions on bits_per_sample.  BPack supports anything in the range
    of 2-31.  I haven't done the benchmarks to determine which is faster for 
    the values of bits_per_sample that both classes support (9,10,12).  I suspect
    that the answer to that question would be platform-dependent, but I have no
    hard data to back that up.

    This class is based on a subset of the functions in BVarStream.  Unlike BVarStream,
    however, this class is meant to encode samples of fixed length.
*/
class BPack {
  private:
    unsigned int *m_data; /*!< payload */
    int m_allocated;    /*!< amount of data allocated */
    int m_used;     /*!< current int being written */
    int m_bit;      /*!< current bit getting written */
    int m_rused;        /*!< next int to be read */
    int m_rbit;     /*!< next bit to be read */
    int m_can_reallocate; /*!< if we don't own the pointer, we can't reallocate it */
    int m_status;       /*!< a flag indicating an error condition */
    unsigned int m_mask[33]; /*!< precalculated values to efficiently mask reads */
    unsigned int m_range[33]; /*!< precalculated values maximum possible value given bit length  */


    /*! Set all of the member variables to known values */
    void Reset() {
        int i;
        m_data = 0;
        m_allocated = 0;
        m_used = 0;
        m_bit = 0;
        m_rused = 0;
        m_rbit = 0;
        m_can_reallocate = 0;
        m_status = STATUS_NORMAL;
        m_mask[0] = 0;
        m_range[0] = 0;
        for( i = 1 ; i <= 32 ; i++ ) {
            m_mask[i] = ((unsigned int)(-1)) >> (32-i);
            m_range[i] = (0x1 << (i-1)) - 1;
        }
    };

    /*! helper */
    void Reallocate( )
    {

        if( m_can_reallocate ) {
            unsigned int *temp;
            m_allocated *= 2;
            temp = new unsigned int[ m_allocated ];
            if( temp == NULL ) {
                /* Allocation failed.  Declare error, jettison all data to avoid a seg fault */
                m_status = STATUS_ERROR;
                m_used = 0; 
            }
            else {
                memcpy( temp, m_data, (m_used+1) * sizeof( unsigned int ) );
                delete [] m_data;
                m_data = temp;
            }
        }
        else {
            /* We don't own the poitner. Declare error, jettison all data to avoid a seg fault */
            m_status = STATUS_ERROR;
            m_used = 0;
        }
    };



  public:

    BPack() { 
        m_can_reallocate = 0;
        m_data = NULL;
    };

    ~BPack( ) {
        if( m_data != NULL ) {
            if( m_can_reallocate )
                delete [] m_data;
            m_data = NULL;
        }
    };

    /*! We already have a buffer that we would like to use varstream to fill w/ data.
     * matching this call with a vsfree is unnecessary (but would do no harm).
     * pointer must be 4-byte aligned. 
     */
    void InitWrite( int size, void *pointer ) {
        Reset();
        m_allocated = size/4;
        m_data = (unsigned int *) pointer;
        if (m_data) m_data[0] = 0;
        m_can_reallocate = 0;
    };

    /*! We have a buffer that we would like to use varstream to read from.
     * matching this call with a vsfree is unnecessary (but would do no harm). 
     * pointer must be 4-byte aligned. 
     */
    void InitRead( int size, const void *pointer )
    {
        Reset();
        m_allocated = size/4;
        m_data = (unsigned int *) pointer;
        m_can_reallocate = 0;
    };

    void Put( int numbits, int val ) 
    {
        if( m_bit + numbits <= 32 ) {
            m_data[m_used] |= val << (32 - m_bit - numbits);
            m_bit += numbits;
        }
        else {
            int shift = (numbits + m_bit - 32);

            if( m_used + 1 >= m_allocated )
                Reallocate( );
            m_data[m_used++] |= val >> shift;
            m_data[m_used] = val << (32-shift);
            m_bit += numbits - 32;
        }
    };

    int Get( int numbits ) 
    {
        int return_val;

        if( m_rbit + numbits <= 32 ) {
            return_val = (m_data[m_rused] >> (32 - m_rbit - numbits)) & m_mask[numbits];
            m_rbit += numbits;
        }
        else {
            int shift = (numbits + m_rbit - 32);
            return_val = (m_data[m_rused++] << shift) & m_mask[numbits];
            return_val |= m_data[m_rused] >> (32-shift);
            m_rbit += numbits - 32;
        }
        return return_val;
    };

    int NumBytes() const { return (m_used+(m_bit?1:0)) * 4; };
    void SetCanReallocate(int val) alter { m_can_reallocate = val; };
    int GetStatus() const { return m_status; };

    /*! Account for big endian systems. Call after the last Put() or before the first Get() */
    #ifdef STREAM_BIGENDIAN
	void SwapBytes( ) 
	{
	    int i;
	    for( i = 0 ; i < m_allocated ; i++ )
		m_data[i] = SWAP32( m_data[i] );
	};
    #else
	inline void SwapBytes() {};
    #endif

}; /* end definition of class BPack */


#endif /* _B_BYTE_STREAM_H_ */

