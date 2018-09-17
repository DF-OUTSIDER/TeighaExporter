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

#include "OdCharMapper.h"

/** \details
  <group OdExport_Classes> 
*/
namespace TD_SVG_EXPORT { 

/** \details
  Definitions for SVG export 
*/
namespace svg
{
  // Scans for special symbols with side-effects (tolerance divider).
  //
  inline bool isUnicodeConvertible( const OdString& text )
  {
    return text.find(OD_T("%%v")) == -1 && text.find(OD_T("%%V")) == -1;
  }
  
  // Set style attribute to <text> or <tspan> node
  //
  void addStyle( xml::Node* node, const OdGiTextStyle* pTextStyle, bool bigFont )
  {
    OdString s; 

    bool bRscFont = false;

    if ( bigFont && !pTextStyle->bigFontFileName().isEmpty() )
      s.format( OD_T("font-family: %ls"), pTextStyle->bigFontFileName().c_str() );
    else
    {
      s.format( OD_T("font-family: %ls"), pTextStyle->ttfdescriptor().fileName().c_str() );

      if (pTextStyle->getFont() && (pTextStyle->getFont()->getFlags() & OdFont::kFontRsc))
      {
        bRscFont = true;
      }
    }
    // adobe viewer does not allow font-family to have a . inside

    if( !bRscFont )
    {
      if ( s.right(4) == OD_T(".shx") ) 
        s = s.left( s.getLength() - 4 );
    }
    else 
    {
      if( s.right(4) != OD_T(".shx") ) 
      {
        s += OD_T(".shx");
      }
    }

    node->addAttribute( OD_T("style"), s );
  }
 
  void addTextDecorations(xml::Node* ts, bool bOverline, bool bUnderline)
  {
    if (bOverline && bUnderline)
      ts->_attributes.push_back(xml::Attribute(L"text-decoration", L"overline,underline"));
    else if (bOverline)
      ts->_attributes.push_back(xml::Attribute(L"text-decoration", L"overline"));
    else if (bUnderline)
      ts->_attributes.push_back(xml::Attribute(L"text-decoration", L"underline"));
  }
  // Add <tspan> element to <text> node
  //
  void addTspan( xml::Node* text, const OdString& s, const OdGiTextStyle* pTextStyle, bool bigFont, bool bOverline, bool bUnderline )
  {
    if ( s.isEmpty() ) return;
    xml::Node* ts = text->addChild( new xml::Node( OD_T("tspan") ) );
    ts->_contents = s;
    addTextDecorations(ts, bOverline, bUnderline);
    addStyle( ts, pTextStyle, bigFont );
  }
  
  // Set text content, converting special sequences
  // if font is switched - tspans are added
  //
  void setContents( xml::Node* text, OdBaseTextIteratorPtr& iter, const OdGiTextStyle* pTextStyle )
  {
    OdString res;
    bool bigFont = false;
    bool bOverline = false;
    bool bUnderline = false;
    do
    {
      OdUInt16 ch = iter->nextChar();
      if ( ch == 0 ) break;
      if ( bigFont != iter->currProperties().bInBigFont 
        || bOverline != iter->currProperties().bOverlined 
        || bUnderline != iter->currProperties().bUnderlined )
      {
        if (!res.isEmpty())
          addTspan( text, res, pTextStyle, bigFont, bOverline, bUnderline );
        bigFont = iter->currProperties().bInBigFont;
        bOverline = iter->currProperties().bOverlined;
        bUnderline = iter->currProperties().bUnderlined;
        res = OD_T("");
      }
      if (ch < 18)
      {
      }
      else if ( ch < 128 )
      {
        res += (char)ch;
      }
      else 
      {
        OdChar unicodeChar = ch; 
        if (bigFont)
        {
          OdCharMapper::codepageToUnicode(ch, OdCharMapper::getCpByBigFont(pTextStyle->getBigFont()->getFileName()), unicodeChar);
        }
        OdString u; u.format( L"&#%d;", unicodeChar );
        res += u;
      }
    }
    while ( !iter->currProperties().bLastChar );
    // if font did not switch - set contents directly
    if ( text->_children.empty() && !res.isEmpty() )
    {
      text->_contents = res;
      addTextDecorations(text, bOverline, bUnderline);
      // set shx font style
      if ( pTextStyle->ttfdescriptor().typeface().isEmpty() )
        addStyle( text, pTextStyle, bigFont );
    }
    else addTspan( text, res, pTextStyle, bigFont, bOverline, bUnderline );
  }
  
