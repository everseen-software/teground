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
    title: "Teground - Add Sequence"
    modality: Qt.ApplicationModal

    visible: false
    minimumWidth: 500
    minimumHeight: 330
    maximumWidth: 500
    maximumHeight: 330

    property variant decoderModel : null
    property variant sequenceModel : null

    property QtObject theme: ThemeDark{}

    property real videoInfoFps: 0
    property int  videoInfoFrames: 0

    signal added(string videoFile, string testFile)
    signal canceled()

    function getVideoData(){
        root.videoInfoFps = 0
        root.videoInfoFrames = 0

        if ( videoPathInput.text === "" )
            return;
        if ( selectedDecoder.selectedItem === "" )
            return;
        if ( root.decoderModel === null || root.sequenceModel === null )
            return;

        var sequenceData = root.sequenceModel.getSequenceData(selectedDecoder.selectedItem, videoPathInput.text)
        if ( "error" in sequenceData ){
            errorBox.visible = true
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
        color: root.theme.addSequenceBackgroundColor
        anchors.fill: parent

        // row 1

        Text{
            anchors.top: parent.top
            anchors.topMargin: 33
            anchors.right: parent.right
            anchors.rightMargin: 420
            color: root.theme.addSequenceTextColor
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
            color: root.theme.addSequenceBoxColor

            TextInput{
                id: videoPathInput
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                selectByMouse: true
                selectionColor: "#777"
                color: root.theme.addSequenceTextColor
                font.pixelSize: 12
                clip: true
                text: ""
                onTextChanged: {
                    errorBox.visible = false
                    root.getVideoData()
                }
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
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.left: videoPathInputBox.right
            width: 40
            height: videoPathInputBox.height
            color: openPath.containsMouse ? root.theme.addSequenceBoxHoverColor : root.theme.addSequenceBoxColor
            Text{
                anchors.centerIn: parent
                text: "/"
                color: "#ccc"
                font.pixelSize: 14
                font.family: "Open Sans, Arial, Helvetica, sans-serif"
            }
            MouseArea{
                id: openPath
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    videoOpenDialog.open()
                }
            }
        }

        // row 2

        Text{
            anchors.top: parent.top
            anchors.topMargin: 83
            anchors.right: parent.right
            anchors.rightMargin: 420
            color: root.theme.addSequenceTextColor
            font.pixelSize: 13
            font.family: "Open Sans, Arial, Helvetica, sans-serif"
            text: "Type:"
        }
        DropDown{
            id: selectedType
            anchors.top: parent.top
            anchors.topMargin: 70
            anchors.left: parent.left
            anchors.leftMargin: 90
            width: 390
            height: 40
            z: 120

            model: ["Video", "Image"]

            highlightColor: root.theme.addSequenceBoxHoverColor
            backgroundColor: root.theme.addSequenceBoxColor
            textColor: root.theme.addSequenceTextColor
        }

        // row 3

        Text{
            anchors.top: parent.top
            anchors.topMargin: 133
            anchors.right: parent.right
            anchors.rightMargin: 420
            color: root.theme.addSequenceTextColor
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

            highlightColor: root.theme.addSequenceBoxHoverColor
            backgroundColor: root.theme.addSequenceBoxColor
            textColor: root.theme.addSequenceTextColor

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
            color: root.theme.addSequenceTextColor
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
            color: root.theme.addSequenceReadonlyBoxColor

            Text{
                id: videoInfoFps
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                color: root.theme.addSequenceTextColor
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
            color: root.theme.addSequenceTextColor
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
            color: root.theme.addSequenceReadonlyBoxColor

            Text{
                id: videoInfoNoFrames
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                color: root.theme.addSequenceTextColor
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
                root.theme.addSequenceAddButtonHoverBackground :
                root.theme.addSequenceAddButtonBackground
            width: 140
            height: 40
            Text{
                anchors.centerIn: parent
                text: "Add"
                color: "#ccc"
                font.pixelSize: 12
                font.family: "Open Sans, Arial, Helvetica, sans-serif"
            }
            MouseArea{
                id: createButtonArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    if ( videoPathInput.text === "" || selectedDecoder.selectedItem === "" ){
                        errorBox.visible = true
                        errorBoxMessage.text = "Video path and decoders must be selected!"
                    } else if ( root.videoInfoFrames == 0 || root.videoInfoFps == 0){
                        errorBox.visible = true
                        errorBoxMessage.text =
                                "Decoder cannot process file! No fps and frame count given."
                    } else {
                        if ( !document.containsFile(videoPathInput.text) ){
                            inputDialog.visible = true
                            inputDialog.message =
                                "The video file is not present with the current teground data file. Would you like " +
                                "to move the in the teground data directory in order to make it portable?\n" +
                                "If yes, the relative path can be changed below:\n\n" +
                                document.dataPath() + "/"
                            inputDialog.inputValue = document.fileName(videoPathInput.text)
                            inputDialog.onOkCallback = function(){
                                if ( document.copyFileLocally(videoPathInput.text, inputDialog.inputValue) ){
                                    root.sequenceModel.addSequence(
                                        inputDialog.inputValue,
                                        selectedType.selectedItem,
                                        selectedDecoder.selectedItem
                                    )
                                    videoPathInput.text = ""
                                    root.videoInfoFps = 0
                                    root.videoInfoFrames = 0
                                    root.close()
                                } else {
                                    videoPathInput.text = ""
                                    root.videoInfoFps = 0
                                    root.videoInfoFrames = 0
                                    root.close()
                                }
                            }
                            inputDialog.onCancelCallback = function(){
                                root.sequenceModel.addSequence(
                                    videoPathInput.text,
                                    selectedType.selectedItem,
                                    selectedDecoder.selectedItem
                                )
                                videoPathInput.text = ""
                                root.videoInfoFps = 0
                                root.videoInfoFrames = 0
                                root.close()
                            }
                        } else {
                            root.sequenceModel.addSequence(
                                document.createLocalPath(videoPathInput.text),
                                selectedType.selectedItem,
                                selectedDecoder.selectedItem
                            )
                            videoPathInput.text = ""
                            root.videoInfoFps = 0
                            root.videoInfoFrames = 0
                            root.close()
                        }

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
                    root.theme.addSequenceCancelButtonHoverBackground :
                    root.theme.addSequenceCancelButtonBackground
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
                    videoPathInput.text = ""
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

    InputDialogInternal{
        id: inputDialog
        anchors.fill: parent
        visible: false

        okLabel: "Yes"
        cancelLabel: "No"

        onOk: onOkCallback()
        onCancel: onCancelCallback()

        theme: root.theme

        property var onOkCallback : function(){}
        property var onCancelCallback : function(){}
    }
}
