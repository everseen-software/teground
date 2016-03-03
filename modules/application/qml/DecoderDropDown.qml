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

import QtQuick 2.2
import Teground 1.0

Rectangle {
    id: root

    property variant model: decoderCollection

    property color backgroundColor: "#444"
    property color highlightColor: "#44444a"
    property color textColor: "#ccc"

    property int dropBoxHeight: 80

    property alias selectedItem: chosenItemText.text
    property alias selectedIndex: listView.currentIndex

    signal comboClicked

    z: 100

    Rectangle {
        id: chosenItem
        width: parent.width
        height: root.height
        color: chosenItemMouse.containsMouse ? root.highlightColor : root.backgroundColor
        Text{
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            id: chosenItemText
            text: ''
            font.family: "Arial, Helvetica, sans-serif"
            font.pixelSize: 13
            color: "#ddd"
        }

        MouseArea{
            id: chosenItemMouse
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                root.state = root.state === "dropDown" ? "" : "dropDown"
            }
            cursorShape: Qt.PointingHandCursor
        }
    }

    Rectangle {
        id: dropDown
        width: root.width
        height: 0
        clip: true
        anchors.top: chosenItem.bottom
        color: root.backgroundColor

        ListView {
            id: listView
            height: root.dropBoxHeight
            model: root.model
            currentIndex: 0
            delegate: Rectangle {
                width: root.width
                height: root.height
                color: delegateArea.containsMouse ? root.highlightColor : "transparent"

                Text{
                    text: name
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 10
                    color: "#ddd"
                    font.family: "Arial, Helvetica, sans-serif"
                    font.pixelSize: 13
                }
                MouseArea{
                    id: delegateArea
                    anchors.fill: parent
                    onClicked: {
                        root.state = ""
                        var prevSelection = chosenItemText.text
                        chosenItemText.text = name
                        if (chosenItemText.text != prevSelection) {
                            root.comboClicked()
                        }
                        listView.currentIndex = index
                    }
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }
    }

    states: State {
        name: "dropDown"
        PropertyChanges {
            target: dropDown
            height: root.height * (root.model ? root.model.totalItems() : 1)
        }
    }

    transitions: Transition {
        NumberAnimation {
            target: dropDown
            properties: "height"
            easing.type: Easing.OutExpo
            duration: 1000
        }
    }
}
