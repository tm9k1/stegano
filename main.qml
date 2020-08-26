import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4
import QtQuick.Controls 2.15

import "assets/components/"
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

    maximumHeight: screen.height // 200%
    maximumWidth: mainWindow.maximumHeight * mainWindow.aspectRatio

    visible: true
    title: qsTr("Hello World")

    Component.onCompleted: {
        setX(screen.width/2 - mainWindow.width/2)
        setY(screen.height/2 - mainWindow.height/2)
    }

    color: "#31363b"

    ShaderEffect {
        height: mainWindow.height
        width: mainWindow.width

        fragmentShader: "
    varying highp vec2 qt_TexCoord0;
    void main() {
        gl_FragColor = vec4(0, qt_TexCoord0.x, qt_TexCoord0.y, 1.0);
    }";
    }

    FontLoader { id: sanFransicoPro; source: "assets/fonts/sf-pro-text-regular.otf" }

    //    flags: Qt.FramelessWindowHint TODO: Make an OP border system

    GridLayout {
        id : mainGridLayout
        anchors.margins: 10
        anchors.fill: parent

        focus: true
        columns: mainWindow.isLandscapeMode ? 5 : 1
        rows: mainWindow.isLandscapeMode ? 1 : 5

        ImageUI {
            id: origImageUI
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
            font.pixelSize: origImageUI.titleSize
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

            Button {
                id: retrieveButton
                opacity: 0.8

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

                Layout.preferredWidth: retrieveButton.width
                horizontalAlignment: TextInput.AlignHCenter

                background: Item {
                    Rectangle {
                        id: bitCountBackgroundRectangle
                        anchors.fill: parent
                        color:  "#88000000"
                    }
                }

                validator: IntValidator { bottom: 1 ; top: 3 }

                font.bold: true
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

            Button {
                id: modulateButton
                opacity: 0.8

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

            title: "Result"

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
            value: origImageUI.imageSource
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
                origImageUI.imageSource = imageProc.carrierImageUrl;
            }
        }
    }
}



/*##^##
Designer {
    D{i:0;formeditorZoom:0.75}
}
##^##*/
