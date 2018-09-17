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

#include "PdfExportCommon.h"
#include "PdfLinearizer.h"
#include "OdBinaryData.h"

#include "qpdf/QPDF.hh"
#include "qpdf/QPDFExc.hh"
#include "qpdf/QPDFWriter.hh"
#include "qpdf/Constants.h"

#define STL_USING_ALL
#include "OdaSTL.h"

namespace TD_PDF_2D_EXPORT {

  class PdfPipeline: public Pipeline
  {
  public:
    PdfPipeline(OdStreamBufPtr& stream) :
      Pipeline("1", 0)
    {
      m_Stream = stream;
    };
    ~PdfPipeline() {};

    void write(unsigned char* data, size_t len)
    {
      m_Stream->putBytes(data, (OdUInt32)len);
    };

    void finish() {};

  private:
    OdStreamBufPtr m_Stream;
  };

  class PdfInputSource : public InputSource
  {
  public:
    PdfInputSource(OdStreamBufPtr& stream)
    {
      m_name = "";
      m_Stream = stream;
    };
    ~PdfInputSource() {};

    virtual qpdf_offset_t findAndSkipNextEOL()
    {
      qpdf_offset_t result = 0;
      bool done = false;
      char buf[10240];
      while (!done)
      {
        qpdf_offset_t cur_offset = tell();
        size_t len = read(buf, sizeof(buf));
        if (len == 0)
        {
          done = true;
          result = tell();
        }
        else
        {
          char* p1 = static_cast<char*>(memchr(buf, '\r', len));
          char* p2 = static_cast<char*>(memchr(buf, '\n', len));
          char* p = (p1 && p2) ? std::min(p1, p2) : p1 ? p1 : p2;
          if (p)
          {
            result = cur_offset + (p - buf);
            // We found \r or \n.  Keep reading until we get past
            // \r and \n characters.
            seek(result + 1, SEEK_SET);
            char ch;
            while (!done)
            {
              if (read(&ch, 1) == 0)
              {
                done = true;
              }
              else if (!((ch == '\r') || (ch == '\n')))
              {
                unreadCh(ch);
                done = true;
              }
            }
          }
        }
      }
      return result;
    };
    virtual std::string const& getName() const { return m_name; };
    virtual qpdf_offset_t tell() { return (qpdf_offset_t)m_Stream->tell(); };
    virtual void seek(qpdf_offset_t offset, int whence) { m_Stream->seek(offset, (OdDb::FilerSeekType)whence); };
    virtual void rewind() { m_Stream->rewind(); };

    virtual size_t read(char* buffer, size_t length)
    {
      setLastOffset(m_Stream->tell());
      OdUInt64 rest = m_Stream->length() - m_Stream->tell();
      if (rest < length)
        length = rest;
      m_Stream->getBytes(buffer, (OdUInt32)length);
      return length;
    };
    virtual void unreadCh(char ch) { m_Stream->seek(-1, OdDb::kSeekFromCurrent); };

  private:
    OdStreamBufPtr m_Stream;
    std::string    m_name;
  };

  void PDFLinearizer::linearize(OdStreamBufPtr& inStream, OdStreamBufPtr& outStream)
  {
    QPDF* qpdf = 0;
    QPDFWriter* qpdf_writer = 0;
    PdfPipeline* pipeline = 0;
    PdfInputSource* inputSource = 0;
    try
    {
      //init qpdf
      qpdf = new QPDF();
      
      inStream->rewind();
      inputSource = new PdfInputSource(inStream);
      //parse pdf from buffer
      qpdf->processInputSource(inputSource);

      //init pdf writer to write in memory buffer
      qpdf_writer = new QPDFWriter(*qpdf);
      pipeline = new PdfPipeline(outStream);
      qpdf_writer->setOutputPipeline(pipeline);
      //set linearization
      qpdf_writer->setLinearization(true);
      //keep streams and document structure as is
      qpdf_writer->setObjectStreamMode(qpdf_o_preserve);
      qpdf_writer->setStreamDataMode(qpdf_s_preserve);
      //linearize pdf and write it to memory buffer
      qpdf_writer->write();

      //release qpdf
      delete qpdf_writer;
      delete qpdf;
      delete pipeline;
    }
    catch (...)
    {
      outStream->putBytes(0, 0);
      delete qpdf_writer;
      delete qpdf;
      delete pipeline;
    }
  }

}
