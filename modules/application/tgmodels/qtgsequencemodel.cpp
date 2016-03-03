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

#include "qtgsequencemodel.h"
#include "qtgdocumentpath.h"
#include "tgabstractdecoder.h"
#include "qdecodercollection.h"

#include <QDebug>

using namespace tg;

QTGSequenceModel::QTGSequenceModel(tg::DataFile *dataFile, QTGDocumentPath *docPath, QDecoderCollection* decoderCollection, QObject *parent)
    : QAbstractListModel(parent)
    , m_dataFile(dataFile)
    , m_documentPath(docPath)
    , m_decoderCollection(decoderCollection)
    , m_contentLength(0)
{
    m_roles[Path]    = "path";
    m_roles[Decoder] = "decoder";
    m_roles[Type]    = "type";
    m_roles[Length]  = "length";
}

QTGSequenceModel::~QTGSequenceModel(){
}

QHash<int, QByteArray> QTGSequenceModel::roleNames() const{
    return m_roles;
}

QVariant QTGSequenceModel::data(const QModelIndex &index, int role) const{
    if ( role == QTGSequenceModel::Path)
        return QVariant::fromValue(QString::fromStdString(m_dataFile->sequenceAt(index.row())->path()));
    else if ( role == QTGSequenceModel::Decoder )
        return QVariant::fromValue(QString::fromStdString(m_dataFile->sequenceAt(index.row())->decoder()));
    else if ( role == QTGSequenceModel::Type )
        return QVariant::fromValue((int)m_dataFile->sequenceAt(index.row())->type());
    else if ( role == QTGSequenceModel::Length )
        return QVariant::fromValue(m_dataFile->sequenceAt(index.row())->length());
    return QVariant();
}

int QTGSequenceModel::rowCount(const QModelIndex &) const{
    return static_cast<int>(m_dataFile->sequenceCount());
}

DataFile::SequenceIterator QTGSequenceModel::sequenceFrom(qint64 position, qint64 &sequencePosition){
    qint64 currentPosition = 0;
    for ( DataFile::SequenceIterator it = m_dataFile->sequencesBegin(); it != m_dataFile->sequencesEnd(); ++it){
        Sequence* seq = *it;
        if ( currentPosition + seq->length() > position ){
            sequencePosition = currentPosition;
            return it;
        }
        currentPosition += seq->length();
    }
    return m_dataFile->sequencesEnd();
}

DataFile::SequenceIterator QTGSequenceModel::sequenceEnd(){
    return m_dataFile->sequencesEnd();
}

void QTGSequenceModel::addSequence(const QString &path, const QString& type, const QString& decoder){
    beginInsertRows(QModelIndex(), (int)m_dataFile->sequenceCount(), (int)m_dataFile->sequenceCount());

    QVariantMap vmap = getSequenceData(decoder, path).toMap();
    if ( vmap.contains("error") ){
        emit error(vmap["error"].toString());
        return;
    }

    VideoTime totalFrames = vmap["totalFrames"].toLongLong();
    Sequence* seq = new Sequence(
        path.toStdString(),
        decoder.toStdString(),
        Sequence::typeFromString(type.toStdString()), totalFrames
    );

    emit sequenceAboutToBeInserted((int)m_dataFile->sequenceCount());
    m_dataFile->appendSequence(seq);
    m_contentLength += totalFrames;

    emit sequenceInserted();
    endInsertRows();
    emit contentLengthChanged(m_contentLength);
}

void QTGSequenceModel::setSequenceDecoder(int index, const QString &decoder){
    if ( index >= m_dataFile->sequenceCount() )
        return;
    Sequence* seq = m_dataFile->sequenceAt(static_cast<size_t>(index));

    QVariantMap vmap = getSequenceData(decoder, QString::fromStdString(seq->path())).toMap();
    if ( vmap.contains("error") ){
        emit error(vmap["error"].toString());
        return;
    }

    VideoTime totalFrames = vmap["totalFrames"].toLongLong();
    if ( totalFrames != seq->length() ){
        emit error("Different frame count returned by given decoder.");
        return;
    }

    seq->setDecoder(decoder.toStdString());
    emit dataChanged(
        createIndex(index, 0),
        createIndex(index, 0),
        QVector<int>() << QTGSequenceModel::Decoder
    );
}

