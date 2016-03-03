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

    signal ok()
    signal cancel()

    property alias okLabel: okButtonLabel.text
    property alias cancelLabel: cancelButtonLabel.text

    color: "transparent"

    property alias inputValue : inputData.text

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
        color: root.theme.inputDialogBackgroundColor
        width: 400
        height: messageLabel.height < 130 ? messageLabel.height + 130 : 200
        focus: true

        Keys.onEscapePressed: {
            root.visible = false
            root.cancel()
        }

        ScrollView{
            anchors.fill: parent
            anchors.bottomMargin: 110

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
                font.pixelSize: 13
                font.family: "Arial, Helvetica, sans-serif"
            }
        }


        Rectangle{
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 60
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 12
            height: videoPathInputBox.height
            color: root.theme.inputDialogInputBackgroundColor
            TextInput{
                id: inputData
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                selectByMouse: true
                selectionColor: "#777"
                color: root.theme.inputDialogInputTextColor
                font.pixelSize: 12
                clip: true
                text: ""
                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.IBeamCursor
                    onClicked: mouse.accepted = false;
                    onPressed: mouse.accepted = false;
                    onReleased: mouse.accepted = false
                    onDoubleClicked: mouse.accepted = false;
                    onPositionChanged: mouse.accepted = false;
                    onPressAndHold: mouse.accepted = false;
                }
            }
        }

        Rectangle{
            anchors.left: parent.left
            anchors.leftMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            color: okButtonArea.containsMouse ? "#222240" : "#222230"
            width: 100
            height: 30
            Text{
                id: okButtonLabel
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
                    root.ok()
                }
            }
        }

        Rectangle{
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            color: cancelButtonArea.containsMouse ? "#222240" : "#222230"
            width: 100
            height: 30
            Text{
                id: cancelButtonLabel
                anchors.centerIn: parent
                text: "Cancel"
                color: "#ccc"
                font.pixelSize: 12
                font.family: "Arial, Helvetica, sans-serif"
            }
            MouseArea{
                id: cancelButtonArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    root.visible = false
                    root.cancel()
                }
            }
        }
    }
}
