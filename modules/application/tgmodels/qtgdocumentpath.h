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

#ifndef QTGDOCUMENTPATH_H
#define QTGDOCUMENTPATH_H

#include <QString>

class QTGDocumentPath{

public:
    explicit QTGDocumentPath();

    void setFilePath(const QString& filePath);
    const QString& filePath() const;

    QString createLocalPath(const QString& absolutePath) const;
    QString dataPath() const;
    QString dataPathName() const;

    QString absolutePath(const QString& path);

private:
    QString m_filePath;

};

inline void QTGDocumentPath::setFilePath(const QString &filePath){
    m_filePath = filePath;
}

inline const QString &QTGDocumentPath::filePath() const{
    return m_filePath;
}

#endif // QTGDOCUMENTPATH_H