QVariant QTGSequenceModel::getSequenceData(const QString& decoderName, const QString &path){
    QVariantMap vmap;

    tg::AbstractDecoder* decoder = m_decoderCollection->getDecoder(decoderName);
    if ( decoder->isOpened() )
        decoder->release();
    if ( !decoder ){
        vmap["error"] = "Cannot find specified decoder: " + decoderName + ".";
        return vmap;
    }

    if ( !decoder->open(m_documentPath->absolutePath(path).toStdString().c_str()) ){
        vmap["error"] = "The decoder cannot open the given file: " + path;
        return vmap;
    }

    VideoTime totalFrames = 0;
    float fps = 0.0;
    int width = 0, height = 0;
    decoder->getInfo(&totalFrames, &fps, &width, &height);
    decoder->release();

    if ( totalFrames <= 0 ){
        vmap["error"] = "Selected decoder failed fetch the number of frames for the given file.";
        return vmap;
    }
    if ( fps <= 0.0 ){
        vmap["error"] = "Selected decoder failed to get the fps for the given file.";
        return vmap;
    }

    vmap["totalFrames"] = QVariant(totalFrames);
    vmap["fps"]         = QVariant(fps);

    return QVariant::fromValue(vmap);
}

int QTGSequenceModel::sequenceIndex(qint64 position){
    qint64 sequencePosition = 0;
    tg::DataFile::SequenceIterator it = sequenceFrom(position, sequencePosition);
    if ( it == m_dataFile->sequencesEnd() )
        return -1;
    return it - m_dataFile->sequencesBegin();
}

QString QTGSequenceModel::sequencePath(int index){
    if ( index >= (int)m_dataFile->sequenceCount() )
        return "";
    return QString::fromStdString(m_dataFile->sequenceAt(index)->path());
}

QString QTGSequenceModel::sequenceDecoder(int index){
    if ( index >= (int)m_dataFile->sequenceCount() )
        return "";
    return QString::fromStdString(m_dataFile->sequenceAt(index)->decoder());
}

qint64 QTGSequenceModel::sequencePosition(int index){
    if ( index >= (int)m_dataFile->sequenceCount() )
        return 0;

    qint64 position = 0;
    for ( size_t i = 0; i < static_cast<size_t>(index); ++i ){
        position += m_dataFile->sequenceAt(i)->length();
    }
    return position;
}

void QTGSequenceModel::removeSequence(int index){
    if ( index >= (int)m_dataFile->sequenceCount() )
        return;

    beginRemoveRows(QModelIndex(), index, index);
    emit sequenceAboutToBeRemoved(index);
    Sequence* seq = m_dataFile->sequenceAt(index);
    VideoTime sequenceFrames = seq->length();
    m_dataFile->removeSequence(m_dataFile->sequenceAt(index));
    emit sequenceRemoved();
    endRemoveRows();

    m_contentLength -= sequenceFrames;
    emit contentLengthChanged(m_contentLength);
}

void QTGSequenceModel::moveSequence(int indexFrom, int indexTo){
    if ( indexFrom >= (int)m_dataFile->sequenceCount() ||
         indexTo >= (int)m_dataFile->sequenceCount() ||
         indexFrom == indexTo)
        return;

    beginResetModel();
    Sequence* seq = m_dataFile->sequenceAt(indexFrom);
    m_dataFile->moveSequence(seq, indexTo);
    endResetModel();
}

QString QTGSequenceModel::typeString(int type) const{
    return QString::fromStdString(Sequence::typeToString(static_cast<Sequence::Type>(type)));
}

void QTGSequenceModel::getContentLength(){
    m_contentLength = 0;
    for ( DataFile::SequenceConstIterator it = m_dataFile->sequencesBegin(); it != m_dataFile->sequencesEnd(); ++it ){
        m_contentLength += (*it)->length();
    }
    emit contentLengthChanged(m_contentLength);
}

