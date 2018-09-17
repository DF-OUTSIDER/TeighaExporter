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
#if !defined FIFO_HEADER
#define FIFO_HEADER

///
///\file        dwf/whiptk/fifo.h
///

#include "whiptk/whipcore.h"

#define WD_FIFO_SLACK_FRACTION 0.25

/// A utility class template describing a FIFO queue.
template<class _ItemType>
class WT_FIFO
{
private:

    int            m_size;         // The number of generic items being held in the FIFO.
    int            m_capacity;     // The number of items that can fit in the current FIFO.
    int            m_start;        // The position of the first item in the FIFO ring buffer.
    _ItemType *    m_buffer;       // The FIFO buffer itself.

    WT_FIFO (WT_FIFO const &)
      : m_size(0)
      , m_capacity(0)
      , m_start(0)
      , m_buffer(WD_Null)
    {
        WD_Complain ("cannot copy WT_FIFO");
    } // prohibited

    WT_FIFO & operator= (WT_FIFO const &)
    {
        WD_Complain ("cannot assign WT_FIFO");
        return *this;
    } // prohibited

public:

    /// Constructs a WT_FIFO object.
    WT_FIFO()
        : m_size(0)
        , m_capacity(0)
        , m_start(0)
        , m_buffer(WD_Null)
    { }

    /// Constructs a WT_FIFO object with given initial queue size.
    WT_FIFO(int initial_size)
        : m_size(0)
        , m_capacity(initial_size + 1)
        , m_start(0)
        , m_buffer(WD_Null)
    {
        m_buffer = new _ItemType[initial_size + 1];
        if (!m_buffer)
            throw WT_Result::Out_Of_Memory_Error;
    }

    /// Destroys a WT_FIFO object.
    virtual ~WT_FIFO()
    {
        delete []m_buffer;
    }
public:
    /// Returns the size of the queue.
    int size() const
    {
        return m_size;
    }

    /// Adds an array of items to the queue.
    WT_Result add(
        int add_size, /**< Number of items in the array. */
        _ItemType const * add_buffer /**< Pointer to the array of items. */
        );
    /// Retrieves an array of items from the queue
    /** \warning Client is responsible for ensuring that the queue has at least as many items as are requested. */
    void fetch(
        int fetch_size, /**< Number of items to retrieve. */
        int start_item, /**< Index in queue to start retrieval. */
        _ItemType * fetch_buffer /**< Destination buffer, receives requested items. */
        ) const;
    /// Removes an array of items from the queue (from the beginning of the queue) and copies them to the array.
    /** \warning Client is responsible for ensuring that the queue has at least as many items as are indicated. */
    void remove(
        int remove_size, /**< Number of items to remove. */
        _ItemType * remove_buffer /**< Destination buffer, receives removed items. */
        );
    /// Removes an array of items from the queue (from the beginning of the queue.)
    /** \warning Client is responsible for ensuring that the queue has at least as many items as are indicated. */
    void remove(
        int remove_size
        );
    /// Clears the queue.
    void clear_all();
    /// Returns the pseudo-index of the item in the queue.
    /** The architecture of the queue is a wraparound buffer.  This hides the architecture and
     *  returns the index relative to the start position as if the start were always at zero.
     *  \warning Client is responsible for ensuring the item actually exists in the queue.
     */
    int pointer_to_index(
        _ItemType const * ptr /**< Item to look for. */
        ) const;
    /// Retrieves the item at the given pseudo-index.
    /** The architecture of the queue is a wraparound buffer.  This hides the architecture and
     *  uses the given index as if the start were always at zero. */
    _ItemType &  item(int index) const;
    /// Removes the LAST item that was placed in the queue.
    void       pop();

};

template<class _ItemType>
WT_Result WT_FIFO<_ItemType>::add(int add_size, _ItemType const * add_buffer)
{
    WD_Assert(add_size > 0);
    WD_Assert(add_buffer);

    // Will we overflow the current buffer?
    if ( (m_size + add_size) > m_capacity)
    {
        // We're about to overflow the FIFO buffer, so let's allocate
        // a bigger FIFO buffer and copy the old one into it.
        _ItemType *    new_buffer;
        int            new_capacity;

        new_capacity  = m_size + add_size + 1;
        new_capacity += (int)(new_capacity * WD_FIFO_SLACK_FRACTION);
        new_buffer = new _ItemType[new_capacity];
        if (!new_buffer)
            return WT_Result::Out_Of_Memory_Error;

        if ( (m_start + m_size) <= m_capacity)
        {
            // The data doesn't wrap around the ring buffer in the orig. buffer,
            // so it is an easy copy to the new buffer.
            //WD_COPY_MEMORY(m_buffer + m_start, m_size, new_buffer);
            _ItemType *    source = m_buffer + m_start;
            _ItemType *    dest   = new_buffer;
            for (int loop = 0; loop < m_size; loop++)
                *dest++ = *source++;
        }
        else
        {
            int    split_size = m_capacity - m_start;
            // The data is wrapped around in the source ring buffer.  Need to do
            // more work to copy it to the new buffer.
            // WD_COPY_MEMORY(m_buffer + m_start, split_size         , new_buffer);
            // WD_COPY_MEMORY(m_buffer          , m_size - split_size, new_buffer + split_size);
            _ItemType *     source = m_buffer + m_start;
            _ItemType *     dest   = new_buffer;
            int             loop;

            for (loop = 0; loop < split_size; loop++)
                *dest++ = *source++;

            source = m_buffer;
            for (loop = 0; loop < (m_size - split_size); loop++)
                *dest++ = *source++;
        }

        m_start = 0;
        m_capacity = new_capacity;
        delete []m_buffer;
        m_buffer = new_buffer;
    } // If orig buffer would have overflowed

    // Now add the new data to the buffer

    // Find the tail end where we start adding to the FIFO.  Note that the "tail" might
    // be "behind" the start if we've wrapped around the ring buffer.
    int tail = m_start + m_size;
    if (tail >= m_capacity)
        tail -= m_capacity;

    if ( (tail + add_size) <= m_capacity)
    {
        // The new data wont wrap around the ring buffer,
        // so it is an easy copy into the buffer.
        // WD_COPY_MEMORY(add_buffer, add_size, m_buffer + tail);
        _ItemType const *    source = add_buffer;
        _ItemType *        dest   = m_buffer + tail;
        for (int loop = 0; loop < add_size; loop++)
            *dest++ = *source++;
    }
    else
    {
        int    split_size = m_capacity - tail;
        // The new data will wrap around the ring buffer.  Need to do
        // more work to copy it in two chunks.
        // WD_COPY_MEMORY(add_buffer             , split_size           , m_buffer + tail);
        // WD_COPY_MEMORY(add_buffer + split_size, add_size - split_size, m_buffer       );
        _ItemType const *   source = add_buffer;
        _ItemType *         dest   = m_buffer + tail;
        int                 loop;

        for (loop = 0; loop < split_size; loop++)
            *dest++ = *source++;

        dest = m_buffer;
        for (loop = 0; loop < (add_size - split_size); loop++)
            *dest++ = *source++;
    }

    m_size += add_size;

    return WT_Result::Success;
}

