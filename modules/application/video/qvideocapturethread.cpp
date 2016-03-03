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

#include "qvideocapturethread.h"
#include "QMat.hpp"

#include "opencv2/highgui/highgui.hpp"
#include "tgabstractdecoder.h"

#include <QMutex>
#include <QTime>
#include <QTimer>
#include <QReadWriteLock>
#include <QWaitCondition>


//#define QVIDEO_CAPTURE_THREAD_DEBUG_FLAG
#ifdef QVIDEO_CAPTURE_THREAD_DEBUG_FLAG
#define QVIDEO_CAPTURE_THREAD_DEBUG(_param) qDebug() << (_param)
#else
#define QVIDEO_CAPTURE_THREAD_DEBUG(_param)
#endif


using namespace cv;
using namespace tg;

class QVideoCaptureThreadPrivate{

public:
    tg::AbstractDecoder* decoder;
    QMat*          inactiveMat;

    bool           abord;
    bool           inactiveMatReady;

    int            width;
    int            height;
    float          fps;

    int            seekRequest;

    QMutex         mutex;
    QWaitCondition condition;

};


/*!
  \class QVideoCaptureThread
  \internal
  \brief Internal video capture thread used by QVideoCapture.
 */

QVideoCaptureThread::QVideoCaptureThread(const QString &file, AbstractDecoder* decoder, int seekPos, QObject *parent)
    : QThread(parent)
    , m_file(file)
    , m_paused(false)
    , m_framePos(0)
    , m_totalFrames(0)
    , m_isSeeking(false)
    , m_activeMat(new QMat)
    , d_ptr(new QVideoCaptureThreadPrivate)
{
    Q_D(QVideoCaptureThread);
    d->inactiveMat      = new QMat;
    d->inactiveMatReady = false;
    d->seekRequest      = -1;
    d->decoder          = decoder;

    if ( d->decoder->isOpened() )
        d->decoder->release();

    if ( d->decoder->open(file.toStdString().c_str()) ){
        long long totalFrames;
        d->decoder->getInfo(&totalFrames, &d->fps, &d->width, &d->height);
        d->inactiveMat->cvMat()->create(d->height, d->width, CV_8UC3);
        m_activeMat->cvMat()->create(d->height, d->width, CV_8UC3);
        m_activeMat->cvMat()->setTo(0);

        m_totalFrames = static_cast<int>(totalFrames);
    }

    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(tick()));

    m_framePos = seekPos;

    if ( seekPos > 0 ){
        d->mutex.lock();
        beginSeek();
        d->decoder->seekTo(seekPos);
        m_framePos = d->decoder->currentFrame();
        endSeek();
        d->mutex.unlock();
        tick();
    }
}

QVideoCaptureThread::~QVideoCaptureThread(){
    Q_D(QVideoCaptureThread);
    d->mutex.lock();
    d->abord = true;
    d->condition.wakeOne();
    d->mutex.unlock();
    wait(); // wait till thread finishes
    QVIDEO_CAPTURE_THREAD_DEBUG( QString("Video capture \"") + m_file + "\" thread released." );
    d->decoder->release();
    delete m_timer;
    delete m_activeMat;
    delete d->inactiveMat;
    delete d;
}

bool QVideoCaptureThread::isCaptureOpened(){
    Q_D(QVideoCaptureThread);
    if ( !d->decoder->isOpened() )
        d->decoder->open(file().toStdString().c_str());

    if ( d->decoder->isOpened() ){
        long long totalFrames;
        d->decoder->getInfo(&totalFrames, &d->fps, &d->width, &d->height);
        m_totalFrames = static_cast<int>(totalFrames);
    }

    return d->decoder->isOpened();
}

void QVideoCaptureThread::processNextFrame(){
    Q_D(QVideoCaptureThread);
    d->inactiveMatReady = false;
}

void QVideoCaptureThread::tick(){
    Q_D(QVideoCaptureThread);
    QMutexLocker lock(&d->mutex);
    if ( !isRunning() ){
        start(NormalPriority);
    } else {
        if ( !d->inactiveMatReady ){
            d->condition.wakeOne();
        }
    }
}

void QVideoCaptureThread::seekTo(int frame){
    Q_D(QVideoCaptureThread);
    if ( m_totalFrames != 0 ){
        d->seekRequest = frame;
        if ( m_paused )
            tick();
    }
}

void QVideoCaptureThread::run(){
    Q_D(QVideoCaptureThread);

    d->abord = false;

    forever{
        if ( d->seekRequest != -1 ){
            if ( m_totalFrames == 0 )
                qDebug() << "Error : Seek is not available for this video.";
            else {
                if ( d->seekRequest != m_framePos ){
                    QVIDEO_CAPTURE_THREAD_DEBUG("Seek request: " + QString::number(d->seekRequest));
                    beginSeek();
                    d->decoder->seekTo(d->seekRequest);
                    m_framePos = d->decoder->currentFrame();
                    endSeek();
                }
            }
            d->seekRequest = -1;
        }

        if ( d->decoder->grab() ){
            cv::Mat* retrievedFrame = d_ptr->inactiveMat->cvMat();
            d->decoder->retrieve(
                retrievedFrame->data,
                retrievedFrame->cols,
                retrievedFrame->rows,
                static_cast<int>(retrievedFrame->step),
                retrievedFrame
            );
            d->inactiveMatReady = true;
            QMat* tempSwitch;
            tempSwitch     = d->inactiveMat;
            d->inactiveMat = m_activeMat;
            m_activeMat    = tempSwitch;
            m_framePos = d->decoder->currentFrame();
            emit inactiveMatChanged();
        } else {
            setPaused(true);
        }

        d->mutex.lock();
        if ( d->inactiveMatReady && !d->abord )
            d->condition.wait(&d->mutex);
        if ( d->abord ){
            d->mutex.unlock();
            return;
        }
        d->mutex.unlock();
    }
}

void QVideoCaptureThread::beginSeek(){
    m_isSeeking = true;
    emit isSeekingChanged();
}

void QVideoCaptureThread::endSeek(){
    m_isSeeking = false;
    emit isSeekingChanged();
}

int QVideoCaptureThread::captureWidth() const{
    Q_D(const QVideoCaptureThread);
    return d->width;
}

int QVideoCaptureThread::captureHeight() const{
    Q_D(const QVideoCaptureThread);
    return d->height;
}

double QVideoCaptureThread::captureFps() const{
    Q_D(const QVideoCaptureThread);
    return d->fps;
}
