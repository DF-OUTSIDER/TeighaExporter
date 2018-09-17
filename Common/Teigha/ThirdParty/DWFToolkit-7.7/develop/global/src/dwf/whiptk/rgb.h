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

#if !defined RGB_HEADER
#define RGB_HEADER

///
///\file        dwf/whiptk/rgb.h
///

/// A simple 24-bit RGB color.
class WHIPTK_API WT_RGB {
    public:
        unsigned char   m_red; /**< Red component of the color. */
        unsigned char   m_green; /**< Green component of the color. */
        unsigned char   m_blue; /**< Blue component of the color. */

        /// Constructs a WT_RGB object (defaults to black.)
        WT_RGB () : m_red(0), m_green(0), m_blue(0) {}

        /// Constructs a WT_RGB object.  Copy constructor.
        WT_RGB (WT_RGB const & in)
          : m_red   (in.m_red)
          , m_green (in.m_green)
          , m_blue  (in.m_blue)
        { }

        /// Constructs a WT_RGB object from the given color values.
        /** \note The values are converted to a byte value by casting them each to an unsigned char.
         */
        WT_RGB (int red, int green, int blue)
          : m_red   (static_cast<unsigned char>(red))
          , m_green (static_cast<unsigned char>(green))
          , m_blue  (static_cast<unsigned char>(blue))
        { }

        /// Constructs a WT_RGB object from the given color scalars (each value should be 0 <= x <= 1.0)
        /** \note The values are converted to a byte value by multiplying with 255.99 and then casting
         *  them each to an unsigned char.
         */
        WT_RGB (float const & red, float const & green, float const & blue)
          : m_red   ((unsigned char)(red   * 255.99))
          , m_green ((unsigned char)(green * 255.99))
          , m_blue  ((unsigned char)(blue  * 255.99))
        { }

        /// Returns WD_True if the colors are equal.
        WT_Boolean operator== (WT_RGB const & test) const
        {
            return (m_red   == test.m_red   &&
                    m_green == test.m_green &&
                    m_blue  == test.m_blue     );
        }

        /// Returns WD_True if the colors are NOT equal.
        WT_Boolean operator!= (WT_RGB const & test) const
        {
            return (m_red   != test.m_red   ||
                    m_green != test.m_green ||
                    m_blue  != test.m_blue     );
        }

        /// Assignment operator.  Assigns the given color to this one.
        WT_RGB const & operator= (WT_RGB const & in)
        {
            m_red   = in.m_red;
            m_green = in.m_green;
            m_blue  = in.m_blue;
            return *this;
        }
};

/// A 32-bit RGBA color.
class WHIPTK_API WT_RGBA32 {
    public:
        /// Stores the color values red, green, blue, and alpha.
        union
        {
            WT_Integer32        m_whole;
            /// Contains the preferred storage ordering of color / alpha components.
            struct
            {
                unsigned char   WD_PREFERRED_RGB32;
            } m_rgb;
        };

        /// Constructs a WT_RGB object (defaults to transparent black.)
        WT_RGBA32 ()
          : m_whole(0)
        { }

        /// Constructs a WT_RGB object.  Copy constructor.
        WT_RGBA32 (WT_RGBA32 const & in)
          : m_whole (in.m_whole)
        { }

        /// Constructs a WT_RGB object from the given color values.
        /** \note The values are converted to a byte value by casting them each to an unsigned char.
         */
        WT_RGBA32 (int red, int green, int blue, int alpha = 0xFF) {
            m_rgb.r = static_cast<unsigned char>(red);
            m_rgb.g = static_cast<unsigned char>(green);
            m_rgb.b = static_cast<unsigned char>(blue);
            m_rgb.a = static_cast<unsigned char>(alpha);
        }
        /// Constructs a WT_RGB object from the given color scalars (each value should be 0 <= x <= 1.0)
        /** \note The values are converted to a byte value by multiplying with 255.99 and then casting
         *  them each to an unsigned char.
         */
        WT_RGBA32 (float const & red, float const & green, float const & blue, float alpha = 1.0f) {
            m_rgb.r = (unsigned char) (red   * 255.99);
            m_rgb.g = (unsigned char) (green * 255.99);
            m_rgb.b = (unsigned char) (blue  * 255.99);
            m_rgb.a = (unsigned char) (alpha * 255.99);
        }


        /// Returns WD_True if the colors are equal.
        WT_Boolean operator== (WT_RGBA32 const & test) const
                        {return m_whole == test.m_whole;}

        /// Returns WD_True if the colors are NOT equal.
        WT_Boolean operator!= (WT_RGBA32 const & test) const
                        {return m_whole != test.m_whole;}

        /// Assignment operator.  Assigns the given color to this one.
        WT_RGBA32 const & operator= (WT_RGBA32 const & in)
                        {m_whole = in.m_whole;  return *this;}

        /// Assignment operator.  Assigns the given color to this one (alpha set to full opacity.)
        WT_RGBA32 const & operator= (WT_RGB const & in)
        {
            m_rgb.r = in.m_red;
            m_rgb.g = in.m_green;
            m_rgb.b = in.m_blue;
            m_rgb.a = 0xFF;
            return *this;
        }
};

#endif // RGB_HEADER
