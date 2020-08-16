import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15

Item {
    id: container
    property alias title: headerLabel.text
    property alias titleSize: headerLabel.font.pixelSize

    ColumnLayout {
        id: columnLayout
        property int roundRadius: 5

        anchors.fill: parent

        Image {
            id: image
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip:  true
            source: "../images/placeholder.png"
            fillMode: Image.PreserveAspectCrop

            MouseArea {
                id: loadImageMouseArea
                anchors.fill: parent
                hoverEnabled: true

                Rectangle {
                    id: loadOverLayRectangle
                    property var baseColor: "#4d4d4d"
                    anchors.fill: parent
                    color: loadOverLayRectangle.baseColor
                }

                RoundButton {
                    id: loadButton
                    anchors.centerIn: parent

                    opacity: 0.8
                    visible: false

                    font.underline: true
                    font.bold: true
                    font.pixelSize: headerLabel.font.pixelSize
                    text: "\u21A5"

                    onClicked: {
                        openFileDialog.open();
                    }
                }

                FileDialog {
                    id: openFileDialog
                    title: "Open an image"
                    folder: shortcuts.desktop
                    nameFilters: [ "Image files (*.jpg *.png *.bmp)", "All files (*)" ]

                    onAccepted: {
                        if (openFileDialog.fileUrl) {
                            loadOverLayRectangle.baseColor = "#55000000";
                            image.source = openFileDialog.fileUrl
                        }
                    }
                }
            }

            Label {
                id: headerLabel
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter

                font.family: sanFransicoPro.name
                color: "#eff0f1"
                font.pixelSize: 18
                style: Text.Raised
                styleColor: "#4d4d4d"
                text: "Some Title"
            }

            RowLayout {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter

                RoundButton {
                    id: saveButton

                    Layout.bottomMargin: saveButton.height >> 1
                    radius: columnLayout.roundRadius

                    font.family: sanFransicoPro.name
                    font.pixelSize: headerLabel.font.pixelSize >> 1
                    text: "Save Image"

                    onClicked: saveFileDialog.open()
                }

                FileDialog {
                    id: saveFileDialog
                    title: "Save the tmage"
                    folder: shortcuts.desktop
                    nameFilters: [ "Image files (*.jpg *.png *.bmp)", "All files (*)" ]
                    selectExisting: false
                    onAccepted: {
                        //                        fun(saveFileDialog.fileUrl);
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "hovered"
            when: loadImageMouseArea.containsMouse

            PropertyChanges {
                target: loadButton
                visible: true
            }

            PropertyChanges {
                target: loadOverLayRectangle
                opacity: 0.5
            }
        },
        State {
            name: "reveal"
            // SET when: TO THE POINT WHEN WE HAVE A GOOD RESULT -- SIGNAL COMES FROM C++
        }

    ]

    transitions: [
        Transition {
            from: ""
            to: "hovered"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    target: loadOverLayRectangle
                    properties: "opacity"
                    easing.type: Easing.InOutQuad
                    duration: 200
                }
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:1.3300000429153442;height:480;width:640}
}
##^##*/
