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

#include "qvideocapture.h"
#include "qvideocapturethread.h"
#include "qstandardvideodecoder.h"
#include "QMatNode.hpp"
#include "QMatShader.hpp"
#include "QStateContainer.hpp"
#include <QSGSimpleMaterial>

#include <QTimer>
#include <QLibrary>
#include <QFileInfo>


QVideoCapture::QVideoCapture(QQuickItem *parent)
    : QQuickItem(parent)
    , m_file("")
    , m_fps(0)
    , m_output(QMat::nullMat())
    , m_linearFilter(true)
    , m_paused(true)
    , m_decoder(0)
    , m_decoderCollection(0)
    , m_thread(0)
{
    setFlag(ItemHasContents, true);
}

bool QVideoCapture::openFile(const QString &file, const QString& decoderName){
    return openFileAtPos(file, decoderName, 0);
}

bool QVideoCapture::openFileAtPos(const QString &file, const QString &decoderName, int position){
    if ( m_decoderCollection == 0 ){
        emit videoError("No decoder collection set.");
        return false;
    }

    m_decoder = m_decoderCollection->getDecoder(decoderName);
    if ( !m_decoder ){
        emit videoError("No such decoder: " + decoderName);
        return false;
    }

    if ( !QFile::exists(file) ){
        emit videoError("File does not exist: " + file);
        return false;
    }

    m_decoderName = decoderName;
    m_file        = file;
    emit fileChanged();
    emit decoderNameChanged();

    return loadVideo(position);
}

void QVideoCapture::closeFile(){
    m_output = QMat::nullMat();
    delete m_thread;
    m_thread = 0;
}

bool QVideoCapture::loadVideo(int seekPos){
    if ( !QFile::exists(m_file) )
        return false;

    if ( m_thread ){
        m_output = QMat::nullMat();
        delete m_thread;
    }

    if ( seekPos < 0 )
        seekPos = 0;

    m_thread = new QVideoCaptureThread(m_file, m_decoder, seekPos);
    m_output = m_thread->output();

    connect(m_thread, SIGNAL(inactiveMatChanged()), this, SLOT(switchMat()));

    setImplicitWidth (m_thread->captureWidth());
    setImplicitHeight(m_thread->captureHeight());
    m_fps = m_thread->captureFps();

    m_thread->setPaused(m_paused);
    if ( !m_paused ){
        m_thread->timer()->start(1000 / m_fps);
    }

    emit implicitFpsChanged();
    emit totalFramesChanged();

    return true;
}

void QVideoCapture::setFps(qreal fps){
    if ( fps != m_fps ){
        m_fps = fps;
        if ( m_thread ){
            if ( !m_thread->paused() && m_thread->isCaptureOpened() ){
                if ( m_thread->timer()->isActive() )
                    m_thread->timer()->stop();
                m_thread->timer()->start(1000 / m_fps);
            }
        }
        emit fpsChanged();
    }
}

int QVideoCapture::totalFrames() const{
    if ( m_thread )
        return m_thread->totalFrames();
    return 0;
}

int QVideoCapture::currentFrame() const{
    if ( m_thread )
        return m_thread->currentFrame();
    return 0;
}

qreal QVideoCapture::implicitFps() const{
    if ( m_thread )
        return m_thread->captureFps();
    return 0.0;
}

void QVideoCapture::seekTo(int frame){
    if ( frame >= 0 && frame < totalFrames() )
        if ( m_thread ){
            m_thread->seekTo(frame);
        }
}

void QVideoCapture::switchMat(){
    if ( m_thread ){
        m_output = m_thread->output();
        emit outChanged();
        m_thread->processNextFrame();
        update();
    }
}

void QVideoCapture::setPaused(bool paused){
    if ( m_paused != paused ){
        m_paused = paused;

        if ( m_thread ){
            m_thread->setPaused(paused);

            if ( paused )
                m_thread->timer()->stop();
            else
                m_thread->timer()->start(1000 / m_fps);
        }

        emit pausedChanged();
    }
}

bool QVideoCapture::paused() const{
    if ( m_thread )
        return m_thread->paused();
    return m_paused;
}


QSGNode *QVideoCapture::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *){
    QMatNode *n = static_cast<QMatNode*>(node);
    if (!node)
        n = new QMatNode();

    QSGGeometry::updateTexturedRectGeometry(n->geometry(), boundingRect(), QRectF(0, 0, 1, 1));
    QMatState* state = static_cast<QSGSimpleMaterial<QMatState>*>(n->material())->state();
    state->mat          = m_output;
    state->textureSync  = false;
    state->linearFilter = m_linearFilter;
    n->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

    return n;
}

void QVideoCapture::componentComplete(){
    QQuickItem::componentComplete();
}

QVideoCapture::~QVideoCapture(){
    delete m_thread;
}
