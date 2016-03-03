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

#include "qtgtimelineheader.h"
#include "qtgsequencemodel.h"
#include <QVariant>

// QTGTimelineHeaderIterator
// ----------------------------------------------------------------------------

QTGTimelineHeaderIterator::QTGTimelineHeaderIterator(
        qint64 scale,
        qint64 from,
        qint64 to,
        qint64 sequencePosition,
        tg::DataFile::SequenceIterator seqIt,
        tg::DataFile::SequenceIterator seqEndIt)
    : m_scale(scale)
    , m_from(from)
    , m_to(to)
    , m_sequenceScaledPosition(sequencePosition * scale)
    , m_seqIt(seqIt)
    , m_seqEndIt(seqEndIt)
    , m_previousSequenceLength(0)
{
    if ( m_scale == 5 ){
        m_markDivision  = 25;
        m_labelDivision = 5 * 25;
    }

    qint64 calculatedFrom = m_from - (m_sequenceScaledPosition);
    m_current = calculatedFrom - (calculatedFrom % m_markDivision);
}

bool QTGTimelineHeaderIterator::isEnd() const{
    if ( m_seqIt == m_seqEndIt )
        return true;
    return m_current + m_sequenceScaledPosition > m_to;
}

void QTGTimelineHeaderIterator::nextItem(){
    m_current += m_markDivision;
    if ( m_current > m_sequenceScaledPosition + (*m_seqIt)->length() * m_scale ){
        m_sequenceScaledPosition += (*m_seqIt)->length() * m_scale;
        m_previousSequenceLength  = (*m_seqIt)->length();
        m_current = 0;
        ++m_seqIt;
    }
}

QVariant QTGTimelineHeaderIterator::data(int role){
    if ( role == QTGTimelineHeader::PositionRole)
        return m_current + m_sequenceScaledPosition;
    else if (role == QTGTimelineHeader::LengthRole)
        return QVariant(1);
    else if (role == QTGTimelineHeader::HasLabel)
        return QVariant(m_current % m_labelDivision == 0);
    else if (role == QTGTimelineHeader::Label)
        return QVariant(m_current == 0 ? m_previousSequenceLength : m_current / m_scale);
    else if (role == QTGTimelineHeader::IsDelimiter)
        return QVariant(m_current == 0);
    return QVariant();
}

// QTGTimelineHeader
// ----------------------------------------------------------------------------

QTGTimelineHeader::QTGTimelineHeader(QTGSequenceModel* sequenceModel, QObject *parent)
    : QAbstractRangeModel(parent)
    , m_scale(5)
    , m_insertionIndex(0)
    , m_sequenceModel(sequenceModel)
{
    connect(m_sequenceModel, SIGNAL(sequenceAboutToBeInserted(int)), SLOT(sequenceAboutToBeInsertedHandler(int)));
    connect(m_sequenceModel, SIGNAL(sequenceInserted()), SLOT(sequenceInsertedHandler()));
    connect(m_sequenceModel, SIGNAL(sequenceAboutToBeRemoved(int)), SLOT(sequenceAboutToBeRemovedHandler(int)));
    connect(m_sequenceModel, SIGNAL(sequenceRemoved()), SLOT(sequenceRemovedHandler()));
    connect(m_sequenceModel, SIGNAL(modelReset()), SLOT(sequenceReset()));
}

QTGTimelineHeader::~QTGTimelineHeader(){
}

QAbstractRangeModelIterator *QTGTimelineHeader::dataBetween(qint64 startPosition, qint64 endPosition){
    qint64 sequencePosition;
    tg::DataFile::SequenceIterator it = m_sequenceModel->sequenceFrom(startPosition / m_scale, sequencePosition);

    return new QTGTimelineHeaderIterator(
        m_scale,
        startPosition,
        endPosition,
        sequencePosition,
        it,
        m_sequenceModel->sequenceEnd()
    );
}

QHash<int, QByteArray> QTGTimelineHeader::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[QTGTimelineHeader::HasLabel]    = "hasLabel";
    roles[QTGTimelineHeader::Label]       = "label";
    roles[QTGTimelineHeader::IsDelimiter] = "isDelimiter";
    return roles;
}

void QTGTimelineHeader::setScale(qint64 arg){
    if (m_scale == arg)
        return;

    beginDataChange(0, m_sequenceModel->contentLength() * m_scale);
    m_scale = arg;
    endDataChange();
    emit scaleChanged(arg);
}

void QTGTimelineHeader::sequenceAboutToBeInsertedHandler(int index){
    m_insertionIndex = index;
}

void QTGTimelineHeader::sequenceInsertedHandler(){
    qint64 from = m_sequenceModel->sequencePosition(m_insertionIndex);
    beginDataChange(from, m_sequenceModel->contentLength());
    endDataChange();

    m_insertionIndex = 0;
}

void QTGTimelineHeader::sequenceAboutToBeRemovedHandler(int index){
    qint64 from = m_sequenceModel->sequencePosition(index);
    beginDataChange(from, m_sequenceModel->contentLength());
}

void QTGTimelineHeader::sequenceRemovedHandler(){
    endDataChange();
}

void QTGTimelineHeader::sequenceReset(){
    beginDataChange(0, m_sequenceModel->contentLength() * m_scale);
    endDataChange();
}
