import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.12

Window {
    id: mainWindow
    minimumHeight: 480
    minimumWidth: 640
    width: 1280
    height: 720
    maximumHeight: 1080
    maximumWidth: 1920
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
            radius: 30

            color: "#1399fd"

            ColumnLayout {
                id: origColumnLayout
                anchors.fill: parent
            }
        }

        Label {
            font.bold: true
            font.pixelSize: 50
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
            font.pixelSize: 50
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
