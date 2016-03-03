TEMPLATE = app
TARGET   = teground

QT += qml quick

# Uses tglib

INCLUDEPATH += $$PWD/../tglib/include
INCLUDEPATH += $$PWD/../../include

# Uses lcvlib

INCLUDEPATH += $$PWD/../lcvlib/src
DEPENDPATH  += $$PWD/../lcvlib/src

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lcvlib/release/ -llcvlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lcvlib/debug/ -llcvlib
else:unix: LIBS += -L$$OUT_PWD/../application/ -llcvlib

# Uses qml-gantt

INCLUDEPATH += $$PWD/../ganttlib/lib
DEPENDPATH  += $$PWD/../ganttlib/lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ganttlib/lib/release/ -lqmlgantt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ganttlib/lib/debug -lqmlgantt
else:unix: LIBS += -L$$OUT_PWD/../ganttlib/build -lqmlgantt

# Include sources

REQUIRES_CV_HIGHGUI = 1
REQUIRES_CV_FFMPEG  = 1
include($$PWD/../../3rdparty/opencvconfig.pro)

include($$PWD/src/application.pri)
include($$PWD/video/video.pri)
include($$PWD/tgmodels/tgmodels.pri)

# Deployment

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
