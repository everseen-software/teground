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

#ifndef QTGSEQUENCEMODEL_H
#define QTGSEQUENCEMODEL_H

#include <QAbstractListModel>
#include "tgdatafile.h"

class QDecoderCollection;
class QTGDocumentPath;
class QTGSequenceModel : public QAbstractListModel{

    Q_OBJECT
    Q_PROPERTY(qint64 contentLength READ contentLength NOTIFY contentLengthChanged)

public:
    enum Roles{
        Path = Qt::UserRole + 1,
        Decoder,
        Type,
        Length
    };

public:
    explicit QTGSequenceModel(
        tg::DataFile* dataFile,
        QTGDocumentPath* docPath,
        QDecoderCollection* decoderCollection,
        QObject *parent = 0
    );
    ~QTGSequenceModel();

    virtual QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;

    void resetModel();

    qint64 contentLength() const;

    tg::DataFile::SequenceIterator sequenceFrom(qint64 position, qint64& sequencePosition);
    tg::DataFile::SequenceIterator sequenceEnd();

    void signalBeginModelReset();
    void signalEndModelReset();

signals:
    void sequenceAboutToBeInserted(int index);
    void sequenceInserted();

    void sequenceAboutToBeRemoved(int index);
    void sequenceRemoved();


    void contentLengthChanged(qint64 arg);

    void error(QString);

public slots:
    void addSequence(const QString& path, const QString &type, const QString &decoder);
    void setSequenceDecoder(int index, const QString& decoder);
    QVariant getSequenceData(const QString& decoderName, const QString& path);



    int sequenceIndex(qint64 position);
    QString sequencePath(int index);
    QString sequenceDecoder(int index);
    qint64 sequencePosition(int index);
    void removeSequence(int index);
    void moveSequence(int indexFrom, int indexTo);

    QString typeString(int type) const;

private:
    void getContentLength();

    QHash<int, QByteArray> m_roles;
    tg::DataFile*          m_dataFile;
    QTGDocumentPath*       m_documentPath;
    QDecoderCollection*    m_decoderCollection;
    qint64                 m_contentLength;
};

inline qint64 QTGSequenceModel::contentLength() const{
    return m_contentLength;
}

inline void QTGSequenceModel::signalBeginModelReset(){
    beginResetModel();
}

inline void QTGSequenceModel::signalEndModelReset(){
    getContentLength();
    endResetModel();
}

#endif // QTGSEQUENCEMODEL_H
