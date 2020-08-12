import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: mainWindow
    width: 1280
    height: 720
    visible: true
    title: qsTr("Hello World")
    Component.onCompleted: {
        setX(screen.width/2 - mainWindow.width/2)
        setY(screen.height/2 - mainWindow.height/2)
    }

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
