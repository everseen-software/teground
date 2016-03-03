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

#include "qtgdocumentpath.h"
#include <QFileInfo>
#include <QDir>

QTGDocumentPath::QTGDocumentPath(){
}

QString QTGDocumentPath::createLocalPath(const QString &absolutePath) const{
    return absolutePath.mid(0, QFileInfo(m_filePath).dir().absolutePath().length());
}

QString QTGDocumentPath::dataPath() const{
    QFileInfo filePathInfo(m_filePath);
    return filePathInfo.dir().absolutePath() + "/" + filePathInfo.baseName() + "_data";
}

QString QTGDocumentPath::dataPathName() const{
    return QFileInfo(m_filePath).baseName() + "_data";
}

QString QTGDocumentPath::absolutePath(const QString &path){
    QFileInfo pathInfo(path);
    if ( pathInfo.isAbsolute() )
        return path;
    else
        return QDir::cleanPath(dataPath() + "/" + path);
}


