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

#ifndef QDECODERCOLLECTION_H
#define QDECODERCOLLECTION_H

#include <QAbstractListModel>

namespace tg{ class AbstractDecoder; }

class QDecoderContainer;
class QDecoderCollection : public QAbstractListModel{

    Q_OBJECT

public:
    enum Roles{
        Path = Qt::UserRole + 1,
        Name
    };

    typedef void(*DeleteDecoderFunction)(tg::AbstractDecoder*);
    typedef tg::AbstractDecoder*(*CreateDecoderFunction)();

public:
    QDecoderCollection(QObject* parent = 0);
    ~QDecoderCollection();

    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;

    void initialize(const QString& path);

    tg::AbstractDecoder* getDecoder(const QString& name);

public slots:
    int totalItems() const;

signals:
    void loadError(QString message);

private:
    void searchPath(const QString& path);
    void addDecoder(const QString& path);

    QHash<int, QByteArray>    m_roles;
    QList<QDecoderContainer*> m_decoders;
};

inline int QDecoderCollection::totalItems() const{
    return m_decoders.size();
}

#endif // QDECODERCOLLECTION_H
