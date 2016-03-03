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
import QtQuick.Dialogs 1.2
import Teground 1.0

Window{
    id: root
    title: "Teground - Edit Sequence"
    modality: Qt.ApplicationModal

    visible: false
    minimumWidth: 500
    minimumHeight: 330
    maximumWidth: 500
    maximumHeight: 330

    property string sequencePath : ""
    property string sequenceDecoder : ""
    property string sequenceType : ""
    property int    sequenceIndex: 0

    property variant decoderModel : null
    property variant sequenceModel : null

    property QtObject theme : ThemeDark{}

    property real videoInfoFps: 0
    property int  videoInfoFrames: 0

    signal decoderSet(int index, string decoder)
    signal canceled()

    function getVideoData(){
        root.videoInfoFps = 0
        root.videoInfoFrames = 0

        if ( selectedDecoder.selectedItem === "" )
            return;
        if ( root.decoderModel === null || root.sequenceModel === null )
            return;

        var sequenceData = root.sequenceModel.getSequenceData(selectedDecoder.selectedItem, root.sequencePath)
        if ( "error" in sequenceData ){
            errorBox.visible     = true
            errorBoxMessage.text = sequenceData["error"]
        } else {
            root.videoInfoFps    = sequenceData["fps"]
            root.videoInfoFrames = sequenceData["totalFrames"]
        }
    }

    FileDialog {
        id: videoOpenDialog
        title: "Open Video File"
        selectExisting : true
        visible : false
        onAccepted: {
            videoPathInput.text = document.pathToLocale(fileUrl)
        }
    }

    Rectangle{
        color: root.theme.editSequenceBackgroundColor
        anchors.fill: parent

        // row 1

        Text{
            anchors.top: parent.top
            anchors.topMargin: 33
            anchors.right: parent.right
            anchors.rightMargin: 420
            color: root.theme.editSequenceTextColor
            font.pixelSize: 13
            font.family: "Open Sans, Arial, Helvetica, sans-serif"
            text: "Video:"
        }

        Rectangle{
            id: videoPathInputBox
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 90
            anchors.rightMargin: 60
            height: 40
            color: root.theme.editSequenceBoxColor

            Text{
                id: videoPath
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                color: root.theme.editSequenceTextColor
                font.pixelSize: 13
                clip: true
                text: root.sequencePath
            }
        }

        // row 2

        Text{
            anchors.top: parent.top
            anchors.topMargin: 83
            anchors.right: parent.right
            anchors.rightMargin: 420
            color: root.theme.editSequenceTextColor
            font.pixelSize: 13
            font.family: "Open Sans, Arial, Helvetica, sans-serif"
            text: "Type:"
        }

        Rectangle{
            id: selectedTypeBox
            anchors.top: parent.top
            anchors.topMargin: 70
            anchors.left: parent.left
            anchors.leftMargin: 90
            width: 390
            height: 40
            color: root.theme.editSequenceBoxColor

            Text{
                id: selectedType
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                color: root.theme.editSequenceTextColor
                font.pixelSize: 13
                clip: true
                text: root.sequenceType
            }
        }

        // row 3

        Text{
            anchors.top: parent.top
            anchors.topMargin: 133
            anchors.right: parent.right
            anchors.rightMargin: 420
            color: root.theme.editSequenceTextColor
            font.pixelSize: 13
            font.family: "Open Sans, Arial, Helvetica, sans-serif"
            text: "Decoder:"
        }
        DecoderDropDown{
            id: selectedDecoder
            anchors.top: parent.top
            anchors.topMargin: 120
            anchors.left: parent.left
            anchors.leftMargin: 90
            width: 390
            height: 40

            model: root.decoderModel
            selectedItem: root.sequenceDecoder

            highlightColor: root.theme.editSequenceBoxHoverColor
            backgroundColor: root.theme.editSequenceBoxColor
            textColor: root.theme.editSequenceTextColor

            onSelectedItemChanged: {
                errorBox.visible = false
                root.getVideoData()
            }
        }

        // row 4

        Text{
            anchors.top: parent.top
            anchors.topMargin: 183
            anchors.right: parent.right
            anchors.rightMargin: 420
            color: root.theme.editSequenceTextColor
            font.pixelSize: 13
            font.family: "Open Sans, Arial, Helvetica, sans-serif"
            text: "Fps:"
        }

        Rectangle{
            id: videoInfoFpsBox
            anchors.top: parent.top
            anchors.topMargin: 170
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 90
            anchors.rightMargin: 250
            height: 40
            color: root.theme.editSequenceReadonlyBoxColor

            Text{
                id: videoInfoFps
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                color: root.theme.editSequenceTextColor
                font.pixelSize: 13
                clip: true
                text: root.videoInfoFps
            }
        }

        // row 5

        Text{
            anchors.top: parent.top
            anchors.topMargin: 233
            anchors.right: parent.right
            anchors.rightMargin: 420
            color: root.theme.editSequenceTextColor
            font.pixelSize: 13
            font.family: "Open Sans, Arial, Helvetica, sans-serif"
            text: "No. Frames:"
        }

        Rectangle{
            id: videoInfoNoFramesBox
            anchors.top: parent.top
            anchors.topMargin: 220
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 90
            anchors.rightMargin: 250
            height: 40
            color: root.theme.editSequenceReadonlyBoxColor

            Text{
                id: videoInfoNoFrames
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                color: root.theme.editSequenceTextColor
                font.pixelSize: 13
                clip: true
                text: root.videoInfoFrames
            }
        }

        // row bottom

        Rectangle{
            id: addButton
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            color: createButtonArea.containsMouse ?
                root.theme.editSequenceAddButtonHoverBackground :
                root.theme.editSequenceAddButtonBackground
            width: 140
            height: 40

            Text{
                anchors.centerIn: parent
                text: "Update"
                color: "#ccc"
                font.pixelSize: 12
                font.family: "Open Sans, Arial, Helvetica, sans-serif"
            }
            MouseArea{
                id: createButtonArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    if ( root.sequencePath === "" || selectedDecoder.selectedItem === "" ){
                        errorBox.visible = true
                        errorBoxMessage.text = "Video path and decoders must be selected!"
                    } else if ( root.videoInfoFrames == 0 || root.videoInfoFps == 0){
                        errorBox.visible = true
                        errorBoxMessage.text =
                                "Decoder cannot process file! No fps and frame count given."
                    } else {
                        root.sequenceModel.setSequenceDecoder(root.sequenceIndex, selectedDecoder.selectedItem)
                        root.sequencePath = ""
                        root.sequenceDecoder = ""
                        root.sequenceType = ""
                        root.sequenceIndex = 0
                        root.videoInfoFps = 0
                        root.videoInfoFrames = 0
                        root.close()
                        root.decoderSet(root.sequenceIndex, selectedDecoder.selectedItem)
                    }
                }
            }
        }

        Rectangle{
            id: cancelButton
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
            color: cancelButtonArea.containsMouse ?
                root.theme.editSequenceCancelButtonHoverBackground :
                root.theme.editSequenceCancelButtonBackground
            width: 140
            height: 40

            Text{
                anchors.centerIn: parent
                text: "Cancel"
                color: "#ccc"
                font.pixelSize: 12
                font.family: "Open Sans, Arial, Helvetica, sans-serif"
            }
            MouseArea{
                id: cancelButtonArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    root.sequencePath = ""
                    root.sequenceDecoder = ""
                    root.sequenceType = ""
                    root.sequenceIndex = 0
                    root.videoInfoFps = 0
                    root.videoInfoFrames = 0
                    root.close()
                    root.canceled()
                }
            }
        }

        Rectangle{
            id: errorBox

            visible: false

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 70
            anchors.left: parent.left
            anchors.leftMargin: 90
            width: 390
            height: errorBoxMessage.height + 20
            color: "#111"
            border.width: 1
            border.color: "#222"

            Text{
                id : errorBoxMessage
                anchors.centerIn: parent
                text: ""
                wrapMode: Text.WordWrap
                width: 290
                color: "#aa3333"
                font.pixelSize: 13
                font.family: "Open Sans, Arial, Helvetica, sans-serif"
            }
        }
    }
}
