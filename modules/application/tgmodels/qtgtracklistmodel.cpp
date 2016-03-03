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

#include "qtgtracklistmodel.h"
#include "qtgtrackmodel.h"

#include <QDebug>

QTGTrackListModel::QTGTrackListModel(QTGSequenceModel* sequenceModel, tg::DataFile* dataFile, QObject *parent)
    : QAbstractListModel(parent)
    , m_sequenceModel(sequenceModel)
    , m_dataFile(dataFile)
{
    m_roles[Track] = "track";
    generateModel();
}

QTGTrackListModel::~QTGTrackListModel(){
    clearTracks();
}

QVariant QTGTrackListModel::data(const QModelIndex& index, int role) const{
    if ( role == QTGTrackListModel::Track)
        return QVariant::fromValue(m_items[index.row()]);
    return QVariant();
}

void QTGTrackListModel::generateModel(){
    beginResetModel();
    clearTracks();
    for( tg::DataFile::TrackHeaderIterator it = m_dataFile->tracksBegin(); it != m_dataFile->tracksEnd(); ++it )
        m_items.append(new QTGTrackModel(m_sequenceModel, m_dataFile, *it));
    endResetModel();
}

void QTGTrackListModel::clearTracks(){
    for ( QList<QAbstractRangeModel*>::iterator it = m_items.begin(); it != m_items.end(); ++it ){
        delete *it;
    }
    m_items.clear();
}

void QTGTrackListModel::appendTrack(){
    beginInsertRows(QModelIndex(), m_items.length(), m_items.length());
    tg::TrackHeader* theader = m_dataFile->appendTrack("Segment");
    m_items.append(new QTGTrackModel(m_sequenceModel, m_dataFile, theader));
    endInsertRows();
}

void QTGTrackListModel::removeTrack(int index){
    beginRemoveRows(QModelIndex(), index, index);
    delete m_items.takeAt(index);
    m_dataFile->removeTrack(m_dataFile->trackAt(index));
    endRemoveRows();
}

void QTGTrackListModel::removeTrack(QAbstractRangeModel *track){
    if ( !track )
        return;
    int index = m_items.indexOf(track);
    if ( index == -1 )
        return;
    removeTrack(index);
}
