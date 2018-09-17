/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#ifndef _SVG_WRAPPERS_H_INCLUDED_
#define _SVG_WRAPPERS_H_INCLUDED_

#if _MSC_VER > 1
#pragma once 
#endif

#include "OdDToStr.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_SVG_EXPORT {

/** \details
  Definitions for XML export
*/
namespace xml
{
  OdStreamBuf& operator << ( OdStreamBuf& stream, const OdString& s )
  {
    stream.putBytes( (const char*)s, s.getLengthA() );
    return stream;
  }

  OdStreamBuf& operator << ( OdStreamBuf& stream, char c )
  {
    stream.putByte( c );
    return stream;
  }

  struct Attribute
  {
    OdString _name;
    OdString _value;
    Attribute( const OdString& name = OD_T(""), const OdString& value = OD_T("") ) 
      : _name( name ), _value( value ) {}
    static int _doublePrecision;
    static OdString formatDouble( double d )
    {
      return odDToStr( d, 'g', _doublePrecision );
    }
  };

  OdStreamBuf& operator << ( OdStreamBuf& stream, const Attribute& a )
  {
    stream << a._name << '=' << '\"' << a._value << '\"' << ' ';
    return stream;
  }

  struct Node
  {
    OdString _name;
    OdString _contents;
    typedef std::vector<Attribute> AttributeMap;
    AttributeMap _attributes;
    std::vector<Node*> _children;
    Node( const OdChar* name ) : _name( name ) {}
    virtual ~Node()
    {
      for ( unsigned int i = 0; i < _children.size(); i++ ) delete _children[i];
    }
    Node* addChild( Node* child )
    {
      if ( child ) _children.push_back( child );
      return child;
    }
    Node* addChild( const OdChar* name )
    {
      if ( name ) return addChild( new Node( name ) );
      else return 0;
    }
    AttributeMap::iterator addAttribute( const Attribute& a )
    {
      _attributes.push_back( a );
      return _attributes.end() - 1;
    }
    AttributeMap::iterator addAttribute( const OdString& name, const OdString& value = OD_T("") )
    {
      return addAttribute( Attribute( name, value ) );
    }
    AttributeMap::iterator addAttribute( const OdString& name, double value )
    {
      Attribute a( name );
      a._value = a.formatDouble( value );
      return addAttribute( a );
    }
    Attribute* getAttribute( const OdString& name )
    {
      AttributeMap::iterator p = _attributes.begin();
      for (; p != _attributes.end(); ++p ) if ( p->_name == name ) break;
      if ( p == _attributes.end() ) p = addAttribute( Attribute( name ) );
      return &(*p);
    }
    void removeAttribute( const OdString& name )
    {
      AttributeMap::iterator p = _attributes.begin();
      for (; p != _attributes.end(); ++p ) if ( p->_name == name ) break;
      if ( p == _attributes.end() ) return;
      _attributes.erase( p );
    }
  };
  
  OdStreamBuf& operator << ( OdStreamBuf& stream, const Node& node )
  {
    // open tag
    stream << '<' << node._name << ' ';
    // write attributes
    Node::AttributeMap::const_iterator i = node._attributes.begin(); 
    for ( ; i != node._attributes.end(); ++i ) stream << *i;
    // if no contents - close
    if ( node._contents.isEmpty() && node._children.empty() )
      stream << '/' << '>' << '\r' << '\n';
    else
    {
      stream << '>';
      // contents are not separated by \r\n because of xml:space="preserve"
      if ( !node._contents.isEmpty() )
        stream << node._contents;
      else
        stream << '\r' << '\n';
      for ( unsigned int i = 0; i < node._children.size(); i++ )
        stream << *node._children[i];
      stream << '<' << '/' << node._name << '>' << '\r' << '\n';
    }
    return stream;
  };
}

/** \details
  Definitions for SVG export
*/
namespace svg
{
  inline OdString formatColor( ODCOLORREF color )
  {
    OdString s; s.format( OD_T("rgb(%d,%d,%d)"), 
      ODGETRED( color ), ODGETGREEN( color ), ODGETBLUE( color ) );
    return s;
  }
  inline OdString formatColor( const OdCmEntityColor& color )
  {
    OdString s; 
    s.format( L"rgb(%d,%d,%d)", color.red(), color.green(), color.blue());
    return s;
  }
  
  struct Stroke : xml::Attribute
  {
    Stroke() : xml::Attribute( OD_T("stroke"), OD_T("none") ) {};
    Stroke( const OdString& s) : xml::Attribute( OD_T("stroke"), s ) {};
    Stroke( ODCOLORREF color ) : xml::Attribute( OD_T("stroke"), formatColor( color ) ) {};
  };
 
  struct Fill : xml::Attribute
  {
    Fill( ODCOLORREF color ) : xml::Attribute( OD_T("fill"), formatColor( color ) ) {}
    Fill() : xml::Attribute( OD_T("fill"), OD_T("none") ) {}
  };

  struct Points : xml::Attribute
  {
    static OdString formatPoint(double x, double y)
    {
      OdString s = formatDouble( x );
      s += ',';
      s += formatDouble( y );
      s += ' ';
      return s;
    }

    // Solaris complains if these are combined into a template function.
    static OdString formatPoint( const OdGePoint2d& p )
    {
      return formatPoint(p.x, p.y);
    }
    
    static OdString formatPoint( const OdGePoint3d& p )
    {
      return formatPoint(p.x, p.y);
    }

