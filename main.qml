import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick 2.1

Window {
    id: mainWindow

    property double aspectRatio: (screen.width / screen.height)
    property int startHeight: (screen.height >> 1)

    height: 720
    width: 1280
    //    height: mainWindow.startHeight // 100%
    //    width: mainWindow.startHeight * mainWindow.aspectRatio
    //   aspectRatio minimumHeight: 480
    minimumHeight: (mainWindow.startHeight >> 1) + (mainWindow.startHeight >> 2) // 75%
    minimumWidth: mainWindow.minimumHeight * mainWindow.aspectRatio
    //    maximumHeight: 1080
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
        gl_FragColor = vec4(200.0, qt_TexCoord0.x, qt_TexCoord0.y, 1.0);
    }";
    }

    FontLoader { id: productSans; source: "assets/fonts/sf-pro-text-regular.otf" }

    //    flags: Qt.FramelessWindowHint TODO: Make an OP border system

    GridLayout {
        anchors.margins: 10
        anchors.fill: parent
        focus: true
        columns: (mainWindow.width / mainWindow.height >=1) ? 5 : 1
        rows: (mainWindow.width / mainWindow.height >=1) ? 1 : 5

        Rectangle {
            id: origRectangle
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "#eff0f1"
            radius: 5
            property double aspectRatio: origRectangle.width / origRectangle.height
            ColumnLayout {
                id: origColumnLayout
                anchors.fill: parent
                anchors.margins: 10

                Image {
                    id: origImage
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    verticalAlignment: Image.AlignTop
                    fillMode: Image.PreserveAspectCrop
                    source: "assets/images/placeholder.png"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    MouseArea {
                        id: origImageMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onHoveredChanged: loadOverLayRectangle.visible = origImageMouseArea.containsMouse


                        Rectangle {
                            id: loadOverLayRectangle
                            anchors.fill: parent
                            opacity: 0.5
                            visible: false
                        }

                        RoundButton {
                            id: loadButton
                            anchors.centerIn: parent
                            font.bold: true
                            font.pixelSize: (origRectangle.width) >> 3
                            radius: 20
                            opacity: 0.8
                            visible: loadOverLayRectangle.visible
                            text: "\u21A5"
                            onClicked: openFileDialog.open()
                        }

                    }

                    Label {
                        id: origLabel
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        font.bold: true
                        font.family: productSans.name
                        style: Text.Raised
                        styleColor: "#4d4d4d"
                        color: "#eff0f1"

                        font.pixelSize: Math.min(origColumnLayout.height * 0.546, origColumnLayout.width) / origLabel.text.length
                        horizontalAlignment: Text.AlignHCenter
                        text: "Original Image"
                    }

                    RowLayout {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: origColumnLayout.width >> 2
                        anchors.bottomMargin: anchors.margins >> 2

                        RoundButton {
                            id: saveButton
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            radius: 5

                            font.family: productSans.name
                            font.pixelSize: Math.max(saveButton.width / saveButton.text.length, 12)
                            text: "Save Image"

                            onClicked: {
                                saveFileDialog.open();
                            }
                        }

                    }

                    FileDialog {
                        id: openFileDialog
                        title: "Open an image"
                        folder: shortcuts.desktop
                        nameFilters: [ "Image files (*.jpg *.png *.bmp)", "All files (*)" ]

                        onAccepted: {
                            image.source = openFileDialog.fileUrl
                        }


                    }
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

        Label {
            Layout.alignment: Qt.AlignCenter
            font.bold: true
            font.pixelSize: (origRectangle.width) >> 3
            text: "+"
            color: "#eff0f1"
        }

        Rectangle {
            id: hideRectangle
            Layout.fillHeight: true
            Layout.fillWidth: true
            radius: 10

            color: "#eff0f1"

            ColumnLayout {
                id: hideColumnLayout
                anchors.fill: parent
            }
        }


        Label {
            Layout.alignment: Qt.AlignCenter
            font.bold: true
            font.pixelSize: Math.min(origRectangle.height * origRectangle.aspectRatio, origRectangle.width) >> 3
            text: "="
            color: "#eff0f1"

        }

        Rectangle {
            id: mixRectangle
            Layout.fillHeight: true
            Layout.fillWidth: true
            radius: 10

            color: "#eff0f1"
            ColumnLayout {
                id: mixColumnLayout
                anchors.fill: parent
            }
        }


        Keys.onPressed: {
            if (event.key == Qt.Key_Escape) {
                mainWindow.close();
                event.accepted = true;
            }
        }



    }


}



/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/
