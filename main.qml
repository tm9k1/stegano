import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: mainWindow
    width: 1280
    height: 720
    visible: true
    title: qsTr("Hello World")

Item {
    focus:true
    Keys.onPressed: {
        if (event.key == Qt.Key_Escape) {
            mainWindow.close();
            event.accepted = true;
        }
    }
}

}
