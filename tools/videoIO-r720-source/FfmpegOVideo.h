#ifndef FFMPEGOVIDEO_H
#define FFMPEGOVIDEO_H

// $Date: 2014/11/06 $
// $Revision: #1 $

/*
videoIO: granting easy, flexible, and efficient read/write access to video 
                 files in Matlab on Windows and GNU/Linux platforms.
    
Copyright (c) 2006 Gerald Dalley
  
Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

    Portions of this software link to code licensed under the Gnu General 
    Public License (GPL).  As such, they must be licensed by the more 
    restrictive GPL license rather than this MIT license.  If you compile 
    those files, this library and any code of yours that uses it automatically
    becomes subject to the GPL conditions.  Any source files supplied by 
    this library that bear this restriction are clearly marked with internal
    comments.

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
*/

#include "debug.h"
#include "OVideo.h"
#include "matarray.h"
#include "parse.h"
#include "FfmpegCommon.h"

// Normal includes
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <math.h>
#include <limits>
#include <memory>

namespace VideoIO 
{

  // AO = "Assert Open"
#undef AO
#define AO VrRecoverableCheck(isOpen())

  /** The FfmpegOVideo class is used to write videos using the ffmpeg library
   *  (hosted at http://ffmpeg.mplayerhq.hu).  This class was designed to be
   *  used by the videoIO library for Matlab, but was written in a way that
   *  it can be easily used in other contexts as a consise abstraction to 
   *  ffmpeg.
   *
   *  Copyright (C) 2007 Gerald Dalley
   *
   *  This code is released under the MIT license (see the accompanying MIT.txt
   *  file for details), unless the ffmpeg library requires a more restrictive
   *  license.  
   */
  class FfmpegOVideo : public OVideo
  {
  public:
    // Constructors/Destructors
    FfmpegOVideo();
    virtual ~FfmpegOVideo() { TRACE; close(); };

    virtual void setup(KeyValueMap &kvm);

    // I/O Operations
    virtual void         open(std::string const &fname);
    virtual void         close();
    virtual void         addframe(int w, int h, int d,
                                  IVideo::Frame const &f);

    // Only callable when !isOpen()
    void setFramesPerSecond(double newVal);
    void setFramesPerSecond(int num, int denom);
    void setBitRate(int newVal);
    void setBitRateTolerance(int newVal);
    void setWidth(int newVal);
    void setHeight(int newVal);
    void setGopSize(int newVal);
    void setMaxBFrames(int newVal);
    void setCodec(CodecID newCodecId);
    void setCodec(std::string const &codecName);

    // Callable any time.  For int and CodecID return vals, -1 means the
    // default value will be (is being) used.  No default framesPerSecond
    // is allowed.
    bool                isOpen()              const;
    bool                isConfigurable()      const;
    virtual KeyValueMap getSetupAndStats()    const; 
    double              getFramesPerSecond()  const { return ((double)fpsNum)/fpsDenom; }
    int                 getBitRate()          const { return bitRate; }
    int                 getBitRateTolerance() const { return bitRateTolerance;}
    virtual int         getWidth()            const { return width; }
    virtual int         getHeight()           const { return height; }
    int                 getDepth()            const { return 3 /* RGB only */;}
    int                 getGopSize()          const { return gopSize; }
    int                 getMaxBFrames()       const { return maxBFrames; }
    CodecID             getCodec()            const { return codecId; }
    std::string         getCodecName()        const;

  private:
    // Helper methods
    void      finalizeOpen();  // locks in the configuration, calls openVideo
    void      openVideo();     // just opens the video file
    AVStream *addVideoStream();

    // config params
    int     fpsNum, fpsDenom;
    int     bitRate, bitRateTolerance, gopSize, maxBFrames;
    int     width, height;
    CodecID codecId;

    // runtime data structures
    AVOutputFormat    *fmt;
    AVFormatContext   *oc;
    AVStream          *videoStream;
    int64              currFrameNum;
    AVFrame           *rgbPicture;  
    AVFrame           *codecPicture;
    // perhaps this should be a function of bitRate?
    static const int  OutputBufferSize = 1000000;
    uint8             *outputBuffer;
    bool               urlOpened;   
    bool               codecOpened; 
#ifdef VIDEO_READER_USE_SWSCALER
    struct SwsContext *imgConvertCtx;
#endif 
  };

#undef AO

}; /* namespace VideoIO */

#endif 
