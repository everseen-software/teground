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

#include "qtgdocument.h"
#include "tgdatafile.h"

#include <QFileInfo>
#include <QDir>

#include <QDebug>

QTGDocument::QTGDocument(QDecoderCollection *decoderCollection, QQuickItem* parent)
    : QQuickItem(parent)
    , m_path(new QTGDocumentPath)
    , m_dataFile(new tg::DataFile)
    , m_sequenceList(new QTGSequenceModel(m_dataFile, m_path, decoderCollection, this))
    , m_trackList(new QTGTrackListModel(m_sequenceList, m_dataFile, this))
    , m_timelineHeader(new QTGTimelineHeader(m_sequenceList, this))
{
}

QTGDocument::~QTGDocument(){
    delete m_path;
}

QString QTGDocument::openFile(const QUrl& path){
    QString filePath = path.toLocalFile();
    if ( !QFile::exists(filePath)){
        emit error("Error opening file \'" + filePath + "\'. File does not exist.");
        return "";
    }

    m_trackList->signalBeginModelReset();
    m_sequenceList->signalBeginModelReset();

    try{
        if ( !m_dataFile->readFrom(filePath.toStdString()) )
            emit error("Cannot read from given path: " + filePath);
    } catch (tg::Exception& e){
        emit error("Exception when reading file \'" + filePath + "\': " + QString::fromStdString(e.message()));
    }

    m_path->setFilePath(filePath);

    QString sequenceList = "";
    for ( tg::DataFile::SequenceConstIterator it = m_dataFile->sequencesBegin(); it != m_dataFile->sequencesEnd(); ++it ){
        const tg::Sequence* seq = *it;
        if ( !QFileInfo(m_path->absolutePath(QString::fromStdString(seq->path()))).exists() )
            sequenceList += QString::fromStdString(seq->path()) + "\n";
    }
    if ( sequenceList != "" )
        emit error("Failed to find the following sequences, have they been moved?\n\n" + sequenceList);

    m_trackList->signalEndModelReset();
    m_sequenceList->signalEndModelReset();

    emit filePathChanged();

    return m_path->filePath();
}

void QTGDocument::newFile(){
    m_trackList->signalBeginModelReset();
    m_sequenceList->signalBeginModelReset();

    m_dataFile->clearSequences();
    m_dataFile->clearTracks();

    m_trackList->signalEndModelReset();
    m_sequenceList->signalEndModelReset();
}

void QTGDocument::saveToFile(const QUrl& path){
    QString prevFilePath = m_path->filePath();
    QString prevDataPath = m_path->dataPath();

    m_path->setFilePath(path.toLocalFile());
    QString extension = QFileInfo(m_path->filePath()).suffix();
    if ( extension == "" )
        m_path->setFilePath(m_path->filePath() + (m_path->filePath().endsWith(QChar('.')) ? "tgd" : ".tgd"));

    if ( prevFilePath == m_path->filePath() ){
        save();
        return;
    }

    for ( tg::DataFile::SequenceIterator it = m_dataFile->sequencesBegin(); it != m_dataFile->sequencesEnd(); ++it ){
        QString seqPath = QString::fromStdString((*it)->path());
        QFileInfo seqInfo(seqPath);
        if ( !seqInfo.isAbsolute() ){
            QString seqAbsolutePath = QDir::cleanPath(prevDataPath + "/" + seqPath);
            if ( !copyFileLocally(seqAbsolutePath, seqPath) ){
                m_path->setFilePath(prevFilePath);
                return;
            }
        }
    }

    m_dataFile->writeTo(m_path->filePath().toStdString());

    emit filePathChanged();
}

void QTGDocument::save(){
    m_dataFile->writeTo(m_path->filePath().toStdString());
}

QString QTGDocument::pathToLocale(const QUrl &path){
    return path.toLocalFile();
}

QString QTGDocument::dataPath(){
    return m_path->dataPath();
}

QString QTGDocument::dataPathName(){
    return m_path->dataPathName();
}

bool QTGDocument::containsFile(const QString &path){
    return QFileInfo(path).absolutePath().startsWith(dataPath());
}

QString QTGDocument::createLocalPath(const QString &absolutePath){
    return m_path->createLocalPath(absolutePath);
}

QString QTGDocument::fileName(const QString &path){
    return QFileInfo(path).fileName();
}

QString QTGDocument::absolutePath(const QString &path){
    return m_path->absolutePath(path);
}

bool QTGDocument::copyFileLocally(const QString &path, const QString &localPath){
    QDir localDir(m_path->dataPath());
    if ( !localDir.mkpath(QFileInfo(localPath).dir().path()) ){
        emit error("Failed to create path: " + QFileInfo(localPath).dir().path() );
        return false;
    }

    QString copyTo = QDir::cleanPath(localDir.absolutePath() + "/" + localPath);
    if ( !QFile(path).copy(copyTo) ){
        if ( QFile(path).exists() ){
            emit error("File already exists: " + copyTo);
            return true;
        } else {
            emit error("Failed to copy file to: " + copyTo);
            return false;
        }
    }
    return true;
}
