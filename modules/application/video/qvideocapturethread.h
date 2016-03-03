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

#ifndef QVideoCaptureTHREAD_HPP
#define QVideoCaptureTHREAD_HPP

#include <QThread>
#include "QLCVGlobal.hpp"

namespace tg{ class AbstractDecoder; }

class QMat;
class QTimer;
class QVideoCaptureThreadPrivate;
class QVideoCaptureThread: public QThread{

    Q_OBJECT

public:
    QVideoCaptureThread(
        const QString& file,
        tg::AbstractDecoder* decoder,
        int seekPos = 0,
        QObject* parent = 0
    );
    ~QVideoCaptureThread();

    QMat*   output();
    QTimer* timer();
    const QString& file() const;
    int     captureWidth() const;
    int     captureHeight() const;
    double  captureFps() const;

    bool    paused() const;
    void    setPaused(bool paused);

    bool    isSeeking() const;

    int     currentFrame() const;
    int     totalFrames() const;
    bool    isCaptureOpened();

    void    processNextFrame();

signals:
    void inactiveMatChanged();
    void isSeekingChanged();

public slots:
    void tick();
    void seekTo(int frame);

protected:
    void run();

private:
    void beginSeek();
    void endSeek();

    QVideoCaptureThread(const QVideoCaptureThread& other);
    QVideoCaptureThread& operator= (const QVideoCaptureThread& other);

    QString m_file;
    bool    m_paused;
    int     m_framePos;
    int     m_totalFrames;
    bool    m_isSeeking;
    QMat*   m_activeMat;
    QTimer* m_timer;

    QVideoCaptureThreadPrivate* const d_ptr;

    Q_DECLARE_PRIVATE(QVideoCaptureThread)

};

inline QTimer *QVideoCaptureThread::timer(){
    return m_timer;
}

inline const QString& QVideoCaptureThread::file() const{
    return m_file;
}

inline bool QVideoCaptureThread::paused() const{
    return m_paused;
}

inline void QVideoCaptureThread::setPaused(bool paused){
    m_paused = paused;
}

inline bool QVideoCaptureThread::isSeeking() const{
    return m_isSeeking;
}

inline int QVideoCaptureThread::currentFrame() const{
    return m_framePos;
}

inline int QVideoCaptureThread::totalFrames() const{
    return m_totalFrames;
}

inline QMat *QVideoCaptureThread::output(){
    return m_activeMat;
}

#endif // QVideoCaptureTHREAD_HPP
