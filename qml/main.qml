import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4
import QtQuick.Controls 2.15

import "qml/components/"
import ImageProc.ImageProcUtil 1.0

Window {
    id: mainWindow

    property double aspectRatio: (screen.width / screen.height)
    property int startHeight: (screen.height / 2)
    property bool isLandscapeMode: (mainWindow.width > mainWindow.height)
    // FOR TESTING ONLY
    height: 720
    width: 1280

    //    height: mainWindow.startHeight // 100%
    //    width: mainWindow.startHeight * mainWindow.aspectRatio

    minimumHeight: (mainWindow.startHeight / 2) + (mainWindow.startHeight / 4) // 75%
    minimumWidth: mainWindow.minimumHeight * mainWindow.aspectRatio

    visible: true
    title: qsTr("Stegano")

    Component.onCompleted: {
        setX(screen.width/2 - mainWindow.width/2)
        setY(screen.height/2 - mainWindow.height/2)
    }

    color: "#737c85"

    //    flags: Qt.FramelessWindowHint TODO: Make an OP border system
    ColumnLayout {

        anchors.margins: 10
        anchors.fill: parent

        Label {
            text: "Stegano: A Digital Image Manipulation App"
            font.pixelSize: carrierImageUI.titleSize
            color: "#eff0f1"

        }
        Label {
            text: "A pet project by Piyush Aggarwal"
            font.pixelSize: carrierImageUI.titleSize / 1.4
            color: "#eff0f1"

        }

        GridLayout {
            id : mainGridLayout

            Layout.fillWidth: true
            Layout.fillHeight: true
            focus: true
            columns: mainWindow.isLandscapeMode ? 5 : 1
            rows: mainWindow.isLandscapeMode ? 1 : 5

            ImageUI {
                id: carrierImageUI
                Layout.fillWidth: true
                Layout.fillHeight: true

                title: "Original Image"

                onClickedOnImage: imageProc.openImage(imageProc.carrierImageUrl)

                onSaveFileRequested: {
                    if (imageProc.saveImage(imageProc.carrierImageUrl, destinationUrl)) {
                        console.log("saveImage() success");
                    } else {
                        console.log("saveImage() failed");
                    }
                }
            }

            Label {
                id: plusLabel
                Layout.alignment: Qt.AlignCenter

                font.bold: true
                font.pixelSize: carrierImageUI.titleSize
                color: "#eff0f1"
                text: "+"
            }

            ImageUI {
                id: payloadImageUI
                Layout.fillWidth: true
                Layout.fillHeight: true

                grayscaleMode: true
                title: "Payload Image"

                onClickedOnImage: imageProc.openImage(imageProc.payloadImageUrl)

                onSaveFileRequested: {
                    if (imageProc.saveImage(imageProc.payloadImageUrl, destinationUrl)) {
                        console.log("saveImage() success");
                    } else {
                        console.log("saveImage() failed");
                    }
                }
            }

            GridLayout {
                id: flowGridLayout
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter

                columns: mainWindow.isLandscapeMode ? 1 : 3
                rows: mainWindow.isLandscapeMode ? 3 : 1

                CustomButton {
                    id: retrieveButton
                    Layout.preferredWidth: bitCountTextField.width

                    horizontalPadding: 0
                    font.pixelSize: carrierImageUI.titleSize
                    text: (mainWindow.isLandscapeMode ? "\u21e6" : "\u21e7")

                    onClicked: {
                        var returnCode = imageProc.retrieveImage();
                        if (returnCode === ImageProcUtil.Success) {
                            console.log("retrieveImage returned successfully");
                        } else {
                            console.log("retrieveImage had an error! Error Code: ", returnCode);
                        }
                    }
                }

                TextField {
                    id: bitCountTextField

                    Layout.preferredWidth: bitCountTextField.placeholderText.length * 5
                    horizontalAlignment: TextInput.AlignHCenter

                    background: Item {
                        Rectangle {
                            id: bitCountBackgroundRectangle
                            anchors.fill: parent
                            color:  "#88000000"
                        }
                    }

                    validator: IntValidator { bottom: 1 ; top: 3 }

                    color: "#eff0f1"
                    placeholderText: "<b>Bits (1/2/3)</b>"

                    ToolTip.text: "Number of bits to use for the process (1- poor payload quality, 2, 3- best payload quality)"

                    onFocusChanged: bitCountTextField.ToolTip.visible = (bitCountTextField.focus !== Boolean(Qt.NoFocus))

                    onTextEdited: {
                        if (bitCountTextField.acceptableInput) {
                            bitCountTextField.state = "acceptable"
                        } else {
                            bitCountTextField.state = ""
                        }
                    }

                    states: [
                        State {
                            name: "acceptable"
                            PropertyChanges { target: bitCountBackgroundRectangle; color: "#8800cc00" }
                            PropertyChanges { target: bitCountTextField; ToolTip.visible: false }
                        }
                    ]
                }

                CustomButton {
                    id: modulateButton
                    Layout.preferredWidth: bitCountTextField.width

                    horizontalPadding: 0
                    font.pixelSize: carrierImageUI.titleSize
                    text: (mainWindow.isLandscapeMode ? "\u21e8" : "\u21e9")

                    onClicked: {
                        var returnCode = imageProc.hideImage();
                        if (returnCode === ImageProcUtil.Success) {
                            console.log("hideImage returned successfully");
                        } else {
                            console.log("hideImage had an error! Error Code: ", returnCode);
                        }
                    }
                }
            }

            ImageUI {
                id: modulatedImageUI
                Layout.fillWidth: true
                Layout.fillHeight: true

                title: "Modulated Image"

                onClickedOnImage: imageProc.openImage(imageProc.modulatedImageUrl)

                onSaveFileRequested: {
                    if (imageProc.saveImage(imageProc.modulatedImageUrl, destinationUrl)) {
                        console.log("saveImage() success");
                    } else {
                        console.log("saveImage() failed");
                    }
                }
            }

            Keys.onPressed: {
                if (event.key === Qt.Key_Escape) {
                    mainWindow.close();
                    event.accepted = true;
                }
            }

            /* updation for QML -> C++
         */
            Binding {
                target: imageProc
                property: "carrierImageUrl"
                value: carrierImageUI.imageSource
            }

            Binding {
                target: imageProc
                property: "payloadImageUrl"
                value: payloadImageUI.imageSource
            }

            Binding {
                target: imageProc
                property: "modulatedImageUrl"
                value: modulatedImageUI.imageSource
            }

            Binding {
                target: imageProc
                property: "bitCount"
                value: (bitCountTextField.acceptableInput) ? parseInt(bitCountTextField.text) : -1
            }

            /* updation for C++ -> QML
         */
            Connections {
                target: imageProc
                function onModulatedImageUrlChanged() {
                    modulatedImageUI.imageSource = imageProc.modulatedImageUrl;
                }
            }

            Connections {
                target: imageProc
                function onPayloadImageUrlChanged() {
                    payloadImageUI.imageSource = imageProc.payloadImageUrl;
                }
            }

            Connections {
                target: imageProc
                function onCarrierImageUrlChanged() {
                    carrierImageUI.imageSource = imageProc.carrierImageUrl;
                }
            }
        }
    }

    CustomButton {
        id: introCustomButton
        anchors.centerIn: parent
        dontAnimate: true
        backgroundOpacity: 0.9
        text:
"
This app has two operating modes:-
1. Encoding mode
The encoding mode takes a carrier image and payload image as inputs and generates a modulated image.
    1. Provide a carrier image and a payload image.
    2. Enter the number of bits to be used from each RGB component to store the payload.
    3. Once that is done press the forward button and the modulated image should show up.

2. Decoding mode
The decoding mode takes an image as input and returns the sanitized carrier and the payload as separate images.
    1. Provide the modulated image.
    2. Enter the number of bits that were used for modulation.
    3. Press the backward button and the two images should show up.

Click on this tip to dismiss it!
"

        onClicked: introCustomButton.visible = false
    }
}



