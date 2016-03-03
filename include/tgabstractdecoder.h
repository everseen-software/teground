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

#ifndef TGABSTRACTDECODER_H
#define TGABSTRACTDECODER_H

namespace tg{

typedef void* MatPtr;

class AbstractDecoder{

public:
    AbstractDecoder(){}
    virtual ~AbstractDecoder(){}

    virtual bool isOpened() const = 0;
    virtual bool open(const char* file) = 0;
    virtual void getInfo(long long* totalFrames, float* fps, int* width, int* height) = 0;
    virtual void release() = 0;

    virtual bool seekTo(long long time) = 0;
    virtual bool grab() = 0;
    virtual bool retrieve(unsigned char* bgrBuffer, int width, int height, int step, MatPtr mat) = 0;
    virtual long long currentFrame() = 0;

};

}// namespace tg

#endif