  // Replace xml special characters '&','"', etc.
  //
  inline static void replaceXmlSpecials( OdString& text )
  {
    text.replace( OD_T("&"), OD_T("&amp;") );
    text.replace( OD_T("\""), OD_T("&quot;") );
    text.replace( OD_T("<"), OD_T("&lt;") );
    text.replace( OD_T(">"), OD_T("&gt;") );
  }

  // Calculate SVG ttf font size, corresponding to this textstyle.
  // This formula contradicts documentation, but is verified by practics.
  //
  inline static double textHeight( const OdGiTextStyle* pTextStyle )
  {
    return ( pTextStyle->getFont()->getHeight() 
      - pTextStyle->getFont()->getInternalLeading() )
      / pTextStyle->getFont()->fontAbove();
  }
  
  // <glyph> svg element (shx font glyph)
  //
  struct Glyph : Path 
  {
    Glyph( OdUInt16 n ) : Path( OD_T("glyph") )
    {
      OdString s;
      if (n < 18)
      {
      }
      else if ( n < 128 )
      {
        s.format( OD_T("%c"), (char)n );
        replaceXmlSpecials( s );
      }
      else s.format( OD_T("&#%d;"), n );
      addAttribute( OD_T("unicode"), s );
    }
  };

  /** \details

     This class implements SHX glyph geometry writer.
  */
  class GlyphWriter : public OdGiGeometrySimplifier
  {
  private:
    svg::Glyph * _glyph;
    OdUInt16 _n; 
    double m_dScale;
    bool   m_bGlyphMustBeFilled;
  public:
    GlyphWriter( OdUInt16 n ) : _glyph( new svg::Glyph( n ) ), _n(n), m_dScale(1.0), m_bGlyphMustBeFilled(false){}
    ~GlyphWriter(){ delete _glyph; }
    void setScale( double dScale){ m_dScale = dScale;}
    bool getGliphIsFilled(){ return m_bGlyphMustBeFilled; }
    Glyph* handOverTo( xml::Node* font )
    {
      Glyph* ret = static_cast<Glyph*>( font->addChild( _glyph ) );
      _glyph = new svg::Glyph(_n);
      return ret;
    }
    virtual void polylineOut( OdInt32 nbPoints, const OdGePoint3d* pVertexList )
    {
      if( OdZero(m_dScale - 1.0) )
      {
        _glyph->addLine( nbPoints, pVertexList );
      }
      else
      {
        OdGePoint3d* pScaledVertexList = new OdGePoint3d[nbPoints];

        for( OdInt32 i = 0; i < nbPoints; i++ )
        {
          pScaledVertexList[i] = pVertexList[i]*m_dScale;
        }

        _glyph->addLine( nbPoints, pScaledVertexList );

        delete[] pScaledVertexList;
      }
    }

    void circleProc(const OdGePoint3d& center,
      double radius, const OdGeVector3d& normal,
      const OdGeVector3d* pExtrusion = 0)
    {
      if ( !pExtrusion )
      {
        _glyph->addChild( new svg::Circle( center*m_dScale, radius*m_dScale ) );
      }
      else
        OdGiGeometrySimplifier::circleProc( center, radius, normal, pExtrusion );
    }

    TD_USING(OdGiGeometrySimplifier::circleProc); 
    
