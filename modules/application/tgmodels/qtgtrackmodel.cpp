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

#include "qtgtrackmodel.h"
#include "qtgsequencemodel.h"
#include <QByteArray>
#include <QVariant>
#include <QHash>

using namespace tg;

// QTGTrackModelIterator
// ------------------------------------------------------------------------------

QTGTrackModelIterator::QTGTrackModelIterator(DataFile::SequenceIterator seqIt,
        DataFile::SequenceIterator seqEndIt,
        SegmentTrack::SegmentIterator segmIt,
        SegmentTrack::SegmentIterator segmEndIt,
        TrackHeader* trackHeader,
        qint64 sequencePosition,
        qint64 endPosition)
    : m_seqIt(seqIt)
    , m_seqEndIt(seqEndIt)
    , m_segmIt(segmIt)
    , m_segmEndIt(segmEndIt)
    , m_trackHeader(trackHeader)
    , m_sequencePosition(sequencePosition)
    , m_endPosition(endPosition)
{
}

bool QTGTrackModelIterator::isEnd() const{
    if ( m_seqIt == m_seqEndIt )
        return true;

    if ( m_segmIt == m_segmEndIt ){
        DataFile::SequenceIterator localSeqIt = m_seqIt;
        qint64 localSeqPosition = m_sequencePosition + (*localSeqIt)->length();

        ++localSeqIt;
        while( localSeqIt != m_seqEndIt ){
            Sequence* seq = *localSeqIt;
            SegmentTrack* segmTrack = static_cast<SegmentTrack*>(seq->track(m_trackHeader));
            if ( !segmTrack )
                return true;

            SegmentTrack::SegmentIterator localSegmIterator = segmTrack->begin();
            if ( localSegmIterator != segmTrack->end() ){
                Segment* segment = *localSegmIterator;
                if ( segment->position() + localSeqPosition >= m_endPosition )
                    return true;
                else
                    return false;
            }

            localSeqPosition += (*localSeqIt)->length();
            ++localSeqIt;
        }
        return true;
    }

    Segment* segm = *m_segmIt;
    if ( segm->position() >= m_endPosition )
        return true;
    return false;
}

void QTGTrackModelIterator::nextItem(){
    if ( m_segmIt != m_segmEndIt )
        ++m_segmIt;
    if ( m_segmIt != m_segmEndIt )
        return;

    m_sequencePosition += (*m_seqIt)->length();
    ++m_seqIt;
    while ( m_seqIt != m_seqEndIt ){
        SegmentTrack* segmTrack = static_cast<SegmentTrack*>( (*m_seqIt)->track(m_trackHeader));
        if ( segmTrack->begin() != segmTrack->end() ){
            m_segmIt    = segmTrack->begin();
            m_segmEndIt = segmTrack->end();
            return;
        }
        m_sequencePosition += (*m_seqIt)->length();
        ++m_seqIt;
    }
}

QVariant QTGTrackModelIterator::data(int role){
    if ( role == QTGTrackModel::PositionRole )
        return (*m_segmIt)->position() + m_sequencePosition;
    else if ( role == QTGTrackModel::LengthRole )
        return (*m_segmIt)->length();
    else if ( role == QTGTrackModel::ModelData)
        return QString::fromStdString((*m_segmIt)->data());
    return QVariant();
}


// QTGTrackModel
// -------------------------------------------------------------------------------

QTGTrackModel::QTGTrackModel(
        QTGSequenceModel* sequenceModel,
        tg::DataFile *dataFile,
        tg::TrackHeader* theader,
        QObject* parent)
    : QAbstractRangeModel(parent)
    , m_sequenceModel(sequenceModel)
    , m_dataFile(dataFile)
    , m_trackHeader(theader)
{
}

QTGTrackModel::~QTGTrackModel(){
}

