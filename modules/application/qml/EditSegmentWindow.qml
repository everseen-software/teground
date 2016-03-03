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

Window{
    id: root
    title: "Edit Segment"
    modality: Qt.ApplicationModal

    visible: false
    minimumWidth: 300
    minimumHeight: 200
    width: 400
    height: 200

    property int segmentPosition : 0
    property int segmentLength : 0
    property string segmentData : ""
    property variant segmentDelegate : null
    property variant segmentView : null

    signal update(string data)
    signal canceled()

    property variant theme: ThemeDark{}

    Rectangle{
        color: root.theme.segmentEditWindowBackground
        anchors.fill: parent
        focus: true

        Keys.onEscapePressed: {
            valueInput.text = ""
            root.close()
            root.canceled()
        }

        Text{
            anchors.top: parent.top
            anchors.topMargin: 12
            anchors.left: parent.left
            anchors.leftMargin: 12

            text: "Location: " + root.segmentPosition + ", " + root.segmentLength

            color: "#ccc"
            font.pixelSize: 12
            font.family: "Open Sans, Arial, Helvetica, sans-serif"
        }

        TextArea{
            id: valueInput
            anchors.top: parent.top
            anchors.topMargin: 40
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            height: parent.height - 100
            clip: true
            text: root.segmentData
            selectByMouse: true
            font.pixelSize: 12
            font.family: "Courier New, Courier"
            style: TextAreaStyle {
                textColor: "#ccc"
                selectionColor: "#eee"
                selectedTextColor: "#333"
                backgroundColor: root.theme.segmentEditWindowInput
                renderType: Text.QtRendering
            }
        }

        Rectangle{
            id: updateButton
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            color: updateButtonArea.containsMouse ? "#222240" : "#222230"
            width: 100
            height: 30
            Text{
                anchors.centerIn: parent
                text: "Update"
                color: "#ccc"
                font.pixelSize: 12
                font.family: "Open Sans, Arial, Helvetica, sans-serif"
            }
            MouseArea{
                id: updateButtonArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    root.close()
                    root.update(valueInput.text)
                    root.segmentView.setDataViaDelegate(root.segmentDelegate, "modelData", valueInput.text)
                    root.segmentView     = 0
                    root.segmentDelegate = 0
                    root.segmentPosition = 0
                    root.segmentLength   = 0
                    root.segmentData     = 0
                }
            }
        }

        Rectangle{
            id: cancelButton
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            color: cancelButtonArea.containsMouse ? "#402222" : "#302222"
            width: 100
            height: 30
            Text{
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
                    root.segmentView     = 0
                    root.segmentDelegate = 0
                    root.segmentPosition = 0
                    root.segmentLength   = 0
                    root.segmentData     = 0
                    root.close()
                    root.canceled()
                }
            }
        }
    }
}
