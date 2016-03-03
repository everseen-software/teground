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

#include "qdecodercollection.h"
#include "tgabstractdecoder.h"
#include "qstandardimagedecoder.h"
#include "qstandardvideodecoder.h"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QLibrary>

#include <QDebug>


#ifdef Q_OS_WIN
#include "Windows.h"
#endif

void setDecoderLibPath(const char* path){
#ifdef Q_OS_WIN
    SetDllDirectoryA(path);
#endif
}


using namespace tg;

class QDecoderContainer{
public:
    QString path;
    QString name;
    AbstractDecoder* decoder;

    QDecoderCollection::DeleteDecoderFunction destroy;
};

QDecoderCollection::QDecoderCollection(QObject* parent)
    : QAbstractListModel(parent)
{
    m_roles[Path] = "path";
    m_roles[Name] = "name";
}

QDecoderCollection::~QDecoderCollection(){
    for ( QList<QDecoderContainer*>::iterator it = m_decoders.begin(); it != m_decoders.end(); ++it )
        delete *it;
    m_decoders.clear();
}

QHash<int, QByteArray> QDecoderCollection::roleNames() const{
    return m_roles;
}

QVariant QDecoderCollection::data(const QModelIndex& index, int role) const{
    if ( role == QDecoderCollection::Path )
        return m_decoders[index.row()]->path;
    else if ( role == QDecoderCollection::Name )
        return m_decoders[index.row()]->name;
    return QVariant();
}

int QDecoderCollection::rowCount(const QModelIndex&) const{
    return m_decoders.size();
}

void QDecoderCollection::initialize(const QString& path){

    QDecoderContainer* dec = new QDecoderContainer;
    dec->name = "StandardVideoDecoder";
    dec->path = "StandardVideoDecoder";
    dec->decoder = new QStandardVideoDecoder;
    dec->destroy = &QStandardVideoDecoder::deleteDecoder;
    m_decoders.append(dec);

    dec = new QDecoderContainer;
    dec->name = "StandardImageDecoder";
    dec->path = "StandardImageDecoder";
    dec->decoder = new QStandardImageDecoder;
    dec->destroy = &QStandardImageDecoder::deleteDecoder;
    m_decoders.append(dec);

    searchPath(path);
}

AbstractDecoder* QDecoderCollection::getDecoder(const QString &name){
    for ( QList<QDecoderContainer*>::iterator it = m_decoders.begin(); it != m_decoders.end(); ++it ){
        QDecoderContainer* dc = *it;
        if ( dc->name == name )
            return dc->decoder;
    }
    return 0;
}

void QDecoderCollection::searchPath(const QString &path){
    QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()){
        QString file = it.next();
        QFileInfo fileInfo = it.fileInfo();
        if ( fileInfo.baseName().toLower().endsWith("decoder") ){
            addDecoder(file);
        }
    }
}

void QDecoderCollection::addDecoder(const QString &path){
    setDecoderLibPath(QFileInfo(path).path().toLocal8Bit().data());

    QLibrary library(path);
    if ( !library.load() ){
        qDebug() << library.errorString();
        return;
    }

    CreateDecoderFunction createDecoder = (CreateDecoderFunction)library.resolve("createDecoder");
    if ( !createDecoder ){
        qDebug() << "Cannot access createDecoder function.";
        return;
    }

    DeleteDecoderFunction deleteDecoder = (DeleteDecoderFunction)library.resolve("deleteDecoder");
    if ( !deleteDecoder ){
        qDebug() << "Cannot access deleteDecoder function.";
        return;
    }


    QDecoderContainer* dec = new QDecoderContainer;
    dec->name = QFileInfo(path).baseName();
    dec->path = path;
    dec->decoder = createDecoder();
    dec->destroy = deleteDecoder;
    m_decoders.append(dec);
}