template<class _ItemType>
void WT_FIFO<_ItemType>::fetch(int fetch_size, int start_item, _ItemType * users_buffer) const
{
    // Make sure that this FIFO has at least the amount of data that was asked for.
    WD_Assert((fetch_size + start_item) <= m_size);
    WD_Assert(users_buffer);

    int    pseudo_start = m_start + start_item;
    if (pseudo_start >= m_capacity)
        pseudo_start -= m_capacity;

    // Copy the data out of the FIFO and into the user's buffer.
    if ( (pseudo_start + fetch_size) <= m_capacity)
    {
        // The *requested* data doesn't wrap around the ring buffer,
        // so it is an easy copy to the user's buffer.
        _ItemType *    source = m_buffer + pseudo_start;
        _ItemType *    dest   = users_buffer;
        for (int loop = 0; loop < fetch_size; loop++)
            *dest++ = *source++;
    }
    else
    {
        int    split_size = m_capacity - pseudo_start;
        // The data is wrapped around in the FIFO ring buffer.  Need to do
        // more work to copy it to the user's buffer.
        _ItemType *     source = m_buffer + pseudo_start;
        _ItemType *     dest   = users_buffer;
        int             loop;

        for (loop = 0; loop < split_size; loop++)
            *dest++ = *source++;

        source = m_buffer;
        for (loop = 0; loop < (fetch_size - split_size); loop++)
            *dest++ = *source++;
    }
}

template<class _ItemType>
void WT_FIFO<_ItemType>::remove(int remove_size, _ItemType * remove_buffer)
{
    fetch(remove_size, 0, remove_buffer);
    // Now adjust for the fact that data was removed from the ring buffer.
    remove(remove_size);
}


template<class _ItemType>
void WT_FIFO<_ItemType>::remove(int remove_size)
{
    // Make sure that this FIFO has at least the amount of data that was asked for.
    WD_Assert(remove_size <= m_size);
    WD_Assert(remove_size >= 0);


    // Adjust for the fact that data was removed from the ring buffer.
    m_start += remove_size;
    if (m_start >= m_capacity)
        m_start -= m_capacity;

    m_size -= remove_size;

    if (!m_size)
        m_start = 0;

    WD_Assert (m_size >= 0);
    WD_Assert (m_start >= 0);
    WD_Assert (m_start < m_capacity);
}

template<class _ItemType>
void WT_FIFO<_ItemType>::pop()
{
    // pop the last added item off the fifo queue
    // (a little lifo routine)
    WD_Assert(1 <= m_size);

    m_size--;
    if (!m_size)
        m_start = 0;

    WD_Assert (m_size >= 0);
    WD_Assert (m_start >= 0);
    WD_Assert (m_start < m_capacity);
}

template<class _ItemType>
_ItemType & WT_FIFO<_ItemType>::item(int index) const
{
    // Make sure that this FIFO has at least the amount of data that was asked for.
    WD_Assert(index < m_size);

    // Copy the data out of the FIFO and into the user's buffer.
    int    position = m_start + index;
    if (position >= m_capacity)
    {
        // The *requested* data wrapped around the ring buffer.
        position -= m_capacity;
    }

    return m_buffer[position];
}

template<class _ItemType>
void    WT_FIFO<_ItemType>::clear_all()
{
    m_start = 0;
    m_size = 0;
}

template<class _ItemType>
int WT_FIFO<_ItemType>::pointer_to_index(_ItemType const * ptr) const
{

    WD_Assert(ptr >= m_buffer);
    WD_Assert(ptr < (m_buffer + m_capacity));

    int    pseudo_index = static_cast<int>(ptr - m_buffer);
    WD_Assert(pseudo_index == (ptr - m_buffer));

    // See if we wrap around or not
    if (pseudo_index < m_start)
        pseudo_index += m_capacity;

    WD_Assert(pseudo_index < (m_start + m_size));
    return pseudo_index - m_start;
}

#endif // FIFO_HEADER
