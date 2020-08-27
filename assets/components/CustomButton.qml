import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: container
    signal clicked();
    property alias text: buttonLabel.text
    property alias font: buttonLabel.font
    property alias horizontalPadding: buttonRectangle.horizontalPadding

    implicitWidth: buttonRectangle.width
    implicitHeight: buttonRectangle.height

    Rectangle {
        id: buttonRectangle
        property int horizontalPadding: 10
        property int verticalPadding: 5
        anchors.fill: parent

        radius: 10
        width: horizontalPadding + buttonLabel.width + horizontalPadding
        height: verticalPadding + buttonLabel.height + verticalPadding

        border.color: "#000000"
        border.width: 1
        color: "#4d4d4d"
        opacity: (container.enabled) ? 0.5 : 0.1

        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            hoverEnabled: true

            onClicked: container.clicked()
        }

        states: [
            State {
                name: "hovered"
                when: buttonMouseArea.containsMouse

                PropertyChanges {
                    target: buttonRectangle
                    opacity: 0.3
                }
            }
        ]

        transitions: [
            Transition {
                from: ""
                to: "hovered"
                reversible: true

                ParallelAnimation {
                    NumberAnimation {
                        target: buttonRectangle
                        property: "opacity"
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        ]
    }

    Label {
        id: buttonLabel
        anchors.centerIn: parent

        color: "#eff0f1"
//        font.bold: true
        style: Text.Raised
        styleColor: "#4d4d4d"

        text: "Label"
    }
}

