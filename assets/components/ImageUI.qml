import QtQuick 2.0
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Item {
    id: container
    property alias title: headerLabel.text
    property alias imageSource: image.source
    property alias titleSize: headerLabel.font.pixelSize
    property alias grayscaleMode: colorizeImage.visible
    signal clickedOnImage()
    signal saveFileRequested(url destinationUrl)

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Image {
                id: placeholderImage
                anchors.fill: parent
                clip: true
                source: "../images/placeholder"
                fillMode: Image.PreserveAspectCrop

                opacity: 1.0
            }

            Image {
                id: image
                anchors.fill: parent
                clip: true
                source: ""
                fillMode: Image.PreserveAspectCrop

                opacity: 0.0

                Colorize {
                    id: colorizeImage
                    anchors.fill: image
                    source: image
                    saturation: 0
                    visible: false
                }
            }

            MouseArea {
                id: loadImageMouseArea

                anchors.top: headerRectangle.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: saveAreaRectangle.top

                hoverEnabled: true
                onHoveredChanged: loadOverLayRectangle.state = (loadImageMouseArea.containsMouse) ? "hovered" : ""
                onClicked: container.clickedOnImage();

                Rectangle {
                    id: loadOverLayRectangle
                    anchors.fill: parent
                    color: "#4d4d4d"
                    opacity: 0.4

                    states: [
                        State {
                            name: "hovered"
                            PropertyChanges {
                                target: loadOverLayRectangle
                                opacity: 0.7
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
                                    target: loadOverLayRectangle
                                    properties: "opacity"
                                    duration: 200
                                    easing.type: Easing.InOutQuad
                                }
                            }
                        }
                    ]
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

                        onAccepted: image.source = loadImageDialog.fileUrl
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

                    enabled: false
                    onClicked: container.state = ""
                }

            }

            Rectangle {
                id: headerRectangle
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: headerLabel.font.pixelSize * 2
                color: loadOverLayRectangle.color
                opacity: 0.6
            }

            Label {
                id: headerLabel
                anchors.centerIn: headerRectangle

                font.underline: true
                font.family: sanFransicoPro.name
                color: "#eff0f1"
                font.pixelSize: 20
                style: Text.Raised
                styleColor: "#4d4d4d"

                text: "Some Title"
            }

            Rectangle {
                id: saveAreaRectangle
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: headerLabel.font.pixelSize * 4

                color: loadOverLayRectangle.color
                opacity: 0.6

                MouseArea {
                    id: saveAreaMouseArea
                    anchors.fill: parent
                    enabled: false

                    onHoveredChanged: {
                        if (saveAreaMouseArea.containsMouse) {
                            saveAreaRectangle.opacity = 0.7;
                        } else {
                            saveAreaRectangle.opacity = 0.6;
                        }
                    }
                }
            }

            RoundButton {
                id: saveButton
                anchors.centerIn: saveAreaRectangle
                font.family: sanFransicoPro.name
                font.pixelSize: headerLabel.font.pixelSize / 2
                text: "Save Image"
                enabled: false
                onClicked: saveFileDialog.open()


                FileDialog {
                    id: saveFileDialog
                    title: "Save the tmage"
                    folder: shortcuts.desktop
                    nameFilters: [ "Image files (*.jpg *.png *.bmp)", "All files (*)" ]
                    selectExisting: false
                    onAccepted: container.saveFileRequested(saveFileDialog.fileUrl)
                }
            }
        }
    }

    states: [
        State {
            name: "loaded"
            when: image.source != ""

            PropertyChanges { target: saveButton; enabled: true }
            PropertyChanges { target: resetImageButton; enabled: true }
            PropertyChanges { target: image; opacity: 1 }
            PropertyChanges { target: placeholderImage; opacity: 0 }
            PropertyChanges { target: saveAreaMouseArea; enabled: true }
            PropertyChanges { target: saveAreaMouseArea; hoverEnabled: true }
        }
    ]

    transitions: [
        Transition { from: ""; to: "loaded"; reversible: true
            SequentialAnimation {
                // keep this above so it triggers as the last animation in reverse
                // This is a workaround. It'd be nice to have a cleaner way to
                // reset image.source after the cross-fade animation is complete
                //
                ScriptAction {
                    script: {
                        if(container.state == "") {
                            image.source = "";
                        }
                    }
                }

                ParallelAnimation {
                    NumberAnimation {
                        target: placeholderImage
                        property: "opacity"
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }

                    NumberAnimation {
                        target: image
                        property: "opacity"
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
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
