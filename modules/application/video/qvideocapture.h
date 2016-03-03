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

#ifndef QVideoCapture_HPP
#define QVideoCapture_HPP

#include "QMatDisplay.hpp"
#include "QLCVGlobal.hpp"

#include "qdecodercollection.h"

namespace tg{ class AbstractDecoder; }

class QVideoCaptureThread;
class QVideoCapture : public QQuickItem{

    Q_OBJECT
    Q_PROPERTY(QMat*   output       READ output       NOTIFY outChanged)
    Q_PROPERTY(int     totalFrames  READ totalFrames  NOTIFY totalFramesChanged)
    Q_PROPERTY(qreal   implicitFps  READ implicitFps  NOTIFY implicitFpsChanged)
    Q_PROPERTY(QString file         READ file         NOTIFY fileChanged)
    Q_PROPERTY(QString decoderName  READ decoderName  NOTIFY decoderNameChanged)
    Q_PROPERTY(bool    linearFilter READ linearFilter WRITE setLinearFilter NOTIFY linearFilterChanged)
    Q_PROPERTY(bool    paused       READ paused       WRITE setPaused       NOTIFY pausedChanged)
    Q_PROPERTY(qreal   fps          READ fps          WRITE setFps          NOTIFY fpsChanged)
    Q_PROPERTY(int     currentFrame READ currentFrame WRITE seekTo          NOTIFY outChanged)

    Q_PROPERTY(QDecoderCollection* decoders READ decoders WRITE setDecoders NOTIFY decodersChanged)

public:
    explicit QVideoCapture(QQuickItem *parent = 0);
    virtual ~QVideoCapture();

    const QString& file() const;

    bool paused() const;
    void setPaused(bool paused);

    qreal fps() const;
    void setFps(qreal fps);

    int totalFrames() const;
    int currentFrame() const;
    qreal implicitFps() const;

    QMat* output();

    bool linearFilter() const;
    void setLinearFilter(bool filter);

    QDecoderCollection* decoders() const;
    void setDecoders(QDecoderCollection* arg);

    QString decoderName() const;

public slots:
    void switchMat();
    void seekTo(int frame);

    bool openFile(const QString& file, const QString& decoderName);
    bool openFileAtPos(const QString& file, const QString& decoderName, int position);
    void closeFile();

signals:
    void outChanged();
    void linearFilterChanged();
    void fileChanged();
    void decoderNameChanged();
    void pausedChanged();
    void fpsChanged();
    void implicitFpsChanged();
    void totalFramesChanged();
    void loopChanged();
    void decodersChanged();

    void videoError(QString message);


protected:
    virtual QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *nodeData);
    virtual void componentComplete();

private:
    bool loadVideo(int seekPos = 0);

    QVideoCapture(const QVideoCapture& other);
    QVideoCapture& operator= (const QVideoCapture& other);

    QString m_file;
    qreal   m_fps;
    QMat*   m_output;
    bool    m_linearFilter;
    bool    m_paused;

    tg::AbstractDecoder*  m_decoder;
    QDecoderCollection*   m_decoderCollection;

    QVideoCaptureThread*  m_thread;

    QString m_decoderName;
};

inline const QString &QVideoCapture::file() const{
    return m_file;
}

inline qreal QVideoCapture::fps() const{
    return m_fps;
}

inline QMat *QVideoCapture::output(){
    return m_output;
}

inline bool QVideoCapture::linearFilter() const{
    return m_linearFilter;
}

inline void QVideoCapture::setLinearFilter(bool filter){
    if ( filter != m_linearFilter ){
        m_linearFilter = filter;
        emit linearFilterChanged();
        update();
    }
}

inline QDecoderCollection *QVideoCapture::decoders() const{
    return m_decoderCollection;
}

inline void QVideoCapture::setDecoders(QDecoderCollection *arg){
    if (m_decoderCollection == arg)
        return;

    m_decoderCollection = arg;
    emit decodersChanged();
}

inline QString QVideoCapture::decoderName() const{
    return m_decoderName;
}


#endif // QVideoCapture_HPP
