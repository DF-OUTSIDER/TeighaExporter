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

#if !defined LIST_HEADER
#define LIST_HEADER

///
///\file        dwf/whiptk/list.h
///

/// A base class representing an item in a linked list.
class WHIPTK_API WT_Item
{
    friend class WT_Item_List;

private:
    WT_Item* m_next;
    WT_Item* m_prev;

    virtual void _deleteObject (void *) {
        WD_Complain ("object not deleted");
    } // prohibited, must be implemented in subclass

    WT_Item (WT_Item const &)
        : m_next (WD_Null)
        , m_prev (WD_Null)
    {
        WD_Complain ("cannot copy WT_Item");
    } // prohibited

    WT_Item & operator= (WT_Item const &)
    {
        WD_Complain ("cannot assign WT_Item");
        return *this;
    } // prohibited

protected:
    /// Constructs a WT_Item object.
    WT_Item()
        : m_next(WD_Null)
        , m_prev(WD_Null)
    { };

    /// Destroys a WT_Item object.
    virtual ~WT_Item() { m_next = m_prev = WD_Null; };

public:
    /// Accesses the next item relative to this one.
    WT_Item* next() { return m_next; };

    /// Accesses the previous item relative to this one.
    WT_Item* prev() { return m_prev; };

    /// Assigns the next item relative to this one.
    void set_next(
        WT_Item *item /**< The new next item. */
        )
    { m_next = item; };

    /// Assigns the previous item relative to this one.
    void set_prev(
        WT_Item *item /**< The new previous item. */
        )
    { m_prev = item; };

};

/// Abstract base class representing a linked list.
class WHIPTK_API WT_Item_List
{

private:
    WT_Item *m_head;
    WT_Item *m_tail;

    WT_Item_List (WT_Item_List const &)
        : m_head (WD_Null)
        , m_tail (WD_Null)
    {
        WD_Complain ("cannot copy WT_Item_List");
    } // prohibited

    WT_Item_List operator= (WT_Item_List const &)
    {
        WD_Complain ("cannot assign WT_Item_List");
        return *this;
    } // prohibited

protected:
    /// Constructs a WT_Item_List object.
    WT_Item_List()
    : m_head(WD_Null), m_tail(WD_Null) {}

    /// Constructs a WT_Item_List object with the given initial item.
    WT_Item_List(
        WT_Item* first /**< The initial list item. */
        )
    : m_head(first), m_tail(first) {}

    /// Destroys a WT_Item_List object.
    virtual ~WT_Item_List() {}
public:
    /// Inserts an item at the beginning of the list.
    void add_front( WT_Item* item )
    {
        if (item)
        {
            item->set_prev(WD_Null);
            item->set_next(m_head);
            set_head(item);
        }
    }

    /// Appends an item to the end of the list.
    void add_tail(WT_Item *item)
    {
        if (item)
        {
            if (!m_head)
            {
                set_head(item);
            }
            else
            {
                item->set_prev(m_tail);
                item->set_next(WD_Null);
                m_tail->set_next(item);
                m_tail = item;
            }
        }
    }

    /// Returns the number of items in the list.
    WT_Integer32 count() const
    {
        WT_Item *current = m_head;
        WT_Integer32 count = 0;
        while (current)
        {   count++;
            current = current->next();
        }
        return count;
    }

    /// Returns the first item in the list.
    WT_Item*  get_head() const
    {
        return m_head;
    }

    /// Returns the last item in the list.
    WT_Item*  get_tail() const
    {
        return m_tail;
    }

    /// Returns WD_True if the list is empty.
    WT_Boolean is_empty() const
    {
        return ((m_head == WD_Null) && (m_head == m_tail)) ? WD_True : WD_False;
    }

    /// Removes all of the items in the list.
    void remove_all()
    {
        WT_Item *head,*tail;
        while( !is_empty() )
        {
            head = get_head();
            tail = head->next();
            head->_deleteObject(head);
            set_head(tail);
            if (!tail)
                set_tail(WD_Null);
        }
    }

protected:
    /// Replaces the first element.  If the only element in the list, also sets it as the last element.
    void  set_head(WT_Item* item)
    {
        m_head = item;
        if (item && !m_tail) set_tail(item);
    }

    /// Replaces the last element.  If the only element in the list, also sets it as the first element.
    void  set_tail(WT_Item* item)
    {
        m_tail = item;
        if (item && !m_head) set_head(item);
    }
};

#endif // LIST_HEADER
