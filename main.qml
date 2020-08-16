import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4
import "./assets/components/"

Window {
    id: mainWindow

    property double aspectRatio: (screen.width / screen.height)
    property int startHeight: (screen.height >> 1)

    // FOR TESTING ONLY
    height: 720
    width: 1280

    //    height: mainWindow.startHeight // 100%
    //    width: mainWindow.startHeight * mainWindow.aspectRatio

    minimumHeight: (mainWindow.startHeight >> 1) + (mainWindow.startHeight >> 2) // 75%
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
        columns: (mainWindow.width / mainWindow.height >=1) ? 5 : 1
        rows: (mainWindow.width / mainWindow.height >=1) ? 1 : 5

        property int maxTitleLength: Math.max(origImageUI.title.length, payloadImageUI.title.length, resultImageUI.title.length)

        ImageUI {
            id: origImageUI
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "Original Image"
            titleSize: Math.min(origImageUI.width, origImageUI.height) / mainGridLayout.maxTitleLength
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
            title: "Payload Image"
            titleSize: origImageUI.titleSize
        }

        Label {
            id: equalLabel
            Layout.alignment: Qt.AlignCenter

            font.bold: true
            font.pixelSize: origImageUI.titleSize
            color: "#eff0f1"
            text: "="
        }

        ImageUI {
            id: resultImageUI
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "Result"
            titleSize: origImageUI.titleSize
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
