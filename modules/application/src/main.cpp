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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "QMat.hpp"
#include "QMatDisplay.hpp"
#include "QMatAccess.hpp"
#include "qvideocapture.h"

#include "qtgdocument.h"
#include "qtgsequencemodel.h"
#include "qtgtracklistmodel.h"
#include "qdecodercollection.h"

#include "qrangeview.h"
#include "qabstractrangemodel.h"
#include "qganttmodel.h"
#include "qganttmodelitem.h"
#include "qtgtimelineheader.h"

#include "tgdatafile.h"

using namespace cv;
using namespace tg;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QStringList arguments = app.arguments();

    QQmlApplicationEngine engine;

    QDecoderCollection decoderModel;
    decoderModel.initialize(QGuiApplication::applicationDirPath() + "/decoders");

    QTGDocument document(&decoderModel);

    qmlRegisterType<QMat>(         "Teground", 1, 0, "Mat");
    qmlRegisterType<QMatAccess>(   "Teground", 1, 0, "MatAccess");
    qmlRegisterType<QMatDisplay>(  "Teground", 1, 0, "MatDisplay");
    qmlRegisterType<QVideoCapture>("Teground", 1, 0, "VideoCapture");
    qmlRegisterType<QRangeView>(   "Teground", 1, 0, "RangeView");

    qmlRegisterUncreatableType<QTGTrackListModel>(
                "Teground", 1, 0, "TGSequenceTrackListModel", "TGSequenceTrackListModel is not constructable.");
    qmlRegisterUncreatableType<QTGDocument>(
                "Teground", 1, 0, "TGDocument", "TGDocument is not constructable.");
    qmlRegisterUncreatableType<QDecoderCollection>(
                "Teground", 1, 0, "DecoderCollection", "DecoderCollection is not constructable.");
    qmlRegisterUncreatableType<QAbstractRangeModel>(
                "Teground", 1, 0, "AbstractRangeModel", "AbstractRangeModel is of abstract type.");
    qmlRegisterUncreatableType<QTGSequenceModel>(
                "Teground", 1, 0, "SequenceModel", "SequenceModel is not constructable.");
    qmlRegisterUncreatableType<QTGTimelineHeader>(
                "Teground", 1, 0, "TimelineHeader", "TimelineHeader is not constructable.");

    qmlRegisterType<QGanttModel>(    "Teground", 1, 0, "GanttModel");
    qmlRegisterType<QGanttModelItem>("Teground", 1, 0, "GanttModelItem");

    engine.rootContext()->setContextProperty("decoderCollection", &decoderModel);
    engine.rootContext()->setContextProperty("document", &document);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if ( arguments.size() == 2 )
        document.openFile(QUrl::fromLocalFile(arguments[1]));

    return app.exec();
}
