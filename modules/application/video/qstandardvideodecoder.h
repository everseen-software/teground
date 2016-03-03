/****************************************************************************
**
** Copyright (C) 2016 Everseen Ltd.
**
**
** Concept, design and implementation by Dinu SV
** (contact: mail@dinusv.com)
**
** This file is part of Teground Application.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
****************************************************************************/

#ifndef QSTANDARDVIDEODECODER_H
#define QSTANDARDVIDEODECODER_H

#include "tgabstractdecoder.h"

namespace cv{
class VideoCapture;
}

class QStandardVideoDecoder : public tg::AbstractDecoder{

public:
    QStandardVideoDecoder();
    ~QStandardVideoDecoder();

    bool isOpened() const;
    bool open(const char* file);
    void getInfo(long long* totalFrames, float* fps, int* width, int* height);
    void release();

    bool seekTo(long long time);
    bool grab();
    bool retrieve(unsigned char*, int, int, int, tg::MatPtr mat);
    long long currentFrame();

    static void deleteDecoder(tg::AbstractDecoder* decoder);

private:
    cv::VideoCapture* m_capture;
    long long         m_currentFrame;
    long long         m_totalFrames;
    float             m_fps;
    int               m_width;
    int               m_height;
};

inline long long QStandardVideoDecoder::currentFrame(){
    return m_currentFrame;
}

#endif // QSTANDARDVIDEODECODER_H