QAbstractRangeModelIterator *QTGTrackModel::dataBetween(qint64 startPosition, qint64 endPosition){
    qint64 sequencePosition;
    SegmentTrack::SegmentIterator segmIt;
    DataFile::SequenceIterator seqIt = m_sequenceModel->sequenceFrom(startPosition, sequencePosition);
    if ( seqIt == m_dataFile->sequencesEnd() )
        return new QTGTrackModelIterator(
            seqIt,
            m_dataFile->sequencesEnd(),
            segmIt, segmIt,
            m_trackHeader,
            sequencePosition,
            endPosition
        );

    Sequence* seq = *seqIt;
    SegmentTrack* segmentTrack = static_cast<SegmentTrack*>(seq->track(m_trackHeader));
    segmIt = segmentTrack->segmentFrom(startPosition - sequencePosition);

    SegmentTrack::SegmentIterator lastSegmIt = segmIt;
    while(lastSegmIt != segmentTrack->begin()){
        --lastSegmIt;
        if ( (*lastSegmIt)->position() + (*lastSegmIt)->length() + sequencePosition > startPosition ){
            segmIt = lastSegmIt;
        } else
            break;
    }

    return new QTGTrackModelIterator(
        seqIt,
        m_dataFile->sequencesEnd(),
        segmIt,
        static_cast<SegmentTrack*>((*seqIt)->track(m_trackHeader))->end(),
        m_trackHeader,
        m_sequenceModel->sequencePosition(seqIt - m_dataFile->sequencesBegin()),
        endPosition
    );
}

void QTGTrackModel::setItemPosition(
        qint64 itemPosition,
        qint64 itemLength,
        int relativeIndex,
        qint64 itemNewPosition)
{
    qint64 sequencePosition;
    DataFile::SequenceIterator seqIt = m_sequenceModel->sequenceFrom(itemPosition, sequencePosition);
    if ( seqIt == m_dataFile->sequencesEnd() )
        return;
    Sequence* seq = *seqIt;
    SegmentTrack* segmentTrack = static_cast<SegmentTrack*>(seq->track(m_trackHeader));

    SegmentTrack::SegmentIterator segmIt = segmentTrack->segmentFrom(itemPosition - sequencePosition, itemLength);
    segmIt = segmIt + relativeIndex;
    if ( segmIt == segmentTrack->end() )
        return;

    Segment* segm = *segmIt;

    qint64 itemMiddleNewPosition = itemNewPosition + itemLength / 2;

    if ( itemMiddleNewPosition >= sequencePosition && itemMiddleNewPosition < sequencePosition + seq->length() ){
        qint64 insertPosition = itemNewPosition - sequencePosition;
        qint64 insertLength   = segm->length();
        adjustSegmentCoords(insertPosition, insertLength, (*seqIt)->length());
        segmIt = segmentTrack->assignSegmentCoords(segmIt, insertPosition, insertLength);
    } else {
        seqIt = m_sequenceModel->sequenceFrom(itemMiddleNewPosition, sequencePosition);
        if ( seqIt != m_sequenceModel->sequenceEnd() ){
            segmentTrack->removeSegment(segmIt);
            segmentTrack = static_cast<SegmentTrack*>((*seqIt)->track(m_trackHeader));
            qint64 insertPosition = itemNewPosition - sequencePosition;
            qint64 insertLength   = segm->length();
            adjustSegmentCoords(insertPosition, insertLength, (*seqIt)->length());

            Segment* segm = new Segment(insertPosition, insertLength, (*segmIt)->data());
            segmIt = segmentTrack->insertSegment(segm);
        }
    }

    QTGTrackModelIterator* modelIt = new QTGTrackModelIterator(
        seqIt,
        seqIt + 1,
        segmIt,
        segmIt + 1,
        m_trackHeader,
        sequencePosition,
        itemNewPosition + 1
    );
    emit itemCoordinatesChanged(itemPosition, itemLength, relativeIndex, modelIt, 0);
}

void QTGTrackModel::setItemLength(
        qint64 itemPosition,
        qint64 itemLength,
        int relativeIndex,
        qint64 newLength)
{
    qint64 sequencePosition;
    DataFile::SequenceIterator seqIt = m_sequenceModel->sequenceFrom(itemPosition, sequencePosition);
    if ( seqIt == m_dataFile->sequencesEnd() )
        return;
    Sequence* seq = *seqIt;
    SegmentTrack* segmentTrack = static_cast<SegmentTrack*>(seq->track(m_trackHeader));

    SegmentTrack::SegmentIterator segmIt = segmentTrack->segmentFrom(itemPosition - sequencePosition, itemLength);
    segmIt = segmIt + relativeIndex;
    if ( segmIt == segmentTrack->end() )
        return;

    Segment* segm = *segmIt;
    if ( segm->position() + newLength > seq->length() )
        newLength = seq->length() - segm->position();

    segmIt = segmentTrack->assignSegmentCoords(segmIt, itemPosition - sequencePosition, newLength);
    QTGTrackModelIterator* modelIt = new QTGTrackModelIterator(
        seqIt,
        seqIt + 1,
        segmIt,
        segmIt + 1,
        m_trackHeader,
        sequencePosition,
        itemPosition + 1
    );
    emit itemCoordinatesChanged(itemPosition, itemLength, relativeIndex, modelIt, 0);
}

