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

#ifndef _DWFTK_PLOT_H
#define _DWFTK_PLOT_H


///
///\file        dwf/publisher/plot/Plot.h
///\brief       This file contains the DWFPlot class declaration.
///

#ifndef DWFTK_READ_ONLY

#include "dwfcore/STL.h"
#include "dwfcore/TempFile.h"
#include "dwfcore/OutputStream.h"
using namespace DWFCore;

#include "dwf/Toolkit.h"
#include "dwf/publisher/Publisher.h"
#include "dwf/package/Paper.h"

namespace DWFToolkit
{
///
///\ingroup         dwfpublish2d
///
///\class       DWFPlot   dwf/publisher/plot/Plot.h    "dwf/publisher/plot/Plot.h"
///\brief       A publishable object that represents a single EPlot DWF section.
///\since       7.1.0
///

class DWFPlot : public DWFPublishableSection
                _DWFTK_EXPORT_API_MEMORY_CONTROL_BASE_MEMBER
{
public:
    typedef _DWFTK_STD_VECTOR(double)   tDoubleList;

public:

    ///
    ///             Constructor
    ///
    ///             Use this constructor to manage the graphics stream manually.
    ///
    ///\param       zPlotTitle      Corresponds to the EPlot section title.
    ///\param       zPlotSource     Describes the source (application, component, etc.) of the plot data.
    ///\param       zPlotSourceID   Uniquely identifies the source (document, file, database, etc.) of the plot data.
    ///\param       zPlotSourceHRef A locator for more information about (or the actual location of) the original document.
    ///\param       zPlotLabel      A label for the plot data.
    ///\throw       None
    ///
    _DWFTK_API
    DWFPlot( const DWFString& zPlotTitle = "",
             const DWFString& zPlotSource = "",
             const DWFString& zPlotSourceID = "",
             const DWFString& zPlotSourceHRef = "",
             const DWFString& zPlotLabel = "")
        throw();

    ///
    ///             Destructor
    ///
    ///\throw       None
    ///
    _DWFTK_API
    ~DWFPlot()
        throw();

    ///
    ///             Prepares the plot for first use.
    ///
    ///\param       rPaper                  Defines the virtual paper onto which the 2D section graphics will be placed.
    ///
    ///\param       pTransform              TODO
    ///
    ///\param       pExtents                TODO
    ///
    ///\param       pClip                   TODO
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    void open( const DWFPaper& rPaper,
               const double*   pTransform = NULL,
               const double*   pExtents   = NULL,
               const double*   pClip      = NULL)
        throw( DWFException );


    ///
    ///             Completes the plot.
    ///
    ///             This method should be paired with \a open() to scope the
    ///             model's usage.
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    void close()
        throw( DWFException );

    ///
    ///             Binds a W2D data stream to the object
    ///
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    void attach( DWFInputStream* pW2DStream, bool bOwnStream )
        throw( DWFException );

    ///
    ///             Use the publisher to extract the plot structure.
    ///
    ///\param       rPublisher      The publisher.
    ///\throw       DWFException
    ///
    _DWFTK_API
    void publish( DWFPublisher& rPublisher )
        throw( DWFException );

    ///
    ///             Returns the plot paper
    ///
    ///             The virtual paper onto which the 2D section graphics will be placed. The caller must not delete
    //              this object.
    ///
    ///\throw       DWFException
    ///
    _DWFTK_API
    DWFPaper* getPaper()
        throw();

    ///
    ///             Returns the plot to paper transform.
    ///
    ///\return      TODO
    ///
    ///\throw       None
    ///
    _DWFTK_API
    void getTransform( double* pTransform ) const
        throw();

    ///
    ///             Returns the plot extents
    ///
    ///\return      TODO
    ///
    ///\throw       None
    ///
    _DWFTK_API
    void getPlotExtents( tDoubleList& rExtents) const
        throw();

    ///
    ///             Returns the plot clip
    ///
    ///\return      TODO
    ///
    ///\throw       None
    ///
    _DWFTK_API
    void getPlotClip( tDoubleList& rClip) const
        throw();

        //
        //
        // Publishable Interface
        //
        //

    ///
    ///\copydoc DWFPublishableSection::getVersion
    ///
    virtual double getVersion() const
        throw()
    {
        return _DWF_FORMAT_EPLOT_VERSION_CURRENT_FLOAT;
    }

    ///
    ///\copydoc DWFPublishableSection::getMIMEType
    ///
    _DWFTK_API
    const DWFString& getMIMEType()
        throw( DWFException );

    ///
    ///\copydoc DWFPublishableSection::getInputStream
    ///
    _DWFTK_API
    DWFInputStream* getInputStream()
        throw( DWFException );

    ///
    ///\copydoc DWFPublishableSection::embedFont
    ///
    _DWFTK_API
    void embedFont( DWFEmbeddedFont* pFont )
        throw( DWFException );

    ///
    ///\copydoc DWFPublishableSection::addResource
    ///
    _DWFTK_API
    void addResource( DWFPublishableResource* pResource )
        throw( DWFException );

    ///
    ///\copydoc DWFPublishableSection::getResources
    ///
    _DWFTK_API
    DWFIterator<DWFPublishableResource*>* getResources()
        throw( DWFException );

private:
    DWFString               _zMIME;
    DWFPaper                _oPaper;
    double                  _anPaperTransform[16];
    tDoubleList             _oExtents;
    tDoubleList             _oClip;
    DWFInputStream*         _pW2DStream;
    bool                    _bOwnStream;

    DWFEmbeddedFont::tList          _oEmbeddedFonts;
    DWFPublishableResource::tList   _oResources;

private:

    //
    // Not Implemented
    //

    DWFPlot( const DWFPlot& );
    DWFPlot& operator=( const DWFPlot& );
};

}

#endif

#endif