    void circularArcProc(const OdGePoint3d& start,
      const OdGePoint3d& point,
      const OdGePoint3d& end,
      OdGiArcType arcType = kOdGiArcSimple,
      const OdGeVector3d* pExtrusion = 0)
    {
      if ( pExtrusion || arcType != kOdGiArcSimple )
      {
        OdGiGeometrySimplifier::circularArcProc( start, point, end, arcType, pExtrusion );
      }
      else
      {
        OdGeCircArc3d arc(start, point, end);
        OdString s(L"M");
        s += svg::Points::formatPoint(arc.startPoint()*m_dScale);
        s += xml::Attribute::formatDouble(arc.radius()*m_dScale);
        s += L',';
        s += xml::Attribute::formatDouble(arc.radius()*m_dScale);
        s += L" 0 0,0 ";
        s += svg::Points::formatPoint(arc.endPoint()*m_dScale);
        _glyph->getAttribute( OD_T("d") )->_value += s;
      }
    }

    virtual void triangleOut(const OdInt32* vertices, const OdGeVector3d* pNormal)
    {
      const OdGePoint3d* vd = vertexDataList();

      OdGePoint3d ptsTriangle[3];
      ptsTriangle[0] = vd[vertices[0]];
      ptsTriangle[1] = vd[vertices[1]];
      ptsTriangle[2] = vd[vertices[2]];

      polylineOut(3, ptsTriangle );
    }

    virtual void shellProc(OdInt32 nbVertex,
                             const OdGePoint3d* pVertexList,
                               OdInt32 faceListSize,
                                 const OdInt32* pFaceList,
                                   const OdGiEdgeData* pEdgeData,
                                     const OdGiFaceData* pFaceData,
                                       const OdGiVertexData* pVertexData
                          )
    {
      if( !faceListSize )
      {
        return;
      }

      m_bGlyphMustBeFilled = true;

      OdUInt32 uCount = 0;

      while( uCount < (OdUInt32)faceListSize )
      {
        OdUInt32 nPtrInContour = Od_abs(pFaceList[uCount]);
        uCount++;

        OdGePoint3dArray arrPts;

        for( OdUInt32 i = 0; i < nPtrInContour; i++ )
        {
          arrPts.push_back(pVertexList[pFaceList[uCount]] );
          uCount++;
        }

        polylineProc( arrPts.size(), arrPts.asArrayPtr() );
      }
    }

    TD_USING(OdGiGeometrySimplifier::circularArcProc);
    TD_USING(OdGiGeometrySimplifier::polylineOut);
    TD_USING(OdGiGeometrySimplifier::triangleOut);
    TD_USING(OdGiGeometrySimplifier::shellProc);
  };
  
  /** \details
     This class implements the Embedded SHX font (<font> element).
  */
  class Font
  {
    friend class GlyphWriter;
    std::set<OdUInt16> _glyphs;
    std::map<OdUInt16, bool>     _glyphsFill;
    xml::Node* _font;
    OdString _name;
  public:
    Font( const OdString& name ) 
      : _font( new xml::Node( OD_T("font") ) )
      , _name( name )
    {
      // font-family must differ from <font> id
      xml::Node* fontFace = _font->addChild( new xml::Node( OD_T("font-face") ) );
      if ( _name.right(4) != OD_T(".shx") )
        _name += OD_T(".shx");
      _font->addAttribute( OD_T("id"), _name.mid( 0, _name.getLength() - 4 ) );
      fontFace->addAttribute( OD_T("font-family"), _name );
    }
    ~Font(){ delete _font; }
    void handOverTo( xml::Node* defs )
    {
      // do not add empty font
      if ( _font->_children.size() > 1 )
        defs->addChild( _font );
      else delete _font;
      _font = 0;
    }
    bool has( OdUInt16 n, bool& bFilledGlyph ) const
    {
      bool bRet = _glyphs.find( n ) != _glyphs.end();

      if( bRet )
      {
        std::map<OdUInt16, bool>::const_iterator iterGlyph = _glyphsFill.find( n );

        if( iterGlyph != _glyphsFill.end() )
        {
          bFilledGlyph = iterGlyph->second;
        }
      }

      return bRet;
    }
    Glyph* addGlyph( GlyphWriter* gw, OdUInt16 n, bool bFilledGlyph )
    {
      ODA_ASSERT( _font && !has( n, bFilledGlyph ) );
      _glyphs.insert( n );
      _glyphsFill[n] = bFilledGlyph;
      return gw->handOverTo( _font );
    }
  };

