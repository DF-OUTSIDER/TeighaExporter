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

#if !defined XAML_PATH_GEOMETRY
#define XAML_PATH_GEOMETRY

#include "XAML/XamlAttribute.h"
#include "XAML/MemoryBuffer.h"

class XamlGraphicsObject;

class XamlPathGeometry
{
public:
    XamlPathGeometry(void);
    XamlPathGeometry(const XamlPathGeometry& s);
    virtual ~XamlPathGeometry(void);

    XamlPathGeometry& operator=(const XamlPathGeometry& s);
    bool operator==(const XamlPathGeometry& s) const;
    
    WT_Result serializeAttributeValue( WT_XAML_File &rFile, tMemoryBuffer*& rpBuffer ) const;
    WT_Result materializeAttribute( WT_XAML_File &rFile, const char* pAttribute );

public:
    class XamlPathFigure : public DWFOwner
    {
    private:
        typedef vector<WT_Point2D> tPointVector;
    public:
        typedef vector<XamlGraphicsObject*> tGraphicsVector;

    public:
        XamlPathFigure()
        : _voGraphicsObjects()
        , _oPointVector()
        {}
        XamlPathFigure(const XamlPathFigure& s);
        virtual ~XamlPathFigure() throw();
        XamlPathFigure& operator=(const XamlPathFigure& s);
        bool operator==(const XamlPathFigure& s) const;

        void appendFigure(const XamlPathFigure&);
        void addDrawable(XamlGraphicsObject* d);
        void reset();

        const tGraphicsVector& graphics() const { return _voGraphicsObjects; }

        WT_Result serializeAttributeValue( WT_XAML_File &rFile, tMemoryBuffer*& rpBuffer ) const;
        WT_Result materializeAttribute( WT_XAML_File &rFile, const char* pAttribute );

    public:
        static WT_Result getPoints( WT_XAML_File *pFile, char *& rpRead, tPointVector &points, bool bIsRelative, bool bAddFirstPoint = true );
        static WT_Result getPoint( WT_XAML_File *pFile,  char *& rpRead, WT_Point2D& rPoint, bool bIsRelative );
        static WT_Result getPointFromString( WT_XAML_File *pFile, char *pCoords, WT_Point2D& rPoint, bool bIsRelative );
        static WT_Result getScalar( char *& rpRead, double& rScalar );
        static WT_Result getScalarFromString( char *pCoords, double& rScalar);

    private:
        WT_Result _createPathGeometry( WT_XAML_File& rFile, char *& rpRead );
        char _getNextCommand( char *& rpRead ) const;

    private:
        tGraphicsVector _voGraphicsObjects;
        //used during materialization
        tPointVector _oPointVector; 

    protected:
    void notifyOwnableDeletion( DWFOwnable& rOwnable )
        throw( DWFException );

    };


    const XamlPathFigure& figure() const { return _oPathFigure; }
          XamlPathFigure& figure()       { return _oPathFigure; }

private:
    XamlPathFigure  _oPathFigure;

public:
    static const char kcAbsolute_Move;
    static const char kcRelative_Move;
    static const char kcAbsolute_Line;
    static const char kcRelative_Line;
    static const char kcAbsolute_Close;
    static const char kcRelative_Close;
    static const char kcAbsolute_HorizontalLine;
    static const char kcRelative_HorizontalLine;
    static const char kcAbsolute_VerticalLine;
    static const char kcRelative_VerticalLine;
    static const char kcAbsolute_EllipticalArc;
    static const char kcRelative_EllipticalArc;

};

#endif //XAML_PATH_GEOMETRY
