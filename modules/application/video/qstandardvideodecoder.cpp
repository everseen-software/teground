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

#include "qstandardvideodecoder.h"
#include "opencv2/highgui/highgui.hpp"

#include <QDebug>

using namespace cv;

QStandardVideoDecoder::QStandardVideoDecoder()
    : m_capture(new VideoCapture)
    , m_currentFrame(0)
    , m_totalFrames(0)
    , m_fps(0)
    , m_width(0)
    , m_height(0)
{
}

QStandardVideoDecoder::~QStandardVideoDecoder(){
    delete m_capture;
}

bool QStandardVideoDecoder::isOpened() const{
    return m_capture->isOpened();
}

bool QStandardVideoDecoder::open(const char* file){
    if ( m_capture->open(file) ){
        m_totalFrames = static_cast<long long>(m_capture->get(CV_CAP_PROP_FRAME_COUNT));
        m_fps         = static_cast<float>(m_capture->get(CV_CAP_PROP_FPS));
        m_width       = static_cast<int>(m_capture->get(CV_CAP_PROP_FRAME_WIDTH));
        m_height      = static_cast<int>(m_capture->get(CV_CAP_PROP_FRAME_HEIGHT));
        if ( m_width == 0 || m_height == 0 ){
            if ( m_capture->grab() ){
                cv::Mat firstFrame;
                m_capture->retrieve(firstFrame);
                m_width   = firstFrame.size().width;
                m_height  = firstFrame.size().height;
                m_capture->release();
                m_capture->open(file);
            }
        }
        return true;
    }
    return false;
}

void QStandardVideoDecoder::getInfo(long long *totalFrames, float* fps, int* width, int* height){
    if ( totalFrames )
        *totalFrames = m_totalFrames;
    if ( fps )
        *fps = m_fps;
    if ( width )
        *width = m_width;
    if ( height )
        *height = m_height;
}

void QStandardVideoDecoder::release(){
    m_fps = 0.0;
    m_width = 0;
    m_height = 0;
    m_totalFrames = 0;
    m_currentFrame = 0;
    m_capture->release();
}

bool QStandardVideoDecoder::seekTo(long long time){
    m_capture->set(CV_CAP_PROP_POS_FRAMES, static_cast<double>(time));
    m_currentFrame = static_cast<long long>(m_capture->get(CV_CAP_PROP_POS_FRAMES));
    if ( m_currentFrame != time )
        return false;
    return true;
}

bool QStandardVideoDecoder::grab(){
    ++m_currentFrame;
    return m_capture->grab();
}

bool QStandardVideoDecoder::retrieve(unsigned char *, int, int, int, tg::MatPtr mat){
    cv::Mat* frame = reinterpret_cast<cv::Mat*>(mat);
    return m_capture->retrieve(*frame);
}

void QStandardVideoDecoder::deleteDecoder(tg::AbstractDecoder *decoder){
    delete decoder;
}