  // Embed one character glyph to svg font
  //
  void embedCharacter( svg::Font* font, OdFont* shx, OdUInt16 ch, OdChar unicodeChar, 
                       OdGiConveyorContext* pDrawContext, bool& bTextMustBeFilled )
  {
    ODA_ASSERT( font != 0 );
    if ( font->has( ch, bTextMustBeFilled ) ) return;
    svg::GlyphWriter gw( unicodeChar );

    gw.setDrawContext( pDrawContext );

    double dScale = 1.0;

    if (shx && (shx->getFlags() & OdFont::kFontRsc))
    {
      dScale = 1000.0 / shx->getHeight();
    }

    gw.setScale( dScale );

    OdGePoint2d advance;
    OdTextProperties tp;
    tp.setIncludePenups( false );
    tp.setLastChar( false );
    tp.setTrackingPercent( 1 );
    // draw geometry
    shx->drawCharacter( ch, advance, &gw, tp );
    bTextMustBeFilled = gw.getGliphIsFilled();
    svg::Glyph* g = font->addGlyph( &gw, unicodeChar, bTextMustBeFilled );
    // calculate horz-advance
    tp.setIncludePenups( true );
    shx->drawCharacter( ch, advance, &gw, tp );
    g->addAttribute( OD_T("horiz-adv-x"), advance.x * dScale );
    tp.setVerticalText( true );
    // calculate vert-advance
    shx->drawCharacter( ch, advance, &gw, tp );
    g->addAttribute( OD_T("vert-adv-y"), -advance.y * dScale );
  }
 
  // Iterate through string, and embed missing characters in svg font
  //
  void embedCharacters( svg::Font* font, svg::Font* bigfont, 
    OdBaseTextIteratorPtr& it, const OdGiTextStyle* pTextStyle, 
    OdGiConveyorContext* pDrawContext, bool& bTextMustBeFilled )
  {
    bTextMustBeFilled = false;

    do
    {
      bool bFilledGlyph = false;

      OdUInt16 ch = it->nextChar();
      if ( ch == 0 ) break;
      if ( it->currProperties().bInBigFont )
      {
        OdChar uc;
        OdCharMapper::codepageToUnicode(ch, OdCharMapper::getCpByBigFont(pTextStyle->getBigFont()->getFileName()), uc);
        embedCharacter( bigfont, pTextStyle->getBigFont(), ch, uc, pDrawContext, bFilledGlyph );
      }
      else
        embedCharacter( font, pTextStyle->getFont(), ch, ch, pDrawContext, bFilledGlyph );

      if( bFilledGlyph )
      {
        bTextMustBeFilled = true;
      }
    }
    while( !it->currProperties().bLastChar );
  }

  // Set text attributes equal for ttf and shx
  //
  void setTextAttributes( xml::Node* node, const OdGiTextStyle* pTextStyle, bool bFillText, ODCOLORREF fillColor )
  {
    if ( pTextStyle->isVertical() )
    {
      node->addAttribute( OD_T("writing-mode"), OD_T("tb") );
      node->addAttribute( OD_T("glyph-orientation-vertical"), 
        pTextStyle->isUpsideDown() ? OD_T("180") : OD_T("0") );
    }
    else
    {
      if ( pTextStyle->isBackward() )
        node->addAttribute( OD_T("writing-mode"), OD_T("rl") );
      if( pTextStyle->isUpsideDown() )
        node->addAttribute( OD_T("glyph-orientation-vertical"), OD_T("180") );

      if( bFillText )
      {
        node->addAttribute( svg::Fill( fillColor ) );
      }
    }
    if ( pTextStyle->trackingPercent() != 1 )
      node->addAttribute( OD_T("letter-spacing"), pTextStyle->trackingPercent() - 1 );
    node->addAttribute( OD_T("font-size"), svg::textHeight( pTextStyle ) );
  }
}
}
