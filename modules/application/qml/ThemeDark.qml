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
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

QtObject{

    // Header
    // ------

    property color menubarBackground: "#171b20"
    property color headerBackground: "#171b20"
    property color footerBackground : "#222328"

    // Sequence
    // --------

    property color sequenceBackground: "#15171c"
    property color sequenceBorder: "#121317"
    property color sequenceItemBackground: "#171b20"
    property color sequenceItemSelectedBackground: "#1e232a"

    property color sequenceAddButton: "#303a55"

    // Add Sequence Window
    // -------------------

    property color addSequenceTextColor : "#f1f1f1"
    property color addSequenceBackgroundColor: "#15161a"
    property color addSequenceReadonlyBoxColor: "#111115"
    property color addSequenceBoxColor: "#222328"
    property color addSequenceBoxHoverColor: "#22232f"
    property color addSequenceAddButtonBackground: "#13284c"
    property color addSequenceAddButtonHoverBackground: "#132853"
    property color addSequenceCancelButtonBackground : "#441719"
    property color addSequenceCancelButtonHoverBackground : "#44191c"

    // Edit Sequence Window
    // --------------------

    property color editSequenceTextColor : "#f1f1f1"
    property color editSequenceBackgroundColor: "#15161a"
    property color editSequenceReadonlyBoxColor: "#111115"
    property color editSequenceBoxColor: "#222328"
    property color editSequenceBoxHoverColor: "#22232f"
    property color editSequenceAddButtonBackground: "#13284c"
    property color editSequenceAddButtonHoverBackground: "#132853"
    property color editSequenceCancelButtonBackground : "#441719"
    property color editSequenceCancelButtonHoverBackground : "#44191c"

    // Sequence - Video
    // ----------------

    property Component videoSequenceSplitHandle : Rectangle {
        implicitWidth: 1
        implicitHeight: 1
        gradient: Gradient {
            GradientStop { position: 0.09; color: "transparent" }
            GradientStop { position: 0.1; color: "#121317" }
            GradientStop { position: 0.9; color: "#121317" }
            GradientStop { position: 0.91; color: "transparent" }
        }
    }

    // Video
    // -----

    property color videoBackground: "#121317"
    property color videoProgressBackground: "#121619"
    property color videoProgressForeground: "#2b3842"

    property color videoControlsBackground: "#141c22"

    // Timeline
    // --------

    property color timelineBorderColor: "#21252a"
    property color timelineRowBackground: "#0c0d0f"

    property color timelineTopHeaderBackgroundColor: "#151a1f"

    property color timelineHeaderBorderColor: "#21252a"
    property color timelineHeaderRowBackground: "#151a1f"

    property color timelineScrollBackground: "#121317"
    property color timelineScrollHandleBackgronud: "#2d3037"

    property Component videoTimelineSplitHandle : Rectangle {
        implicitWidth: 1
        implicitHeight: 1
        color: "#181c23"
    }

    // Segment
    // -------

    property color segmentBackground: "#121a22"
    property color segmentBorder : "#525a62"
    property color segmentEditWindowBackground : "#15161a"
    property color segmentEditWindowInput : "#0c0d0f"

    // Error Dialog Internal
    // ---------------------

    property color errorDialogBackgroundColor : "#15161a"

    // Input Dialog Internal
    // ---------------------

    property color inputDialogBackgroundColor: "#15161a"
    property color inputDialogInputBackgroundColor: "#222328"
    property color inputDialogInputTextColor: "#f1f1f1"

}

