import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15

Item {
    id: container
    property alias title: headerLabel.text
    property alias titleSize: headerLabel.font.pixelSize
    property alias imageSource: image.source
    signal loadRequestedFile(var url)

    ColumnLayout {
        id: columnLayout
        property int roundRadius: 5

        anchors.fill: parent
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Image {
                id: image
                anchors.fill: parent
                clip: true
                source: "../images/placeholder"
                fillMode: Image.PreserveAspectCrop
            }

            MouseArea {
                id: loadImageMouseArea
                anchors.fill: parent
                hoverEnabled: true

                Rectangle {
                    id: loadOverLayRectangle
                    anchors.fill: parent
                    color: "#4d4d4d"
                    opacity: 0.8
                }

                RoundButton {
                    id: loadImageButton
                    anchors.centerIn: parent

                    font.underline: true
                    font.bold: true
                    font.pixelSize: headerLabel.font.pixelSize
                    text: "\u21A5"

                    onClicked: loadImageDialog.open();

                    FileDialog {
                        id: loadImageDialog
                        title: "Open an image"
                        folder: shortcuts.desktop
                        nameFilters: [ "Image files (*.jpg *.png *.bmp)", "All files (*)" ]

                        onAccepted: container.loadRequestedFile(loadImageDialog.fileUrl)
                    }

                }

                RoundButton {
                    id: resetImageButton
                    width: loadImageButton.width / 2
                    height: loadImageButton.height / 2
                    anchors.verticalCenter: loadImageButton.verticalCenter
                    anchors.left: loadImageButton.right
                    anchors.leftMargin: resetImageButton.font.pixelSize

                    font.bold: true
                    font.pixelSize: Math.max(loadImageButton.font.pixelSize / 2 , 12)
                    text: "\u2205"

                    onClicked: container.loadRequestedFile(undefined)
                }

            }

            Label {
                id: headerLabel
                anchors.top: parent.top
                anchors.topMargin: headerLabel.font.pixelSize / 4
                anchors.horizontalCenter: parent.horizontalCenter

                font.underline: true
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

                    Layout.bottomMargin: saveButton.height / 2
                    radius: columnLayout.roundRadius

                    font.family: sanFransicoPro.name
                    font.pixelSize: headerLabel.font.pixelSize / 2
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
                        // fun(saveFileDialog.fileUrl);
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
                target: loadOverLayRectangle
                opacity: 0.7
            }

        }
        //        ,
        //        State {
        //            name: "reveal"
        //            // SET when: TO THE POINT WHEN WE HAVE A GOOD RESULT -- SIGNAL COMES FROM C++
        //        }

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
                    duration: 200
                    easing.type: Easing.InOutQuad
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
