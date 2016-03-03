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

#ifndef QTGTIMELINEHEADER_H
#define QTGTIMELINEHEADER_H

#include "qabstractrangemodel.h"
#include "qtgsequencemodel.h"

class QTGTimelineHeaderIterator : public QAbstractRangeModelIterator{

public:
    QTGTimelineHeaderIterator(qint64 scale,
        qint64 from,
        qint64 to,
        qint64 sequencePosition,
        tg::DataFile::SequenceIterator seqIt,
        tg::DataFile::SequenceIterator seqEndIt);

    bool isEnd() const;
    void nextItem();
    QVariant data(int role = 0);

private:
    qint64 m_markDivision;
    qint64 m_labelDivision;
    qint64 m_current;
    qint64 m_scale;
    qint64 m_from;
    qint64 m_to;
    qint64 m_sequenceScaledPosition;
    qint64 m_previousSequenceLength;

    tg::DataFile::SequenceIterator m_seqIt;
    tg::DataFile::SequenceIterator m_seqEndIt;

};


class QTGTimelineHeader : public QAbstractRangeModel{

    Q_OBJECT
    Q_PROPERTY(qint64 scale READ scale WRITE setScale NOTIFY scaleChanged)

public:
    enum Role{
        HasLabel = QAbstractRangeModel::LengthRole + 1,
        Label,
        IsDelimiter
    };

public:
    QTGTimelineHeader(QTGSequenceModel* sequenceModel, QObject* parent = 0);
    ~QTGTimelineHeader();

    QAbstractRangeModelIterator* dataBetween(qint64 startPosition, qint64 endPosition);

    void setItemPosition(qint64, qint64, int, qint64){}
    void setItemLength(qint64, qint64, int, qint64){}
    void setItemData(qint64, qint64, int, int, const QVariant&){}

    virtual QHash<int, QByteArray> roleNames() const;
    qint64 scale() const;

public slots:
    void setScale(qint64 arg);

    void sequenceAboutToBeInsertedHandler(int index);
    void sequenceInsertedHandler();

    void sequenceAboutToBeRemovedHandler(int index);
    void sequenceRemovedHandler();

    void sequenceReset();

signals:
    void scaleChanged(qint64 arg);

private:
    qint64 m_scale;
    qint64 m_insertionIndex;
    QTGSequenceModel* m_sequenceModel;
};

inline qint64 QTGTimelineHeader::scale() const{
    return m_scale;
}

#endif // QTGTIMELINEHEADER_H
