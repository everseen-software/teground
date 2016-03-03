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

#include "qstandardimagedecoder.h"
#include "opencv2/highgui/highgui.hpp"

QStandardImageDecoder::QStandardImageDecoder()
    : m_currentFrame(0)
{
}

QStandardImageDecoder::~QStandardImageDecoder(){
}

bool QStandardImageDecoder::isOpened() const{
    return m_image.data == NULL;
}

bool QStandardImageDecoder::open(const char *file){
    m_image = cv::imread(file);
    m_currentFrame = 0;
    return m_image.data != NULL;
}

void QStandardImageDecoder::getInfo(long long *totalFrames, float *fps, int *width, int *height){
    if ( totalFrames )
        *totalFrames = 1;
    if ( fps )
        *fps = 1;
    if ( width )
        *width = m_image.cols;
    if ( height )
        *height = m_image.rows;
}

void QStandardImageDecoder::release(){
    m_image        = cv::Mat();
    m_currentFrame = 0;
}

bool QStandardImageDecoder::seekTo(long long time){
    if ( time > 1 )
        return false;
    m_currentFrame = time;
    return true;
}

bool QStandardImageDecoder::grab(){
    if ( m_currentFrame > 0 )
        return false;
    ++m_currentFrame;
    return true;
}

bool QStandardImageDecoder::retrieve(unsigned char *, int, int, int, tg::MatPtr mat){
    if ( m_currentFrame != 1 )
        return false;
    cv::Mat* matOb = reinterpret_cast<cv::Mat*>(mat);
    m_image.assignTo(*matOb);
    return true;
}

void QStandardImageDecoder::deleteDecoder(tg::AbstractDecoder *decoder){
    delete decoder;
}



