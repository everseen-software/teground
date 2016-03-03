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

#ifndef QTGSEQUENCETRACKLISTMODEL_HPP
#define QTGSEQUENCETRACKLISTMODEL_HPP

#include <QAbstractListModel>
#include "qabstractrangemodel.h"
#include "tgdatafile.h"

namespace tg{
class SequenceTrackList;
}
class QTGSequenceModel;

class QTGTrackListModel : public QAbstractListModel{

    Q_OBJECT

public:
    enum Roles{
        Track = Qt::UserRole + 1
    };

public:
    explicit QTGTrackListModel(QTGSequenceModel* sequenceModel, tg::DataFile* dataFile, QObject *parent = 0);
    ~QTGTrackListModel();

    virtual QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;
    int  rowCount(const QModelIndex &) const;

    void generateModel();

    void assignFromTgData(const tg::SequenceTrackList& tracklist);
    void assignToTgData(tg::SequenceTrackList& trackList);

    void signalBeginModelReset();
    void signalEndModelReset();

    void removeTrack(QAbstractRangeModel* track);
signals:

public slots:
    void appendTrack();
    void removeTrack(int index);

private:
    void clearTracks();

    QHash<int, QByteArray>      m_roles;
    QList<QAbstractRangeModel*> m_items;

    QTGSequenceModel* m_sequenceModel;
    tg::DataFile*     m_dataFile;

};

inline QHash<int, QByteArray> QTGTrackListModel::roleNames() const{
    return m_roles;
}

inline int QTGTrackListModel::rowCount(const QModelIndex&) const{
    return m_items.size();
}

inline void QTGTrackListModel::signalBeginModelReset(){
    beginResetModel();
}

inline void QTGTrackListModel::signalEndModelReset(){
    generateModel();
    endResetModel();
}

#endif // QTGSEQUENCETRACKLISTMODEL_HPP
