/***************************************************************************
**
** This file is part of libqtlayerman example **
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).*
**
** All rights reserved.
** Contact:  Nokia Corporation (qt-info@nokia.com)
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**  * Redistributions of source code must retain the above copyright notice,
**    this list of conditions and the following disclaimer.
**  * Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and/or other materials provided with ** the distribution.
**  * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor the
**    names of its contributors may be used to endorse or promote products
**    derived from this software without specific ** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**************************************************************************/

import QtQuick 1.0

Rectangle {
    id: rect1
    width: 800
    height: 360

    state:  "center"
    property int count: 1

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
           if(mouse.button==Qt.LeftButton)
           {
            if(rect1.count==1)
                rect1.state = "toright"
            else if(rect1.count==2)
                rect1.state = "toleft"
            else
                rect1.state = "center"

            rect1.count = rect1.count+1
           }
           else
           {
               if(rect1.count==1)
                   rect1.state = "toleft"
               else if(rect1.count==2)
                   rect1.state = "center"
               else
                   rect1.state = "toright"

               rect1.count = rect1.count-1
           }

            if(rect1.count > 3)
                rect1.count=1
            if(rect1.count < 1)
                rect1.count=3

        }
    }

    Flipable {
        id: centerflip
        x: 250
        y: 10
        width: 300
        height: 300

        property int position: 1

        front: Rectangle{x:0; y:0; width: 300; height: 300; color:  "blue"}

        transform: Rotation {
            id: rotation3
            origin.x: centerflip.width/2
            origin.y: centerflip.height/2
            axis.x: 0; axis.y: 1; axis.z: 0;
            angle: 0
        }

        states:  [
            State {
            name:  "left"
            PropertyChanges { target: rotation3; angle: 45 }
            PropertyChanges { target: centerflip; x: 0; }
            },
            State {
            name:  "right"
            PropertyChanges { target: rotation3; angle: -45 }
            PropertyChanges { target: centerflip; x: 500; }
            },
            State {
            name:  "center"
            PropertyChanges { target: rotation3; angle: 0 }
            PropertyChanges { target: centerflip; x: 250; }
            }
        ]

        transitions: Transition {
            NumberAnimation { target: rotation3; property:  "angle"; duration : 4000;}
            NumberAnimation { target: centerflip; property:  "x"; duration : 4000;}
        }
    }
    Flipable {
        id: leftflip
        x: 0
        y: 10
        width: 300
        height: 300

        property int position: 1

        front: Rectangle{ x:0; y:0; width: 300; height: 300; color:  "red"}

        transform: Rotation {
            id: rotation2
            origin.x: leftflip.width/2
            origin.y: leftflip.height/2
            axis.x: 0; axis.y: 1; axis.z: 0;
            angle: 0
        }

        states:  [
            State {
            name:  "left"
            PropertyChanges { target: rotation2; angle: 45 }
            PropertyChanges { target: leftflip; x: 0; }
            },
            State {
            name:  "right"
            PropertyChanges { target: rotation2; angle: -45 }
            PropertyChanges { target: leftflip; x: 500; }
            }
            ,
            State {
            name:  "center"
            PropertyChanges { target: rotation2; angle: 0 }
            PropertyChanges { target: leftflip; x: 250; }
            }
        ]

        transitions: Transition {
            NumberAnimation { target: rotation2; property:  "angle"; duration : 4000;}
            NumberAnimation { target: leftflip; property:  "x"; duration : 4000;}
        }
    }
    Flipable {
        id: rightflip
        x: 500
        y: 10
        width: 300
        height: 300

        property int position: 1

        front: Rectangle{ id: rectangle3; x:0; y:0; width: 300; height: 300; color:  "green"}

        transform: Rotation {
            id: rotation1
            origin.x: rightflip.width/2
            origin.y: rightflip.height/2
            axis.x: 0; axis.y: 1; axis.z: 0;
            angle: 0
        }

        states:  [
            State {
            name:  "left"
            PropertyChanges { target: rotation1; angle: 45 }
            PropertyChanges { target: rightflip; x: 0; }
            },
            State {
            name:  "right"
            PropertyChanges { target: rotation1; angle: -45 }
            PropertyChanges { target: rightflip; x: 500; }
            }
            ,
            State {
            name:  "center"
            PropertyChanges { target: rotation1; angle: 0 }
            PropertyChanges { target: rightflip; x: 250; }
            }
        ]

        transitions: Transition {
            NumberAnimation { target: rotation1; property:  "angle"; duration : 4000;}
            NumberAnimation { target: rightflip; property:  "x"; duration : 4000;}
        }
    }
    states: [
        State {
            name: "toright"
            PropertyChanges { target: centerflip; state: "right" }
            PropertyChanges { target: rightflip; state: "left" }
            PropertyChanges { target: leftflip; state: "center" }
        },
        State {
            name: "toleft"
            PropertyChanges { target: centerflip; state: "left" }
            PropertyChanges { target: rightflip; state: "center" }
            PropertyChanges { target: leftflip; state: "right" }
        },
        State {
            name: "center"
            PropertyChanges { target: centerflip; state: "center" }
            PropertyChanges { target: rightflip; state: "right" }
            PropertyChanges { target: leftflip; state: "left" }
        }
    ]

    Timer {
             interval: 5000; running: false; repeat: false
             onTriggered: {
                     if(rect1.count==1)
                         rect1.state = "toleft"
                     else if(rect1.count==2)
                         rect1.state = "center"
                     else
                         rect1.state = "toright"

                     rect1.count = rect1.count-1

                  if(rect1.count > 3)
                      rect1.count=1
                  if(rect1.count < 1)
                      rect1.count=3
              }
         }

}

