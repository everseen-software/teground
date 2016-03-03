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

import QtQuick 2.0
import Teground 1.0

Rectangle {
    id : root

    property int videoLength : 50000
    property int zoomScale : 5

    property color elementColor: "#505050"
    property color elementBorderColor: "#555"

    property variant model : ListModel{}

    width : timelineView.width
    height : 28
    clip : true

    MouseArea{
        anchors.fill: parent
        onDoubleClicked: {
            parent.model.appendSequence(Math.round(mouse.x / parent.zoomScale));
        }
    }

    Repeater {
        id: timelineView
        anchors.left: parent.left
        anchors.leftMargin: 0

        width: root.videoLength * root.zoomScale
        height: root.height

        model : root.model

        delegate: Rectangle{

            id: timelineDelegate

            width: sequence.length * root.zoomScale
            height: timelineView.height

            clip: true
            color: root.elementColor

            x: sequence.position * root.zoomScale

            Keys.onDeletePressed: {
                event.accepted = true
                root.model.removeSequence(sequence)
            }

            Rectangle{
                anchors.left: parent.left
                anchors.top: parent.top
                width : 2
                height : parent.height
                color: timelineDelegate.activeFocus ? "#666" : root.elementBorderColor

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.SizeHorCursor

                    property int oldMouseX

                    onPressed: oldMouseX = mouseX
                    onPositionChanged: {
                        if ( pressed ){
                            var newWidth  = timelineDelegate.width + oldMouseX - mouseX
                            var newX      = timelineDelegate.x + mouseX - oldMouseX
                            var newLength = Math.round(newWidth / root.zoomScale)
                            if ( newLength > 0 && newX + newWidth < timelineView.width ){
                                timelineDelegate.width = newWidth
                                timelineDelegate.x     = newX
                            } else {
                                timelineDelegate.x     =
                                        timelineDelegate.x + timelineDelegate.width - (1 * root.zoomScale)
                                timelineDelegate.width = 1 * root.zoomScale
                            }
                        }
                    }

                    onReleased: {
                        sequence.length        = Math.round(timelineDelegate.width / root.zoomScale)
                        sequence.position      = Math.round(timelineDelegate.x / root.zoomScale)
                        timelineDelegate.width = sequence.length * root.zoomScale
                        timelineDelegate.x     = sequence.position * root.zoomScale
                    }
                }
            }
            Rectangle{
                anchors.left: parent.left
                anchors.leftMargin: 2
                anchors.right: parent.right
                anchors.rightMargin: 2
                width : parent.width
                height : parent.height
                color : parent.color
                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.SizeAllCursor
                    drag.target: timelineDelegate
                    drag.axis: Drag.XAxis
                    drag.minimumX: 0
                    drag.maximumX: timelineView.width - timelineDelegate.width
                    onClicked: {
                        timelineDelegate.focus = true
                    }
                    onReleased :{
                        sequence.position = Math.round(timelineDelegate.x / root.zoomScale)
                        timelineDelegate.x = sequence.position * root.zoomScale
                    }
                }
            }
            Rectangle{
                anchors.right: parent.right
                anchors.top: parent.top
                width : 2
                height : parent.height
                color: timelineDelegate.activeFocus ? "#666" : root.elementBorderColor

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.SizeHorCursor

                    property int oldMouseX

                    onPressed: oldMouseX = mouseX
                    onPositionChanged: {
                        if ( pressed ){
                            var newWidth  = timelineDelegate.width + (mouseX - oldMouseX)
                            var newLength = Math.round(newWidth / root.zoomScale)
                            if ( newLength > 0){
                                timelineDelegate.width = newWidth
                            } else
                                timelineDelegate.width = 1 * root.zoomScale
                        }
                    }
                    onReleased: {
                        sequence.length        = Math.round(timelineDelegate.width / root.zoomScale)
                        timelineDelegate.width = sequence.length * root.zoomScale
                    }
                }
            }

        }
    }
}
