import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11

Window {
    visible: true
    width: 600
    height: 300
    title: qsTr("Fake device client")

    RowLayout {
        id: mainLayout
        objectName: "mainLayout"
        spacing: 30
    }
}
