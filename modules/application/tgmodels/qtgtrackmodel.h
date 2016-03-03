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

#ifndef QTGTRACKMODEL_H
#define QTGTRACKMODEL_H

#include "qabstractrangemodel.h"
#include "tgdatafile.h"
#include "tgsegmenttrack.h"

class QTGSequenceModel;


class QTGTrackModelIterator : public QAbstractRangeModelIterator{

public:
    QTGTrackModelIterator(
        tg::DataFile::SequenceIterator seqIt,
        tg::DataFile::SequenceIterator seqEndIt,
        tg::SegmentTrack::SegmentIterator segmIt,
        tg::SegmentTrack::SegmentIterator segmEndIt,
        tg::TrackHeader *trackHeader,
        qint64 sequencePosition,
        qint64 endPosition
    );

    bool isEnd() const;
    void nextItem();

    QVariant data(int role);

private:
    tg::DataFile::SequenceIterator m_seqIt;
    tg::DataFile::SequenceIterator m_seqEndIt;
    tg::SegmentTrack::SegmentIterator m_segmIt;
    tg::SegmentTrack::SegmentIterator m_segmEndIt;
    tg::TrackHeader* m_trackHeader;
    qint64 m_sequencePosition;
    qint64 m_endPosition;
};


class QTGTrackModel : public QAbstractRangeModel{

    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    enum Role{
        ModelData = QAbstractRangeModel::LengthRole + 1
    };

public:
    QTGTrackModel(
        QTGSequenceModel* sequenceModel,
        tg::DataFile* dataFile,
        tg::TrackHeader* theader,
        QObject* parent = 0
    );
    ~QTGTrackModel();

    QString name() const;
    void setName(QString arg);

    QAbstractRangeModelIterator* dataBetween(qint64 startPosition, qint64 endPosition);

    void setItemPosition(
        qint64 itemPosition,
        qint64 itemLength,
        int relativeIndex,
        qint64 itemNewPosition
    );
    void setItemLength(
        qint64 itemPosition,
        qint64 itemLength,
        int relativeIndex,
        qint64 newLength
    );
    void setItemData(
        qint64 itemPosition,
        qint64 itemLength,
        int relativeIndex,
        int role,
        const QVariant& value
    );
    QHash<int, QByteArray> roleNames() const;

public slots:
    void insertItem(qint64 position, qint64 length);
    void removeItem(qint64 position, qint64 length, qint64 relativeIndex);

    void sequenceAboutToBeInsertedHandler(int index);
    void sequenceInsertedHandler();

    void sequenceAboutToBeRemovedHandler(int index);
    void sequenceRemovedHandler();

signals:
    void nameChanged();

private:
    static void adjustSegmentCoords(qint64& position, qint64& length, qint64 seqLength);

    QString m_name;

    QTGSequenceModel* m_sequenceModel;
    tg::DataFile*     m_dataFile;
    tg::TrackHeader*  m_trackHeader;

};

inline QString QTGTrackModel::name() const{
    return QString::fromStdString(m_trackHeader->name());
}

inline void QTGTrackModel::setName(QString arg){
    std::string name = arg.toStdString();
    if ( name == m_trackHeader->name() )
        return;

    m_trackHeader->setName(name);
    emit nameChanged();
}

#endif // QTGTRACKMODEL_H
