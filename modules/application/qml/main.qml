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
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2
import QtQuick.Dialogs 1.2
import Teground 1.0

ApplicationWindow {
    id: mainWindow

    visible: true
    width: 1024
    height: 640
    color:"#292930"

    title: qsTr("TeGround")

    property bool documentDirty : false
    property variant theme: ThemeDark{}

    FontLoader{ id: openSansRegular;    source: "qrc:/fonts/OpenSans-Regular.ttf"; }
    FontLoader{ id: openSansBold;       source: "qrc:/fonts/OpenSans-Italic.ttf"; }
    FontLoader{ id: openSansBoldItalic; source: "qrc:/fonts/OpenSans-BoldItalic.ttf"; }
    FontLoader{ id: openSansItalic;     source: "qrc:/fonts/OpenSans-Italic.ttf"; }

    Action {
        id: newAction
        text: "New..."
        shortcut: "Ctrl+N"
        onTriggered: {
            if ( mainWindow.documentDirty ){
                fileSaveDialog.onAcceptedCallback = function(){
                    videoArea.closeFile()
                    document.newFile()
                    fileSaveDialog.open()
                }
                fileSaveDialog.open()
            } else {
                document.newFile()
                fileSaveDialog.open()
            }
        }
        tooltip: "New teground file."
    }

    Action {
        id: openAction
        text: "Open..."
        shortcut: "Ctrl+O"
        onTriggered: {
            if ( mainWindow.documentDirty ){
                saveCallback = function(){
                    fileOpenDialog.open()
                }
                saveTestFile.open()
            } else {
                fileOpenDialog.open()
            }
        }
        tooltip: "Open teground file."
    }

    Action {
        id: saveAction
        text: "Save"
        shortcut: "Ctrl+S"
        onTriggered: {
            document.save()
        }
        tooltip: "Save teground file."
    }

    Action {
        id: saveAsAction
        text: "Save As..."
        onTriggered: {
            fileSaveDialog.open()
        }
        tooltip: "Save teground file as."
    }

    menuBar: MenuBar {
        style: MenuBarStyle{
            background: Rectangle{
                color: mainWindow.theme.menubarBackground
            }
            itemDelegate: Rectangle{
                implicitWidth: lab.contentWidth * 2
                implicitHeight: lab.contentHeight * 1.5
                color: styleData.selected || styleData.open ? "#1e232a" : "transparent"
                Label {
                    id: lab
                    anchors.centerIn: parent
                    color: styleData.selected  || styleData.open ? "white" : "#ccc"
                    font.wordSpacing: 10
                    text: styleData.text
                }
            }

            menuStyle: MenuStyle{
                frame: Rectangle{
                    color: mainWindow.theme.menubarBackground
                }
                itemDelegate.label: Label {
                    color: styleData.selected ? "#ccc" : "#aaa"
                    text: styleData.text
                }
            }
        }

        Menu {
            title: qsTr("File")
            MenuItem { action: newAction }
            MenuItem { action: openAction }
            MenuItem { action: saveAction }
            MenuItem { action: saveAsAction }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit()
            }
        }
    }

    AddSequenceWindow{
        id: addSequence
        visible: false

        property int recoverFrame : 0
        onVisibleChanged: {
            if ( !visible && videoArea.file != "" ){
                videoArea.openFile(videoArea.file, videoArea.decoderName)
                videoArea.seekTo(recoverFrame)
                recoverFrame = 0
            }
        }

        decoderModel: decoderCollection
        sequenceModel: document.sequenceList
    }

    EditSequenceWindow{
        id: editSequence
        visible: false

        property int recoverFrame : 0
        onVisibleChanged: {
            if ( !visible && videoArea.file != "" ){
                videoArea.openFile(videoArea.file, videoArea.decoderName)
                videoArea.seekTo(recoverFrame)
                recoverFrame = 0
            }
        }
        decoderModel: decoderCollection
        sequenceModel: document.sequenceList
    }


    EditSegmentWindow{
        id: editSegmentWindow
        visible: false
    }

    Connections{
        target: document
        onError : {
            errorDialog.message = message
            errorDialog.visible = true
        }
    }

    FileDialog {
        id: fileOpenDialog
        title: "Please choose a file"
        nameFilters: [ "Teground files (*.tgd)", "All files (*)" ]
        selectExisting : true
        visible : false
        onAccepted: {
            mainWindow.documentDirty = false
            videoArea.paused = true
            videoArea.closeFile()
            document.openFile(fileUrl)
        }
    }

    FileDialog{
        id : fileSaveDialog
        title: "Please choose a file"
        nameFilters: [ "Teground files (*.tgd)" ]
        selectExisting : false
        visible : false

        property var onAcceptedCallback : function(){}

        onAccepted: {
            document.saveToFile(fileUrl)
            mainWindow.documentDirty = false
            onAcceptedCallback()
            onAcceptedCallback = function(){}
        }
        onRejected:{
            onAcceptedCallback = function(){}
        }
    }

    Rectangle{
        id: header
        anchors.top: parent.top
        width : parent.width
        height : 20
        color: mainWindow.theme.headerBackground

        Text{
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: document.filePath
            color : "#666"
            font.pixelSize: 9
            font.weight: Font.Normal
            font.family: "Arial, Helvetica"
        }

        Text{
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: videoArea.currentFrame
            visible : hoverData.text === ""
            color : "#aaa"
            font.pixelSize: 11
            font.weight: Font.Normal
            font.family: "Open Sans, Arial, Helvetica"
        }


        Text {
            id: hoverData
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: ""
            color : "#aaa"
            font.pixelSize: 11
            font.weight: Font.Normal
            font.family: "Open Sans, Arial, Helvetica"
        }
    }

    SplitView{
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        width: parent.width
        height: parent.height - 20
        orientation: Qt.Vertical

        handleDelegate: mainWindow.theme.videoTimelineSplitHandle

        Rectangle{
            id: videoAreaBox

            height: 400

            color: mainWindow.theme.videoBackground

            property int videoHeight : height - 25

            SplitView{
                width: parent.width
                anchors.top: parent.top
                height: parent.videoHeight
                orientation: Qt.Horizontal

                handleDelegate: mainWindow.theme.videoSequenceSplitHandle

                Rectangle{
                    id: sequenceAreaBox
                    width: 300
                    height: parent.height
                    color: mainWindow.theme.sequenceBackground

                    SequenceView{
                        anchors.fill: parent
                        onIndexSwap: {
                            if ( dst !== -1 )
                                document.sequenceList.moveSequence(src, dst)
                        }
                        onItemTriggered: {
                            videoArea.openFile(document.absolutePath(path), decoderName)
                            videoArea.sequencePosition = document.sequenceList.sequencePosition(index)
                            videoArea.paused = false
                        }
                        onItemAdd:{
                            if ( document.filePath === "" ){
                                confirmDialog.message =
                                        "You need to create a new data file before you can add a sequence."
                                confirmDialog.onOkCallback = function(){
                                    fileSaveDialog.onAcceptedCallback = function(){
                                        videoArea.paused = true
                                        videoArea.closeFile()
                                        addSequence.show()
                                    }
                                    fileSaveDialog.open()
                                }
                                confirmDialog.visible = true
                            } else {
                                addSequence.recoverFrame = videoArea.currentFrame
                                videoArea.paused = true
                                videoArea.closeFile()
                                addSequence.show()
                            }
                        }
                        onItemEdit: {
                            editSequence.recoverFrame = videoArea.currentFrame
                            videoArea.paused = true
                            videoArea.closeFile()
                            editSequence.sequencePath = path
                            editSequence.sequenceDecoder = decoderName
                            editSequence.sequenceType = document.sequenceList.typeString(type)
                            editSequence.show()
                        }

                        onItemRemove:{
                            confirmDialog.message = "Are you sure you want to remove this sequence?"
                            confirmDialog.onOkCallback = function(){
                                document.sequenceList.removeSequence(index)
                            }
                            confirmDialog.visible = true

                        }

                        model: document.sequenceList
                        theme: mainWindow.theme
                    }
                }

                Rectangle{
                    height: parent.height
                    color: "transparent"

                    VideoCapture{
                        id : videoArea
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter

                        property int sequencePosition: 0

                        property real scaleWidth: parent.width / implicitWidth
                        property real scaleHeight: parent.height / implicitHeight
                        property real scaleWidthOrHeight : scaleWidth < scaleHeight ? scaleWidth : scaleHeight

                        paused : true

                        decoders: decoderCollection

                        width : implicitWidth * scaleWidthOrHeight
                        height : implicitHeight * scaleWidthOrHeight

                        onVideoError: {
                            errorDialog.visible = true
                            errorDialog.message = message
                        }
                    }
                }
            }


            Rectangle{
                id: videoControls
                width : parent.width
                height : 30
                anchors.bottom: parent.bottom

                Rectangle{
                    id: videoControlButtons
                    anchors.bottom: parent.bottom
                    height: 40
                    width: 135
                    color: mainWindow.theme.videoControlsBackground

                    Rectangle{
                        id : playPause
                        anchors.left: parent.left
                        anchors.leftMargin: 18
                        width : 25
                        height : parent.height
                        color : "transparent"
                        MouseArea{
                            anchors.fill : parent
                            onClicked : {
                                videoArea.paused = !videoArea.paused
                            }
                        }
                        Image{
                            width: 19
                            height: 19
                            anchors.centerIn: parent
                            source: videoArea.paused ? "qrc:/images/button-play.png" : "qrc:/images/button-pause.png"
                        }
                    }

                    Rectangle{
                        id : oneLeft
                        width : 25
                        height : parent.height
                        color : "transparent"
                        anchors.left: parent.left
                        anchors.leftMargin: 60
                        MouseArea{
                            anchors.fill : parent
                            onClicked : {
                                videoArea.paused = true
                                if ( videoArea.currentFrame - 2 >= 0 )
                                    videoArea.seekTo(videoArea.currentFrame - 2)
                            }
                        }
                        Image{
                            width: 14
                            height: 14
                            anchors.centerIn: parent
                            source: "qrc:/images/button-previous-frame.png"
                        }
                    }

                    Rectangle{
                        id : oneRight
                        anchors.left: parent.left
                        anchors.leftMargin: 90
                        width : 25
                        height : parent.height
                        color : "transparent"
                        MouseArea{
                            anchors.fill : parent
                            onClicked : {
                                videoArea.paused = true
                                if ( videoArea.currentFrame <= videoArea.totalFrames )
                                    videoArea.seekTo(videoArea.currentFrame)
                            }
                        }
                        Image{
                            width: 14
                            height: 14
                            anchors.centerIn: parent
                            source: "qrc:/images/button-next-frame.png"
                        }
                    }
                }

                Rectangle{
                    anchors.right : parent.right
                    anchors.top   : parent.top
                    width  : parent.width - 135
                    height : parent.height
                    color  : mainWindow.theme.videoProgressBackground
                    Rectangle{
                        anchors.top: parent.top
                        anchors.topMargin: 1
                        height : parent.height - 2
                        width : videoArea.totalFrames > 0 ?
                            Math.round( (parent.width / videoArea.totalFrames) * videoArea.currentFrame ) : 0
                        color : mainWindow.theme.videoProgressForeground
                    }

                    MouseArea{
                        anchors.fill : parent
                        hoverEnabled: true
                        onMouseXChanged: {
                            hoverData.text = Math.floor(mouse.x * (videoArea.totalFrames / parent.width))
                            if ( pressed )
                                videoArea.seekTo(mouse.x * (videoArea.totalFrames / parent.width ))
                        }
                        onExited: hoverData.text = ""
                    }

                }
            }
        }

        Rectangle{
            id: timelineArea

            width : parent.width

            color: mainWindow.theme.timelineBorderColor
            focus : true

            Keys.onPressed: {
                if ( event.key === Qt.Key_Space )
                    videoArea.paused = !videoArea.paused
            }

            Rectangle{
                id: timelineOptions
                width: timelineLeftHeader.width
                height: 35
                color: mainWindow.theme.timelineTopHeaderBackgroundColor
                Rectangle{
                    width: 35
                    height: 35
                    color: "transparent"
                    Image{
                        id: addTrackImage
                        source: "qrc:/images/button-add-track.png"
                        anchors.centerIn: parent
                        width: 18
                        height: 18
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: document.trackList.appendTrack()
                    }
                }
            }

            Rectangle{
                id: timelineLeftHeader
                color: mainWindow.theme.timelineHeaderRowBackground

                anchors.top: parent.top
                anchors.topMargin: 35

                height: parent.height - 35
                clip: true
                width: 150

                ListView{
                    id: timelineNameView

                    anchors.fill: parent

                    interactive: false
                    contentY: timelineView.contentY

                    model: document.trackList
                    delegate: Rectangle{
                        id: trackNameDelegate
                        width: parent.width
                        height: 32
                        color: mainWindow.theme.timelineHeaderBorderColor

                        Rectangle{
                            color: mainWindow.theme.timelineHeaderRowBackground
                            width: parent.width
                            height: 31
                            anchors.top: parent.top
                            Text{
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: track.name
                                font.family: "Open Sans, Arial, Helvetica, sans-serif"
                                font.pixelSize: 13
                                color: "#ccc"
                            }
                            MouseArea{
                                anchors.fill: parent
                                acceptedButtons: Qt.LeftButton | Qt.RightButton
                                onDoubleClicked: {
                                    if ( mouse.button & Qt.LeftButton ){
                                        trackEditBox.visible = true
                                        trackEdit.selectAll()
                                        trackEdit.forceActiveFocus()
                                    }
                                }
                                onClicked: {
                                    if ( mouse.button & Qt.RightButton ){
                                        if ( trackContextMenu.activeFocus )
                                            trackContextMenu.focus = false
                                        else
                                            trackContextMenu.forceActiveFocus()
                                    }
                                }
                            }
                            Rectangle{
                                id: trackContextMenu
                                anchors.right: parent.right

                                width: 0
                                height: 32
                                color: "#000"
                                visible: activeFocus

                                onVisibleChanged: if(visible) width = 50; else width = 0

                                Behavior on width{
                                    SmoothedAnimation{ duration: 400 }
                                }

                                Rectangle{
                                    anchors.left: parent.left
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 20
                                    height: 20
                                    MouseArea{
                                        anchors.fill: parent
                                        onClicked:{
                                            trackEditBox.visible = true
                                            trackEdit.selectAll()
                                            trackEdit.forceActiveFocus()
                                        }
                                    }
                                }

                                Rectangle{
                                    anchors.left: parent.left
                                    anchors.leftMargin: 25
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 20
                                    height: 20
                                    MouseArea{
                                        anchors.fill: parent
                                        onClicked: document.trackList.removeTrack(index)
                                    }
                                }
                            }
                            Rectangle{
                                id: trackEditBox
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.fill: parent
                                anchors.margins: 5
                                visible: false
                                color: "#aaa"
                                border.color: "#6666cc"
                                border.width: 1
                                TextInput{
                                    id: trackEdit
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 5
                                    width: parent.width - 10
                                    text: track.name
                                    color: "#000"
                                    font.pixelSize: 13
                                    font.bold: false
                                    font.family: "Open Sans, Arial, Helvetica, sans-serif"
                                    selectByMouse: true
                                    selectionColor: "#444499"
                                    onActiveFocusChanged: {
                                        if ( !activeFocus ){
                                            track.name = text
                                            trackEditBox.visible = false
                                        }
                                    }
                                    Keys.onReturnPressed: {
                                        track.name = text
                                        event.accepted = true
                                        trackEditBox.visible = false
                                    }
                                    Keys.onEnterPressed: {
                                        track.name = text
                                        event.accepted = true
                                        trackEditBox.visible = false
                                    }

                                    Keys.onEscapePressed: {
                                        text = track.name
                                        event.accepted = true
                                        trackEditBox.visible = false
                                    }
                                    MouseArea{
                                        anchors.fill: parent
                                        acceptedButtons: Qt.NoButton
                                        cursorShape: Qt.IBeamCursor
                                    }
                                }
                            }
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        onWheel: {
                            wheel.accepted = true
                            var newContentY = timelineView.contentY -= wheel.angleDelta.y / 6
                            if ( newContentY > timelineView.contentHeight - timelineView.height )
                                timelineView.contentY = timelineView.contentHeight - timelineView.height
                            else if ( newContentY < 0 )
                                timelineView.contentY = 0
                            else
                                timelineView.contentY = newContentY
                        }
                        onClicked: mouse.accepted = false;
                        onPressed: mouse.accepted = false;
                        onReleased: mouse.accepted = false
                        onDoubleClicked: mouse.accepted = false;
                        onPositionChanged: mouse.accepted = false;
                        onPressAndHold: mouse.accepted = false;
                    }
                }

                Rectangle{
                    width: parent.width
                    height: 10
                    color: mainWindow.theme.timelineScrollBackground
                    anchors.bottom: parent.bottom
                }
            }

            Flickable{
                contentX: timelineView.contentX
                anchors.left: parent.left
                anchors.leftMargin: 150
                height: parent.height
                width: parent.width - 150
                interactive: false
                clip: true

                Rectangle{
                    id : timelineTopHeader

                    property int scale: timeline.zoom

                    width : scale * document.sequenceList.contentLength
                    height : 35
                    color : mainWindow.theme.timelineTopHeaderBackgroundColor

                    RangeView{
                        id: timelineHeaderView
                        height: parent.height
                        width: parent.width
                        model: document.timelineHeader

                        viewportX: timelineView.contentX
                        viewportWidth: timelineView.width

                        delegate: Component{
                            Item{
                                x: position
                                height : timelineTopHeader.height

                                Text{
                                    anchors.top: parent.top
                                    anchors.topMargin: 10

                                    anchors.horizontalCenter: isDelimiter ? undefined : parent.horizontalCenter
                                    anchors.right: isDelimiter ? parent.left : undefined
                                    anchors.rightMargin: isDelimiter ? 8 : undefined

                                    text: label
                                    color: "#999"
                                    font.pixelSize: 8
                                    visible: hasLabel
                                }

                                Text{
                                    anchors.top: parent.top
                                    anchors.topMargin: 10

                                    anchors.left: parent.right
                                    anchors.leftMargin: 8

                                    text: "0"
                                    color: "#999"
                                    font.pixelSize: 8
                                    visible: isDelimiter
                                }

                                Rectangle{
                                    width: 10
                                    height: 1
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    y: 33 - 18
                                    color: "#999"
                                    visible: isDelimiter
                                }

                                Rectangle{
                                    width: 1
                                    height: hasLabel ? isDelimiter ? 17 : 8 : 5
                                    y: 33 - height
                                    color: "#999"
                                }
                            }
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true
                        onMouseXChanged: {
                            var seekPosition     = Math.floor(mouse.x / timelineTopHeader.scale)
                            var sequencePosition = videoArea.sequencePosition
                            if ( ( seekPosition >= 0 ) &&
                                 (  seekPosition < sequencePosition ||
                                    seekPosition >= sequencePosition + videoArea.totalFrames)
                            ){
                                var sequenceIndex = document.sequenceList.sequenceIndex(seekPosition)
                                if ( sequenceIndex === -1 )
                                    return;
                                sequencePosition  = document.sequenceList.sequencePosition(sequenceIndex)

                                if ( pressed ){
                                    videoArea.openFileAtPos(
                                        document.absolutePath(document.sequenceList.sequencePath(sequenceIndex)),
                                        document.sequenceList.sequenceDecoder(sequenceIndex),
                                        seekPosition - sequencePosition
                                    )
                                    videoArea.sequencePosition = sequencePosition
                                }
                            } else if ( pressed )
                                videoArea.seekTo(seekPosition - sequencePosition)
                            hoverData.text = seekPosition - sequencePosition
//                            if ( pressed /*&& !timelineScroll.interactive*/ ){

//                            }
                        }
                        onExited: hoverData.text = ""
                    }


                    Rectangle{
                        anchors.bottom: parent.bottom
                        width : 1
                        height : parent.height / 2
                        color: "#ccc"

                        x: (videoArea.currentFrame + videoArea.sequencePosition) * timeline.zoom
                        onXChanged: {
                            if ( x > mainScroll.flickableItem.contentX + timeline.width - 10 ){
                                var newContentXForwardPos = x - 30
                                if ( newContentXForwardPos > mainScroll.flickableItem.contentWidth - timeline.width )
                                    newContentXForwardPos = mainScroll.flickableItem.contentWidth - timeline.width
                                mainScroll.flickableItem.contentX = newContentXForwardPos
                            } else if ( x < mainScroll.flickableItem.contentX ){
                                var newContentXBackPos = x - timeline.width + 30
                                if ( newContentXBackPos < 0 )
                                    newContentXBackPos = 0
                                mainScroll.flickableItem.contentX = newContentXBackPos
                            }
                        }
                    }

                } // end header

            }

            Rectangle{
                id: timeline

                anchors.top: parent.top
                anchors.topMargin: 35

                property int zoom : 5

                height: parent.height - 35
                width: parent.width - 150
                x: 150

                ScrollView{
                    id: mainScroll
                    anchors.fill: parent

                    style: ScrollViewStyle {
                        transientScrollBars: false
                        handle: Item {
                            implicitWidth: 10
                            implicitHeight: 10
                            Rectangle {
                                color: mainWindow.theme.timelineScrollHandleBackgronud
                                anchors.fill: parent
                            }
                        }
                        scrollBarBackground: Item{
                            implicitWidth: 10
                            implicitHeight: 10
                            Rectangle{
                                anchors.fill: parent
                                color: mainWindow.theme.timelineScrollBackground
                            }
                        }
                        decrementControl: null
                        incrementControl: null
                        frame: Rectangle{color: mainWindow.theme.timelineScrollBackground}
                        corner: Rectangle{color: mainWindow.theme.timelineScrollHandleBackgronud}
                    }

                    ListView{
                        id: timelineView
                        boundsBehavior: Flickable.StopAtBounds

                        height: parent.height
                        contentWidth: document.sequenceList.contentLength * timeline.zoom + 5
                        model: document.trackList
                        delegate: Rectangle{
                            height: 32
                            width: ganttLine.width
                            color: mainWindow.theme.timelineBorderColor

                            TimelineRow{
                                id: ganttLine
                                color: mainWindow.theme.timelineRowBackground
                                height: 31
                                anchors.top: parent.top

                                zoomScale: timeline.zoom

                                elementColor: mainWindow.theme.segmentBackground
                                elementBorderColor: mainWindow.theme.segmentBorder

                                viewportX: timelineView.contentX / ganttLine.zoomScale
                                viewportWidth: timelineView.width / ganttLine.zoomScale
                                model: track
                                modelLength: document.sequenceList.contentLength

                                onEditItem: {
                                    editSegmentWindow.segmentView     = view
                                    editSegmentWindow.segmentDelegate = item
                                    editSegmentWindow.segmentPosition = position
                                    editSegmentWindow.segmentLength   = length
                                    editSegmentWindow.segmentData     = data
                                    editSegmentWindow.visible = true
                                }
                            }

                            Rectangle{
                                width : 1
                                height : parent.height
                                color: "#ccc"
                                x: (videoArea.currentFrame + videoArea.sequencePosition) * timeline.zoom
                            }
                        }


                    }


                }
            }
        }
    }

    ErrorDialogInternal{
        id: errorDialog
        visible: false
        anchors.fill: parent
    }

    MessageDialogInternal{
        id: confirmDialog
        anchors.fill: parent
        visible: false

        onOk: onOkCallback()

        property var onOkCallback : function(){}
    }
}
