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

import QtQuick 2.3
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2
import Teground 1.0

Rectangle{
    id: root
    property string message : ""

    visible: false
    onVisibleChanged: if (!visible) message = "";

    color: "transparent"

    property QtObject theme : ThemeDark{}

    MouseArea{
        anchors.fill: parent
        onClicked: mouse.accepted = true;
        onPressed: mouse.accepted = true;
        onReleased: mouse.accepted = true
        onDoubleClicked: mouse.accepted = true;
        onPositionChanged: mouse.accepted = true;
        onPressAndHold: mouse.accepted = true;
        onWheel: wheel.accepted = true
    }

    Rectangle{
        opacity: 0.7
        anchors.fill: parent
        color: "#000"
    }

    Rectangle{
        id: box
        anchors.centerIn: parent
        color: root.theme.errorDialogBackgroundColor
        width: 400
        height: messageLabel.height < 100 ? messageLabel.height + 100 : 200
        focus: true

        Keys.onEscapePressed: {
            root.close()
        }

        ScrollView{
            anchors.fill: parent
            anchors.bottomMargin: 60

            Text{
                id: messageLabel
                anchors.top: parent.top
                anchors.topMargin: 12
                anchors.left: parent.left
                anchors.leftMargin: 12

                text: root.message

                width: box.width - 35

                wrapMode: Text.WordWrap

                color: "#ccc"
                font.pixelSize: 12
                font.family: "Arial, Helvetica, sans-serif"
            }

        }

        Rectangle{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            color: okButtonArea.containsMouse ? "#222240" : "#222230"
            width: 100
            height: 30
            Text{
                anchors.centerIn: parent
                text: "Ok"
                color: "#ccc"
                font.pixelSize: 12
                font.family: "Arial, Helvetica, sans-serif"
            }
            MouseArea{
                id: okButtonArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    root.visible = false
                }
            }
        }
    }
}
