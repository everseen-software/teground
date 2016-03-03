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
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Rectangle {
    id: root

    property variant model : ListModel{}

    property variant theme: ThemeDark{}

    signal indexSwap(int src, int dst)
    signal itemTriggered(string path, string decoderName, int index)
    signal itemEdit(string path, string decoderName, string type, int index)
    signal itemRemove(int index)
    signal itemAdd()

    ScrollView{
        width: parent.width
        height: parent.height
        style: ScrollViewStyle{
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
            frame: Rectangle{color: mainWindow.theme.sequenceBackground}
            corner: Rectangle{color: mainWindow.theme.timelineScrollHandleBackgronud}
        }

        ListView {
            id: listView
            width: parent.width
            height: parent.height
            boundsBehavior: Flickable.StopAtBounds
            property int dragItemIndex: -1

            model: root.model

            delegate: Item {
                id: delegateItem
                width: listView.width
                height: 90

                property bool isSelected : ListView.isCurrentItem

                Rectangle{
                    id: dragRect
                    width: listView.width
                    height: 90
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: delegateItem.isSelected ? root.theme.sequenceItemSelectedBackground : root.theme.sequenceItemBackground

                    Text{
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        width: listView.width - 40
                        text: path
                        font.pixelSize: 13
                        font.family: "Open Sans, Arial, Helvetica, open-sans"
                        color: "#ccc"
                        wrapMode: Text.WordWrap
                    }

                    Text{
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.top: parent.top
                        anchors.topMargin: 60
                        width: listView.width - 40
                        text: decoder
                        font.pixelSize: 9
                        font.family: "Open Sans, Arial, Helvetica, open-sans"
                        color: "#ccc"
                        wrapMode: Text.WordWrap
                    }

                    Text{
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        anchors.top: parent.top
                        anchors.topMargin: 60
                        text: length + " frames"
                        font.pixelSize: 9
                        font.family: "Open Sans, Arial, Helvetica, open-sans"
                        color: "#ccc"
                        wrapMode: Text.WordWrap
                    }

                    MouseArea {
                        id: mouseArea
                        hoverEnabled: true
                        anchors.fill: parent
                        drag.target: dragRect
                        drag.onActiveChanged: {
                            if (mouseArea.drag.active) {
                                listView.dragItemIndex = index;
                            }
                            removeSequenceButton.visible = mouseArea.drag.active
                            dragRect.Drag.drop();
                        }
                        onClicked: {
                            listView.currentIndex = index
                        }
                        onDoubleClicked: {
                            listView.currentIndex = index
                            root.itemTriggered(path, decoder, index)
                        }
                    }

                    Rectangle{
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.bottom: parent.bottom
                        width: 25
                        height: 25
                        color: "#666"
                        visible: mouseArea.containsMouse
                        MouseArea{
                            anchors.fill: parent
                            onClicked: root.itemEdit(path, decoder, type, index)
                        }
                    }

                    Rectangle{
                        height: 1
                        width: listView.width
                        color: root.theme.sequenceBorder
                        anchors.bottom: parent.bottom
                    }

                    states: [
                        State {
                            when: dragRect.Drag.active
                            ParentChange {
                                target: dragRect
                                parent: root
                            }

                            AnchorChanges {
                                target: dragRect
                                anchors.horizontalCenter: undefined
                                anchors.verticalCenter: undefined
                            }
                        }
                    ]

                    Drag.active: mouseArea.drag.active
                    Drag.hotSpot.x: dragRect.width / 2
                    Drag.hotSpot.y: dragRect.height / 2
                }
            }

            Rectangle{
                id: fileHighlight
                width: parent.width
                height: 2
                color: "#000"
                visible: false

                transitions: Transition {
                    NumberAnimation { properties: "y"; duration: 1000 }
                }
            }

            DropArea{
                id: dropArea
                anchors.fill: parent
                onPositionChanged: {
                    var item = listView.itemAt(listView.contentX + drag.x, listView.contentY + drag.y)

                    fileHighlight.visible = true
                    fileHighlight.y = item ? item.y - listView.contentY : drag.y > listView.height ? listView.height : 0
                }
                onDropped:{
                    fileHighlight.visible = false
                    var dragIndex = listView.dragItemIndex
                    var dropIndex = listView.indexAt(listView.contentX + drop.x, listView.contentY + drop.y)
                    if ( dragIndex !== dropIndex )
                        root.indexSwap(dragIndex, dropIndex)
                }
            }

        }
    }


    Rectangle{
        id: addSequenceButton
        anchors.right: parent.right
        width: 34
        height: 34
        color: root.theme.sequenceAddButton
        opacity: addSequenceButtonMouse.containsMouse ? 1.0 : 0.3
        visible: !removeSequenceDropArea.visible
        radius: 20
        MouseArea{
            id: addSequenceButtonMouse
            anchors.fill: parent
            hoverEnabled: true
            onClicked: root.itemAdd()
        }
        Behavior on opacity{
            NumberAnimation{ duration: 100 }
        }
        Text{
            anchors.centerIn: parent
            text: "+"
            font.family: "Courier"
            font.weight: Font.Normal
            font.pixelSize: 30
            color: "#fff"
        }
    }

    Rectangle{
        id: removeSequenceButton
        anchors.left: parent.right
        anchors.top: parent.top
        height: 100
        width: 100
        color: "#770000"
        z: 1000
        visible: false
        opacity: {
            if ( removeSequenceDropArea.containsDrag )
                return 1.0;
            else if (visible)
                return 0.5;
            else
                return 0.0;
        }
        DropArea{
            id: removeSequenceDropArea
            anchors.fill: parent
            onDropped:{ root.itemRemove(listView.dragItemIndex) }
        }
        Behavior on opacity{
            NumberAnimation{ duration: 200 }
        }
    }

}