void QTGTrackModel::setItemData(
        qint64 itemPosition,
        qint64 itemLength,
        int relativeIndex,
        int role,
        const QVariant &value)
{
    qint64 sequencePosition;
    DataFile::SequenceIterator it = m_sequenceModel->sequenceFrom(itemPosition, sequencePosition);
    if ( it == m_dataFile->sequencesEnd() )
        return;
    Sequence* seq = *it;
    SegmentTrack* segmentTrack = static_cast<SegmentTrack*>(seq->track(m_trackHeader));

    SegmentTrack::SegmentIterator segmIt = segmentTrack->segmentFrom(itemPosition - sequencePosition, itemLength);
    segmIt = segmIt + relativeIndex;
    if ( segmIt == segmentTrack->end() )
        return;

    Segment* segm = *segmIt;
    if ( role == QTGTrackModel::ModelData )
        segm->setData(value.toString().toStdString());

    QTGTrackModelIterator* modelIt = new QTGTrackModelIterator(
        it,
        it + 1,
        segmIt,
        segmIt + 1,
        m_trackHeader,
        sequencePosition,
        itemPosition + 1
    );
    emit itemsDataChanged(modelIt, relativeIndex, QList<int>() << QTGTrackModel::ModelData );
}

QHash<int, QByteArray> QTGTrackModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[QTGTrackModel::ModelData] = "modelData";
    return roles;
}

void QTGTrackModel::insertItem(qint64 position, qint64 length){
    qint64 middlePosition = position + length / 2;

    qint64 sequencePosition;
    DataFile::SequenceIterator seqIt = m_sequenceModel->sequenceFrom(middlePosition, sequencePosition);
    if ( seqIt == m_dataFile->sequencesEnd() )
        return;

    SegmentTrack* segmentTrack = static_cast<SegmentTrack*>((*seqIt)->track(m_trackHeader));

    qint64 insertPosition = position - sequencePosition;
    adjustSegmentCoords(insertPosition, length, (*seqIt)->length());

    beginDataChange(sequencePosition + insertPosition, sequencePosition + insertPosition + length);
    segmentTrack->insertSegment(new Segment(insertPosition, length));
    endDataChange();
}

void QTGTrackModel::removeItem(qint64 position, qint64 length, qint64 relativeIndex){
    qint64 sequencePosition;
    DataFile::SequenceIterator it = m_sequenceModel->sequenceFrom(position, sequencePosition);
    if ( it == m_dataFile->sequencesEnd() )
        return;
    Sequence* seq = *it;
    SegmentTrack* segmentTrack = static_cast<SegmentTrack*>(seq->track(m_trackHeader));

    SegmentTrack::SegmentIterator segmIt = segmentTrack->segmentFrom(position - sequencePosition, length);
    segmIt = segmIt + static_cast<size_t>(relativeIndex);
    if ( segmIt == segmentTrack->end() )
        return;

    beginDataChange(position, position + length);
    segmentTrack->removeSegment(segmIt);
    endDataChange();
}

void QTGTrackModel::sequenceAboutToBeInsertedHandler(int index){
    qint64 from = m_sequenceModel->sequencePosition(index);
    beginDataChange(from, m_sequenceModel->contentLength());
}

void QTGTrackModel::sequenceInsertedHandler(){
    endDataChange();
}

void QTGTrackModel::sequenceAboutToBeRemovedHandler(int index){
    qint64 from = m_sequenceModel->sequencePosition(index);
    beginDataChange(from, m_sequenceModel->contentLength());
}

void QTGTrackModel::sequenceRemovedHandler(){
    endDataChange();
}

void QTGTrackModel::adjustSegmentCoords(qint64 &position, qint64 &length, qint64 seqLength){
    if ( length > seqLength ){
        position = 0;
        length   = seqLength;
    } else {
        if ( position + length > seqLength )
            position = seqLength - length;
        if ( position < 0 )
            position = 0;
    }
}