    Points( OdUInt32 nPts, const OdGePoint3d* points ) : xml::Attribute( OD_T("points") )
    {
      for ( OdUInt32 i = 0; i < nPts; i++ )
        _value += formatPoint( points[i] );
      // if single point - append it twice
      if ( nPts == 1 ) 
        _value += formatPoint( points[0] ); 
    }
    Points( OdUInt32 nPts, const OdGePoint2d* points ) : xml::Attribute( OD_T("points") )
    {
      for ( OdUInt32 i = 0; i < nPts; i++ )
        _value += formatPoint( points[i] );
      // if single point - append it twice
      if ( nPts == 1 ) 
        _value += formatPoint( points[0] ); 
    }
  };
  OdString formatMatrix(const OdGeMatrix2d& m)
  {
    OdString s(L"matrix(");
    s += xml::Attribute::formatDouble(m.entry[0][0]);
    s += L' ';
    s += xml::Attribute::formatDouble(m.entry[1][0]);
    s += L' ';
    s += xml::Attribute::formatDouble(m.entry[0][1]);
    s += L' ';
    s += xml::Attribute::formatDouble(m.entry[1][1]);
    s += L' ';
    s += xml::Attribute::formatDouble(m.entry[0][2]);
    s += L' ';
    s += xml::Attribute::formatDouble(m.entry[1][2]);
    s += L')';
    return s;

  }

  struct DashArray : xml::Attribute
  {
    DashArray( OdArray<double> dDashArray ) : xml::Attribute( OD_T("stroke-dasharray") )
    {
      for( OdUInt32 i = 0; i < dDashArray.size(); i++ )
      {
        OdString strDash;
        strDash.format(OD_T("%f"), dDashArray[i] );
        strDash.replace(L',', L'.');
        _value += strDash + ","; 
      }

      if( _value.getLength() > 0 )
      {
        _value = _value.left( _value.getLength() - 1);
      }
    }
  };

  struct Path : xml::Node
  {
    Path( const OdString& name = OD_T("path") ) : xml::Node( name ) {}
    template <class P> void addLine( int nCount, const P* points )
    {
      OdString loop( OD_T("M") );
      for ( int i = 0; i < nCount; i++ )
        loop += Points::formatPoint( points[i] );
      getAttribute( OD_T("d") )->_value += loop;
    }
    template <class P> void addLoop( int nCount, const P* points )
    {
      addLine( nCount, points );
      getAttribute( OD_T("d") )->_value += OD_T("z");
    }
  };
  
  struct Circle : xml::Node
  {
    Circle( const OdGePoint3d& center, double radius ) : xml::Node( OD_T("circle") )
    {
      addAttribute( OD_T("cx"), center.x );
      addAttribute( OD_T("cy"), center.y );
      addAttribute( OD_T("r"), radius );
    }
  };

  struct Ellipse : xml::Node
  {
    Ellipse( const OdGePoint3d& center, double rx, double ry, double angle = 0.0 ) 
      : xml::Node( OD_T("ellipse") )
    {
      addAttribute( OD_T("rx"), rx );
      addAttribute( OD_T("ry"), ry );
      if ( angle != 0.0 )
      {
        OdString s = OD_T("translate(");
        s += xml::Attribute::formatDouble( center.x );
        s += ' ';
        s += xml::Attribute::formatDouble( center.y );
        s += OD_T(") rotate(");
        s += xml::Attribute::formatDouble( angle );
        s += ')';
        addAttribute( OD_T("transform"), s );
      }
      else
      {
        addAttribute( OD_T("cx"), center.x );
        addAttribute( OD_T("cy"), center.y );
      }
    }
  };
 
  struct Image : xml::Node
  {
    Image( const OdString& path) : xml::Node( OD_T("image") )
    {
      addAttribute( OD_T("width"), OD_T("1") );
      addAttribute( OD_T("height"), OD_T("1") );
      addAttribute( OD_T("xlink:href"), path );
      addAttribute( OD_T("preserveAspectRatio"), OD_T("none") );
    }
  };
  
  struct RootNode : xml::Node
  {
    RootNode( const OdGsDCRect& rect, ODCOLORREF bkColor ) : xml::Node( OD_T("svg") )
    {
      OdString s; s.format( OD_T("%d %d %d %d"), 
        odmin(rect.m_min.x, rect.m_max.x), odmin(rect.m_min.y, rect.m_max.y),
        Od_abs(rect.m_max.x - rect.m_min.x), Od_abs(rect.m_max.y - rect.m_min.y) );
      addAttribute( OD_T("viewBox"), s );
      addAttribute( OD_T("version"),          OD_T("1.1") );
      addAttribute( OD_T("xmlns"),            OD_T("http://www.w3.org/2000/svg") );
      addAttribute( OD_T("xmlns:xlink"),      OD_T("http://www.w3.org/1999/xlink") );
      addAttribute( OD_T("stroke-linecap"),   OD_T("round") );
      addAttribute( OD_T("stroke-linejoin"),  OD_T("round") );
      addAttribute( OD_T("fill-rule"),        OD_T("evenodd") );
      addAttribute( OD_T("xml:space"),        OD_T("preserve") );
      if ( bkColor != ODRGB( 0xff, 0xff, 0xff ) )
      {
        xml::Node* bk = addChild( OD_T("rect") );
        bk->addAttribute( OD_T("width"), OD_T("100%") );
        bk->addAttribute( OD_T("height"), OD_T("100%") );
        bk->addAttribute( Fill( bkColor ) );
      }
    }
  };
}
}
#endif
