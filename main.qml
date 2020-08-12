import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.12

Window {
    id: mainWindow

    property double aspectRatio: (screen.width / screen.height)
    property int startHeight: 720

    height: mainWindow.startHeight
//    width: mainWindow.startHeight * mainWindow.aspectRatio
    width: 1280
    minimumHeight: 480
    minimumWidth: mainWindow.minimumHeight * mainWindow.aspectRatio
    maximumHeight: 1080
    maximumWidth: mainWindow.maximumHeight * mainWindow.aspectRatio

    visible: true
    title: qsTr("Hello World")

    Component.onCompleted: {
        setX(screen.width/2 - mainWindow.width/2)
        setY(screen.height/2 - mainWindow.height/2)
    }

    color: "#31363b"

    RowLayout {
        anchors.margins: 10
        anchors.fill: parent
        focus: true

        Rectangle {
            id: origRectangle
            Layout.fillHeight: true
            Layout.fillWidth: true
            radius: 20

            color: "#1399fd"

            ColumnLayout {
                id: origColumnLayout
                anchors.fill: parent
                anchors.margins: 10
                spacing: 20
                Label {
                    id: origLabel
                    font.bold: true
                    color: "#eff0f1"

                    font.pixelSize: (origRectangle.width) >> 4
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    text: "Original Image"
                }

                Image {
                    Layout.fillWidth: true
                    verticalAlignment: Image.AlignTop
                    fillMode: Image.PreserveAspectFit
                    source: "assets/images/placeholder.png"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillHeight: true

                }

                Button {
                    id: loadButton
                    Layout.fillWidth: true
                    text: "LOAD"
                    Layout.fillHeight: true
                }

                Button {
                    Layout.fillWidth: true
                    text: "SAVE IMAGE"
                    Layout.fillHeight: true
                }


            }
        }

        Label {
            font.bold: true
            font.pixelSize: (origRectangle.width) >> 3
            text: "+"
            color: "#eff0f1"

        }

        Rectangle {
            id: hideRectangle
            Layout.fillHeight: true
            Layout.fillWidth: true
            radius: 30

            color: "#eff0f1"

            ColumnLayout {
                id: hideColumnLayout
                anchors.fill: parent
            }
        }


        Label {
            font.bold: true
            font.pixelSize: (origRectangle.width) >> 3
            text: "="
            color: "#eff0f1"

        }

        Rectangle {
            id: mixRectangle
            Layout.fillHeight: true
            Layout.fillWidth: true
            radius: 30

            color: "#3daee9"
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
