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

#ifndef QTGDOCUMENT_H
#define QTGDOCUMENT_H

#include <QQuickItem>
#include "qtgdocumentpath.h"
#include "qtgsequencemodel.h"
#include "qtgtracklistmodel.h"
#include "qtgtimelineheader.h"

class QTGDocument : public QQuickItem{

    Q_OBJECT
    Q_PROPERTY(QTGSequenceModel*  sequenceList    READ sequenceList   NOTIFY sequenceListChanged)
    Q_PROPERTY(QTGTrackListModel* trackList      READ trackList      NOTIFY trackListChanged)
    Q_PROPERTY(QTGTimelineHeader* timelineHeader READ timelineHeader NOTIFY timelineHeaderChanged)
    Q_PROPERTY(QString            filePath        READ filePath       NOTIFY filePathChanged)
    Q_ENUMS(SaveSequenceBehavior)

public:
    QTGDocument(QDecoderCollection* decoderCollection, QQuickItem* parent = 0);
    ~QTGDocument();

    QTGTrackListModel* trackList();
    QTGSequenceModel* sequenceList();
    QTGTimelineHeader* timelineHeader();

    const QString& filePath() const;

public slots:
    QString openFile(const QUrl& path);
    void newFile();
    void saveToFile(const QUrl &path);
    void save();

    QString pathToLocale(const QUrl& path);

    bool containsFile(const QString& path);
    QString dataPath();
    QString dataPathName();
    QString createLocalPath(const QString& absolutePath);
    QString fileName(const QString& path);
    QString absolutePath(const QString& path);
    bool copyFileLocally(const QString& path, const QString& localPath);

signals:
    void sequenceListChanged();
    void trackListChanged();
    void timelineHeaderChanged();
    void filePathChanged();

    void error(QString message);

private:
    QTGDocumentPath*   m_path;
    tg::DataFile*      m_dataFile;
    QTGSequenceModel*  m_sequenceList;
    QTGTrackListModel* m_trackList;
    QTGTimelineHeader* m_timelineHeader;

};

inline QTGTrackListModel* QTGDocument::trackList(){
    return m_trackList;
}

inline const QString& QTGDocument::filePath() const{
    return m_path->filePath();
}

inline QTGSequenceModel *QTGDocument::sequenceList(){
    return m_sequenceList;
}

inline QTGTimelineHeader *QTGDocument::timelineHeader(){
    return m_timelineHeader;
}


#endif // QTGDOCUMENT_H
